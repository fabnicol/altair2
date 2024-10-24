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
#include <string.h>
#include "ligne_paye.h"
#include "validator.h"

extern mutex mut;

using namespace std;


/// \file    ligne_paye.cpp
/// \author  Fabrice Nicol
/// \brief   Ce fichier contient le code relatif au traitement individuel des lignes de paye

/// Assigne la valeur NA_STRING de type xmlChar* à l'élément courant de info.Table
/// \details Assignation sur le tas à libérer par xmlFree. \n
///          \note Sur les valeurs manquantes \n
///          Pour des variables caractères : NA (#NA_ASSIGN) \n
///          Pour des variables pseudo-numériques (caractères convertibles en numériques) :\n
///          0 (#ZERO_ASSIGN) \n
///          On peut donc garantir que \e Année, \e Mois, \e NbEnfants, \e Indice, \e NBI, \n
///          \e QuotiteTrav, \n
///          \e NbHeureTotal, \e NbHeureSup, \e MtBrut, \e MtNet, \e MtNetAPayer ne sont \n
///          jamais NA mais à 0

#define NA_ASSIGN(X)        info.Table[info.NCumAgentXml][X] = nullptr;


/// Assigne la valeur "0" de type xmlChar* à l'élément courant de info.Table
/// \details Assignation sur le tas à libérer par xmlFree.

#define ZERO_ASSIGN(X)      info.Table[info.NCumAgentXml][X] = (xmlChar*) xmlStrdup((const xmlChar*) "0")

/* Remplace les occurrences d'un caractère séparateur à l'intérieur d'un champ par le caractère '_' qui ne doit donc jamais
   être séparateur de champ (c'est bien rare !) */

/// Remplace les occurrences d'un caractère à l'intérieur d'une chaîne xmlChar* par le \n
/// caractère '_'. \n
/// \b Windows Convertit l'encodage de la chaîne UTF-8 en Latin-1. \n
///          \b Autres Pas de conversion.
/// \param     s     Chaîne à contrôler
/// \param     sep   Caractère à nettoyer (le séparateur des bases CSV)
/// \return    ret   Chaîne nettoyée ou entre guillemets.
/// \details Le caractère de remplacement ne doit jamais être séparateur de champ CSV. \n
///          Il est donc interdit d'avoir des bases de type CSV séparées par le caractère
///  '_' (au lieu de ',' ou ';').
/// \attention <pre>
///  \b Windows \n
///       Cette opération peut échouer si les hypothèses techniques suivantes,
///       relatives à la conversion Latin-1, ne sont pas remplies.
///                            a) pas de caractères spéciaux multioctets
///                            b) seuls sont convertis : à, â, ç, è, é, ê, ë, î, ï, ô, û ...
///                               et les majuscules correspondantes autrement dit, dont le
///                               code UTF-8 commence par 0xC3. Il suffit d'ajouter 0x40 sur
///                               les quatre bits hauts de l'octet.
///       Aucune vérification n'est opérée sur la réalisation de ces hypothèses.
///  \b Autres \n
///       La fonction ne convertit pas les caractères de sortie en Latin-1.
/// </pre>
/// \todo    \b Windows \n
///  Elaborer une vérification minimale des hypothèses. \n
///  Vérifier l'évolution du point suivant. \n
///  Le caractère '°' (degré) est bien codé en Latin-1 comme 0xB0, mais il y a un problème
///  avec le paquet texlive \n
///  \e inputenc pour la conversion pdf. On remplace donc par (0x65). Apparemment plus
///  nécessaire \n
///  \code if (info.Table[info.NCumAgentXml][l][i] == 0xB0)
///           info.Table[info.NCumAgentXml][l][i] = 0x65; \endcode
///  \note A surveiller en cas de développement Windows.

static inline xmlChar*  sanitize (xmlChar* s,  const char GCC_UNUSED sep)
{
#ifdef NO_SANITIZING_QUOTES
    while (*s != 0)
    {
        // Non-switchable car info.seperateur n'est pas une expression constante.
        if (*s == sep)  *s = '_';

        switch (*s)
        {

        case '\n':
        case '<':
        case '>':
        case ':':
        case '"':
        case '/':
        case '\\':
        case '|':
        case '?':
        case '*':

            *s = '_';
            break;


#ifdef CONVERTIR_LATIN_1
#if ! defined(USE_ICONV)

            //   Gros hack de pseudo-conversion UTF-8 vers Latin-1, qui permet d'économiser les 40 %
            //   de surcoût d'exécution lié à l'utilisation d'iconv pour retraiter les fichiers de
            //   sortie (fonction convertir(const char*))
            //   Ce hack est presque sans coût. Il se base sur les hypothèses suivantes :
            //       a) pas de caractères spéciaux multioctets
            //       b) seuls sont convertis : à, â, ç, è, é, ê, ë, î, ï, ô, û ... et les majuscules
            //         correspondantes càd
            //   dont le code UTF-8 commence par 0xC3. Il suffit d'ajouter 0x40 sur les quatre bits
            //   hauts de l'octet.

        case 0xC3:

            *s = ((* (s + 1) & 0xF0) + 0x40) | (* (s + 1) & 0x0F);

            effacer_char (s + 1);

            break;

        case 0xC2:

            *s = * (s + 1);
            //    Le caractère ° (degré) est bien codé en Latin-1 comme 0xB0, mais il y a un
            //    problème avec le paquet texlive inputenc pour la conversion pdf.
            //    On remplace donc par (0x65). Apparemment plus nécessaire */
            //    if (info.Table[info.NCumAgentXml][l][i] == 0xB0)
            //            info.Table[info.NCumAgentXml][l][i] = 0x65;

            effacer_char (s + 1);

            break;
#endif
#endif
        }

        ++s;
    }

    return s;

#elif defined SANITIZING_QUOTES

    unsigned long L = xmlStrlen(s);
    xmlChar ss[L+3];
    memcpy(ss + 1, s, L);
    ss[L+1] = '"';
    ss[0] = '"';
    ss[L + 2] = 0;
    xmlFree(s);
    s = xmlStrdup(ss);


#endif

    return s;
}


/// Atteint le prochain noeud de libellé donné, après un saut éventuel, et le lit.
/// \param tag  nom du noeud
/// \param cur  noeud courant
/// \param l    indice courant de la table
/// \param info table d'informations
/// \details Va au prochain noeud de libellé \a tag. \n
/// Assigne ce noeud XML dans le pointeur courant \a cur. Lit la propriété "V" de ce noeud
/// dans la table \a info à l'indice \a l de l'agent courant.\n
/// Renvoie un code d'exception \b #LINE_MEMORY_EXCEPTION en cas d'allocation mémoire
/// impossible ou \b #NO_NEXT_ITEM si \a drapeau_cont est vrai pour \a info.\n
/// Sinon appelle #sanitize et retourne \b #NODE_FOUND \n
/// \return  \b #NODE_FOUND sauf si \b #LINE_MEMORY_EXCEPTION ou \b #NO_NEXT_ITEM.

static inline int GCC_INLINE Bulletin (const char*  tag, xmlNodePtr& cur, int l, info_t& info)
{
    // attention faire en sorte que cur ne soit JAMAIS nul en entrée ou en sortie

    const xmlNodePtr nextcur = atteindreNoeud (tag, cur);

    if ( nullptr == nextcur)
    {
        return NODE_NOT_FOUND;
    }

    // On a à présent la garantie que cur->name correspond à tag


    if ((info.Table[info.NCumAgentXml][l]
         = xmlGetProp (nextcur, (const xmlChar *) "V"))
            == nullptr)

        return LINE_MEMORY_EXCEPTION;

    if (info.drapeau_cont)
    {
        if (nextcur->next != nullptr)
            cur = nextcur->next;
        else
            return NO_NEXT_ITEM;  // pour garantir que cur ne devient pas nul.
    }

    return NODE_FOUND;

}

/// Appelle #Bulletin et affiche l'interprétation des erreurs.
/// \param tag  nom du noeud
/// \param cur  noeud courant
/// \param l    indice courant de la table
/// \param info table d'informations
/// \details Si Bulletin renvoie NODE_NOT_FOUND, appelle #NA_ASSIGN et affiche
/// "Impossible d'atteindre" le noeud de libellé \a tag à partir du libellé du pointeur
/// courant. \n
/// Si Bulletin renvoie #LINE_MEMORY_EXCEPTION, appelle #NA_ASSIGN et affiche
/// "Allocation mémoire impossible" le noeud suivant de libellé \a tag. \n
/// Si Bulletin renvoie #NO_NEXT_ITEM, affiche "Pas d'item successeur pour" le noeud de
/// libellé \b tag à partir du libellé du pointeur courant.
/// \return  \b true sauf si  #Bulletin ne renvoie pas NODE_FOUND et si il n'y a pas de
/// noeud suivant.
/// \note Dans ce cas, si la compilation est définie avec STRICT, sortie du programme
/// de code -1.


static inline bool GCC_INLINE bulletin_obligatoire_char (const char* tag, xmlNodePtr& cur, int l,  info_t& info)
{

    //     attention faire en sorte que cur ne soit JAMAIS nul
    int res = Bulletin (tag, cur, l, info);

    //     sanitisation

    info.Table[info.NCumAgentXml][l] = sanitize (info.Table[info.NCumAgentXml][l], info.separateur);

    switch (res)
    {
    //         on sait que cur ne sera jamais nul
    case NODE_FOUND :
        return true;

    case NODE_NOT_FOUND :
        {
            LOCK_GUARD
                    cerr << ERROR_HTML_TAG "Balise manquante "
                         << tag << " avant la balise " << cur->name << ENDL;
        }

        if (verbeux)
            afficher_environnement_xhl (info, cur);

        NA_ASSIGN (l);
        break;

    case LINE_MEMORY_EXCEPTION :
    {
        LOCK_GUARD
                cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
    }

        if (verbeux)
            afficher_environnement_xhl (info, cur);

        NA_ASSIGN (l);
        break;

    case NO_NEXT_ITEM :
    {
        LOCK_GUARD
                cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
    }

        if (verbeux)
            afficher_environnement_xhl (info, cur);

        break;

    }

    //     Ne pas mettre de lock ici, il y en a un dans warning_msg

    warning_msg (tag, info, cur);

#ifdef STRICT
    exit (-1);
#else

    if (nullptr != cur->next)
    {
        return true;
    }
    else
        return false;

#endif
}

///   \note  <pre>
///   Cette fonction est nécessaire pour assurer une sortie convenablement lisible sous
///   tableur dans la locale française.
///   A ce stade nous stockons tous les champs lus en char, pour écriture identique en
///   .csv dans la table, avec substitution
///   'manuelle' de la virgule au point dans la chaîne en output. </pre>

///   \todo
///   Si un jour nous décidons d'utilisr \e strold pour convertir les chaînes de caractère
///   numériques en float, nous \n
///   gagnerons de la place en stockage temporaire (peut être utile pour les gros fichiers)
///   et alors \e printf et \e setlocale \n
///   feront le travail de substitution de la virgule au point lors de l'écriture de la base.


/// Substitue le séparateur décimal passé en paramètre au séparateur par défaut (.)
/// \param ligne  chaîne de caractères traitée
/// \param decimal séparateur décimal substitué à '.'
/// \note potentiellement optimisable

static inline void substituer_separateur_decimal (xmlChar* ligne, const char decimal)
{
    const int size = xmlStrlen (ligne);

    for (int i = 0; i < size; ++i)
        if (ligne[i] == '.') ligne[i] = decimal;
}

// optionnel


/// Appelle #Bulletin. Ne requiert pas qu'un noeud correspondant au libellé en premier arg.
/// soit trouvé.
/// \param tag  nom du noeud
/// \param cur  noeud courant
/// \param l    indice courant de la table
/// \param info table d'informations
/// \details Si le noeud n'est pas trouvé appelle #NA_ASSIGN.
/// Affiche l'interprétation des erreurs.\n
/// Si Bulletin renvoie #LINE_MEMORY_EXCEPTION, appelle #NA_ASSIGN et affiche
/// "Allocation mémoire impossible" le noeud suivant de libellé \a tag.
/// Si Bulletin renvoie #NO_NEXT_ITEM, affiche "Pas d'item successeur pour" le noeud de
/// libellé \b tag à partir du libellé du pointeur courant.
/// \return  \b true sauf si \b #Bulletin renvoie #NO_NEXT_ITEM ou #LINE_MEMORY_EXCEPTION \n
/// et s'il n'y a pas de noeud suivant.
/// \note Dans ce cas, si la compilation est définie avec STRICT, sortie du programme
/// de code -1.
///

static inline bool GCC_INLINE bulletin_optionnel_char (const char* tag,
                                                       xmlNodePtr& cur,
                                                       int l,
                                                       info_t& info)
{
    //     attention faire en sorte que cur ne soit JAMAIS nul
    int res = Bulletin (tag, cur, l, info);

    //     sanitisation

    info.Table[info.NCumAgentXml][l] = sanitize (info.Table[info.NCumAgentXml][l], info.separateur);

    switch(res)
    {
    //         on sait que cur ne sera jamais nul
    case NODE_FOUND :
        return true;

    case NODE_NOT_FOUND :
        NA_ASSIGN (l);
        return true;

    case LINE_MEMORY_EXCEPTION :
        if (verbeux)
        {
            LOCK_GUARD
                    cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
        }

        NA_ASSIGN (l);
        break;

    case NO_NEXT_ITEM :
        if (verbeux)
        {
            LOCK_GUARD
                    cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
        }

        break;
    }

    //     Ne pas mettre de lock ici, il y en a un dans warning_msg

    warning_msg (tag, info, cur);

#ifdef STRICT
    exit (-1);
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


/// Appelle #Bulletin. Ne s'applique que si la valeur est numérique. \n
/// Ne requiert pas qu'un noeud correspondant au libellé en premier arg. soit trouvé.
/// \param tag  nom du noeud
/// \param cur  noeud courant
/// \param l    indice courant de la table
/// \param info table d'informations
/// \details Si le noeud est trouvé, avec ou sans successseur, substitue le séparateur
/// décimal au point.\n
/// Si le noeud n'est pas trouvé ou en cas d'exception, appelle #ZERO_ASSIGN. \n
/// Affiche l'interprétation des erreurs.\n
/// Si Bulletin renvoie #LINE_MEMORY_EXCEPTION, affiche "Allocation mémoire impossible" le
/// noeud suivant de libellé \a tag.
/// Si Bulletin renvoie #NO_NEXT_ITEM, affiche "Pas d'item successeur pour" le noeud de
/// libellé \b tag à partir du libellé du pointeur courant.
/// \return  \b true sauf si \b #Bulletin renvoie #NO_NEXT_ITEM ou #LINE_MEMORY_EXCEPTION \n
/// et s'il n'y a pas de noeud suivant.
/// \note Dans ce cas, si la compilation est définie avec STRICT, sortie du programme
/// de code -1.


static inline bool GCC_INLINE bulletin_optionnel_numerique (const char* tag,
                                                            xmlNodePtr& cur,
                                                            int l,
                                                            info_t& info)
{
    //     attention faire en sorte que cur ne soit JAMAIS nul

    int res = Bulletin (tag, cur, l, info);

    switch(res)
    {
    //         on sait que cur ne sera jamais nul
    case NODE_FOUND :
#ifndef DECIMAL_NON_EN
        if (info.decimal != '.')
#endif
            substituer_separateur_decimal (info.Table[info.NCumAgentXml][l], info.decimal);

        return true;

    case NODE_NOT_FOUND :
        ZERO_ASSIGN (l);
        return true;

    case LINE_MEMORY_EXCEPTION :
        if (verbeux)
        {
            LOCK_GUARD
                    cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
        }

        ZERO_ASSIGN (l);
        break;

    case NO_NEXT_ITEM :
        if (verbeux)
        {
            LOCK_GUARD
            cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
        }

#ifndef DECIMAL_NON_EN

        if (info.decimal != '.')
#endif
            substituer_separateur_decimal (info.Table[info.NCumAgentXml][l], info.decimal);

        break;
    }

    //     Ne pas mettre de lock ici, il y en a un dans warning_msg

    warning_msg (tag, info, cur);

#ifdef STRICT
    exit (-1);
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


/// Appelle #Bulletin. Ne s'applique que si la valeur est numérique. \n
/// \param tag  nom du noeud
/// \param cur  noeud courant
/// \param l    indice courant de la table
/// \param info table d'informations
/// \details Si le noeud est trouvé, avec ou sans successseur, substitue le séparateur
/// décimal au point.\n
/// Si Bulletin renvoie #NODE_NOT_FOUND, appelle #ZERO_ASSIGN et affiche
/// "Impossible d'atteindre" le noeud de libellé \a tag à partir du libellé du pointeur
/// courant. \n
/// Si Bulletin renvoie #LINE_MEMORY_EXCEPTION, appelle #ZERO_ASSIGN et affiche
/// "Allocation mémoire impossible" le noeud suivant de libellé \a tag. \n
/// Si Bulletin renvoie #NO_NEXT_ITEM, affiche "Pas d'item successeur pour" le noeud de
/// libellé \b tag à partir du libellé du pointeur courant.
/// \return  \b true sauf si  #Bulletin ne renvoie pas NODE_FOUND et si il n'y a pas de
/// noeud suivant.
/// \note Dans ce cas, si la compilation est définie avec STRICT, sortie du programme
/// de code -1.

static inline bool GCC_INLINE bulletin_obligatoire_numerique (const char* tag, xmlNodePtr& cur, int l, info_t& info)
{
    // attention faire en sorte que cur ne soit JAMAIS nul

    int res = Bulletin (tag, cur, l, info);

    switch(res)
    {
    // on sait que cur ne sera jamais nul
    case NODE_FOUND :
#ifndef DECIMAL_NON_EN
        if (info.decimal != '.')
#endif
            substituer_separateur_decimal (info.Table[info.NCumAgentXml][l], info.decimal);

        return true;

    case NODE_NOT_FOUND :
        ZERO_ASSIGN (l);
        return true;

    case LINE_MEMORY_EXCEPTION :
        if (verbeux)
        {
            LOCK_GUARD
            cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;
        }

        NA_ASSIGN (l);
        break;

    case NO_NEXT_ITEM :
        {
           LOCK_GUARD
           cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
        }
#ifndef DECIMAL_NON_EN

        if (info.decimal != '.')
#endif
            substituer_separateur_decimal (info.Table[info.NCumAgentXml][l], info.decimal);

        break;
    }

    //    Ne pas mettre de lock ici, il y en a un dans warning_msg

    warning_msg (tag, info, cur);

#ifdef STRICT
    exit (-1);
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




/// \page page1 Documentation de l'algorithme d'analyse des noeuds Remuneration
/// \tableofcontents
///
/// \section  sec1 Spécifications de la convention cadre au 1er Oct. 2016
///
/// \subsection subsec1  Noeuds XML
/// \par
/// \code
/// <pre>
/// <PayeIndivMensuel>
///   <Agent>{1,1}</Agent>
///   <Evenement>{0,unbounded}</Evenement>
///   <Service V="">{1,1}</Service>
///   <NBI V="">{1,unbounded}</NBI>
///   <QuotiteTrav V="">{1,1}</QuotiteTrav>
///   <Periode>{1,1}</Periode>
///   <Remuneration>{1,1}</Remuneration>
///   <NbHeureTotal V="">{0,1}</NbHeureTotal>
///   <TauxHor V="">{0,1}</TauxHor>
///   <NbHeureSup V="">{1,1}</NbHeureSup>
///   <MtBrut V="">{1,1}</MtBrut>
///   <MtNet V="">{1,1}</MtNet>
///   <MtNetAPayer V="">{1,1}</MtNetAPayer>
///   <DatePaiement V="">{1,1}</DatePaiement>
///   <MtImposable V="">{1,1}</MtImposable>
///   <CumulMtImposable V="">{1,1}</CumulMtImposable>
///   <CumulMtBrut V="">{1,1}</CumulMtBrut>
///   <CumulBaseSS V="">{1,1}</CumulBaseSS>
///   <RepartitionBudget>{0,unbounded}</RepartitionBudget>
///   <PJRef>{0,unbounded}</PJRef>
/// </PayeIndivMensuel>
/// </pre>
/// \endcode
/// \warning Toute évolution significative de la convention devra donner lieu à des
///  ajustements de code dans cette fonction
///
/// \section sec2 Description de l'algorithme
///
/// A l'exécution de cette fonction le noeud courant est une catégorie de ligne de paye
/// dont le nom est une valeur du tableau #type_remuneration
/// ("TraitBrut",...,"Indemnite",..., "Commentaire").\n
///
/// \subsection subsec3 Analyse des drapeaux
/// \subsubsection drap Définition d'un drapeau
/// Un \em drapeau est une chaîne de caractères xmlChar du type "1", "2", ..., "n",
/// n < #nbType \n
/// Les drapeaux sont stockés dans le tableau statique #drapeau \n
/// Ils encodent chacune des catégories successives de ligne de paye décrites par l'annexe
/// de la convention-cadre nationale de dématérialisation.
/// Ils jouent le rôle de séparateurs entre les catégories dans la table d'informations.
/// \subsubsection cor Correspondance drapeau-nom de catégorie
/// Correspondance drapeaux-libellés des catégories de ligne de paye.\n
/// A chaque drapeau est associé une valeur du tableau de caractères #type_remuneration \n
/// Une boucle parcourt #type_remuneration jusqu'à trouver la chaîne qui correspond à la
/// valeur du nom du noeud courant. l'indice de la chaîne dans #type_remuneration est
/// celui du drapeau du tableau #drapeau copié dans la table d'informations.
/// \subsubsection rebouclage Rebouclage des noms de catégorie
/// Rebouclage du parcours en cas de non-conformité de l'ordre des noeuds de
/// catégorie de ligne de paye.\n
/// En principe les éléments constitutifs des enregistrements
/// \code <Remuneration>....</Remuneration> \endcode sont enregistrés dans l'ordre du tableau
/// #type_remuneration.\n
/// Toutefois quelques cas de désordre sont observés. Dans ces cas là on doit
/// réinitialiser le parcours du tableau.\n
/// La constante \b #TYPE_LOOP_LIMIT est définie à la compilation et si ce n'est pas le cas
/// prend la valeur par défaut encodée dans le fichier validator.h \n
/// Elle définit le maximum du nombre de réinitialisation du parcours du tableau
/// #type_remuneration autrement dit le nombre maximum de non-conformités à
/// l'ordonnancement des catégories de ligne de paye prévu par la convention-cadre.\n
/// Ce maximum n'est jamais atteint en pratique. S'il est atteint la fonction retourne
/// {\b nbLignePaye, 1}, où \b nbLignePaye est le nombre de lignes de paye effectivement
/// lues dans la fonction au cours de cet appel.\n
///
/// \subsection subsec4 Vérification de l'allocation globale
/// \par
/// Si l'allocation de la mémoire de la tableau d'informations est faite par passage en
/// paramètres de ligne de commande, il faut vérifier que l'utilisateur n'a pas
/// insuffisamment dimensionné la mémoire dans le paramètres imposés.\n
/// Cette vérification est opérée par #verifier_taille.\n
///
/// \subsection subsec5 Cas de noeuds de type Commentaire
/// \par
/// Pour éviter des problèmes de cohérence typage en base, les noeuds Commentaire
/// ne sont pas lus.
/// \par
/// En cas de noeud commentaire, le parcours de drapeaux et de #type_remuneration est
/// réinitialisé et il n'en est pas tenu compte pour la vérification du plafond
/// #TYPE_LOOP_LIMIT.
///
/// \subsection subsec6 Cas d'anomalie
/// \par
/// Une anomalie peut être l'absence de noeuds fils décrivant le contenu de la paye :\n
/// absence des noeuds \e Libellé, \e Code, \e Base, \e Taux, \e NbUnite, \e Mt.\n
/// Cette anomalie donne lieu à appel de #NA_ASSIGN et message d'avertissement.\n
/// Elle donne lieu au décompte d'une ligne de paye (assignée de valeurs manquantes).
/// \warning Toujours s'assurer que dans ce cas l'allocation mémoire prévoit 6
/// assignations de valeur manquante dans la table d'informations.
///
/// \subsection subsec7 Cas général
/// \par
/// Dans le cas général, examen des noeuds fils.\n
/// Appel successif de #bulletin_obligatoire à 2 reprises et #bulletin_optionnel_numerique
/// à 4 reprises, pour les noeuds cités \e supra. Au terme de la lecture de ces 6 noeuds
/// fils, le noeud courant est assigné au noeud \code <Remuneration> \endcode suivant.


/// Analyse les noeuds fils du noeud \code <PayeIndivMensuel> \endcode
/// \param cur  Noeud courant
/// \param info Structure contenant l'information analysée de type #info_t
/// \details Les détails de l'algorithme sont décrits dans \ref page1
/// \return  {\b nbLignePaye, 1}  de type #LineCount, \b nbLignesPaye étant le nombre de\n
/// lignes lues, sauf si #STRICT est définie (sortie de code -11) dans les
/// circonstances décrites par \ref rebouclage ou s'il y a erreur d'allocation de la copie
/// des drapeaux.

static inline LineCount lignePaye (xmlNodePtr cur, info_t& info)
{
    int l = BESOIN_MEMOIRE_ENTETE;

    int nbLignePaye = 0;

    unsigned int t = 0;

    //  +1 pour éviter la confusion avec \0 des chaines vides
    info.Table[info.NCumAgentXml][l] = (xmlChar*) xmlStrdup (drapeau[t]);
    ++l;

    //  Besoins en mémoire :
    //    BESOIN_MEMOIRE_ENTETE [champs hors ligne] + nombre de lignes
    //        + flags (maximum nbType * nb de rembobinages)

    int type_loop_counter = 0;

    while (cur != nullptr)
    {
        bool new_type = false;

        while (xmlStrcmp (cur->name, (const xmlChar *) type_remuneration[t]))
        {
            // Cas rare dans lequel <Remuneration> n'existe pas
            if (xmlStrcmp (cur->name, (const xmlChar *) "NbHeureTotal") == 0)
                return  { nbLignePaye, l};

            ++t;

            if (t == nbType)
            {
                //              En principe les éléments constitutifs des enregistrements
                //                <Remunération>....</Remuneration> sont enregistrés
                //              dans l'ordre du tableau type_remuneration. Toutefois quelques cas de
                //              désordre sont observés. Dans ces cas là on peut
                //              "rembobiner le tableau". On évite toutefois de faire une recherche
                //              ensembliste systématique, qui éviterait cela mais
                //              freinerait 99,9 % des recherches
                LOCK_GUARD
                if (++type_loop_counter < TYPE_LOOP_LIMIT)
                {
                    t = 0;
                    continue;
                }

                //              On ne rembobine qu'au maximum TYPE_LOOP_LIMIT. Si l'essai échoue,
                //              on déclenche une exception ou on retourne

                if (type_loop_counter ==  TYPE_LOOP_LIMIT)
                {

                    cerr << ERROR_HTML_TAG "En excès du nombre de réordonnancements autorisés (" << type_loop_counter  << "/" << TYPE_LOOP_LIMIT << ")." ENDL;
                    cerr << ERROR_HTML_TAG "En excès du nombre de types de lignes de paye autorisé (" << nbType << ")." ENDL;

                    cerr << ERROR_HTML_TAG "Ligne: " <<  xmlGetLineNo(cur) << ENDL;
                }

                if (cur)
                {

                    cerr << ERROR_HTML_TAG "Type litigieux " << cur->name
                         << " aux alentours du matricule "
                                     << info.Table[info.NCumAgentXml][Matricule] << ENDL;
                } else
                {

                    cerr << ERROR_HTML_TAG "Pointeur noeud courant nul" << ENDL;
                }


#ifdef STRICT
                exit (-11);
#else
                cerr << ERROR_HTML_TAG "Arrêt du décodage de la ligne de paye." << ENDL;
                return {nbLignePaye, l};
#endif
            }

            new_type = true;
        }

        if (new_type && t < nbType)
        {
            //          +1 pour éviter la confusion avec \0 des chaines vides
            if ((info.Table[info.NCumAgentXml][l]
                 = (xmlChar*) xmlStrdup (drapeau[t])) == nullptr)
            {
                LOCK_GUARD

                        if (verbeux) cerr << ERROR_HTML_TAG "Erreur dans l'allocation des drapeaux de catégories." << ENDL;

#ifdef STRICT
                exit (-12);
#else

                if (verbeux) cerr << ERROR_HTML_TAG "Arrêt du décodage de la ligne de paye." << ENDL;

#endif
                return {nbLignePaye, l};
            }

            ++l;
        }

        //         ici on pourrait in fine se passer de ce test par compilation séparée

        if (! info.reduire_consommation_memoire)
        {
            verifier_taille (nbLignePaye, info);
        }

        //         Si on arrive à un noeud de type Commentaire, on le saute et on réinitialise
        //          "gratuitement" le parcours des drapeaux.

        if (xmlStrcmp (cur->name, (const xmlChar*) "Commentaire") == 0)
        {
            cur = cur->next;
            t = 0;
            --type_loop_counter; // 'Rembobinage gratuit'

            continue; // garantit incidemment que cur != nullptr dans la boucle
        }

        //      cur n'est pas nul à ce point

        cur = cur->xmlChildrenNode;

        if (cur == nullptr)
        {
            NA_ASSIGN (l);
#if 0

            // Normalement c'est ce qu'il faudrait faire
            // Mais il n'y a pas eu de préallocation de mémoire pour ce cas

            for (short c = 0; c < 6; ++c) NA_ASSIGN (l++);

            ++nbLignePaye;

            if (verbeux) cerr << WARNING_HTML_TAG
                                 "Anomalie : la ligne de paye est vide."
                                          << ENDL;

#endif
            break;
        }

        //      cur n'est pas nul à ce point et ne devient jamais nul ci-après
        //      Libellé, obligatoire
        info.drapeau_cont = true;
        bulletin_obligatoire_char ("Libelle", cur, l, info);

        ++l;

        //      Code, obligatoire

        bulletin_obligatoire_char ("Code", cur, l, info);

        ++l;

        //      Base, si elle existe

        bulletin_optionnel_numerique ("Base", cur,  l, info);

        ++l;

        //     Taux, s'il existe

        bulletin_optionnel_numerique ("Taux", cur, l, info);

        ++l;

        //      Nombre d'unités, s'il existe

        bulletin_optionnel_numerique ("NbUnite", cur, l, info);

        ++l;

        //      Montant obligatoire

        bulletin_obligatoire_numerique ("Mt", cur, l, info);

        ++l;

        if (t == 8) // La catégorie de ligne de paye est "Rappel"
        {
            if (cur->xmlChildrenNode == nullptr)
            {
                if (verbeux)
                {
                    LOCK_GUARD
                            cerr << WARNING_HTML_TAG "Pas de période de référence pour le rappel "

                                    " pour le matricule "
                                 << info.Table[info.NCumAgentXml][Matricule]
                                 << " -- Ligne";

                    // Regrettablement xmlGetLineNo ne peut pas aller au-delà de 65535 (unsigned short).
                    long lineN = xmlGetLineNo (cur);

                    if (lineN != 65535)
                    {
                        cerr << " " << lineN << ENDL;
                    }
                    else
                    {
                        if (info.ligne_debut.size() > info.NCumAgentXml
                                && info.ligne_fin.size() > info.NCumAgentXml)
                        {
                            cerr << "s "  << info.ligne_debut.at (info.NCumAgentXml)[0] + 1
                                 << " - " << info.ligne_fin.at (info.NCumAgentXml)[0] + 1 << ENDL;
                        }
                    }
                }

                NA_ASSIGN (l);
                NA_ASSIGN (++l);
            }
            else
            {
                cur = cur->xmlChildrenNode;

                bulletin_optionnel_char ("DateDebut", cur, l, info);  // Attention pas d'accent

                ++l;
                info.drapeau_cont = false; // pas de noeud successeur
                bulletin_optionnel_char ("DateFin", cur, l, info);
                cur = cur->parent->next;
                info.drapeau_cont = true; // noeud successeur existe
            }
        }
        else
        {
            NA_ASSIGN (l);
            NA_ASSIGN (++l);
        }

#if LARGEUR == LARGEUR_MAX
            ++l;
            //    CodeCaisse, optionnel

            bulletin_optionnel_char ("CodeCaisse", cur, l, info);

            ++l;
            // Ordre, obligatoire
            info.drapeau_cont = false; // pas de noeud successeur
            bulletin_obligatoire_numerique("Ordre", cur, l, info);
#endif

        ++l;
        //      cur ne sera pas nul à ce point

        ++nbLignePaye;

        cur =  cur->parent->next;

        //      Le parent ne peut être nul
        //      attention si du code est rajouté ici il doit l'être sous garde cur != nullptr
        //      Lorsque on a épuisé tous les types licites on a nécessairement cur = nullptr et
        //      la boucle s'arrête

    }

    return  { nbLignePaye, l};
}

/// Macro permettant de simplifier l'appel de #bulletin_obligatoire lorsque l'indice \n
/// de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info

#define BULLETIN_OBLIGATOIRE(X) bulletin_obligatoire_char(#X, cur, X, info)

/// Macro permettant de simplifier l'appel de #bulletin_obligatoire_numerique lorsque \n
/// l'indice de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info

#define BULLETIN_OBLIGATOIRE_NUMERIQUE_(X)  bulletin_obligatoire_numerique(#X, cur, X, info)

/// Macro permettant de simplifier l'appel de #bulletin_obligatoire_numerique lorsque \n
/// l'indice de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info

#define BULLETIN_OBLIGATOIRE_NUMERIQUE(X) BULLETIN_OBLIGATOIRE_NUMERIQUE_(X)

/// Macro permettant de simplifier l'appel de #bulletin_optionnel_numerique lorsque l'indice \n
/// de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info

#define BULLETIN_OPTIONNEL_NUMERIQUE_(X)  bulletin_optionnel_numerique(#X, cur, X, info)

/// Macro permettant de simplifier l'appel de #bulletin_optionnel_numerique lorsque l'indice \n
/// de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info

#define BULLETIN_OPTIONNEL_NUMERIQUE(X)  BULLETIN_OPTIONNEL_NUMERIQUE_(X)

/// Macro permettant de simplifier l'appel de #bulletin_optionnel_char lorsque l'indice \n
/// de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info

#define BULLETIN_OPTIONNEL_CHAR(X)  bulletin_optionnel_char(#X, cur, X, info)


// REFERENCE

//  <Agent>
//      <Civilite V="">{0,1}</Civilite>
//      <Nom V="">{1,1}</Nom>
//      <ComplNom V="">{0,1}</ComplNom>
//      <Prénom V="">{0,1}</Prénom>
//      <Matricule V="">{1,1}</Matricule>
//      <NIR V="">{1,1}</NIR>
//      <Adresse>{1,1}</Adresse>
//      <NbEnfants V="">{1,1}</NbEnfants>
//      <Statut V="">{1,1}</Statut>
//      <RefNomenStatutaire>{0,1}</RefNomenStatutaire>
//      <EmploiMetier V="">{1,1}</EmploiMetier>
//      <Grade V="">{1,1}</Grade>
//      <Echelon V="">{1,1}</Echelon>
//      <Indice V="">{1,1}</Indice>
//      <CptBancaire>{0,1}</CptBancaire>
//   </Agent>


/// Concatène la propriété "V" du noeud cur passé en premier argument avec la chaîne
/// contenue de la table d'information en second argument à l'indice #Description
/// \param cur  pointeur courant
/// \param info table d'informations
/// \details Insère " - " entre les deux parties de la chaîne

inline void GCC_INLINE concat (xmlNodePtr cur, info_t& info)
{
    xmlChar* addCode2 = xmlGetProp (cur, (const xmlChar*) "V");

    if (addCode2)
    {
        xmlChar* desc_hyphen = xmlStrncatNew (info.Table[info.NCumAgentXml][Description],
                (const xmlChar*) " - ",
                -1);

        if (desc_hyphen)
        {
            xmlChar* desc_code2 = xmlStrncatNew (desc_hyphen,
                                                 addCode2,
                                                 -1);

            xmlFree (info.Table[info.NCumAgentXml][Description]);
            xmlFree (addCode2);
            xmlFree (desc_hyphen);
            info.Table[info.NCumAgentXml][Description]
                    = sanitize (desc_code2, info.separateur);
        }
    }
}


/// Teste si il y a un montant brut ou net payé ou remboursé non nul
/// mais pas de ligne de paye détaillant la liquidation de ce total
/// \param info table d'informations
/// \details Génère seulement un effet de bord sur la sortie d'erreur

inline void test_bulletin_irregulier (info_t& info)
{
    int brut = atoi ((const char*) info.Table[info.NCumAgentXml][MtBrut]) ;
    int net = atoi ((const char*) info.Table[info.NCumAgentXml][MtNetAPayer]);

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


/// Réalloue un ligne de paye en NA sur les variables propres aux lignes de paye (Code, Libellé, Base,...)
/// \param info table d'informations
/// \param ligne nombre de lignes (fixé à 1)
/// \param memoire_p_ligne_allouee allocation de mémoire de la ligne réinitialisée en NA
/// \details Modifie info.Table à l'index info.NCumAgentXml. Peut poser un problème en cas de préallocation insuffisante.

inline void allouer_ligne_NA (info_t &info, int &ligne, int &memoire_p_ligne_allouee)
{
    info.NLigne[info.NCumAgentXml] = 1;  // 1 ldp
    memoire_p_ligne_allouee = BESOIN_MEMOIRE_ENTETE + INDEX_MAX_COLONNNES // nombre de NAs mis pour les variables de paye de la ligne
            + 1  // TraitBrut
            + 1;

    info.Table[info.NCumAgentXml].resize (memoire_p_ligne_allouee);
    info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE] = (xmlChar*) xmlStrdup (drapeau[0]); // TraitBrut

    for (int k = 1; k <= INDEX_MAX_COLONNNES; ++k)
    {
        info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE + k] = nullptr;
    }

    ligne = 1;
}

/// Lance le décodage des variables Bulletins de paye (Nom, Prénom, Matricule,...)
/// \param cur pointeur XML courant
/// \param info table d'informations
/// \return nombre de lignes de paye
/// \details Commence par atteindre le noeud Agent. Vérifie son identification (Nom, Prénom, Matricule, NIR, NbEnfants, Statut, EmploiMetier, Grade, Echelon, Indice)
/// Décode ensuite Evenement, Service, NBI, QuotiteTrav.
/// Appelle ensuite la fonction lignePaye qui décode les lignes de paye de la balise Remuneration.
/// Décode ensuite les champs de fin de fichier : NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer.

uint64_t  parseLignesPaye (xmlNodePtr cur, info_t& info)
{
    bool result = true;
    int na_assign_level = 0;
    constexpr
    // Ne recense que les balises obligatoires !

    const char* local_tag[] = {"Nom", "Matricule",  "NIR", "Adresse", "NbEnfants",
                               "Statut", "EmploiMetier", "Grade", "Echelon", "Indice" };
    xmlNodePtr cur_parent = cur;

    cur = atteindreNoeud ("Agent", cur);

    // Vérification de l'identification de l'agent

    if (cur == nullptr)
    {
        LOCK_GUARD
                cerr << ERROR_HTML_TAG "L'agent est non identifié pour le fichier : " << info.threads->argv[info.fichier_courant] << ENDL
                     << ERROR_HTML_TAG  "Année " << info.Table[info.NCumAgentXml][Annee] << ENDL
                     << ERROR_HTML_TAG  "Mois "  << info.Table[info.NCumAgentXml][Mois]  << ENDL;

        long lineN = xmlGetLineNo (cur_parent);

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
                cerr << "s "  << info.ligne_debut.at (info.NCumAgentXml)[0] + 1
                     << " - " << info.ligne_fin.at (info.NCumAgentXml)[0] + 1 << ENDL;
            }
        }

        if (info.NCumAgentXml &&  info.Table[info.NCumAgentXml - 1].size() > Matricule
                && info.Table[info.NCumAgentXml - 1][Matricule] != nullptr)

            cerr << WARNING_HTML_TAG "Matricule précédent : " << info.Table[info.NCumAgentXml - 1][Matricule] << ENDL;

        for (int l :
        {
     #if LARGEUR >= LARGEUR_EXT
             Civilite, Adresse, RefNomenStatutaire, CptBancaire,
     #endif
             Nom, Prenom, Matricule,
             EmploiMetier, Statut, NbEnfants, Grade, Echelon, Indice, NIR
    })
        {
            NA_ASSIGN(l);
        }

        cur = cur_parent;
        cur = atteindreNoeud ("Service", cur);
        goto level0;
    }

    // cur n'est pas nul à ce point
    // Décodage des caractéristiques de l'agent : Nom, Prénom, etc.

    cur_parent = cur;
    cur = cur->xmlChildrenNode;

    if (cur == nullptr ||  xmlIsBlankNode (cur)) return 0;

#ifdef TOLERANT_TAG_HIERARCHY
    xmlNodePtr cur_save = cur;
#endif

    // dans certains schémas on peut ne pas avoir la civilité
    // passer à la balise adjacente après lecture

    info.drapeau_cont = true;

    // if (result) va garantir notamment que le pointeur cur filé implicitement est non nul

#if LARGEUR >= LARGEUR_EXT
    BULLETIN_OPTIONNEL_CHAR(Civilite);
    // Balise optionnelle, il ne faut pas bloquer la suite
#endif

        result  &= BULLETIN_OBLIGATOIRE (Nom);

        if (result)
        {
                BULLETIN_OPTIONNEL_CHAR(Prenom);

                result &= BULLETIN_OBLIGATOIRE (Matricule);

                if (result)
                {
                    result &= BULLETIN_OBLIGATOIRE (NIR);

#ifdef TOLERANT_TAG_HIERARCHY       // on refait le parcours depuis le haut en cas d'ordre inexact des balises
                    cur = cur_save;
#endif
#if LARGEUR >= LARGEUR_EXT
                    xmlNodePtr cur1 = cur;
#endif
                    if (result)
                    {
#if LARGEUR >= LARGEUR_EXT
                        result &= analyser_adresse(cur, info);

                        if (result)
                        {
#endif
                            result &= BULLETIN_OBLIGATOIRE_NUMERIQUE(NbEnfants);

                            if (result)
                            {
                                result &= BULLETIN_OBLIGATOIRE (Statut);

                                //  NOTA : on ne contrôle pas le respect du champ Adresse, normalement obligatoire
                                //  et situé entre NIR et NbEnfants, ce champ devant être regardé comme trop volatile
                                //  pour que le contrôle s'y attarde.

                                // NOTA2: on ne contrôle pas le champ optionnel:
                                //
                                // <RefNomenStatutaire>
                                //   <Code V="YYX2"/>
                                //   <Libelle V="..."/>
                                // </RefNomenStatutaire>
                                //
                                // TODO: peut être utile notamment s'agissant des élus.
                                // Problème: renseigné de manière inconsistante.

                                if (result)
                                {
#if LARGEUR >= LARGEUR_EXT
                                    cur1 = cur;
                                    cur = atteindreNoeud("RefNomenStatutaire", cur);

                                    if (cur)
                                    {
                                        xmlNodePtr cur_parent = cur;
                                        cur = cur->xmlChildrenNode;
                                        cur1 = cur;
                                        xmlChar* code = nullptr, *libelle = nullptr;
                                        result = true;

                                        ATTEINDRE_NOEUD_OPT(Code, code);
                                        ATTEINDRE_NOEUD_OPT(Libelle, libelle);

                                        int l1 = 0, l2 = 0;

                                        if (code)    l1 = xmlStrlen(code);
                                        if (libelle) l2 = xmlStrlen(libelle);

                                        xmlChar buffer[1024] = {0};
                                        if (l1)
                                        {
                                            memcpy(buffer, code, l1);
                                            buffer[l1] = ' ';
                                        }
                                        if (l2)
                                        {
                                            memcpy(buffer + l1 + 1, libelle, l2);
                                        }
                                        xmlChar* refnomenstat = xmlStrdup((const xmlChar*) buffer);
                                        if (code)    xmlFree(code);
                                        if (libelle) xmlFree(libelle);
                                        if (! refnomenstat) refnomenstat=(xmlChar*) xmlStrdup(NA_STRING);
                                        info.Table[info.NCumAgentXml][RefNomenStatutaire] = refnomenstat;
                                        cur = cur_parent;
                                    }
                                    else
                                    {
                                        cur = cur1;
                                        // Balise optionnelle
                                        // ile ne faut pas bloquer la suite...
                                    }

#endif


                                    result &= BULLETIN_OBLIGATOIRE (EmploiMetier);
                                    if (result)
                                    {
#ifdef TOLERANT_TAG_HIERARCHY
                                        cur = cur_save;
#endif
                                        result &= BULLETIN_OBLIGATOIRE (Grade);

                                        if (result)
                                        {
#ifdef TOLERANT_TAG_HIERARCHY
                                            cur = cur_save;
#endif

                                            result &= BULLETIN_OBLIGATOIRE (Echelon);

                                            // ne pas obligatoirement lire la balise adjacente : fin du niveau subordonné Agent

                                            info.drapeau_cont = false;

                                            if (result)
                                            {
                                                result &= BULLETIN_OBLIGATOIRE_NUMERIQUE (Indice);
#ifdef TOLERANT_TAG_HIERARCHY
                                                cur = cur_save;
#endif
                                                if (result)
                                                {
#if LARGEUR >= LARGEUR_EXT
                                                    analyser_compte_bancaire(cur, info);

#endif
                                                }
                                                else
                                                    na_assign_level = 10;
                                            }
                                            else
                                                na_assign_level = 9;
                                        }
                                        else
                                            na_assign_level = 8;

                                   }
                                   else
                                       na_assign_level = 7;

                               }
                                else
                                   na_assign_level = 6;


                            }
                            else
                            na_assign_level = 5;
#if LARGEUR >= LARGEUR_EXT
                        }
                        else
                            na_assign_level = 4;
#endif
                    }
                    else
                        na_assign_level = 3;
                }
                else
                    na_assign_level = 2;
        }
        else
            na_assign_level = 1;

    // Pas de break
    // Ce switch permet d'assigner NA ou 0 sélectivement aux variables d'identification de l'agent qui ne sont pas
    // identifiées si la balise n'est pas trouvée ET qu'il n'y a pas de noeud suivant.
    // Ce cas arrive notamment en cas de fichier XML corrompu.
    // Pour une variable de nature caractère (y compris le NIR),
    // assignation de NA; Pour Indice et NbEnfants, assignation de 0.

    switch (na_assign_level)
    {
    case 1:
        NA_ASSIGN (Nom);
        [[fallthrough]];

    case 2:
        NA_ASSIGN (Matricule);
        [[fallthrough]];

    case 3:
        NA_ASSIGN (NIR);
        [[fallthrough]];

#if LARGEUR >=1
    case 4:
        NA_ASSIGN (Adresse);
        [[fallthrough]];
#endif
    case 5:
        ZERO_ASSIGN (NbEnfants);
        [[fallthrough]];

    case 6:
        NA_ASSIGN (Statut);
        [[fallthrough]];

    case 7:
        NA_ASSIGN (EmploiMetier);
        [[fallthrough]];

    case 8:
        NA_ASSIGN (Grade);
        [[fallthrough]];

    case 9:
        NA_ASSIGN (Echelon);
        [[fallthrough]];

    case 10:
        ZERO_ASSIGN (Indice);
        [[fallthrough]];

    default:
        break;
    }

    if (! result)
    {
        if (na_assign_level)
        {
            LOCK_GUARD
            cerr << ERROR_HTML_TAG "Problème de conformité des données : absence de la balise obligatoire " << local_tag[na_assign_level - 1] << ENDL;
            cerr << ERROR_HTML_TAG "Ligne n°" << xmlGetLineNo(cur) << ENDL;

            if (verbeux)
            {
                for (unsigned long i = na_assign_level; i < sizeof(local_tag) / sizeof(char*); ++i)
                    cerr << ERROR_HTML_TAG "Les balises suivantes n'ont pas été décodées : " << local_tag[i] << ENDL;

                cerr << ERROR_HTML_TAG "Remontée d'un niveau" ENDL;

            }
        }

#ifdef STRICT
        exit (-512);
#endif
    }

    // on remonte d'un niveau : décodage de Evenement puis NBI

    cur = cur_parent;

#ifdef TOLERANT_TAG_HIERARCHY
    cur_save = cur;
#endif

    // Long saut vers cette étiquette dans le cas Agent non reconnu

level0:

    //  Référence
    //
    //    <Evenement>
    //      <Code V="">{1,1}</Code>
    //      <Description V="">{0,1}</Description>
    //    </Evenement>
    //

    info.drapeau_cont = true;

    // C'est extrêmement rare mais idéalement il faudrait pouvoir être à même de récupérer plus de 2 événements : A FAIRE

    if (cur) cur = cur-> next;

    if (cur && xmlStrcmp (cur->name, (const xmlChar*) "Evenement") == 0)
    {
        cur_parent = cur;
        cur = cur->xmlChildrenNode;

        if (cur &&  !xmlIsBlankNode (cur))
        {
            info.drapeau_cont = false;
            BULLETIN_OBLIGATOIRE (Code);
            cur = cur->next;

            if (cur)
            {
                BULLETIN_OPTIONNEL_CHAR (Description);
            }
            else
            {
                NA_ASSIGN (Description);
            }

            info.drapeau_cont = true;
        }

        cur = cur_parent->next;
    }
    else
    {
        NA_ASSIGN (Code);
        NA_ASSIGN (Description);
    }

    // Vu la rareté du 2e évenement, il est rationnel de ne pas réserver systématiquement de place en mémoire de type Description2.
    // Mieux vaut concaténer, même si le code est plus lourd et l'allocation de mémoire ponctuellement plus lente : on gagne
    // sur l'allocation-déallocation d'un très grand nombre de champs Description2 non remplis.

    if (cur && xmlStrcmp (cur->name, (const xmlChar*) "Evenement") == 0)
    {
        cur_parent = cur;
        cur = cur->xmlChildrenNode;

        if (cur &&  !xmlIsBlankNode (cur))
        {
            info.drapeau_cont = false;

            concat (cur, info);

            cur = cur->next;

            if (cur)
            {
                concat (cur, info);
            }

            info.drapeau_cont = true;
        }

        cur = cur_parent->next;
    }

    if (cur)
    {
        BULLETIN_OBLIGATOIRE (Service);
    }
    else
    {
        LOCK_GUARD
                cerr << ERROR_HTML_TAG "Service introuvable." ENDL;
#ifdef STRICT
        exit (-5);
#endif
    }

#ifdef TOLERANT_TAG_HIERARCHY
    cur = cur_save;
    cur = atteindreNoeud ("NBI", cur);
#endif

    BULLETIN_OBLIGATOIRE_NUMERIQUE (NBI);

    // Problème :  NBI est codé comme "unbounded" dans le modèle de BDD Xemelios
    // Or en droit il ne peut y avoir qu'une seule NBI !
    // On se contente de sommer sans relever le problème.
    // TODO : envoyer un message d'alerte sur violation de l'unicité de la NBI

    int v = 0;

    while (cur && xmlStrcmp (cur->name, (const xmlChar*) "NBI") == 0)
    {
        v += atoi ((const char*) xmlGetProp (cur, (const xmlChar*) "V"));
        cur =  cur->next;
    }

    if (v > 0)
    {
        xmlFree (info.Table[info.NCumAgentXml][NBI]);
        char buffer[12] = {0};
        sprintf (buffer, "%d", atoi ((const char*)info.Table[info.NCumAgentXml][NBI]) + v);
        info.Table[info.NCumAgentXml][NBI] = xmlStrdup ((xmlChar*) buffer);
        if (v > 1 && verbeux)
        {
            LOCK_GUARD
                    cerr << WARNING_HTML_TAG << v << " NBI détectées - " << "Année : " << info.Table[info.NCumAgentXml][Annee]
                            << ", Mois : "  << info.Table[info.NCumAgentXml][Mois]  << ", Matricule : " << info.Table[info.NCumAgentXml][Matricule] << ENDL;
        }
    }

#ifdef TOLERANT_TAG_HIERARCHY
    cur = cur_save;
    cur = atteindreNoeud ("QuotiteTrav", cur);
#endif

    // obligatoire, substitution du séparateur décimal

    BULLETIN_OBLIGATOIRE_NUMERIQUE (QuotiteTrav);

    xmlNodePtr cur_save = cur;

    // Décodage des données de rémunération (lignes de paye)

    cur = atteindreNoeud ("Remuneration", cur);

    int ligne = 0, memoire_p_ligne_allouee = 0;

    bool pas_de_ligne_de_paye = false;

    if (cur)
    {
        xmlNodePtr cur_save = cur;

        // Si la balise <Remuneration> contient des fils

        if (cur != nullptr && (cur =  cur->xmlChildrenNode) != nullptr && ! xmlIsBlankNode (cur))
        {
            // ! C'est ici qu'est lancée la fonction de décodage de la ligne de paye proprement dite

            LineCount result = lignePaye (cur, info);

            // Elle renvoie le nombre de lignes de paye ainsi que l'allocation de mémoire effective
            // après décodage XML correspondant à ces données "ligne de paye" dans memoire_p_ligne_allouee

            ligne = result.nbLignePaye;
            memoire_p_ligne_allouee = result.memoire_p_ligne_allouee;
        }
        else
            // si la balise <Remuneration/> est fermante ou
            // si <Remuneration>....</Remuneration> ne contient pas de ligne de paye codée
            // alors on attribue quand même une ligne, codée NA sur tous les champs
        {
            // on devrait utiliser la fonction allouer_ligne_NA mais le problème qui se pose est
            // celui de la compatibilité avec la préallocation de mémoire dans fonctions_auxiliaires.cpp
            // TODO : mettre en cohérence la préallocation et allouer_ligne_NA
            //
            //allouer_ligne_NA(info, ligne, memoire_p_ligne_allouee);

            if (verbeux)
            {

                LOCK_GUARD

                        long lineN = xmlGetLineNo (cur_save);

                // Il y a un bug dans la bibliothèque libxml2
                // qui dans certains cas bloque lorsque le numéro de ligne du fichier XML renvoyé par xmlGetLineNo est supérieur à 65535
                // Ce bug est semi-systématique, certains fichiers y échappent pour des raisons non déterminées
                // Lorsqu'il se manifeste, il faut utiliser le vecteur info.ligne_debut et le vecteur info.ligne_fin
                // qui indiquent pour l'agent de rang info.NCumAgentXml les numéros de lignes de début et de fin du bulletin de l'agent.
                // Manuellement, il est alors assez facile de localiser la balise Remuneration problématique par édition du fichier XML

                if (lineN == 65535)
                {
                    cerr << WARNING_HTML_TAG;

                    if (info.ligne_debut.size() > info.NCumAgentXml && info.ligne_fin.size() > info.NCumAgentXml)
                    {
                        cerr << "Ligne " << info.ligne_debut.at (info.NCumAgentXml)[0] + 1
                             << " à " << info.ligne_fin.at (info.NCumAgentXml)[0]
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
                cur = atteindreNoeud (type_remuneration[k], cur);

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
                            long  lineN = xmlGetLineNo (cur_save);

                    cerr << WARNING_HTML_TAG "Absence de lignes de paye également";

                    if (lineN != 65535)
                        cerr << ", ligne : " << lineN;

                    cerr << ENDL;

                    if (info.ligne_debut.size() > info.NCumAgentXml
                            && info.ligne_fin.size() > info.NCumAgentXml)
                    {
                        cerr << WARNING_HTML_TAG  "Bulletin entre les lignes "
                                                 << info.ligne_debut.at (info.NCumAgentXml)[0] + 1 << " et "
                                                 <<  info.ligne_fin.at (info.NCumAgentXml)[0]
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
                    long  lineN = xmlGetLineNo (cur);
                    cerr << WARNING_HTML_TAG "Lignes de paye néanmoins présentes";

                    if (lineN != 65535)
                        cerr << ", ligne : " << lineN;

                    cerr << ENDL;

                    if (info.ligne_debut.size() > info.NCumAgentXml
                            && info.ligne_fin.size() > info.NCumAgentXml)
                    {
                        cerr << WARNING_HTML_TAG  "Bulletin entre les lignes "
                                                 << info.ligne_debut.at (info.NCumAgentXml)[0] + 1 << " et "
                                                 <<  info.ligne_fin.at (info.NCumAgentXml)[0]
                                                  << ENDL;
                    }
                }

                LineCount result = lignePaye (cur, info);
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
                        long  lineN = xmlGetLineNo (cur);

                cerr << WARNING_HTML_TAG "Absence de lignes de paye également";

                if (lineN != 65535)
                    cerr << ", ligne : " << lineN;

                cerr << ENDL;

                if (info.ligne_debut.size() > info.NCumAgentXml
                        && info.ligne_fin.size() > info.NCumAgentXml)
                {
                    cerr << WARNING_HTML_TAG  "Bulletin entre les lignes "
                                         << info.ligne_debut.at (info.NCumAgentXml)[0] + 1 << " et "
                                         <<  info.ligne_fin.at (info.NCumAgentXml)[0]
                                          << ENDL;
                }
            }

            pas_de_ligne_de_paye = true;
        }

#       if ! NO_DEBUG
        errorLine_t env = afficher_environnement_xhl (info, nullptr);
        cerr << env.pres;
#       endif
        if (cur_save) cur = cur_save->next;

#       ifdef STRICT
        exit (-4);
#       endif
    }

    // Décodage des autres champs : NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer
    // obligatoire , substitution du sparateur décimal

    result = BULLETIN_OPTIONNEL_NUMERIQUE (NbHeureTotal);

#if LARGEUR == LARGEUR_MAX
    result = result & BULLETIN_OPTIONNEL_NUMERIQUE (TauxHor);
#endif

    cur = atteindreNoeud ("NbHeureSup", cur);

    // obligatoire, substitution du sparateur décimal. Attention : utiliser des bitwise, pas des logical && sinon le deuxième opérande peut ne pas être évalué

    result = result & BULLETIN_OPTIONNEL_NUMERIQUE (NbHeureSup);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE (MtBrut);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE (MtNet);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE (MtNetAPayer);

#if LARGEUR == LARGEUR_MAX
    result = result & BULLETIN_OBLIGATOIRE (DatePaiement);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE (MtImposable);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE (CumulMtImposable);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(CumulMtBrut);

    // ce drapeau signifie que l'on peut ne plus avoir de champ successeur au même niveau d'enchassement
    // après la recherche de bulletin de l'expression qui suit l'indications du drapeau.

    info.drapeau_cont = false;

    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(CumulBaseSS);
#endif

    // Pour RepartitionBudget et PJRef un implémentation exacte bouclerait, car les champs sont "unbounded"
    // Mais c'est rare.

    cur_save = cur;

#if LARGEUR >= LARGEUR_EXT
    cur = atteindreNoeud ("RepartitionBudget", cur);

    if (cur == nullptr)
    {
        NA_ASSIGN(CodeBudget);
        NA_ASSIGN(TauxBudget);
        NA_ASSIGN(MtBudget);
        cur = cur_save;
    }
    else
    {
        cur_save = cur;
        if ((cur =  cur->xmlChildrenNode) != nullptr && ! xmlIsBlankNode (cur))
        {
            BULLETIN_OBLIGATOIRE(CodeBudget);
            BULLETIN_OPTIONNEL_NUMERIQUE(TauxBudget);   // Normalement obligatoire mais la norme n'est pas toujours respectée
            BULLETIN_OPTIONNEL_NUMERIQUE(MtBudget);  // Normalement obligatoire mais la norme n'est pas toujours respectée
        }
        else
        {
            NA_ASSIGN(CodeBudget);
            NA_ASSIGN(TauxBudget);
            NA_ASSIGN(MtBudget);
        }
        cur = cur_save;
    }
#endif

#if LARGEUR == LARGEUR_MAX
    cur_save = cur;
    cur = atteindreNoeud("PJRef", cur);

    if (cur == nullptr)
    {
        NA_ASSIGN(Support);
        NA_ASSIGN(IdUnique);
        NA_ASSIGN(NomPJ);
       // cur = cur_save;
    }
    else
    {
        //cur_save = cur;
        if ((cur =  cur->xmlChildrenNode) != nullptr && ! xmlIsBlankNode (cur))
        {
            BULLETIN_OBLIGATOIRE(Support);
            BULLETIN_OBLIGATOIRE(IdUnique);
            BULLETIN_OPTIONNEL_CHAR(NomPJ);
        }
        else
        {
            NA_ASSIGN(Support);
            NA_ASSIGN(IdUnique);
            NA_ASSIGN(NomPJ);
        }
        //cur = cur_save;
    }
#endif


    // Tester si le bulletin contient des paiements globaux (MtBrut, MtNetAPayer) sans détail de la liquidation

    if (result && pas_de_ligne_de_paye)
    {
        test_bulletin_irregulier (info);
    }

    if (! result)
    {
        cerr << ERROR_HTML_TAG "Problème de conformité des données sur les champs des bulletins de paye." ENDL;
#ifdef STRICT
        exit (-513);
#endif
    }

    // Si la balise Remuneration est vide, allouer une ligne de NA

    if (ligne == 0)
        allouer_ligne_NA (info, ligne, memoire_p_ligne_allouee);

    return ligne;
}

#if LARGEUR >= LARGEUR_EXT

inline bool analyser_adresse(xmlNodePtr& cur, info_t& info)
{

    xmlNodePtr cur_save = cur;
    cur = atteindreNoeud("Adresse", cur);
    bool result = true;

    if (cur)
    {

        xmlChar *adr1 = nullptr, *adr2 = nullptr, *adr3 = nullptr,
                *cp = nullptr, *ville = nullptr;
        cur_save = cur;
        cur = cur->xmlChildrenNode;
        if (cur)
        {
            xmlNodePtr cur1 = cur;
            ATTEINDRE_NOEUD_OPT(Adr1, adr1);
            ATTEINDRE_NOEUD_OPT(Adr2, adr2);
            ATTEINDRE_NOEUD_OPT(Adr3, adr3);
            ATTEINDRE_NOEUD(CP, cp);
            ATTEINDRE_NOEUD_OPT(Ville, ville);

            #ifdef DEBUG_ATTEINDRE
              if (cur) cerr << ville << ENDL;
              else cerr << "none!" << ENDL;
            #endif

            int l1 = 0, l2 = 0, l2b = 0, l3 = 0, l4 = 0;

            if (adr1)   l1  = min(xmlStrlen(adr1),  MAX_STRLEN_ADR);
            if (adr2)   l2  = min(xmlStrlen(adr2),  MAX_STRLEN_ADR);
            if (adr3)   l2b = min(xmlStrlen(adr3),  MAX_STRLEN_ADR);
            if (cp)     l3  = min(xmlStrlen(cp),    MAX_STRLEN_ADR);
            if (ville)  l4  = min(xmlStrlen(ville), MAX_STRLEN_ADR);

            xmlChar buffer[(MAX_STRLEN_ADR + 1) * 5] = {0};

            if (l1)
            {
                memcpy(buffer, adr1, l1);
                buffer[l1] = ' ';
                ++l1;
            }

            if (l2)
            {
                memcpy(buffer + l1, adr2, l2);
                buffer[l1 + l2] = ' ';
                ++l2;
            }

            if (l2b)
            {
                memcpy(buffer + l1 + l2, adr3, l2b);
                buffer[l1 + l2 + l2b] = ' ';
                ++l2b;
            }

            if (l3)
            {
                memcpy(buffer + l1 + l2 + l2b, cp, l3);
                buffer[l1 + l2 + l2b + l3] = ' ';
                ++l3;
            }

            xmlChar* adresse;
#ifdef WITH_XMLSTRCAT
            if (buffer[0]) adresse = xmlStrdup(buffer);
            else
            {
                adresse = nullptr;
                result = false;
            }

            if (l4)
            {
               adresse = xmlStrcat(adresse, ville);
            }
#else
            if (l4)
            {
               memcpy(buffer + l1 + l2 + l2b + l3, ville, l4);
            }

            if (buffer[0])
                adresse = xmlStrdup(buffer);
            else
            {
                adresse = nullptr;
                result = false;
            }
#endif
            #ifdef DEBUG_ATTEINDRE
              cerr << adresse << ENDL;
            #endif

            if (adr1)  xmlFree(adr1);
            if (adr2)  xmlFree(adr2);
            if (adr3)  xmlFree(adr3);
            if (cp)    xmlFree(cp);
            if (ville) xmlFree(ville);

            info.Table[info.NCumAgentXml][Adresse] = adresse;

            cur = cur_save;
        }
        else
        {
            cur = cur_save;
            result = false;
        }
    }
    else
    {
        cur = cur_save;
        result = false;
    }

    return result;
}

void analyser_compte_bancaire(xmlNodePtr& cur, info_t& info)
{

    bool result = true;
    xmlNodePtr cur_save = cur;

    cur = atteindreNoeud("CptBancaire", cur);
    if (cur)
    {
        cur = cur->xmlChildrenNode;

        if (cur)
        {
            xmlChar *codeetab = nullptr, *codeguic = nullptr,
                    *idcpte = nullptr, *clerib = nullptr,
                    *libbanc = nullptr, *titcpte = nullptr,
                    *bic = nullptr, *iban = nullptr,
                    *dtebanc = nullptr;
            xmlChar buffer[2056] = {0};
            xmlNodePtr cur1;
            int l5 = 0, l6 = 0, l9 = 0;

            cur1 = cur;
            cur = atteindreNoeud("BIC", cur);
            if (cur)
            {
                bic = xmlGetProp (cur, (const xmlChar *) "V");

                cur1 = cur;
                ATTEINDRE_NOEUD(IBAN, iban);
                int l7 = 0, l8 = 0;

                if (bic)  l7 = xmlStrlen(bic);
                if (iban) l8 =  xmlStrlen(iban);

                if (l7)
                {
                    memcpy(buffer, bic, l7);
                    buffer[l7] = ' ';
                }
                if (l8)
                {
                    memcpy(buffer + l7 + 1, iban, l8);
                    buffer[l7 + 1 + l8] = ' ';
                }
                ATTEINDRE_NOEUD_OPT(LibBanc, libbanc);
                ATTEINDRE_NOEUD(TitCpte, titcpte);
                ATTEINDRE_NOEUD_OPT(DteBanc, dtebanc);
                if (libbanc) l5 = xmlStrlen(libbanc);
                if (titcpte) l6 = xmlStrlen(titcpte);
                if (dtebanc) l9 = xmlStrlen(dtebanc);
                if (l5)
                {
                    memcpy(buffer + l7 + l8 + 2, libbanc, l5);
                    buffer[l7 + l8 + 2 + l5] = ' ';
                }
                if (l6)
                {
                    memcpy(buffer + l7 + l8 + l5 + 3, titcpte, l6);
                    buffer[l7 + l8 + l5 + 3 + l6] = ' ';
                }
                if (l9)
                {
                    memcpy(buffer + l7 + l8 + l5 + l6 + 4, dtebanc, l9);
                }

            }
            else
            {
                cur = cur1;
                ATTEINDRE_NOEUD(CodeEtab, codeetab);
                ATTEINDRE_NOEUD(CodeGuic, codeguic);
                ATTEINDRE_NOEUD(IdCpte, idcpte);
                ATTEINDRE_NOEUD(CleRib, clerib);
                int l1 = 0, l2 = 0, l3 = 0, l4 = 0;
                if (codeetab)  l1 = xmlStrlen(codeetab);
                if (codeguic)  l2 = xmlStrlen(codeguic);
                if (idcpte)    l3 = xmlStrlen(idcpte);
                if (clerib)    l4 = xmlStrlen(clerib);
                if (l1)
                {
                    memcpy(buffer, codeetab, l1);
                    buffer[l1] = ' ';
                }
                if (l2)
                {
                    memcpy(buffer + l1 + 1, codeguic, l2);
                    buffer[l1 + 1 + l2] = ' ';
                }
                if (l3)
                {
                    memcpy(buffer + l1 + l2 + 2, idcpte, l3);
                    buffer[l1 + l2 + 2 + l3] = ' ';
                }
                if (l4)
                {
                    memcpy(buffer + l1 + l2 + l3 + 3, clerib, l4);
                    buffer[l1 + l2 + l3 + 3 + l4] = ' ';
                }
                ATTEINDRE_NOEUD_OPT(LibBanc, libbanc);
                ATTEINDRE_NOEUD(TitCpte, titcpte);
                ATTEINDRE_NOEUD_OPT(DteBanc, dtebanc);
                if (libbanc) l5 = xmlStrlen(libbanc);
                if (titcpte) l6 = xmlStrlen(titcpte);
                if (dtebanc) l9 = xmlStrlen(dtebanc);
                if (l5)
                {
                    memcpy(buffer + l1 + l2 + l3 + l4 + 4, libbanc, l5);
                    buffer[l1 + l2 + l3 + l4 + 4 + l5] = ' ';
                }
                if (l6)
                {
                    memcpy(buffer + l1 + l2 + l3 + l4 + l5 + 5, titcpte, l6);
                    buffer[l1 + l2 + l3 + l4 + l5 + 5 + l6] = ' ';
                }
                if (l9)
                {
                    memcpy(buffer + l1 + l2 + l3 + l4 + l5 + l6 + 6, dtebanc, l9);
                }

            }

            xmlChar* rib = xmlStrdup((const xmlChar*) buffer);
            if (bic)      xmlFree(bic);
            if (iban)     xmlFree(iban);
            if (codeetab) xmlFree(codeetab);
            if (codeguic) xmlFree(codeguic);
            if (idcpte)   xmlFree(idcpte);
            if (clerib)   xmlFree(clerib);
            if (libbanc)  xmlFree(libbanc);
            if (titcpte)  xmlFree(titcpte);
            if (dtebanc)  xmlFree(dtebanc);
            if (! rib) rib =(xmlChar*) xmlStrdup(NA_STRING);
            info.Table[info.NCumAgentXml][CptBancaire] = rib;
        }
        else
        {
            result = false;
        }
    }
    else
    {
        result = false;
    }

    if (! result && verbeux)
    {
        LOCK_GUARD
        cerr << WARNING_HTML_TAG "Données bancaires incomplètes : "
                       << "Année : " << info.Table[info.NCumAgentXml][Annee]
                       << " Mois : " << info.Table[info.NCumAgentXml][Mois];

       if (info.Table[info.NCumAgentXml][Matricule] )
           cerr << " pour le matricule : " << info.Table[info.NCumAgentXml][Matricule]
                << ENDL;
    }
    cur = cur_save;
}
#endif
