/*  Programme �crit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est �dit�, le pr�sent code doit �tre converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'�tre compil�.
 *  En entr�e d'Altair pr�ciser encodage.entr�e en conformit� avec l'encodage du pr�sent fichier, qui sera celui de la base g�n�r�e.
 */

/* Constantes de compilation pouvant �tre red�finies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, NO_REGEXP */

#include <iomanip>
#include <iostream>
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

    #ifdef STRICT

      if (cur == nullptr)
      {
          std::cerr << "Erreur : Impossible d'atteindre " << cur->name << std::endl;
          exit(-1);
      }

    #endif

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
        std::cerr << "\n\
                En exc�s du nombre de lignes de paye autoris� (" << info.nbLigneUtilisateur << ").\n\
                Omettre -n ... et utiliser -L fichier_log pour d�tecter le maximum de lignes de paye dans les fichiers.\n\
                Utiliser -N ce_maximum ou bien recompiler en augmentant MAX_LIGNES_PAYE, � condition de disposer d'assez de m�moire.\n";

        exit(-10);
    }
}

/* obligatoire */

#define ligne_l  info.Table[info.NCumAgentXml][l]

/* Remplace les occurrences d'un caract�re s�parateur � l'int�rieur d'un champ par le caract�re '_' qui ne doit donc jamais
   �tre s�parateur de champ (c'est bien rare !) */

static inline bool GCC_INLINE Bulletin(const char* tag, xmlNodePtr* cur, int l, info_t& info)
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

static inline void GCC_INLINE _Bulletin(const char* tag, xmlNodePtr* cur,  int l,  info_t& info)
{
    if (! Bulletin(tag, cur, l, info))
    {
        if (*cur)
        {
            std::cerr << "Erreur : Trouv� " << (*cur)->name << " au lieu de " << tag << std::endl;
            std::cerr << "Erreur : dans le fichier " << info.threads->argv[info.fichier_courant] << " \n  pour le matricule "
                      << info.Table[info.NCumAgentXml][Matricule] << std::endl;
            #ifdef STRICT
              exit(-1);
            #endif

        }
        else
        {
            std::cerr << "Erreur : Noeud courant null au stade de la v�rification de "
                      << tag
                      << ", fichier " <<  info.threads->argv[info.fichier_courant] << std::endl;

            for (int l=0; l < Service; ++l)
                std::cerr << "info.Table[" << info.NCumAgentXml << "][" << l << "]=" << info.Table[info.NCumAgentXml][l] << std::endl;

            #ifdef STRICT
              exit(-1);
            #endif
        }
    }
}

/* A tester : la substitution du caract�re d�cimal , au . de la locale anglaise utilis� par X�m�lios (h�las)
   reste n�cessaire tant que nous utiliserons un stockage uniforme en cha�nes de caract�res.
   Si un jour nous d�cidons d'utilisr strold pour convertir les cha�nes de caract�re num�riques en float, nous
   gagnerons de la place en stockage temporaire (peut �tre utile pour les gros fichiers) et alors printf et setlocale
   feront le travail de substitution de la virgule au point lors de l'�criture de la base.
   A ce stade nous stockons tous les champs lus en char, pour �criture identique en .csv dans la table, avec substition
   'manuelle' de la virgule au point dans la cha�ne en output. */

static inline void GCC_INLINE substituer_separateur_decimal(xmlChar* ligne, const char decimal)
{
    for (int i = 0; i < xmlStrlen(ligne); ++i)
        if (ligne[i] == '.') ligne[i] = decimal;
}

/* optionnel */

static inline void GCC_INLINE _Bulletin_(const char* tag, xmlNodePtr* cur,  int l, info_t& info)
{
    if (! Bulletin(tag, cur, l,  info))
    {
        ligne_l = (xmlChar*) xmlStrdup(NA_STRING);
    }
    else
    if (info.decimal != '.') substituer_separateur_decimal(ligne_l, info.decimal);

}

/* obligatoire et avec substitution s�parateur d�cimal */

static inline void GCC_INLINE Bulletin_(const char* tag, xmlNodePtr* cur, int l, info_t& info)
{
    _Bulletin(tag, cur, l, info) ;
    if (info.decimal != '.')  substituer_separateur_decimal(ligne_l, info.decimal);

}

static inline int lignePaye(xmlNodePtr cur, info_t& info)
{
    int l = info.minimum_memoire_p_ligne;

    int nbLignePaye = 0;
    unsigned int t = 0;

    ligne_l = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour �viter la confusion avec \0 des chaines vides
    ++l;

    /* Besoins en m�moire : 18 [champs hors ligne] + nombre de lignes + flags (maximum nbType) */
    bool rembobiner = false;

    while (cur != nullptr)
    {
        bool new_type = false;

        while (xmlStrcmp(cur->name, (const xmlChar *) type_remuneration[t]))
        {
			++t;
            if (t == nbType)
            {
                /* En principe les �l�ments constitutifs des enregistrements <Remun�ration>....</Remuneration> sont enregistr�s
                   dans l'ordre du tableau type_remuneration. Toutefois quelques cas de d�sordre sont observ�s. Dans ces cas l� on peut
                   "rembobiner le tableau". On �vite toutefois de faire une recherche ensembliste syst�matique, qui �viterait cela mais
                   freinerait 99,9 % des recherches */

                if (rembobiner == false)
                {
                    rembobiner = true;
                    t = 0;
                    continue;
                }

                /* On ne rembobine qu'une seule fois. Si l'essai �choue, on d�clenche une exception */

                std::cerr << "Erreur : En exc�s du nombre de types de lignes de paye autoris� (" << nbType << ").\n";
                if (cur)
                    std::cerr << "Erreur : Type litigieux " << cur->name << " aux alentours du matricule " << info.Table[info.NCumAgentXml][Matricule] << std::endl;
                else
                    std::cerr << "Erreur : Pointeur noeud courant nul" << std::endl;

                exit(-11);
            }

            new_type = true;
        }

        rembobiner = false;

        if (new_type && t < nbType)
        {
            ligne_l = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour �viter la confusion avec \0 des chaines vides
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
        /* Libell�, obligatoire */

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

        /* Nombre d'unit�s, s'il existe */
        _Bulletin_("NbUnite", &cur, l, info);
		++l;

        /* Montant , obligatoire */
        cur = atteindreNoeud("Mt", cur);

        Bulletin_("Mt", &cur, l, info);
		++l;
        ++nbLignePaye;

        REMONTER_UN_NIVEAU

        // Lorsque on a �puis� tous les types licites on a n�cessairement cur = nullptr
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
        std::cerr << "Erreur : Impossible d'atteindre \"Agent\"\n";
        return 0;
    }

    DESCENDRE_UN_NIVEAU

    /* dans certains sxh�mas on peut ne pas avoir la civilit� */
    cur = atteindreNoeud("Nom", cur);

    /* passer � la balise adjacente apr�s lecture */
    info.drapeau_cont = true;
    _BULLETIN(Nom)

#ifdef TOLERANT
    cur = cur->parent;
    cur = cur->xmlChildrenNode;
    xmlNodePtr cur_save = cur;

#endif
    /* Dans certains cas on peut avoir un compl�ment de nom */
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
    /* dans certains sch�mas on peut avoir ici des balises */

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
    info.drapeau_cont = false; /* ne pas lire la balise adjacente : fin du niveau subordonn� Agent*/
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

    BULLETIN_(NBI)

#ifdef TOLERANT
    cur = cur_save;
    cur = atteindreNoeud("QuotiteTrav", cur);
#else
    cur = cur->next;
    cur = cur->next;
#endif

    /* obligatoire, substitution du s�parateur d�cimal */
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
        perror("Erreur : R�mun�ration introuvable.");
        exit(-4);
    }

    /* non obligatoire , substitution du sparateur d�cimal */

    _BULLETIN_(NbHeureTotal)
    cur = atteindreNoeud("NbHeureSup", cur);

    /* obligatoire, substitution du sparateur d�cimal */
    BULLETIN_(NbHeureSup)
    BULLETIN_(MtBrut)
    BULLETIN_(MtNet)

    info.drapeau_cont=false; // fin du niveau PayeIndivMensuel
    BULLETIN_(MtNetAPayer)

    // R�muneration tag vide
    if (ligne == 0) ligne = 1 ;

    return ligne;
}

/* agent_total est une variable de contr�le pour info->NCumAgent */

static void parseFile(info_t& info)
{
    std::ofstream log;
    xmlDocPtr doc;
    xmlNodePtr cur = nullptr;
    info.NAgent[info.fichier_courant] = 0;
    xmlChar *annee_fichier = nullptr,
            *mois_fichier = nullptr, 
            *etab_fichier = nullptr,
            *siret_fichier = nullptr,
            *budget_fichier = nullptr;

    doc = xmlParseFile(info.threads->argv[info.fichier_courant]);

    if (doc == nullptr) return;

    if (! info.chemin_log.empty())
       {
         log.open(info.chemin_log, std::ios::app);
       }

    cur = xmlDocGetRootElement(doc);

    if (cur == nullptr)
    {
        std::cerr << "Erreur : document vide\n";
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
        std::cerr << "Erreur : Ann�e non d�tectable\n";
        exit(-502);
    }

    if (cur != nullptr)
    {
        mois_fichier = xmlGetProp(cur, (const xmlChar *) "V");
    }
    else
    {
        std::cerr << "Erreur : Mois non d�tectable\n";
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
        std::cerr << "Erreur : Budget non d�tectable\n";
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
                std::cerr << "Erreur : Etablissement/Employeur non d�tectable\n";
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
                std::cerr  << "\nErreur : Siret non d�tectable\n";
                std::cerr << "Ann�e " << info.Table[info.NCumAgentXml][Annee]
                          << "Mois "  << info.Table[info.NCumAgentXml][Mois];
                exit(-506);
            }
            cur=cur_save2;

        while(cur != nullptr)
        {

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
                    std::cerr << "Erreur : Incoh�rence des d�comptes de lignes entre le contr�le C : "
                              << info.NLigne[info.NCumAgentXml]
                              << "et l'analyse Libxml2 : "
                              << ligne_p
                              << "\nPour l'agent "
                              << info.Table[info.NCumAgentXml][Matricule]
                              << " Ann�e "
                              << info.Table[info.NCumAgentXml][Annee]
                              << " Mois "
                              << info.Table[info.NCumAgentXml][Mois];

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
                log << "Ann�e " << P
                    << info.Table[info.NCumAgentXml][Annee] << P
                    << "Mois "  << std::setw(2) << info.Table[info.NCumAgentXml][Mois] << P
                    << "Matricule " << std::setw(6) <<  info.Table[info.NCumAgentXml][Matricule] << P
                    << "Rang global " << std::setw(6) <<  info.NCumAgentXml << P
                    << "Rang dans fichier " << std::setw(5) <<  info.NAgent[info.fichier_courant] << P
                    << "Analyseur C : " << std::setw(6) << info.NLigne[info.NCumAgentXml] << P
                    << "Xml : " << std::setw(6) << ligne_p << P
                    << "Diff�rence " << std::setw(4) << diff << std::endl;
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

    std::cerr << "[MSG] Fichier n�" << info.fichier_courant + 1 << " :\n[MSG] Population du fichier  " <<  info.threads->argv[info.fichier_courant] << ":\n[MSG] "
              <<  info.NAgent[info.fichier_courant] << " bulletins    Total : " <<  info.NCumAgentXml << "bulletins  " << info.nbLigne <<" lignes cumul�es.\n";


    xmlFreeDoc(doc);
    if (log.is_open())
       log.close();

}

 /* Les expressions r�guli�res correctes ne sont disponibles sur MINGW GCC qu'� partir du build 4.9.2 */

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
#include <mutex>

using namespace std;
#else
#error "C++11 doit �tre utilis�."
#endif


#endif // defined

extern int rankNumber;
extern std::ofstream rankFile;
extern char* rankFilePath;
std::mutex mut;

void* decoder_fichier(info_t& info)
{
    /* environ 6000 bulletins par seconde en processus sumple, et 15000 en multithread ; rajoute 1/3 du temps */

    int temp_rank = 1;

    #if  defined GCC_REGEX //&& !defined __WIN32__ && !defined GCC_4_8

     regex pat {"�lu"/*info->expression_reg_elus*/,  regex_constants::icase};
     regex pat2 {EXPRESSION_REG_VACATIONS, regex_constants::icase};
     regex pat3 {EXPRESSION_REG_ASSISTANTES_MATERNELLES, regex_constants::icase};

    #endif

    xmlKeepBlanksDefault(0);

    if (info.reduire_consommation_memoire)
    {
        int err = calculer_memoire_requise(info);
        if (err)
        {
            perror("Erreur : Calcul de la m�moire requise");
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
            perror("Erreur : Probl�me d'allocation m�moire de info.NLigne");
            exit(1003);
        }
    }

    info.NAgent = new uint32_t[info.threads->argc];
    info.Table  = new xmlChar**[info.NCumAgent];

    if (info.Table == nullptr)
    {
        perror("Erreur : M�moire insuffisante");
        exit(-18);
    }

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
    {
        info.Table[agent] = (xmlChar**) calloc(info.minimum_memoire_p_ligne + nbType + (info.NLigne[agent])*6, sizeof(xmlChar*));
        if (info.Table[agent] == nullptr)
        {
            std::cerr <<  "Erreur : Erreur d'allocation de drapeau I. pour l'agent "
                      <<  agent
                      <<  "et pour "
                      <<  info.minimum_memoire_p_ligne + nbType + (info.NLigne[agent]) * 6
                      <<  " B\n";

            exit(-63);
        }
    }

    for (unsigned i = 0; i < info.threads->argc ; ++i)
    {
        info.fichier_courant = i;
        parseFile(info);
        if (mut.try_lock())
        {
            rankNumber += temp_rank;
            temp_rank = 1;
            rankFile.open(rankFilePath, std::ios::out|std::ios::trunc);
            rankFile << rankNumber ;
            rankFile.close();
        }
        else
            ++temp_rank;

        mut.unlock();
    }

    if (info.NCumAgentXml != info.NCumAgent)
    {
        std::cerr << "Erreur : Incoh�rence de l'allocation m�moire ex-ante " << info.NCumAgent
                  << " unit�s et ex-post " <<  info.NCumAgentXml << " unit�s d'information.\n"
                  << "Sortie pour �viter une erreur de segmentation.\n";
        std::cerr << "Fichier : " << info.threads->argv[info.fichier_courant] << std::endl;

        exit(1005);
    }


    // attention, pas info<-NCumAgent ici

    /* Le champ statut est modifi� comme suit :
        ELU   pour un �lu
        V     pour un vacataire
        A     pour une assistante maternelle */

#define VAR(X) info.Table[agent][X]
    for (unsigned agent = 0; agent < info.NCumAgentXml; ++agent)
    {
        /* Les �lus peuvent �tre identifi�s soit dans le service soit dans l'emploi m�tier */

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

        /* les vacations peuvent �tre indiqu�es comme telles dans les libell�s de paie mais pas dans les emplois m�tiers.
           On les r�cup�re en parcourant les libell�s */

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

