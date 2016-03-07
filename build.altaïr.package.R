devtools::document("altaïr")
system("R CMD build altaïr")
system("R CMD INSTALL --html altair_0.1.tar.gz")
# méthodologie : utiliser des encodages latin1 partout et préciser Encoding: latin1 dans le DESCRIPTION
# Ensuite corriger les <e9> des codes en Usage des fichier Rd
# puis builder et installer
# puis convertir en utf8 le fichier d'index html
