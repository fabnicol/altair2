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
