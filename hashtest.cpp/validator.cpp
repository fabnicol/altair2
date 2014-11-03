/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */

/* Constantes de compilation pouvant être redéfinies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, NO_REGEXP */

//
//#ifdef __cplusplus
//extern "C" {
//#endif
#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "table.hpp"
#ifdef __cplusplus
 #include <regex>
 #include <string>
 #include <iostream>
 using namespace std;
#endif

static inline xmlNodePtr atteindreNoeud(const char* noeud, xmlNodePtr cur)
{
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
    }

    while (cur != NULL)
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

static inline void  verifier_taille(const int nbLignePaye, info_t* info)
{
    if (nbLignePaye >= info->nbLigneUtilisateur)
    {
        fprintf(stderr, "\n\
                En excès du nombre de lignes de paye autorisé (%d).\n\
                Omettre -n ... et utiliser -L fichier_log pour détecter le maximum de lignes de paye dans les fichiers.\n\
                Utiliser -N ce_maximum ou bien recompiler en augmentant MAX_LIGNES_PAYE, à condition de disposer d'assez de mémoire.\n",
                info->nbLigneUtilisateur);

        exit(-10);
    }
}

/* obligatoire */

#define ligne_l  info->Table[info->NCumAgentXml][l]

static inline bool Bulletin(const char* tag, xmlNodePtr* cur, int l, info_t* info)
{
    bool test = (cur != NULL && *cur != NULL && (! xmlStrcmp((*cur)->name,  (const xmlChar*) tag)));

    if (test)
    {
        ligne_l = xmlGetProp(*cur, (const xmlChar *) "V");

        if (ligne_l == NULL)
        {
            ligne_l = (xmlChar*) strdup(NA_STRING);
        }
        else if (ligne_l[0] == '\0')
        {
            xmlFree(ligne_l);
            ligne_l = (xmlChar*) strdup(NA_STRING);
        }

        /* sanitisation */

        else
          for (int i = 0; i < xmlStrlen(ligne_l); i++)
              if (ligne_l[i] == info->separateur)
                  ligne_l[i] = '_';

        if (info->drapeau_cont)
            *cur = (*cur)? (*cur)->next: NULL;
    }
    return test;
}

static inline void _Bulletin(const char* tag, xmlNodePtr* cur,  int l, info_t* info)
{
    if (! Bulletin(tag, cur, l, info))
    {
        if (*cur)
            fprintf(stderr, "Trouvé %s au lieu de %s \n", (*cur)->name, tag);
        else
        {
            fprintf(stderr, "Noeud courant null au stade de la vérification de %s\n", tag);
            for (int l=0; l < Service; l++) fprintf(stderr, "info->Table[info->NCumAgentXml][%d]=%s\n", l, info->Table[info->NCumAgentXml][l]);
        }
    }

}

static inline void substituer_separateur_decimal(xmlChar* ligne, const char decimal)
{
    for (int i = 0; i < xmlStrlen(ligne); i++)
        if (ligne[i] == '.') ligne[i] = decimal;
}

/* optionnel */

static inline void _Bulletin_(const char* tag, xmlNodePtr* cur,  int l, info_t* info)
{
    if (! Bulletin(tag, cur, l,  info))
    {
        ligne_l = (xmlChar*) strdup(NA_STRING);

    }

    if (info->decimal != '.') substituer_separateur_decimal(ligne_l, info->decimal);

}

/* obligatoire et avec substitution séparateur décimal */

static inline void Bulletin_(const char* tag, xmlNodePtr* cur, int l, info_t* info)
{
    _Bulletin(tag, cur, l, info) ;
    if (info->decimal != '.')  substituer_separateur_decimal(ligne_l, info->decimal);

}

static inline int lignePaye(xmlNodePtr cur, info_t* info)
{
    int l = info->minimum_memoire_p_ligne;

    int nbLignePaye = 0;
    unsigned int t = 0;

    ligne_l = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour éviter la confusion avec \0 des chaines vides
    l++;

    /* Besoins en mémoire : 18 [champs hors ligne] + nombre de lignes + flags (maximum nbType) */
    while (cur != NULL)
    {
        bool new_type = false;

        while (xmlStrcmp(cur->name, (const xmlChar *) type_remuneration[t]))
        {
            t++;
            if (t == nbType)
            {
                fprintf(stderr, "En excès du nombre de types de lignes de paye autorisé (%d)\n", nbType);
                if (cur) fprintf(stderr, "Type litigieux %s aux alentours du matricule %s \n",
                                     cur->name,
                                     info->Table[info->NCumAgentXml][Matricule]);
                else fprintf(stderr, "%s", "Pointeur noeud courant nul\n");
                exit(-11);
            }

            new_type = true;
        }

        if (new_type && t < nbType)
        {
            ligne_l = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour éviter la confusion avec \0 des chaines vides
            l++;
        }

        if (! info->reduire_consommation_memoire) { verifier_taille(nbLignePaye, info); }

        if (! xmlStrcmp(cur->name, (const xmlChar*) "Commentaire"))
        {
            cur = cur->next;
            continue;
        }

        DESCENDRE_UN_NIVEAU
        /* Libellé, obligatoire */

        cur = atteindreNoeud("Libelle", cur);

        _Bulletin("Libelle", &cur, l, info);
        l++;
        /* Code, obligatoire */
        cur = atteindreNoeud("Code", cur);
        _Bulletin("Code", &cur, l, info);

        l++;

        /* Base, si elle existe */

        _Bulletin_("Base", &cur,  l, info);
        l++;

        /* Taux, s'il existe */
        _Bulletin_("Taux", &cur, l, info);
        l++;

        /* Nombre d'unités, s'il existe */
        _Bulletin_("NbUnite", &cur, l, info);
        l++;

        /* Montant , obligatoire */
        cur = atteindreNoeud("Mt", cur);

        Bulletin_("Mt", &cur, l, info);
        l++;
        nbLignePaye++;

        REMONTER_UN_NIVEAU

        // Lorsque on a épuisé tous les types licites on a nécessairement cur = NULL
    }

#undef ligne_l

    return nbLignePaye;
}

#define _BULLETIN(X) _Bulletin(#X, &cur, X, info);

#define BULLETIN_(X)  Bulletin_(#X, &cur, X, info);

#define _BULLETIN_(X)  _Bulletin_(#X, &cur, X, info);

static uint64_t  parseBulletin(xmlNodePtr cur, info_t* info)
{
    DEBUG("Parsage")
    int ligne = 0;

    if (cur == NULL) return 0;
    cur = atteindreNoeud("Agent", cur);

    if (cur == NULL)
    {
        fprintf(stderr, "%s\n", "Impossible d'atteindre \"Agent\"");
        return 0;
    }

    DESCENDRE_UN_NIVEAU

    cur = (cur)? cur->next : NULL;
    /* passer à la balise adjacente après lecture */
    info->drapeau_cont = true;
    _BULLETIN(Nom)

#ifdef TOLERANT
    cur = cur->parent;
    cur = cur->xmlChildrenNode;
    xmlNodePtr cur_save = cur;

    cur = atteindreNoeud("Prenom", cur);

#endif
    _BULLETIN(Prenom)
    _BULLETIN(Matricule)
    _BULLETIN(NIR)

#ifdef TOLERANT
    cur = cur_save;
    cur = atteindreNoeud("Statut", cur);
#else
    cur = cur->next;
    cur = cur->next;
#endif
    _BULLETIN(Statut)
    cur = atteindreNoeud("EmploiMetier", cur);
#ifdef TOLERANT
    cur = cur_save;
    cur = atteindreNoeud("EmploiMetier", cur);
#endif
    _BULLETIN(EmploiMetier)

#ifdef TOLERANT
    cur = cur_save;
    cur = atteindreNoeud("Grade", cur);
#endif
    _BULLETIN(Grade)
#ifdef TOLERANT
    cur = cur_save;
    cur = atteindreNoeud("Indice", cur);
#else
    cur = cur->next;
#endif
    info->drapeau_cont = false; /* ne pas lire la balise adjacente : fin du niveau subordonné Agent*/
    _BULLETIN(Indice)

    REMONTER_UN_NIVEAU

    info->drapeau_cont = true;

#ifdef TOLERANT
    cur = atteindreNoeud("Service", cur);
#else
    while (cur && xmlStrcmp(cur->name, (const xmlChar*)"Service")) cur = cur->next;
#endif
    _BULLETIN(Service)
#ifdef TOLERANT
    cur = cur->parent;
    cur = cur->xmlChildrenNode;
    cur_save = cur;
    cur = atteindreNoeud("NBI", cur);
#endif
    _BULLETIN(NBI)

#ifdef TOLERANT
    cur = cur_save;
    cur = atteindreNoeud("QuotiteTrav", cur);
#endif

    /* obligatoire, substitution du séparateur décimal */
    BULLETIN_(QuotiteTrav)

#ifdef TOLERANT
    cur = atteindreNoeud("Remuneration", cur);
#else
    cur = cur->next;
#endif
    if (cur)
    {
        xmlNodePtr cur_save = cur;

        if (xmlChildElementCount(cur))
        {
            DESCENDRE_UN_NIVEAU
            ligne = lignePaye(cur, info);
        }
        else
        {
            // Rémuneration tag vide
            ligne = 1 ;
            for (int k=0; k < 6; k++) info->Table[info->NCumAgentXml][info->minimum_memoire_p_ligne + k]=(xmlChar*) strdup(NA_STRING);
        }
        cur = cur_save->next;
    }
    else
    {
        perror("Rémunération introuvable.");
        exit(-4);
    }

    /* non obligatoire , substitution du sparateur décimal */

    _BULLETIN_(NbHeureTotal)
    cur = atteindreNoeud("NbHeureSup", cur);

    /* obligatoire, substitution du sparateur décimal */
    BULLETIN_(NbHeureSup)
    BULLETIN_(MtBrut)
    BULLETIN_(MtNet)

    info->drapeau_cont=false; // fin du niveau PayeIndivMensuel
    BULLETIN_(MtNetAPayer)

    return ligne;
}

/* agent_total est une variable de contrôle pour info->NCumAgent */

static void parseFile(info_t* info)
{
    xmlDocPtr doc;
    xmlNodePtr cur = NULL;
    info->NAgent[info->fichier_courant] = 0;
    xmlChar *annee_fichier = NULL, *mois_fichier = NULL;

    doc = xmlParseFile(info->threads->argv[info->fichier_courant]);

    if (doc == NULL) return;

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        fprintf(stderr,"document vide\n");
        xmlFreeDoc(doc);
    }

    DESCENDRE_UN_NIVEAU

    cur = atteindreNoeud("Annee", cur);

    if (cur != NULL)
    {
        annee_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        cur = (cur)? cur->next : NULL;
    }
    else
    {
        fprintf(stderr, "%s\n", "Année non détectable");
        exit(-502);
    }

    if (cur != NULL)
    {
        mois_fichier = xmlGetProp(cur, (const xmlChar *) "V");
    }
    else
    {
        fprintf(stderr, "%s\n", "Mois non détectable");
        exit(-503);
    }

    cur = atteindreNoeud("DonneesIndiv", cur);

    while(! xmlStrcmp(cur->name, (const xmlChar*) "DonneesIndiv"))
    {
        xmlNodePtr cur_save = cur;

        DESCENDRE_UN_NIVEAU

        while(cur != NULL)
        {
            cur = atteindreNoeud("PayeIndivMensuel", cur);
            xmlNodePtr cur_save =  cur;

            DESCENDRE_UN_NIVEAU

            info->Table[info->NCumAgentXml][Annee] = xmlStrdup(annee_fichier);
            info->Table[info->NCumAgentXml][Mois]  = xmlStrdup(mois_fichier);

            int32_t ligne_p = parseBulletin(cur, info);
            info->drapeau_cont = true;

            if (info->reduire_consommation_memoire)
            {
                if (ligne_p != info->NLigne[info->NCumAgentXml] && info->chemin_log == NULL)
                {
                    fprintf(stderr, "Incohérence des décomptes de lignes entre le contrôle C : %d et l'analyse Libxml2 : %d\nPour l'agent %s, Année %s Mois %s\n",
                            info->NLigne[info->NCumAgentXml],
                            ligne_p,
                            info->Table[info->NCumAgentXml][Matricule],
                            info->Table[info->NCumAgentXml][Annee],
                            info->Table[info->NCumAgentXml][Mois]);
                    exit(-1278);
                }
            }
            else
                info->NLigne[info->NCumAgentXml] = ligne_p;

            info->nbLigne += ligne_p;

            if (info->chemin_log)
            {
                FILE* log=fopen(info->chemin_log, "a+");
                int diff = info->NLigne[info->NCumAgentXml]-ligne_p;
                fprintf(log, "Année %s | Mois %2s | Matricule %6s | Rang global %6d | Rang dans fichier %5d | Analyseur C : N.ligne %6d | Xml : N.ligne %6d | Différence %4d\n",
                            info->Table[info->NCumAgentXml][Annee],
                            info->Table[info->NCumAgentXml][Mois],
                            info->Table[info->NCumAgentXml][Matricule],
                            info->NCumAgentXml,
                            info->NAgent[info->fichier_courant],
                            info->NLigne[info->NCumAgentXml],
                            ligne_p,
                            diff);

                fclose(log);
            }
            // Ici il est normal que cur = NULL

            cur = cur_save->next;

            AFFICHER_NOEUD(cur->name)

            info->NAgent[info->fichier_courant]++;
            info->NCumAgentXml++;
        }

        cur = cur_save->next;
    }

    xmlFree(mois_fichier);
    xmlFree(annee_fichier);
    fprintf(stderr, "Fichier n°%d:\nPopulation du fichier  %s :\n %4d bulletins    Total : %4d bulletins  %4" PRIu64 " lignes cumulées.\n",
           info->fichier_courant,
           info->threads->argv[info->fichier_courant],
           info->NAgent[info->fichier_courant],
           info->NCumAgentXml,
           info->nbLigne);


     xmlFreeDoc(doc);
}


void* decoder_fichier(void* tinfo)
{
    /* environ 6000 bulletins par seconde en processus sumple, et 15000 en multithread ; rajoute 1/3 du temps */
    info_t* info = (info_t*) tinfo;

    xmlKeepBlanksDefault(0);

    if (info->reduire_consommation_memoire)
    {
        int err = calculer_memoire_requise(info);
        if (err)
        {
            perror("Calcul de la mémoire requise");
            exit(-1001);
        }
    }
    else
    {
        info->NCumAgent = info->nbAgentUtilisateur * info->threads->argc;
        info->NLigne = (uint16_t*) calloc(info->threads->argc, info->nbAgentUtilisateur* sizeof(uint16_t));
    }

    info->NAgent = (uint32_t*)  calloc(info->threads->argc, sizeof(int32_t));
    info->Table  = (xmlChar***) malloc(info->NCumAgent *  sizeof(xmlChar**));

    if (info->Table == NULL)
    {
        perror("Mémoire insuffisante");
        exit(-18);
    }

    for (unsigned agent = 0; agent < info->NCumAgent; agent++)
    {
        info->Table[agent] = (xmlChar**) calloc(((info->reduire_consommation_memoire)?
                                                  info->minimum_memoire_p_ligne + nbType + (info->NLigne[agent])*6
                                                : info->minimum_memoire_p_ligne + nbType + info->nbLigneUtilisateur*6), sizeof(xmlChar*));
        if (info->Table[agent] == NULL)
        {
            perror("Erreur d'allocation de drapeau I.");
            exit(-63);
        }
    }

    for (unsigned i = 0; i < info->threads->argc ; i++)
    {
        info->fichier_courant = i;

        parseFile(info);
    }

     #ifdef __cplusplus
     #ifndef NO_REGEXP
     #define VAR(X) info->Table[agent][X]

                  // g++-4.8.2 : le compilateur GNU n'implémente pas encore les groupements [...]. Utilisation de parenthèses à la place
                  // Cette foncitionnalité coûte, en -j 4, environ 800 ms/M lignes de paie.

     // attention, pas info<-NCumAgent ici
     #if !defined __WIN32__ && !defined GCC_4_8
     regex pat {info->expression_reg_elus,  regex_constants::icase};
     for (unsigned agent = 0; agent < info->NCumAgentXml; agent++)
        {
                  if (regex_match((const char*) VAR(EmploiMetier), pat) || regex_match((const char*) VAR(Service), pat))
                      VAR(Statut) = (xmlChar*) xmlStrdup((const xmlChar*)"ELU");
        }
     #else
      regex pat1  {".*V.*PRESIDENT.*",  regex_constants::icase};
      regex pat2  {".*CONS.*COMMUN.*",  regex_constants::icase};
      regex pat3  {".*ADJ.*MAIRE.*",  regex_constants::icase};
      regex pat4  {"ELUS?",  regex_constants::icase};
      regex pat5  {".*CONS.*MUNI.*",  regex_constants::icase};


     for (unsigned agent = 0; agent < info->NCumAgentXml; agent++)
        {
                  if (regex_match((const char*) VAR(EmploiMetier), pat1) || regex_match((const char*) VAR(Service), pat1) ||
                      regex_match((const char*) VAR(EmploiMetier), pat2) || regex_match((const char*) VAR(Service), pat2) ||
                      regex_match((const char*) VAR(EmploiMetier), pat3) || regex_match((const char*) VAR(Service), pat3) ||
                      regex_match((const char*) VAR(EmploiMetier), pat4) || regex_match((const char*) VAR(Service), pat4) ||
                      regex_match((const char*) VAR(EmploiMetier), pat5) || regex_match((const char*) VAR(Service), pat5) ||
                      !strcasecmp((const char*) VAR(EmploiMetier), "PRESIDENT") || !strcasecmp((const char*) VAR(EmploiMetier), "MAIRE"))
                      VAR(Statut) = (xmlChar*) xmlStrdup((const xmlChar*)"ELU");
        }

    #endif
    #undef VAR
    #endif // __cplusplus
    #endif // REGEXP
    return NULL;
}
//
//#ifdef __cplusplus
//}
//#endif

