#ifndef LIGNE_PAYE
#define LIGNE_PAYE
/*  Programme �crit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est �dit�, le pr�sent code doit �tre converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'�tre compil�.
 *  En entr�e d'Altair pr�ciser encodage.entr�e en conformit� avec l'encodage du pr�sent fichier, qui sera celui de la base g�n�r�e.
 */

/*
   Constantes de compilation pouvant �tre red�finies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG
   MAX_NB_AGENTS d�termine le nombre maximal d'agents par mois potentiellement trait�s
*/

#include <iomanip>
#include <iostream>
#include <cstring>

#include "fonctions_auxiliaires.hpp"
#include "table.hpp"
#include "tags.h"
#include "thread_handler.h"

extern bool verbeux;
extern mutex mut;


#if 0
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
#endif

vector<errorLine_t> Analyseur::errorLineStack;

void Analyseur::warning_msg(const char* noeud, const info_t& info, const xmlNodePtr cur)
{
       /* pour des raisons pratiques il peut �tre n�cessaire de limiter le nombre de sorties de ce type */



    static int warning_count;
    static string fichier_last = "";

    lock_guard<mutex> guard(mut);

      #ifdef WARNING_LIMIT
       if (warning_count < WARNING_LIMIT)
       {
           ++warning_count;
           if (verbeux) cerr << WARNING_HTML_TAG "Impossible d'atteindre " << noeud << ENDL;
             errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));

       }
       else
           if (warning_count == WARNING_LIMIT)
           {
               cerr << WARNING_HTML_TAG "Impossible d'atteindre " << noeud << ". Messages d'avertissement supprim�s par la suite."  ENDL;
               warning_count = WARNING_LIMIT + 1;
               errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
           }

       if (fichier_last !=  "" && info.threads->argv[info.fichier_courant].first != fichier_last)
           warning_count = 0;

       /* on remet � z�ro le maximum d'avertissements � chaque nouveau fichier */

       fichier_last = info.threads->argv[info.fichier_courant].first;

      #else
           if (verbeux) cerr << WARNING_HTML_TAG "Impossible d'atteindre " << noeud << ENDL;
             errorLineStack.emplace_back(afficher_environnement_xhl(info, cur));
      #endif

}

static inline void GCC_INLINE  verifier_taille(const int nbLignePaye, info_t& info)
{
    if (nbLignePaye >= info.nbLigneUtilisateur)
    {
        cerr << "\
                En exc�s du nombre de lignes de paye autoris� (" << info.nbLigneUtilisateur << ")."  ENDL
                "Omettre -n ... et utiliser -L fichier_log pour d�tecter le maximum de lignes de paye dans les fichiers."  ENDL
                "Utiliser -N ce_maximum ou bien recompiler en augmentant MAX_LIGNES_PAYE, � condition de disposer d'assez de m�moire."  ENDL;

        exit(-10);
    }
}


/* NOTA Sur les valeurs manquantes
 * Pour des variables caract�res : NA (NA_ASSIGN)
 * Pour des variables pseudo-num�riques (caract�res convertibles en num�riques) : 0 (ZERO_ASSIGN)
 * On peut donc garantir que Ann�e, Mois, NbEnfants, Indice, NBI, QuotiteTrav,
 * NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer ne sont jamais NA mais � 0 */

/* obligatoire */

#define NA_ASSIGN(X)        info.Table[info.NCumAgentXml][X] = (xmlChar*) xmlStrdup(NA_STRING)
#define ZERO_ASSIGN(X)      info.Table[info.NCumAgentXml][X] = (xmlChar*) xmlStrdup((const xmlChar*) "0")

/* Remplace les occurrences d'un caract�re s�parateur � l'int�rieur d'un champ par le caract�re '_' qui ne doit donc jamais
   �tre s�parateur de champ (c'est bien rare !) */

static inline void GCC_INLINE sanitize(xmlChar* s, const char sep)
{
    int size = xmlStrlen(s);

    for (int i = 0; i < size; ++i)
    {
        // Non-switchable car info.seperateur n'est pas une expression constante.
        if (s[i] == sep)  s[i] = '_';

        switch(s[i])
        {

        case '\n':
            s[i] = ' ';
            break;


#if defined(__WIN32__) && !defined(USE_ICONV)

            /* Gros hack de pseudo-conversion UTF-8 vers Latin-1, qui permet d'�conomiser les 40 % de surco�t d'ex�cution
             * li� � l'utilisation d'iconv pour retraiter les fichiers de sortie (fonction convertir(const char*))
             * Ce hack est presque sans co�t. Il se base sur les hypoth�ses suivantes :
             *   a) pas de caract�res sp�ciaux multioctets
             *   b) seuls sont convertis : �, �, �, �, �, �, �, �, �, �, � ... et les majuscules correspondantes c�d
             * dont le code UTF-8 commence par 0xC3. Il suffit d'ajouter 0x40 sur les quatre bits hauts de l'octet. */

        case 0xC3:

            s[i] = ((s[i + 1] & 0xF0) + 0x40) | (s[i + 1] & 0x0F);
            for (int j = i + 1; s[j] != 0; ++j)
            {
                s[j] = s[j + 1];
            }
            --size;
            break;

        case 0xC2:

            s[i] = s[i + 1];
            /* Le caract�re � (degr�) est bien cod� en Latin-1 comme 0xB0, mais il y a un probl�me avec le paquet texlive
             * inputenc pour la conversion pdf. On remplace donc par e (0x65) */

            //if (info.Table[info.NCumAgentXml][l][i] == 0xB0) info.Table[info.NCumAgentXml][l][i] = 0x65;
            for (int j = i + 1; s[j] != 0; ++j)
            {
                s[j] = s[j + 1];
            }
            --size;
            break;
#endif
        }
    }
}



static inline int GCC_INLINE Bulletin(const char*  tag, xmlNodePtr& cur, int l, info_t& info)
{
    // attention faire en sorte que cur ne soit JAMAIS nul en entr�e ou en sortie

    const xmlNodePtr nextcur = move(atteindreNoeud(tag, cur));

    if ( nullptr == nextcur)
    {
        return NODE_NOT_FOUND;
    }

    // On a � pr�sent la garantie que cur->name correspond � tag


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


/* obligatoire, mais possibilit� de fallback si STRICT n'est pas d�fini */

//             cerr << ERROR_HTML_TAG "Noeud courant null au stade de la v�rification de " << tag << ENDL;

static inline bool GCC_INLINE bulletin_obligatoire(const char* tag, xmlNodePtr& cur, int l,  info_t& info)
{

    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info))
    {
    // on sait que cur ne sera jamais nul
    case NODE_FOUND : return true;

    case NODE_NOT_FOUND :
        //if (verbeux)
        cerr << ERROR_HTML_TAG "Impossible d'atteindre " << tag << " � partir de " << cur->name << ENDL;
        NA_ASSIGN(l);
        break;

    case LINE_MEMORY_EXCEPTION :
        //if (verbeux)
        cerr << ERROR_HTML_TAG "Allocation m�moire impossible pour la ligne " << l << ENDL;
        NA_ASSIGN(l);
        break;

    case NO_NEXT_ITEM :
        //if (verbeux)
        cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
        break;

    }

    /* Ne pas mettre de lock ici, il y en a un dans warning_msg */

    Analyseur::warning_msg(tag, info, cur);

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
            cerr << ERROR_HTML_TAG "Allocation m�moire impossible pour la ligne " << l << ENDL;
        NA_ASSIGN(l);
        break;

    case NO_NEXT_ITEM :
        if (verbeux) cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
        break;
    }

    /* Ne pas mettre de lock ici, il y en a un dans warning_msg */

    Analyseur::warning_msg(tag, info, cur);

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
            cerr << ERROR_HTML_TAG "Allocation m�moire impossible pour la ligne " << l << ENDL;
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

    Analyseur::warning_msg(tag, info, cur);

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


/* obligatoire et avec substitution s�parateur d�cimal */

static inline bool GCC_INLINE bulletin_obligatoire_numerique(const char* tag, xmlNodePtr& cur, int l, info_t& info, int normalJump GCC_UNUSED = 0 )
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
        if (verbeux) cerr << ERROR_HTML_TAG "Allocation m�moire impossible pour la ligne " << l << ENDL;
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

    Analyseur::warning_msg(tag, info, cur);

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


#define BULLETIN_OBLIGATOIRE_(X) bulletin_obligatoire(#X, cur, X, info)
#define BULLETIN_OBLIGATOIRE(X) BULLETIN_OBLIGATOIRE_(X)

#define BULLETIN_OBLIGATOIRE_NUMERIQUE_(X)  bulletin_obligatoire_numerique(#X, cur, X, info)
#define BULLETIN_OBLIGATOIRE_NUMERIQUE(X) BULLETIN_OBLIGATOIRE_NUMERIQUE_(X)

#define BULLETIN_OPTIONNEL_NUMERIQUE_(X)  bulletin_optionnel_numerique(#X, cur, X, info)
#define BULLETIN_OPTIONNEL_NUMERIQUE(X)  BULLETIN_OPTIONNEL_NUMERIQUE_(X)

#define BULLETIN_OPTIONNEL_CHAR(X)  bulletin_optionnel_char(#X, cur, X, info)

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


static inline LineCount lignePaye(xmlNodePtr cur, info_t& info)
{
    int l = BESOIN_MEMOIRE_ENTETE;

    int nbLignePaye = 0;

    unsigned int t = 0;

    info.Table[info.NCumAgentXml][l] = (xmlChar*) xmlStrdup(drapeau[t]);  // +1 pour �viter la confusion avec \0 des chaines vides
    ++l;

    /* Besoins en m�moire : BESOIN_MEMOIRE_ENTETE [champs hors ligne] + nombre de lignes + flags (maximum nbType * nb de rembobinages) */
    int type_loop_counter = 0;

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

                if (++type_loop_counter < TYPE_LOOP_LIMIT)
                {
                    t = 0;
                    continue;
                }

                /* On ne rembobine qu'au maximum TYPE_LOOP_LIMIT. Si l'essai �choue, on d�clenche une exception ou on retourne */

                cerr << ERROR_HTML_TAG "En exc�s du nombre de types de lignes de paye autoris� (" << nbType << ")." ENDL;
                if (cur)
                    cerr << ERROR_HTML_TAG "Type litigieux " << cur->name << " aux alentours du matricule " << info.Table[info.NCumAgentXml][Matricule] << ENDL;
                else
                    cerr << ERROR_HTML_TAG "Pointeur noeud courant nul" << ENDL;

#ifdef STRICT
                exit(-11);
#else
                cerr << ERROR_HTML_TAG "Arr�t du d�codage de la ligne de paye." << ENDL;
                return {nbLignePaye, l};
#endif
            }

            new_type = true;
        }

        if (new_type && t < nbType)
        {
            // +1 pour �viter la confusion avec \0 des chaines vides
            if ((info.Table[info.NCumAgentXml][l] = (xmlChar*) xmlStrdup(drapeau[t])) == nullptr)
            {
                if (verbeux) cerr << ERROR_HTML_TAG "Erreur dans l'allocation des drapeaux de cat�gories." << ENDL;
#ifdef STRICT
                exit(-12);
#else
                if (verbeux) cerr << ERROR_HTML_TAG "Arr�t du d�codage de la ligne de paye." << ENDL;
#endif
                return {nbLignePaye, l};
            }

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
            --type_loop_counter; // 'Rembobinage gratuit'
            continue; // garantit incidemment que cur != nullptr dans la boucle
        }

        // cur n'est pas nul � ce point

        cur = cur->xmlChildrenNode;

        if (cur == nullptr)
        {
            NA_ASSIGN(l);
            break;
        }

        // cur n'est pas nul � ce point et ne devient jamais nul ci-apr�s

        /* Libell�, obligatoire */

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
        /* Nombre d'unit�s, s'il existe */

        bulletin_optionnel_numerique("NbUnite", cur, l, info);

        ++l;
        /* Montant, obligatoire */

        bulletin_obligatoire_numerique("Mt", cur, l, info);

        ++l;
        // cur ne sera pas nul � ce point

        ++nbLignePaye;

        cur =  cur->parent->next;
        // le parent ne peut �tre nul

        // attention si du code est rajout� ici il doit l'�tre sous garde cur != nullptr
        // Lorsque on a �puis� tous les types licites on a n�cessairement cur = nullptr et la boucle s'arr�te
    }


    return  { nbLignePaye, l};
}



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

inline uint64_t  GCC_INLINE parseLignesPaye(xmlNodePtr cur, info_t& info, ofstream& log)
{
    bool result = true;
    int na_assign_level = 0;

    xmlNodePtr cur_parent = cur;

    cur = atteindreNoeud("Agent", cur);

    if (cur == nullptr)
    {

        string temp_logpath =getexecpath();

        cerr << ERROR_HTML_TAG "Agent non identifi�. Consulter le fichier erreur.log sous " << temp_logpath  << " pour avoir les d�tails de l'incident." ENDL;

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

            log << "\n\nErreur : L'agent est non identifi� pour le fichier : " << info.threads->argv[info.fichier_courant].first << "\n"
                << "Ann�e " << info.Table[info.NCumAgentXml][Annee] << "\n"
                << "Mois "  << info.Table[info.NCumAgentXml][Mois]  << "\n\n";

            if (info.NCumAgentXml && info.Memoire_p_ligne[info.NCumAgentXml - 1] > Matricule && info.Table[info.NCumAgentXml - 1][Matricule] != nullptr)
                log << "Matricule pr�c�dent : " << info.Table[info.NCumAgentXml - 1][Matricule] << "\n\n";

            log.flush();
            log.seekp(ios_base::end);

            if (log_to_be_closed) log.close();
        }
        else
            cerr << ERROR_HTML_TAG " Impossible d'�crire le log des erreurs." ENDL;

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

    /* if (result) va garantir notamment que le pointeur cur fil� implicitement est non nul */



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
                        /* NOTA : on ne contr�le pas le respect du champ Adresse, normalement obligatoire
                                                * et situ� entre NIR et NbEnfants, ce champ devant �tre regard� comme trop volatile
                                                * pour que le contr�le s'y attarde. */
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
                                        /* ne pas lire la balise adjacente : fin du niveau subordonn� Agent*/

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
        cerr << ERROR_HTML_TAG "Probl�me de conformit� des donn�es [512]" ENDL;
#ifdef STRICT
        exit(-512);
#endif
    }

    /* on remonte d'un niveau */

    cur = cur_parent;
    if (!result && verbeux) cerr << ERROR_HTML_TAG "Remont�e d'un niveau" ENDL;

#ifdef TOLERANT_TAG_HIERARCHY
    cur_save = cur;
#endif

    /* Long saut vers cette �tiquette dans le cas Agent non reconnu */

level0:

    /*  R�f�rence
  *
  *    <Evenement>
  *      <Code V="">{1,1}</Code>
  *      <Description V="">{0,1}</Description>
  *    </Evenement>
  */

    info.drapeau_cont = true;

    /* C'est extr�mement rare mais id�alement il faudrait pouvoir �tre � m�me de r�cup�rer plus de 2 �v�nements : A FAIRE */

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

    /* Vu la raret� du 2e �venement, il est rationnel de ne pas r�server syst�matiquement de place en m�moire de type Description2.
     * Mieux vaut concat�ner, m�me si le code est plus lourd et l'allocation de m�moire ponctuellement plus lente : on gagne
     * sur l'allocation-d�allocation d'un tr�s grand nombre de champs Description2 non remplis. */

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

    /* Probl�me : unbounded NBI ... */

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


    /* obligatoire, substitution du s�parateur d�cimal */
    BULLETIN_OBLIGATOIRE_NUMERIQUE(QuotiteTrav);

#ifdef TOLERANT_TAG_HIERARCHY
    cur = cur_save;
    cur = atteindreNoeud("Remuneration", cur);
#else
    cur = cur->next;
    //cur = atteindreNoeud("Remuneration", cur);

#endif

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

        /* si la balise <Remuneration/> est fermante ou si <Remuneration>....</Remuneration> ne contient pas de ligne de paye cod�e
         * alors on attribue quand m�me une ligne, cod�e NA sur tous les champs */

        if (ligne == 0)
        {
            for (int k = 0; k <= INDEX_MAX_COLONNNES; ++k)
            {
                info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE + k] = (xmlChar*) xmlStrdup(NA_STRING);
            }
            info.Memoire_p_ligne[info.NCumAgentXml] = BESOIN_MEMOIRE_ENTETE + INDEX_MAX_COLONNNES + 1;
        }
        else
            info.Memoire_p_ligne[info.NCumAgentXml] = memoire_p_ligne_allouee;

        cur = cur_save->next;
    }
    else
    {
        perror(ERROR_HTML_TAG "R�mun�ration introuvable.");
#ifdef STRICT
        exit(-4);
#endif
    }

    /* obligatoire , substitution du sparateur d�cimal */


    result = BULLETIN_OPTIONNEL_NUMERIQUE(NbHeureTotal);

    cur = atteindreNoeud("NbHeureSup", cur);

    /* obligatoire, substitution du sparateur d�cimal. Attention : utiliser des bitwise, pas des logical && sinon le deuxi�me op�rande peut ne pas �tre �valu� */
    result = result & BULLETIN_OPTIONNEL_NUMERIQUE(NbHeureSup);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(MtBrut);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNet);

    info.drapeau_cont=false; // fin du niveau PayeIndivMensuel
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNetAPayer);

    if (!result)
    {
        cerr << ERROR_HTML_TAG "Probl�me de conformit� des donn�es sur les champs des bulletins de paye." ENDL;
#ifdef STRICT
        exit(-513);
#endif
    }

    // R�muneration tag vide
    if (ligne == 0) ligne = 1 ;

    return ligne;
}

#endif
