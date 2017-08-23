library(readr)
CRC_BASE3 <- read_delim("~/Desktop/SDIS/Bases/CRC-BASE3.csv",
";", escape_double = FALSE, locale = locale(date_names = "fr",
grouping_mark = " ", encoding = "WINDOWS-1252"),
trim_ws = TRUE)

# les exécutions précédentes doivent avoir produit Bulletins.paie[Matricule, Année, Mois, Type.personnel = "SPP" ou "PATS"] à fusionner avec CRC_BASE3
# veiller à ce que la fusion soit propre :"SPP", "PATS" ou "NA"

CRC_BASE3 <- as.data.table(CRC_BASE3)
merge(CRC_BASE3, Bulletins.paie, by = c("Matricule", "Année", "Mois", "Type.personnel"))

# write.csv2(unique(CRC_BASE3$Evenement), "CRC_BASE3_E.csv")
# Catégoriser et classer dans Type_sortie et Type_entrée

CRC_BASE3_E <- read_delim("CRC_BASE3_E.csv",
";", escape_double = FALSE, trim_ws = TRUE)

# Sorties

CRC_BASE_3_1 <- merge(CRC_BASE3, CRC_BASE3_E[, -1])
Base_3_2 <- CRC_BASE_3_1[! is.na(Type_sortie)]
Base_3_3 <- CRC_BASE_3_1[! is.na(Type_entrée)]

flux <- function (type) {
  
  Sorties <- Base_3_2[! is.na(Type.personnel) & Type.personnel == type , .(Cum = uniqueN(Matricule)), by = .(Type_sortie, Année)]
  
  S <- dcast(Sorties, Type_sortie ~ Année, value.var = "Cum")
  
  # Remplacer les NA par des 0
  
  for (x in names(T)) T[is.na(get(x)), (x) := 0]
  
  # Marge colonnes en cumuls
  
  tot <- c("Total", colSums(S[, -1], na.rm = TRUE))
  names(tot)[[1]] <- "Type_sortie"
  T <- rbind(S, t(tot))
  T[[1]] <- c("Autre", "Décès", "Disponibilité-Congé parental", "Démission", "Détachement", "Inaptitude", "Licenciement", "Mutation", "Retraite", "Total")
  names(T)[[1]] <- ( "Année")
  
  # Marge lignes en variations 
  
  T[ , Variation := 0]
  n <- ncol(T)
  for (i in 1:nrow(T)) set(T, i, n, round((T[i, n]/T[i, 2] - 1) * 100, digits = 1))
  
  write.csv2(T, "Flux.sortie." %+% catégorie.personnel +".csv")

  T
}


Sorties.SPP <- flux("SPP")
Sorties.PATS <- flux("PATS")
