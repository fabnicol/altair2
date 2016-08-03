##
#  Fonctions auxiliaires
##



chemin <-  function(fichier)
  file.path(chemin.dossier.données, fichier)

file2utf8 <- function(nom, encodage.in = encodage.entrée)  {
  
 chem <- chemin(nom)
 err <- system2(iconv, c("-f", encodage.in, "-t", "UTF-8", shQuote(chem), "-o", "temp"))
 if (! err)  err <- system2("mv", c("temp", shQuote(chem))) else stop("Erreur d'encodage avec iconv")
 if (! err)  message("Conversion réussie") else stop("Erreur de copie fichier après encodage avec iconv")
}

essayer <- function(X, Y) {
  
  if (inherits(try(X), 'try-error')) cat(Y)
  
}


#obsolète
# en.séparateurs <- function(chem)  {
# 
#   commande <- sed %+% " -e s/,/\\./g -e s/;/,/g -i " %+% shQuote(chem)
#   shell(commande)
# }

# fr.séparateurs <- function(chem)  {
#   
#   commande <- sed %+% " -e s/,/;/g -e s/\\./,/g -i " %+% shQuote(chem)
#   shell(commande)
# }


# Obsolète. 
# Trouve le numéro de la ligne à laquelle se situe la liste des noms de variables
# en recherchant soit le mot "Matricule" soit une expression du type "Code..."
# Il faudra déduire ce "skip" du read.csv pour récupérer proprement les noms de variable

# Pour cela on scanne les 25 premières lignes de la table une première fois


# trouver.valeur.skip <-  function(chemin.table, encodage, classes = NA, séparateur.liste = séparateur.liste.entrée, séparateur.décimal = séparateur.décimal.entrée)
#   max(
#     sapply(
#       read.csv(chemin.table, sep=séparateur.liste, dec=séparateur.décimal, nrows = 25, fileEncoding = encodage.entrée, colClasses = classes),
#       function(x)
#       {
#         m <- match(champ.détection.1, x, nomatch = 0 )
#         if (m == 0)
#           m <- pmatch(champ.détection.2, x, nomatch = 0, duplicates.ok = FALSE )
#         return(m)
#       }
#     ))


# selectionner.cle.matricule <-  function(Base1, Base2)
# {
#   if (fusionner.nom.prénom) {
#      subset(Base1,
#            select = c("Nom", "Prénom", étiquette.matricule, setdiff(names(Base1),names(Base2))))
#     } else {
#      subset(Base1,
#          select = c(étiquette.matricule, setdiff(names(Base1), names(Base2))))
#     }
# }

# sélectionner.clé <-  function(base1, base2)
# {
#   Base1 <- get(base1)
#   Base2 <- get(base2)
# 
#   if (fusionner.nom.prénom) {
# 
#     Set1 <- c("Mois", "Année", étiquette.matricule, setdiff(names(Base1), names(Base2)))
#     Set2 <- setdiff(names(Base2), c("Nom", "Prénom", étiquette.matricule))
# 
#     assign(base1,
#            subset(Base1, select = c("Nom", "Prénom", Set1)),
#            envir = .GlobalEnv)
# 
#     assign(base1,
#            cbind(as.data.frame(convertir.nom.prénom.majuscules(Base1[, c("Nom", "Prénom")])),
#                  Base1[, Set1]),
#            envir = .GlobalEnv)
# 
#     assign(base2,
#            cbind(as.data.frame(convertir.nom.prénom.majuscules(Base2[, c("Nom", "Prénom")])),
#                  Base2[, Set2]),
#            envir = .GlobalEnv)
# 
# 
# 
#   } else {
# 
#     assign(base1, subset(Base1,
#                          select = c(étiquette.matricule,"Mois","Année",
#                          setdiff(names(Base1), names(Base2)))), envir = .GlobalEnv)
# 
#   }
# }
# 


read.csv.skip <- function(x, encodage = encodage.entrée, classes = NA, drop = NULL, skip = 0,
                          rapide = FALSE, séparateur.liste = séparateur.liste.entrée, séparateur.décimal = séparateur.décimal.entrée,
                          convertir.encodage = TRUE)
{
  
  
  if (! rapide) {

    T <- read.csv(x,
                   comment.char = "",
                   sep = séparateur.liste,
                   dec = séparateur.décimal,
                   colClasses = classes,
                   skip = skip, 
                    # obsolète : trouver.valeur.skip(chem, encodage, séparateur.liste = séparateur.liste, séparateur.décimal = séparateur.décimal),
                   encoding = encodage)

    if (!is.null(drop)) { T <- T[-(drop)] }

  } else {
    
    if (encodage != "UTF-8" && convertir.encodage) {
      message("La table en entrée doit être encodée en UTF-8")
      if (convertir.encodage) message("Conversion via iconv du format " %+% encodage %+% " au format UTF-8...") else stop("Arrêt : convertir l'encodage de la table en UTF-8.")
      file2utf8(x, encodage.in = encodage)
    }
      
    # data.table n'admet d'argument dec qu'à partir de la version 1.9.5 
    
    if (is.na(classes)) classes = NULL
   
    T <- try(data.table::fread(x,
                      sep = séparateur.liste,
                      dec = séparateur.décimal,
                      header = TRUE,
                      skip = skip,
                      colClasses = classes,
                      showProgress = FALSE,
                      encoding = ifelse(setOSWindows, "Latin-1", "UTF-8")))


  }

  if (sécuriser.types.sortie) {
  # procédure de vérification et de coercition des type de sortie
  # il peut arriver que data.table produise des colonnes de type différent (classes.expost) de celui qui est demandé
  # par le paramètre classes. Cela peut arriver quand un charactère est utilisé à la place d'un chiffre dans les données
  # spécifiées comme purement numériques. Le code ci-dessous corrige les cas les plus utiles.
  # Le code C++ de l'extracteur de données pourrait à terme éliminer ces risques en garantissant le type des données en sortie.
  
  if (! is.null(classes)) {
    
   classes.expost <- sapply(T, class) 
   
   test <- (classes != classes.expost)
   
   if (any(test)) {

     sapply(1:length(classes), function(x){  
              
              if (classes.expost[x] == "character" && (classes[x] == "numeric" || classes[x] == "integer")) {

                T[[x]] <<- as.numeric(gsub(",", ".", T[[x]], fixed=TRUE))
                    # <<- impératif
                
              } else if ((classes.expost[x] == "numeric" || classes.expost[x] == "integer")
                    && (classes[x] == "character")) 
                
                       T[[x]] <<- as.character(T[[x]])
     })
    }
   }
  }

return(T)
}

Sauv.base <- function(chemin.dossier, nom, nom.sauv, encodage = encodage.sortie, sep = séparateur.liste.sortie, dec = séparateur.décimal.sortie, environment = .GlobalEnv)
{
  message("Sauvegarde de ", nom.sauv)
 
  write.table(get(nom, envir = environment),
             paste0(chemin.dossier, "/", nom.sauv, ".csv"), 
             quote = FALSE,
             sep = sep,
             dec = dec,
             row.names = FALSE,
             fileEncoding = encodage)
}

sauv.bases <- function(dossier, env, ...)
{
  if (! dir.exists(dossier))
  {
    stop("Pas de dossier de travail spécifié")
  }

  tmp <- as.list(match.call())
  tmp[1] <- NULL

  message("Dans le dossier ", dossier," :")
  invisible(lapply(tmp[-c(1,2)], function(x) if (exists(x, where = env)) Sauv.base(dossier, x, x, environment = env)))
}

# Utiliser une assignation globale
# car la fonction anonyme ne comporte que de variables locales

Read.csv <- function(base.string, fichiers, charger = charger.bases, colClasses = NA, skip = 0,
                     drop = NULL, séparateur.liste = séparateur.liste.entrée, séparateur.décimal = séparateur.décimal.entrée,
                     rapide = FALSE, convertir.encodage = TRUE, encodage = encodage.entrée)  {

    if (charger.bases) {

        assign(base.string,
               do.call(rbind, lapply(fichiers,
                                     read.csv.skip,
                                        classes = colClasses,
                                        skip = skip,
                                        séparateur.liste = séparateur.liste,
                                        séparateur.décimal = séparateur.décimal,
                                        drop = drop,
                                        convertir.encodage = convertir.encodage,
                                        encodage = encodage,
                                        rapide = rapide)),
               envir = .GlobalEnv)
    }
}


Résumé <- function(x,y, align = 'r', extra = 0, type = "pond")  {
    
      Y <- na.omit(y)
 
      
      if (! is.list(Y) || is.data.frame(Y)) {

        if (type == "pond") {
          T <- sapply(Y, function(x) {
            q <- bigvis::weighted.quantile(x, Y$quotité.moyenne, na.rm = TRUE) 
            q <- append(q, weighted.mean(x, Y$quotité.moyenne, na.rm = TRUE), 3)
            prettyNum(q, big.mark = " ", digits = 2)
          })
          
          S <- cbind(c("Minimum", "1er quartile", "Médiane", "Moyenne", "3ème quartile", "Maximum"), T)
          
        } else if (type == "standard") {
          S <- cbind(c("Minimum", "1er quartile", "Médiane", "Moyenne", "3ème quartile", "Maximum"),
                     sapply(Y, function(x) prettyNum(summary(x), big.mark = " ")))
        }
        
        
        if (! missing(extra)) {
          if (extra == "length") {

             S <- cbind(S, c("", "", "", ifelse(is.vector(Y), length(Y), nrow(Y)), "", ""))
   
          } else {    
            
            if (is.numeric(extra))
              S <- cbind(S, c("", "", "", as.character(extra), "", ""))
          }
       }
               
      } else {

        if (type == "pond") {
          S <- cbind(c("Minimum", "1er quartile", "Médiane", "Moyenne", "3ème quartile", "Maximum"),
                     sapply(Y, function(x) {
                                 q <- bigvis::weighted.quantile(x[[1]],
                                                   floor(x[[2]] * 1000), na.rm = TRUE)
                                 
                                 q <- append(q, weighted.mean(x[[1]], x[[2]], na.rm = TRUE), 3)
                               
                                 prettyNum(q, big.mark = " ")
                       }))
        } else if (type == "standard") {
          S <- cbind(c("Minimum", "1er quartile", "Médiane", "Moyenne", "3ème quartile", "Maximum"),
                     sapply(Y, function(x) prettyNum(summary(x), big.mark = " ")))
        }
        
        if (! missing(extra) && extra == "length") {
          
           n <- ncol(S) - 1  
           temp <- S[ , 1]
           nrow.S <- length(temp)
          
           for (i in 1:n) {
               
               if (length(S[ , i + 1]) == nrow.S) {
                 temp <- cbind(temp, S[ , i + 1])

                 temp <- cbind(temp, c("", "", "", ifelse(is.vector(Y[[i]]), length(Y[[i]]), nrow(Y[[i]])), "", ""))
               } else {
                 cat("Impossible de générer le tableau : ligne manquante.")
                 return("")
               }
               

           }
           # S est de type matrix
           
           S <- temp
        }
      }

     if (length(dimnames(S)[[2]]) == length(x) + 1) {
       
       dimnames(S)[[2]] <- c("Statistique", x)

       kable(S, row.names = FALSE, align = align, booktabs = TRUE)
     } else {
       cat("Table non générée.")
     }

}

Tableau <- function(x, ...)
{
  V <- c(...)
  if ("sep.milliers" %in% names(V))
  {
    sep.milliers <- V["sep.milliers"]
    V$sep.milliers <- NULL
  }  else
  sep.milliers <- " "

  T <- t(prettyNum(V, big.mark = sep.milliers))
  T <- as.data.frame(T)
  names(T) <- x
  kable(T, row.names = FALSE, align = "c", booktabs= TRUE)
}

Tableau.vertical <- function(colnames, rownames, extra = "", ...)   # extra functions in ... first labeled f
{
    tmp <- c(...)
   
    if (! all(lapply(tmp, is.function))) {
      message("all arguments must be functions")
      return("")
    }
    
    lr <- length(rownames)
    
    h <- function(x) as.numeric(sub(",",".", sub(" ", "", x, fixed=T), fixed=T))
    
    g <- function(f) {
        S <- rep("", lr)
    
        S[ceiling(lr/2)] <- as.character(prettyNum((h(f(rownames[lr]))/h(f(rownames[1])) - 1) * 100, digits = 3))
    
        S
    }
      
  
    if (! missing(extra) && is.character(extra) && extra == "variation") {
      T <- data.frame(rownames)
      NT <- colnames[1]
      ltmp <- length(tmp)
      
      for (x in seq_len(ltmp)) {
        
        res1 <- try(T1 <- unlist(sapply(rownames, tmp[[x]])))
        res2 <- try(T2 <- g(tmp[[x]]))
        
        if (inherits(T1, 'try-error') || inherits(res2, 'try-error')) {
          cat("Impossible de générer le tableau.")
          return("")
        }
        
        # unlist est rendu nécessaire par le fait que lorsque character(0) apparaît, sapply n'arrive pas à délister.
        
        if (length(T1) == lr && length(T2) == lr) {
          
          T <- cbind(T, T1, T2)
          NT <- c(NT, colnames[[x + 1]], "Variation (%)")
          
        } else {
          cat("Il manque une ligne au moins dans la table. Annulation. [extra = variation]")
          return("")
        }
      }
    
      names(T) <- NT
      
    } else {
 
      M <- lapply(tmp, function(f) sapply(rownames, f))
      
      if (nrow(M) == lr) {

        T <- data.frame(rownames, M)
        names(T) <- colnames
        
      } else {
        cat("Il manque une ligne au moins dans la table. Annulation. [extra != variation]")
        return("")
      }
    }
    
    kable(T, row.names = FALSE, align = "c", booktabs= TRUE)
}

Tableau.vertical2 <- function(colnames, rownames, ...)
{
  tmp <- list(...)
  
  res <- try(T <- data.frame(rownames, 
                  lapply(tmp, function(y) formatC(y, 
                                                          big.mark=" ",
                                                          width="12",
                                                          format="f",
                                                          digits=1,
                                                          decimal.mark=",",
                                                          preserve.width="common"))))
  if (inherits(res, 'try-error')) return("")
  
  names(T) <- colnames

  kable(T, row.names = FALSE, align = NULL, booktabs= TRUE)
}



v.jmois  <-  c(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
v.jmois.leap  <-  c(31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)

calcul.nb.jours.mois <- function(Mois, année)   if ((année - 2008) %% 4 == 0) {
    return(sum(v.jmois.leap[Mois])) 
    } else {
    return(sum(v.jmois[Mois]))
  }
    

positive <- function(X) X[!is.na(X) & X > 0]
non.null <- function(X) X[!is.na(X) & X != 0]
significatif <- function(x) !is.na(x) & abs(x) > 0.01

# obsolète

# installer.paquet <- function(paquet, rigoureusement = FALSE)
# {
#   if (missing(paquet))  return(NA_character_)
#   Paquet <- deparse(paquet)
#   if (length(find.package(Paquet, quiet = TRUE)) == 0)
#   {
#     install.packages(Paquet)
#     if (length(find.package(Paquet, quiet = TRUE)) !=0 )
#     {
#       message(Paquet, " a été installé.")
#       return(invisible(1))
#     }
#     else
#     {
#       message(Paquet, " n'a pas été installé.")
#       if (rigoureusement == TRUE)
#       {
#         message("Arrêt: le paquet ", Paquet, " n'a pas pu être installé.")
#         stop("Fin")
#       }
#       return(invisible(0))
#     }
#   }
#   else
#     message(Paquet, " est déjà installé.")
#   return(invisible(0))
# }
# 
# installer.paquets <- function(..., rigoureusement = FALSE)
# {
#   tmp <- as.list(match.call())
#   tmp[1] <- NULL
#   if (!missing(rigoureusement)) tmp[length(tmp)] <- NULL
#   if (length(tmp) == 0) return(0)
# 
#   invisible(do.call(sum, lapply(tmp, function(x) installer.paquet(x, rigoureusement))))
# }

convertir.nom.prénom.majuscules <- function(S)
{

  S[ , c("Nom", "Prénom")] <- apply(S[ , c("Nom", "Prénom")],
                                    2,
                                    function(x)
                                      toupper(chartr("éèôâçë","eeoaice", x)))


}




# tester.homogeneite.matricules(Base)

#  Teste si, dans une base, la proportion d'enregistrements Noms-Prénoms dont les matricules ne sont pas identiques
#  reste inférieure à une marge de tolérance fixée (taux.tolérance.homonymie)
#  utilité : tester si l'appariement sur Nom-Prénom au lieu de matricule sera acceptable

tester.homogeneite.matricules <- function(Base) {

  message("Contrôle sur la cohérence de l'association Nom-Prénom-Matricule (homonymies et changements de matricule)")
  S <- convertir.nom.prénom.majuscules(Base[ , c("Nom", "Prénom", "Matricule")])

  with.matr    <-   nrow(unique(S))
  without.matr <-   nrow(unique(S[ , c("Nom", "Prénom")]))

  message("Matricules distincts: ", with.matr)
  message("Noms-Prénoms distincs: ", without.matr)

  if (with.matr  >   (1 + taux.tolérance.homonymie/100) * without.matr)
  {
     message(paste0("Résultats trop différents (", taux.tolérance.homonymie, " % de marge tolérée). Changement de régime de matricule."))
     if (fusionner.nom.prénom == FALSE)
       stop("Vous pouvez essayer de fusionner sur Nom, Prénom en spécifiant fusionner.nom.prénom <- TRUE dans prologue.R", call. = FALSE)
  }
}

longueur.non.na <- function(v) if (is.vector(v)) length(v[!is.na(v)]) else if (is.data.frame(v)) nrow(na.omit(v))

# opérateurs infixe

# concaténer deux strings

`%+%` <- function(x, y) paste0(x,  y)
`..` <- function(...) list(expand.grid(...))


# saut de page

newpage <- function() {
  if (PDF == TRUE) {
  cat("\n\\newpage\n")
  } else {
  cat("<p style=\"page-break-after:always;\"></p>")
  }
}

newline <- function() {
  if (PDF == TRUE) {
    cat("\n\\newline\n")
  } else {
    cat("<br>")
  }
}

# numérotation des tableaux

numéro.tableau <- 0

incrément <- function() { 
  numéro.tableau <<- numéro.tableau + 1 
  numéro.tableau
}

chapitre <- 0

incrémenter.chapitre <- function() {
  chapitre <<- chapitre + 1
  invisible(chapitre)
}

FR <- function(x) formatC(x, big.mark = " ")

essayer <- function(X, Y) {
    
      if (inherits(try(X), 'try-error')) cat(Y)
    
}

