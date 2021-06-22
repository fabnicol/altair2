# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
#
# Ce logiciel est un programme informatique servant à extraire et analyser
# les fichiers de paye produits au format spécifié par l'annexe de la
# convention-cadre de dématérialisation en vigueur à partir de 2008.
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

#'Génération d'un rapport d'analyse pdf, docx et/ou odt ou exécution sans rapport
#'@param type Type du rapport : "latex" pour pdf, "docx" pour MS Word et ODT, "sans" pour seulement créer les bases auxiliaires,
#' et c("pdf", "docx") ou "pdf,docx" ou toute expression combinant "pdf" et "docx" pour obtenir les deux formats de rapport.
#'@export
#'
generer_rapport <- function(type = "latex") {

  "currentDir" %a% getwd()

  # dossier d'exécution principal

  "chemin.dossier" %a% file.path(currentDir, "scripts", "R")

  setwd(chemin.dossier)

  # variables globales

  "generer.rapport" %a% ifelse(type == "sans", FALSE, TRUE)

  source("prologue.R", encoding = "UTF-8")

  script_env <- ls()
  script_env <- c(script_env, "script_env")

  if (type != "sans") library(rmarkdown)

  # Pour chaque répertoire de la cle (dossier R-Altair,export des bases CSV),
  # écraser les rapports et sourcer rapport_pdf_start.R
  # enfin nettoyer l'environnement
  # et revenir au dossier racine

  for (rep in reps) {

    "chemin.cle" %a% rep
    message("************************")
    message("*")
    message(paste("* Analyse du dossier", rep))
    message("*")
    message("************************")

    # variables dépendantes du mode distributif, éventuellement
    # il faut exporter tous ces chemins dans l'environnement global

    "chemin.dossier.bases"   %a% file.path(chemin.cle, "Bases")
    "chemin.dossier.donnees" %a% file.path(chemin.cle, "..", "xhl", ifelse(user == "fab", "", user))
    "chemin.dossier.docs"    %a% file.path(chemin.cle, "Docs")

    if (basename(chemin.cle) == basename(racine)) {
      source("prologue_codes.R", encoding = encodage.code.source)
    } else {
      source(file.path(chemin.cle, "prologue_codes.R"), encoding = encodage.code.source)
    }

    # no-op mais précaution en cas de crash
    setwd(chemin.dossier)

    # nettoyage

    invisible(lapply(c("altair.pdf", "altair.odt", "altair.docx"), function(x) file.remove(file.path(chemin.cle.racine, x))))
    invisible(file.remove(list.files(chemin.dossier, "*.(Rmd|tex|docx|odt|pdf)$", full.name = TRUE)))

    # lancement de la fabrication du rapport

    if (type == "sans") {

      res <- source("altair_start.R", encoding = "UTF-8")
    } else {

      if (grepl("pdf", type)) {

        t <- "pdf"
        if (grepl("docx", type)) t <- c("pdf", "docx")

        } else {

         if (grepl("docx", type)) {

            t <- "docx"

          } else {

            t <- "sans"
          }
       }

      res <- try({rendre(to = t)})
    }

    # test d'erreur

    if (inherits(res, "try-error")) {
        message("************************")
        message("*")
        message(paste("*", rep, "N'a pas pu être traité"))
        message("*")
        message("************************")
    }

    # retour au dossier racine et nettoyage de l'environnement

    setwd(currentDir)

  if (! debug.code)
     rm(list = setdiff(ls(), script_env))

  }

  if (! debug.code)
      rm(list = ls())
}



#' Convertir le fichier tex du rapport en pdf
#' @param infile  le nom de sortie du fichier, par défaut altair.tex
#' @param outfile le nom de sortie du fichier, par défaut altair.pdf
#' @param args Arguments à passer à pandoc comme options de forme
#' @param keep Garder les fichier .tex temporaires à la fin de l'exécution
#' @export

tex2pdf <- function(infile = "altair.tex", outfile = "altair.pdf",
                    args = c("-V", "papersize=A4", 
                    "-M", "lang=fr",
                    "-V", "geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm",
                    "-V", "urlcolor=cyan",
                    "--highlight-style", "tango"),
                    keep = keep_md) {

  system2(get("chemin_pandoc", envir = .GlobalEnv), c(infile, args, "-o", outfile))

  cleanup()

  file.copy(outfile, chemin.cle)

  cle_outfile <- file.path(chemin.cle, outfile)

  if (ouvrir.document && file.exists(cle_outfile)) {
    if (setOSWindows) {

      shell(paste("start acrord32.exe",  cle_outfile))

    } else {
      system(paste("okular", file.path(chemin.cle, outfile)))
    }
  }

  if (! keep) file.remove(infile)

}




cleanup <- function() {

  if (! keep_md) {
    unlink("altair.ansi_pdf", recursive=TRUE)
    unlink("altair.md")
    unlink("altair_files", recursive = TRUE)
    files.remove(list.files("modules.*\\.tex"))
  }
}

ajuster_chemins_odt <- function(files) {

  for (f in files) {
    con <- file(f, open = "r", encoding = encodage.code.source)
    con2 <- file("temp.tex", open = "w", encoding = encodage.code.source)
    V <- readLines(con, encoding = encodage.code.source)
    V <- gsub("\\href{Bases", "\\href{../Bases", V, fixed = TRUE)
    V <- gsub("\\href{Docs", "\\href{../Docs", V, fixed = TRUE)
    writeLines(V, con2)
    close(con)
    close(con2)
    file.remove(f)
    file.rename("temp.tex", f)
  }
}

#' Générer les fichiers docx et odt à partir des fichier intermédiaires .tex générés pour le rapport pdf
#' @param infile Nom du ou des fichier(s) latex, par défaut altair.tex
#' @param outfile Nom du fichier docx de sortie, par défaut altair.docx
#' @param outfile2 Nom du fichier odt de sortie, par défaut altair.odt
#' @param args Arguments à passer à pandoc comme options de forme
#' @note Permet de gagner du temps en recyclant des calculs déjà réalisés.
#' @export

generer_docx_odt <- function(infile = "altair.tex",
                             outfile = "altair.docx",
                             outfile2 = "altair.odt",
                             args = c("-M", "lang=fr", "-V", "papersize=A4",  "-V", "geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm", "-V", "urlcolor=cyan", "--highlight-style", "tango")) {

  pandoc <- get("chemin_pandoc", envir = .GlobalEnv)

  system2(
    pandoc,
    c(infile,
      "+RTS",
      "-K512m",
      "-RTS",
      "--to",
      "docx",
      args,
      "--output",
      outfile))

  ajuster_chemins_odt(infile)

  system2(
    pandoc,
    c(infile,
      "+RTS",
      "-K512m",
      "-RTS",
      "--to",
      "odt",
      args,
      "--output",
      outfile2))

  cleanup()

  file.copy(c(outfile, outfile2), chemin.cle)

  cle_outfile <- file.path(chemin.cle, outfile)
  cle_outfile2 <- file.path(chemin.cle, outfile2)

  if (ouvrir.document) {
    if (setOSWindows) {

      if (file.exists(cle_outfile)) shell(paste("start winword.exe",  cle_outfile))

    } else {

      if (file.exists(cle_outfile2)) system(paste("lowriter",  cle_outfile2))
    }
  }
}
