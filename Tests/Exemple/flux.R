
library(readr)
library(data.table)


#################  Sorties ###################


CRC_BASE3 <- read_delim("~/Desktop/SDIS/Bases/CRC-BASE3.csv",
                        ";", escape_double = FALSE, locale = locale(date_names = "fr",
                        grouping_mark = " ", encoding = "UTF-8"),
                        trim_ws = TRUE)

CRC_ES <- read_delim("~/Desktop/SDIS/Bases/ES.csv",
                        ";", escape_double = FALSE, locale = locale(date_names = "fr",
                                                                    grouping_mark = " ", encoding = "UTF-8"),
                        trim_ws = TRUE)

CRC_ES <- data.table(CRC_ES)
CRC_ES[ , Entrée := substr(`Date entrée`, 1, 4)]
CRC_ES[ , Sortie := substr(`Date sortie`, 1, 4)]

flux.total <- function(type = "total", Base = CRC_ES) {

if (type != "total")   Base <- Base[Categorie == type]

Entrées <- Base[Entrée %in% période, .(Nombre = uniqueN(Matricule)), keyby = Entrée]
Sorties <- Base[Sortie %in% période, .(Nombre = uniqueN(Matricule)), keyby = Sortie]
Entrées_Sorties <- Base[Sortie %in% période & Entrée == Sortie,
                             .(Nombre = uniqueN(Matricule[as.Date(`Date entrée`) < as.Date(`Date sortie`)])), keyby = Sortie]

Sorties_Entrées <- Base[Sortie %in% période & Entrée == Sortie,
                          .(Nombre = uniqueN(Matricule[as.Date(`Date entrée`) >= as.Date(`Date sortie`)])), keyby = Sortie]

data.table(période, Entrées = Entrées$Nombre, Sorties = Sorties$Nombre,
                                  Entrées.Sorties = Entrées_Sorties$Nombre,
                                  Entrées.nettes = Entrées$Nombre - Entrées_Sorties$Nombre,
                                  Sorties.nettes = Sorties$Nombre - Sorties_Entrées$Nombre)[ , Flux.net := Entrées.nettes - Sorties.nettes]
}

Flux.total.PATS <- flux.total(type = "PATS")
Flux.total.SPP  <- flux.total(type = "Professionnel")
Flux.total      <- flux.total(type = "Volontaire")


CRC_BASE3$Matricule <- as.character(CRC_BASE3$Matricule)
CRC_BASE3$Mois <- as.integer(CRC_BASE3$Mois)

Bulletins.paie[Grade %chin% grades.pats, Type.personnel := "PATS"]
Bulletins.paie[Grade %chin% grades.spp, Type.personnel  :=  "SPP"]

# les exécutions précédentes doivent avoir produit Bulletins.paie[Matricule, Année, Mois, Type.personnel = "SPP" ou "PATS"] à fusionner avec CRC_BASE3
# veiller à ce que la fusion soit propre :"SPP", "PATS" ou "NA"

CRC_BASE3 <- as.data.table(CRC_BASE3)
BP <- Bulletins.paie[, .(Matricule = substr(Matricule, 1, 4), Année, Mois, Type.personnel, quotité.moyenne)]
CRC_BASE3 <- merge(CRC_BASE3, BP, by = c("Matricule", "Année", "Mois"), all = FALSE)

# write.csv2(unique(CRC_BASE3$Evenement), "CRC_BASE3_E.csv")
# Catégoriser et classer dans Type_sortie et Type_entrée

CRC_BASE3_E <- read_delim("CRC_BASE3_E.csv",
";", escape_double = FALSE, trim_ws = TRUE)

# Sorties

T <- merge(CRC_BASE3, CRC_BASE3_E[, -1], by = "Evenement", all = FALSE)
Base_sortie <- T[! is.na(Type_sortie)]
Base_entrée <- T[! is.na(Type_entrée)]
rm(T)

flux <- function (type = "Total", io = "out") {
  
  Base <- if (io == "out") Base_sortie else Base_entrée
  
  if (type == "Total") {
    Sorties <- Base[ , .(Cum = uniqueN(Matricule)), by = .(Type_sortie, Année)]
  } else {
    Sorties <- Base[ ! is.na(Type.personnel) & Type.personnel == type , .(Cum = uniqueN(Matricule)), by = .(Type_sortie, Année)]  
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
  
  for (i in 2:nrow(T)) set(T,
                           i,
                           n + 1,
                           ifelse(T[i, 2] != 0 & !is.na(T[i, n, with = FALSE]),
                                           as.numeric(T[i, n, with = FALSE]) - as.numeric(T[i, 2, with = FALSE]), NA))
  
  write.csv2(T, "Flux." %+% io %+% ".detail." %+% type %+% ".csv")

  T
}

Sorties.SPP  <- flux("SPP")
Sorties.PATS <- flux("PATS")
Sorties.total <- flux("Total")



#############  Mobilité ##################


ES <- CRC_ES[Entrée < 2007 & Sortie > 2016 & Année == 2016 & Mois == 12][ , Matricule := as.character(Matricule)]

ES <- merge(ES, Bulletins.paie[Année == 2016 & Mois == 12], by= "Matricule")

série <- function(cat) {
  
  filtre <- if (cat == "Professionnel") {
               function(Grade, Catégorie, Type.personnel) grepl(pattern = e.santé.offsup, Grade, perl = TRUE, ignore.case = TRUE)  == TRUE
            } else {
               function(Grade, Catégorie, Type.personnel) Catégorie == "A" & Type.personnel == cat
            }
  
  sapply(c(10, 15, 20), function(x) ES[Categorie == cat 
                                                & Entrée < 2017 - x 
                                                & (is.na(Sortie) | Sortie > 2016)
                                                & filtre(Grade, Catégorie, Type.personnel), 
                                                       uniqueN(Matricule)])

}

ES_SPP <- rbind(c("Officiers SPP", série(cat = "Professionnel")),
                c("PATS cat. A", série(cat = "PATS")))

kable(data.table(ES_SPP), 
      align = "r",
      col.names = c("Ancienneté", "Plus de 10 années", "Plus de 15 années", "Plus de 20 années"))


