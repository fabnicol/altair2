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

extern std::mutex mut;

static inline xmlNodePtr GCC_INLINE atteindreNoeud(const xmlChar * noeud, xmlNodePtr cur, bool opt = true, int normalJump = 0)
{
   #if 0
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
    }
   #endif

    for (int i = 0; i < normalJump; ++i)
           cur = cur->next;

    while (cur != nullptr && xmlStrcmp(cur->name,  noeud))
    {
           cur = cur->next;
    }

      if (cur == nullptr)
      {
          AFFICHER_NOEUD(noeud)  // cur->name == oeud

          if (opt)
             std::cerr << "Erreur : Impossible d'atteindre " << noeud << "\n";
          //mut.unlock();
          #ifdef STRICT
            exit(-1);
          #endif

      }

     return cur;  // soit un pointer vers le bon noeud, soit nullptr
}

static inline xmlNodePtr GCC_INLINE atteindreNoeud(const char* noeud, xmlNodePtr cur, bool  opt = true, int normalJump = 0)
  { 
    return atteindreNoeud(reinterpret_cast<const xmlChar*>(noeud),  cur, opt, normalJump);
  }

static inline xmlNodePtr GCC_INLINE atteindreNoeudArret(const char* noeud, xmlNodePtr cur, const char* arret)
{
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
    }

    while (cur != nullptr)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) noeud))  // cur->name != noeud
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


static inline void GCC_INLINE  verifier_taille(const int nbLignePaye, info_t& info)
{
    if (nbLignePaye >= info.nbLigneUtilisateur)
    {
        std::cerr << "\n\
                En excès du nombre de lignes de paye autorisé (" << info.nbLigneUtilisateur << ").\n\
                Omettre -n ... et utiliser -L fichier_log pour détecter le maximum de lignes de paye dans les fichiers.\n\
                Utiliser -N ce_maximum ou bien recompiler en augmentant MAX_LIGNES_PAYE, à condition de disposer d'assez de mémoire.\n";

        exit(-10);
    }
}

/* obligatoire */

#define ligne_l  info.Table[info.NCumAgentXml][l]


/* utilité d'affichage de l'environnement xhl en cas de problème de conformité des données */

static  void afficher_environnement_xhl(const info_t& info)
{

    if (mut.try_lock())
    {
        std::cerr << "\nFichier   " <<  info.threads->argv[info.fichier_courant] << "\n"
                  << "Matricule   "    << info.Table[info.NCumAgentXml][Matricule] << "\n";
        for (int l=0; l < BESOIN_MEMOIRE_ENTETE; ++l)
          std::cerr << "info.Table[" << info.NCumAgentXml << "][" << l << "]=" << info.Table[info.NCumAgentXml][l] << "\n";
        mut.unlock();
    }
    else
        afficher_environnement_xhl(info);

}

static inline void GCC_INLINE assigner_ligne_l_NA(xmlChar* ligneL)
{
        ligneL =  xmlStrdup(NA_STRING);
}


/* Remplace les occurrences d'un caractère séparateur à l'intérieur d'un champ par le caractère '_' qui ne doit donc jamais
   être séparateur de champ (c'est bien rare !) */

static inline int GCC_INLINE Bulletin(const xmlChar*  tag, xmlNodePtr& cur, int l, info_t& info, bool opt = true, int normalJump = 0)
{
    // attention faire en sorte que cur ne soit JAMAIS nul en entrée ou en sortie

    const xmlNodePtr nextcur = std::move(atteindreNoeud(tag, cur, opt, normalJump));

    if ( nullptr == nextcur)
    {
        return NODE_NOT_FOUND;
    }

// On a à présent la garantie que cur->name correspond à tag

        ligne_l = xmlGetProp(nextcur, (const xmlChar *) "V");
        if (ligne_l == nullptr)
            return LINE_MEMORY_EXCEPTION;

// dans les cas où false est retourné, la référence à cur n'a pas été modifiée et est donc non nulle

        if (info.drapeau_cont)
        {
            if (nextcur->next != nullptr)
                cur = nextcur->next;
            else
                return NO_NEXT_ITEM;  // pour garantir que cur ne devient pas nul.
        }

    /* sanitisation */

        const int size = xmlStrlen(ligne_l);
        for (int i = 0; i < size; ++i)
            if (ligne_l[i] == info.separateur)
                ligne_l[i] = '_'; 

        return NODE_FOUND;

}

/* obligatoire, mais possibilité de fallback si STRICT n'est pas défini */

//             std::cerr << "Erreur : Noeud courant null au stade de la vérification de " << tag << "\n";

static inline bool GCC_INLINE bulletin_obligatoire(const xmlChar* tag, xmlNodePtr& cur, int l,  info_t& info, int normalJump = 0)
{

    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info, normalJump))
    {
        // on sait que cur ne sera jamais nul
        case NODE_FOUND : return true;

        case NODE_NOT_FOUND :
                std::cerr << "Erreur : Impossible d'atteindre " << tag << " à partir de " << cur->name << "\n";
                ligne_l = xmlStrdup(NA_STRING);
                break;

        case LINE_MEMORY_EXCEPTION :
                std::cerr << "Erreur : Allocation mémoire impossible pour la ligne " << l << "\n";
                ligne_l = xmlStrdup(NA_STRING);
                break;

        case NO_NEXT_ITEM :
                std::cerr << "Erreur : Pas d'item successeur pour le noeud " << tag <<  "\n";
                break;

    }

        afficher_environnement_xhl(info) ;

        #ifdef STRICT
          exit(-1);
        #else
          if (nullptr != cur->next)
          {
              cur = cur->next;
              return true;
          }
          else
          return false;
        #endif
}


static inline bool GCC_INLINE bulletin_obligatoire(const char* tag, xmlNodePtr& cur, int l,  info_t& info, int normalJump = 0)
{
    return bulletin_obligatoire(reinterpret_cast<const xmlChar*>(tag), cur, l, info, normalJump);
}

/* A tester : la substitution du caractère décimal , au . de la locale anglaise utilisé par Xémélios (hélas)
   reste nécessaire tant que nous utiliserons un stockage uniforme en chaînes de caractères.
   Si un jour nous décidons d'utilisr strold pour convertir les chaînes de caractère numériques en float, nous
   gagnerons de la place en stockage temporaire (peut être utile pour les gros fichiers) et alors printf et setlocale
   feront le travail de substitution de la virgule au point lors de l'écriture de la base.
   A ce stade nous stockons tous les champs lus en char, pour écriture identique en .csv dans la table, avec substition
   'manuelle' de la virgule au point dans la chaîne en output. */

static inline void GCC_INLINE substituer_separateur_decimal(xmlChar* ligne, const char decimal)
{
    const int size = xmlStrlen(ligne);
    for (int i = 0; i < size; ++i)
        if (ligne[i] == '.') ligne[i] = decimal;
}

/* optionnel */


static inline bool GCC_INLINE bulletin_optionnel_numerique(const xmlChar* tag, xmlNodePtr& cur,  int l, info_t& info, int normalJump = 0)
{
    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info, false, normalJump))
    {
        // on sait que cur ne sera jamais nul
        case NODE_FOUND :
             #ifndef DECIMAL_NON_EN
               if (info.decimal != '.')
             #endif
                substituer_separateur_decimal(ligne_l, info.decimal);
             return true;

        case NODE_NOT_FOUND :
             ligne_l = xmlStrdup(NA_STRING);


             return true;

        case LINE_MEMORY_EXCEPTION :
             std::cerr << "Erreur : Allocation mémoire impossible pour la ligne " << l << "\n";
             ligne_l = xmlStrdup(NA_STRING);
             break;

        case NO_NEXT_ITEM :
             std::cerr << "Erreur : Pas d'item successeur pour le noeud " << tag <<  "\n";
             #ifndef DECIMAL_NON_EN
              if (info.decimal != '.')
             #endif
               substituer_separateur_decimal(ligne_l, info.decimal);
             break;
    }

        afficher_environnement_xhl(info) ;

        #ifdef STRICT
          exit(-1);
        #else
          if (nullptr != cur->next)
          {
              cur = cur->next;
              return true;
          }
          else
          return false;
        #endif

}

static inline bool GCC_INLINE bulletin_optionnel_numerique(const char* tag, xmlNodePtr& cur,  int l, info_t& info, int normalJump = 0)
{
    return bulletin_optionnel_numerique(reinterpret_cast<const xmlChar*>(tag), cur, l, info, normalJump);
}

/* obligatoire et avec substitution séparateur décimal */

static inline bool GCC_INLINE bulletin_obligatoire_numerique(const xmlChar* tag, xmlNodePtr& cur, int l, info_t& info, int normalJump = 0)
{
    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info, normalJump))
    {
        // on sait que cur ne sera jamais nul
        case NODE_FOUND :
            #ifndef DECIMAL_NON_EN
               if (info.decimal != '.')
            #endif
               substituer_separateur_decimal(ligne_l, info.decimal);
             return true;

        case NODE_NOT_FOUND :
             ligne_l = xmlStrdup(NA_STRING);
             return true;

        case LINE_MEMORY_EXCEPTION :
             std::cerr << "Erreur : Allocation mémoire impossible pour la ligne " << l << "\n";
             ligne_l = xmlStrdup(NA_STRING);
             break;

        case NO_NEXT_ITEM :
             std::cerr << "Erreur : Pas d'item successeur pour le noeud " << tag <<  "\n";
             #ifndef DECIMAL_NON_EN
              if (info.decimal != '.')
             #endif
                   substituer_separateur_decimal(ligne_l, info.decimal);
             break;
    }
        afficher_environnement_xhl(info) ;

        #ifdef STRICT
          exit(-1);
        #else
          if (nullptr != cur->next)
          {
              cur = cur->next;
              return true;
          }
          else
          return false;
        #endif
}


static inline bool GCC_INLINE bulletin_obligatoire_numerique(const char* tag, xmlNodePtr& cur,  int l, info_t& info, int normalJump = 0)
{
    return bulletin_obligatoire_numerique(reinterpret_cast<const xmlChar*>(tag), cur, l, info, normalJump);
}

static inline int lignePaye(xmlNodePtr cur, info_t& info)
{
    int l = info.minimum_memoire_p_ligne;

    int nbLignePaye = 0;
    unsigned int t = 0;

    ligne_l = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour éviter la confusion avec \0 des chaines vides
    ++l;

    /* Besoins en mémoire : BESOIN_MEMOIRE_ENTETE [champs hors ligne] + nombre de lignes + flags (maximum nbType) */
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

                std::cerr << "Erreur : En excès du nombre de types de lignes de paye autorisé (" << nbType << ").\n";
                if (cur)
                    std::cerr << "Erreur : Type litigieux " << cur->name << " aux alentours du matricule " << info.Table[info.NCumAgentXml][Matricule] << "\n";
                else
                    std::cerr << "Erreur : Pointeur noeud courant nul" << "\n";

                exit(-11);
            }

            new_type = true;
        }

        rembobiner = false;

        if (new_type && t < nbType)
        {
            ligne_l = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour éviter la confusion avec \0 des chaines vides
            // on ne teste pas ligne_l != nullptr ici mais en principe on devrait
            ++l;
        }

        // ici on pourrait in fine se passer de ce test par compilation séparée

        if (! info.reduire_consommation_memoire)
        {
            verifier_taille(nbLignePaye, info);
        }

        if (! xmlStrcmp(cur->name, (const xmlChar*) "Commentaire"))
        {
            cur = cur->next;
            t=0;
            rembobiner=false; // 'Rembobinage gratuit'
            continue; // garantit incidemment que cur != nullptr dans la boucle
        }

        // cur n'est pas nul à ce point

        cur =  cur->xmlChildrenNode;

        if (cur == nullptr) break;

        // cur n'est pas nul à ce point et ne devient jamais nul ci-après

        /* Libellé, obligatoire */

        if (bulletin_obligatoire("Libelle", cur, l, info)

        /* Code, obligatoire */

           && bulletin_obligatoire("Code", cur, l, info)

        /* Base, si elle existe */

           && bulletin_optionnel_numerique("Base", cur,  l, info)

        /* Taux, s'il existe */

           && bulletin_optionnel_numerique("Taux", cur, l, info)

        /* Nombre d'unités, s'il existe */

           && bulletin_optionnel_numerique("NbUnite", cur, l, info)

        /* Montant, obligatoire */

           && bulletin_obligatoire_numerique("Mt", cur, l, info)) {}

        // cur ne sera pas nul à ce point

		++l;
        ++nbLignePaye;

        cur =  cur->parent->next;
        // le parent ne peut être nul

        // attention si du code est rajouté ici il doit l'être sous garde cur != nullptr
        // Lorsque on a épuisé tous les types licites on a nécessairement cur = nullptr et la boucle s'arrête
    }

#undef ligne_l

    return nbLignePaye;
}

#define BULLETIN_OBLIGATOIRE_(X, normalJump) bulletin_obligatoire(reinterpret_cast<const xmlChar*>(#X), cur, X, info, normalJump)
#define BULLETIN_OBLIGATOIRE(X) BULLETIN_OBLIGATOIRE_(X, 0)

#define BULLETIN_OBLIGATOIRE_NUMERIQUE_(X, normalJump)  bulletin_obligatoire_numerique(reinterpret_cast<const xmlChar*>(#X), cur, X, info, normalJump)
#define BULLETIN_OBLIGATOIRE_NUMERIQUE(X) BULLETIN_OBLIGATOIRE_NUMERIQUE_(X, 0)

#define BULLETIN_OPTIONNEL_NUMERIQUE_(X, normalJump)  bulletin_optionnel_numerique(#X, cur, X, info, normalJump)
#define BULLETIN_OPTIONNEL_NUMERIQUE(X)  BULLETIN_OPTIONNEL_NUMERIQUE_(X, 0)

static uint64_t  parseBulletin(xmlNodePtr cur, info_t& info)
{

    xmlNodePtr cur_parent = nullptr;

    if (cur == nullptr) return 0;

    cur = atteindreNoeud("Agent", cur);

    if (cur == nullptr)
    {
        std::cerr << "Erreur : Impossible d'atteindre \"Agent\"\n";
        return 0;
    }

    // cur n'est pas nul à ce point

    cur_parent = cur;
    cur =  cur->xmlChildrenNode;
    
#ifdef TOLERANT_TAG_HIERARCHY
    xmlNodePtr cur_save = cur;
#endif

    if (cur == nullptr) return 0;

    /* dans certains schémas on peut ne pas avoir la civilité */
    /* passer à la balise adjacente après lecture */

    info.drapeau_cont = true;
    bool result = true;

    /* result va garantir notamment que le pointeur cur filé implicitement est non nul */

    if ((result = (BULLETIN_OBLIGATOIRE(Nom)
        && BULLETIN_OBLIGATOIRE(Prenom)
        && BULLETIN_OBLIGATOIRE(Matricule)
        && BULLETIN_OBLIGATOIRE(NIR)))) {} // no-op

    #ifdef TOLERANT_TAG_HIERARCHY       // on refait le parcours depuis le haut en cas d'ordre inexact des balises
        cur = cur_save;
    #endif

    if ((result = (result                  // garde spécifique
        && BULLETIN_OBLIGATOIRE_(NbEnfants, 1)
        && BULLETIN_OBLIGATOIRE(Statut)
        && BULLETIN_OBLIGATOIRE_(EmploiMetier, 1)))) {} // no-op

    #ifdef TOLERANT_TAG_HIERARCHY
        cur = cur_save;
    #endif

    if ((result = (result && BULLETIN_OBLIGATOIRE(Grade)))) {} // no-op

    #ifdef TOLERANT_TAG_HIERARCHY
        cur = cur_save;
    #endif

    /* ne pas lire la balise adjacente : fin du niveau subordonné Agent*/

    info.drapeau_cont = false;

    if (result && BULLETIN_OBLIGATOIRE_(Indice, 1)) {}

    /* on remonte d'un niveau */

    cur = cur_parent;
    #ifdef TOLERANT_TAG_HIERARCHY
      cur_save = cur;
    #endif
    info.drapeau_cont = true;

    if (cur != nullptr && BULLETIN_OBLIGATOIRE(Service)) {}

#ifdef TOLERANT_TAG_HIERARCHY
    cur = cur_save;
    cur = atteindreNoeud("NBI", cur);
#endif

    BULLETIN_OBLIGATOIRE_NUMERIQUE(NBI);

#ifdef TOLERANT_TAG_HIERARCHY
    cur = cur_save;
    cur = atteindreNoeud("QuotiteTrav", cur);
#endif


    /* obligatoire, substitution du séparateur décimal */
    BULLETIN_OBLIGATOIRE_NUMERIQUE(QuotiteTrav);

#ifdef TOLERANT_TAG_HIERARCHY
    cur = cur_save;
    cur = atteindreNoeud("Remuneration", cur);
#else
    cur = cur->next;
    //cur = atteindreNoeud("Remuneration", cur);

#endif

    int ligne = 0;

    if (cur)
    {
        xmlNodePtr cur_save = cur;

        if ((cur =  cur->xmlChildrenNode) != nullptr)
        {
            ligne = lignePaye(cur, info);
        }

        if (ligne == 0)
        {
            for (int k=0; k < 6; ++k)
                info.Table[info.NCumAgentXml][info.minimum_memoire_p_ligne + k] = (xmlChar*) xmlStrdup(NA_STRING);
        }

        cur = cur_save->next;

#if 0
        if (strcmp((char*) cur->name, "NbHeureTotal"))
        {
            std::cerr << cur->name << "\n";
            afficher_environnement_xhl(info);
            exit(1);
        }
#endif
    }
    else
    {
        perror("Erreur : Rémunération introuvable.");
        exit(-4);
    }

    /* obligatoire , substitution du sparateur décimal */


    BULLETIN_OPTIONNEL_NUMERIQUE(NbHeureTotal);
    //cur = atteindreNoeud("NbHeureSup", cur);

    /* obligatoire, substitution du sparateur décimal */
    BULLETIN_OPTIONNEL_NUMERIQUE_(NbHeureSup, 1);
    BULLETIN_OBLIGATOIRE_NUMERIQUE(MtBrut);
    BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNet);

    info.drapeau_cont=false; // fin du niveau PayeIndivMensuel
    BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNetAPayer);

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
        exit(-502);
    }

    if (cur != nullptr)
    {
        mois_fichier = xmlGetProp(cur, (const xmlChar *) "V");
    }
    else
    {
        std::cerr << "Erreur : Mois non détectable\n";
        exit(-503);
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
        std::cerr << "Erreur : Budget non détectable\n";
        exit(-504);
    }

  while((cur = atteindreNoeudArret("DonneesIndiv", cur, "Nomenclatures")) != nullptr)
  {
        xmlNodePtr cur_save = cur;
        xmlNodePtr cur_save2 = nullptr;

        cur =  cur->xmlChildrenNode;

            cur = atteindreNoeud("Etablissement", cur);
            if (cur == nullptr) cur = atteindreNoeud("Employeur", cur);
            cur_save2 =  cur;
            if (cur_save2 == nullptr) break;

            cur =  cur->xmlChildrenNode;

            cur = atteindreNoeud("Nom", cur);
            if (cur != nullptr)
            {
                etab_fichier = xmlGetProp(cur, (const xmlChar *) "V");
                cur = (cur)? cur->next : nullptr;
            }
            else
            {
                std::cerr << "Erreur : Etablissement/Employeur non détectable\n";
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
                std::cerr  << "Erreur : Siret non détectable\n";
                std::cerr << "Année " << info.Table[info.NCumAgentXml][Annee]
                          << "Mois "  << info.Table[info.NCumAgentXml][Mois];
                exit(-506);
            }
            cur=cur_save2;

        while(cur != nullptr)
        {

            cur = atteindreNoeud("PayeIndivMensuel", cur);

            cur_save2 =  cur;

            if (cur_save2 == nullptr) break;

            cur =  cur->xmlChildrenNode;

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
                    << "Différence " << std::setw(4) << diff << "\n";
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

    std::cerr << "[MSG] Fichier n°" << info.fichier_courant + 1 << " :\n[MSG] Population du fichier  " <<  info.threads->argv[info.fichier_courant] << ":\n[MSG] "
              <<  info.NAgent[info.fichier_courant] << " bulletins    Total : " <<  info.NCumAgentXml << "bulletins  " << info.nbLigne <<" lignes cumulées.\n";


    xmlFreeDoc(doc);
    if (log.is_open())
       log.close();

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

    info.NAgent = new uint32_t[info.threads->argc]();
    info.Table  = new xmlChar**[info.NCumAgent]();

    if (info.Table == nullptr)
    {
        perror("Erreur : Mémoire insuffisante");
        exit(-18);
    }

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
    {
        info.memoire_p_ligne  = info.minimum_memoire_p_ligne  // chaque agent a au moins BESOIN_MEMOIRE_ENTETE champs du bulletins de paye en colonnes
                                                              // sans la table ces champs sont répétés à chaque ligne de paye.
                                    + nbType // espace pour les drapeaux de séparation des champs (taille de type_remuneration). Nécessaire pour l'algorithme
                                    + (info.NLigne[agent])*(INDEX_MAX_CONNNES + 1);   // nombre de lignes de paye x nombre maximum de types de balises distincts de lignes de paye
                                                                                      // soit N+1 pour les écritures du type Var(l+i), i=0,...,N dans ECRIRE_LIGNE_l_COMMUN

        info.Table[agent] = new xmlChar* [info.memoire_p_ligne]; // ne pas oublier d'initialiser à nullptr !
        memset(info.Table[agent], 0, info.memoire_p_ligne * sizeof(xmlChar*));

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
        generate_rank_signal();
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
#endif
    return nullptr;
}

