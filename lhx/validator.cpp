/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */

/* Constantes de compilation pouvant être redéfinies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, NO_REGEXP */

#include <iomanip>
#include <iostream>
#include <cstring>
#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "table.hpp"
#include "ligne_paye.hpp"
extern std::mutex mut;
extern  inline uint64_t  GCC_INLINE parseLignesPaye(xmlNodePtr cur, info_t& info, std::ofstream& log);

/* agent_total est une variable de contrôle pour info->NCumAgent */

static int parseFile(info_t& info)
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

    std::ofstream log;
    xmlDocPtr doc;
    xmlNodePtr cur = nullptr;
    info.NAgent[info.fichier_courant] = 0;
    xmlNodePtr cur_save = cur;
    xmlChar *annee_fichier = nullptr,
            *mois_fichier = nullptr, 
            *etab_fichier = nullptr,
            *siret_fichier = nullptr,
            *budget_fichier = nullptr;

    doc = xmlParseFile(info.threads->argv[info.fichier_courant]);

    if (doc == nullptr)
    {
        std::cerr << "Erreur :  problème d'allocation mémoire pour le scan XML.\n";
        return SKIP_FILE;
    }

    if (! info.chemin_log.empty())
       {
         log.open(info.chemin_log, std::ios::app);
       }

    cur = xmlDocGetRootElement(doc);

    if (cur == nullptr)
    {
        std::cerr << "Erreur : document vide\n";
        xmlFreeDoc(doc);
        if (log.is_open())
           log.close();
        return SKIP_FILE;
    }

    cur =  cur->xmlChildrenNode;

    cur = atteindreNoeud("Annee", cur);

    if (cur != nullptr)
    {
        annee_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        cur = (cur)? cur->next : nullptr;
    }
    else
    {
        std::cerr << "Erreur : Année non détectable\n";
        if (log.is_open())
           log.close();
#ifdef STRICT
        if (log.is_open())
           log.close();
        exit(-502);
#else
        return SKIP_FILE;
#endif
    }

    if (cur != nullptr)
    {
        mois_fichier = xmlGetProp(cur, (const xmlChar *) "V");
    }
    else
    {
        std::cerr << "Erreur : Mois non détectable\n";

#ifdef STRICT
        if (log.is_open())
           log.close();
        exit(-503);
#else
        return SKIP_FILE;
#endif

    }

    cur = atteindreNoeud("Budget", cur);

    if (cur != nullptr)
    {
        cur =  cur->xmlChildrenNode;
        budget_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        cur = cur->parent;
        cur = cur->next;
        // si cur == nullptr on aura un arrêt de la boucle.
    }
    else
    {
        std::cerr << "[MSG] Budget non détectable\n";
    }

    cur_save = cur;

    if (nullptr == (cur = atteindreNoeud("Employeur", cur)))
    {
#ifdef STRICT
        std::cerr << "Erreur :  non conformité aux normes. Le rattachement comptable de l'agent est incertain !\n";
        exit(-515);
#endif
        afficher_environnement_xhl(info);
        cur = cur_save;
    }

  while((cur = atteindreNoeudArret("DonneesIndiv", cur, "Nomenclatures")) != nullptr)
  {
        xmlNodePtr cur_save2 = nullptr;

        cur =  cur->xmlChildrenNode;  // Niveau Etablissement et PayeIndivMensuel
        cur_save2 = cur;

        if (cur == nullptr || xmlIsBlankNode(cur))
        {
            std::cerr << "Erreur :  pas de données individuelles de paye (PayeIndivMensuel)\n";
            afficher_environnement_xhl(info);
#ifdef STRICT
            if (log.is_open())
               log.close();
            exit(-515);
#else
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
            if (cur == nullptr )
            {

                    std::cerr << "[MSG]  Pas d'information sur l'établissement. Le rattachement comptable de l'agent est incertain !\n";
                    afficher_environnement_xhl(info);
                    cur = cur_save2;
            }
            else
            {

                /* On recherhce le nom, le siret de l'établissement */

                cur =  cur->xmlChildrenNode;

                if (cur == nullptr || xmlIsBlankNode(cur))
                {
                    std::cerr << "Erreur :  pas de données sur le nom de l'établissement\n";
                    afficher_environnement_xhl(info);
                }

                cur = atteindreNoeud("Nom", cur);
                if (cur != nullptr)
                {
                    etab_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                    cur = (cur)? cur->next : nullptr;
                }
                else
                {
                    std::cerr << "Erreur : Etablissement/Employeur non identifié (pas de nom).\n";
                    etab_fichier = xmlStrdup(NA_STRING);

                }

                cur = atteindreNoeud("Siret", cur);

                if (cur != nullptr)
                {
                    siret_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                    cur = (cur)? cur->next : nullptr;
                }
                else
                {
                    std::cerr  << "Erreur : Siret non identifié.\n";
                    std::cerr << "Année " << annee_fichier
                              << " Mois "  << mois_fichier << "\n";
                    siret_fichier = xmlStrdup(NA_STRING);

                }
            }


        /* on remonte d'un niveau */

        cur = cur_save2;

        while(cur != nullptr)
        {

            cur = atteindreNoeud("PayeIndivMensuel", cur);

            cur_save2 =  cur;

            if (cur == nullptr)
                break;

            cur =  cur->xmlChildrenNode;

            if (cur == nullptr || xmlIsBlankNode(cur))  //pas de champs fils de PayeIndivMensuel : on doit sauter sans compter de ligne
                break;

            info.Table[info.NCumAgentXml][Annee] = xmlStrdup(annee_fichier);
            info.Table[info.NCumAgentXml][Mois]  = xmlStrdup(mois_fichier);
            info.Table[info.NCumAgentXml][Budget] = xmlStrdup(budget_fichier);
            info.Table[info.NCumAgentXml][Etablissement]  = xmlStrdup(etab_fichier);
            info.Table[info.NCumAgentXml][Siret]  = xmlStrdup(siret_fichier);

            /* LECTURE DES LIGNES DE PAYE STRICTO SENSU */

            int32_t ligne_p = parseLignesPaye(cur, info, log);

            /*  */

            info.drapeau_cont = true;

            int diff = info.NLigne[info.NCumAgentXml] - ligne_p;

            if (info.reduire_consommation_memoire)
            {
                if (diff != 0)
                {
                    std::cerr << "Erreur : Incohérence des décomptes de lignes entre le contrôle C : "
                              << info.NLigne[info.NCumAgentXml]
                              << "et l'analyse Libxml2 : "
                              << ligne_p
                              << "\nPour l'agent "
                              << info.Table[info.NCumAgentXml][Matricule]
                              << " Année "
                              << info.Table[info.NCumAgentXml][Annee]
                              << " Mois "
                              << info.Table[info.NCumAgentXml][Mois];

                     ecrire_log(info, log, diff);
                     if (log.is_open())
                        log.close();

#ifdef STRICT
                    exit(-1278);
#else
                    if (ligne_p > info.NLigne[info.NCumAgentXml])
                        info.NLigne[info.NCumAgentXml] = ligne_p;

                    /* il faut tout réallouer */

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

        cur = cur_save->next;
  }

    xmlFree(mois_fichier);
    xmlFree(annee_fichier);

    std::cerr << "[MSG] Fichier n°" << info.fichier_courant + 1 << " :\n[MSG] Population du fichier  " <<  info.threads->argv[info.fichier_courant] << ":\n[MSG] "
              <<  info.NAgent[info.fichier_courant] << " bulletins    Total : " <<  info.NCumAgentXml << "bulletins  " << info.nbLigne <<" lignes cumulées.\n";


    xmlFreeDoc(doc);
    if (log.is_open())
       log.close();

    return 0;

}

 /* Les expressions régulières correctes ne sont disponibles sur MINGW GCC qu'à partir du build 4.9.2 */

#if !defined GCC_REGEX && !defined NO_REGEX && (defined __WIN32__ || defined GCC_4_8)
#include <regex.h>

bool regex_match(const char *string, const char *pattern)
{
    int status;
    regex_t re;
    if (string == nullptr) return false;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB|REG_ICASE|REG_NEWLINE) != 0)
    {
        return(false); /* Report error. */
    }
    status = regexec(&re, string, (size_t) 0, nullptr, 0);
    regfree(&re);
    if (status != 0)
    {
        return(false); /* Report error. */
    }
    return(true);
}

const char* pat = EXPRESSION_REG_ELUS;
const char* pat2 = EXPRESSION_REG_VACATIONS;

#else
#ifdef __cplusplus
#include <regex>
#include <string>
#include <iostream>
#include <ctype.h>
#include <mutex>

using namespace std;
#else
#error "C++11 doit être utilisé."
#endif


#endif // defined


static inline  int GCC_INLINE memoire_p_ligne(const info_t& info, const unsigned agent)
{
    return BESOIN_MEMOIRE_ENTETE  // chaque agent a au moins BESOIN_MEMOIRE_ENTETE champs du bulletins de paye en colonnes
                                                      // sans la table ces champs sont répétés à chaque ligne de paye.
                            + nbType // espace pour les drapeaux de séparation des champs (taille de type_remuneration). Nécessaire pour l'algorithme
                            + (info.NLigne[agent])*(INDEX_MAX_CONNNES + 1);   // nombre de lignes de paye x nombre maximum de types de balises distincts de lignes de paye
                                                                              // soit N+1 pour les écritures du type Var(l+i), i=0,...,N dans ECRIRE_LIGNE_l_COMMUN
}


static inline void GCC_INLINE allouer_memoire_table(info_t& info)
{
    // utiliser NCumAgent ici et pas NCumAgentXml

    info.Memoire_p_ligne = new int[info.NCumAgent]();

    for (int agent = 0; agent < info.NCumAgent; ++agent)
    {
          info.Memoire_p_ligne[agent] = memoire_p_ligne(info, agent);
    }


    if (info.NAgent != nullptr) delete[] info.NAgent;
    if (info.Table != nullptr)
    {
        for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
        {

            for (int i = 0; i < info.Memoire_p_ligne[agent] ; ++i)
            {
                if (info.Table[agent][i] != nullptr) xmlFree(info.Table[agent][i]);
            }

            if (info.Table[agent]) delete info.Table[agent];

        }
      delete[] info.Table;
    }

    info.NAgent = new uint32_t[info.threads->argc]();
    info.Table  = new xmlChar**[info.NCumAgent]();
    info.NCumAgentXml = 0;

    if (info.NAgent == nullptr)
    {
        std::cerr << "Erreur : Mémoire insuffisante pour la table du nombre d'agents\n";
        exit(-19);
    }


    if (info.Table == nullptr)
    {
        std::cerr << "Erreur : Mémoire insuffisante pour la table de lignes de paye\n";
        exit(-18);
    }

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
    {

        info.Table[agent] = new xmlChar* [info.Memoire_p_ligne[agent]](); // ne pas oublier d'initialiser à nullptr !

        if (info.Table[agent] == nullptr)
        {
            std::cerr <<  "Erreur : Erreur d'allocation de drapeau I. pour l'agent "
                      <<  agent
                      <<  "et pour "
                      <<  info.Memoire_p_ligne[agent]
                      <<  " B\n";

            exit(-63);
        }
    }
}


void* decoder_fichier(info_t& info)
{
    /* environ 6000 bulletins par seconde en processus sumple, et 15000 en multithread ; rajoute 1/3 du temps */

    #if  defined GCC_REGEX && !defined NO_REGEX //&& !defined __WIN32__ && !defined GCC_4_8

     regex pat {"élu"/*info->expression_reg_elus*/,  regex_constants::icase};
     regex pat2 {EXPRESSION_REG_VACATIONS, regex_constants::icase};
     regex pat3 {EXPRESSION_REG_ASSISTANTES_MATERNELLES, regex_constants::icase};

    #endif

    xmlKeepBlanksDefault(0);

    if (info.reduire_consommation_memoire)
    {
        int err = calculer_memoire_requise(info);
        if (err)
        {
            std ::cerr << "Erreur : Calcul de la mémoire requise\n";
            exit(-1001);
        }
    }
    else
    {
        info.NCumAgent = info.nbAgentUtilisateur * info.threads->argc;
        info.NLigne.resize(info.NCumAgent);
        if (! info.NLigne.empty())
        {
            for (unsigned i = 0 ; i < info.NCumAgent; ++i)
                 info.NLigne[i] = info.nbLigneUtilisateur;
        }
        else
        {
            perror("Erreur : Problème d'allocation mémoire de info.NLigne");
            exit(1003);
        }
    }

    for (unsigned i = 0; i < info.threads->argc ; ++i)
    {
        if (i == 0)
        {
            /* première allocation ou réallocation à la suite d'un incident */

            allouer_memoire_table(info);

        }

#ifdef GENERATE_RANK_SIGNAL
        generate_rank_signal();
#endif
        info.fichier_courant = i;
        switch(parseFile(info))
        {
          case RETRY:
            i = 0;
            /* on réalloue tout depuis le début à la site d'un incident */
            std::cerr << "Erreur :  Il est nécessaire de réallouer la mémoire à la suite d'un incident dû aux données...\n";
            continue;

          case SKIP_FILE:
            std::cerr << "Erreur :  Le fichier  " << info.threads->argv[info.fichier_courant] << " n'a pas pu être traité\n"
                      << "   Fichier suivant...\n";
            continue;

          default :
            #ifndef NO_DEBUG
               std::cerr << "[INF] Fichier " << info.threads->argv[info.fichier_courant]  << "  traité\n";
            #endif
               break;
        }

    }

    if (info.NCumAgentXml != info.NCumAgent)
    {
        std::cerr << "Erreur : Incohérence de l'allocation mémoire ex-ante " << info.NCumAgent
                  << " unités et ex-post " <<  info.NCumAgentXml << " unités d'information.\n"
                  << "Sortie pour éviter une erreur de segmentation.\n";
        std::cerr << "\nFichier : " << info.threads->argv[info.fichier_courant] << "\n";

        exit(1005);
    }


    // attention, pas info<-NCumAgent ici

    /* Le champ statut est modifié comme suit :
        ELU   pour un élu
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
        }
        else
        /* vacataires */
        if (regex_match((const char*) VAR(EmploiMetier), pat2))
            {
                xmlFree(VAR(Grade));
                VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"V");
            }
        else
        /* assistantes maternelles */
        if (regex_match((const char*) VAR(EmploiMetier), pat3))
            {
                xmlFree(VAR(Grade));
                VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"A");
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

