require(qtbase)

## Caractéristiques globales ##  

wizard <- Qt$QWizard()
wizard$setWindowTitle("Analyse des Lignes de Traitement, Attributions Indemnitaires et autres Rémunérations (ALTAIR)")
wizard$setWizardStyle(Qt$QWizard$MacStyle)
wizard$setPixmap(Qt$QWizard$BackgroundPixmap, Qt$QPixmap("/home/fab/altair.png"))
wizard$setGeometry(600,400,650,300)

wizard$setButtonText(Qt$QWizard$BackButton, "Page précédente")
wizard$setButtonText(Qt$QWizard$NextButton, "Page suivante")
wizard$setButtonText(Qt$QWizard$FinishButton, "Terminer")
wizard$setButtonText(Qt$QWizard$CancelButton, "Annuler")

annee.courante <- as.numeric(substr(Sys.Date(), 0, 4))

## Première page ##

info.étiquettes <- c(
"champ de detection 1",
"champ de detection 2",
"champ du matricule",
"champ du montant",
"libellé traitement indiciaire",
"libellé prime/contractuel",
"libellé vacations",
"libellé autres codes")


valeurs.par.défaut <- c(
"Matricule",
"Code",
"Matricule",
"Montant",
"TRAITEMENT",
"INDEMNITAIRE.OU.CONTRACTUEL",
"VACATIONS",
"AUTRES")


commentaires.champs.noms <- c(
  "libellé de champ de la base qui sera détecté en priorité",
  "libellé de champ de la base qui sera détecté sinon",
  "libellé exact du champ du matricule" ,
  "libellé exact du champ du montant",
  "libellé de classification d'un code de type Traitement",
  "libellé de classification d'un code de type Indemnités ou Rémunération contractuelle",
  "libellé de classification d'un code de type Vacations",
  "libellé de classification d'un code de type Autres rémunérations")


objets <- c(
  "champ.detection.1",
  "champ.detection.2",
  "etiquette.matricule",
  "etiquette.montant",
  "traitement",
  "indemnités",
  "vacations",
  "autres"
)


info.form.layout <- Qt$QFormLayout()

chemin.dossier.étiquette <- "chemin du dossier de travail"

info.form.layout$addRow(chemin.dossier.étiquette, dirButton <- Qt$QPushButton("..."))

créer.lignes <- function(x, y, z, t)  {
                                   line.edit <- Qt$QLineEdit()
                                   line.edit$setToolTip(z)
                                   line.edit$setText(t)
                                   line.edit$setAlignment(Qt$Qt$AlignRight)
                                   line.edit$setMinimumWidth(150)
                                   info.form.layout$addRow(x, line.edit)
                                   return(line.edit)
                                }

formulaire <- mapply(créer.lignes, info.étiquettes, objets, commentaires.champs.noms, valeurs.par.défaut)

sélectionner.répertoire <- function(x) 
  {
    qconnect(x, "clicked",  function(x) {filename <<- Qt$QFileDialog$getExistingDirectory(NULL,
                                                                             "Sélectionner un répertoire", getwd())
             if(!is.null(filename))  x$setText(filename) }
    )
  }

sélectionner.répertoire(dirButton)

info_page <- Qt$QWizardPage(wizard)
info_page$setTitle("Champs à définir")
info_page$setLayout(info.form.layout)
wizard$addPage(info_page)

## Deuxième page ##

periode_page <- Qt$QWizardPage(wizard)
periode_page$setTitle("Dates et durées")

info.étiquettes.2 <- c(
"nombre de jours de travail",
"format de date")

valeurs.par.défaut.2 <- c(
"100",
"%d-%m-%y")

objets.2 <- c("seuil.troncature",
                 "date.format")

commentaires.champs.noms.2 <- c(
  "Nombre de jours minimum qui devront être enregistrés en base de paie, au cours de la première et la dernière année de travail\npour pouvoir être pris en compte dans les statistiques relatives à ces deux années.",
"Utiliser %d pour jour,\n %m pour mois, %y pour année. La convention française est la valeur par défaut.")  

info.form.layout <- Qt$QFormLayout()

formulaire.2 <-  mapply(créer.lignes, info.étiquettes.2, objets.2, commentaires.champs.noms.2, valeurs.par.défaut.2)

layout <- Qt$QFormLayout()

période <- (annee.courante-10):annee.courante

début.période.sous.revue <- Qt$QComboBox()
début.période.sous.revue$addItems(période)
début.période.sous.revue$setCurrentIndex(5)
début.période.sous.revue$setToolTip("Donner le premier exercice sous revue")

fin.période.sous.revue <- Qt$QComboBox()
fin.période.sous.revue$addItems(période)
fin.période.sous.revue$setCurrentIndex(9)
fin.période.sous.revue$setToolTip("Donner le dernier exercice sous revue")

layout$addRow("Période sous revue : premier exercice", début.période.sous.revue)
layout$addRow("Période sous revue : dernier exercice", fin.période.sous.revue)

objets.2 <- c(objets.2, "début.période.sous.revue", "fin.période.sous.revue")

formulaire.2 <- c(formulaire.2, début.période.sous.revue, fin.période.sous.revue)

periode_layout <- Qt$QGridLayout()
periode_layout$setColumnMinimumWidth(0, 60)
periode_layout$setColumnMinimumWidth(2, 60)
periode_layout$setRowMinimumHeight(0, 40)
periode_layout$addLayout(info.form.layout,1, 1, Qt$Qt$AlignRight)
periode_layout$setRowMinimumHeight(2, 40)
periode_layout$addLayout(layout, 3, 1, Qt$Qt$AlignRight)
periode_page$setLayout(periode_layout)

wizard$addPage(periode_page)

## Troisième page  ##

base_page <- Qt$QWizardPage(wizard)
base_page$setTitle("Importation des données")

base_page$setLayout(base_layout <- Qt$QGridLayout())

base_layout$addWidget(parseXml <- Qt$QCheckBox("Importer directement les fichiers xhl"), 0,1)

base_layout$setRowMinimumHeight(1, 40)

base<-data.frame()

base.étiquettes <- c("Lignes de paie", "Bulletins de paie", "Codes", "Avantages en nature", "Catégories")

boutons.bases <- lapply(base.étiquettes,
                         function(i) 
                          Qt$QPushButton("Sélectionner le fichier (.csv)"))

names(boutons.bases) <- c("ldp", "bdp", "codes", "avantages", "catégories")

lapply(boutons.bases, function(x)
                        qconnect(x, "clicked", function() {
                      
                          name_filter <- "CSV  (*.csv)"
                          filename <<- Qt$QFileDialog$getOpenFileName(base_page, "Sélectionner un fichier CSV...", getwd(), name_filter)
                          if(!is.null(filename))  x$setText(basename(filename))
                        }))

base.form.layout <- Qt$QFormLayout()
mapply(base.form.layout$addRow, base.étiquettes, boutons.bases)

base.form.gbox <- Qt$QGroupBox("Bases csv")

indicesOk <- Qt$QCheckBox("Sélectionner tous les fichiers\nde même nom racine")

gbox.layout <- Qt$QGridLayout()
gbox.layout$addWidget(indicesOk, 0, 0, Qt$Qt$AlignLeft)
gbox.layout$setRowMinimumHeight(1, 20)

gbox.layout$addLayout(base.form.layout, 2, 0)
base.form.gbox$setLayout(gbox.layout)

base_layout$addWidget(base.form.gbox, 2, 1)

indicesOk$setChecked(TRUE);

button_box <-  Qt$QDialogButtonBox()
button_box$addButton("Annuler", Qt$QDialogButtonBox$RejectRole)
button_box$addButton("Importer les bases", Qt$QDialogButtonBox$AcceptRole)

base_layout$setRowMinimumHeight(3, 20)
base_layout$addWidget(button_box, 4, 1, Qt$Qt$AlignRight)

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

qconnect(parseXml, "clicked", function() { base.form.gbox$setEnabled(!parseXml$checked)})

objets.3 <- c("ldp0",
              "bdp",
              "code.prime",
              "matricule.avantage",
              "matricule.categorie")

## Quatrième page  ##

actions_page <- Qt$QWizardPage(wizard)
actions_page$setTitle("Générer")

actions_layout <- Qt$QGridLayout()

actions_layout$setRowMinimumHeight(0,30)

info.out.layout <- Qt$QFormLayout()

info.etiquettes.4 <- c(
"Générer les bases de codes et libellés",
"Générer l'analyse des distributions de rémunération",
"Générer l'analyse des variations de rémunération",
"Générer l'analyse des incompatibilités",
"Générer les bases .csv des résultats statistiques",
"Exporter et fusionner les bases bases xhl au format csv")

boutons <- c(
  générerCodes  <-  Qt$QCheckBox(),
  générerDist   <-  Qt$QCheckBox(),
  générerVar    <-  Qt$QCheckBox(),
  générerIncomp <-  Qt$QCheckBox(),
  générerStats  <-  Qt$QCheckBox(),
  exporterBases <-  Qt$QCheckBox())
  
mapply(function(x,y)  { info.out.layout$addRow(x, y) ;  y$setChecked(TRUE) }, info.etiquettes.4, boutons)

chemin.dossier.bases <- "chemin du dossier des bases"
info.out.layout$addRow(chemin.dossier.bases, dirBasesButton <- Qt$QPushButton("..."))
chemin.dossier.stats <- "chemin du dossier des statistiques"
info.out.layout$addRow(chemin.dossier.stats, dirStatsButton <- Qt$QPushButton("..."))

sélectionner.répertoire(dirBasesButton)
sélectionner.répertoire(dirStatsButton)

actions_layout$setRowMinimumHeight(6,30)

actions_layout$addLayout(info.out.layout, 7,1)

actions_page$setLayout(actions_layout)

wizard$addPage(actions_page)

objets4 <-  c("chemin.dossier", "dossier.bases", "dossier.statistiques")

############################## Intéraction avec l'environnement et le source  #############################################


wizard$setFocus()
wizard$raise()

response<-wizard$exec()

if(response)
{
  # il faut actualiser les variables (objets) avec la saisie dynamique dans l'assistant  (formulaires, boutions...)
  
  mapply(function(x, y) assign(x, y$text, envir = .GlobalEnv), 
                        c(objets, objets.2, objets.3),
                        c(formulaire, formulaire.2, boutons.bases))
  
  message(matricule.categorie)
}

