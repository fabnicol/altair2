#include "ligne_paye.hpp"
#include "validator.hpp"

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

#define NA_ASSIGN(X)        info.Table[info.NCumAgentXml][X] = (xmlChar*) xmlStrdup(NA_STRING)


/// Assigne la valeur "0" de type xmlChar* à l'élément courant de info.Table
/// \details Assignation sur le tas à libérer par xmlFree.

#define ZERO_ASSIGN(X)info.Table[info.NCumAgentXml][X] = (xmlChar*) xmlStrdup((const xmlChar*) "0")


/// Remplace les occurrences d'un caractère à l'intérieur d'une chaîne xmlChar* par le \n 
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


static void GCC_INLINE sanitize(xmlChar* s,  const char sep)
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

            *s = ((*(s + 1) & 0xF0) + 0x40) | (*(s + 1) & 0x0F);

            effacer_char(s + 1);

            break;

        case 0xC2:

            *s = *(s + 1);
//    Le caractère ° (degré) est bien codé en Latin-1 comme 0xB0, mais il y a un 
//    problème avec le paquet texlive inputenc pour la conversion pdf.
//    On remplace donc par (0x65). Apparemment plus nécessaire */
//    if (info.Table[info.NCumAgentXml][l][i] == 0xB0)
//            info.Table[info.NCumAgentXml][l][i] = 0x65;

            effacer_char(s + 1);

            break;
#endif
#endif
     }

        ++s;
    }
}


/// Atteint le prochain noeud de libellé donné, après un saut éventuel, et le lit.
/// \param tag  nom du noeud
/// \param cur  noeud courant
/// \param l    indice courant de la table
/// \param info table d'informations
/// \param normalJump  nombre de noeuds sautés (défaut 0)
/// \details Va au prochain noeud de libellé \a tag, après un saut éventuel \a normalJump. \n
/// Assigne ce noeud XML dans le pointeur courant \cur. Lit la propriété "V" de ce noeud 
/// dans la table \a info à l'indice \a l de l'agent courant.\n
/// Renvoie un code d'exception \b #LINE_MEMORY_EXCEPTION en cas d'allocation mémoire 
/// impossible ou \b #NO_NEXT_ITEM si \a drapeau_cont est vrai pour \a #info.\n
/// Sinon appelle #sanitize et retourne \b #NODE_FOUND \n
/// \return  \b #NODE_FOUND sauf si \b #LINE_MEMORY_EXCEPTION ou \b #NO_NEXT_ITEM.

static int GCC_INLINE Bulletin(const char*  tag,
                               xmlNodePtr& cur,
                               int l,
                               info_t& info,
                               int normalJump = 0)
{
// attention faire en sorte que cur ne soit JAMAIS nul en entrée ou en sortie

    const xmlNodePtr nextcur = move(atteindreNoeud(tag, cur, normalJump));

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

//     sanitisation 

       sanitize(info.Table[info.NCumAgentXml][l], info.separateur);

       return NODE_FOUND;

}

/// Appelle #Bulletin et affiche l'interprétation des erreurs.
/// \param tag  nom du noeud
/// \param cur  noeud courant
/// \param l    indice courant de la table
/// \param info table d'informations
/// \param normalJump  nombre de noeuds sautés (défaut 0)
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


static bool GCC_INLINE bulletin_obligatoire(const char* tag, 
                                            xmlNodePtr& cur,
                                            int l, 
                                            info_t& info,
                                            int normalJump = 0)
{

//     attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info, normalJump))
    {
//         on sait que cur ne sera jamais nul
        case NODE_FOUND : return true;

        case NODE_NOT_FOUND :
//                if (verbeux)
                    cerr << ERROR_HTML_TAG "Impossible d'atteindre " << tag 
                         << " à partir de " << cur->name << ENDL;
                NA_ASSIGN(l);
                break;

        case LINE_MEMORY_EXCEPTION :
//                if (verbeux)
                    cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " 
                         << l << ENDL;
                NA_ASSIGN(l);
                break;

        case NO_NEXT_ITEM :
//                if (verbeux)
                    cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " 
                         << tag <<  ENDL;
                break;

    }

//     Ne pas mettre de lock ici, il y en a un dans warning_msg 

    warning_msg(tag, info, cur);

        #if  STRICT
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

static void GCC_INLINE substituer_separateur_decimal(xmlChar* ligne, const char decimal)
{
    const int size = xmlStrlen(ligne);
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
/// \param normalJump  nombre de noeuds sautés (défaut 0)
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

static inline bool GCC_INLINE bulletin_optionnel_char(const char* tag, 
                                                      xmlNodePtr& cur,
                                                      int l,
                                                      info_t& info)
{
//     attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info, 0))
    {
//         on sait que cur ne sera jamais nul
        case NODE_FOUND :
             return true;

        case NODE_NOT_FOUND :
            cerr << ERROR_HTML_TAG "Impossible d'atteindre " << tag 
                 << " à partir de " << cur->name << ENDL;
        
             NA_ASSIGN(l);
             return true;

        case LINE_MEMORY_EXCEPTION :
             if (verbeux)
                 cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne "
                      << l << ENDL;
             NA_ASSIGN(l);
             break;

        case NO_NEXT_ITEM :
             if (verbeux) cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " 
                               << tag <<  ENDL;
             break;
    }

//     Ne pas mettre de lock ici, il y en a un dans warning_msg 

    warning_msg(tag, info, cur);

        #if  STRICT
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


/// Appelle #Bulletin. Ne s'applique que si la valeur est numérique. \n
/// Ne requiert pas qu'un noeud correspondant au libellé en premier arg. soit trouvé. 
/// \param tag  nom du noeud
/// \param cur  noeud courant
/// \param l    indice courant de la table
/// \param info table d'informations
/// \param normalJump  nombre de noeuds sautés (défaut 0)
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


static inline bool GCC_INLINE bulletin_optionnel_numerique(const char* tag, 
                                                           xmlNodePtr& cur,
                                                           int l,
                                                           info_t& info, 
                                                           int normalJump = 0)
{
//     attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info, normalJump))
    {
//         on sait que cur ne sera jamais nul
        case NODE_FOUND :
             #ifndef DECIMAL_NON_EN
               if (info.decimal != '.')
             #endif
                substituer_separateur_decimal(info.Table[info.NCumAgentXml][l],
                                              info.decimal);
             return true;

        case NODE_NOT_FOUND :
            cerr << ERROR_HTML_TAG "Impossible d'atteindre " << tag 
                 << " à partir de " << cur->name << ENDL;
        
             ZERO_ASSIGN(l);
             return true;

        case LINE_MEMORY_EXCEPTION :
             if (verbeux)
                 cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne " 
                      << l << ENDL;
             ZERO_ASSIGN(l);
             break;

        case NO_NEXT_ITEM :
             if (verbeux) cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " 
                               << tag <<  ENDL;
             #ifndef DECIMAL_NON_EN
              if (info.decimal != '.')
             #endif
               substituer_separateur_decimal(info.Table[info.NCumAgentXml][l],
                                             info.decimal);
             break;
    }

//     Ne pas mettre de lock ici, il y en a un dans warning_msg 

    warning_msg(tag, info, cur);

        #if  STRICT
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


/// Appelle #Bulletin. Ne s'applique que si la valeur est numérique. \n
/// \param tag  nom du noeud
/// \param cur  noeud courant
/// \param l    indice courant de la table
/// \param info table d'informations
/// \param normalJump  nombre de noeuds sautés (défaut 0)
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

static inline bool GCC_INLINE bulletin_obligatoire_numerique(const char* tag,
                                                             xmlNodePtr& cur,
                                                             int l, 
                                                             info_t& info,
                                                             int normalJump = 0)
{
    // attention faire en sorte que cur ne soit JAMAIS nul

    switch (Bulletin(tag, cur, l, info, normalJump))
    {
        // on sait que cur ne sera jamais nul
        case NODE_FOUND :
            #ifndef DECIMAL_NON_EN
               if (info.decimal != '.')
            #endif
               substituer_separateur_decimal(info.Table[info.NCumAgentXml][l],
                                             info.decimal);
             return true;

        case NODE_NOT_FOUND :
            cerr << ERROR_HTML_TAG "Impossible d'atteindre " << tag 
                 << " à partir de " << cur->name << ENDL;
        
             ZERO_ASSIGN(l);
             break;

        case LINE_MEMORY_EXCEPTION :
             if (verbeux) cerr << ERROR_HTML_TAG "Allocation mémoire impossible pour la ligne "
                               << l << ENDL;
             ZERO_ASSIGN(l);
             break;

        case NO_NEXT_ITEM :
             cerr << ERROR_HTML_TAG "Pas d'item successeur pour le noeud " << tag <<  ENDL;
             #ifndef DECIMAL_NON_EN
              if (info.decimal != '.')
             #endif
                substituer_separateur_decimal(info.Table[info.NCumAgentXml][l], 
                                              info.decimal);
             break;
    }

//    Ne pas mettre de lock ici, il y en a un dans warning_msg 

    warning_msg(tag, info, cur);

        #if  STRICT
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




/// \page page1 Documentation de l'algorithme d'analyse des noeuds Remuneration
/// \tableofcontents
///
/// \section  sec1 Spécifications de la convention cadre au 1er Oct. 2016
///
/// \subsection subsec1  Noeuds XML
/// \par
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
///
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
/// Une boucle parcours #type_remuneration jusqu'à trouver la chaîne qui correspond à la
/// valeur du nom du noeud courant. l'indice de la chaîne dans #type_remuneration est
/// celui du drapeau du tableau #drapeau copié dans la table d'informations.
/// \subsubsection rebouclage Rebouclage des noms de catégorie
/// Rebouclage du parcours en cas de non-conformité de l'ordre des noeuds de
/// catégorie de ligne de paye.\n
/// En principe les éléments constitutifs des enregistrements
/// <Remuneration>....</Remuneration> sont enregistrés dans l'ordre du tableau 
/// #type_remuneration.\n
/// Toutefois quelques cas de désordre sont observés. Dans ces cas là on doit
/// réinitialiser le parcours du tableau.\n
/// La constante \b #TYPE_LOOP_LIMIT est définie à la compilation et si ce n'est pas le cas
/// prend la valeur par défaut encodée dans le fichier validator.hpp \n
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
/// fils, le noeud courant est assigné au noeud <Remuneration> suivant.


/// Analyse les noeuds fils du noeud <PayeIndivMensuel>
/// \param cur  Noeud courant
/// \param info Structure contenant l'information analysée de type #info_t
/// \details Les détails de l'algorithme sont décrits dans \ref page1
/// \return  {\b nbLignePaye, 1}  de type #LineCount, \b nbLignesPaye étant le nombre de\n
/// lignes lues, sauf si #STRICT est définie (sortie de code -11) dans les
/// circonstances décrites par \ref rebouclage ou s'il y a erreur d'allocation de la copie 
/// des drapeaux.

static inline LineCount lignePaye(xmlNodePtr cur, info_t& info)
{
    int l = BESOIN_MEMOIRE_ENTETE;

    int nbLignePaye = 0;

    unsigned int t = 0;

//  +1 pour éviter la confusion avec \0 des chaines vides
    info.Table[info.NCumAgentXml][l] = (xmlChar*) xmlStrdup(drapeau[t]);  
    ++l;

//  Besoins en mémoire :
//    BESOIN_MEMOIRE_ENTETE [champs hors ligne] + nombre de lignes 
//        + flags (maximum nbType * nb de rembobinages) 
    
    int type_loop_counter = 0;

    while (cur != nullptr)
    {
        bool new_type = false;

        while (xmlStrcmp(cur->name, (const xmlChar *) type_remuneration[t]))
        {
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

                cerr << ERROR_HTML_TAG 
                        "En excès du nombre de types de lignes de paye autorisé (" 
                     << nbType << ")." ENDL;
                if (cur)
                    cerr << ERROR_HTML_TAG "Type litigieux " << cur->name 
                         << " aux alentours du matricule " 
                         << info.Table[info.NCumAgentXml][Matricule] << ENDL;
                else
                    cerr << ERROR_HTML_TAG "Pointeur noeud courant nul" << ENDL;

              #if  STRICT
                 exit(-11);
              #else
                 cerr << ERROR_HTML_TAG "Arrêt du décodage de la ligne de paye." << ENDL;
                 cur = cur->next;
                 return {nbLignePaye, l};
              #endif
            }

            new_type = true;
        }

        if (new_type && t < nbType)
        {
//          +1 pour éviter la confusion avec \0 des chaines vides
            if ((info.Table[info.NCumAgentXml][l] 
                 = (xmlChar*) xmlStrdup(drapeau[t])) == nullptr)
            {
                if (verbeux) cerr << ERROR_HTML_TAG
                                     "Erreur dans l'allocation des drapeaux de catégories."
                                  << ENDL;
                #if  STRICT
                   exit(-12);
                #else
                   if (verbeux) cerr << ERROR_HTML_TAG 
                                        "Arrêt du décodage de la ligne de paye."
                                     << ENDL;
                #endif
                return {nbLignePaye, l};
            }

            ++l;
        }

//         ici on pourrait in fine se passer de ce test par compilation séparée

        if (! info.reduire_consommation_memoire)
        {
            verifier_taille(nbLignePaye, info);
        }

//         Si on arrive à un noeud de type Commentaire, on le saute et on réinitialise
//          "gratuitement" le parcours des drapeaux.

        if (xmlStrcmp(cur->name, (const xmlChar*) "Commentaire") == 0)
        {
            cur = cur->next;
            t=0;
            --type_loop_counter;
            continue; // garantit incidemment que cur != nullptr dans la boucle
        }

//      cur n'est pas nul à ce point

        cur = cur->xmlChildrenNode;

        if (cur == nullptr)
        {
            for (short c = 0; c < 6; ++c) NA_ASSIGN(l++);
            ++nbLignePaye;

            if (verbeux) cerr << WARNING_HTML_TAG
                                 "Anomalie : la ligne de paye est vide."
                              << ENDL;
            break;
        }

//      cur n'est pas nul à ce point et ne devient jamais nul ci-après

//      Libellé, obligatoire 

        bulletin_obligatoire("Libelle", cur, l, info);

        ++l;
        
//      Code, obligatoire 

        bulletin_obligatoire("Code", cur, l, info);

        ++l;
        
//      Base, si elle existe 

        bulletin_optionnel_numerique("Base", cur,  l, info);

        ++l;
        
//     Taux, s'il existe 

        bulletin_optionnel_numerique("Taux", cur, l, info);

        ++l;
        
//      Nombre d'unités, s'il existe 

        bulletin_optionnel_numerique("NbUnite", cur, l, info);

        ++l;
        
//      Montant, obligatoire 

        bulletin_obligatoire_numerique("Mt", cur, l, info);

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
/// \param normalJump nombre de noeuds sautés (défaut 0)

#define BULLETIN_OBLIGATOIRE_(X, normalJump) bulletin_obligatoire(#X, cur, X, info, normalJump)

/// Macro permettant de simplifier l'appel de #bulletin_obligatoire lorsque l'indice \n
/// de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info
/// \param normalJump nombre de noeuds sautés (défaut 0)

#define BULLETIN_OBLIGATOIRE(X) BULLETIN_OBLIGATOIRE_(X, 0)

/// Macro permettant de simplifier l'appel de #bulletin_obligatoire_numerique lorsque \n
/// l'indice de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info
/// \param normalJump nombre de noeuds sautés (défaut 0)

#define BULLETIN_OBLIGATOIRE_NUMERIQUE_(X, normalJump)  bulletin_obligatoire_numerique(#X, cur, X, info, normalJump)

/// Macro permettant de simplifier l'appel de #bulletin_obligatoire_numerique lorsque \n
/// l'indice de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info

#define BULLETIN_OBLIGATOIRE_NUMERIQUE(X) BULLETIN_OBLIGATOIRE_NUMERIQUE_(X, 0)

/// Macro permettant de simplifier l'appel de #bulletin_optionnel_numerique lorsque l'indice \n
/// de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info
/// \param normalJump nombre de noeuds sautés (défaut 0)

#define BULLETIN_OPTIONNEL_NUMERIQUE_(X, normalJump)  bulletin_optionnel_numerique(#X, cur, X, info, normalJump)

/// Macro permettant de simplifier l'appel de #bulletin_optionnel_numerique lorsque l'indice \n
/// de la table d'informations est donné (soit X = #Grade, #Echelon, etc.)
/// \param X  indice de la table d'informations info

#define BULLETIN_OPTIONNEL_NUMERIQUE(X)  BULLETIN_OPTIONNEL_NUMERIQUE_(X, 0)

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



/// \page page2 Documentation de l'algorithme d'analyse des noeuds PayeIndivMensuel
/// \tableofcontents
///
/// \section  pisec1 Noeud Agent
///
/// \subsection pisec1
/// \par
/// Le pointeur courant est mis sur le prochain noeud \b Agent.\n
/// Si ce noeud n'est pas identifié, ce qui est extremement rare, un log d'erreur est \n
/// généré adjacent à l'exécutable \e lhx, avec le nom \b erreurs.log
/// \par
/// Ce journal d'erreurs produit les informations suivantes: \n
/// <pre>
/// "\n\nErreur : L'agent est non identifié pour le fichier : "
///         [nom du fichier]
///         Année  [année]
///         Mois   [mois]
/// si possible:
///         Matricule précédent : [Matricule]
///         </pre>
/// \par
/// Si la compilation est réalisée avec le symbole STRCT défini, sortie du programme
/// avec la valeur -520.
/// \par
/// Sinon les variables filles du noeud Agent \n (soit : Nom, Prenom,
///  Matricule, NIR, EmploiMetier, Statut, NbEnfants, Grade, Echelon, Indic) \n reçoivent
/// la valeur NA.
/// Si le noeud Agent est identifié, saut sur le noeud Service et examen de ses fils.
///
/// \subsection pisec2
///
///
///
///


/// Fonction principale réalisant l'analyse des lignes de paye
/// \param cur   noeud courant
/// \param info  table d'informations
/// \param log   journal d'exécution
/// \details Les détails de l'algorithme sont décrits dans \ref page2

uint64_t  parseLignesPaye(xmlNodePtr cur, info_t& info, ofstream& log)
{
    bool result = true;
    int na_assign_level = 0;

    xmlNodePtr cur_parent = cur;

    cur = atteindreNoeud("Agent", cur);

    if (cur == nullptr)
    {

        string temp_logpath =getexecpath();

        cerr << ERROR_HTML_TAG "Agent non identifié. Consulter le fichier erreur.log sous "
             << temp_logpath  << " pour avoir les détails de l'incident." ENDL;

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

            log <<

            log.flush();
            log.seekp(ios_base::end);

            if (log_to_be_closed) log.close();
        }
        else
            cerr << ERROR_HTML_TAG " Impossible d'écrire le log des erreurs." ENDL;

#if  STRICT
        if (log.is_open()) log.close();
        exit(-520);
#endif

        for (int l : {Nom, Prenom, Matricule, NIR, EmploiMetier, Statut, NbEnfants, Grade,
                      Echelon, Indice})
        {
           NA_ASSIGN(l);
        }

        cur = cur_parent;
        cur = atteindreNoeud("Service", cur);
        goto level0;
    }

//  cur n'est pas nul à ce point

    cur_parent = cur;
    cur = cur->xmlChildrenNode;

    if (cur == nullptr ||  xmlIsBlankNode(cur)) return 0;

#ifdef TOLERANT_TAG_HIERARCHY
    xmlNodePtr cur_save = cur;
#endif

//  dans certains schémas on peut ne pas avoir la civilité 
//  passer à la balise adjacente après lecture 

    info.drapeau_cont = true;

//     if (result) va garantir notamment que le pointeur cur filé implicitement est
//     non nul 



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
// on refait le parcours depuis le haut en cas d'ordre inexact des balises
#ifdef TOLERANT_TAG_HIERARCHY   
                cur = cur_save;
#endif
                if (result)
                {
                    result &= BULLETIN_OBLIGATOIRE(NbEnfants);
                    if (result)
                    {
                        result &= BULLETIN_OBLIGATOIRE(Statut);
                        
//                      NOTA : on ne contrôle pas le respect du champ Adresse, normalement
//                      obligatoire et situé entre NIR et NbEnfants, ce champ devant
//                      être regardé comme trop volatile pour que le contrôle s'y attarde. 
                        
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
//                                     ne pas lire la balise adjacente : fin du niveau 
//                                     subordonné Agent

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

//  pas de break 

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
        #if  STRICT
           exit(-512);
        #endif
    }

//     on remonte d'un niveau 

    cur = cur_parent;
    if (!result && verbeux) cerr << ERROR_HTML_TAG "Remontée d'un niveau" ENDL;

    #ifdef TOLERANT_TAG_HIERARCHY
      cur_save = cur;
    #endif

//   Long saut vers cette étiquette dans le cas Agent non reconnu 

  level0:

//   Référence
//  
//      <Evenement>
//        <Code V="">{1,1}</Code>
//        <Description V="">{0,1}</Description>
//      </Evenement>
//  

    info.drapeau_cont = true;

//     C'est extrêmement rare mais idéalement il faudrait pouvoir être à même de récupérer
//     plus de 2 événements : A FAIRE 

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

//  Vu la rareté du 2e évenement, il est rationnel de ne pas réserver systématiquement
//  de place en mémoire de type Description2.
//  Mieux vaut concaténer, même si le code est plus lourd et l'allocation de mémoire 
//  ponctuellement plus lente : on gagne
//  sur l'allocation-déallocation d'un très grand nombre de champs Description2 non 
//  remplis. 

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
#if  STRICT
        exit(-5);
#endif
    }

#ifdef TOLERANT_TAG_HIERARCHY
    cur = cur_save;
    cur = atteindreNoeud("NBI", cur);
#endif

    BULLETIN_OBLIGATOIRE_NUMERIQUE(NBI);

//     Problème : unbounded NBI ... 

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


//  obligatoire, substitution du séparateur décimal 
    
    BULLETIN_OBLIGATOIRE_NUMERIQUE(QuotiteTrav);

#ifdef TOLERANT_TAG_HIERARCHY
    cur = cur_save;
    cur = atteindreNoeud("Remuneration", cur);
#else
    cur = cur->next;
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

//        si la balise <Remuneration/> est fermante 
//        ou si <Remuneration>....</Remuneration>
//        ne contient pas de ligne de paye codée
//        alors on attribue quand même une ligne, codée NA sur tous les champs 

        if (ligne == 0)
        {
            for (int k = 0; k <= INDEX_MAX_COLONNNES; ++k)
              {
                info.Table[info.NCumAgentXml][BESOIN_MEMOIRE_ENTETE + k] 
                        = (xmlChar*) xmlStrdup(NA_STRING);
              }
                info.Memoire_p_ligne[info.NCumAgentXml] = BESOIN_MEMOIRE_ENTETE
                                                          + INDEX_MAX_COLONNNES + 1;
        }
        else
            info.Memoire_p_ligne[info.NCumAgentXml] = memoire_p_ligne_allouee;

        cur = cur_save->next;
    }
    else
    {
        perror(ERROR_HTML_TAG "Rémunération introuvable.");
#if  STRICT
        exit(-4);
#endif
    }

//  obligatoire , substitution du sparateur décimal 


    result = BULLETIN_OPTIONNEL_NUMERIQUE(NbHeureTotal);

    cur = atteindreNoeud("NbHeureSup", cur);

//  obligatoire, substitution du sparateur décimal. Attention : utiliser des bitwise, 
//  pas des logical && sinon le deuxième opérande peut ne pas être évalué */
    result = result & BULLETIN_OPTIONNEL_NUMERIQUE(NbHeureSup);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(MtBrut);
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNet);

    info.drapeau_cont=false; // fin du niveau PayeIndivMensuel
    result = result & BULLETIN_OBLIGATOIRE_NUMERIQUE(MtNetAPayer);

    if (!result)
    {
        cerr << ERROR_HTML_TAG "Problème de conformité des données sur les champs des "
                               "bulletins de paye." ENDL;
#if  STRICT
        exit(-513);
#endif
    }

// Rémuneration tag vide
    if (ligne == 0) ligne = 1 ;

    return ligne;
}


