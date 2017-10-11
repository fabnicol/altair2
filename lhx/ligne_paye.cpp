// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie,
// de modification et de redistribution accordés par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
// seule une responsabilité restreinte pèse sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les concédants successifs.
//
// A cet égard l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels
// avertis possédant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invités à charger et tester l'adéquation du
// logiciel à leurs besoins dans des conditions permettant d'assurer la
// sécurité de leurs systèmes et ou de leurs données et, plus généralement,
// à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accepté les
// termes.
//
//
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
#if !defined(__linux__) && ! defined(USE_ICONV)

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
                      LOCK_GUARD
                      cerr << ERROR_HTML_TAG "Balise manquante " << tag << " avant la balise " << cur->name << ENDL;
                    }  
                    
                    if (verbeux)
                    afficher_environnement_xhl(info, cur);
                NA_ASSIGN(l);
                break;

        case LINE_MEMORY_EXCEPTION :
                    {
                      LOCK_GUARD
                      cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
                    }
                    if (verbeux)
                    afficher_environnement_xhl(info, cur);
                NA_ASSIGN(l);
                break;

        case NO_NEXT_ITEM :
                    {
                      LOCK_GUARD
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
              //cur = cur->next;
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
             {
                 LOCK_GUARD
                 cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
             }
             NA_ASSIGN(l);
             break;

        case NO_NEXT_ITEM :
             if (verbeux)
             {
                 LOCK_GUARD
                 cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
             }
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
             {
                 LOCK_GUARD
                 cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
             }
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

                if (type_loop_counter ==  TYPE_LOOP_LIMIT)
                    cerr << ERROR_HTML_TAG "En excès du nombre de réordonnancements autorisés (" << type_loop_counter  << "/" << TYPE_LOOP_LIMIT << ")." ENDL;

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
                LOCK_GUARD
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

        /* PériodeRef, obligatoire si rappel */

        if (t == 8) // La catégorie de ligne de paye est "Rappel"
        {
            if (cur->xmlChildrenNode == nullptr)
            {
                if (verbeux)
                {
                    LOCK_GUARD
                    cerr << WARNING_HTML_TAG "Pas de période de référence pour le rappel" " pour le matricule " << info.Table[info.NCumAgentXml][Matricule] << " -- Ligne";
                    long lineN = xmlGetLineNo(cur);
                    if (lineN != 65535)
                    {
                        cerr << " " << lineN << ENDL;
                    }
                    else
                    {
                        if (info.ligne_debut.size() > info.NCumAgentXml
                             && info.ligne_fin.size() > info.NCumAgentXml)
                        {
                                cerr << "s "  << info.ligne_debut.at(info.NCumAgentXml)[0] + 1
                                     << " - " << info.ligne_fin.at(info.NCumAgentXml)[0] + 1 << ENDL;
                        }
                    }
                }
                NA_ASSIGN(l);
                NA_ASSIGN(++l);
            }
            else
            {
                cur = cur->xmlChildrenNode;
                // On ne tient pas rigueur du manque de qualité éventuelle
                // tellement la norme est peu respectée

                bulletin_optionnel_char("DateDebut", cur, l, info);

                ++l;
                info.drapeau_cont = false; // pas de noeud successeur
                bulletin_optionnel_char("DateFin", cur, l, info);
                cur = cur->parent->next;
                info.drapeau_cont = true; // noeud successeur existe
            }
        }
        else
        {
            NA_ASSIGN(l);
            NA_ASSIGN(++l);
        }

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

inline void test_bulletin_irregulier(info_t& info)
{
    int brut = atoi((const char*) info.Table[info.NCumAgentXml][MtBrut]) ;
    int net = atoi((const char*) info.Table[info.NCumAgentXml][MtNetAPayer]);
    
    if (brut || net)
    {
        LOCK_GUARD
        cerr << ERROR_HTML_TAG "Bulletin irrégulier : montant " 
               <<   (net ? "net" : "brut") 
               << " payé : " 
               << (net ? net : brut) 
               << " mais pas de ligne de paye." ENDL;
    }
}

inline void allouer_ligne_NA(info_t &info, int &ligne, int &memoire_p_ligne_allouee)
{
    info.NLigne[info.NCumAgentXml] = 1;  // 1 ldp
    memoire_p_ligne_allouee = BESOIN_MEMOIRE_ENTETE + INDEX_MAX_COLONNNES // nombre de NAs mis pour les variables de paye de la ligne
            + 1  // TraitBrut
            + 1;
    
    info.Table[info.NCumAgentXml].resize(memoire_p_ligne_allouee);
    info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE] = (xmlChar*) xmlStrdup(drapeau[0]);  // TraitBrut
    
    for (int k = 1; k <= INDEX_MAX_COLONNNES; ++k)
    {
      info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE + k] = (xmlChar*) xmlStrdup(NA_STRING);
    }
        
    ligne = 1;
}

uint64_t  parseLignesPaye(xmlNodePtr cur, info_t& info)
{
    bool result = true;
    int na_assign_level = 0;
    constexpr 
    const char* local_tag[] = {"Nom", "Prenom", "Matricule", "NIR", "NbEnfants", 
                               "Statut", "EmploiMetier", "Grade", "Echelon", "Indice"};
    xmlNodePtr cur_parent = cur;

    cur = atteindreNoeud("Agent", cur);

    if (cur == nullptr)
    {
       LOCK_GUARD
        cerr << ERROR_HTML_TAG "L'agent est non identifié pour le fichier : " << info.threads->argv[info.fichier_courant] << ENDL
               << ERROR_HTML_TAG  "Année " << info.Table[info.NCumAgentXml][Annee] << ENDL
               << ERROR_HTML_TAG  "Mois "  << info.Table[info.NCumAgentXml][Mois]  << ENDL;
        
        long lineN = xmlGetLineNo(cur_parent);
        
        cerr  << WARNING_HTML_TAG "Ligne";
        
        if (lineN != 65535)
        {
            cerr << " "
                 << lineN
                 << ENDL;
        }
        else
        {    
            if (verbeux 
                 && info.ligne_debut.size() > info.NCumAgentXml
                 && info.ligne_fin.size() > info.NCumAgentXml)
            {
                    cerr << "s "  << info.ligne_debut.at(info.NCumAgentXml)[0] + 1 
                         << " - " << info.ligne_fin.at(info.NCumAgentXml)[0] + 1 << ENDL;
            }
        }
                    
        if (info.NCumAgentXml &&  info.Table[info.NCumAgentXml - 1].size() > Matricule 
             && info.Table[info.NCumAgentXml - 1][Matricule] != nullptr)
            
             cerr << WARNING_HTML_TAG "Matricule précédent : " << info.Table[info.NCumAgentXml - 1][Matricule] << ENDL;

        for (int l : {Nom, Prenom, Matricule, NIR, EmploiMetier, Statut, NbEnfants, Grade, Echelon, Indice})
        {
           info.Table[info.NCumAgentXml][l] = xmlStrdup((xmlChar*)"");
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
                                    result &= BULLETIN_OBLIGATOIRE(Grade);

                                    if (result)
                                    {
#ifdef TOLERANT_TAG_HIERARCHY
                                        cur = cur_save;
#endif
                                        
                                        result &= BULLETIN_OBLIGATOIRE(Echelon);

                                    /* ne pas lire la balise adjacente : fin du niveau subordonné Agent*/

                                        info.drapeau_cont = false;
                                        if (result)
                                        {
#ifdef TOLERANT_TAG_HIERARCHY
                                            cur = cur_save;
#endif
                                            
                                            result &= BULLETIN_OBLIGATOIRE_NUMERIQUE(Indice);
                                            if (! result) na_assign_level = 10;
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
             NA_ASSIGN(Echelon);
        case 10:
            ZERO_ASSIGN(Indice);
        default:
        break;
    }
    
   

    if (!result)
    {
            if (na_assign_level) 
            {
                LOCK_GUARD
                cerr << ERROR_HTML_TAG "Problème de conformité des données : absence de la balise obligatoire " << local_tag[na_assign_level -1] << ENDL;
                if (verbeux)
                {
                    for (int i = na_assign_level; i < 10; ++i) 
                        cerr << ERROR_HTML_TAG "Les balises suivantes n'ont pas été décodées : " << local_tag[i] << ENDL;
                    
                    cerr << ERROR_HTML_TAG "Remontée d'un niveau" ENDL;
                }
            }
          
        
        #ifdef STRICT
           exit(-512);
        #endif
    }

    /* on remonte d'un niveau */

    cur = cur_parent;
    
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
        NA_ASSIGN(Code);
        NA_ASSIGN(Description);
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
    
    bool pas_de_ligne_de_paye = false;

    if (cur)
    {
        xmlNodePtr cur_save = cur;

        if (cur != nullptr && (cur =  cur->xmlChildrenNode) != nullptr && ! xmlIsBlankNode(cur))
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
          //allouer_ligne_NA(info, ligne, memoire_p_ligne_allouee);
            if (verbeux)
            {
            
              LOCK_GUARD
                      long lineN = xmlGetLineNo(cur_save);
              if (lineN == 65535)
              {
                  cerr << WARNING_HTML_TAG;
                  
                  if (info.ligne_debut.size() > info.NCumAgentXml && info.ligne_fin.size() > info.NCumAgentXml)
                  {
                      cerr << "Ligne " << info.ligne_debut.at(info.NCumAgentXml)[0] + 1
                           << " à " << info.ligne_fin.at(info.NCumAgentXml)[0]
                           << " :";
                  }
                  
                  cerr     << " Balise Remuneration sans ligne de paye."  ENDL;
                  
              }
              else
                  cerr << WARNING_HTML_TAG "Ligne " << lineN <<  " : Balise Remuneration sans ligne de paye."  ENDL; // apparemment ça marche ici...mais pas toujours !
            }
              
            pas_de_ligne_de_paye =  true;
          
        }

        cur = cur_save->next;
    }
    else
    {
        {
            LOCK_GUARD
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
                //allouer_ligne_NA(info, ligne, memoire_p_ligne_allouee);
                cur = cur_save;
                if (verbeux)
                {
                    LOCK_GUARD
                    long  lineN = xmlGetLineNo(cur_save);
                    
                    cerr << WARNING_HTML_TAG "Absence de lignes de paye également";
                    
                    if (lineN != 65535)
                       cerr << ", ligne : " << lineN;
                    
                    cerr << ENDL;
                    
                    if (info.ligne_debut.size() > info.NCumAgentXml
                        && info.ligne_fin.size() > info.NCumAgentXml)    
                    {
                        cerr << WARNING_HTML_TAG  "Bulletin entre les lignes "
                             << info.ligne_debut.at(info.NCumAgentXml)[0] + 1<< " et "
                             <<  info.ligne_fin.at(info.NCumAgentXml)[0] 
                             << ENDL;
                    }
                    
                }
                
                pas_de_ligne_de_paye = true;
            }
            else
            {
                if (verbeux)
                {
                    LOCK_GUARD
                    long  lineN = xmlGetLineNo(cur);
                    
                    cerr << WARNING_HTML_TAG "Lignes de paye néanmoins présentes";
                    
                    if (lineN != 65535)
                        cerr << ", ligne : " << lineN;
                    
                    cerr << ENDL;
                    
                    if (info.ligne_debut.size() > info.NCumAgentXml
                        && info.ligne_fin.size() > info.NCumAgentXml)    
                    {
                        cerr << WARNING_HTML_TAG  "Bulletin entre les lignes "
                             << info.ligne_debut.at(info.NCumAgentXml)[0] + 1<< " et "
                             <<  info.ligne_fin.at(info.NCumAgentXml)[0] 
                             << ENDL;
                    }
                }
                
                LineCount result = lignePaye(cur, info);
                ligne = result.nbLignePaye;
                memoire_p_ligne_allouee = result.memoire_p_ligne_allouee;
            }
        }
        else
        // Il n'y a pas de ligne de paye. On en met quand même une remplie de NAs.
        {
          // allouer_ligne_NA(info, ligne, memoire_p_ligne_allouee);
          cur = cur_save;
          if (verbeux)
          {
              LOCK_GUARD
              long  lineN = xmlGetLineNo(cur);
              
              cerr << WARNING_HTML_TAG "Absence de lignes de paye également";
              
              if (lineN != 65535)
                  cerr << ", ligne : " << lineN;
              
              cerr << ENDL;
              
              if (info.ligne_debut.size() > info.NCumAgentXml
                  && info.ligne_fin.size() > info.NCumAgentXml)    
              {
                  cerr << WARNING_HTML_TAG  "Bulletin entre les lignes "
                       << info.ligne_debut.at(info.NCumAgentXml)[0] + 1<< " et "
                       <<  info.ligne_fin.at(info.NCumAgentXml)[0] 
                       << ENDL;
              }
          }
          
          pas_de_ligne_de_paye =true;
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

    if (result && pas_de_ligne_de_paye)
    {
        test_bulletin_irregulier(info);
    }
    
    if (!result)
    {
        cerr << ERROR_HTML_TAG "Problème de conformité des données sur les champs des bulletins de paye." ENDL;
#ifdef STRICT
        exit(-513);
#endif
    }

    // Rémuneration tag vide
    if (ligne == 0) 
        allouer_ligne_NA(info, ligne, memoire_p_ligne_allouee);

    return ligne;
}
