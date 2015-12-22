siren         <- "219740110"
nom.commune   <- ""
code.dpt      <- "000"
population    <- 0
surclassement <- FALSE
chef.lieu.dpt    <- FALSE
chef.lieu.arrdt  <- FALSE
chef.lieu.canton <- FALSE
station.touristique <- FALSE

# L'interface graphique devra préciser ces 6 variables exogènes à la base de paye.

table.communes <- data.table::fread("comsimp2015.txt", stringsAsFactors = FALSE, colClasses = "character")
populations.légales.communes.2014 <- data.table::fread("popleg2011.csv", 
                                                       stringsAsFactors = FALSE, 
                                                       colClasses = c("character", "character", "numeric", "numeric"),
                                                       header = TRUE,
                                                       sep = ";")

table.communes <- merge(populations.légales.communes.2014, table.communes, by=c("DEP", "COM"))

extraire.avec.siren <- function () {
  
    Siren    <- unlist(strsplit(siren, NULL))
    code.dpt <- paste0(Siren[3], Siren[4])
    if (Siren[5] != '0') 
      code.dpt <- paste0(code.dpt, Siren[5])
    
    code.commune <- paste0(Siren[6], Siren[7], Siren[8])
    
    commune <- table.communes[DEP == code.dpt & COM == code.commune, .(CHEFLIEU, NCC, POPMUN, POPTOT)]
    
    if (nrow(commune) != 1) {
      
      cat("Impossible de trouver le nom de commune à partir du SIREN\n")
      return(FALSE)
      
    } else {
      
    population        <- commune$POPMUN
    population.totale <- commune$POPTOT
    nom.commune       <- commune$NCC
    cheflieu.insee    <- commune$CHEFLIEU

    chef.lieu.dpt <- (cheflieu.insee == "3" | cheflieu.insee == "4") 
    chef.lieu.arrdt <- (cheflieu.insee == "2") 
    chef.lieu.canton <- (cheflieu.insee == "1") 
  
    cat("Nom INSEE de la commune :", nom.commune, "\n")
    cat("Département :", code.dpt, "\n")
    cat("Population municipale :",   population, "\n")
    cat("Population totale :", population.totale, "\n")
    
    if (chef.lieu.dpt == TRUE) cat("Chef-lieu de département\n")
    if (chef.lieu.arrdt == TRUE) cat("Chef-lieu d'arrondissement\n")
    if (chef.lieu.canton == TRUE) cat("Chef-lieu de canton\n")
      
    }
    
    return(TRUE)
  
}

extraire.avec.nom.commune <- function () {

    commune <- table.communes[NCC == nom.commune, .(DEP, COM, CHEFLIEU, NCC, POPMUN, POPTOT)]
    
    if (nrow(commune) > 1 && code.dpt != "000")
      commune <- commune[DEP == code.dpt]
    
    if (nrow(commune) != 1) {
      
      cat("Impossible de trouver la population à partir du nom de commune\n")
      return(FALSE)
      
    } else {
      
      population        <- commune$POPMUN
      population.totale <- commune$POPTOT
      cheflieu.insee    <- commune$CHEFLIEU
      
      code.dpt <- commune$DEP
      code.commune <- commune$COM
      
      code.dpt.siren <- if (code.dpt == "2A" || code.dpt == "2B")  "20" else code.dpt
      
      if (nchar(code.dpt.siren) == 2) code.dpt.siren <- paste0(code.dpt.siren, "0")
      
      # on ne reconstitue pas la clé de contrôle.
      # attention les siren de quelques communes seront faux (communes résultant de divisions)
      
      siren <- paste0("21", code.dpt.siren, code.commune, "X")
      
      chef.lieu.dpt <- (cheflieu.insee == "3" | cheflieu.insee == "4") 
      chef.lieu.arrdt <- (cheflieu.insee == "2") 
      chef.lieu.canton <- (cheflieu.insee == "1") 
      
      cat("Nom INSEE de la commune :", nom.commune, "\n")
      cat("Département :", code.dpt, "\n")
      cat("Population municipale :",   population, "\n")
      cat("Population totale :", population.totale, "\n")
      cat("Siren (voir nota) :", siren, "\n")
      
      if (chef.lieu.dpt == TRUE) cat("Chef-lieu de département\n")
      if (chef.lieu.arrdt == TRUE) cat("Chef-lieu d'arrondissement\n")
      if (chef.lieu.canton == TRUE) cat("Chef-lieu de canton\n")
        
    }
    
    return(TRUE)
  
}
  
extraire.donnees.insee <- function() {
  
  if (exists("siren") && siren != "000000000") {    
    
      result <- extraire.avec.siren()
    
  } else {
    
      if (exists("nom.commune") && nom.commune != "") {
        
          result <- extraire.avec.nom.commune()
          
      } else {
        
        cat("Impossible d'extraire les données des collectivités avec le Siren ou avec le nom de commune\n")
        result <- FALSE
      }
  }
}

res <- try(extraire.donnees.insee())

if (inherits(res, 'try-error'))
  cat("Problème de lecture des bases de données INSEE sur les collectivités (populations et codes)")



