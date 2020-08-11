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

source("prologue_rapport.R", encoding = "UTF-8")

for (rep in reps) {
  
  env <<- new.env()
  assign("chemin.cle", rep, envir = env)
  message("************************")
  message("*")
  message(paste("* Analyse du dossier", rep))
  message("*")
  message("************************")

  setwd(chemin.dossier)
  
  invisible(lapply(c("altair.pdf", "altair.odt", "altair.docx"), function(x) file.remove(file.path(chemin.cle.racine, x))))
  invisible(lapply(c("altair.pdf", "altair.odt", "altair.docx"), function(x) file.remove(file.path(chemin.dossier, x))))
  invisible(file.remove(list.files(chemin.modules, "*.Rmd", full.name = TRUE)))
  
  res <- try({
      source("rapport_msword_et_pdf_start.R", encoding = encodage.code.source, echo = TRUE)
  })
  
  if (inherits(res, "try-error")) {
      message("************************")
      message("*")
      message(paste("*", rep, "N'a pas pu être traité"))
      message("*")
      message("************************")
      setwd(currentDir)
  }
  
 rm(list = setdiff(ls(), script_env))
}

rm(list = ls())

