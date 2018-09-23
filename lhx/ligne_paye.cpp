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
#include "ligne_paye.h"
#include "validator.h"

#include "xmlconv.h"

using namespace std;


/// \file    ligne_paye.cpp
/// \author  Fabrice Nicol
/// \brief   Ce fichier contient le code relatif au traitement individuel des lignes de paye

/// Assigne la valeur NA_STRING de type char* à l'élément courant de info.Table
/// \details Assignation sur le tas à libérer par xmlFree. \n
///          \note Sur les valeurs manquantes \n
///          Pour des variables caractères : NA (#NA_ASSIGN) \n
///          Pour des variables pseudo-numériques (caractères convertibles en numériques) :\n
///          0 (#ZERO_ASSIGN) \n
///          On peut donc garantir que \e Année, \e Mois, \e NbEnfants, \e Indice, \e NBI, \n
///          \e QuotiteTrav, \n
///          \e NbHeureTotal, \e NbHeureSup, \e MtBrut, \e MtNet, \e MtNetAPayer ne sont \n
///          jamais NA mais à 0

#define NA_ASSIGN(X)        info.Table[info.NCumAgentXml][X] = (char*) string(NA_STRING).c_str()


/// Assigne la valeur "0" de type char* à l'élément courant de info.Table
/// \details Assignation sur le tas à libérer par xmlFree.

#define ZERO_ASSIGN(X)      info.Table[info.NCumAgentXml][X] = (char*) string("0").c_str();

/* Remplace les occurrences d'un caractère séparateur à l'intérieur d'un champ par le caractère '_' qui ne doit donc jamais
   être séparateur de champ (c'est bien rare !) */

/// Remplace les occurrences d'un caractère à l'intérieur d'une chaîne char* par le \n
/// caractère '_'. \n
/// \b Windows Convertit l'encodage de la chaîne UTF-8 en Latin-1. \n
///          \b Autres Pas de conversion.
/// \param     s     Chaîne à contrôler
/// \param     sep   Caractère à nettoyer (le séparateur des bases CSV)
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


static void GCC_INLINE sanitize (string &st,  const char sep)
{
    char u[st.length() + 1];
    char* s = u;
    strcpy(s, st.c_str());
    while (*s != 0)
        {
            // Non-switchable car info.seperateur n'est pas une expression constante.
            if (*s == sep)  *s = '_';

            switch (*s)
                {

                case '\n':
                    *s = ' ';
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
    
    
    st.assign(s);
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

static inline int GCC_INLINE Bulletin (const char*  tag, XMLNode*& cur, int l, info_t& info)
{
// attention faire en sorte que cur ne soit JAMAIS nul en entrée ou en sortie

    XMLNode* nextcur = atteindreNoeud (tag, cur);

    if ( nullptr == nextcur)
        {
            return NODE_NOT_FOUND;
        }

// On a à présent la garantie que cur->ToElement()->Name() correspond à tag

    if (info.drapeau_cont)
        {
            if (nextcur->NextSibling() != nullptr)
                cur = nextcur->NextSibling();
            else
                return NO_NEXT_ITEM;  // pour garantir que cur ne devient pas nul.
        }

//     sanitisation

    sanitize (info.Table[info.NCumAgentXml][l], info.separateur);

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


static inline bool GCC_INLINE bulletin_obligatoire (const char* tag, XMLNode*& cur, int l,  info_t& info)
{

//     attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin (tag, cur, l, info))
        {
//         on sait que cur ne sera jamais nul
        case NODE_FOUND :
            return true;

        case NODE_NOT_FOUND :
        {
            LOCK_GUARD
            cerr << ERROR_HTML_TAG "Balise manquante " << tag << " avant la balise " << cur->ToElement()->Name() << ENDL;
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

    if (nullptr != cur->NextSibling())
        {
            //cur = cur->NextSibling();
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

static void GCC_INLINE substituer_separateur_decimal (string& ligne, const char decimal)
{
    for (int i = 0; i < ligne.length(); ++i)
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
        XMLNode*& cur,
        int l,
        info_t& info)
{
//     attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin (tag, cur, l, info))
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

    if (nullptr != cur->NextSibling())
        {
            cur = cur->NextSibling();
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
        XMLNode*& cur,
        int l,
        info_t& info)
{
//     attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin (tag, cur, l, info))
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
            if (verbeux) cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;

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

    if (nullptr != cur->NextSibling())
        {
            cur = cur->NextSibling();
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

static inline bool GCC_INLINE bulletin_obligatoire_numerique (const char* tag, XMLNode*& cur, int l, info_t& info)
{
    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin (tag, cur, l, info))
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
            if (verbeux) cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " << l << ENDL;

            NA_ASSIGN (l);
            break;

        case NO_NEXT_ITEM :
            cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
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

    if (nullptr != cur->NextSibling())
        {
            cur = cur->NextSibling();
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
/// Un \em drapeau est une chaîne de caractères char du type "1", "2", ..., "n",
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
/// absence des noeuds \e Libelle, \e Code, \e Base, \e Taux, \e NbUnite, \e Mt.\n
/// Cette anomalie donne lieu à appel de #NA_ASSIGN et message d'avertissement.\n
/// Elle donne lieu au décompte d'une ligne de paye (assignée de valeurs manquantes).
/// \warning Toujours s'assurer que dans ce cas l'allocation mémoire prévoit 6
/// assignations de valeur manquante dans la table d'informations.
///
/// \subsection subsec7 Cas général
/// \par
/// Dans le cas général, examen des noeuds fils.\n
/// Appel succesif de #bulletin_obligatoire à 2 reprises et #bulletin_optionnel_numerique
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

static inline LineCount lignePaye (XMLNode* cur, info_t& info)
{
    int l = BESOIN_MEMOIRE_ENTETE;

    int nbLignePaye = 0;

    unsigned int t = 0;

//  +1 pour éviter la confusion avec \0 des chaines vides
    info.Table[info.NCumAgentXml][l] = (char*) string(drapeau[t]).c_str();
    ++l;

//  Besoins en mémoire :
//    BESOIN_MEMOIRE_ENTETE [champs hors ligne] + nombre de lignes
//        + flags (maximum nbType * nb de rembobinages)

    int type_loop_counter = 0;

    while (cur != nullptr)
        {
            bool new_type = false;

            while (cur->ToElement()->Name() == (const char *) type_remuneration[t])
                {
                    // Cas rare dans lequel <Remuneration> n'existe pas
                    if (strcmp(cur->ToElement()->Name(), "NbHeureTotal") == 0 )
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
                                }

                            if (cur)
                                cerr << ERROR_HTML_TAG "Type litigieux " << cur->ToElement()->Name()
                                     << " aux alentours du matricule "
                                     << info.Table[info.NCumAgentXml][Matricule] << ENDL;
                            else
                                cerr << ERROR_HTML_TAG "Pointeur noeud courant nul" << ENDL;

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
                    info.Table[info.NCumAgentXml][l] = string(drapeau[t]);

                    ++l;
                }

//         ici on pourrait in fine se passer de ce test par compilation séparée

            if (! info.reduire_consommation_memoire)
                {
                    verifier_taille (nbLignePaye, info);
                }

//         Si on arrive à un noeud de type Commentaire, on le saute et on réinitialise
//          "gratuitement" le parcours des drapeaux.

            if (strcmp(cur->ToElement()->Name(), (const char*) "Commentaire") == 0)
                {
                    cur = cur->NextSibling();
                    t = 0;
                    --type_loop_counter; // 'Rembobinage gratuit'

                    continue; // garantit incidemment que cur != nullptr dans la boucle
                }

//      cur n'est pas nul à ce point

            cur = cur->FirstChild();

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

            bulletin_obligatoire ("Libelle", cur, l, info);

            ++l;

//      Code, obligatoire

            bulletin_obligatoire ("Code", cur, l, info);

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
                    if (cur->FirstChild() == nullptr)
                        {
                            if (verbeux)
                                {
                                    LOCK_GUARD
                                    cerr << WARNING_HTML_TAG "Pas de période de référence pour le rappel" " pour le matricule " << info.Table[info.NCumAgentXml][Matricule] << " -- Ligne";
                                    int64_t lineN = -1;
                                    cur->ToElement()->QueryInt64Attribute("V", &lineN);

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
                            cur = cur->FirstChild();
                            // On ne tient pas rigueur du manque de qualité éventuelle
                            // tellement la norme est peu respectée

                            bulletin_optionnel_char ("DateDebut", cur, l, info);

                            ++l;
                            info.drapeau_cont = false; // pas de noeud successeur
                            bulletin_optionnel_char ("DateFin", cur, l, info);
                            cur = cur->Parent()->NextSibling();
                            info.drapeau_cont = true; // noeud successeur existe
                        }
                }
            else
                {
                    NA_ASSIGN (l);
                    NA_ASSIGN (++l);
                }

            ++l;
//      cur ne sera pas nul à ce point

            ++nbLignePaye;

            cur =  cur->Parent()->NextSibling();

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

#define BULLETIN_OBLIGATOIRE_(X) bulletin_obligatoire(#X, cur, X, info)

/// Macro permettant de simplifier l'appel de #bulletin_obligatoire lorsque l'indice \n
/// de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info

#define BULLETIN_OBLIGATOIRE(X) BULLETIN_OBLIGATOIRE_(X)

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
//      <Prenom V="">{0,1}</Prenom>
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

inline void GCC_INLINE concat (XMLElement* cur, info_t& info)
{
    //char tmp[500] = {0}; 
    const char* tmp;// = &tmp[0];
    cur->QueryStringAttribute("V", &tmp);
    
    if (! tmp) return;
        
    string addCode2 = string(tmp);
    sanitize(addCode2, info.separateur);
    string desc_hyphen  = info.Table[info.NCumAgentXml][Description] + " - ";
    info.Table[info.NCumAgentXml][Description] = desc_hyphen + addCode2;
}


/// Teste si il y a un montant brut ou net payé ou remboursé non nul
/// mais pas de ligne de paye détaillant la liquidation de ce total
/// \param info table d'informations
/// \details Génère seulement un effet de bord sur la sortie d'erreur

inline void test_bulletin_irregulier (info_t& info)
{
    int brut = stoi(info.Table[info.NCumAgentXml][MtBrut]) ;
    int net = stoi(info.Table[info.NCumAgentXml][MtNetAPayer]);

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
    info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE] = string(drapeau[0]); // TraitBrut).c_str()

    for (int k = 1; k <= INDEX_MAX_COLONNNES; ++k)
        {
            info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE + k] = string(NA_STRING);
        }

    ligne = 1;
}

/// Lance le décodage des variables Bulletins de paye (Nom, Prenom, Matricule,...)
/// \param cur pointeur XML courant
/// \param info table d'informations
/// \return nombre de lignes de paye
/// \details Commence par atteindre le noeud Agent. Vérifie son identification (Nom, Prenom, Matricule, NIR, NbEnfants, Statut, EmploiMetier, Grade, Echelon, Indice)
/// Décode ensuite Evenement, Service, NBI, QuotiteTrav.
/// Appelle ensuite la fonction lignePaye qui décode les lignes de paye de la balise Remuneration.
/// Décode ensuite les champs de fin de fichier : NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer.

uint64_t  parseLignesPaye (XMLNode* cur, info_t& info)
{
    bool result = true;
    int na_assign_level = 0;
    constexpr
    const char* local_tag[] = {"Nom", "Prenom", "Matricule", "NIR", "NbEnfants",
                               "Statut", "EmploiMetier", "Grade", "Echelon", "Indice"
                              };
    XMLNode* cur_parent = cur;

    cur = atteindreNoeud ("Agent", cur);

    // Vérification de l'identification de l'agent

    if (cur == nullptr)
        {
            LOCK_GUARD
            cerr << ERROR_HTML_TAG "L'agent est non identifié pour le fichier : " << info.threads->argv[info.fichier_courant] << ENDL
                 << ERROR_HTML_TAG  "Année " << info.Table[info.NCumAgentXml][Annee] << ENDL
                 << ERROR_HTML_TAG  "Mois "  << info.Table[info.NCumAgentXml][Mois]  << ENDL;

            long lineN = -1;
            cur->ToElement()->QueryInt64Attribute("V", &lineN);

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
                    && ! info.Table[info.NCumAgentXml - 1][Matricule].empty())

                cerr << WARNING_HTML_TAG "Matricule précédent : " << info.Table[info.NCumAgentXml - 1][Matricule] << ENDL;

            for (int l :
                    {
                        Nom, Prenom, Matricule, NIR, EmploiMetier, Statut, NbEnfants, Grade, Echelon, Indice
                    })
                {
                    info.Table[info.NCumAgentXml][l] = string("");
                }

            cur = cur_parent;
            cur = atteindreNoeud ("Service", cur);
            goto level0;
        }

    // cur n'est pas nul à ce point
    // Décodage des caractéristiques de l'agent : Nom, Prenom, etc.

    cur_parent = cur;
    cur = cur->FirstChild();

    if (cur == nullptr ||  cur->NoChildren()) return 0;

#ifdef TOLERANT_TAG_HIERARCHY
    XMLElement* cur_save = cur;
#endif

    // dans certains schémas on peut ne pas avoir la civilité
    // passer à la balise adjacente après lecture

    info.drapeau_cont = true;

    // if (result) va garantir notamment que le pointeur cur filé implicitement est non nul

    result    = BULLETIN_OBLIGATOIRE (Nom);

    if (result)
        {
            result &= BULLETIN_OBLIGATOIRE (Prenom);

            if (result)
                {
                    result &= BULLETIN_OBLIGATOIRE (Matricule);

                    if (result)
                        {
                            result &= BULLETIN_OBLIGATOIRE (NIR);

#ifdef TOLERANT_TAG_HIERARCHY       // on refait le parcours depuis le haut en cas d'ordre inexact des balises
                            cur = cur_save;
#endif

                            if (result)
                                {
                                    result &= BULLETIN_OBLIGATOIRE (NbEnfants);

                                    if (result)
                                        {
                                            result &= BULLETIN_OBLIGATOIRE (Statut);

                                            //  NOTA : on ne contrôle pas le respect du champ Adresse, normalement obligatoire
                                            //  et situé entre NIR et NbEnfants, ce champ devant être regardé comme trop volatile
                                            //  pour que le contrôle s'y attarde.

                                            if (result)
                                                {
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

                                                                    // ne pas lire la balise adjacente : fin du niveau subordonné Agent

                                                                    info.drapeau_cont = false;

                                                                    if (result)
                                                                        {
#ifdef TOLERANT_TAG_HIERARCHY
                                                                            cur = cur_save;
#endif

                                                                            result &= BULLETIN_OBLIGATOIRE_NUMERIQUE (Indice);

                                                                            if (! result) na_assign_level = 10;
                                                                        }
                                                                    else na_assign_level = 9;

                                                                }
                                                            else na_assign_level = 8;

                                                        }
                                                    else na_assign_level = 7;
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

// Pas de break
// Ce switch permet d'assigner NA ou 0 sélectivement aux variables d'identification de l'agent qui ne sont pas
// identifiées faute de données adéquates. Pour une variable de nature caractère (y compris le NIR),
// assignation de NA; Pour Indice et NbEnfants, assignation de 0.

    switch (na_assign_level)
        {
        case 1:
            NA_ASSIGN (Nom);
            [[fallthrough]];

        case 2:
            NA_ASSIGN (Prenom);
            [[fallthrough]];
        case 3:
            NA_ASSIGN (Matricule);
            [[fallthrough]];
        case 4:
            NA_ASSIGN (NIR);
            [[fallthrough]];
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

                    if (verbeux)
                        {
                            for (int i = na_assign_level; i < 10; ++i)
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

    if (cur) cur = cur->NextSibling();

    if (cur && strcmp(cur->ToElement()->Name(), (const char*) "Evenement") == 0)
        {
            cur_parent = cur;
            cur = cur->FirstChild();

            if (cur &&  ! cur->NoChildren())
                {
                    info.drapeau_cont = false;
                    BULLETIN_OBLIGATOIRE (Code);
                    cur = cur->NextSibling();

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

            cur = cur_parent->NextSibling();
        }
    else
        {
            NA_ASSIGN (Code);
            NA_ASSIGN (Description);
        }

    // Vu la rareté du 2e évenement, il est rationnel de ne pas réserver systématiquement de place en mémoire de type Description2.
    // Mieux vaut concaténer, même si le code est plus lourd et l'allocation de mémoire ponctuellement plus lente : on gagne
    // sur l'allocation-déallocation d'un très grand nombre de champs Description2 non remplis.

    if (cur && strcmp(cur->ToElement()->Name(), (const char*) "Evenement") == 0)
        {
            cur_parent = cur;
            cur = cur->FirstChild();

            if (cur &&  !cur->NoChildren())
                {
                    info.drapeau_cont = false;

                    concat (cur->ToElement(), info);

                    cur = cur->NextSibling();

                    if (cur)
                        {
                            concat (cur->ToElement(), info);
                        }

                    info.drapeau_cont = true;
                }

            cur = cur_parent->NextSibling();
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

    float v = 0;  // les NBI sont en principe des entiers mais dans les faits....

    while (strcmp(cur->ToElement()->Name(), (const char*) "NBI") == 0)
        {
            float v0 = 0;
            v += atof((const char*)  cur->ToElement()->QueryFloatAttribute("V", &v0));
            cur =  cur->NextSibling();
        }

    if (v > 0)
        {
            info.Table[info.NCumAgentXml][NBI].clear();
            
            info.Table[info.NCumAgentXml][NBI] = to_string(stof(info.Table[info.NCumAgentXml][NBI]) + v);
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

    XMLNode* cur_save = cur;

    // Décodage des données de rémunération (lignes de paye)

    cur = atteindreNoeud ("Remuneration", cur);

    int ligne = 0, memoire_p_ligne_allouee = 0;

    bool pas_de_ligne_de_paye = false;

    if (cur)
        {
            XMLNode* cur_save = cur;

            // Si la balise <Remuneration> contient des fils

            if (cur != nullptr && (cur =  cur->FirstChild()) != nullptr && ! cur->NoChildren())
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

                            long  lineN = -1;
                            cur->ToElement()->QueryInt64Attribute("V", &lineN);

                            // Il y a un bug dans la bibliothèque libxml2
                            // qui dans certains cas bloque lorsque le numéro de ligne du fichier XML renvoyé par cur->ToElement()->Attribute("V")
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

            cur = cur_save->NextSibling();
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
                                    long  lineN = -1;
                                    cur->ToElement()->QueryInt64Attribute("V", &lineN);

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
                                    long  lineN = -1;
                                    cur->ToElement()->QueryInt64Attribute("V", &lineN);

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
                            long  lineN = -1;
                            cur->ToElement()->QueryInt64Attribute("V", &lineN);

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
            cur = cur_save->NextSibling();

#       ifdef STRICT
            exit (-4);
#       endif
        }

    // Décodage des autres champs : NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer

    // obligatoire , substitution du sparateur décimal

    result = BULLETIN_OPTIONNEL_NUMERIQUE (NbHeureTotal);

    cur = atteindreNoeud ("NbHeureSup", cur);

    // obligatoire, substitution du sparateur décimal. Attention : utiliser des bitwise, pas des logical && sinon le deuxième opérande peut ne pas être évalué

    result = result & BULLETIN_OPTIONNEL_NUMERIQUE (NbHeureSup);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE (MtBrut);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE (MtNet);

    info.drapeau_cont = false; // fin du niveau PayeIndivMensuel
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE (MtNetAPayer);

    // Tester si le bulletin contient des paiements globaux (MtBrut, MtNetAPayer) sans détail de la liquidation

    if (result && pas_de_ligne_de_paye)
        {
            test_bulletin_irregulier (info);
        }

    if (!result)
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

