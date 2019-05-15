#include "analyseur.h"
#include "table.hpp"
#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "ligne_paye.hpp"
#include "tags.h"

using namespace std;

extern bool verbeux;

inline int GCC_INLINE Analyseur::memoire_p_ligne(const info_t& info, const unsigned agent)
{
    /* Attention on peut "rembobiner" les types dans la limite de TYPE_LOOP_LIMIT, ce qui implique que l'on doit allouer ex ante TYPE_LOOP_LIMIT fois nbType
     *  pour les drapeux de séparation des catégories */

    return BESOIN_MEMOIRE_ENTETE  // chaque agent a au moins BESOIN_MEMOIRE_ENTETE champs du bulletins de paye en colonnes
            // sans la table ces champs sont répétés à chaque ligne de paye.
            + nbType  *  TYPE_LOOP_LIMIT                   // espace pour les drapeaux de séparation des champs (taille de type_remuneration).
            + (info.NLigne[agent]) * (INDEX_MAX_COLONNNES + 1);   // nombre de lignes de paye x nombre maximum de types de balises distincts de lignes de paye
    // soit INDEX_MAX_COLONNES = N pour les écritures du type Var(l+i), i=0,...,N dans ECRIRE_LIGNE_l_COMMUN
}


inline void GCC_INLINE Analyseur::allouer_memoire_table(info_t& info)
{
    // utiliser NCumAgent ici et pas NCumAgentXml, puisqu'il s'agit d'une préallocation

    info.Memoire_p_ligne.resize(info.NCumAgent);

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
    {
        info.Memoire_p_ligne[agent] = memoire_p_ligne(info, agent);
    }


    if (info.Table.size() > 0 && info.Table.size() == info.NCumAgent)
    {
        for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
        {
            for (int i = 0; i < info.Memoire_p_ligne[agent] ; ++i)
            {
                xmlFree(info.Table[agent][i]);
            }

        }
    }

    info.NAgent.resize(info.threads->argc);
    info.Table.resize(info.NCumAgent);
    info.NCumAgentXml = 0;

    if (info.NAgent.empty())
    {
        erreur("Mémoire insuffisante pour la table du nombre d'agents");
    }

    if (info.Table.empty())
    {
        cerr << msg_erreur("Nombre total d'agents (info.NCumAgent) : ", info.NCumAgent);

        erreur("Mémoire insuffisante pour la table de lignes de paye");
    }

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
    {

        info.Table[agent].resize(info.Memoire_p_ligne[agent]); // ne pas oublier d'initialiser à nullptr !

        if (verbeux && info.Table[agent].empty())
        {
            erreur(string("Erreur d'allocation de drapeau I. pour l'agent ")
                   +  to_string(agent)
                   + string("et pour ")
                   +  to_string(info.Memoire_p_ligne[agent])
                   +  " B");

        }
    }
}


Analyseur::Analyseur(Commandline& c) : nb_fil {c.get_nb_fil()},
                                       nb_segment {c.nb_segment()}
{
    if (verbeux)
    {
        if (nb_segment > 1)
            cerr << PROCESSING_HTML_TAG << "Les bases en sortie seront analysées en " << nb_segment << " segments." ENDL;
        else
            cerr << PROCESSING_HTML_TAG << "Les bases en sortie seront analysées en une seule itération."  ENDL;
    }


    lanceur(c);
}

Analyseur::~Analyseur()
{
  cerr << PROCESSING_HTML_TAG "Fin de l'analyse" << ENDL;
}

void Analyseur::lanceur(Commandline& commande)
{
    for(int rang_segment = 0; rang_segment < nb_segment; ++rang_segment)
    {
        int nb_fichier = commande.get_nb_fichier(rang_segment);  // nombre d'éléments de quad par segment

        if (commande.get_nb_fil() > nb_fichier)
        {
            cerr << msg_erreur("Segment n°", rang_segment, " : trop de fils (", commande.info.nbfil,
                               ") pour le nombre de fichiers (", nb_fichier, ")");

            cerr << msg_erreur("Exécution avec ", nb_fichier, pluriel(nb_fichier, " fil"), ".");

            commande.set_nbfil(nb_fichier);
        }
        else
            commande.set_nbfil(commande.info.nbfil);

        /* Le nombre de fils du segment doit impérativement être ajusté avant de lancer la production */

        produire_segment(commande, rang_segment);
    }
}


int Analyseur::produire_segment(Commandline& commande, int rang_segment)
#ifdef CATCH
 try
#endif
{
    /* Lancement des fils */

      thread_handler th{ commande, rang_segment };
      gestionnaire_fils = &th;

    if (commande.is_pretend()) return 2;

    /* Gestion des informations de fin d'exécution */

    if (commande.is_calculer_maxima())
    {
        calculer_maxima(gestionnaire_fils->Info);
    }

    if (! commande.get_chemin_log().empty())
    {
        ofstream LOG;
        LOG.open(commande.get_chemin_log(), ios::app);
        calculer_maxima(gestionnaire_fils->Info, &LOG);
        LOG.close();
    }

    /* Résumé des erreurs rencontrées */

    if (errorLineStack.size() > 0)
    {
        cerr << WARNING_HTML_TAG "<b>Récapitulatif des erreurs rencontrées</b>" << ENDL;
        for (const errorLine_t& e :  errorLineStack)
        {
            cerr << e.filePath;
            if (e.lineN != -1)
                cerr << " -- Ligne n°" << e.lineN << ENDL;
            else
                cerr << " -- Ligne inconnue." << ENDL;

        }
    }

    if (! commande.get_chemin_log().empty())
    {
        ofstream LOG;
        LOG.open(commande.get_chemin_log(), ios::app);
        if (LOG.good())
            for (const errorLine_t& e :  errorLineStack)
            {
                if (e.lineN != -1)
                    LOG << " -- Ligne n°" << e.lineN << ENDL;
                else
                    LOG << " -- Ligne inconnue." << ENDL;
            }
    }

    /* Génération des tables CSV */

    if (commande.is_generer_table())
    {
        cerr << ENDL << PROCESSING_HTML_TAG "Exportation des bases de données au format CSV..." << ENDL ENDL;
        boucle_ecriture(gestionnaire_fils->Info, rang_segment);
    }

    /* Signal de fin de barre de progression */

    generate_rank_signal(-1);
    cerr << " \n";

    /* Libération de la mémoire
     * En cas de problème d'allocation mémoire le mieux est encore de ne pas désallouer car on ne connait pas exacteemnt l'état
     * de la mémoire dynamique */

    return 1;
}
#ifdef CATCH
    catch(...)
    {
        erreur("Erreur dans le lancement des fils");
    }
#endif





void* Analyseur::decoder_fichier(info_t& info)
{
    /* environ 6000 bulletins par seconde en processus sumple, et 15000 en multithread ; rajoute 1/3 du temps */

    if (info.pretend) return nullptr;

#if  defined GCC_REGEX && !defined NO_REGEX

    regex pat {EXPRESSION_REG_ELUS,  regex_constants::icase};
    regex pat2 {EXPRESSION_REG_VACATIONS, regex_constants::icase};
    regex pat3 {EXPRESSION_REG_ASSISTANTES_MATERNELLES, regex_constants::icase};
    regex pat_adjoints {EXPRESSION_REG_ADJOINTS, regex_constants::icase};
    regex pat_agents {EXPRESSION_REG_AGENTS, regex_constants::icase};
    regex pat_cat_a {EXPRESSION_REG_CAT_A, regex_constants::icase};
    regex pat_cat_b {EXPRESSION_REG_CAT_B, regex_constants::icase};

#endif

    xmlKeepBlanksDefault(0);

    if (info.reduire_consommation_memoire)
    {
        int err = calculer_memoire_requise(info);
        if (err)
        {
            erreur("Calcul de la mémoire requise");
        }
    }
    else
    {
        info.NCumAgent = info.nbAgentUtilisateur * info.threads->argc;
        info.NLigne.resize(info.NCumAgent);

        memory_debug("decoder_fichier : info.NLigne.resize(info.NCumAgent)");

        if (! info.NLigne.empty())
        {
            for (unsigned i = 0 ; i < info.NCumAgent; ++i)
                info.NLigne[i] = info.nbLigneUtilisateur;
        }
        else
        {
            erreur("Problème d'allocation mémoire de info.NLigne");
        }
    }

    info.fichier_courant = 0;

    for (auto &&q :  info.threads->argv)
    {
        if (info.fichier_courant == 0)
        {
            /* première allocation ou réallocation à la suite d'un incident */

            allouer_memoire_table(info);
            memory_debug("decoder_fichier : allouer_memoire_table(info)");
        }

        memory_debug("decoder_fichier : pre_parseFile(info)");

        if (info.verifmem) return nullptr;

        switch(Analyseur::parseFile(info))
        {
            case RETRY:
                info.fichier_courant = 0;
                /* on réalloue tout depuis le début à la site d'un incident */
                cerr << msg_erreur(" Il est nécessaire de réallouer la mémoire à la suite d'un incident dû aux données...");
                continue;

            case SKIP_FILE:
                cerr << msg_erreur(" Le fichier  ",  q.value, " n'a pas pu être traité. Fichier suivant...");
                continue;

            default :
    #ifndef NO_DEBUG
                cerr << PROCESSING_HTML_TAG "Fichier " << info.threads->argv[info.fichier_courant]  << "  traité" ENDL;
    #endif
                break;
        }

        memory_debug("decoder_fichier : post_parseFile(info)");

        ++info.fichier_courant;
    }

    if (info.reduire_consommation_memoire && info.NCumAgentXml != info.NCumAgent)
    {
        if (verbeux)
        {
            cerr << ERROR_HTML_TAG "Incohérence de l'allocation mémoire ex-ante " << info.NCumAgent
                 << " unités et ex-post " <<  info.NCumAgentXml << " unités d'information." ENDL
                 << "Sortie pour éviter une erreur de segmentation." ENDL;
            cerr << " Fichier : " << info.threads->argv[info.fichier_courant].value << ENDL;
        }
        else
            erreur("Erreur critique lors de l'analyse du fichier : "
                   + info.threads->argv[info.fichier_courant].value);

    }

    // attention, pas info<-NCumAgent ici

    /* Le champ statut est modifié comme suit :
        ELU   pour un élu
        et le champ grade :
        V     pour un vacataire
        A     pour une assistante maternelle */

#if !defined NO_REGEX
#define VAR(X) info.Table[agent][X]
    for (unsigned agent = 0; agent < info.NCumAgentXml; ++agent)
    {
        /* Les élus peuvent être identifiés soit dans le service soit dans l'emploi métier */


        if (regex_match((const char*) VAR(EmploiMetier), pat) || regex_match((const char*) VAR(Service), pat))
        {
            xmlFree(VAR(Statut)) ;
            VAR(Statut) = (xmlChar*) xmlStrdup((const xmlChar*)"ELU");
            VAR(Categorie) = (xmlChar*) NA_STRING;
        }
        else
        {
            /* vacataires */

            if (regex_match((const char*) VAR(EmploiMetier), pat2))
            {
                xmlFree(VAR(Grade));
                VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"V");
            }

            /* assistantes maternelles */

            else if (regex_match((const char*) VAR(EmploiMetier), pat3))
            {
                xmlFree(VAR(Grade));
                VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"A");
            }

            /* identification des catégories A, B, C */

            /* gestion de mémoire : ne pas allouer avec xmlStrdup et ne pas libérer
                 * à la fin de main.cpp dans la double boucle de libération de mémoire car
                 * A, B, C, NA ne sont pas alloués sur le tas. */

            if (regex_match((const char*) VAR(Grade), pat_adjoints)
                    || regex_match((const char*) VAR(Grade), pat_agents))
            {
                VAR(Categorie) = (xmlChar*)"C";
            }
            else if (regex_match((const char*) VAR(Grade), pat_cat_a))
            {
                VAR(Categorie) = (xmlChar*)"A";
            }

            /* Il faut teste d'abord cat A et seulement ensuite cat B */

            else if (regex_match((const char*) VAR(Grade), pat_cat_b))
            {
                VAR(Categorie) = (xmlChar*)"B";
            }
            else
            {
                VAR(Categorie) = (xmlChar*) NA_STRING;
            }
        }

        /* les vacations peuvent être indiquées comme telles dans les libellés de paie mais pas dans les emplois métiers.
           On les récupère en parcourant les libellés */

        if (info.reduire_consommation_memoire)
        {
            /* inutile de boucler sur la partie vide du tableau... */

            for (int j = BESOIN_MEMOIRE_ENTETE; j < info.NLigne[agent]; ++j)
                if (regex_match((const char*) VAR(j), pat2))
                {
                    xmlFree(VAR(Grade));
                    VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"V");
                }
        }
        else
        {
            for (int j = BESOIN_MEMOIRE_ENTETE ; j < info.NLigne[agent] && info.Table[agent][j] != nullptr; ++j)
                if (regex_match((const char*) VAR(j), pat2))
                {
                    xmlFree(VAR(Grade));
                    VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"V");
                }
        }
    }


#undef VAR
#endif
    return nullptr;
}

extern mutex mut;
extern int rang_global;

/* agent_total est une variable de contrôle pour info->NCumAgent */

// Problème des accumulations de champs <DonneesIndiv> non résolu !
// -1 erreur, 0 : fichier petit; sinon nombre de fichiers découpés.

int Analyseur::parseFile(const xmlDocPtr doc, info_t& info, int cont_flag, xml_commun* champ_commun)
{
    ofstream log;
    xmlNodePtr cur = nullptr;
    xmlNodePtr cur_save = cur;
    xmlChar *annee_fichier = champ_commun->annee_fichier,
            *mois_fichier = champ_commun->mois_fichier,
            *employeur_fichier = champ_commun->employeur_fichier,
            *etablissement_fichier = champ_commun->etablissement_fichier,
            *siret_fichier = champ_commun->siret_fichier,
            *budget_fichier = champ_commun->budget_fichier;

    info.NAgent[info.fichier_courant] = 0;

    memory_debug("parseFile : parseFile doc");

    if (doc == nullptr)
    {
        cerr << ERROR_HTML_TAG " problème d'allocation mémoire pour le scan XML." ENDL;
        return SKIP_FILE;
    }

    if (! info.chemin_log.empty())
    {
        log.open(info.chemin_log, ios::app);
    }

    cur = xmlDocGetRootElement(doc);

    memory_debug("parseFile : parseFile get root");

    if (cur == nullptr)
    {
        cerr << ERROR_HTML_TAG "document vide" ENDL;
        xmlFreeDoc(doc);
        if (verbeux)
            cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tolérant)." ENDL;
        if (log.is_open())
            log.close();
        return SKIP_FILE;
    }

    cur =  cur->xmlChildrenNode;

    if (cont_flag != PREMIER_FICHIER) goto donnees_indiv;

    cur = atteindreNoeud("Annee", cur);

    if (cur != nullptr)
    {
        annee_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        int annee;
        annee = (annee_fichier[0] == '\0')? 0 : atoi((const char*) annee_fichier);

        /* Altaïr est écrit pour durer 100 ans :) */

        if (annee <= 2000 || annee >= 2100)
        {
            cerr << ERROR_HTML_TAG " pas de données sur l'année." ENDL;

#ifdef STRICT
            if (log.open()) log.close();
            exit(-517);
#endif
            /* Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger */
            if (verbeux)
                cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tolérant)." ENDL;
            return SKIP_FILE;
        }

        cur = (cur)? cur->next : nullptr;
    }
    else
    {
        cerr << msg_erreur("Année non détectable");
        if (log.is_open())
            log.close();
#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-502);
#else
        /* Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger */
        if (verbeux)
            cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tolérant)." ENDL;

        return SKIP_FILE;
#endif
    }

    if (cur != nullptr)
    {
        mois_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        int mois = atoi((const char*) mois_fichier);
        if (mois <= 0 || mois > 12)
        {
            cerr << ERROR_HTML_TAG " pas de données sur le mois." ENDL;

#ifdef STRICT
            if (log.open()) log.close();
            exit(-517);
#endif
            /* Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger */
            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tolérant)." ENDL;
            return SKIP_FILE;
        }

    }
    else
    {
        cerr << ERROR_HTML_TAG "Mois non détectable" ENDL;

#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-503);
#else
        /* Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger */
        if (verbeux)
            cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tolérant)." ENDL;
        return SKIP_FILE;
#endif

    }

    cur_save = cur;
    cur = atteindreNoeud("Budget", cur);

    if (cur != nullptr)
    {
        cur_save = cur;
        cur =  cur->xmlChildrenNode;
        budget_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        if (budget_fichier[0] == '\0')
        {
            xmlFree(budget_fichier);
            budget_fichier = xmlStrdup(NA_STRING);
            lock_guard<mutex> lock(mut);
            if (verbeux)
                cerr << STATE_HTML_TAG "Aucune information sur le budget [optionnel]." ENDL;
            if (verbeux)
                cerr << PROCESSING_HTML_TAG "Poursuite du traitement." ENDL;
        }

        cur = cur_save->next;
    }
    else
    {
        budget_fichier = xmlStrdup(NA_STRING);
        lock_guard<mutex> lock(mut);
        if (verbeux)
            cerr << STATE_HTML_TAG "Aucune information sur le budget [optionnel]." ENDL;
        if (verbeux)
            cerr << PROCESSING_HTML_TAG "Poursuite du traitement." ENDL;
        cur = cur_save;
    }

    cur_save = cur;

    /* REFERENCE */

    /* <Employeur>
    *      <Nom V="">{1,1}</Nom>
    *      <APE V="">{1,1}</APE>
    *      <Adresse>{1,1}</Adresse>
    *      <NumUrssaf V="">{1,1}</NumUrssaf>
    *      <Siret V="">{1,1}</Siret>
    * </Employeur>
    */

    if (nullptr == (cur = atteindreNoeud("Employeur", cur)))
    {

        warning_msg("la balise Employeur [non-conformité à la norme].", info, cur);

#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-515);
#endif
        if (verbeux)
            cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

        employeur_fichier = xmlStrdup(NA_STRING);
        siret_fichier = xmlStrdup(NA_STRING);
        cur = cur_save;
    }
    else
    {
        cur_save = cur;
        /* On recherche le nom, le siret de l'employeur */
        do {
            cur =  cur->xmlChildrenNode;

            if (cur == nullptr || xmlIsBlankNode(cur))
            {
                cerr << ERROR_HTML_TAG "Pas de données sur le nom de l'employeur [non-conformité à la norme]." ENDL;
                if (mut.try_lock())
                {
                    errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
                    mut.unlock();
                }
                employeur_fichier = xmlStrdup(NA_STRING);
                siret_fichier = xmlStrdup(NA_STRING);
                break;
            }

            cur = atteindreNoeud("Nom", cur);
            if (cur != nullptr)
            {
                employeur_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                cur = (cur)? cur->next : nullptr;
            }
            else
            {
                cerr << ERROR_HTML_TAG "Employeur non identifié [non-conformité à la norme]." ENDL;
                employeur_fichier = xmlStrdup(NA_STRING);
            }

            if (cur != nullptr) cur = atteindreNoeud("Siret", cur);

            if (cur != nullptr)
            {
                siret_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                if (siret_fichier[0] == '\0')
                {
                    warning_msg("la balise Siret", info, cur);
#ifdef STRICT
                    if (log.open()) log.close();
                    exit(-517);
#endif
                    if (verbeux)
                        cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                    xmlFree(siret_fichier);
                    siret_fichier = xmlStrdup(NA_STRING);

                }
            }
            else
            {
                cerr  << ERROR_HTML_TAG "Siret de l'empoyeur non identifié [non-conformité à la norme]." ENDL;
                if (verbeux)
                    cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                cerr << "Année " << annee_fichier
                     << " Mois " << mois_fichier << ENDL;
                siret_fichier = xmlStrdup(NA_STRING);
            }

        } while (false);

        /* on remonte d'un niveau */

        cur = cur_save;
    }

    /* On effectue un saut direct par étiquetage ici dans le cas de fichiers découpés dont les informations
     * d'entête (année, mois...) sont définits par les pointeurs statiques déclarés en début de fonction */

donnees_indiv:

    cur = atteindreNoeud("DonneesIndiv", cur);

    if (cur == nullptr || xmlIsBlankNode(cur))
    {
        cerr << ERROR_HTML_TAG "Pas de données individuelles de paye [DonneesIndiv, non conformité à la norme]." ENDL;
        warning_msg("la balise DonneesIndiv", info, cur);

#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-525);
#else
        /* Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger */
        return SKIP_FILE;
#endif
    }

    bool siret_etablissement =  false;

    while(cur != nullptr)
    {
        cur_save = cur;
        xmlNodePtr cur_save2 = nullptr;
        cur =  cur->xmlChildrenNode;  // Niveau Etablissement et PayeIndivMensuel
        cur_save2 = cur;

        if (cur == nullptr || xmlIsBlankNode(cur))
        {
            cerr << ERROR_HTML_TAG "Pas de données individuelles de paye [non conformité à la norme]." ENDL;
            if (mut.try_lock())
            {
                errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
                mut.unlock();
            }
#ifdef STRICT
            if (log.is_open())
                log.close();
            exit(-515);
#else
            /* Il faudra sans doute ajuster les différences entre le parsing C et l'analyse Xml, qui vont diverger */
            return SKIP_FILE;
#endif
        }

        /* Les données sur l'établissement sont optionnelles */

        /* REFERENCE
         *
         * <DonneesIndiv>
         *   <Etablissement>{0,1}</Etablissement>
         *   <PayeIndivMensuel>{1,unbounded}</PayeIndivMensuel>
         * </DonneesIndiv>
         *
         */

        cur = atteindreNoeud("Etablissement", cur);
        if (cur == nullptr)
        {
            cur = cur_save2;
            if (verbeux) cerr << STATE_HTML_TAG "Pas d'information sur l'Etablissement" ENDL;
            etablissement_fichier = xmlStrdup(NA_STRING);
        }
        else
        {

            /* REFERENCE */
            /*
                 * <Etablissement>
                 *   <Nom V="">{1,1}</Nom>
                 *   <NumUrssaf V="">{1,1}</NumUrssaf>
                 *   <Adresse>{1,1}</Adresse>
                 *   <Siret V="">{1,1}</Siret>
                 * </Etablissement>
                */

            /* Altaïr a ici deux modes : strict, le non-repect de la norme entraine une exception
                 * ou souple : on continue l'exécution en considérant que bien que le nom/siret soit obligatoire,
                 * l'ensemble du champ lui-même est en fait optionnel. [la norme est quelque peu inconsistante sur ce point]
                 */

            cur_save2 = cur;

            /* On recherche le nom, le siret de l'établissement */
            do {
                cur =  cur->xmlChildrenNode;

                if (cur == nullptr || xmlIsBlankNode(cur))
                {
                    warning_msg("les données nominales de l'établissement [non-conformité]", info, cur);

#ifdef STRICT
                    if (log.open()) log.close();
                    exit(-517);
#endif
                    if (verbeux)
                        cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                    etablissement_fichier = xmlStrdup(NA_STRING);
                    /* on garde le siret de l'employeur */
                    break;
                }

                cur = atteindreNoeud("Nom", cur);

                if (cur != nullptr)
                {
                    etablissement_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                    if (etablissement_fichier[0] == '\0')
                    {
                        warning_msg("les données nominales de l'établissement [non-conformité]", info, cur);
#ifdef STRICT
                        if (log.open()) log.close();
                        exit(-517);
#endif
                        if (verbeux)
                            cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                        xmlFree(etablissement_fichier);
                        etablissement_fichier = xmlStrdup(NA_STRING);
                    }

                    cur = (cur)? cur->next : nullptr;

                }
                else
                {
                    warning_msg("les données nominales de l'établissement [non-conformité]", info, cur);
#ifdef STRICT
                    if (log.open()) log.close();
                    exit(-517);
#endif
                    if (verbeux)
                        cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                    etablissement_fichier = xmlStrdup(NA_STRING);

                    /* on garde le siret de l'employeur */
                }

                if (cur != nullptr)
                    cur = atteindreNoeud("Siret", cur);

                siret_etablissement =  true;
                if (cur != nullptr)
                {
                    xmlFree(siret_fichier);
                    siret_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                    if (siret_fichier[0] == '\0')
                    {
                        warning_msg("les données de Siret de l'établissement [non-conformité]", info, cur);
#ifdef STRICT
                        if (log.open()) log.close();
                        exit(-517);
#endif
                        if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;
                        xmlFree(siret_fichier);
                        siret_fichier = xmlStrdup(NA_STRING);
                    }
                }
                else
                {
#ifdef STRICT
                    if (log.open()) log.close();
                    exit(-517);
#endif
                    warning_msg("les données de Siret de l'établissement [non-conformité]", info, cur);
                    siret_fichier = xmlStrdup(NA_STRING);
                }

            } while (false);

            /* on remonte d'un niveau */

            cur = cur_save2;
        }

        /* REFERENCE */

        /* <PayeIndivMensuel>
              <Agent>{1,1}</Agent>
              <Evenement>{0,unbounded}</Evenement>
              <Service V="">{1,1}</Service>
              <NBI V="">{1,unbounded}</NBI>
              <QuotiteTrav V="">{1,1}</QuotiteTrav>
              <Periode>{1,1}</Periode>
              <Remuneration>{1,1}</Remuneration>
              <NbHeureTotal V="">{0,1}</NbHeureTotal>
              <TauxHor V="">{0,1}</TauxHor>
              <NbHeureSup V="">{1,1}</NbHeureSup>
              <MtBrut V="">{1,1}</MtBrut>
              <MtNet V="">{1,1}</MtNet>
              <MtNetAPayer V="">{1,1}</MtNetAPayer>
              <DatePaiement V="">{1,1}</DatePaiement>
              <MtImposable V="">{1,1}</MtImposable>
              <CumulMtImposable V="">{1,1}</CumulMtImposable>
              <CumulMtBrut V="">{1,1}</CumulMtBrut>
              <CumulBaseSS V="">{1,1}</CumulBaseSS>
              <RepartitionBudget>{0,unbounded}</RepartitionBudget>
              <PJRef>{0,unbounded}</PJRef>
           </PayeIndivMensuel>
       */

        while(cur != nullptr)
        {

            cur_save2 =  cur;
            cur = atteindreNoeud("PayeIndivMensuel", cur);

            if (cur == nullptr || cur->xmlChildrenNode == nullptr || xmlIsBlankNode(cur->xmlChildrenNode))
            {
                cerr << ERROR_HTML_TAG "Pas d'information sur les lignes de paye [non-conformité à la norme : PayeIndivMensuel]." ENDL;

                if (cur == nullptr)
                    warning_msg("la balise PayeIndivMensuel de l'établissement [non-conformité]", info, cur);
#ifdef STRICT
                if (log.open()) log.close();
                exit(-518);
#endif
                if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;
                /* Ici on ne risque pas d'avoir une divergence entre le parsage C et Xml */
                if (cur == nullptr)
                    cur = cur_save2->next;
                else
                    cur = cur->next;

                continue;
            }

            /* ici on sait que cur->xmlChildrenNode est non vide */
            cur_save2 = cur;
            cur = cur->xmlChildrenNode;  // Niveau Agent

            /* remarque atoi retourne zéro s'il rencontre "" ou des caractères non numériques */

            info.Table[info.NCumAgentXml][Annee] = xmlStrdup(annee_fichier);
            info.Table[info.NCumAgentXml][Mois]  = xmlStrdup(mois_fichier);
            info.Table[info.NCumAgentXml][Budget] = xmlStrdup(budget_fichier);
            info.Table[info.NCumAgentXml][Employeur]  = xmlStrdup(employeur_fichier);

            /* Nota : le Siret est, si l'établissement existe, celui de l'établissement, sinon celui de l'Employeur */

            info.Table[info.NCumAgentXml][Siret]  = xmlStrdup(siret_fichier);
            info.Table[info.NCumAgentXml][Etablissement]  = xmlStrdup(etablissement_fichier);

            /* LECTURE DES LIGNES DE PAYE STRICTO SENSU */

            int32_t ligne_p = parseLignesPaye(cur, info, log);

            /*  */

            info.drapeau_cont = true;

            int diff = info.NLigne[info.NCumAgentXml] - ligne_p;

            if (info.reduire_consommation_memoire)
            {
                if (diff != 0)
                {
                    if (verbeux)
                        cerr << ERROR_HTML_TAG "Incohérence des décomptes de lignes entre le contrôle C : "
                             << info.NLigne[info.NCumAgentXml]
                             << "et l'analyse Libxml2 : "
                             << ligne_p
                             << ENDL "Pour l'agent "
                             << "de rang  " << info.NCumAgentXml << " dans le fichier" ENDL
                             << info.Table[info.NCumAgentXml][Matricule]
                                << " Année "
                                << info.Table[info.NCumAgentXml][Annee]
                                   << " Mois "
                                   << info.Table[info.NCumAgentXml][Mois]
                                      << ENDL   ;

                    ecrire_log(info, log, diff);
                    if (log.is_open())
                        log.close();
#ifdef STRICT
                    exit(-1278);
#else
                    if (ligne_p > info.NLigne[info.NCumAgentXml])
                        info.NLigne[info.NCumAgentXml] = ligne_p;

                    /* il faut tout réallouer */
                    exit(0);
                    return RETRY;
#endif
                }
            }
            else
                info.NLigne[info.NCumAgentXml] = ligne_p;

            info.nbLigne += ligne_p;

            ecrire_log(info, log, diff);

            // Ici il est normal que cur = nullptr

            cur = cur_save2->next;

            AFFICHER_NOEUD(cur->name)

                    ++info.NAgent[info.fichier_courant];
            ++info.NCumAgentXml;
        }

        if (cont_flag == DERNIER_FICHIER_DECOUPE)
        {
            xmlFree(etablissement_fichier);
        }
        /* si pas d'établissement (NA_STRING) alors on utilise le siret de l'employeur, donc
         * ne pas libérer dans ce cas ! */

        cur = cur_save->next;  // next DonneesIndiv
        if (cur == nullptr || xmlStrcmp(cur->name, (const xmlChar*) "DonneesIndiv")) break;   // on ne va pas envoyer un message d'absence de DonneesIndiv si on a fini la boucle...
    }

#ifdef GENERATE_RANK_SIGNAL
    generate_rank_signal();
#endif

    {

        if (verbeux)
        {
            lock_guard<mutex> guard(mut);
            cerr << STATE_HTML_TAG "Fichier "
        #ifdef GENERATE_RANK_SIGNAL
                 << "n°" <<  rang_global
        #endif
                 << " : " << info.threads->argv[info.fichier_courant].value << ENDL;

            cerr << STATE_HTML_TAG << "Fil n°" << info.threads->thread_num + 1 << " : " << "Fichier courant : " << info.fichier_courant + 1 << ENDL;
            cerr << STATE_HTML_TAG << "Total : " <<  info.NCumAgentXml << " bulletins -- " << info.nbLigne <<" lignes cumulées." ENDL;
        }
    }

    if (cont_flag == PREMIER_FICHIER)
    {
        champ_commun->annee_fichier =  annee_fichier;
        champ_commun->mois_fichier  =  mois_fichier;
        champ_commun->employeur_fichier = employeur_fichier;
    }

    champ_commun->siret_fichier = siret_fichier;
    champ_commun->budget_fichier = budget_fichier;
    champ_commun->etablissement_fichier = etablissement_fichier;

    if (cont_flag == DERNIER_FICHIER_DECOUPE)
    {
        xmlFree(annee_fichier);
        xmlFree(mois_fichier);
        xmlFree(employeur_fichier);
        if (siret_etablissement)   //?
            xmlFree(siret_fichier);
        xmlFree(budget_fichier);
    }

    xmlFreeDoc(doc);

    if (log.is_open())
        log.close();

    return 0;
}

int Analyseur::parseFile(info_t& info)
{
    /* REFERENCE */
    /*
        <DocumentPaye xmlns="http://www.minefi.gouv.fr/cp/helios/pes_v2/paye_1_1">
          <IdVer V="">{1,1}</IdVer>
          <Annee V="">{1,1}</Annee>
          <Mois V="">{1,1}</Mois>
          <Train V="">{0,1}</Train>
          <Budget>{0,1}</Budget>
          <Employeur>{1,1}</Employeur>
          <DonneesIndiv>{1,unbounded}</DonneesIndiv>
          <Nomenclatures>{1,1}</Nomenclatures>
          <RepartitionParNature>{1,1}</RepartitionParNature>
        </DocumentPaye>
    */

    /* Note sur les modes d'exécution :
     * Le mode == strict == envoie une exception dès la première non-conformité,
     * sauf exceptions contraires signalées en commentaire.
     * Le mode == tolérant == arrête l'exécution du fichier et passe suivant.
     * Cela peut poser des problèmes de (ré)allocation/réservation/libération de mémoire
     * Le mode == souple == continue l'exécution le plus possible dans le même fichier.
     * Le mode strict fait l'objet d'une compilation séparée. Les modes tolérant ou souple
     * choisis au cas par cas en fonction d'une évaluation plus ou moins subjective de la gravité
     * de la non-conformité. */

    int res = 0;
    int cont_flag = PREMIER_FICHIER;
    xml_commun champ_commun;
    quad<> &q = info.threads->argv.at(info.fichier_courant);
    int rang_segment = info.threads->rang_segment;

    q.elements = info.threads->in_memory_file[q.value][rang_segment].size(); // correction par rapport à la prévision
    info.threads->argv.at(info.fichier_courant).elements = q.elements;

    try {
         if (q.elements == 0)
         throw runtime_error { string(q.value + string(" ne fait pas partie du segment ") + to_string(rang_segment + 1)).c_str()};
    }
    catch(...)
    {
      cerr << ERROR_HTML_TAG "Fichier hors segment" << endl;
      cerr << string(q.value + string(" ne fait pas partie du segment ") + to_string(rang_segment + 1)) << endl;
      for (auto && h : info.threads->in_memory_file) if (h.first == q.value) cerr << "rang du segment (0-based) : " << rang_segment << " nb élém. " << q.elements << endl;
      return 0;
    }

    if (q.elements == 1)
    {
#if defined(STRINGSTREAM_PARSING)

        xmlDocPtr doc;
        char* s;
        try
        {
           s = (char*) move(info.threads->in_memory_file[q.value][rang_segment][0]).c_str();
            doc = xmlParseDoc(reinterpret_cast<const xmlChar*>(s));
        }
        catch(...)
        {
            cerr << endl << "Sortie d'erreur du parseur XML" << endl << s << endl;
        }

        info.threads->in_memory_file[q.value][rang_segment][0].clear();

#else
        xmlDocPtr doc = xmlParseFile(info.threads->argv.at(info.fichier_courant).value.c_str());
#endif
    return Analyseur::parseFile(doc, info, cont_flag, &champ_commun);
    }

    // -----  nb_decoupe != 0

    int element = 0;

    for (string ss : info.threads->in_memory_file[q.value][rang_segment])  // no const!
    {

       ++element;

        if (verbeux)
        {
            cerr << ENDL;
            cerr << ".....     .....     ....." ENDL;
            cerr << ENDL;

            cerr << PROCESSING_HTML_TAG "Analyse du fichier scindé fil " << info.threads->thread_num + 1
                 << " - n°" << info.fichier_courant + 1 << "-" << element << "/" << q.elements;

            cerr << ENDL;
        }


        if (element == 1)
        {
            if (q.status == LEFTOVER)
                cont_flag = FICHIER_SUIVANT_DECOUPE;
            else
                cont_flag = PREMIER_FICHIER;
        }
        else
        if (element == q.elements)
                cont_flag = DERNIER_FICHIER_DECOUPE;
        else
            cont_flag = FICHIER_SUIVANT_DECOUPE;


#if defined(STRINGSTREAM_PARSING)

        xmlDocPtr doc;
        char* s ;
        try
        {
                s = (char*) move(ss).c_str();
                if (s[0] == '\0')
                {
                    throw runtime_error {"Fichier XML Vide !"};

                }

                doc = xmlParseDoc(reinterpret_cast<const xmlChar*>(s));
                ss.clear();
        }
        catch(...)
        {
            cerr <<  ERROR_HTML_TAG "Erreur XML" << endl
                 << s << endl;
        }

        //s.clear();
#else
        xmlDocPtr doc = xmlParseFile(s.c_str());
#endif

        if (verbeux) cerr << ENDL;

        if (doc == nullptr)
        {
            cerr << msg_erreur("L'analyse du parseur XML n'a pas pu être réalisée.");
        }
        else
            res = Analyseur::parseFile(doc, info, cont_flag, &champ_commun);

        if (verbeux)
        {
            cerr << PROCESSING_HTML_TAG "Fin de l'analyse du fichier scindé fil " << info.threads->thread_num + 1
                 << " n°" << info.fichier_courant + 1 << "-" << element << "/" << q.elements ;
#ifdef FGETC_PARSING
            cerr << " : " << s << ENDL;
#else
            cerr << ENDL;
#endif
        }

#ifdef FGETC_PARSING
        if (res == 0 && ! info.preserve_tempfiles)
        {
            remove(s.c_str());
            if (verbeux)
                cerr << PROCESSING_HTML_TAG "Effacement du fichier scindé fil " << info.threads->thread_num + 1
                     << " n°" << info.fichier_courant + 1<< "-" << element  << "/" << nb_decoupe << " de " << NDecoupe << " : " << s << ENDL;
        }
#endif
        if (res == SKIP_FILE || res == RETRY) return res;
    }

    return 0;
}

