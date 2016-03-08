source("Tests/Exemple/rtoolspath.R")

# Utiliser Encoding: latin1 dans le fichier DESCRIPTION.
# Utiliser des fichiers source latin1.
# Se limiter à é et è comme caractères accentués dans les fonctions.

call_rtools()
devtools::document("altaïr")

fl <- list.files("altaïr/man", full.names = TRUE)

lapply(fl, function(x) { 
  y <- paste0(x, ".bak")
  writeLines(enc2utf8(readLines(x, encoding="latin1")), y, useBytes = TRUE) 
  file.rename(from = y, to = x)
  })

system("sed -i -e 's/<e9>/é/g' -e 's/<e8>/è/g' altaïr/man/*.Rd")

lapply(fl, function(x) { 
  y <- paste0(x, ".bak")
  writeLines(enc2native(readLines(x, encoding="UTF-8")), y, useBytes = TRUE) 
  file.rename(from = y, to = x)
})

system("R CMD build altaïr")
system("R CMD INSTALL --html altair_0.1.tar.gz")
system("sed -i 's/charset=utf-8/charset=latin1/' R/library/altair/html/00Index.html")

restore_path()
