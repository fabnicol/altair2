
source("bibliothèque.altair.R")
installer.paquets(qtutils, qtbase, rigoureusement = TRUE)
library(compiler)
enableJIT(3)
library(qtbase)

# Fonctions globales


#' Interface graphique assistant
#'
#' assistant() lance l'interface graphique Altair qui permet de renseigner
#' la totalité des champs nécessaires au traitement statistique.
#' grâce au générateur \code{\link{altair.générateur}} de la classe Altair.
#' @note Uniquement testé sur linux et Qt 4.8.5 à l'aide du paquet qtbase
#' @author Fabrice Nicol
#' @seealso Link to functions in the same package with
#' \code{\link{base.générateur}} \code{\link{altair.générateur}}
#' paquet qtbase: 
#' http:\\
#' @examples
#' # Génère altair pour les années 2009 à 2013 et enregistre les bases 
#' # .csv en sortie du programme
#' assistant()
#' @export

assistant <- function()
{
sélectionner.répertoire <- function(x, z) 
{
  qconnect(x, "clicked",
           function()
           {
             filename <- Qt$QFileDialog$getExistingDirectory(NULL,
                                                             "Sélectionner un répertoire", 
                                                             getwd())
             if(nchar(filename) != 0) 
               {
                  base.name <- basename(filename)
                  x$setText(base.name)
                  x$resize(6*x$width, x$height)
                  boutons.hash[z] <<- filename
               }
             wizard$resize(largeur.initiale*1.5, hauteur.initiale)  
           })
}

boutons.hash <- list()

sélectionner <- function(y, z, type)
{
  qconnect(y, "clicked", function() 
    {

    name_filter <- paste0(type, " (*.", type, ")")
    
    # Contrairement à Qt C++, les résultats de qtbase pour un dialogue getOpenFileNames 
    # ne sont pas sous forme de liste mais de vecteur
    
    filenames <<- Qt$QFileDialog$getOpenFileNames(NULL, paste("Sélectionner un fichier", type, "..."), getwd(), name_filter)
    if(!is.null(filenames) && sum(nchar(filenames)) != 0)  
      {
         y$setText(do.call(paste, list(basename(filenames), collapse = "\n")))
         y$resize(1.5*y$width, length(filenames)*y$height)
         boutons.hash[[z]] <<- filenames
      }
    #wizard$repaint()
    #wizard$resize(largeur.initiale*1.5, hauteur.initiale*1.5)  
        
   })
}

sélectionner.csv <- function(y, z) sélectionner(y, z, "csv")
sélectionner.xhl <- function(y, z) sélectionner(y, z, "xhl")

source("bibliothèque.altair.R", encoding="UTF-8")
source("classes.R", encoding="UTF-8")

altair <<- altair.générateur$new()

## Caractéristiques globales ##  

wizard <- Qt$QWizard()
wizard$setWindowTitle("Analyse des Lignes de Traitement, Attributions Indemnitaires et autres Rémunérations (ALTAIR)")
wizard$setWizardStyle(Qt$QWizard$MacStyle)
wizard$setPixmap(Qt$QWizard$BackgroundPixmap, Qt$QPixmap("altair.png"))
wizard$setGeometry(600,400,650,300)

wizard$setButtonText(Qt$QWizard$BackButton, "Page précédente")
wizard$setButtonText(Qt$QWizard$NextButton, "Page suivante")
wizard$setButtonText(Qt$QWizard$FinishButton, "Terminer")
wizard$setButtonText(Qt$QWizard$CancelButton, "Annuler")

annee.courante <- as.numeric(substr(Sys.Date(), 0, 4))

## Première page ##

info.étiquettes <- c(
"champ de détection 1",
"champ de détection 2",
"champ du matricule",
"champ du montant",
"champ du total",
"champ de catégorie",
"champ de codage",
"champ de libellé",
"champ du statut",
"champ du type de rémunération",
"colonnes.sélectionnées",
"libellé de stagiaire",
"libellé de titulaire",
"libellé service élu",
"libellé traitement indiciaire",
"libellé NBI",
"libellé prime/contractuel",
"libellé vacations",
"libellé autres codes")


valeurs.par.défaut <- c(
altair$champ.détection.1,
altair$champ.détection.2,
altair$étiquette.matricule,
altair$étiquette.montant,
altair$étiquette.totalgénéral,
altair$étiquette.catégorie,
altair$étiquette.code,
altair$étiquette.libellé,
altair$étiquette.statut,
altair$étiquette.type.rémunération,
paste(altair$colonnes.sélectionnées, collapse=" "),
altair$code.stagiaire,
altair$code.titulaire,
altair$code.élu,
altair$code.traitement,
altair$code.nbi,
altair$code.prime.ou.contractuel,
altair$code.vacation,
altair$code.autre)


commentaires.champs.noms <- c(
  "libellé de champ de la base qui sera détecté en priorité",
  "libellé de champ de la base qui sera détecté sinon",
  "libellé exact du champ du matricule" ,
  "libellé exact du champ du montant",
  "libellé exact du champ du total",
  "libellé exact du champ de catégorie",
  "libellé exact du champ de codage",
  "libellé exact du champ de libellé",
  "libellé exact du champ du statut",
  "champs qui seront inclus en colonnes dans la base globale, séparés par au moins un espace",
  "libellé exact du champ du type de rémunération",
  "libellé exact de stagiaire",
  "libellé exact de titulaire",
  "libellé de classification du service des élus",
  "libellé de classification d'un code de type Traitement",
  "libellé de classification d'un code de type NBI",
  "libellé de classification d'un code de type Indemnités ou Rémunération contractuelle",
  "libellé de classification d'un code de type Vacations",
  "libellé de classification d'un code de type Autres rémunérations")


info.form.layout <- Qt$QFormLayout()
info.form.layout$setAlignment(Qt$Qt$AlignRight)

formulaire <- list()

créer.lignes <- function(x, z, t) 
                     {
                         line.edit <- Qt$QLineEdit()
                         line.edit$setToolTip(z)
                         line.edit$setText(t)
                         line.edit$setAlignment(Qt$Qt$AlignRight)
                         info.form.layout$addRow(x, line.edit)
                         formulaire <<- c(formulaire, line.edit)
                         
                         return(line.edit)
                      }

mapply(créer.lignes, info.étiquettes, commentaires.champs.noms, valeurs.par.défaut, SIMPLIFY = FALSE)

info_page <- Qt$QWizardPage(wizard)
info_page$setTitle("Champs et libellés")
info_page$setLayout(info.form.layout)
wizard$addPage(info_page)

## Deuxième page ##

periode_page <- Qt$QWizardPage(wizard)
periode_page$setTitle("Dates et durées")

info.étiquettes.2 <- "format de date"

valeurs.par.défaut.2 <- "%d-%m-%y"

commentaires.champs.noms.2 <- "Utiliser %d pour jour,\n %m pour mois, %y pour année. La convention française est la valeur par défaut."  

info.form.layout <- Qt$QFormLayout()

ligne.format.date <- créer.lignes(info.étiquettes.2, commentaires.champs.noms.2, valeurs.par.défaut.2)

ligne.format.date$setMaximumWidth(100)

période <- (annee.courante-10):annee.courante

nombre.de.jours.de.travail <- Qt$QComboBox()
nombre.de.jours.de.travail$addItems((altair$seuil.troncature-20):altair$seuil.troncature+20)
nombre.de.jours.de.travail$setCurrentIndex(20)
nombre.de.jours.de.travail$setToolTip( "Nombre de jours minimum qui devront être enregistrés en base de paie, au cours de la première et la dernière année de travail\npour pouvoir être pris en compte dans les statistiques relatives à ces deux années.")

début.période.sous.revue <- Qt$QComboBox()
début.période.sous.revue$addItems(période)
début.période.sous.revue$setCurrentIndex(5)
début.période.sous.revue$setToolTip("Donner le premier exercice sous revue")

fin.période.sous.revue <- Qt$QComboBox()
fin.période.sous.revue$addItems(période)
fin.période.sous.revue$setCurrentIndex(9)
fin.période.sous.revue$setToolTip("Donner le dernier exercice sous revue")

formulaire <- c(formulaire, nombre.de.jours.de.travail, début.période.sous.revue, fin.période.sous.revue)

info.form.layout$addRow("Nombre minimum de jours de travail", nombre.de.jours.de.travail)
info.form.layout$addRow("Période sous revue : premier exercice", début.période.sous.revue)
info.form.layout$addRow("Période sous revue : dernier exercice", fin.période.sous.revue)

periode_layout <- Qt$QGridLayout()
periode_layout$setColumnMinimumWidth(0, 40)
periode_layout$setColumnMinimumWidth(2, 40)
periode_layout$setRowMinimumHeight(0, 20)
periode_layout$addLayout(info.form.layout,1, 1)
periode_layout$setRowMinimumHeight(2, 20)
periode_page$setLayout(periode_layout)

wizard$addPage(periode_page)

## Troisième page  ##

base_page <- Qt$QWizardPage(wizard)
base_page$setTitle("Importer")
base_page$setLayout(base_layout <- Qt$QGridLayout())

base<-data.frame()

indicesOk <- Qt$QCheckBox("Sélectionner tous les fichiers\nde même nom racine")
indicesOk$setChecked(TRUE);

base.étiquettes <- c("Fichiers Xémélios", "Lignes de paie", "Bulletins de paie", "Nouvelle bonification indiciaire", "Codes", "Avantages en nature", "Categories")

parseXml <- Qt$QCheckBox("Importer directement les fichiers xhl")

fichiers.xhl.gbox <- Qt$QGroupBox("Bases xhl")
fichiers.csv.gbox <- Qt$QGroupBox("Bases csv")

xhl.gbox.layout <- Qt$QGridLayout()
csv.gbox.layout <- Qt$QGridLayout()

fichiers.csv.layout <- Qt$QFormLayout()
fichiers.xhl.layout <- Qt$QFormLayout()

valeurs.par.défaut <- c(altair$nom.de.fichier.xhl,
                        altair$nom.de.fichier.lignes,
                        altair$nom.de.fichier.bulletins,
                        altair$nom.de.fichier.nbi,
                        altair$nom.de.fichier.codes,
                        altair$nom.de.fichier.avantages,
                        altair$nom.de.fichier.catégories)

largeur.initiale <- wizard$sizeHint$width()
hauteur.initiale <- wizard$sizeHint$height()

créer.boutons.fichiers <- function(étiquette, défaut) 
{
  bouton <- Qt$QPushButton("Sélectionner le fichier")
  boutons.hash[étiquette] <<- défaut
  if (étiquette != "Fichiers Xémélios") 
  {
    sélectionner.csv(bouton, étiquette)
    fichiers.csv.layout$addRow(étiquette, bouton)
  }
  else
  {
    sélectionner.xhl(bouton, étiquette)
    fichiers.xhl.layout$addRow(étiquette, bouton)
  }
  return(bouton)
}

boutons.bases <- mapply(créer.boutons.fichiers, base.étiquettes, valeurs.par.défaut)

names(boutons.bases) <- base.étiquettes

xhl.gbox.layout$addLayout(fichiers.xhl.layout, 0, 0)

csv.gbox.layout$addLayout(fichiers.csv.layout, 2, 0)
csv.gbox.layout$addWidget(indicesOk, 0, 0, Qt$Qt$AlignLeft)
csv.gbox.layout$setRowMinimumHeight(1, 10)

fichiers.csv.gbox$setEnabled(!parseXml$checked)
fichiers.xhl.gbox$setEnabled(parseXml$checked)
fichiers.xhl.gbox$setLayout(xhl.gbox.layout)
fichiers.csv.gbox$setLayout(csv.gbox.layout)

formulaire <- c(formulaire, boutons.bases)

button_box <-  Qt$QDialogButtonBox()
button_box$addButton("Annuler", Qt$QDialogButtonBox$RejectRole)
button_box$addButton("Importer les bases", Qt$QDialogButtonBox$AcceptRole)

base_layout$addWidget(parseXml, 0, 1)
base_layout$addWidget(fichiers.xhl.gbox, 2, 1)
base_layout$addWidget(fichiers.csv.gbox, 3, 1)
base_layout$addWidget(button_box, 5, 1, Qt$Qt$AlignRight)
base_layout$setRowMinimumHeight(1, 20)
base_layout$setRowMinimumHeight(4, 10)

base_page$setLayout(base_layout)
wizard$addPage(base_page)

qconnect(button_box, "rejected", function() window$hide())

qconnect(button_box, "accepted",
         function()
         {
           lapply(boutons.bases,
                  function(x)
                    if(!is.null(x$text))
                    {
                      args <- list(file=x$text)
                      val <- do.call("read.csv", args)
                      assign("base", val, .GlobalEnv)
                      message("La base est : ", base)
                    }
                  else
                  {
                    Qt$QMessageBox$warning(parent = window,
                                           title = "Attention !",
                                           text = "Sélectionner un fichier")
                  })
         })

qconnect(parseXml, "clicked", function() 
                                {
                                  fichiers.csv.gbox$setEnabled(!parseXml$checked)
                                  fichiers.xhl.gbox$setEnabled(parseXml$checked)
                                })


## Quatrième page  ##

actions_page <- Qt$QWizardPage(wizard)
actions_page$setTitle("Générer")

actions_layout <- Qt$QGridLayout()
actions_layout$setRowMinimumHeight(0,10)

info.out.layout <- Qt$QFormLayout()

info.étiquettes <- c(
"Générer les bases de codes et libellés",
"Générer l'analyse des distributions de rémunération",
"Générer l'analyse des variations de rémunération",
"Générer l'analyse des tests statutaires",
"Générer les bases .csv des résultats statistiques",
"Fusionner les bases en mode intégral",
"Exporter et fusionner les bases bases xhl au format csv")

cases.générer <- list()

mapply(function(x)  
        {
          y <- Qt$QCheckBox()
          y$setChecked(TRUE)
          cases.générer <<- c(cases.générer, y)
          info.out.layout$addRow(x, y)
        },
        info.étiquettes)

names(cases.générer) <- info.étiquettes 

cases.générer$"Exporter et fusionner les bases bases xhl au format csv"$setChecked(FALSE)

formulaire <- c(formulaire, cases.générer)

chemin.dossiers  <- c("dossier de travail",
                      "dossier des bases",
                      "dossier des statistiques")

valeurs.par.défaut <- c(dossier.travail,
                        altair$dossier.bases,
                        altair$dossier.stats)

créer.boutons.dossiers <- function(étiquette, défaut) 
{
  bouton <- Qt$QPushButton("...")
  boutons.hash[étiquette] <<- défaut
  sélectionner.répertoire(bouton, étiquette)
  info.out.layout$addRow(étiquette, bouton)
  return(bouton)
}

boutons.dossiers <- mapply(créer.boutons.dossiers, chemin.dossiers, valeurs.par.défaut)
names(boutons.bases) <- chemin.dossiers

formulaire <- c(formulaire, boutons.dossiers)

actions_layout$setRowMinimumHeight(6,10)
actions_layout$addLayout(info.out.layout, 7,1)

actions_page$setLayout(actions_layout)

wizard$addPage(actions_page)


############################## Intéraction avec l'environnement et le source  #############################################

  wizard$raise()
  wizard$activateWindow()
  
  response<-wizard$exec()
  
  valeur.widget <- function(y, z)
  {
    if (y$metaObject()$className() == "QLineEdit")
      return(strsplit(y$text, split = " +"))
    else
      if (y$metaObject()$className() == "QPushButton")
        return(boutons.hash[z])
    else
      if (y$metaObject()$className() == "QComboBox")   
        return(list(as.numeric(y$currentText)))
    else   
      if (y$metaObject()$className() == "QCheckBox")
        return(list(y$checked))
  }
  
  if(response)
  {
    # il faut actualiser les variables (objets) avec la saisie dynamique dans l'assistant  (formulaires, boutions...)
    objets <- c(
      "champ.détection.1",
      "champ.détection.2",
      "étiquette.matricule",
      "étiquette.montant",
      "étiquette.totalgénéral",
      "étiquette.catégorie",
      "étiquette.code",
      "étiquette.libellé",
      "étiquette.statut",
      "étiquette.type.rémunération",
      "colonnes.sélectionnées",
      "code.stagiaire",
      "code.titulaire",
      "code.élu",
      "code.traitement",
      "code.nbi",
      "code.prime.ou.contractuel",
      "code.vacation",
      "code.autre",
      "date.format",
      "seuil.troncature",
      "début.période.sous.revue",
      "fin.période.sous.revue",
      "nom.de.fichier.xhl",
      "nom.de.fichier.lignes",
      "nom.de.fichier.bulletins",
      "nom.de.fichier.nbi",
      "nom.de.fichier.codes",
      "nom.de.fichier.avantages",
      "nom.de.fichier.catégories",
      "générer.codes",
      "générer.distributions",
      "générer.variations",
      "générer.tests",
      "générer.bases",
      "fusion.intégrale",
      "décoder.xhl",
      "dossier.travail",
      "dossier.bases",
      "dossier.stats"
      )

    mapply(function(x, y, z) assign(x,  unlist(valeur.widget(y, z)), envir=altair),
           objets,  
           formulaire,
           names(formulaire))
    
  }
}
