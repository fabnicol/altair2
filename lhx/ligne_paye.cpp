#include "ligne_paye.hpp"

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

xmlNodePtr GCC_INLINE atteindreNoeud(const char* noeud, xmlNodePtr cur, bool  opt, int normalJump)
  {
    return atteindreNoeud(reinterpret_cast<const xmlChar*>(noeud),  cur, opt, normalJump);
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

void afficher_environnement_xhl(const info_t& info)
{

    if (mut.try_lock())
    {
        std::cerr << "\nFichier   " <<  info.threads->argv[info.fichier_courant] << "\n";

        for (int l = 0; l < info.Memoire_p_ligne[info.NCumAgentXml]; ++l)
        {
         if (nullptr != info.Table[info.NCumAgentXml][l])
            std::cerr << "** info.Table[" << info.NCumAgentXml << "][" << Tableau_entete[l] << "]=" << info.Table[info.NCumAgentXml][l] << "\n";
        }

        mut.unlock();
    }
    else
        afficher_environnement_xhl(info);
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


static inline bool GCC_INLINE bulletin_optionnel_numerique(const xmlChar* tag, xmlNodePtr& cur, int l, info_t& info, int normalJump = 0)
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

static inline bool GCC_INLINE bulletin_optionnel_numerique(const char* tag, xmlNodePtr& cur, int l, info_t& info, int normalJump = 0)
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
    int l = BESOIN_MEMOIRE_ENTETE;

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
            rembobiner = false; // 'Rembobinage gratuit'
            continue; // garantit incidemment que cur != nullptr dans la boucle
        }

        // cur n'est pas nul � ce point
        //std::cerr << "^" << cur->name <<"++\n";
        cur = cur->xmlChildrenNode;

        if (cur == nullptr) break;

        // cur n'est pas nul � ce point et ne devient jamais nul ci-apr�s

        /* Libell�, obligatoire */

        bulletin_obligatoire("Libelle", cur, l, info);
        //std::cerr << l << "  *" << ligne_l << "*\n";
        ++l;
        /* Code, obligatoire */

        bulletin_obligatoire("Code", cur, l, info);
        //std::cerr << l << "  *" << ligne_l << "*\n";
        ++l;
        /* Base, si elle existe */

        bulletin_optionnel_numerique("Base", cur,  l, info);
        //std::cerr << l << "  *" << ligne_l << "*\n";
        ++l;
        /* Taux, s'il existe */

        bulletin_optionnel_numerique("Taux", cur, l, info);
        //std::cerr << l << "  *" << ligne_l << "*\n";
        ++l;
        /* Nombre d'unit�s, s'il existe */

        bulletin_optionnel_numerique("NbUnite", cur, l, info);
        //std::cerr << l << "  *" << ligne_l << "*\n";
        ++l;
        /* Montant, obligatoire */

        bulletin_obligatoire_numerique("Mt", cur, l, info);
        //std::cerr << l << "  *" << ligne_l << "*\n";
        ++l;
        // cur ne sera pas nul � ce point

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

uint64_t  parseLignesPaye(xmlNodePtr cur, info_t& info, std::ofstream& log)
{
    bool result = true;

    xmlNodePtr cur_parent = cur;

    cur = atteindreNoeud("Agent", cur);

    if (cur == nullptr)
    {
        std::cerr << "Erreur : Impossible d'atteindre \"Agent\"\n";

        std::string temp_logpath =getexecpath();

        std::cerr << "Erreur : Agent non identifi�. Consulter le fichier erreur.log sous " << temp_logpath  << " pour avoir les d�tails de l'incident.\n";

        if (info.chemin_log.empty())
        {
              temp_logpath = temp_logpath + "/erreurs.log";
        }

        bool log_to_be_closed = false;

        if (!log.is_open())
        {
            log_to_be_closed = true;
            log.open(temp_logpath, std::ios::app);
        }

        if (log.good())
        {

            log.flush();
            log.seekp(std::ios_base::end);

            log << "\n\nErreur : L'agent est non identifi� pour le fichier : " << info.threads->argv[info.fichier_courant] << "\n"
                << "Ann�e " << info.Table[info.NCumAgentXml][Annee] << "\n"
                << "Mois "  << info.Table[info.NCumAgentXml][Mois]  << "\n\n";

            if (info.NCumAgentXml)
                log << "Matricule pr�c�dent : " << info.Table[info.NCumAgentXml - 1][Matricule] << "\n\n";

            log.flush();
            log.seekp(std::ios_base::end);

            if (log_to_be_closed) log.close();
        }
        else
            std::cerr << "Erreur :  Impossible d'�crire le log des erreurs.\n";

        for (int l : {Nom, Prenom, Matricule, NIR, EmploiMetier, NbEnfants, Grade, Indice})
        {
           info.Table[info.NCumAgentXml][l] = xmlStrdup((xmlChar*)"*");
        }

        cur = cur_parent;
        cur = atteindreNoeud("Service", cur);
        goto level0;
    }

    // cur n'est pas nul � ce point

    cur_parent = cur;
    cur = cur->xmlChildrenNode;

    if (cur == nullptr ||  xmlIsBlankNode(cur)) return 0;

#ifdef TOLERANT_TAG_HIERARCHY
    xmlNodePtr cur_save = cur;
#endif

    /* dans certains sch�mas on peut ne pas avoir la civilit� */
    /* passer � la balise adjacente apr�s lecture */

    info.drapeau_cont = true;

    /* result va garantir notamment que le pointeur cur fil� implicitement est non nul */

    if ((result = (BULLETIN_OBLIGATOIRE(Nom)
        && BULLETIN_OBLIGATOIRE(Prenom)
        && BULLETIN_OBLIGATOIRE(Matricule)))) {}
        result = BULLETIN_OBLIGATOIRE(NIR);

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

    if ((result = (result && BULLETIN_OBLIGATOIRE_(Indice, 1)))) {}

    if (!result)
    {
        std::cerr << "Erreur : Probl�me de conformit� des donn�es [512]\n";
#ifdef STRICT
        exit(-512);
#endif
    }

    /* on remonte d'un niveau */

    cur = cur_parent;
    if (!result) std::cerr << "Erreur : Remont�e d'un niveau\n";

    #ifdef TOLERANT_TAG_HIERARCHY
      cur_save = cur;
    #endif

  /* Long saut vers cette �tiquette dans le cas Agent non reconnu */

  level0:

 /*  */

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

        if ((cur =  cur->xmlChildrenNode) != nullptr && ! xmlIsBlankNode(cur))
        {
            ligne = lignePaye(cur, info);
        }

        /* si la balise <Remuneration/> est fermante ou si <Remuneration>....</Remuneration> ne contient pas de ligne de paye cod�e
         * alors on attribue quand m�me une ligne, cod�e NA sur tous les champs */

        if (ligne == 0)
        {
            for (int k = 0; k <= INDEX_MAX_CONNNES; ++k)
              {
                info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE + k] = (xmlChar*) xmlStrdup(NA_STRING);
              }
                info.Memoire_p_ligne[info.NCumAgentXml] = BESOIN_MEMOIRE_ENTETE + INDEX_MAX_CONNNES + 1;
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


    result = BULLETIN_OPTIONNEL_NUMERIQUE(NbHeureTotal);
    //cur = atteindreNoeud("NbHeureSup", cur);

    /* obligatoire, substitution du sparateur d�cimal */
    result = result && BULLETIN_OPTIONNEL_NUMERIQUE_(NbHeureSup, 1);
    result = result && BULLETIN_OBLIGATOIRE_NUMERIQUE(MtBrut);
    result = result && BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNet);

    info.drapeau_cont=false; // fin du niveau PayeIndivMensuel
    result = result && BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNetAPayer);

    if (!result)
    {
        std::cerr << "Erreur : Probl�me de conformit� des donn�es [513]\n";
#ifdef STRICT
        exit(-513);
#endif
    }

    // R�muneration tag vide
    if (ligne == 0) ligne = 1 ;

    return ligne;
}
