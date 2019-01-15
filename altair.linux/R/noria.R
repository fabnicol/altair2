# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'année 2008.
# 
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
# 
# A cet égard l'attention de l'utilisateur est attirée sur les risques
# associés au chargement, à l'utilisation, à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invités à charger et tester l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# 
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.
# 
# 

# Modélisation : en première année sous revue, est réputé absent le Matricule ayant une quotite non uniformément nulle sur l'année, qui n'a pas de quotite positive dans les trois premiers mois
# Modélisation : en dernière année sous revue, est réputé absent le Matricule ayant une quotite non uniformément nulle sur l'année, qui n'a pas de quotite positive dans les trois derniers mois
# Cette modélisation est adoptée par défaut si un fichier des entrées sorties n'est pas passé en paramètre fichier.
# Sont exclus les vacataires et assistantes maternelles détectées.

#' Décomposition de l'évolution des salaires, RMPP, SMPT et GVT
#'
#' Elabore des tableaux permettant de relier l'évolution de la RMPP, du salaire moyen et des effets d'entrées-sorties.
#' 
#' La RMPP ici utilisée se distingue de la définition officielle pour la première année.
#' On considère en effet en début de période que les salaires considérés sont ceux des agents
#' présents toute la première année et toute la seconde avec la même quotite.
#' Ce devrait être en principe "et toute l'année précédente", mais celle-ci n'est pas documentée
#' et l'approximation reste valable pour l'évaluation du GVT.
#'
#' @param Bulletins Base mensuelle des bulletins de paie, comportant pour l'ensemble de la période
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}                 
#'                 \item \code{Categorie}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1, quotite mensuelle}}}}
#' @param Base Base de paye, ou extraction de cette base, comportant pour l'ensemble de la période
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Categorie}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite.moyenne}}{réel entre 0 et 1, quotite annuelle moyenne (somme des quotites divisée par 12).}
#'                 \item{\code{Montant.net.eqtp}}{salaire net moyen annuel EQTP}
#'                 \item{\code{Montant.brut.eqtp}}{salaire brut moyen annuel en EQTP}}}}.
#' @param classe Valeur caractère définissant une sous-population.                  
#' @param champ  "brut" ou "net" selon que le tableau est calculé sur rémunérations brutes ou nettes.
#' @param filtre "A", "B", "C" pour les catégories statutaires correspondantes, ou bien un vecteur de libellés de statuts possibles (exemple c("TITULAIRE", "STAGIAIRE"). Vecteur de chaînes de caractères exactes. 
#' @param fichier  Si absent, seules les bases de paye seront utilisées. 
#' En première année sous revue, est réputé absent le Matricule ayant une quotite non uniformément nulle sur l'année, qui n'a pas de quotite positive dans les trois premiers mois
#' En dernière année sous revue, est réputé absent le Matricule ayant une quotite non uniformément nulle sur l'année, qui n'a pas de quotite positive dans les trois derniers mois
#' Cette modélisation est adoptée par défaut si un fichier des entrées sorties n'est pas passé en paramètre fichier.
#' Sont exclus les vacataires et assistantes maternelles détectées.
#' Si présent, fichier indique le chemin complet du fichier des entrées-sorties, comportant pour l'ensemble de la période :  
#'        \enumerate{
#'          \item{ à titre obligatoire :
#'             \describe{
#'                 \item{\code{Matricule}}{Matricule de l'agent, au même format que dans les autres bases}
#'                 \item{\code{Entrée}}{Date d'entrée de l'agent en service, de la forme numérique XX/XX/20XX}                 
#'                 \item{\code{Sortie}}{Date de sortie de l'agent du service, de la forme numérique XX/XX/20XX}}}                 
#'           \item{ à titre optionnel :
#'               \describe{
#'                 \item{\code{Classe}}{une variable qualitative de type caractère définissant une sous-population.}}}}
#' @param dec  Paramètre \code{dec} de la fonction \code{data.table::fread}.                 
#' @param sep  Paramètre \code{sep} de la fonction \code{data.table::fread}. 
#' @param encoding  Paramètre \code{encoding} de la fonction \code{data.table::fread}.
#' @param afficher.tableau Si TRUE, affiche quatre tableaux correspondant à la valeur de retour sur la sortie standard.
#' @param controle.quotite Si TRUE, calcule la RMPP comme dans la définition (quotites identiques sur deux exercices, à 0,1 point près). Si FALSE, relâche cette hypothèse.  
#' @return Si afficher.tableau = TRUE : 
#'  \enumerate{
#'      \item{
#'        Affiche un tableau des entrées-sorties, comportant les variables numériques suivantes :
#'               \describe{
#'                 \item{\code{Annee}}{Annees de la période sous revue, puis le libellé "Total".}
#'                 \item{\code{Effectifs}}{Effectifs en janvier, hors quotites nulles, vacataires et assistantes maternelles identifiés.}
#'                 \item{\code{ETPT}}{ETPT calculés sur base de paye, postes actifs non annexes (voir fonction \link{effectifs}).}
#'                 \item{\code{ETPT entrants}}{ETPT des entrants de l'année, par année.}
#'                 \item{\code{ETPT sortants}}{ETPT des sortants de l'année, par année.}
#'                 \item{\code{Entrants}}{Entrants (effectifs physiques) de l'année, par année, puis Total.}
#'                 \item{\code{Sortants}}{Sortants (effectifs physiques) de l'année, par année, puis Total.}                 
#'                 \item{\code{Var. effectifs}}{différence du nombre d'entrants et de sortants, par année, puis Total.}
#'                 \item{\code{Taux de rotation}}{ratio égal à la moyenne du nombre d'entrants et de sortants rapportée à l'effectif en janvier}}}
#'      \item{           
#' Un deuxième tableau détaille l'effet des entrées sorties sur la masse des rémunérations brutes ou nettes. \cr
#' Des colonnes indiquent la valeur de l'effet en point de pourcentage du salaire moyen (\% SMPT) : \cr
#'               \describe{
#'                 \item{\code{Annee}}{Annees de la période sous revue, puis le libellé "Total".}
#'                 \item{\code{Effet noria}}{Cumul des économies (si négatif) ou des coûts (si positif) sur salaires (nets ou bruts) dus au remplacement des sortants par les entrants, par année, puis Total.}
#'                 \item{\code{Effet var. effectifs}}{Cumul des économies (si négatif) ou des coûts (si positif) sur salaires (nets ou bruts) dus aux variations d'effectifs. \cr
#'                 Est positif si le nombre d'entrants excède le nombre de sortants, par année, puis Total.}
#'                 \item{\code{Effet vacances}}{Cumul des économies (si négatif) ou des coûts (si positif) sur salaires (nets ou bruts) dus aux vacances d'emplois. \cr
#'                 Est positif si les entrants intègrent l'organisme en moyenne plus tôt dans l'année que les sortants ne le quittent, par année, puis Total.}                                  
#'                 \item{\code{Total}}{Total des trois effets liés aux entrées-sorties (GVT négatif et variation d'effectifs vacances comprises)}}}
#'    \item{             
#' Un troisième tableau détaille la décomposition du salaire moyen par tête (SMPT) en ses composantes stables (RMPP) et variables (flux d'entrées-sorties)
#' Les coefficients des colonnes comprises entre la colonne "RMPP" et la colonne "SMTP" sont des variations relatives exprimées en pourcentage.
#' Pour passer de la colonne "RMPP" à la colonne "SMPT", il suffit de multiplier successivement par chaque coefficient sur la même ligne, augmenté de 1. 
#' 
#'               \describe{
#'                 \item{\code{Annee}}{Annees de la période sous revue}
#'                 \item{\code{RMPP}}{Rémunération moyenne des personnes en place, présentes deux années complètes consécutives.\cr
#'                  La vérification de la permanence de la quotite sur les deux années n'est pas implémentée.}
#'                 \item{\code{Entrée n - 1}}{Effet relatif en \% des entrées de personnels en n - 1 présents en n et des variations de quotite entre n - 1 et n. Pour la première année, n - 1 est remplacé par n + 1 et "entrées" par "sorties".}
#'                 \item{\code{Noria}}{Effet de noria. Effet relatif en \% du remplacement des sortants par les entrants en faisant l'hypothèse que les entrants sont aussi nombreux que les sortants.}
#'                 \item{\code{Var. effectifs}}{Effet relatif en \% sur le salaire moyen distribué des variations d'effectifs.}
#'                 \item{\code{Effet vacances}}{Effet relatif en \%  sur le salaire moyen distribué de la différence entre le nombre d'entrants et le nombre de sortants.}                                  
#'                 \item{\code{Total E/S}}{Effet relatif en \%  sur le salaire moyen distribué des quatre effets précédents liés aux entrées-sorties.}
#'                 \item{\code{Ajustement}}{Correction à ajouter en point de pourcentage au coefficient Total E/S, dues aux approximations utilisées pour le calcul des effets d'entrées-sorties.}
#'                 \item{\code{SMPT}}{Salaire moyen par tête, résultant de l'application multiplicative des corefficients de variation précédents.}}}
#'  \item{
#'  Un quatrième tableau est déduit des précédents et indique la variation de la RMPP, du SMPT, et les effets des entrées-sorties en points de pourcentage :\cr
#'  
#'  \describe{
#'  \item{\code{Var. RMPP}}{Variation relative de la RMPP.}
#'  \item{\code{Var. effets E/S}}{Effet des entrées-sorties : s'obtient par la différence entre la valeur du Total E/S du tableau précédent,\cr
#'   corrigée par le coefficient d'ajustement, pour l'année n + 1, et celle de l'année n.}
#'   \item{\code{Cumul}}{Cumul des deux effets de variation de la RMPP et des effets d'entrées-sorties. Si ce cumul est égal à celui de la variation du SMPT, alors l'ensemble des calculs est correct.}
#'   \item{\code{Var. SMPT}}{Variation du SMPT.}
#'  }}               
#'                 
#'  \item{               
#' Le retour de la fonction est un tableau du GVT négatif au format \code{data.table}, comportant les variables numériques suivantes (les totaux de colonnes figurent seulement dans l'affichage) : 
#'               \describe{
#'                 \item{\code{période}}{Annees de la période sous revue, puis le libellé "Total".}
#'                 \item{\code{effectifs.janvier}}{Effectifs physiques en janvier.}
#'                 \item{\code{etpt.ent}}{ETPT entrants dans l'année, correspondant à des agents présents en décembre et pas en janvier.}
#'                 \item{\code{etpt.sort}}{ETPT sortants dans l'année, correspondant à des agents présents en janvier et pas en décembre.}
#'                 \item{\code{nent}}{Nombre d'entrants, par année, puis Total.}
#'                 \item{\code{nsort}}{Nombre de sortants, par année, puis Total.}
#'                 \item{\code{variation.effectifs}}{Différence du nombre d'entrants et de sortants, par année, puis Total.}
#'                 \item{\code{taux.rotation}}{Ratio égal à la moyenne du nombre d'entrants et de sortants rapportée à l'effectifs de l'année précédente.}                 
#'                 \item{\code{effet.noria}}{Cumul des économies (si négatif) ou des coûts (si positif) sur salaires (nets ou bruts) dus au remplacement des sortants par les entrants, par année, puis Total.}
#'                 \item{\code{effet.variation.effectifs}}{Cumul des économies (si négatif) ou des coûts (si positif) sur salaires (nets ou bruts) dus aux variations d'effectifs.\cr
#'                  Est positif si le nombre d'entrants excède le nombre de sortants, par année, puis Total.}
#'                 \item{\code{effet.vacances}}{Cumul des économies (si négatif) ou des coûts (si positif) sur salaires (nets ou bruts) dus aux vacances d'emplois.\cr
#'                  Est positif si les entrants intègrent l'organisme en moyenne plus tôt dans l'année que les sortants ne le quittent, par année, puis Total.}                                                   
#'                 \item{\code{effet.total.entrées.sorties}}{Cumul des effets précédents, par année, puis Total.}
#'                 \item{\code{rmpp.salaire}}{Rémunération moyenne des personnes en place}
#'                 \item{\code{smpt.salaire}}{Salaire moyen par tête}
#'                 \item{\code{rmpp.etpt}}{ETPT correpondant au calcul de la RMPP}
#'                 \item{\code{smpt.etpt}}{ETPT correpondant au calcul du SMPT}
#'                 \item{\code{coef.entrées.ant}}{Effet relatif en \% des entrées de personnels au cours de l'année précédente.}
#'                 \item{\code{coef.noria}}{Effet de noria relatif}
#'                 \item{\code{coef.var.effectifs}}{Effet relatif en \% sur le salaire moyen distribué des variations d'effectifs}
#'                 \item{\code{coef.vacances}}{Effet relatif en \%  sur le salaire moyen distribué de la différence entre le nombre d'entrants et le nombre de sortants.}
#'                 \item{\code{coef.smpt}}{Produit des variations relatives correspondant aux coefficients précédents.}
#'                 \item{\code{var.rmpp}}{Variation de la RMPP.}
#'                 \item{\code{effet.es}}{Effet des entrées-sorties sur la variation du SMPT.}
#'                 \item{\code{var.smpt}}{Variations du SMPT.}
#'                 \item{\code{qualité}}{Indice de qualité des calculs, égal à l'erreur relative résultant de la modélisation, \cr
#'                 entre le cumul des effets des variables coef.x.y exprimés en masse de salaires, partant d'un salaire moyen fictif égal à la RMPP, et la somme des salaires effectivement versés au même nombre d'agents. }
#'                 \item{\code{delta.coef}}{Correction à ajouter en point de pourcentage au coefficient Total E/S, dues aux approximations utilisées pour le calcul des effets d'entrées-sorties}}}}                   
#'                 
#' @references  \href{Rapport annuel sur l'état de la fonction publique 2016}{https://www.fonction-publique.gouv.fr/files/files/statistiques/rapports_annuels/2016/DGAFP_RA2016_web_signet.pdf}   
#'  \href{https://www.fonction-publique.gouv.fr/files/files/statistiques/rapports_annuels/2017/Rapport_annuel-FP_2017.pdf}{Rapport annuel sur l'état de la fonction publique 2017}   
#'  \href{https://www.fonction-publique.gouv.fr/files/files/statistiques/rapports_annuels/2018/Rapport_annuel_FP-2018.PDF}{Rapport annuel sur l'état de la fonction publique 2018}   
#'    \tabular{cccc}{
#'               \tab   RMPP  \tab Effet E/S  \tab  SMTP  \cr
#'          brut  \tab   +3  \tab        -1,3 \tab +1,7  \cr
#'          net   \tab   +2,7 \tab        -1,3 \tab +1,3 
#'          }
#' Voir aussi : \href{http://www4.minefi.gouv.fr/budget/circpdf/2BPSS-05-5044.pdf}{outil 2BPSS, Minefi, Direction du budget}          
#'     
#' @examples
#'  noria()
#' 
#'  | Annee | Effectifs | ETPT  | ETPT entrants | ETPT sortants | Entrants | Sortants | Var. effectifs | Taux de rotation % |
#'  |-------|-----------|-------|---------------|---------------|----------|----------|----------------|--------------------|
#'  | 2011  | 803       | 789,6 |     14,8      |     10,0      |   30,0   |   23,0   |      7,0       |        3,3         |
#'  | 2012  | 992       | 941,1 |     14,1      |     13,6      |   24,0   |   25,0   |     -1,0       |        2,5         |
#'  | 2013  | 989       | 938,8 |     11,5      |     16,7      |   27,0   |   32,0   |     -5,0       |        3,0         |
#'  | 2014  | 975       | 958,1 |     17,5      |     18,1      |   38,0   |   37,0   |      1,0       |        3,8         |
#' 
#' 
#' 
#'  Lecture : 
#'  L'organisme comptait 939 ETPT en 2013, dont 11,5 ETPT entrant et 16,7 ETPT sortants
#'  correspondant respectivement à 27 entrants physiques et 32 sortants physiques. 
#'  Le taux de rotation était de 3,0 % au cours de cette année.
#'  
#'  | Annee | Effet noria | % SMPT | Effet var. effectifs | % SMPT | Effet vacances | % SMPT |   Total    | % SMPT |
#'  |-------|-------------|--------|----------------------|--------|----------------|--------|------------|--------|
#'  | 2011  |  -64 887,1  |  -0,2  |       97 655,0       |  0,4   |   -45 805,1    |  -0,2  | -13 037,1  |  -0,0  |
#'  | 2012  |  -10 714,7  |  -0,0  |      -16 499,3       |  -0,0  |    90 945,2    |   0,3  |  63 731,2  |   0,2  |
#'  | 2013  | -102 531,4  |  -0,3  |      -61 836,1       |  -0,2  |   -50 050,9    |  -0,1  | -214 418,3 |  -0,6  |
#'  | 2014  |  -88 795,8  |  -0,3  |       11 441,7       |  0,0   |   -46 453,6    |  -0,1  | -123 807,6 |  -0,4  |
#'  
#'  Lecture : 
#'  L'effet de l'effet de noria en 2013 était de -0,3 % en valeur relative en proportion de
#'  la masse des rémunérations brutes 2013 (autrement dit en points de SMPT). 
#'  Le total des effets d'entrées-sorties était de -0,6 point de SMPT, soit une économie 
#'  sur rémunérations brutes versées de 214 418,3 euros.  
#'  
#'  | Annee |   RMPP   | Entrées n - 1 | Noria | Var. effectifs | Vacances | Total E/S | Ajustement |   SMPT   |
#'  |-------|----------|---------------|-------|----------------|----------|-----------|------------|----------|
#'  | 2011  | 34 849,0 |      0,00     | -0,24 |      0,36      |  -0,17   |   -0,05   |   -0,003   | 34 717,2 |
#'  | 2012  | 35 910,5 |     -0,40     | -0,03 |     -0,05      |   0,27   |   -0,21   |   -0,007   | 35 568,7 |
#'  | 2013  | 36 493,9 |     -0,63     | -0,30 |     -0,18      |  -0,15   |   -1,25   |    0,004   | 36 173,4 |
#'  | 2014  | 36 576,0 |     -0,81     | -0,26 |      0,03      |  -0,13   |   -1,16   |   -0,005   | 35 956,1 |
#'  
#'  Lecture :
#'  Le salaire brut moyen 2013 serait égal à la RMPP (rémunération des personnes présentes tout au long de 2012 et 2013)
#'  sans les effets d'entrées et de sorties en 2012 et 2013.
#'  Ces effets interviennent en 2012 (seulement les entrées, première colonne) et en 2013 (colonnes suivantes).
#'  L'effet relatif des entrées 2012 est de -0,63 % : les entrants en 2012 encore présents en 2013 ont des salaires moins élevés 
#'  que les présents-présents (sur toute les années 2012 et 2013) en moyenne.
#'  L'effet de noria 2013 (emplacement des sortants 2013 par un même nombre d'entrants) est de -0,3 % de variation relative :
#'  les sortants avaient en moyenne des salaires plus élevés que les entrants.
#'  L'effet relatif de la variation des effectifs 2013 est de -0,18 % : il y a eu un peu plus de sortants que d'entrants.
#'  L'effet des vacances d'emploi est de -0,15 % : tous les sortants ne sont pas remplacés au moment de leur départ.
#'  Le total de ces effets d'entrées-sorties est de -1,25 %, auquel s'ajoute un redressement mathématique 
#'  lié aux hypothèses de modélisation de 0,004 points de pourcentage.
#'    
#'  |   Annee   | Var. RMPP | Var. effets E/S| Cumul | Var. SMPT | 
#'  |-----------|-----------|----------------|-------|----------| 
#'  | 2011-2012 |   3,05    |     -0,58      |  2,45 |   2,45   | 
#'  | 2012-2013 |   1,62    |      0,07      |  1,70 |   1,70   | 
#'  | 2013-2014 |   0,22    |     -0,82      | -0,60 |  -0,60   | 
#'  
#'  Lien avec les prévisions budgétaires et le GVT : 
#'  Le GVT positif se déduit de la variation de la RMPP en retranchant l'effet, estimé par d'autres moyens,
#'  des mesures catégorielles et générales. Le GVT négatif total, défini comme la somme de l'effet de noria
#'  et des différents effets d'entrées-sorties (en n et n-1) est égal à la colonne "Variation des effets d'entrées-sorties".
#'  
#'  
#'noria(champ = "net", classe = "SPP", filtre = "A")
#'
#' Lecture : Produit des tableaux analogues aux précédents pour la rémunération des agents SPP de catégorie A.
#' 
#'
#' @export


noria <- function(Bulletins = Bulletins.paie,
                  Base = Analyse.variations.par.exercice,
                  classe = "", 
                  champ = "brut",
                  filtre = "",
                  fichier = "", 
                  dec = ",", 
                  sep = ";",
                  encoding = "UTF-8",
                  afficher.tableau = TRUE,
                  controle.quotite = FALSE) { 

  
période.translatée <- 1:durée.sous.revue

if (length(période.translatée) < 2) {
  
  cat("L'effet de noria ne peut être calculé que sur au moins deux exercices consécutifs")
  return(NULL)
}

noria.sur.base.de.paie <- (fichier == ""  | ! file.exists(fichier))

if (! noria.sur.base.de.paie){
  ES <- fread(fichier, dec = dec, sep = sep, encoding = encoding)
  
  ES$Matricule <- as.character(ES$Matricule)
  
  if (classe == "") {
    
    ES <- ES[ , .(Matricule, Entrée, Sortie)]
    
  } else {
    
    ES <- ES[ , .(Matricule, Classe, Entrée, Sortie)] 
  }
  
  
  ES[ , `:=`(Entrée = as.Date(Entrée, format = "%d/%m/%Y"),
                                                    Sortie = as.Date(Sortie, format = "%d/%m/%Y"))
                                          ][ , `:=`(Annee.entrée = as.numeric(format(Entrée, "%Y")),
                                                    Mois.entrée  = as.numeric(format(Entrée, "%m")),
                                                    Jour.entrée  = as.numeric(format(Entrée, "%d")),
                                                    Annee.sortie = as.numeric(format(Sortie, "%Y")),
                                                    Mois.sortie  = as.numeric(format(Sortie, "%m")),
                                                    Jour.sortie  = as.numeric(format(Sortie, "%d")))]
}

if (filtre != "") {
  if (! filtre %chin% c("A", "B", "C")) {
    Base <- Base[Statut %chin% filtre]
  } else {
    Base <- Base[Categorie == filtre]
  } 
}

tot.etpt <- Base[ , .(etpt = sum(quotite.moyenne, na.rm = TRUE)), by = Annee]  
  
if (champ == "brut") salaire.moyen <- "Montant.brut.annuel.eqtp" else salaire.moyen <- "Montant.net.annuel.eqtp"

présents.bulletins <- list()
matricules.présents <- list()

transl <- function(année) année - début.période.sous.revue + 1

filtrage <- function(année) {
  
    if (filtre != "") {
      
      if (! filtre %chin% c("A", "B", "C")) {
        présents.bulletins[[transl(année)]] <<- Bulletins[Statut %chin% filtre
                                                          & Annee == année & quotite.moyenne != 0
                                                          & Grade != "A" 
                                                          & Grade != "V", .(Matricule, Mois, quotite)]
      } else {
        présents.bulletins[[transl(année)]] <<- Bulletins[Categorie == filtre 
                                                          & Annee == année 
                                                          & quotite.moyenne != 0 
                                                          & Grade != "A" 
                                                          & Grade != "V", .(Matricule, Mois, quotite)]
      } 
      
    } else {
      
      présents.bulletins[[transl(année)]] <<- Bulletins[Annee == année & quotite.moyenne != 0
                                                        & Grade != "A" 
                                                        & Grade != "V", .(Matricule, Mois, quotite)]
    }

    if (! noria.sur.base.de.paie) {     
      
       colonnes <- c("Matricule", "Annee.entrée", "Mois.entrée", "Jour.entrée",
                     "Annee.sortie", "Mois.sortie", "Jour.sortie") 
       
       if (classe != "") colonnes <- c(colonnes, "Classe")
       
       présents.bulletins[[transl(année)]] <<- merge(présents.bulletins[[transl(année)]], ES[ , colonnes, with = FALSE], by = "Matricule", all.x = TRUE)
       
       if (classe != "") présents.bulletins[[transl(année)]] <<- présents.bulletins[[transl(année)]][Classe == classe]  
    }
  
    matricules.présents.base <- Base[Annee == année, unique(Matricule)]
    
    matricules.présents[[transl(année)]] <<- intersect(unique(présents.bulletins[[transl(année)]]$Matricule), matricules.présents.base)
}

for (année in période) filtrage(année)

exclure.présents <- function(année, mois) {
  
    matricules.présents <- matricules.présents[[transl(année)]]
 
    matricules.mois <- présents.bulletins[[transl(année)]][Mois == mois & quotite != 0, unique(Matricule)]
    
    matricules.mois <- intersect(matricules.mois, matricules.présents)
    
if (noria.sur.base.de.paie) {
 
     matricules.sélectionnés <- setdiff(matricules.présents, matricules.mois)
     
     date <- -1 
  
} else {    
  
    if (mois == 12) {
      
      matricules.sélectionnés <- présents.bulletins[[transl(année)]][Annee.sortie == année, unique(Matricule)]
      
    } else {
      
      matricules.sélectionnés <- présents.bulletins[[transl(année)]][Annee.entrée == année, unique(Matricule)]
      
    }
    
    matricules.sélectionnés   <- intersect(matricules.sélectionnés, matricules.présents)
    
    if (mois == 12) {
      
      date <- présents.bulletins[[transl(année)]][Matricule %chin% matricules.sélectionnés, 
                                                    mean(Mois.sortie - 1 + Jour.sortie / 30, na.rm = TRUE)] / 12
      
    } else {
      
      date <- présents.bulletins[[transl(année)]][Matricule %chin% matricules.sélectionnés, 
                                                    mean(Mois.entrée - 1 + Jour.entrée / 30, na.rm = TRUE)] / 12
    }
}
    
    etpt.sélectionnés <- unique(Base[Annee == année 
                                 & quotite.moyenne != 0 
                                 & Grade != "A" 
                                 & Grade != "V"
                                 & Matricule %chin% matricules.sélectionnés, 
                                   .(Annee, quotite.moyenne, Matricule)],
                                      by = NULL)[ , sum(quotite.moyenne, na.rm=TRUE)]
    
    list(matricules = matricules.sélectionnés, effectifs = length(matricules.mois), etpt = etpt.sélectionnés,  n = length(matricules.sélectionnés), date = date)
}

entrants <- function(année)   exclure.présents(année, mois = 1)
sortants <- function(année)   exclure.présents(année, mois = 12)



ent   <- lapply(période, entrants)
sort  <- lapply(période, sortants)

effectifs.janvier <- sapply(ent, function(x) x$effectifs)

nsort <- sapply(période.translatée, function(année) sort[[année]][["n"]])
nent  <- sapply(période.translatée, function(année) ent[[année]][["n"]])

etpt.sort <- sapply(période.translatée, function(année) sort[[année]][["etpt"]])
etpt.ent  <- sapply(période.translatée, function(année) ent[[année]][["etpt"]])


if (noria.sur.base.de.paie) {
  qsortants <- sapply(période.translatée, function(année) 1 - sort[[année]][["etpt"]] / nsort[année])
  qentrants <- sapply(période.translatée,  function(année) ent[[année]][["etpt"]]) / nent
} else {
  qsortants <- sapply(période.translatée, function(année)  1 - sort[[année]][["date"]]) 
  qentrants <- sapply(période.translatée,  function(année) ent[[année]][["date"]]) 
}

salaires <- function(X)
                sapply(période, function(année) {
                  B <- Base[Annee == année
                            & Matricule %chin% as.character(X[[transl(année)]][["matricules"]]), 
                            c(salaire.moyen, "quotite.moyenne"),
                            with = FALSE]
                  
                  weighted.mean(B[[1]], B[[2]], na.rm = TRUE)
                })

salaire.entrants <- salaires(ent)
salaire.sortants <- salaires(sort)

#non.entrants_sortants.non.présents_présents 

construire.liste <- function(B) {
  
                        B <- B[ , c(salaire.moyen, "quotite.moyenne"), with = FALSE]
                        
                        c(weighted.mean(B[[1]], B[[2]], na.rm = TRUE), sum(B[[2]], na.rm = TRUE))
}

# La RMPP ici utilisée se distingue de la définition officielle pour la première année.
# On considère en effet en début de période que les salaires considérés sont ceux des agents
# présents toute la première année et toute la seconde. Ce devrait être en principe "et toute l'année
# précédente, mais celle-ci n'est pas documentée et l'approximation reste valable pour l'évaluation
# du GVT.

rmpp <- data.table(t(sapply(période, function(année) {
  
  if (durée.sous.revue < 2) {
    message("La RMPP ne peut être calculée que sur deux années au moins.")
    return(0)
  }
  
  B <- Base[Annee == année
            & ! Matricule %chin% as.character(ent[[transl(année)]][["matricules"]]) 
            & ! Matricule %chin% as.character(sort[[transl(année)]][["matricules"]])]
  
  if (année > début.période.sous.revue) {
    
    Matricules.cond <- unique(Base[Annee == année -1, .(Matricule, quotite.moyenne)], by = NULL)
    
    C <- Matricules.cond[! Matricule %chin% as.character(ent[[transl(année - 1)]][["matricules"]])
                         & ! Matricule %chin% as.character(sort[[transl(année - 1)]][["matricules"]])]  # en principe inutile mais sait-on jamais

  } else {
  
    Matricules.cond <- unique(Base[Annee == année + 1, .(Matricule, quotite.moyenne)], by = NULL)
    
    C <- Matricules.cond[! Matricule %chin% as.character(ent[[transl(année + 1)]][["matricules"]])
                        & ! Matricule %chin% as.character(sort[[transl(année + 1)]][["matricules"]])]  # en principe inutile mais sait-on jamais
  }
  
  if (controle.quotite) {
    
    D <- merge(B, C, by = "Matricule")  # all = FALSE impératif
    D <- D[abs(quotite.moyenne.x - quotite.moyenne.y) < 0.1][ , quotite.moyenne.x := NULL] # présent toute l'année avec la même quotite.
    setnames(D, "quotite.moyenne.y", "quotite.moyenne")
    D <- unique(D[Annee == année , c("Matricule", salaire.moyen, "quotite.moyenne"), with = FALSE])
    
  } else {
  
    D <- B[Matricule %chin% unique(C$Matricule)]  
  }
   
  construire.liste(D)
})))

rmpp_mod <- data.table(t(sapply(période, function(année) {
  
            B <- Base[Annee == année
                                & ! Matricule %chin% as.character(ent[[transl(année)]][["matricules"]]) 
                                & ! Matricule %chin% as.character(sort[[transl(année)]][["matricules"]])]
          
            construire.liste(B)
})))

smpt <- data.table(t(sapply(période, function(année) {
  
  B <- Base[Annee == année]
  
           construire.liste(B)  
})))

setnames(rmpp, c("salaire", "etpt"))
setnames(rmpp_mod, c("salaire", "etpt"))
setnames(smpt, c("salaire", "etpt"))

# GVT négatif = (masse salariale moyenne unitaire des entrants - masse salariale moyenne unitaire des sortants) x nombre sortants
# ici exprimée en rémunérations

form <- function(V, total = FALSE, digits = 1) {
  
  v0 <- formatC(as.numeric(V),
              big.mark = " ",
              digits = digits,
              format = "f")
  
  v0[v0 == "NaN"] <- ""
  
  if (total) c(v0, formatC(sum(as.numeric(V), na.rm = TRUE),
                           big.mark = " ",
                           digits = 1,
                           format = "f")) else c(v0, "")
}

# L'effet de la variation d'effectifs (schéma d'emploi, vacances infra-annuelles incluses) est : 
#  se = (nombre entrants - nombre sortants) x masse salariale moyenne unitaire des entrants


effet.entrées.ant                 <- (rmpp_mod$salaire - rmpp$salaire) * smpt$etpt
effet.noria                       <- (salaire.entrants - salaire.sortants) * nsort * qsortants
effet.noria.relatif               <- effet.noria / (smpt$salaire * smpt$etpt)
variation.effectifs               <- nent - nsort
effet.variation.effectifs         <- salaire.entrants * variation.effectifs * qentrants
effet.variation.effectifs.relatif <- effet.variation.effectifs / (smpt$salaire * smpt$etpt)
effet.vacances                    <- nsort * (qentrants - qsortants) * salaire.entrants
effet.vacances.relatif            <- effet.vacances / (smpt$salaire * smpt$etpt)
effet.total.entrées.sorties       <- salaire.entrants * nent * qentrants - salaire.sortants * nsort * qsortants 
effet.total.entrées.sorties.relatif <- effet.total.entrées.sorties / (smpt$salaire * smpt$etpt)

taux.rotation                     <- 1/2 *(nent + nsort) * 100 / effectifs.janvier
coef.entrées.ant                  <- rmpp_mod$salaire / rmpp$salaire - 1
coef.noria                        <- (rmpp_mod$salaire * smpt$etpt + effet.noria)  / (rmpp_mod$salaire * smpt$etpt) - 1
coef.var.effectifs                <-  (rmpp_mod$salaire * smpt$etpt + effet.noria + effet.variation.effectifs)  / (rmpp_mod$salaire * smpt$etpt + effet.noria) - 1
coef.vacances                     <- (rmpp_mod$salaire * smpt$etpt + effet.noria + effet.variation.effectifs + effet.vacances) / (rmpp_mod$salaire * smpt$etpt + effet.noria + effet.variation.effectifs) - 1
coef.smpt                         <- (1 + coef.entrées.ant) * (1 + coef.noria) * (1 + coef.var.effectifs) * (1 + coef.vacances) - 1 

qualité                           <- ((rmpp$salaire * smpt$etpt + effet.entrées.ant + effet.noria + effet.variation.effectifs + effet.vacances) - smpt$salaire * smpt$etpt)/(smpt$etpt * smpt$salaire) * 100
delta.coef                        <- (smpt$salaire / rmpp$salaire - (1 + coef.smpt))

var.relative <- function(v) (shift(v, 1, type = "lead")/v - 1)

var.rmpp      <- var.relative(rmpp$salaire)
effet.es      <- (1 + shift(coef.smpt + delta.coef, 1, type = "lead"))/(1 + coef.smpt + delta.coef) - 1
total         <- (1 + effet.es) * (1 + var.rmpp) - 1
var.smpt      <- var.relative(smpt$salaire)
var.période   <- sapply(list(var.rmpp, effet.es, total, var.smpt), function(x) (prod((1 + x), na.rm = TRUE) - 1)[1])

rmpp.salaire <- rmpp$salaire
smpt.salaire <- smpt$salaire
rmpp.etpt    <- rmpp$etpt
smpt.etpt    <- smpt$etpt

geom  <- function(x) exp(mean(log(x)))
moy.géom.effets      <- sapply(list(rmpp.salaire, 1 + coef.entrées.ant, 1 + coef.noria, 1 + coef.var.effectifs, 1 + coef.vacances, 1 + coef.smpt, 1, smpt.salaire), geom)
moy.géom.effets[2:6] <- moy.géom.effets[2:6] - 1
moy.géom.effets[7]   <- moy.géom.effets[8] / moy.géom.effets[1] - prod(moy.géom.effets[2:6]) - 1

rmpp.salaire       <- c(rmpp.salaire, moy.géom.effets[1])
coef.entrées.ant   <- c(coef.entrées.ant, moy.géom.effets[2])
coef.noria         <- c(coef.noria, moy.géom.effets[3])
coef.var.effectifs <- c(coef.var.effectifs, moy.géom.effets[4])
coef.vacances      <- c(coef.vacances, moy.géom.effets[5])
coef.smpt          <- c(coef.smpt, moy.géom.effets[6])
delta.coef         <- c(delta.coef, moy.géom.effets[7])
smpt.salaire       <- c(smpt.salaire, moy.géom.effets[8])

#### Mise en forme ####

for (x in c("effet.noria",
            "effet.variation.effectifs",
            "effet.vacances",
            "effet.total.entrées.sorties")) assign(x %+% ".form", form(get(x), total = TRUE))

taux.rotation.form <- form(taux.rotation)
rmpp.form          <- form(rmpp.salaire)
smpt.form          <- form(smpt.salaire)

for(x in c("effet.noria.relatif", 
         "effet.variation.effectifs.relatif", 
         "effet.vacances.relatif",
         "effet.total.entrées.sorties.relatif",
         "effet.total.entrées.sorties.relatif",
         "coef.entrées.ant",
         "coef.noria",
         "coef.var.effectifs",
         "coef.vacances",
         "coef.smpt",
         "var.smpt",
         "total",
         "effet.es",
         "var.rmpp",
         "variation.effectifs")) assign(x %+% ".form", form(get(x) * 100))

var.période.form <- form(var.période * 100)[1:4]

var.rmpp.form[durée.sous.revue] <-  var.période.form[1]
effet.es.form[durée.sous.revue] <-  var.période.form[2]
total.form[durée.sous.revue]    <-  var.période.form[3]
var.smpt.form[durée.sous.revue] <-  var.période.form[4]

var.rmpp.form <- var.rmpp.form[1:durée.sous.revue] 
effet.es.form <- effet.es.form[1:durée.sous.revue] 
total.form    <- total.form[1:durée.sous.revue]    
var.smpt.form <- var.smpt.form[1:durée.sous.revue] 

delta.coef.form <- form(delta.coef, total = FALSE, digits = 2)

#### Affichage ####

if (afficher.tableau) {                        
  
print(
  Tableau.vertical2(colnames = c("Annee", 
                                 "Effectifs",
                                "ETPT",  
                                "ETPT entrants",
                                "ETPT sortants",
                                "Entrants",
                                "Sortants",
                                "Var. effectifs",
                                "Taux de rotation %"),
                    rownames = c(as.character(période), "Total"),
                    form(effectifs.janvier),
                    form(smpt$etpt),
                    form(etpt.ent, total = TRUE),
                    form(etpt.sort, total = TRUE),
                    form(nent, total = TRUE),
                    form(nsort, total = TRUE),
                    form(variation.effectifs, total = TRUE),
                    taux.rotation.form))

print(  
      Tableau.vertical2(colnames = c("Annee", 
                                    "Effet noria",
                                    "% SMPT",
                                    "Effet var. effectifs",
                                    "% SMPT",
                                    "Effet vacances",
                                    "% SMPT",
                                    "Total",
                                    "% SMPT"),
                      rownames = c(as.character(période), "Total"),
                      effet.noria.form,
                      effet.noria.relatif.form,
                      effet.variation.effectifs.form,
                      effet.variation.effectifs.relatif.form,
                      effet.vacances.form,
                      effet.vacances.relatif.form,
                      effet.total.entrées.sorties.form,
                      effet.total.entrées.sorties.relatif.form))


chop  <- function(v) v[-length(v)]

print(  
  Tableau.vertical2(colnames = c("Annee", 
                                  "RMPP",
                                  "Entrées n - 1",
                                  "Noria",
                                  "Var. effectifs",
                                  "Vacances",
                                  "Total E/S",
                                  "Ajustement",
                                  "SMPT"),
                    rownames = c(as.character(période), "Moyenne géom."),
                    chop(rmpp.form), 
                    chop(coef.entrées.ant.form), 
                    chop(coef.noria.form),
                    chop(coef.var.effectifs.form),
                    chop(coef.vacances.form),
                    chop(coef.smpt.form),
                    chop(delta.coef.form),     
                    chop(smpt.form)))

rownames <- as.character(période %+% "-" %+% shift(période, 1, type = "lead"))
rownames[length(rownames)] <- as.character(début.période.sous.revue) %+% "-" %+% as.character(fin.période.sous.revue)

print(
  Tableau.vertical2(colnames = c("Annee",
                                  "Var. RMPP",
                                  "Var. effets E/S",
                                  "Cumul",
                                  "Var. SMPT"),
                    rownames = rownames,
                    var.rmpp.form, 
                    effet.es.form,
                    total.form,
                    var.smpt.form))
 }

output <- c("période",
            "effectifs.janvier",
            "etpt.ent",
            "etpt.sort",
            "nent",
            "nsort",
            "variation.effectifs",
            "taux.rotation",
            "effet.noria",
            "effet.variation.effectifs",
            "effet.vacances",
            "effet.total.entrées.sorties",
            "rmpp.salaire",
            "smpt.salaire",
            "rmpp.etpt",
            "smpt.etpt",
            "coef.entrées.ant",
            "coef.noria",
            "coef.var.effectifs",
            "coef.vacances",
            "coef.smpt",
            "var.rmpp",
            "effet.es",
            "var.smpt",
            "qualité",
            "delta.coef")

# Astuce pour récupérer une data.table avec les noms de colonnes.

do.call(data.table, sapply(output, get, envir = environment()))
}

# rmpp$salaire,
# smpt$salaire,
# rmpp$etpt,
# smpt$etpt,
# qualité,
# delta.coef
