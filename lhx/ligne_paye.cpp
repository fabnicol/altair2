#include "ligne_paye.hpp"
#include "validator.hpp"

using namespace std;


/* NOTA Sur les valeurs manquantes
 * Pour des variables caractères : NA (NA_ASSIGN)
 * Pour des variables pseudo-numériques (caractères convertibles en numériques) : 0 (ZERO_ASSIGN)
 * On peut donc garantir que Année, Mois, NbEnfants, Indice, NBI, QuotiteTrav,
 * NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer ne sont jamais NA mais à 0 */

/* obligatoire */

#define NA_ASSIGN(X)        info.Table[info.NCumAgentXml][X] = (xmlChar*) xmlStrdup(NA_STRING)
#define ZERO_ASSIGN(X)      info.Table[info.NCumAgentXml][X] = (xmlChar*) xmlStrdup((const xmlChar*) "0")

/* Remplace les occurrences d'un caractère séparateur à l'intérieur d'un champ par le caractère '_' qui ne doit donc jamais
   être séparateur de champ (c'est bien rare !) */

static inline void GCC_INLINE sanitize(xmlChar* s, const char sep)
{

    while (*s != 0)
    {
        // Non-switchable car info.seperateur n'est pas une expression constante.
        if (*s == sep)  *s = '_';

        switch(*s)
        {

          case '\n':
            *s = ' ';
            break;

#ifdef CONVERTIR_LATIN_1
#if defined(__WIN32__) && !defined(USE_ICONV)

            /* Gros hack de pseudo-conversion UTF-8 vers Latin-1, qui permet d'économiser les 40 % de surcoût d'exécution
             * lié à l'utilisation d'iconv pour retraiter les fichiers de sortie (fonction convertir(const char*))
             * Ce hack est presque sans coût. Il se base sur les hypothèses suivantes :
             *   a) pas de caractères spéciaux multioctets
             *   b) seuls sont convertis : à, â, ç, è, é, ê, ë, î, ï, ô, û ... et les majuscules correspondantes càd
             * dont le code UTF-8 commence par 0xC3. Il suffit d'ajouter 0x40 sur les quatre bits hauts de l'octet. */

        case 0xC3:

            *s = ((*(s + 1) & 0xF0) + 0x40) | (*(s + 1) & 0x0F);

            effacer_char(s + 1);

            break;

        case 0xC2:

            *s = *(s + 1);
            /* Le caractère ° (degré) est bien codé en Latin-1 comme 0xB0, mais il y a un problème avec le paquet texlive
             * inputenc pour la conversion pdf. On remplace donc par e (0x65) */

            //if (info.Table[info.NCumAgentXml][l][i] == 0xB0) info.Table[info.NCumAgentXml][l][i] = 0x65;

            effacer_char(s + 1);

            break;
#endif
#endif
     }

        ++s;
    }
}



static inline int GCC_INLINE Bulletin(const char*  tag, xmlNodePtr& cur, int l, info_t& info)
{
    // attention faire en sorte que cur ne soit JAMAIS nul en entrée ou en sortie

    const xmlNodePtr nextcur = atteindreNoeud(tag, cur);

    if ( nullptr == nextcur)
    {
        return NODE_NOT_FOUND;
    }

// On a à présent la garantie que cur->name correspond à tag


        if ((info.Table[info.NCumAgentXml][l]
               = xmlGetProp(nextcur, (const xmlChar *) "V"))
                  == nullptr)

            return LINE_MEMORY_EXCEPTION;

        if (info.drapeau_cont)
        {
            if (nextcur->next != nullptr)
                cur = nextcur->next;
            else
                return NO_NEXT_ITEM;  // pour garantir que cur ne devient pas nul.
        }

    /* sanitisation */

       sanitize(info.Table[info.NCumAgentXml][l], info.separateur);

       return NODE_FOUND;

}

/* obligatoire, mais possibilité de fallback si STRICT n'est pas défini */

//             cerr << ERROR_HTML_TAG "Noeud courant null au stade de la vérification de " << tag << ENDL;

static inline bool GCC_INLINE bulletin_obligatoire(const char* tag, xmlNodePtr& cur, int l,  info_t& info)
{

    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info))
    {
        // on sait que cur ne sera jamais nul
        case NODE_FOUND : return true;

        case NODE_NOT_FOUND :
                    {
                      lock_guard<mutex> guard(mut);
                      cerr << ERROR_HTML_TAG "Balise manquante " << tag << " avant la balise " << cur->name << ENDL;
                    }  
                    
                    if (verbeux)
                    afficher_environnement_xhl(info, cur);
                NA_ASSIGN(l);
                break;

        case LINE_MEMORY_EXCEPTION :
                    {
                      lock_guard<mutex> guard(mut);
                      cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
                    }
                    if (verbeux)
                    afficher_environnement_xhl(info, cur);
                NA_ASSIGN(l);
                break;

        case NO_NEXT_ITEM :
                    {
                      lock_guard<mutex> guard(mut);
                      cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
                    }

                    if (verbeux)
                    afficher_environnement_xhl(info, cur);
                break;

    }

    
    /* Ne pas mettre de lock ici, il y en a un dans warning_msg */

    warning_msg(tag, info, cur);

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

static inline bool GCC_INLINE bulletin_optionnel_char(const char* tag, xmlNodePtr& cur, int l, info_t& info)
{
    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info))
    {
        // on sait que cur ne sera jamais nul
        case NODE_FOUND :
             return true;

        case NODE_NOT_FOUND :
             NA_ASSIGN(l);
             return true;

        case LINE_MEMORY_EXCEPTION :
             if (verbeux)
                 cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
             NA_ASSIGN(l);
             break;

        case NO_NEXT_ITEM :
             if (verbeux) cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
             break;
    }

    /* Ne pas mettre de lock ici, il y en a un dans warning_msg */

    warning_msg(tag, info, cur);

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


static inline bool GCC_INLINE bulletin_optionnel_numerique(const char* tag, xmlNodePtr& cur, int l, info_t& info)
{
    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info))
    {
        // on sait que cur ne sera jamais nul
        case NODE_FOUND :
             #ifndef DECIMAL_NON_EN
               if (info.decimal != '.')
             #endif
                substituer_separateur_decimal(info.Table[info.NCumAgentXml][l], info.decimal);
             return true;

        case NODE_NOT_FOUND :
             ZERO_ASSIGN(l);
             return true;

        case LINE_MEMORY_EXCEPTION :
             if (verbeux)
                 cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
             ZERO_ASSIGN(l);
             break;

        case NO_NEXT_ITEM :
             if (verbeux) cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
             #ifndef DECIMAL_NON_EN
              if (info.decimal != '.')
             #endif
               substituer_separateur_decimal(info.Table[info.NCumAgentXml][l], info.decimal);
             break;
    }

    /* Ne pas mettre de lock ici, il y en a un dans warning_msg */

    warning_msg(tag, info, cur);

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

static inline bool GCC_INLINE bulletin_obligatoire_numerique(const char* tag, xmlNodePtr& cur, int l, info_t& info)
{
    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info))
    {
        // on sait que cur ne sera jamais nul
        case NODE_FOUND :
            #ifndef DECIMAL_NON_EN
               if (info.decimal != '.')
            #endif
               substituer_separateur_decimal(info.Table[info.NCumAgentXml][l], info.decimal);
             return true;

        case NODE_NOT_FOUND :
             ZERO_ASSIGN(l);
             return true;

        case LINE_MEMORY_EXCEPTION :
             if (verbeux) cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
             NA_ASSIGN(l);
             break;

        case NO_NEXT_ITEM :
             cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
             #ifndef DECIMAL_NON_EN
              if (info.decimal != '.')
             #endif
                substituer_separateur_decimal(info.Table[info.NCumAgentXml][l], info.decimal);
             break;
    }

   /* Ne pas mettre de lock ici, il y en a un dans warning_msg */

    warning_msg(tag, info, cur);

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

static inline LineCount lignePaye(xmlNodePtr cur, info_t& info)
{
    int l = BESOIN_MEMOIRE_ENTETE;

    int nbLignePaye = 0;

    unsigned int t = 0;

    info.Table[info.NCumAgentXml][l] = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour éviter la confusion avec \0 des chaines vides
    ++l;

    /* Besoins en mémoire : BESOIN_MEMOIRE_ENTETE [champs hors ligne] + nombre de lignes + flags (maximum nbType * nb de rembobinages) */
    int type_loop_counter = 0;

    while (cur != nullptr)
    {
        bool new_type = false;

        while (xmlStrcmp(cur->name, (const xmlChar *) type_remuneration[t]))
        {
            // Cas rare dans lequel <Remuneration> n'existe pas
            if (xmlStrcmp(cur->name, (const xmlChar *) "NbHeureTotal") == 0) 
                return  { nbLignePaye, l};
            
            ++t;
            if (t == nbType)
            {
                /* En principe les éléments constitutifs des enregistrements <Remunération>....</Remuneration> sont enregistrés
                   dans l'ordre du tableau type_remuneration. Toutefois quelques cas de désordre sont observés. Dans ces cas là on peut
                   "rembobiner le tableau". On évite toutefois de faire une recherche ensembliste systématique, qui éviterait cela mais
                   freinerait 99,9 % des recherches */

                if (++type_loop_counter < TYPE_LOOP_LIMIT)
                {
                    t = 0;
                    continue;
                }

                /* On ne rembobine qu'au maximum TYPE_LOOP_LIMIT. Si l'essai échoue, on déclenche une exception ou on retourne */

                cerr << ERROR_HTML_TAG "En excès du nombre de types de lignes de paye autorisé (" << nbType << ")." ENDL;
                if (cur)
                    cerr << ERROR_HTML_TAG "Type litigieux " << cur->name << " aux alentours du matricule " << info.Table[info.NCumAgentXml][Matricule] << ENDL;
                else
                    cerr << ERROR_HTML_TAG "Pointeur noeud courant nul" << ENDL;

              #ifdef STRICT
                 exit(-11);
              #else
                 cerr << ERROR_HTML_TAG "Arrêt du décodage de la ligne de paye." << ENDL;
                 return {nbLignePaye, l};
              #endif
            }

            new_type = true;
        }

        if (new_type && t < nbType)
        {
              // +1 pour éviter la confusion avec \0 des chaines vides
            if ((info.Table[info.NCumAgentXml][l] = (xmlChar*) xmlStrdup(drapeau[t])) == nullptr)
            {
                if (verbeux) cerr << ERROR_HTML_TAG "Erreur dans l'allocation des drapeaux de catégories." << ENDL;
                #ifdef STRICT
                   exit(-12);
                #else
                   if (verbeux) cerr << ERROR_HTML_TAG "Arrêt du décodage de la ligne de paye." << ENDL;
                #endif
                return {nbLignePaye, l};
            }

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
            --type_loop_counter; // 'Rembobinage gratuit'
            continue; // garantit incidemment que cur != nullptr dans la boucle
        }

        // cur n'est pas nul à ce point

        cur = cur->xmlChildrenNode;

        if (cur == nullptr)
        {
            NA_ASSIGN(l);
            break;
        }

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


    return  { nbLignePaye, l};
}

#define BULLETIN_OBLIGATOIRE_(X) bulletin_obligatoire(#X, cur, X, info)
#define BULLETIN_OBLIGATOIRE(X) BULLETIN_OBLIGATOIRE_(X)

#define BULLETIN_OBLIGATOIRE_NUMERIQUE_(X)  bulletin_obligatoire_numerique(#X, cur, X, info)
#define BULLETIN_OBLIGATOIRE_NUMERIQUE(X) BULLETIN_OBLIGATOIRE_NUMERIQUE_(X)

#define BULLETIN_OPTIONNEL_NUMERIQUE_(X)  bulletin_optionnel_numerique(#X, cur, X, info)
#define BULLETIN_OPTIONNEL_NUMERIQUE(X)  BULLETIN_OPTIONNEL_NUMERIQUE_(X)

#define BULLETIN_OPTIONNEL_CHAR(X)  bulletin_optionnel_char(#X, cur, X, info)

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

inline void GCC_INLINE concat(xmlNodePtr cur, info_t& info)
{
    xmlChar* addCode2 = xmlGetProp(cur, (const xmlChar*) "V");
    if (addCode2)
    {

          sanitize(addCode2, info.separateur);

          xmlChar* desc_hyphen = xmlStrncatNew(info.Table[info.NCumAgentXml][Description],
                                               (const xmlChar*) " - ",
                                                         -1);

          if (desc_hyphen)
          {
              xmlChar* desc_code2 = xmlStrncatNew(desc_hyphen,
                                                   addCode2,
                                                   -1);

              xmlFree(info.Table[info.NCumAgentXml][Description]);
              xmlFree(addCode2);
              xmlFree(desc_hyphen);
              info.Table[info.NCumAgentXml][Description] = desc_code2;
          }
    }
}


inline void allouer_ligne_NA(info_t &info, int &ligne, int &memoire_p_ligne_allouee)
{
    info.NLigne[info.NCumAgentXml] = 1;  // 1 ldp
    info.Memoire_p_ligne[info.NCumAgentXml] = BESOIN_MEMOIRE_ENTETE + INDEX_MAX_COLONNNES // nombre de NAs mis pour les variables de paye de la ligne
                                                                     + 1  // TraitBrut
                                                                     + 1   ;  
    info.Table[info.NCumAgentXml].resize(info.Memoire_p_ligne[info.NCumAgentXml]);
    info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE] = (xmlChar*) xmlStrdup(drapeau[0]);  // TraitBrut
    
    for (int k = 1; k <= INDEX_MAX_COLONNNES; ++k)
    {
      info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE + k] = (xmlChar*) xmlStrdup(NA_STRING);
    }
    
    
    ligne = 1;
    
    memoire_p_ligne_allouee = BESOIN_MEMOIRE_ENTETE + INDEX_MAX_COLONNNES // nombre de NAs mis pour les variables de paye de la ligne
            + 1  // TraitBrut
            + 1;


}

uint64_t  parseLignesPaye(xmlNodePtr cur, info_t& info, ofstream& log)
{
    bool result = true;
    int na_assign_level = 0;

    xmlNodePtr cur_parent = cur;

    cur = atteindreNoeud("Agent", cur);

    if (cur == nullptr)
    {
        string temp_logpath =getexecpath();

        cerr << ERROR_HTML_TAG "Agent non identifié. Consulter le fichier erreur.log sous " << temp_logpath  << " pour avoir les détails de l'incident." ENDL;

        if (info.chemin_log.empty())
        {
              temp_logpath = temp_logpath + "/erreurs.log";
        }

        bool log_to_be_closed = false;

        if (!log.is_open())
        {
            log_to_be_closed = true;
            log.open(temp_logpath, ios::app);
        }

        if (log.good())
        {

            log.flush();
            log.seekp(ios_base::end);

            log << "\n\nErreur : L'agent est non identifié pour le fichier : " << info.threads->argv[info.fichier_courant] << "\n"
                << "Année " << info.Table[info.NCumAgentXml][Annee] << "\n"
                << "Mois "  << info.Table[info.NCumAgentXml][Mois]  << "\n\n";

            if (info.NCumAgentXml && info.Memoire_p_ligne[info.NCumAgentXml - 1] > Matricule && info.Table[info.NCumAgentXml - 1][Matricule] != nullptr)
                log << "Matricule précédent : " << info.Table[info.NCumAgentXml - 1][Matricule] << "\n\n";

            log.flush();
            log.seekp(ios_base::end);

            if (log_to_be_closed) log.close();
        }
        else
            cerr << ERROR_HTML_TAG " Impossible d'écrire le log des erreurs." ENDL;

#ifdef STRICT
        if (log.is_open()) log.close();
        exit(-520);
#endif

        for (int l : {Nom, Prenom, Matricule, NIR, EmploiMetier, Statut, NbEnfants, Grade, Echelon, Indice})
        {
           info.Table[info.NCumAgentXml][l] = xmlStrdup((xmlChar*)"");
        }

        cur = cur_parent;
        cur = atteindreNoeud("Service", cur);
        goto level0;
    }

    // cur n'est pas nul à ce point

    info.ligne_debut = xmlGetLineNo(cur);
    cur_parent = cur;
    cur = cur->xmlChildrenNode;

    if (cur == nullptr ||  xmlIsBlankNode(cur)) return 0;

#ifdef TOLERANT_TAG_HIERARCHY
    xmlNodePtr cur_save = cur;
#endif

    /* dans certains schémas on peut ne pas avoir la civilité */
    /* passer à la balise adjacente après lecture */

    info.drapeau_cont = true;

    /* if (result) va garantir notamment que le pointeur cur filé implicitement est non nul */



    result    = BULLETIN_OBLIGATOIRE(Nom);
    if (result)
    {
        result &= BULLETIN_OBLIGATOIRE(Prenom);
        if (result)
        {
            result &= BULLETIN_OBLIGATOIRE(Matricule);
            if (result)
            {
                result &= BULLETIN_OBLIGATOIRE(NIR);

#ifdef TOLERANT_TAG_HIERARCHY       // on refait le parcours depuis le haut en cas d'ordre inexact des balises
                cur = cur_save;
#endif
                if (result)
                {
                    result &= BULLETIN_OBLIGATOIRE(NbEnfants);
                    if (result)
                    {
                        result &= BULLETIN_OBLIGATOIRE(Statut);
                        /* NOTA : on ne contrôle pas le respect du champ Adresse, normalement obligatoire
                                                * et situé entre NIR et NbEnfants, ce champ devant être regardé comme trop volatile
                                                * pour que le contrôle s'y attarde. */
                        if (result)
                        {
                            result &= BULLETIN_OBLIGATOIRE(EmploiMetier);
                            if (result)
                            {
#ifdef TOLERANT_TAG_HIERARCHY
                                cur = cur_save;
#endif
                                if (result)
                                {
                                    result &= BULLETIN_OBLIGATOIRE(Grade);

#ifdef TOLERANT_TAG_HIERARCHY
                                    cur = cur_save;
#endif

                                    if (result)
                                    {
                                        result &= BULLETIN_OBLIGATOIRE(Echelon);

#ifdef TOLERANT_TAG_HIERARCHY
                                        cur = cur_save;
#endif
                                    /* ne pas lire la balise adjacente : fin du niveau subordonné Agent*/

                                        info.drapeau_cont = false;
                                        if (result)
                                        {
                                            result &= BULLETIN_OBLIGATOIRE(Indice);
                                        }
                                        else na_assign_level = 10;

                                    }
                                    else na_assign_level = 9;

                                }
                               else na_assign_level = 8;
                            }
                            else na_assign_level =7;
                        }
                        else na_assign_level = 6;
                    }
                    else na_assign_level = 5;
                }
                else na_assign_level = 4;
            }
            else na_assign_level = 3;
        }
        else na_assign_level = 2;
    }
    else na_assign_level = 1;

 /* pas de break */

    switch(na_assign_level)
    {
        case 1:
            NA_ASSIGN(Nom);
        case 2:
            NA_ASSIGN(Prenom);
        case 3:
            NA_ASSIGN(Matricule);
        case 4:
            NA_ASSIGN(NIR);
        case 5:
            ZERO_ASSIGN(NbEnfants);
        case 6:
            NA_ASSIGN(Statut);
        case 7:
            NA_ASSIGN(EmploiMetier);
        case 8:
            NA_ASSIGN(Grade);
        case 9:
             NA_ASSIGN(Grade);
        case 10:
            ZERO_ASSIGN(Indice);
        default:
        break;
    }

    if (!result)
    {
           cerr << ERROR_HTML_TAG "Problème de conformité des données [512]" ENDL;
        #ifdef STRICT
           exit(-512);
        #endif
    }

    /* on remonte d'un niveau */

    cur = cur_parent;
    if (!result && verbeux) cerr << ERROR_HTML_TAG "Remontée d'un niveau" ENDL;

    #ifdef TOLERANT_TAG_HIERARCHY
      cur_save = cur;
    #endif

  /* Long saut vers cette étiquette dans le cas Agent non reconnu */

  level0:

 /*  Référence
  *
  *    <Evenement>
  *      <Code V="">{1,1}</Code>
  *      <Description V="">{0,1}</Description>
  *    </Evenement>
  */

    info.drapeau_cont = true;

    /* C'est extrêmement rare mais idéalement il faudrait pouvoir être à même de récupérer plus de 2 événements : A FAIRE */

    if (cur) cur = cur-> next;

    if (cur && xmlStrcmp(cur->name, (const xmlChar*) "Evenement") == 0)
    {
            cur_parent = cur;
            cur = cur->xmlChildrenNode;
            if (cur &&  !xmlIsBlankNode(cur))
            {
               info.drapeau_cont = false;
               BULLETIN_OBLIGATOIRE(Code);
               cur = cur->next;
               if (cur)
               {
                   BULLETIN_OPTIONNEL_CHAR(Description);
               }
               else
               {
                   NA_ASSIGN(Description);
               }

               info.drapeau_cont = true;
            }
            cur = cur_parent->next;
    }
    else
    {
        info.Table[info.NCumAgentXml][Code] = (xmlChar*) "";
        info.Table[info.NCumAgentXml][Description] = (xmlChar*) "";
    }

    /* Vu la rareté du 2e évenement, il est rationnel de ne pas réserver systématiquement de place en mémoire de type Description2.
     * Mieux vaut concaténer, même si le code est plus lourd et l'allocation de mémoire ponctuellement plus lente : on gagne
     * sur l'allocation-déallocation d'un très grand nombre de champs Description2 non remplis. */

    if (cur && xmlStrcmp(cur->name, (const xmlChar*) "Evenement") == 0)
    {
            cur_parent = cur;
            cur = cur->xmlChildrenNode;
            if (cur &&  !xmlIsBlankNode(cur))
            {
               info.drapeau_cont = false;

               concat(cur, info);

               cur = cur->next;
               if (cur)
               {
                   concat(cur, info);
               }

               info.drapeau_cont = true;
            }
            cur = cur_parent->next;
    }

    if (cur)
    {
        BULLETIN_OBLIGATOIRE(Service);
    }
    else
    {
        cerr << ERROR_HTML_TAG "Service introuvable." ENDL;
#ifdef STRICT
        exit(-5);
#endif
    }

#ifdef TOLERANT_TAG_HIERARCHY
    cur = cur_save;
    cur = atteindreNoeud("NBI", cur);
#endif

    BULLETIN_OBLIGATOIRE_NUMERIQUE(NBI);

    /* Problème : unbounded NBI ... */

    int v = 0;
    while (xmlStrcmp(cur->name, (const xmlChar*) "NBI") == 0)
    {
        v += atoi((const char*) xmlGetProp(cur, (const xmlChar*) "V"));
        cur =  cur->next;
    }

    if (v > 0)
    {
        xmlFree(info.Table[info.NCumAgentXml][NBI]);
        char buffer[8]={0};
        sprintf(buffer, "%d", atoi((const char*)info.Table[info.NCumAgentXml][NBI]) + v);
        info.Table[info.NCumAgentXml][NBI] = xmlStrdup((xmlChar*) buffer);
    }

#ifdef TOLERANT_TAG_HIERARCHY
    cur = cur_save;
    cur = atteindreNoeud("QuotiteTrav", cur);
#endif


    /* obligatoire, substitution du séparateur décimal */
    BULLETIN_OBLIGATOIRE_NUMERIQUE(QuotiteTrav);

    xmlNodePtr cur_save = cur;
    cur = atteindreNoeud("Remuneration", cur);

    int ligne = 0, memoire_p_ligne_allouee = 0;

    if (cur)
    {
        xmlNodePtr cur_save = cur;

        if ((cur =  cur->xmlChildrenNode) != nullptr && ! xmlIsBlankNode(cur))
        {
            LineCount result = lignePaye(cur, info);
            ligne = result.nbLignePaye;
            memoire_p_ligne_allouee = result.memoire_p_ligne_allouee;
        }
        else  
        /* si la balise <Remuneration/> est fermante ou 
         * si <Remuneration>....</Remuneration> ne contient pas de ligne de paye codée
         * alors on attribue quand même une ligne, codée NA sur tous les champs */
        {
          allouer_ligne_NA(info, ligne, memoire_p_ligne_allouee);
          if (verbeux)
          {
              lock_guard<mutex> guard(mut);
              cerr << WARNING_HTML_TAG "Ligne " << to_string(xmlGetLineNo(cur_save)) << " : Balise Remuneration sans ligne de paye."  ENDL;
          }
        }

        cur = cur_save->next;
    }
    else
    {
        {
            lock_guard<mutex> guard(mut);
            cerr << ERROR_HTML_TAG "Absence de la balise Remuneration " ENDL;
        }

        // Soit il y a des lignes de paye soit il n'y a rien
        // premier cas : il y a des lignes de paye, au maximum info.NLigne[info.NCumAgentXml]
        
        if (info.NLigne[info.NCumAgentXml])
        {
            cur = cur_save;
            
            for (int k = 0; k < nbType; ++k)
            {
                cur = atteindreNoeud(type_remuneration[k], cur);
                if (cur) break;
            }
            
            // premier sous-cas : pas de ligne de paye stricto sensu
            // on avait un cas excessivement rare d'événement codé mais sans ldp
            if (cur == nullptr)
            {
                allouer_ligne_NA(info, ligne, memoire_p_ligne_allouee);
                cur = cur_save;
                if (verbeux)
                {
                    lock_guard<mutex> guard(mut);
                    cerr << WARNING_HTML_TAG "Absence de lignes de paye également, sous la ligne " << to_string(xmlGetLineNo(cur)) <<  ENDL;
                }
            }
            else
            {
                if (verbeux)
                {
                    lock_guard<mutex> guard(mut);
                    cerr << WARNING_HTML_TAG "Lignes de paye néanmoins présentes, sous la ligne " << to_string(xmlGetLineNo(cur)) <<  ENDL;
                }
                LineCount result = lignePaye(cur, info);
                ligne = result.nbLignePaye;
                memoire_p_ligne_allouee = result.memoire_p_ligne_allouee;
            }
        }
        else
            
        // Il n'y a pas de ligne de paye. On en met quand même une remplie de NAs.
        {
          allouer_ligne_NA(info, ligne, memoire_p_ligne_allouee);
          cur = cur_save;
          if (verbeux)
          {
              lock_guard<mutex> guard(mut);
              cerr << WARNING_HTML_TAG "Absence de lignes de paye également, sous la ligne " << to_string(xmlGetLineNo(cur)) <<  ENDL;
          }
        }

#       if ! NO_DEBUG
          errorLine_t env = afficher_environnement_xhl(info, nullptr);
          cerr << env.pres;
#       endif        
        cur = cur_save->next;

#       ifdef STRICT
          exit(-4);
#       endif
    }

    /* obligatoire , substitution du sparateur décimal */


    result = BULLETIN_OPTIONNEL_NUMERIQUE(NbHeureTotal);

    cur = atteindreNoeud("NbHeureSup", cur);

    /* obligatoire, substitution du sparateur décimal. Attention : utiliser des bitwise, pas des logical && sinon le deuxième opérande peut ne pas être évalué */
    result = result & BULLETIN_OPTIONNEL_NUMERIQUE(NbHeureSup);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(MtBrut);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNet);

    info.drapeau_cont=false; // fin du niveau PayeIndivMensuel
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNetAPayer);

    if (!result)
    {
        cerr << ERROR_HTML_TAG "Problème de conformité des données sur les champs des bulletins de paye." ENDL;
#ifdef STRICT
        exit(-513);
#endif
    }

    // Rémuneration tag vide
    if (ligne == 0) ligne = 1 ;

    info.ligne_fin = cur ? xmlGetLineNo(cur) : 0;

    return ligne;
}
