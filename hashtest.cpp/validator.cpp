/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */

/* Constantes de compilation pouvant être redéfinies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, MAX_MEMOIRE_RESERVEE */


#ifdef __cplusplus
extern "C" {
#endif
#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "table.hpp"

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

static inline void  verifier_taille(const int l)
{
    if (l == (MAX_LIGNES_PAYE - 1)*6)
    {
        fprintf(stderr, "En excès du nombre de lignes de paye autorisé (%d)\n", MAX_LIGNES_PAYE);
        exit(-10);
    }
}

/* obligatoire */

#define ligne_l  info->Table[info->NCumAgentLibxml2][l]

static inline bool Bulletin(const char* tag, xmlNodePtr* cur, int l, info_t* info)
{
    bool test = (cur != NULL && *cur != NULL && (! xmlStrcmp((*cur)->name,  (const xmlChar*) tag)));

    if (test)
    {
        ligne_l = xmlGetProp(*cur, (const xmlChar *) "V");

        if (ligne_l == NULL)
            ligne_l = (xmlChar*) xmlStrdup(NA_STRING);

        /* sanitisation */

        else
          for (int i = 0; i < xmlStrlen(ligne_l); i++)
              if (ligne_l[i] == info->separateur)
                  ligne_l[i] = '_';

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
            fprintf(stderr, "Noeud courant null au stade de la vérification de %s\n", tag);

        exit(-32);
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
        ligne_l = (xmlChar*) xmlStrdup(NA_STRING);
        return;
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
                                     info->Table[info->NCumAgentLibxml2][Matricule]);
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

       if (! info->reduire_consommation_memoire) {verifier_taille(l); }

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

        nbLignePaye++;
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
    _BULLETIN(Nom)

    cur = cur->parent;
    cur = cur->xmlChildrenNode;
    xmlNodePtr cur_save = cur;
    cur = atteindreNoeud("Prenom", cur);

    _BULLETIN(Prenom)
    _BULLETIN(Matricule)
    _BULLETIN(NIR)

    cur = cur_save;
    cur = atteindreNoeud("Statut", cur);
    _BULLETIN(Statut)

    cur = cur_save;
    cur = atteindreNoeud("EmploiMetier", cur);
    _BULLETIN(EmploiMetier)

    cur = cur_save;
    cur = atteindreNoeud("Grade", cur);
    _BULLETIN(Grade)

    cur = cur_save;
    cur = atteindreNoeud("Indice", cur);
    _BULLETIN(Indice)

    REMONTER_UN_NIVEAU

    cur = atteindreNoeud("Service", cur);
    _BULLETIN(Service)

    cur = cur->parent;
    cur = cur->xmlChildrenNode;
    cur_save = cur;
    cur = atteindreNoeud("NBI", cur);

    _BULLETIN(NBI)

    cur = cur_save;
    cur = atteindreNoeud("QuotiteTrav", cur);

    /* obligatoire, substitution du sparateur décimal */
    BULLETIN_(QuotiteTrav)

    cur = atteindreNoeud("Remuneration", cur);

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
            for (int k=0; k < 6; k++) info->Table[info->NCumAgentLibxml2][info->minimum_memoire_p_ligne + k]=(xmlChar*) xmlStrdup(NA_STRING);
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
    BULLETIN_(MtNetAPayer)

    return ligne;
}

/* agent_total est une variable de contrôle pour info->NCumAgent */

static void parseFile(info_t* info)
{
    xmlDocPtr doc;
    xmlNodePtr cur = NULL;
    uint16_t agent_du_fichier = 0;
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

            int32_t ligne_p=parseBulletin(cur, info);

            info->Table[info->NCumAgentLibxml2][Annee] = xmlStrdup(annee_fichier);
            info->Table[info->NCumAgentLibxml2][Mois]  = xmlStrdup(mois_fichier);

            if (info->reduire_consommation_memoire)
            {
                if (ligne_p != info->NLigne[info->NCumAgentLibxml2] && info->chemin_log == NULL)
                {
                    fprintf(stderr, "Incohérence des décomptes de lignes entre le contrôle C : %d et l'analyse Libxml2 : %d\nPour l'agent %s, Année %s Mois %s\n",
                            info->NLigne[info->NCumAgentLibxml2],
                            ligne_p,
                            info->Table[info->NCumAgentLibxml2][Matricule],
                            info->Table[info->NCumAgentLibxml2][Annee],
                            info->Table[info->NCumAgentLibxml2][Mois]);
                    exit(-1278);
                }
            }
            else info->NLigne[info->NCumAgentLibxml2] = ligne_p;

            info->nbLigne += ligne_p;

            if (info->chemin_log)
            {
                FILE* log=fopen(info->chemin_log, "a+");
                int diff = info->NLigne[info->NCumAgentLibxml2]-ligne_p;
                if (!diff)
                {
                    fprintf(log, "Année %s Mois %s Matricule %s Rang global agent %d Rang agent du fichier %d Analyseur C, ligne n° %d Analyseur Xml, ligne n° %d Différence %d\n",
                            info->Table[info->NCumAgentLibxml2][Annee],
                            info->Table[info->NCumAgentLibxml2][Mois],
                            info->Table[info->NCumAgentLibxml2][Matricule],
                            info->NCumAgentLibxml2,
                            agent_du_fichier,
                            info->NLigne[info->NCumAgentLibxml2],
                            ligne_p,
                            diff);
                }

                fclose(log);
            }
            // Ici il est normal que cur = NULL

            cur = cur_save->next;

            AFFICHER_NOEUD(cur->name)

            agent_du_fichier++;
            info->NCumAgentLibxml2++;
        }
        cur = cur_save->next;
    }

    xmlFree(mois_fichier);
    xmlFree(annee_fichier);
    printf("Fichier n°%d:\nPopulation du fichier  %s :\n %4d bulletins    Total : %4d bulletins  %4" PRIu64 " lignes cumulées.\n",
           info->fichier_courant,
           info->threads->argv[info->fichier_courant],
           agent_du_fichier, info->NCumAgentLibxml2, info->nbLigne);


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
        info->NAgent = (int32_t*) calloc(info->threads->argc, sizeof(int32_t));
    }


    info->Table = (xmlChar***) malloc(info->NCumAgent *  sizeof(xmlChar**));

    if (info->Table == NULL)
    {
        perror("Mémoire insuffisante");
        exit(-18);
    }

    for (unsigned agent = 0; agent < info->NCumAgent; agent++)
    {
        info->Table[agent] = (xmlChar**) calloc(((info->reduire_consommation_memoire)?
                                                info->minimum_memoire_p_ligne + nbType + (info->NLigne[agent])*6
                                                : info->minimum_memoire_p_ligne + nbType + MAX_LIGNES_PAYE*6), sizeof(xmlChar*));
        if (info->Table[agent] == NULL)
        {
            perror("Erreur d'allocation de drapeau I.");
            exit(-63);
        }
    }

    for (int i = 0; i < info->threads->argc ; i++)
    {
        info->fichier_courant = i;

        parseFile(info);
    }

    return NULL;
}

#ifdef __cplusplus
}
#endif

