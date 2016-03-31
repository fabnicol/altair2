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
     *  pour les drapeux de s�paration des cat�gories */

    return BESOIN_MEMOIRE_ENTETE  // chaque agent a au moins BESOIN_MEMOIRE_ENTETE champs du bulletins de paye en colonnes
            // sans la table ces champs sont r�p�t�s � chaque ligne de paye.
            + nbType  *  TYPE_LOOP_LIMIT                   // espace pour les drapeaux de s�paration des champs (taille de type_remuneration).
            + (info.NLigne[agent]) * (INDEX_MAX_COLONNNES + 1);   // nombre de lignes de paye x nombre maximum de types de balises distincts de lignes de paye
    // soit INDEX_MAX_COLONNES = N pour les �critures du type Var(l+i), i=0,...,N dans ECRIRE_LIGNE_l_COMMUN
}


inline void GCC_INLINE Analyseur::allouer_memoire_table(info_t& info)
{
    // utiliser NCumAgent ici et pas NCumAgentXml, puisqu'il s'agit d'une pr�allocation

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
        erreur("M�moire insuffisante pour la table du nombre d'agents");
    }

    if (info.Table.empty())
    {
       erreur("M�moire insuffisante pour la table de lignes de paye");
    }

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
    {

        info.Table[agent].resize(info.Memoire_p_ligne[agent]); // ne pas oublier d'initialiser � nullptr !

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
                                       nb_segment {c.nb_segment()},
                                       is_liberer_memoire {c.is_liberer_memoire()}
{
    if (verbeux)
    {
        if (nb_segment > 1)
            cerr << PROCESSING_HTML_TAG << "Les bases en sortie seront analys�es en " << nb_segment << " segments." ENDL;
        else
            cerr << PROCESSING_HTML_TAG << "Les bases en sortie seront analys�es en une seule it�ration."  ENDL;
    }


    lanceur(c);
}

Analyseur::~Analyseur()
{
    if (! is_liberer_memoire)
    {
        if (verbeux)
            cerr << ENDL
                 << PROCESSING_HTML_TAG "Lib�ration de la m�moire..."
                 << ENDL;

            for (int i = 0; i < nb_fil; ++i)
            {
                for (unsigned agent = 0; agent < gestionnaire_fils->Info.at(i).NCumAgent; ++agent)
                {
                    for (int j = 0; j < gestionnaire_fils->Info.at(i).Memoire_p_ligne[agent]; ++j)
                    {
                        if (j != Categorie && xmlStrcmp(gestionnaire_fils->Info.at(i).Table[agent][j], (const xmlChar*) "") != 0)
                            xmlFree(gestionnaire_fils->Info[i].Table[agent][j]);
                    }
                }
            }
    }


}

void Analyseur::lanceur(Commandline& commande)
{
    for(int rang_segment = 0; rang_segment < nb_segment; ++rang_segment)
    {
        unsigned nb_fichier = commande.get_nb_fichier(rang_segment);

        if (commande.info.nbfil > nb_fichier)
        {
            cerr << ERROR_HTML_TAG "Segment n�" << rang_segment << " : trop de fils (" << commande.info.nbfil << ") pour le nombre de fichiers (" << nb_fichier << ")" ENDL;
            cerr << ERROR_HTML_TAG "Ex�cution avec " << nb_fichier << pluriel(nb_fichier, " fil") <<"."  ENDL;

            commande.set_nbfil(nb_fichier);
        }
        else
            commande.set_nbfil(commande.info.nbfil);

        /* Le nombre de fils du segment doit imp�rativement �tre ajust� avant de lancer la production */

        produire_segment(commande, rang_segment);
    }
}


int Analyseur::produire_segment(Commandline& commande, int rang_segment)
{
    /* Lancement des fils */

    try
    {
      *gestionnaire_fils = thread_handler { commande, rang_segment };
    }
    catch(...)
    {
        erreur("Erreur dans le lancement des fils");
    }


    if (commande.is_pretend()) return 2;

    /* Gestion des informations de fin d'ex�cution */

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

    /* R�sum� des erreurs rencontr�es */

    if (errorLineStack.size() > 0)
    {
        cerr << WARNING_HTML_TAG "<b>R�capitulatif des erreurs rencontr�es</b>" << ENDL;
        for (const errorLine_t& e :  errorLineStack)
        {
            cerr << e.filePath;
            if (e.lineN != -1)
                cerr << " -- Ligne n�" << e.lineN << ENDL;
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
                    LOG << " -- Ligne n�" << e.lineN << ENDL;
                else
                    LOG << " -- Ligne inconnue." << ENDL;
            }
    }

    /* G�n�ration des tables CSV */

    if (commande.is_generer_table())
    {
        cerr << ENDL << PROCESSING_HTML_TAG "Exportation des bases de donn�es au format CSV..." << ENDL ENDL;
        boucle_ecriture(gestionnaire_fils->Info, rang_segment);
    }

    /* Signal de fin de barre de progression */

    generate_rank_signal(-1);
    cerr << " \n";

    /* Lib�ration de la m�moire
     * En cas de probl�me d'allocation m�moire le mieux est encore de ne pas d�sallouer car on ne connait pas exacteemnt l'�tat
     * de la m�moire dynamique */

    return 1;
}




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
            erreur("Calcul de la m�moire requise");
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
            erreur("Probl�me d'allocation m�moire de info.NLigne");
        }
    }

    for (unsigned i = 0; i < info.threads->argc ; ++i)
    {
        if (i == 0)
        {
            /* premi�re allocation ou r�allocation � la suite d'un incident */

            allouer_memoire_table(info);
            memory_debug("decoder_fichier : allouer_memoire_table(info)");
        }

        memory_debug("decoder_fichier : pre_parseFile(info)");

        if (info.verifmem) return nullptr;

        info.fichier_courant = i;

        switch(Analyseur::parseFile(info))
        {
        case RETRY:
            i = 0;
            /* on r�alloue tout depuis le d�but � la site d'un incident */
            cerr << ERROR_HTML_TAG " Il est n�cessaire de r�allouer la m�moire � la suite d'un incident d� aux donn�es..." ENDL;
            continue;

        case SKIP_FILE:
            cerr << ERROR_HTML_TAG " Le fichier  " << info.threads->argv[info.fichier_courant].first << " n'a pas pu �tre trait�" ENDL
                 << "   Fichier suivant..." ENDL;
            continue;

        default :
#ifndef NO_DEBUG
            cerr << PROCESSING_HTML_TAG "Fichier " << info.threads->argv[info.fichier_courant]  << "  trait�" ENDL;
#endif
            break;
        }

        memory_debug("decoder_fichier : post_parseFile(info)");
    }

    if (info.reduire_consommation_memoire && info.NCumAgentXml != info.NCumAgent)
    {
        if (verbeux)
        {
            cerr << ERROR_HTML_TAG "Incoh�rence de l'allocation m�moire ex-ante " << info.NCumAgent
                 << " unit�s et ex-post " <<  info.NCumAgentXml << " unit�s d'information." ENDL
                 << "Sortie pour �viter une erreur de segmentation." ENDL;
            cerr << " Fichier : " << info.threads->argv[info.fichier_courant].first << ENDL;
        }
        else
            erreur("Erreur critique lors de l'analyse du fichier : "
                   + info.threads->argv[info.fichier_courant].first);

    }

    // attention, pas info<-NCumAgent ici

    /* Le champ statut est modifi� comme suit :
        ELU   pour un �lu
        et le champ grade :
        V     pour un vacataire
        A     pour une assistante maternelle */

#if !defined NO_REGEX
#define VAR(X) info.Table[agent][X]
    for (unsigned agent = 0; agent < info.NCumAgentXml; ++agent)
    {
        /* Les �lus peuvent �tre identifi�s soit dans le service soit dans l'emploi m�tier */


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

            /* identification des cat�gories A, B, C */

            /* gestion de m�moire : ne pas allouer avec xmlStrdup et ne pas lib�rer
                 * � la fin de main.cpp dans la double boucle de lib�ration de m�moire car
                 * A, B, C, NA ne sont pas allou�s sur le tas. */

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

        /* les vacations peuvent �tre indiqu�es comme telles dans les libell�s de paie mais pas dans les emplois m�tiers.
           On les r�cup�re en parcourant les libell�s */

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

/* agent_total est une variable de contr�le pour info->NCumAgent */

// Probl�me des accumulations de champs <DonneesIndiv> non r�solu !
// -1 erreur, 0 : fichier petit; sinon nombre de fichiers d�coup�s.

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
        cerr << ERROR_HTML_TAG " probl�me d'allocation m�moire pour le scan XML." ENDL;
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
            cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tol�rant)." ENDL;
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

        /* Alta�r est �crit pour durer 100 ans :) */

        if (annee <= 2000 || annee >= 2100)
        {
            cerr << ERROR_HTML_TAG " pas de donn�es sur l'ann�e." ENDL;

#ifdef STRICT
            if (log.open()) log.close();
            exit(-517);
#endif
            /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
            if (verbeux)
                cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tol�rant)." ENDL;
            return SKIP_FILE;
        }

        cur = (cur)? cur->next : nullptr;
    }
    else
    {
        cerr << ERROR_HTML_TAG "Ann�e non d�tectable" ENDL;
        if (log.is_open())
            log.close();
#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-502);
#else
        /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
        if (verbeux)
            cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tol�rant)." ENDL;

        return SKIP_FILE;
#endif
    }

    if (cur != nullptr)
    {
        mois_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        int mois = atoi((const char*) mois_fichier);
        if (mois <= 0 || mois > 12)
        {
            cerr << ERROR_HTML_TAG " pas de donn�es sur le mois." ENDL;

#ifdef STRICT
            if (log.open()) log.close();
            exit(-517);
#endif
            /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
            if (verbeux) cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tol�rant)." ENDL;
            return SKIP_FILE;
        }

    }
    else
    {
        cerr << ERROR_HTML_TAG "Mois non d�tectable" ENDL;

#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-503);
#else
        /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
        if (verbeux)
            cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode tol�rant)." ENDL;
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

        warning_msg("la balise Employeur [non-conformit� � la norme].", info, cur);

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
                cerr << ERROR_HTML_TAG "Pas de donn�es sur le nom de l'employeur [non-conformit� � la norme]." ENDL;
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
                cerr << ERROR_HTML_TAG "Employeur non identifi� [non-conformit� � la norme]." ENDL;
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
                cerr  << ERROR_HTML_TAG "Siret de l'empoyeur non identifi� [non-conformit� � la norme]." ENDL;
                if (verbeux)
                    cerr << PROCESSING_HTML_TAG "Poursuite du traitement (mode souple)." ENDL;

                cerr << "Ann�e " << annee_fichier
                     << " Mois " << mois_fichier << ENDL;
                siret_fichier = xmlStrdup(NA_STRING);
            }

        } while (false);

        /* on remonte d'un niveau */

        cur = cur_save;
    }

    /* On effectue un saut direct par �tiquetage ici dans le cas de fichiers d�coup�s dont les informations
     * d'ent�te (ann�e, mois...) sont d�finits par les pointeurs statiques d�clar�s en d�but de fonction */

donnees_indiv:

    cur = atteindreNoeud("DonneesIndiv", cur);

    if (cur == nullptr || xmlIsBlankNode(cur))
    {
        cerr << ERROR_HTML_TAG "Pas de donn�es individuelles de paye [DonneesIndiv, non conformit� � la norme]." ENDL;
        warning_msg("la balise DonneesIndiv", info, cur);

#ifdef STRICT
        if (log.is_open())
            log.close();
        exit(-525);
#else
        /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
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
            cerr << ERROR_HTML_TAG "Pas de donn�es individuelles de paye [non conformit� � la norme]." ENDL;
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
            /* Il faudra sans doute ajuster les diff�rences entre le parsing C et l'analyse Xml, qui vont diverger */
            return SKIP_FILE;
#endif
        }

        /* Les donn�es sur l'�tablissement sont optionnelles */

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

            /* Alta�r a ici deux modes : strict, le non-repect de la norme entraine une exception
                 * ou souple : on continue l'ex�cution en consid�rant que bien que le nom/siret soit obligatoire,
                 * l'ensemble du champ lui-m�me est en fait optionnel. [la norme est quelque peu inconsistante sur ce point]
                 */

            cur_save2 = cur;

            /* On recherche le nom, le siret de l'�tablissement */
            do {
                cur =  cur->xmlChildrenNode;

                if (cur == nullptr || xmlIsBlankNode(cur))
                {
                    warning_msg("les donn�es nominales de l'�tablissement [non-conformit�]", info, cur);

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
                        warning_msg("les donn�es nominales de l'�tablissement [non-conformit�]", info, cur);
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
                    warning_msg("les donn�es nominales de l'�tablissement [non-conformit�]", info, cur);
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
                        warning_msg("les donn�es de Siret de l'�tablissement [non-conformit�]", info, cur);
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
                    warning_msg("les donn�es de Siret de l'�tablissement [non-conformit�]", info, cur);
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
                cerr << ERROR_HTML_TAG "Pas d'information sur les lignes de paye [non-conformit� � la norme : PayeIndivMensuel]." ENDL;

                if (cur == nullptr)
                    warning_msg("la balise PayeIndivMensuel de l'�tablissement [non-conformit�]", info, cur);
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

            /* remarque atoi retourne z�ro s'il rencontre "" ou des caract�res non num�riques */

            info.Table[info.NCumAgentXml][Annee] = xmlStrdup(annee_fichier);
            info.Table[info.NCumAgentXml][Mois]  = xmlStrdup(mois_fichier);
            info.Table[info.NCumAgentXml][Budget] = xmlStrdup(budget_fichier);
            info.Table[info.NCumAgentXml][Employeur]  = xmlStrdup(employeur_fichier);

            /* Nota : le Siret est, si l'�tablissement existe, celui de l'�tablissement, sinon celui de l'Employeur */

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
                        cerr << ERROR_HTML_TAG "Incoh�rence des d�comptes de lignes entre le contr�le C : "
                             << info.NLigne[info.NCumAgentXml]
                             << "et l'analyse Libxml2 : "
                             << ligne_p
                             << ENDL "Pour l'agent "
                             << "de rang  " << info.NCumAgentXml << " dans le fichier" ENDL
                             << info.Table[info.NCumAgentXml][Matricule]
                                << " Ann�e "
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

                    /* il faut tout r�allouer */
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
        /* si pas d'�tablissement (NA_STRING) alors on utilise le siret de l'employeur, donc
         * ne pas lib�rer dans ce cas ! */

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
                 << "n�" <<  rang_global
        #endif
                 << " : " << info.threads->argv[info.fichier_courant].first << ENDL;

            cerr << STATE_HTML_TAG << "Fil n�" << info.threads->thread_num + 1 << " : " << "Fichier courant : " << info.fichier_courant + 1 << ENDL;
            cerr << STATE_HTML_TAG << "Total : " <<  info.NCumAgentXml << " bulletins -- " << info.nbLigne <<" lignes cumul�es." ENDL;
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

#if defined(STRINGSTREAM_PARSING) || defined(MMAP_PARSING)
    info.threads->in_memory_file.at(info.fichier_courant).clear();
#endif

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

    /* Note sur les modes d'ex�cution :
     * Le mode == strict == envoie une exception d�s la premi�re non-conformit�,
     * sauf exceptions contraires signal�es en commentaire.
     * Le mode == tol�rant == arr�te l'ex�cution du fichier et passe suivant.
     * Cela peut poser des probl�mes de (r�)allocation/r�servation/lib�ration de m�moire
     * Le mode == souple == continue l'ex�cution le plus possible dans le m�me fichier.
     * Le mode strict fait l'objet d'une compilation s�par�e. Les modes tol�rant ou souple
     * choisis au cas par cas en fonction d'une �valuation plus ou moins subjective de la gravit�
     * de la non-conformit�. */

    int nb_decoupe = info.threads->argv.at(info.fichier_courant).second;

    int res = 0;
    int cont_flag = PREMIER_FICHIER;
    xml_commun champ_commun;

    if (nb_decoupe == 1)
    {
#if defined(STRINGSTREAM_PARSING) || defined(MMAP_PARSING)
        xmlDocPtr doc = xmlParseFile(info.threads->argv.at(info.fichier_courant).first.c_str());
#else
        xmlDocPtr doc = xmlParseDoc(reinterpret_cast<const xmlChar*>(info.threads->in_memory_file.at(info.fichier_courant).c_str()));
#endif

        return Analyseur::parseFile(doc, info, cont_flag, &champ_commun);
    }

    // -----  nb_decoupe != 0

    int rang  = 0;

#if defined(STRINGSTREAM_PARSING) || defined(MMAP_PARSING)
    vector<string> cut_chunks = info.threads->in_memory_file_cut.at(info.fichier_courant);
#else
    const vector<string> cut_chunks = info.threads->argv_cut.at(info.fichier_courant);
#endif

    int NDecoupe = cut_chunks.size();

    for (auto && s :  cut_chunks)
    {
        ++rang;
        if (rang > nb_decoupe)
        {
         // d�finir les leftovers ici
            break;
        }

        if (verbeux)
        {
            cerr << ENDL;
            cerr << ".....     .....     ....." ENDL;
            cerr << ENDL;

            cerr << PROCESSING_HTML_TAG "Analyse du fichier scind� fil " << info.threads->thread_num + 1
                 << " - n�" << info.fichier_courant + 1 << "-" << rang << "/" << nb_decoupe << " de " << NDecoupe;

#if defined(FGETC_PARSING)
            cerr << " : " << s << ENDL;
#else
            cerr << ENDL;
#endif
        }

        if (rang == 1)
            cont_flag = PREMIER_FICHIER;
        else
        if (rang == NDecoupe)
            cont_flag = DERNIER_FICHIER_DECOUPE;
        else
            cont_flag = FICHIER_SUIVANT_DECOUPE;

#if defined(STRINGSTREAM_PARSING) || defined(MMAP_PARSING)
        xmlDocPtr doc = xmlParseDoc(reinterpret_cast<const xmlChar*>(s.c_str()));
        s.clear();
#else
        xmlDocPtr doc = xmlParseFile(s.c_str());
#endif

        if (verbeux) cerr << ENDL;

        if (doc == nullptr)
        {
            cerr << ERROR_HTML_TAG "L'analyse du parseur XML n'a pas pu �tre r�alis�e." ENDL;
        }
        else
            res = Analyseur::parseFile(doc, info, cont_flag, &champ_commun);

        if (verbeux)
        {
            cerr << PROCESSING_HTML_TAG "Fin de l'analyse du fichier scind� fil " << info.threads->thread_num + 1
                 << " n�" << info.fichier_courant + 1 << "-" << rang << "/" << nb_decoupe << " de " << NDecoupe;
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
                cerr << PROCESSING_HTML_TAG "Effacement du fichier scind� fil " << info.threads->thread_num + 1
                     << " n�" << info.fichier_courant + 1<< "-" << rang  << "/" << nb_decoupe << " de " << NDecoupe << " : " << s << ENDL;
        }
#endif

        if (res == SKIP_FILE || res == RETRY) return res;

    }

    return 0;

}

