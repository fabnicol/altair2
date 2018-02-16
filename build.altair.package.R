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
source("Tests/Exemple/rtoolspath.R")

# Utiliser Encoding: latin1 dans le fichier DESCRIPTION.
# Utiliser des fichiers source latin1.
# Se limiter à é et è comme caractères accentués dans les fonctions.
# utiliser le sed de GNUwin32 pas celui de msys2

call_rtools()
devtools::document("altair")

fl <- list.files("altair/man", full.names = TRUE)

to_utf8 <- function(fl) lapply(fl, function(x) { 
  y <- paste0(x, ".bak")
  writeLines(enc2utf8(readLines(x, encoding="latin1")), y, useBytes = TRUE) 
  file.remove(x)
  file.rename(from = y, to = x)
  })

#to_utf8(fl)

system(paste('Outils/sed.exe -i -e "s/<e9>/é/g" -e "s/<e8>/è/g"' , paste(fl, collapse = " ")))

# lapply(fl, function(x) { 
#   y <- paste0(x, ".bak")
#   writeLines(enc2native(readLines(x, encoding="UTF-8")), y, useBytes = TRUE) 
#   file.remove(x)
#   file.rename(from = y, to = x)
# })

system("R CMD build altair")
system("R CMD INSTALL --html altair_0.1.tar.gz")
to_utf8('R/library/altair/html/00Index.html')

restore_path()
