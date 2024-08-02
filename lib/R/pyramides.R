# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'annee 2008.
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


#' Produire les pyramides des âges par versant de la fonction publique
#' @param  Filtre_bulletins  Fonction permettant de filtrer les bulletins sur les lignes de data.table
#' @param  titre  Titre de la pyramide.
#' @param  versant Versant de la fonction publique ("FPT" ou "FPH")
#' @param  envir Environnement de stockage des caractéristiques des âges (\code{nom.fichier.apres}, \code{nom.fichier.avant}, \code{res} quartiles de distribution des âges)
#' @export
#'
produire_pyramides <- function(Filtre_bulletins, titre, versant = "", envir) {
  
  essayer(label = "+pyramides", produire_pyramides_(Filtre_bulletins, titre, versant, envir), cat("La", titre, "n'a pas pu être générée."))
}
  
produire_pyramides_ <- function(Filtre_bulletins, titre, versant = "", envir) {

  annee.fin.comp <- if (versant != "") {
                      max(debut.periode.sous.revue,
                           min(altair::annee_comparaison(versant)$annee, fin.periode.sous.revue, na.rm = TRUE), na.rm = TRUE)
                  } else fin.periode.sous.revue

  # Extraire les matricules et Nir du début et de la fin de la periode sous revue
  
  extraire_paye(debut.periode.sous.revue, Filtre_bulletins, out = "Bulletins.début.psr")
  
  extraire_paye(fin.periode.sous.revue, Filtre_bulletins, out = "Bulletins.fin.psr")
  
  # Répartition par âge et sexe des individus ayant un NIR en début et fin de periode sous revue
  
       ages.début.psr <- extraire.nir(Bulletins.début.psr, debut.periode.sous.revue, 
                                      ifelse(is.null(Filtre_bulletins), "", paste(Filtre_bulletins, collapse="_")))
  
         ages.fin.psr <- extraire.nir(Bulletins.fin.psr, fin.periode.sous.revue, 
                                      ifelse(is.null(Filtre_bulletins), "", paste(Filtre_bulletins, collapse="_")))
  
  # Extrait la répartition par âge et sexe des individus ayant un NIR.
  #    extraire.nir(Base, annee)
         # 
         # Base	
         #    data.table contenant au moins une variable nommée Nir décrivant le NIR.
         # annee	
         #    Annee civile à la fin de laquelle est évalué l'âge de l'individu. 
         # 

  # Produire les pyramides
 
  pyramides(Bulletins.début.psr, 
            Bulletins.fin.psr,
            ages.début.psr,
            ages.fin.psr,
            titre,
            versant,
            envir = envir)
  
  # ajustement des contraintes ASCII
  
  stub <- gsub(" ", "-", sub("â", "a", titre)) %+% "_"
  
  # Utilisation de l'environnement e pour récupérer les noms de fichier des âges début et fin de periode sous revue
  
  envir$nom.fichier.avant <- stub %+% debut.periode.sous.revue
  envir$nom.fichier.apres <- stub %+% fin.periode.sous.revue
  
  # Sauvegarde des bases des âges début et fin de periode sous revue
  Sauv.base("Effectifs",  "Bulletins.début.psr", "Bulletins.début.psr", environment = environment())
  Sauv.base("Effectifs",  "Bulletins.fin.psr", "Bulletins.fin.psr", environment = environment())
  Sauv.base("Effectifs",  "ages.début.psr", envir$nom.fichier.avant, environment = environment())
  Sauv.base("Effectifs",  "ages.fin.psr", envir$nom.fichier.apres, environment = environment())
  
}

pyramides <- function(Bulletins.début.psr, 
                      Bulletins.fin.psr, 
                      ages.début.psr, 
                      ages.fin.psr,
                      titre,
                      versant,
                      envir) {
                      
m.début <- Bulletins.début.psr[ , age]
m.fin <- Bulletins.fin.psr[ , age]

if (length(m.début) == 0) m.début <- 0
if (length(m.fin) == 0)   m.fin <- 0

envir$res <- Resume(c("Âge des personnels <br>au 31/12/" %+% debut.periode.sous.revue,
               "Eff. physique",
               "Âge des personnels <br>au 31/12/" %+% fin.periode.sous.revue,
               "Eff. physique"),
               list(m.début, 
                    m.fin),
               extra = "length",
               align = 'c',
               type = "standard")

mesg1 <- FALSE
mesg2 <- FALSE

if (longueur.non.na(ages.début.psr) == 0) {
  mesg1 <- TRUE
  ages.début.psr <- ages.fin.psr
} 

if (longueur.non.na(ages.fin.psr) == 0) {
  mesg2 <- TRUE
  ages.fin.psr <- ages.début.psr
}

#  ----- On peut forcer le versant de la FP en renseignant versant
#        sinon détection automatique par VERSANT_FP
 

if (longueur.non.na(ages.début.psr) > 0 || longueur.non.na(ages.fin.psr) > 0) {
  
  if (! is.null(ages.fin.psr)) {
    
    pyramide_ages(ages.début.psr, ages.fin.psr, titre) 

    if (versant != "")  {  
          pyramide_ages(ages.fin.psr, date.fin = fin.periode.sous.revue, versant = versant)
    } 
    
  } else {
    
    pyramide_ages(après)
    
  }

  H0 <- ages.début.psr[ , .(Hommes = sum(Hommes, na.rm = TRUE), 
                           Femmes = sum(Femmes, na.rm = TRUE)),
                            by = floor(age / 5)][
                        , Total := Hommes + Femmes]
 
  H1 <- ages.fin.psr[ , .(Hommes = sum(Hommes, na.rm = TRUE), 
                          Femmes = sum(Femmes, na.rm = TRUE)),
                          by = floor(age / 5)][ 
                      , Total := Hommes + Femmes]
  
  H <- H1 - H0
  
  if (mesg1) {
     cat("\nLa pyramide des âges de début de periode ne peut être produite.\n   ")
  }
  
  if (mesg2) {
     cat("\nLa pyramide des âges de fin de periode ne peut être produite.\n   ")
  }
    
} 

newpage()

if  (exists("H") && ! identical(ages.fin.psr, ages.début.psr) && longueur.non.na(H$Total) > 0) {
  
  # la valeur y du plot serait plutôt c(-3,20) pour une sortie R pure. On privilégie le formatage Rmd à c(-1, 20)
  
  plot(c(min(H$Total, na.rm = TRUE), max(H$Total, na.rm = TRUE)), c(-1, 20), type = "n", frame = FALSE, axes = FALSE, xlab = "", ylab = "",
       main = "Evolution des effectifs par tranche d'âge")
  
  barplot(H$Total, 
          width = 1.5,
          names.arg=c("15-20", 
                      "20-25",
                      "25-30",
                      "30-35",
                      "35-40",
                      "40-45",
                      "45-50",
                      "50-55",
                      "55-60",
                      "60-65",
                      "65-70"),
          xlab = "Variation d'effectifs",
          ylab = "Tranche d'âge",
          xlim = c(min(H$Total, na.rm = TRUE), max(H$Total, na.rm = TRUE)),
          xpd  = FALSE,
          col  = "lightgreen",
          horiz = TRUE,
          add = TRUE,
          cex.names = 0.8)
  

  barplot(H$Femmes, 
          width=1.5,
          col  = "deeppink",
          horiz = TRUE,
          density = 10,
          xlim = c(min(H$Total, na.rm = TRUE), max(H$Total, na.rm = TRUE)),
          xpd  = FALSE,
          add = TRUE)
  
  legend("bottomleft", fill=c("lightgreen", "deeppink"), density=c(NA, 20),
         legend=c("Total " %+% debut.periode.sous.revue %+% "-" %+% fin.periode.sous.revue,
                  "    dont Femmes "), cex = 0.8)
  

} else {
  
  cat("Le graphique des variations d'effectifs par tranche d'âge ne peut être produit.")
}
}


# frame version, added since rev. 1.4, 4th September 2014.
# (C) Minato Nakazawa <minato-nakazawa@umin.net>

pyramidf <- function(data, 
                     Laxis=NULL, 
                     Raxis=NULL,
                     frame=c(-1.15, 1.15, -0.05, 1.1),
                     AxisFM="d",
                     AxisBM="",
                     AxisBI=3,
                     Cgap=0.3, Cstep=5, Csize=1,
                     Rlab="Hommes",
                     Llab="Femmes", 
                     Clab="Âges", 
                     GL=TRUE, 
                     Cadj=-0.03,
                     Rcol="cadetblue1",
                     Lcol="thistle1",
                     Ldens=-1,
                     Rdens=-1,
                     main="",
                     linewidth=2, 
                     ...) {

  Left <- data$Femmes

  Right <- data$Hommes

  if (ncol(data)==2) { Center <- row.names(data) } else { Center <- data$age }
  if (is.null(Laxis)) { Laxis <- seq(0,ceiling(max(c(Left,Right), na.rm=TRUE)/10)*10,len=5) }
  if (is.null(Raxis)) { Raxis <- Laxis }

  # setting x-y axes

  BX <- c(-1-Cgap/2,1+Cgap/2)
  BY <- c(-0.05,1.1)
  XC <- function(XB) { (XB-BX[1])*(frame[2]-frame[1])/(2+Cgap)+frame[1] }
  YC <- function(YB) { (YB-BY[1])*(frame[4]-frame[3])/1.15+frame[3] }

  # scaling factors

  LL <- max(Laxis, na.rm = TRUE)
  LR <- min(Laxis, na.rm = TRUE)
  LS <- LL-LR
  LI <- length(Laxis)
  RL <- min(Raxis, na.rm = TRUE)
  RR <- max(Raxis, na.rm = TRUE)
  RS <- RR-RL
  RI <- length(Raxis)

  # ticks of axis

  segments(XC(-(Laxis-LR)/LS-Cgap/2),YC(-0.01),XC(-(Laxis-LR)/LS-Cgap/2),YC(0.01))
  segments(XC((Raxis-RL)/RS+Cgap/2),YC(-0.01),XC((Raxis-RL)/RS+Cgap/2),YC(0.01))

  # vertical grid lines

  if (GL) {
    segments(XC(-(Laxis-LR)/LS-Cgap/2),YC(0),XC(-(Laxis-LR)/LS-Cgap/2),YC(1),
             lty=3,col="blue")
    segments(XC((Raxis-RL)/RS+Cgap/2),YC(0),XC((Raxis-RL)/RS+Cgap/2),YC(1),
             lty=3,col="blue")
  }

  # axes

  lines(c(XC(-1-Cgap/2),XC(-Cgap/2)),c(YC(0),YC(0)),lty=1)
  lines(c(XC(-Cgap/2),XC(-Cgap/2)),c(YC(0),YC(1)),lty=1)
  lines(c(XC(1+Cgap/2),XC(Cgap/2)),c(YC(0),YC(0)),lty=1)
  lines(c(XC(Cgap/2),XC(Cgap/2)),c(YC(0),YC(1)),lty=1)

  # labels

  text(XC(-0.5-Cgap/2),YC(1),Llab,pos=3)
  text(XC(0.5+Cgap/2),YC(1),Rlab,pos=3)
  text(XC(0),YC(1),Clab,pos=3)
  Ci <- length(Center)
  for (i in 0:(Ci-1)) {
    if ((i%%Cstep)==0) { text(XC(0),YC(i/Ci+Cadj),paste(Center[i+1]),pos=3,cex=Csize) }
  }
  text(XC(-(Laxis-LR)/LS-Cgap/2),YC(rep(0,LI)),
       paste(formatC(Laxis,format=AxisFM,big.mark=AxisBM,big.interval=AxisBI)),pos=1)
  text(XC((Raxis-RL)/RS+Cgap/2),YC(rep(0,RI)),
       paste(formatC(Raxis,format=AxisFM,big.mark=AxisBM,big.interval=AxisBI)),pos=1)

  # main text (above the frame)

  if (length(main)>0) { text(XC(0), YC(1.1), main, pos=3) }

  # draw rectangles

  VB <- 0:(Ci-1)/Ci
  VT <- 1:Ci/Ci
  LeftP <- -(Left-LR)/LS-Cgap/2
  rect(XC(LeftP),YC(VB),XC(rep(-Cgap/2,Ci)),YC(VT),col=Lcol,density=Ldens, lwd=linewidth)
  RightP <- (Right-RL)/RS+Cgap/2
  rect(XC(rep(Cgap/2,Ci)),YC(VB),XC(RightP),YC(VT),col=Rcol,density=Rdens,lwd=linewidth)

  return(list(Raxis, Laxis))
}

#' Pyramide des âges.
#'
#' Elabore une pyramide des âges verticale avec superposition du début et de la fin de la periode sous revue.
#'
#' @param Avant data.table/data.frame décrivant la situation en début de periode
#'        Cette base doit avoir la forme suivante (bornes d'âges non impératifs):
#'        \tabular{ccc}{
#'          age \tab Hommes \tab Femmes \cr
#'          15  \tab   0  \tab    1   \cr
#'          16  \tab   NA \tab    2   \cr
#'          17  \tab   1  \tab    3   \cr
#'          18  \tab  409 \tab    52  \cr
#'          ... \tab  ... \tab ...    \cr
#'          68  \tab 2216 \tab    NA
#'        }
#'        dans laquelle "age" peut être soit un vecteur de nom de lignes soit une colonne.
#' @param Après data.table/data.frame décrivant la situation en fin de periode. Même format que \code{Avant}.
#' @param titre Titre du graphique.
#' @param date.début date du début de la periode.
#' @param date.fin date de fin de periode.
#' @param versant Si non renseigné, sans effet. Si renseigné par "FPT" (resp. "FPH"), le deuxième argument \code{Après} ne
#'                doit pas être renseigné. Il est automatiquement remplacé par une base de données disponible dans le répertoire \code{data/}
#'                du paquet, correspondant à l'annee la plus proche du versant de la fonction publique correspondant. La pyramide superposée représente
#'                celle qu'aurait l'organisme si la distribution de ses âges était celle du versant mentionné de la fonction publique.
#' @param couleur_H couleur utilisée pour représenter les hommes (partie droite de la pyramide). Par défaut \code{darkslateblue}
#' @param couleur_F couleur utilisée pour représenter les femmes (partie gauche de la pyramide). Par défaut \code{firebrick4}
#' @param envir environnement
#' @return Une liste de deux vecteurs numériques représentant chacun des axes (gauche puis droit).
#'         Un graphique comprenat une pyramide, une légende et éventuellement un titre.
#' @examples
#' pyramide_ages(df1, NULL, "Pyramide des âges", 2008, 2012, versant = "FPT", comparer = TRUE)
#' @export

pyramide_ages <- function(Avant,
                          Après = NULL,
                          titre = "",
                          date.début = debut.periode.sous.revue,
                          date.fin = fin.periode.sous.revue,
                          versant = "",
                          couleur_H = "darkslateblue",
                          couleur_F = "firebrick4") {

  
  if (versant != "") {

    compar <- annee_comparaison(versant)
    annee.reference <- compar$annee
    pyr <- compar$pyr
    
    if (is.null(pyr)) {
      cat("La comparaison ne peut pas être effectuée.")
      return(0)
    }

    tot <- pyr[ , .(H = sum(Hommes), F = sum(Femmes))]
    
    s.avant <- Avant[ , .(H = sum(Hommes, na.rm=TRUE), F = sum(Femmes, na.rm=TRUE))]

    H.coef.forme <- s.avant$H / tot$H
    F.coef.forme <- s.avant$F / tot$F

    pyr[ , `:=`(Hommes = Hommes * H.coef.forme,
                Femmes = Femmes * F.coef.forme)]

    leg <- ""
    if (grepl("non", versant, ignore.case = TRUE)) leg <- "non-"
    if (grepl("tit", versant, ignore.case = TRUE)) leg <- leg %+% "titulaires "      
    if (grepl("fpt", versant, ignore.case = TRUE)) leg <- leg %+% "FPT"      
    if (grepl("fph", versant, ignore.case = TRUE)) leg <- leg %+% "FPH"      
    
    pyramide_ages(Avant,
                  pyr,
                  "Comparaison avec les données nationales au 31 décembre " %+% annee.reference,
                  "organisme " %+% date.fin,
                  paste(leg, annee.reference))

    cat("Pour obtenir les effectifs nationaux, multiplier les abscisses des hommes par", formatC(round(1 / H.coef.forme), big.mark = " "),
        "et les abscisses des femmes par", formatC(round(1 / F.coef.forme), big.mark = " "))

    return(0)

  }

  plot(c(0,100), c(0,100), type = "n", frame = FALSE, axes = FALSE, xlab = "", ylab = "",
       main = titre)

  axes <- pyramidf(Avant, frame = c(10, 75, 0, 90), linewidth = 1)

  if (! is.null(Après)) {

    pyramidf(Après, Laxis = axes[[1]], Raxis = axes[[2]], frame = c(10, 75, 0, 90),
             Rcol = couleur_H, Lcol = couleur_F,
             #Lcol="deepskyblue", Rcol = "deeppink",
             Ldens = 7, Rdens = 7)

    legend("right", fill = c("thistle1", "cadetblue1", "firebrick4", "darkslateblue"), density = c(NA, NA, 25, 25),
           legend = c("Femmes " %+% date.début, "Hommes " %+% date.début,
                      "Femmes " %+% date.fin, "Hommes " %+% date.fin), cex = 0.8)
  } else {

    legend("right", fill = c("thistle1", "cadetblue1"), density = c(NA, NA),
           legend = c("Femmes " %+% date.début, "Hommes " %+% date.début), cex = 0.8)
  }

  return(0)

}






