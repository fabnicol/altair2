
source("bibliothèque.altair.R")
installer.paquets(qtutils, qtbase, rigoureusement = TRUE)
enableJIT(3)
library(qtbase)


# Fonctions globales

sélectionner.répertoire <- function(x) 
{
  qconnect(x, "clicked",
           function()
           {
             x$setMaximumSize(80, 40)
             filename <- Qt$QFileDialog$getExistingDirectory(NULL,
                                                             "Sélectionner un répertoire", 
                                                             getwd())
             if(!is.null(filename))  x$setText(filename) 
             
           })
  
}


bouton.hash <- list()

sélectionner <- function(y, type)
{
  qconnect(y, "clicked", function() 
    {
    y$setMaximumSize(80, 40)
    name_filter <- paste0(type, " (*.", type, ")")
    
    filenames <<- Qt$QFileDialog$getOpenFileNames(NULL, paste("Sélectionner un fichier", type, "..."), getwd(), name_filter)
    if(!is.null(filename))  y$setText(do.call(paste, filenames, sep = "\n"))
    
    bouton.hash[names(x)] <<- filenames
    
   })
}

sélectionner.csv <- function(y) sélectionner(y, "csv")
sélectionner.xhl <- function(y) sélectionner(y, "xhl")


source("bibliothèque.altair.R", encoding="UTF-8")
source("classes.R", encoding="UTF-8")

altair <- altair.générateur$new()


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
altair$étiquette.code,
altair$étiquette.matricule,
altair$étiquette.montant,
altair$étiquette.totalgénéral,
altair$étiquette.catégorie,
altair$étiquette.code,
altair$étiquette.libellé,
altair$étiquette.statut,
altair$étiquette.type.rémunération,
altair$colonnes.sélectionnées,
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
  "champs qui seront inclus en colonnes dans la base globale, séparés par au moins un espace"
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

chemin.dossier.étiquette <- "chemin du dossier de travail"

info.form.layout$addRow(chemin.dossier.étiquette, dossier.travail.bouton <<- Qt$QPushButton("..."))

créer.lignes <- function(x, z, t)  {
                                   line.edit <- Qt$QLineEdit()
                                   line.edit$setToolTip(z)
                                   line.edit$setText(t)
                                   line.edit$setAlignment(Qt$Qt$AlignRight)
                                   info.form.layout$addRow(x, line.edit)
                                   return(line.edit)
                                }

formulaire <- mapply(créer.lignes, info.étiquettes, commentaires.champs.noms, valeurs.par.défaut, SIMPLIFY = FALSE)

info_page <- Qt$QWizardPage(wizard)
info_page$setTitle("Champs et libellés")
info_page$setLayout(info.form.layout)
wizard$addPage(info_page)


sélectionner.répertoire(dossier.travail.bouton)

## Deuxième page ##

periode_page <- Qt$QWizardPage(wizard)
periode_page$setTitle("Dates et durées")

info.étiquettes.2 <- "format de date"

valeurs.par.défaut.2 <- "%d-%m-%y"

commentaires.champs.noms.2 <- "Utiliser %d pour jour,\n %m pour mois, %y pour année. La convention française est la valeur par défaut."  

info.form.layout <- Qt$QFormLayout()

ligne.format.date <- créer.lignes(info.étiquettes.2, commentaires.champs.noms.2, valeurs.par.défaut.2)

ligne.format.date$setMaximumWidth(80)

formulaire <-  c(formulaire, ligne.format.date)

période <- (annee.courante-10):annee.courante

nombre.de.jours.de.travail <- Qt$QComboBox()
nombre.de.jours.de.travail$addItems(altair$seuil.troncature-20:altair$seuil.troncature+20)
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

info.form.layout$addRow("Nombre minimum de jours de travail", nombre.de.jours.de.travail)
info.form.layout$addRow("Période sous revue : premier exercice", début.période.sous.revue)
info.form.layout$addRow("Période sous revue : dernier exercice", fin.période.sous.revue)

formulaire <- c(formulaire, nombre.de.jours.de.travail, début.période.sous.revue, fin.période.sous.revue)

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

# nom.de.fichier.base       = "character",
# nom.de.fichier.nbi        = "character",

base.étiquettes <- c("Fichiers Xémélios", "Lignes de paie", "Bulletins de paie", "Nouvelle bonification indiciaire", "Codes", "Avantages en nature", "Catégories")

boutons.bases <- lapply(base.étiquettes,
                         function(i) 
                          Qt$QPushButton("Sélectionner le fichier"))

names(boutons.base) <- base.étiquettes

sélectionner.xhl(boutons.bases[[1]])

fichiers.xhl.gbox <- Qt$QGroupBox("Bases xhl")
xhl.gbox.layout <- Qt$QGridLayout()
xhl.gbox.layout$addWidget(parseXml, 0, 0, Qt$Qt$AlignLeft)
fichiers.csv.gbox$setLayout(xhl.gbox.layout)

lapply(boutons.bases[2:length(boutons.bases)], sélectionner.csv)

formulaire <- c(formulaire, boutons.bases)

fichiers.csv.layout <- Qt$QFormLayout()
mapply(fichiers.csv.layout$addRow, base.étiquettes, boutons.bases)

fichiers.csv.gbox <- Qt$QGroupBox("Bases csv")

indicesOk <- Qt$QCheckBox("Sélectionner tous les fichiers\nde même nom racine")

csv.gbox.layout <- Qt$QGridLayout()
csv.gbox.layout$addWidget(indicesOk, 0, 0, Qt$Qt$AlignLeft)
csv.gbox.layout$setRowMinimumHeight(1, 10)
csv.gbox.layout$addLayout(fichiers.csv.layout, 2, 0)

fichiers.csv.gbox$setLayout(csv.gbox.layout)

indicesOk$setChecked(TRUE);

button_box <-  Qt$QDialogButtonBox()
button_box$addButton("Annuler", Qt$QDialogButtonBox$RejectRole)
button_box$addButton("Importer les bases", Qt$QDialogButtonBox$AcceptRole)

base_layout$addWidget(parseXml <- Qt$QCheckBox("Importer directement les fichiers xhl"), 0, 1)
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

info.etiquettes.4 <- c(
"Générer les bases de codes et libellés",
"Générer l'analyse des distributions de rémunération",
"Générer l'analyse des variations de rémunération",
"Générer l'analyse des tests statutaires",
"Générer les bases .csv des résultats statistiques",
"Exporter et fusionner les bases bases xhl au format csv")

boutons.générer <- list()

mapply(function(x)  
        {
          y <- Qt$QCheckBox()
          y$setChecked(TRUE)
          boutons.générer <<- c(boutons.générer, y)
          info.out.layout$addRow(x, y)
        }, info.etiquettes.4)

formulaire <- c(formulaire, boutons.générer)

chemin.dossier.bases <- "chemin du dossier des bases"

info.out.layout$addRow(chemin.dossier.bases, dossier.bases.bouton <- Qt$QPushButton("..."))

chemin.dossier.stats <- "chemin du dossier des statistiques"

info.out.layout$addRow(chemin.dossier.stats, dossier.stats.bouton <- Qt$QPushButton("..."))

formulaire <- c(formulaire, dossier.travail.bouton, dossier.bases.bouton, dossier.stats.bouton)

sélectionner.répertoire(dossier.bases.bouton)
sélectionner.répertoire(dossier.stats.bouton)

actions_layout$setRowMinimumHeight(6,10)

actions_layout$addLayout(info.out.layout, 7,1)

actions_page$setLayout(actions_layout)

wizard$addPage(actions_page)


############################## Intéraction avec l'environnement et le source  #############################################


objets <- lapply(quote(c(
  champ.détection.1,
  champ.détection.2,
  étiquette.matricule,
  étiquette.montant,
  étiquette.totalgénéral,
  étiquette.catégorie,
  étiquette.code,
  étiquette.libellé,
  étiquette.statut,
  étiquette.type.rémunération,
  code.stagiaire,
  code.titulaire,
  code.élu,
  code.traitement,
  code.nbi,
  code.prime.ou.contractuel,
  code.vacation,
  code.autre,
  date.format,
  seuil.troncature,
  début.période.sous.revue,
  fin.période.sous.revue,
  nom.de.fichier.xhl,
  nom.de.fichier.lignes,
  nom.de.fichier.bulletins,
  nom.de.fichier.nbi,
  nom.de.fichier.codes,
  nom.de.fichier.avantages,
  nom.de.fichier.catégories,
  générer.codes,
  générer.distributions,
  générer.variations,
  générer.tests,
  générer.bases,
  décoder.xhl,
  dossier.travail,
  dossier.bases,
  dossier.stats)), deparse)


wizard$setFocus()
wizard$raise()

response<-wizard$exec()

valeur.widget <- function(y)
{
  if (y$metaObject()$className() == "QLineEdit")
    return(strsplit(y$text, split = " +"))
  else
  if (y$metaObject()$className() == "QPushButton")
     return(boutons.hash[names(y)])
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
  objets[1] <- NULL 
  mapply(function(x, y) assign(x,  unlist(valeur.widget(y)), envir=altair),
                        objets,  
                        formulaire)
  
}

