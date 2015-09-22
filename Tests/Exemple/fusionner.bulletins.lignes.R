# suppression des colonnes Nom Prénom redondantes

message("Nettoyage des bases.")

sélectionner.clé("Bulletins.paie", "Lignes.paie")

# Technique : les espaces de noms sont pollués par la sélection des clés, il faut les nettoyer

# unname(Bulletins.paie$Nom) devrait marcher mais cause une génération de tableau sous RMarkdown, probablement un bug.
# utiliser attr à la place.

attr(Bulletins.paie$Nom, "names")    <- NULL
attr(Bulletins.paie$Prénom, "names") <- NULL
attr(Lignes.paie$Nom, "names")       <- NULL
attr(Lignes.paie$Prénom, "names")    <- NULL

# Extraction de vecteurs représentant les codes de paiement par type de code (indemnitaire, traitement, vacations...)

if (exists("Codes.paiement"))
{
  #W <- rep(0, nrow(Codes.paiement))
  Map(
    function(Data, type) {
      Tab <- unique(Codes.paiement[Codes.paiement$Type.rémunération == type, c(étiquette.code, "Coefficient")])
      
      if (anyDuplicated(Tab[1]))
        stop("Incohérence d'un code utilisé à la fois comme paiement et retenue ou cotisation.")
      
      W        <- Codes.paiement$Coefficient*(Codes.paiement$Type.rémunération == type)
      names(W) <- Codes.paiement[["Code"]]
      assign(Data, W, envir = .GlobalEnv)
    },
    
    c("Codes.paiement.indemnitaire",
      "Codes.paiement.principal.contractuel",
      "Codes.paiement.traitement",
      "Codes.paiement.élu",
      "Codes.paiement.vacations",
      "Codes.paiement.autres"),
    
    c(modalité.indemnitaire,
      modalité.principal.contractuel,
      modalité.traitement,
      modalité.élu,
      modalité.vacations,
      modalité.autres))
  
  
  message("Extraction des codes par type de code.")
  
} else
  stop("Charger le fichier de codes de paiement.")

# Pour assurer une fusion correcte des bulletins et lignes de paie, il importe que les colonnes communes aux deux fichiers soient
# exactement celles utilisées pour la clé d'appariement d'une part, et le tri sous chaque clé d'autre part, autrement dit :
# la clé (Matricule ou (Nom, Prénom) selon le cas) + Année + Mois

if (! setequal(intersect(names(Lignes.paie), names(Bulletins.paie)), union(c("Mois", "Année"), clé.fusion)))
{
  if (fusionner.nom.prénom) {
    
    stop("L'appariement ne peut se faire par les clés Nom, Prénom et Mois")
    
  } else {
    
    stop("L'appariement ne peut se faire par les clés Matricule et Mois")
  }
  
} else {
  
  message("Pas de redondance des colonnes des bulletins et lignes de paie : la fusion peut être réalisée.")
}

# Alternative en cas de difficulté :
#
# Paie <- do.call(rbind, lapply(période,
#                                                     function(x)
#                                                        merge(Bulletins.paie[Bulletins.paie$Année == x, ],
#                                                              Lignes.paie[Lignes.paie$Année == x, ],
#                                                              by=c(étiquette.matricule, "Mois"))))

# Lorsque les bases sont déjà chargées, on peut désactiver le rechargement par charger.bases <- FALSE

if (charger.bases)
{
 
  
  # Fusion non parallélisée
  
  # gain de 24s par l'utilisation de data.table::merge
  
  if (table.rapide)   message("Mode table rapide.") else message("Mode table standard.")
  
  # Inutile de paralléliser en mode table rapide pour les dimensions de tables courantes (à tester pour les très grosses communes)
  
  if (table.rapide || durée.sous.revue < 4)   paralléliser <- FALSE
  
  # On réserve la parallélisation à des durées de période sous revue > 4 ans
  
  
  
  if (! paralléliser) {
    
    # la fonction sera automatiquement déterminée par le type du premier argument (data.table ou data.frame)
    
    message("Mode parallèle non activé.")
    
    Paie <- merge(Bulletins.paie,
                                        Lignes.paie,
                                        by = c(clé.fusion, "Année", "Mois"))
    
    if (!is.null(Paie))
      message(paste("Fusion réalisée")) else stop("Echec de fusion" )
    
    
  } else {
    
    # Fusion parallélisée : gain de plus de moitié sur 4 coeurs (25,5s --> 9,7s) soit environ 16s de gain sous linux [RAG]
    #                       sous windows le merge standard est plus rapide (18s) mais la parallélisation est moins performante,
    #                       le gain est d'environ 4s à 14s, soit 5s de plus que sous linux parallèle.
    # Le merge classique est toutefois loin des performancs de data.table::merge
    
    message("Mode parallèle activé.")
    
    
    cut <- round(durée.sous.revue/4)
    if (cut == 0) cut = 1
    
    library(parallel)
    
    cores   <- detectCores()
    
    L <- lapply(list(Bulletins.paie, Lignes.paie),
                function(X) {
                  lapply(0:3,
                         function(j) {
                           if (durée.sous.revue > j)
                             X[X$Année >= j * cut +  début.période.sous.revue
                               & X$Année < (j + 1) * cut +  début.période.sous.revue, ]})})
    
    L <- lapply(L, function(x) Filter(Negate(is.null), x))
    
    f <- function(x, y, z)  {
      tab <- merge(x, y, by = c(clé.fusion, "Année", "Mois"), sort=FALSE)
      # Le sort du merge classique n'est pas fiable sous parallélisation
      
      if (clé.fusion[1] != "Matricule") {
        tab <- tab[order(tab$Nom, tab$Prénom, tab$Année, tab$Mois), ]
      } else {
        tab <- tab[order(tab$Matricule, tab$Année, tab$Mois), ]
      }
      
      tab
    }
    
    if (setOSWindows) {
      
      cluster <- makePSOCKcluster(cores)
      clusterExport(cluster, c("clé.fusion"))
      
      L <- clusterMap(cluster, f,  L[[1]],  # Bulletins de paie coupés en 4, éventuellement nul
                      L[[2]])  # Lignes de paie coupés en 4, éventuellement nul
      
      stopCluster(cluster)
      rm(cluster)
      
    }  else  {
      
      L <- mcMap(f,   L[[1]],  # Bulletins de paie coupés en 4, éventuellement nul
                 L[[2]],  # Lignes de paie coupés en 4, éventuellement nul
                 mc.cores = cores)
      
    }
    
    rm(temp)
    
    if (table.rapide) {
      Paie <- data.table::rbindlist(L)
    } else {
      Paie <- do.call(rbind, L)
      
    }
    
    if (paralléliser || ! table.rapide)
      with(Paie,
           Paie <- Paie[order(Matricule, Année, Mois), ])
    
    if (!is.null(L[[1]]) & !is.null(L[[2]]) & !is.null(Paie))
      message(paste("Fusion réalisée")) else stop("Echec de fusion" )
    
  }
  
  
  
  
  if (! exists("Codes.paiement.indemnitaire"))  stop("Pas de fichier des Types de codes [INDEMNITAIRE]")
  if (! exists("Codes.paiement.principal.contractuel"))  stop("Pas de fichier des Types de codes [PRINCIPAL.CONTRACTUEL]")
  if (! exists("Codes.paiement.vacations"))     stop("Pas de fichier des Types de codes [VACATAIRE]")
  if (! exists("Codes.paiement.traitement"))    stop("Pas de fichier des Types de codes [TRAITEMENT]")
  if (! exists("Codes.paiement.élu"))           stop("Pas de fichier des Types de codes [ELU]")
  if (! exists("Codes.paiement.autres"))        stop("Pas de fichier des Types de codes [AUTRES]")
  
  if (extraire.population) {
    
    Paie <- Paie[grepl(expression.rég.population, Paie$Service, ignore.case=TRUE), ]
    Bulletins.paie             <- Bulletins.paie[grepl(expression.rég.population, Bulletins.paie$Service, ignore.case=TRUE), ]
    
    if (!is.null(Paie) & !is.null(Bulletins.paie)) message("Extraction réalisée")
    
  }
  
  # Optimisation : il faut impérativement limiter le recours aux hash table lookups pour les gros fichiers.
  # L'optimisation ci-dessous repose sur l'utilisation des informations déjà calculées sur les colonnes précédentes pour éviter
  # de computer Codes....[Code] autant que possible. Gain de temps par rapport à une consultation systématique : x100 à x200
  # data.table ne gère pas les expressions du type if (...ligne précédente..) mais admet les calculs logiques booléens.
  
  if (table.rapide == TRUE) {
    
    Paie[ ,   montant.traitement.indiciaire
                               :=  Codes.paiement.traitement[Code]*Montant]
    
    Paie[,    montant.primes
                               :=  (montant.traitement.indiciaire == 0)*
                                 Montant * Codes.paiement.indemnitaire[Code]]
    
    Paie[ ,   montant.rémunération.principale.contractuel
                               := (montant.traitement.indiciaire == 0
                                   & montant.primes == 0)
                               * Montant * Codes.paiement.principal.contractuel[Code]]
    
    Paie[ ,   montant.rémunération.vacataire
                               :=  (montant.traitement.indiciaire == 0
                                    & montant.primes == 0
                                    & montant.rémunération.principale.contractuel == 0)
                               * Montant * Codes.paiement.vacations[Code]]
    
    Paie[ ,   montant.autres.rémunérations
                               :=  (montant.traitement.indiciaire == 0
                                    & montant.rémunération.principale.contractuel == 0
                                    & montant.rémunération.vacataire == 0
                                    & montant.primes == 0)
                               * Montant * Codes.paiement.autres[Code]]
    
    Paie[ ,   montant.indemnité.élu
                               :=  (montant.traitement.indiciaire  == 0
                                    & montant.rémunération.principale.contractuel == 0
                                    & montant.rémunération.vacataire == 0
                                    & montant.primes == 0
                                    & montant.autres.rémunérations == 0)
                               * Montant * Codes.paiement.élu[Code]]
    
    ### EQTP  ###
    
  }
  else
    Paie <- mutate(Paie,
                                         montant.traitement.indiciaire
                                         =  Codes.paiement.traitement[Code]*Montant,
                                         
                                         montant.primes
                                         =  (montant.traitement.indiciaire == 0)*
                                           Montant * Codes.paiement.indemnitaire[Code],
                                         
                                         montant.rémunération.principale.contractuel
                                         = (montant.traitement.indiciaire == 0
                                            & montant.primes == 0)
                                         * Montant * Codes.paiement.principal.contractuel[Code],
                                         
                                         montant.rémunération.vacataire
                                         =  (montant.traitement.indiciaire == 0
                                             & montant.primes == 0
                                             & montant.rémunération.principale.contractuel == 0)
                                         * Montant * Codes.paiement.vacations[Code],
                                         
                                         montant.autres.rémunérations
                                         =  (montant.traitement.indiciaire == 0
                                             & montant.rémunération.principale.contractuel == 0
                                             & montant.rémunération.vacataire == 0
                                             & montant.primes == 0)
                                         * Montant * Codes.paiement.autres[Code],
                                         
                                         montant.indemnité.élu
                                         =  (montant.traitement.indiciaire  == 0
                                             & montant.rémunération.principale.contractuel == 0
                                             & montant.rémunération.vacataire == 0
                                             & montant.primes == 0
                                             & montant.autres.rémunérations == 0)
                                         * Montant * Codes.paiement.élu[Code]
                                         
    )
  
  if (inherits(Paie, 'try-error') )
    stop("Il est probable que le fichier des codes n'est pas exhaustif. Avez-vous (re-)généré l'ensemble des codes récemment ?")
}
