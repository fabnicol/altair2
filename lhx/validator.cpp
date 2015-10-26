/*  Programme �crit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est �dit�, le pr�sent code doit �tre converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'�tre compil�.
 *  En entr�e d'Altair pr�ciser encodage.entr�e en conformit� avec l'encodage du pr�sent fichier, qui sera celui de la base g�n�r�e.
 */

/* Constantes de compilation pouvant �tre red�finies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, NO_REGEXP */

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
                En exc�s du nombre de lignes de paye autoris� (" << info.nbLigneUtilisateur << ").\n\
                Omettre -n ... et utiliser -L fichier_log pour d�tecter le maximum de lignes de paye dans les fichiers.\n\
                Utiliser -N ce_maximum ou bien recompiler en augmentant MAX_LIGNES_PAYE, � condition de disposer d'assez de m�moire.\n";

        exit(-10);
    }
}

/* obligatoire */

#define ligne_l  info.Table[info.NCumAgentXml][l]


/* utilit� d'affichage de l'environnement xhl en cas de probl�me de conformit� des donn�es */

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


/* Remplace les occurrences d'un caract�re s�parateur � l'int�rieur d'un champ par le caract�re '_' qui ne doit donc jamais
   �tre s�parateur de champ (c'est bien rare !) */

static inline int GCC_INLINE Bulletin(const xmlChar*  tag, xmlNodePtr& cur, int l, info_t& info, bool opt = true, int normalJump = 0)
{
    // attention faire en sorte que cur ne soit JAMAIS nul en entr�e ou en sortie

    const xmlNodePtr nextcur = std::move(atteindreNoeud(tag, cur, opt, normalJump));

    if ( nullptr == nextcur)
    {
        return NODE_NOT_FOUND;
    }

// On a � pr�sent la garantie que cur->name correspond � tag

        ligne_l = xmlGetProp(nextcur, (const xmlChar *) "V");
        if (ligne_l == nullptr)
            return LINE_MEMORY_EXCEPTION;

// dans les cas o� false est retourn�, la r�f�rence � cur n'a pas �t� modifi�e et est donc non nulle

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

/* obligatoire, mais possibilit� de fallback si STRICT n'est pas d�fini */

//             std::cerr << "Erreur : Noeud courant null au stade de la v�rification de " << tag << "\n";

static inline bool GCC_INLINE bulletin_obligatoire(const xmlChar* tag, xmlNodePtr& cur, int l,  info_t& info, int normalJump = 0)
{

    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info, normalJump))
    {
        // on sait que cur ne sera jamais nul
        case NODE_FOUND : return true;

        case NODE_NOT_FOUND :
                std::cerr << "Erreur : Impossible d'atteindre " << tag << " � partir de " << cur->name << "\n";
                ligne_l = xmlStrdup(NA_STRING);
                break;

        case LINE_MEMORY_EXCEPTION :
                std::cerr << "Erreur : Allocation m�moire impossible pour la ligne " << l << "\n";
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

/* A tester : la substitution du caract�re d�cimal , au . de la locale anglaise utilis� par X�m�lios (h�las)
   reste n�cessaire tant que nous utiliserons un stockage uniforme en cha�nes de caract�res.
   Si un jour nous d�cidons d'utilisr strold pour convertir les cha�nes de caract�re num�riques en float, nous
   gagnerons de la place en stockage temporaire (peut �tre utile pour les gros fichiers) et alors printf et setlocale
   feront le travail de substitution de la virgule au point lors de l'�criture de la base.
   A ce stade nous stockons tous les champs lus en char, pour �criture identique en .csv dans la table, avec substition
   'manuelle' de la virgule au point dans la cha�ne en output. */

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
             std::cerr << "Erreur : Allocation m�moire impossible pour la ligne " << l << "\n";
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

/* obligatoire et avec substitution s�parateur d�cimal */

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
             std::cerr << "Erreur : Allocation m�moire impossible pour la ligne " << l << "\n";
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

    ligne_l = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour �viter la confusion avec \0 des chaines vides
    ++l;

    /* Besoins en m�moire : BESOIN_MEMOIRE_ENTETE [champs hors ligne] + nombre de lignes + flags (maximum nbType) */
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
            ligne_l = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour �viter la confusion avec \0 des chaines vides
            // on ne teste pas ligne_l != nullptr ici mais en principe on devrait
            ++l;
        }

        // ici on pourrait in fine se passer de ce test par compilation s�par�e

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

        // cur n'est pas nul � ce point

        cur =  cur->xmlChildrenNode;

        if (cur == nullptr) break;

        // cur n'est pas nul � ce point et ne devient jamais nul ci-apr�s

        /* Libell�, obligatoire */

        if (bulletin_obligatoire("Libelle", cur, l, info)

        /* Code, obligatoire */

           && bulletin_obligatoire("Code", cur, l, info)

        /* Base, si elle existe */

           && bulletin_optionnel_numerique("Base", cur,  l, info)

        /* Taux, s'il existe */

           && bulletin_optionnel_numerique("Taux", cur, l, info)

        /* Nombre d'unit�s, s'il existe */

           && bulletin_optionnel_numerique("NbUnite", cur, l, info)

        /* Montant, obligatoire */

           && bulletin_obligatoire_numerique("Mt", cur, l, info)) {}

        // cur ne sera pas nul � ce point

		++l;
        ++nbLignePaye;

        cur =  cur->parent->next;
        // le parent ne peut �tre nul

        // attention si du code est rajout� ici il doit l'�tre sous garde cur != nullptr
        // Lorsque on a �puis� tous les types licites on a n�cessairement cur = nullptr et la boucle s'arr�te
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

    // cur n'est pas nul � ce point

    cur_parent = cur;
    cur =  cur->xmlChildrenNode;
    
#ifdef TOLERANT_TAG_HIERARCHY
    xmlNodePtr cur_save = cur;
#endif

    if (cur == nullptr) return 0;

    /* dans certains sch�mas on peut ne pas avoir la civilit� */
    /* passer � la balise adjacente apr�s lecture */

    info.drapeau_cont = true;
    bool result = true;

    /* result va garantir notamment que le pointeur cur fil� implicitement est non nul */

    if ((result = (BULLETIN_OBLIGATOIRE(Nom)
        && BULLETIN_OBLIGATOIRE(Prenom)
        && BULLETIN_OBLIGATOIRE(Matricule)
        && BULLETIN_OBLIGATOIRE(NIR)))) {} // no-op

    #ifdef TOLERANT_TAG_HIERARCHY       // on refait le parcours depuis le haut en cas d'ordre inexact des balises
        cur = cur_save;
    #endif

    if ((result = (result                  // garde sp�cifique
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

    /* ne pas lire la balise adjacente : fin du niveau subordonn� Agent*/

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


    /* obligatoire, substitution du s�parateur d�cimal */
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
        perror("Erreur : R�mun�ration introuvable.");
        exit(-4);
    }

    /* obligatoire , substitution du sparateur d�cimal */


    BULLETIN_OPTIONNEL_NUMERIQUE(NbHeureTotal);
    //cur = atteindreNoeud("NbHeureSup", cur);

    /* obligatoire, substitution du sparateur d�cimal */
    BULLETIN_OPTIONNEL_NUMERIQUE_(NbHeureSup, 1);
    BULLETIN_OBLIGATOIRE_NUMERIQUE(MtBrut);
    BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNet);

    info.drapeau_cont=false; // fin du niveau PayeIndivMensuel
    BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNetAPayer);

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

    cur =  cur->xmlChildrenNode;

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
        cur =  cur->xmlChildrenNode;
        budget_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        cur = cur->parent;
        cur = cur->next;
        // si cur == nullptr on aura un arr�t de la boucle.
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
                std::cerr  << "Erreur : Siret non d�tectable\n";
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
                    << "Diff�rence " << std::setw(4) << diff << "\n";
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
#error "C++11 doit �tre utilis�."
#endif


#endif // defined


void* decoder_fichier(info_t& info)
{
    /* environ 6000 bulletins par seconde en processus sumple, et 15000 en multithread ; rajoute 1/3 du temps */



    #if  defined GCC_REGEX && !defined NO_REGEX //&& !defined __WIN32__ && !defined GCC_4_8

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

    info.NAgent = new uint32_t[info.threads->argc]();
    info.Table  = new xmlChar**[info.NCumAgent]();

    if (info.Table == nullptr)
    {
        perror("Erreur : M�moire insuffisante");
        exit(-18);
    }

    for (unsigned agent = 0; agent < info.NCumAgent; ++agent)
    {
        info.memoire_p_ligne  = info.minimum_memoire_p_ligne  // chaque agent a au moins BESOIN_MEMOIRE_ENTETE champs du bulletins de paye en colonnes
                                                              // sans la table ces champs sont r�p�t�s � chaque ligne de paye.
                                    + nbType // espace pour les drapeaux de s�paration des champs (taille de type_remuneration). N�cessaire pour l'algorithme
                                    + (info.NLigne[agent])*(INDEX_MAX_CONNNES + 1);   // nombre de lignes de paye x nombre maximum de types de balises distincts de lignes de paye
                                                                                      // soit N+1 pour les �critures du type Var(l+i), i=0,...,N dans ECRIRE_LIGNE_l_COMMUN

        info.Table[agent] = new xmlChar* [info.memoire_p_ligne]; // ne pas oublier d'initialiser � nullptr !
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
        std::cerr << "Erreur : Incoh�rence de l'allocation m�moire ex-ante " << info.NCumAgent
                  << " unit�s et ex-post " <<  info.NCumAgentXml << " unit�s d'information.\n"
                  << "Sortie pour �viter une erreur de segmentation.\n";
        std::cerr << "\nFichier : " << info.threads->argv[info.fichier_courant] << "\n";

        exit(1005);
    }


    // attention, pas info<-NCumAgent ici

    /* Le champ statut est modifi� comme suit :
        ELU   pour un �lu
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
#endif
    return nullptr;
}

