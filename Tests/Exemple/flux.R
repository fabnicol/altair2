library(readr)
library(data.table)

CRC_BASE3 <- read_delim("~/Desktop/SDIS/Bases/CRC-BASE3.csv",
";", escape_double = FALSE, locale = locale(date_names = "fr",
grouping_mark = " ", encoding = "WINDOWS-1252"),
trim_ws = TRUE)
CRC_BASE3$Matricule <- as.character(CRC_BASE3$Matricule)
CRC_BASE3$Mois <- as.integer(CRC_BASE3$Mois)

Bulletins.paie[Grade %chin% grades.pats, Type.personnel := "PATS"]
Bulletins.paie[Grade %chin% grades.spp, Type.personnel  :=  "SPP"]

# les exécutions précédentes doivent avoir produit Bulletins.paie[Matricule, Année, Mois, Type.personnel = "SPP" ou "PATS"] à fusionner avec CRC_BASE3
# veiller à ce que la fusion soit propre :"SPP", "PATS" ou "NA"

CRC_BASE3 <- as.data.table(CRC_BASE3)
CRC_BASE3 <- merge(CRC_BASE3, Bulletins.paie[, .(Matricule, Année, Mois, Type.personnel)], by = c("Matricule", "Année", "Mois"), all = TRUE)

# write.csv2(unique(CRC_BASE3$Evenement), "CRC_BASE3_E.csv")
# Catégoriser et classer dans Type_sortie et Type_entrée

CRC_BASE3_E <- read_delim("CRC_BASE3_E.csv",
";", escape_double = FALSE, trim_ws = TRUE)

# Sorties

CRC_BASE_3_1 <- merge(CRC_BASE3, CRC_BASE3_E[, -1], by = "Evenement", all = TRUE)
Base_3_2 <- CRC_BASE_3_1[! is.na(Type_sortie)]
Base_3_3 <- CRC_BASE_3_1[! is.na(Type_entrée)]

flux <- function (type = "Total") {
  
  if (type == "Total") {
    Sorties <- Base_3_2[ , .(Cum = uniqueN(Matricule)), by = .(Type_sortie, Année)]
  } else {
    Sorties <- Base_3_2[! is.na(Type.personnel) & Type.personnel == type , .(Cum = uniqueN(Matricule)), by = .(Type_sortie, Année)]  
  }
  
  
  S <- dcast(Sorties, Type_sortie ~ Année, value.var = "Cum")
  
  # Remplacer les NA par des 0
  
  for (x in names(S)) S[is.na(get(x)), (x) := 0]
  
  # Marge colonnes en cumuls
  
  tot <- c("Total", colSums(S[, -1], na.rm = TRUE))
  names(tot)[[1]] <- "Type_sortie"
  T <- rbind(S, t(tot))
  
  names(T)[[1]] <- ( "Année")
  
  # Marge lignes en variations 
  n <- ncol(T) 
  T[ , Variation := 0]
  
  for (i in 2:nrow(T)) set(T, i, n + 1, ifelse(T[i, 2] != 0 & !is.na(T[i, n, with = FALSE]),
                                           round((as.numeric(T[i, n, with = FALSE])/(as.numeric(T[i, 2]) - 1)) * 100, digits = 1), NA))
  
  write.csv2(T, "Flux.sortie." %+% type %+% ".csv")

  T
}

Sorties.SPP  <- flux("SPP")
Sorties.PATS <- flux("PATS")
Sorties.total <- flux("Total")
