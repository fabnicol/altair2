
##---------------------------------------------------------------------------------------------------------------------
#  Chemins
##

# Cette section pourra être modifiée en entrée dans d'autres contextes
# Matricule, Codes, Avantages en nature

chemin.dossier <- "C:/Documents and Settings/fnicol/Bureau/Informatique/VLB"

matricule.categorie <- "LISTES DES PERSONNES REMUNEREES EN 2012 PAR CATEGORIE ET STATUT.csv"     
code.prime          <- "LISTE DES RUBRIQUES DE TRAITEMENT UTILISEES EN 2012.csv"
matricule.avantage  <- "LISTE DES AGENTS AYANT BENEFICIE D'AVANTAGE EN NATURE EN 2012.csv"
code.traitement <- 1010

# Lignes de paie
# On peut lire jusqu'à 10 fichiers csv qui seront générés au format
#  "chemin dossier + paies-Bulletins de paye-j.csv"

ldp <- paste0("paies-Lignes de paye-",1:10,".csv")

# Bulletins de paie

bdp <- "paies-Bulletins de paye-1.csv"