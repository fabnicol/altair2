#include "ligne_paye.hpp"
#include "validator.hpp"



/* obligatoire */

#define ligne_l  info.Table[info.NCumAgentXml][l]


/* Remplace les occurrences d'un caractère séparateur à l'intérieur d'un champ par le caractère '_' qui ne doit donc jamais
   être séparateur de champ (c'est bien rare !) */

static inline int GCC_INLINE Bulletin(const char*  tag, xmlNodePtr& cur, int l, info_t& info, int normalJump = 0)
{
    // attention faire en sorte que cur ne soit JAMAIS nul en entrée ou en sortie

    const xmlNodePtr nextcur = std::move(atteindreNoeud(tag, cur, normalJump));

    if ( nullptr == nextcur)
    {
        return NODE_NOT_FOUND;
    }

// On a à présent la garantie que cur->name correspond à tag

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

/* obligatoire, mais possibilité de fallback si STRICT n'est pas défini */

//             std::cerr << ERROR_HTML_TAG "Noeud courant null au stade de la vérification de " << tag << ENDL;

static inline bool GCC_INLINE bulletin_obligatoire(const char* tag, xmlNodePtr& cur, int l,  info_t& info, int normalJump = 0)
{

    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info, normalJump))
    {
        // on sait que cur ne sera jamais nul
        case NODE_FOUND : return true;

        case NODE_NOT_FOUND :
                if (verbeux) std::cerr << ERROR_HTML_TAG "Impossible d'atteindre " << tag << " à partir de " << cur->name << ENDL;
                ligne_l = xmlStrdup(NA_STRING);
                break;

        case LINE_MEMORY_EXCEPTION :
                if (verbeux) std::cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
                ligne_l = xmlStrdup(NA_STRING);
                break;

        case NO_NEXT_ITEM :
                if (verbeux) std::cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
                break;

    }

        afficher_environnement_xhl(info, cur) ;

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


static inline bool GCC_INLINE bulletin_optionnel_numerique(const char* tag, xmlNodePtr& cur, int l, info_t& info, int normalJump = 0)
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
             if (verbeux) std::cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
             ligne_l = xmlStrdup(NA_STRING);
             break;

        case NO_NEXT_ITEM :
             if (verbeux) std::cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
             #ifndef DECIMAL_NON_EN
              if (info.decimal != '.')
             #endif
               substituer_separateur_decimal(ligne_l, info.decimal);
             break;
    }

        afficher_environnement_xhl(info, cur) ;

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


/* obligatoire et avec substitution séparateur décimal */

static inline bool GCC_INLINE bulletin_obligatoire_numerique(const char* tag, xmlNodePtr& cur, int l, info_t& info, int normalJump = 0)
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
             if (verbeux) std::cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
             ligne_l = xmlStrdup(NA_STRING);
             break;

        case NO_NEXT_ITEM :
             std::cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
             #ifndef DECIMAL_NON_EN
              if (info.decimal != '.')
             #endif
                   substituer_separateur_decimal(ligne_l, info.decimal);
             break;
    }
        afficher_environnement_xhl(info, cur) ;

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



/* REFERENCE */
/*
<PayeIndivMensuel>
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

static inline int lignePaye(xmlNodePtr cur, info_t& info)
{
    int l = BESOIN_MEMOIRE_ENTETE;

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

                std::cerr << ERROR_HTML_TAG "En excès du nombre de types de lignes de paye autorisé (" << nbType << ")." ENDL;
                if (cur)
                    std::cerr << ERROR_HTML_TAG "Type litigieux " << cur->name << " aux alentours du matricule " << info.Table[info.NCumAgentXml][Matricule] << ENDL;
                else
                    std::cerr << ERROR_HTML_TAG "Pointeur noeud courant nul" << ENDL;

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
            rembobiner = false; // 'Rembobinage gratuit'
            continue; // garantit incidemment que cur != nullptr dans la boucle
        }

        // cur n'est pas nul à ce point

        cur = cur->xmlChildrenNode;

        if (cur == nullptr) break;

        // cur n'est pas nul à ce point et ne devient jamais nul ci-après

        /* Libellé, obligatoire */

        bulletin_obligatoire("Libelle", cur, l, info);

        ++l;
        /* Code, obligatoire */

        bulletin_obligatoire("Code", cur, l, info);

        ++l;
        /* Base, si elle existe */

        bulletin_optionnel_numerique("Base", cur,  l, info);

        ++l;
        /* Taux, s'il existe */

        bulletin_optionnel_numerique("Taux", cur, l, info);

        ++l;
        /* Nombre d'unités, s'il existe */

        bulletin_optionnel_numerique("NbUnite", cur, l, info);

        ++l;
        /* Montant, obligatoire */

        bulletin_obligatoire_numerique("Mt", cur, l, info);

        ++l;
        // cur ne sera pas nul à ce point

        ++nbLignePaye;

        cur =  cur->parent->next;
        // le parent ne peut être nul

        // attention si du code est rajouté ici il doit l'être sous garde cur != nullptr
        // Lorsque on a épuisé tous les types licites on a nécessairement cur = nullptr et la boucle s'arrête
    }

#undef ligne_l

    return nbLignePaye;
}

#define BULLETIN_OBLIGATOIRE_(X, normalJump) bulletin_obligatoire(#X, cur, X, info, normalJump)
#define BULLETIN_OBLIGATOIRE(X) BULLETIN_OBLIGATOIRE_(X, 0)

#define BULLETIN_OBLIGATOIRE_NUMERIQUE_(X, normalJump)  bulletin_obligatoire_numerique(#X, cur, X, info, normalJump)
#define BULLETIN_OBLIGATOIRE_NUMERIQUE(X) BULLETIN_OBLIGATOIRE_NUMERIQUE_(X, 0)

#define BULLETIN_OPTIONNEL_NUMERIQUE_(X, normalJump)  bulletin_optionnel_numerique(#X, cur, X, info, normalJump)
#define BULLETIN_OPTIONNEL_NUMERIQUE(X)  BULLETIN_OPTIONNEL_NUMERIQUE_(X, 0)

/* REFERENCE */
/*
 * <Agent>
      <Civilite V="">{0,1}</Civilite>
      <Nom V="">{1,1}</Nom>
      <ComplNom V="">{0,1}</ComplNom>
      <Prenom V="">{0,1}</Prenom>
      <Matricule V="">{1,1}</Matricule>
      <NIR V="">{1,1}</NIR>
      <Adresse>{1,1}</Adresse>
      <NbEnfants V="">{1,1}</NbEnfants>
      <Statut V="">{1,1}</Statut>
      <RefNomenStatutaire>{0,1}</RefNomenStatutaire>
      <EmploiMetier V="">{1,1}</EmploiMetier>
      <Grade V="">{1,1}</Grade>
      <Echelon V="">{1,1}</Echelon>
      <Indice V="">{1,1}</Indice>
      <CptBancaire>{0,1}</CptBancaire>
    </Agent>
*/

uint64_t  parseLignesPaye(xmlNodePtr cur, info_t& info, std::ofstream& log)
{
    bool result = true;

    xmlNodePtr cur_parent = cur;

    cur = atteindreNoeud("Agent", cur);

    if (cur == nullptr)
    {

        std::string temp_logpath =getexecpath();

        std::cerr << ERROR_HTML_TAG "Agent non identifié. Consulter le fichier erreur.log sous " << temp_logpath  << " pour avoir les détails de l'incident." ENDL;

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

            log << "\n\nERROR_HTML_TAG L'agent est non identifié pour le fichier : " << info.threads->argv[info.fichier_courant] << "\n"
                << "Année " << info.Table[info.NCumAgentXml][Annee] << "\n"
                << "Mois "  << info.Table[info.NCumAgentXml][Mois]  << "\n\n";

            if (info.NCumAgentXml && info.Memoire_p_ligne[info.NCumAgentXml - 1] > Matricule && info.Table[info.NCumAgentXml - 1][Matricule] != nullptr)
                log << "Matricule précédent : " << info.Table[info.NCumAgentXml - 1][Matricule] << "\n\n";

            log.flush();
            log.seekp(std::ios_base::end);

            if (log_to_be_closed) log.close();
        }
        else
            std::cerr << ERROR_HTML_TAG " Impossible d'écrire le log des erreurs." ENDL;

#ifdef STRICT
        if (log.is_open()) log.close();
        exit(-520);
#endif

        for (int l : {Nom, Prenom, Matricule, NIR, EmploiMetier, NbEnfants, Grade, Indice})
        {
           info.Table[info.NCumAgentXml][l] = xmlStrdup((xmlChar*)"*");
        }

        cur = cur_parent;
        cur = atteindreNoeud("Service", cur);
        goto level0;
    }

    // cur n'est pas nul à ce point

    cur_parent = cur;
    cur = cur->xmlChildrenNode;

    if (cur == nullptr ||  xmlIsBlankNode(cur)) return 0;

#ifdef TOLERANT_TAG_HIERARCHY
    xmlNodePtr cur_save = cur;
#endif

    /* dans certains schémas on peut ne pas avoir la civilité */
    /* passer à la balise adjacente après lecture */

    info.drapeau_cont = true;

    /* result va garantir notamment que le pointeur cur filé implicitement est non nul */

    if ((result = BULLETIN_OBLIGATOIRE(Nom))) {}

    if ((result = result
        && BULLETIN_OBLIGATOIRE(Prenom)
        && BULLETIN_OBLIGATOIRE(Matricule))) {}

        result = BULLETIN_OBLIGATOIRE(NIR);

    #ifdef TOLERANT_TAG_HIERARCHY       // on refait le parcours depuis le haut en cas d'ordre inexact des balises
        cur = cur_save;
    #endif

    if ((result = (result                  // garde spécifique

                   /* NOTA : on ne contrôle pas le respect du champ Adresse, normalement obligatoire
                    * et situé entre NIR et NbEnfants, ce champ devant être regardé comme trop volatile
                    * pour que le contrôle s'y attarde. */

        && BULLETIN_OBLIGATOIRE(NbEnfants)
        && BULLETIN_OBLIGATOIRE(Statut)
        && BULLETIN_OBLIGATOIRE(EmploiMetier)))) {} // no-op

    #ifdef TOLERANT_TAG_HIERARCHY
        cur = cur_save;
    #endif

    if ((result = (result && BULLETIN_OBLIGATOIRE(Grade)))) {} // no-op

    #ifdef TOLERANT_TAG_HIERARCHY
        cur = cur_save;
    #endif

    /* ne pas lire la balise adjacente : fin du niveau subordonné Agent*/

    info.drapeau_cont = false;

    if ((result = (result && BULLETIN_OBLIGATOIRE_(Indice, 1)))) {}

    if (!result)
    {
        std::cerr << ERROR_HTML_TAG "Problème de conformité des données [512]" ENDL;
#ifdef STRICT
        exit(-512);
#endif
    }

    /* on remonte d'un niveau */

    cur = cur_parent;
    if (!result && verbeux) std::cerr << ERROR_HTML_TAG "Remontée d'un niveau" ENDL;

    #ifdef TOLERANT_TAG_HIERARCHY
      cur_save = cur;
    #endif

  /* Long saut vers cette étiquette dans le cas Agent non reconnu */

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

        if ((cur =  cur->xmlChildrenNode) != nullptr && ! xmlIsBlankNode(cur))
        {
            ligne = lignePaye(cur, info);
        }

        /* si la balise <Remuneration/> est fermante ou si <Remuneration>....</Remuneration> ne contient pas de ligne de paye codée
         * alors on attribue quand même une ligne, codée NA sur tous les champs */

        if (ligne == 0)
        {
            for (int k = 0; k <= INDEX_MAX_CONNNES; ++k)
              {
                info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE + k] = (xmlChar*) xmlStrdup(NA_STRING);
              }
                info.Memoire_p_ligne[info.NCumAgentXml] = BESOIN_MEMOIRE_ENTETE + INDEX_MAX_CONNNES + 1;
        }

        cur = cur_save->next;
    }
    else
    {
        perror(ERROR_HTML_TAG "Rémunération introuvable.");
        exit(-4);
    }

    /* obligatoire , substitution du sparateur décimal */


    result = BULLETIN_OPTIONNEL_NUMERIQUE(NbHeureTotal);

    //cur = atteindreNoeud("NbHeureSup", cur);

    /* obligatoire, substitution du sparateur décimal */
    result = result && BULLETIN_OPTIONNEL_NUMERIQUE_(NbHeureSup, 1);
    result = result && BULLETIN_OBLIGATOIRE_NUMERIQUE(MtBrut);
    result = result && BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNet);

    info.drapeau_cont=false; // fin du niveau PayeIndivMensuel
    result = result && BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNetAPayer);

    if (!result)
    {
        std::cerr << ERROR_HTML_TAG "Problème de conformité des données sur les champs des bulletins de paye." ENDL;
#ifdef STRICT
        exit(-513);
#endif
    }

    // Rémuneration tag vide
    if (ligne == 0) ligne = 1 ;

    return ligne;
}
