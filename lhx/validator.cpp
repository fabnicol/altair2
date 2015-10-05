/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */

/* Constantes de compilation pouvant être redéfinies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, NO_REGEXP */

#include <iomanip>
#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "table.hpp"


static inline xmlNodePtr atteindreNoeud(const char* noeud, xmlNodePtr cur)
{
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
    }

    while (cur != nullptr)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) noeud))
        {
            cur = cur->next;
        }
        else
        {
            AFFICHER_NOEUD(noeud)
            break;
        }
    }
    return cur;
}

static inline xmlNodePtr atteindreNoeudArret(const char* noeud, xmlNodePtr cur, const char* arret)
{
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
    }

    while (cur != nullptr)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) noeud))
        {
            if (xmlStrcmp(cur->name, (const xmlChar *) arret))
                cur = cur->next;
            else
                return nullptr;
        }
        else
        {
            AFFICHER_NOEUD(noeud)
            break;
        }
    }
    return cur;
}


static inline void  verifier_taille(const int nbLignePaye, info_t& info)
{
    if (nbLignePaye >= info.nbLigneUtilisateur)
    {
        fprintf(stderr, "\n\
                En excès du nombre de lignes de paye autorisé (%d).\n\
                Omettre -n ... et utiliser -L fichier_log pour détecter le maximum de lignes de paye dans les fichiers.\n\
                Utiliser -N ce_maximum ou bien recompiler en augmentant MAX_LIGNES_PAYE, à condition de disposer d'assez de mémoire.\n",
                info.nbLigneUtilisateur);

        exit(-10);
    }
}

/* obligatoire */

#define ligne_l  info.Table[info.NCumAgentXml][l]

/* Remplace les occurrences d'un caractère séparateur à l'intérieur d'un champ par le caractère '_' qui ne doit donc jamais
   être séparateur de champ (c'est bien rare !) */

static inline bool Bulletin(const char* tag, xmlNodePtr* cur, int l, info_t& info)
{
    bool test = (cur != nullptr && *cur != nullptr && (! xmlStrcmp((*cur)->name,  (const xmlChar*) tag)));

    if (test)
    {
        ligne_l = xmlGetProp(*cur, (const xmlChar *) "V");
        if (info.drapeau_cont)
            *cur = (*cur)? (*cur)->next: nullptr;
    }

    if (ligne_l == nullptr || ! test)
    {
        ligne_l = (xmlChar*) xmlStrdup(NA_STRING);
    }
    else
    if (ligne_l[0] == '\0')
    {
        xmlFree(ligne_l);
        ligne_l = (xmlChar*) xmlStrdup(NA_STRING);
    }
  /* sanitisation */

    else
        for (int i = 0; i < xmlStrlen(ligne_l); ++i)
            if (ligne_l[i] == info.separateur)
                ligne_l[i] = '_';

    return test;
}

static inline void _Bulletin(const char* tag, xmlNodePtr* cur,  int l,  info_t& info)
{
    if (! Bulletin(tag, cur, l, info))
    {
        if (*cur)
        {
            fprintf(stderr, "Erreur : Trouvé %s au lieu de %s \n", (*cur)->name, tag);
            fprintf(stderr, "Erreur : dans le fichier %s \n  pour le matricule %s\n",
                            info.threads->argv[info.fichier_courant],
                            info.Table[info.NCumAgentXml][Matricule]);
        }
        else
        {
            fprintf(stderr, "Erreur : Noeud courant null au stade de la vérification de %s, fichier %s\n", tag, info.threads->argv[info.fichier_courant]);
            for (int l=0; l < Service; ++l) fprintf(stderr, "info.Table[info.NCumAgentXml][%d]=%s\n", l, info.Table[info.NCumAgentXml][l]);
        }
    }

}

/* A tester : la substitution du caractère décimal , au . de la locale anglaise utilisé par Xémélios (hélas)
   reste nécessaire tant que nous utiliserons un stockage uniforme en chaînes de caractères.
   Si un jour nous décidons d'utilisr strold pour convertir les chaînes de caractère numériques en float, nous
   gagnerons de la place en stockage temporaire (peut être utile pour les gros fichiers) et alors printf et setlocale
   feront le travail de substitution de la virgule au point lors de l'écriture de la base.
   A ce stade nous stockons tous les champs lus en char, pour écriture identique en .csv dans la table, avec substition
   'manuelle' de la virgule au point dans la chaîne en output. */

static inline void substituer_separateur_decimal(xmlChar* ligne, const char decimal)
{
    for (int i = 0; i < xmlStrlen(ligne); ++i)
        if (ligne[i] == '.') ligne[i] = decimal;
}

/* optionnel */

static inline void _Bulletin_(const char* tag, xmlNodePtr* cur,  int l, info_t& info)
{
    if (! Bulletin(tag, cur, l,  info))
    {
        ligne_l = (xmlChar*) xmlStrdup(NA_STRING);

    }

    if (info.decimal != '.') substituer_separateur_decimal(ligne_l, info.decimal);

}

/* obligatoire et avec substitution séparateur décimal */

static inline void Bulletin_(const char* tag, xmlNodePtr* cur, int l, info_t& info)
{
    _Bulletin(tag, cur, l, info) ;
    if (info.decimal != '.')  substituer_separateur_decimal(ligne_l, info.decimal);

}

static inline int lignePaye(xmlNodePtr cur, info_t& info)
{
    int l = info.minimum_memoire_p_ligne;

    int nbLignePaye = 0;
    unsigned int t = 0;

    ligne_l = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour éviter la confusion avec \0 des chaines vides
    ++l;

    /* Besoins en mémoire : 18 [champs hors ligne] + nombre de lignes + flags (maximum nbType) */
    bool rembobiner = false;

    while (cur != nullptr)
    {
        bool new_type = false;

        while (xmlStrcmp(cur->name, (const xmlChar *) type_remuneration[t]))
        {
			++t;
            if (t == nbType)
            {
                /* En principe les éléments constitutifs des enregistrements <Remunération>....</Remuneration> sont enregistrés
                   dans l'ordre du tableau type_remuneration. Toutefois quelques cas de désordre sont observés. Dans ces cas là on peut
                   "rembobiner le tableau". On évite toutefois de faire une recherche ensembliste systématique, qui éviterait cela mais
                   freinerait 99,9 % des recherches */

                if (rembobiner == false)
                {
                    rembobiner = true;
                    t = 0;
                    continue;
                }

                /* On ne rembobine qu'une seule fois. Si l'essai échoue, on déclenche une exception */

                fprintf(stderr, "Erreur : En excès du nombre de types de lignes de paye autorisé (%d)\n", nbType);
                if (cur) fprintf(stderr, "Erreur : Type litigieux %s aux alentours du matricule %s \n",
                                     cur->name,
                                     info.Table[info.NCumAgentXml][Matricule]);
                else fprintf(stderr, "%s", "Erreur : Pointeur noeud courant nul\n");
                exit(-11);
            }

            new_type = true;
        }

        rembobiner = false;

        if (new_type && t < nbType)
        {
            ligne_l = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour éviter la confusion avec \0 des chaines vides
            ++l;
        }

        if (! info.reduire_consommation_memoire)
        {
            verifier_taille(nbLignePaye, info);
        }

        if (! xmlStrcmp(cur->name, (const xmlChar*) "Commentaire"))
        {
            cur = cur->next;
            t=0;
            rembobiner=false; // 'Rembobinage gratuit'
            continue;
        }

        DESCENDRE_UN_NIVEAU
        /* Libellé, obligatoire */

        cur = atteindreNoeud("Libelle", cur);

        _Bulletin("Libelle", &cur, l, info);
        ++l;
        /* Code, obligatoire */
        cur = atteindreNoeud("Code", cur);
        _Bulletin("Code", &cur, l, info);

		++l;

        /* Base, si elle existe */

        _Bulletin_("Base", &cur,  l, info);
		++l;

        /* Taux, s'il existe */
        _Bulletin_("Taux", &cur, l, info);
		++l;

        /* Nombre d'unités, s'il existe */
        _Bulletin_("NbUnite", &cur, l, info);
		++l;

        /* Montant , obligatoire */
        cur = atteindreNoeud("Mt", cur);

        Bulletin_("Mt", &cur, l, info);
		++l;
        ++nbLignePaye;

        REMONTER_UN_NIVEAU

        // Lorsque on a épuisé tous les types licites on a nécessairement cur = nullptr
    }

#undef ligne_l

    return nbLignePaye;
}

#define _BULLETIN(X) _Bulletin(#X, &cur, X, info);

#define BULLETIN_(X)  Bulletin_(#X, &cur, X, info);

#define _BULLETIN_(X)  _Bulletin_(#X, &cur, X, info);

static uint64_t  parseBulletin(xmlNodePtr cur, info_t& info)
{
    DEBUG("Parsage")

    if (cur == nullptr) return 0;
    cur = atteindreNoeud("Agent", cur);

    if (cur == nullptr)
    {
        fprintf(stderr, "%s\n", "Erreur : Impossible d'atteindre \"Agent\"");
        return 0;
    }

    DESCENDRE_UN_NIVEAU

    /* dans certains sxhémas on peut ne pas avoir la civilité */
    cur = atteindreNoeud("Nom", cur);

    /* passer à la balise adjacente après lecture */
    info.drapeau_cont = true;
    _BULLETIN(Nom)

#ifdef TOLERANT
    cur = cur->parent;
    cur = cur->xmlChildrenNode;
    xmlNodePtr cur_save = cur;

#endif
    /* Dans certains cas on peut avoir un complément de nom */
    cur = atteindreNoeud("Prenom", cur);
    _BULLETIN(Prenom)
    _BULLETIN(Matricule)
    _BULLETIN(NIR)

#ifdef TOLERANT
    cur = cur_save;
#endif

	cur = atteindreNoeud("NbEnfants", cur);
	_BULLETIN(NbEnfants)
#ifdef TOLERANT
    cur = atteindreNoeud("Statut", cur);
#else
    cur = cur->next;
#endif

    _BULLETIN(Statut)
    /* dans certains schémas on peut avoir ici des balises */

#ifdef TOLERANT
    cur = cur_save;
#endif

    cur = atteindreNoeud("EmploiMetier", cur);

    _BULLETIN(EmploiMetier)

#ifdef TOLERANT
    cur = cur_save;
    cur = atteindreNoeud("Grade", cur);
#else
    cur = cur->next;
#endif

    _BULLETIN(Grade)

#ifdef TOLERANT
    cur = cur_save;
    cur = atteindreNoeud("Indice", cur);
#else
    cur = cur->next;
#endif
    info.drapeau_cont = false; /* ne pas lire la balise adjacente : fin du niveau subordonné Agent*/
    _BULLETIN(Indice)

    REMONTER_UN_NIVEAU

    info.drapeau_cont = true;

#ifdef TOLERANT
    cur = atteindreNoeud("Service", cur);
#else
    while (cur && xmlStrcmp(cur->name, (const xmlChar*)"Service"))
    {
       if (xmlStrcmp(cur->name, (const xmlChar*)"NBI") == 0)
       {
           info.Table[info.NCumAgentXml][Service]= (xmlChar*) xmlStrdup(NA_STRING);
           goto nbi;
       }
       else
        cur = cur->next;
    }
#endif
    _BULLETIN(Service)
#ifdef TOLERANT
    cur = cur->parent;
    cur = cur->xmlChildrenNode;
    cur_save = cur;
    cur = atteindreNoeud("NBI", cur);
#else
	cur = cur->next;
#endif
    nbi :
      BULLETIN_(NBI)

#ifdef TOLERANT
    cur = cur_save;
    cur = atteindreNoeud("QuotiteTrav", cur);
#else
    cur = cur->next;
    cur = cur->next;
#endif

    /* obligatoire, substitution du séparateur décimal */
    BULLETIN_(QuotiteTrav)

#ifdef TOLERANT
    cur = atteindreNoeud("Remuneration", cur);
#else
    cur = cur->next;
#endif

    int ligne = 0;

    if (cur)
    {
        xmlNodePtr cur_save = cur;

        if (xmlChildElementCount(cur))
        {
            DESCENDRE_UN_NIVEAU
            ligne = lignePaye(cur, info);
        }

        if (ligne == 0)
        {
            for (int k=0; k < 6; ++k)
                info.Table[info.NCumAgentXml][info.minimum_memoire_p_ligne + k] = (xmlChar*) xmlStrdup(NA_STRING);
        }

        cur = cur_save->next;
    }
    else
    {
        perror("Erreur : Rémunération introuvable.");
        exit(-4);
    }

    /* non obligatoire , substitution du sparateur décimal */

    _BULLETIN_(NbHeureTotal)
    cur = atteindreNoeud("NbHeureSup", cur);

    /* obligatoire, substitution du sparateur décimal */
    BULLETIN_(NbHeureSup)
    BULLETIN_(MtBrut)
    BULLETIN_(MtNet)

    info.drapeau_cont=false; // fin du niveau PayeIndivMensuel
    BULLETIN_(MtNetAPayer)

    // Rémuneration tag vide
    if (ligne == 0) ligne = 1 ;

    return ligne;
}

/* agent_total est une variable de contrôle pour info->NCumAgent */

static void parseFile(info_t& info)
{
    std::ofstream log;
    xmlDocPtr doc;
    xmlNodePtr cur = nullptr;
    info.NAgent[info.fichier_courant] = 0;
    xmlChar *annee_fichier = nullptr, *mois_fichier = nullptr, *etab_fichier = nullptr, *siret_fichier = nullptr, *budget_fichier = nullptr;

    doc = xmlParseFile(info.threads->argv[info.fichier_courant]);

    if (doc == nullptr) return;

    if (! info.chemin_log.empty())
       {
         log.open(info.chemin_log, std::ios::app);
       }

    cur = xmlDocGetRootElement(doc);

    if (cur == nullptr)
    {
        fprintf(stderr,"Erreur : document vide\n");
        xmlFreeDoc(doc);
    }

    DESCENDRE_UN_NIVEAU

    cur = atteindreNoeud("Annee", cur);

    if (cur != nullptr)
    {
        annee_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        cur = (cur)? cur->next : nullptr;
    }
    else
    {
        fprintf(stderr, "%s\n", "Erreur : Année non détectable");
        exit(-502);
    }

    if (cur != nullptr)
    {
        mois_fichier = xmlGetProp(cur, (const xmlChar *) "V");
    }
    else
    {
        fprintf(stderr, "%s\n", "Erreur : Mois non détectable");
        exit(-503);
    }

    cur = atteindreNoeud("Budget", cur);

    if (cur != nullptr)
    {
        DESCENDRE_UN_NIVEAU
        budget_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        REMONTER_UN_NIVEAU
    }
    else
    {
        fprintf(stderr, "%s\n", "Erreur : Budget non détectable");
        exit(-504);
    }

  while((cur = atteindreNoeudArret("DonneesIndiv", cur, "Nomenclatures")) != nullptr)
  {
        xmlNodePtr cur_save = cur;
        xmlNodePtr cur_save2 = nullptr;

        DESCENDRE_UN_NIVEAU

            cur = atteindreNoeud("Etablissement", cur);
            if (cur == nullptr) cur = atteindreNoeud("Employeur", cur);
            cur_save2 =  cur;
            if (cur_save2 == nullptr) break;

            DESCENDRE_UN_NIVEAU

            cur = atteindreNoeud("Nom", cur);
            if (cur != nullptr)
            {
                etab_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                cur = (cur)? cur->next : nullptr;
            }
            else
            {
                fprintf(stderr, "%s\n", "Erreur : Etablissement/Employeur non détectable");
                exit(-505);
            }

            cur = atteindreNoeud("Siret", cur);

            if (cur != nullptr)
            {
                siret_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                cur = (cur)? cur->next : nullptr;
            }
            else
            {
                fprintf(stderr, "%s Année %d Mois %d\n", "Erreur : Siret non détectable", info.Table[info.NCumAgentXml][Annee], info.Table[info.NCumAgentXml][Mois]);
                exit(-506);
            }
            cur=cur_save2;

        while(cur != nullptr)
        {
//            if (info.NCumAgentXml != info.NCumAgent)
//            {
//                std::cerr << "Erreur : Incohérence de l'allocation mémoire ex-ante " << info.NCumAgent 
//                          << " B et ex-post " <<  info.NCumAgentXml << " B : sortie pour éviter une erreur de segmentation.\n";
//                std::cerr << "Fichier : " << info.threads->argv[info.fichier_courant] << std::endl;
                        
//                exit(1005);
//            }


            cur = atteindreNoeud("PayeIndivMensuel", cur);

            cur_save2 =  cur;

            if (cur_save2 == nullptr) break;

            DESCENDRE_UN_NIVEAU

            info.Table[info.NCumAgentXml][Annee] = xmlStrdup(annee_fichier);
            info.Table[info.NCumAgentXml][Mois]  = xmlStrdup(mois_fichier);
            info.Table[info.NCumAgentXml][Budget] = xmlStrdup(budget_fichier);
            info.Table[info.NCumAgentXml][Etablissement]  = xmlStrdup(etab_fichier);
            info.Table[info.NCumAgentXml][Siret]  = xmlStrdup(siret_fichier);

            int32_t ligne_p = parseBulletin(cur, info);
            info.drapeau_cont = true;

            if (info.reduire_consommation_memoire)
            {
                if (ligne_p != info.NLigne[info.NCumAgentXml])
                {
                    fprintf(stderr, "Incohérence des décomptes de lignes entre le contrôle C : %d et l'analyse Libxml2 : %d\nPour l'agent %s, Année %s Mois %s\n",
                            info.NLigne[info.NCumAgentXml],
                            ligne_p,
                            info.Table[info.NCumAgentXml][Matricule],
                            info.Table[info.NCumAgentXml][Annee],
                            info.Table[info.NCumAgentXml][Mois]);
                    exit(-1278);
                }
            }
            else
                info.NLigne[info.NCumAgentXml] = ligne_p;

            info.nbLigne += ligne_p;

            if (! info.chemin_log.empty())
            {
                int diff = info.NLigne[info.NCumAgentXml]-ligne_p;
                if (log.good())
                #define P  " | "
                log << "Année " << P
                    << info.Table[info.NCumAgentXml][Annee] << P
                    << "Mois "  << std::setw(2) << info.Table[info.NCumAgentXml][Mois] << P
                    << "Matricule " << std::setw(6) <<  info.Table[info.NCumAgentXml][Matricule] << P
                    << "Rang global " << std::setw(6) <<  info.NCumAgentXml << P
                    << "Rang dans fichier " << std::setw(5) <<  info.NAgent[info.fichier_courant] << P
                    << "Analyseur C : " << std::setw(6) << info.NLigne[info.NCumAgentXml] << P
                    << "Xml : " << std::setw(6) << ligne_p << P
                    << "Différence " << std::setw(4) << diff << std::endl;
                #undef P
            }
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
    std::cerr << "Fichier n°" << info.fichier_courant + 1 << " :\nPopulation du fichier  " <<  info.threads->argv[info.fichier_courant] << ":\n"
              <<  info.NAgent[info.fichier_courant] << " bulletins    Total : " <<  info.NCumAgentXml << "bulletins  " << info.nbLigne <<" lignes cumulées.\n";


    xmlFreeDoc(doc);
    if (log.is_open())
       log.close();

}

 /* Les expressions régulières correctes ne sont disponibles sur MINGW GCC qu'à partir du build 4.9.2 */

#if !defined GCC_REGEX && (defined __WIN32__ || defined GCC_4_8)
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

using namespace std;
#else
#error "C++11 doit être utilisé."
#endif


#endif // defined

void* decoder_fichier(info_t& info)
{
    /* environ 6000 bulletins par seconde en processus sumple, et 15000 en multithread ; rajoute 1/3 du temps */

    #if  defined GCC_REGEX //&& !defined __WIN32__ && !defined GCC_4_8

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
            perror("Erreur : Calcul de la mémoire requise");
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

    info.NAgent = new uint32_t[info.threads->argc];
    info.Table  = new xmlChar**[info.NCumAgent];

    if (info.Table == nullptr)
    {
        perror("Erreur : Mémoire insuffisante");
        exit(-18);
    }

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
    {
        info.Table[agent] = (xmlChar**) calloc(info.minimum_memoire_p_ligne + nbType + (info.NLigne[agent])*6, sizeof(xmlChar*));
        if (info.Table[agent] == nullptr)
        {
            fprintf(stderr,
                    "Erreur : Erreur d'allocation de drapeau I. pour l'agent %d et pour %d B\n",
                     agent,
                     info.minimum_memoire_p_ligne + nbType + (info.NLigne[agent]) * 6);
            exit(-63);
        }
    }

    for (unsigned i = 0; i < info.threads->argc ; ++i)
    {
        info.fichier_courant = i;
        parseFile(info);
    }

    // attention, pas info<-NCumAgent ici

    /* Le champ statut est modifié comme suit :
        ELU   pour un élu
        V     pour un vacataire
        A     pour une assistante maternelle */

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

            for (int j = info.minimum_memoire_p_ligne ; j < info.NLigne[agent]; ++j)
                if (regex_match((const char*) VAR(j), pat2))
                {
                    xmlFree(VAR(Grade));
                    VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"V");
                }
        }
        else
        {
            for (int j = info.minimum_memoire_p_ligne ; j < info.NLigne[agent] && info.Table[agent][j] != nullptr; ++j)
                if (regex_match((const char*) VAR(j), pat2))
                {
                    xmlFree(VAR(Grade));
                    VAR(Grade) = (xmlChar*) xmlStrdup((const xmlChar*)"V");
                }
        }
    }


#undef VAR
    return nullptr;
}
//
//#ifdef __cplusplus
//}
//#endif

