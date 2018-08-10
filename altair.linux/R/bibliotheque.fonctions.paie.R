# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
#
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'année 2008.
#
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
#
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
#
# A cet égard l'attention de l'utilisateur est attirée sur les risques
# associés au chargement, à l'utilisation, à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invités à charger et tester l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
#
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.
#
#


##
#  Fonctions auxiliaires
##

#' Concaténation de chaîne de caractères
#'
#' Equivalent de paste0(,)
#' @param x Vecteur de caractères
#' @param y Vecteur de caractères
#' @return Vecteur de caractères concaténé.
#' @examples
#' x <- "abc"
#' y <- "def"
#' cat(x %+% y) # "abcdef"
#' @export

`%+%` <- function(x, y) paste0(x, y)

#' Chemin complet d'un fichier dans le dossier \code{chemin.dossier.données}
#' @param fichier Nom de fichier
#' @export

chemin <-  function(fichier)
  file.path(chemin.dossier.données, fichier)

#' Conversion d'un fichier de ISO-8859-15 en UTF-8
#'
#' Conversion d'un fichier de ISO-8859-15 en UTF-8
#'
#' @param nom Chemin du fichier à encoder
#' @param encodage.in (= encodage.entrée) Encodage du fichier de lecture
#' @return Lancement dun appel système à iconv -t UTF-8
#' @export

file2utf8 <- function(nom, encodage.in = "ISO-8859-15")  {
 err <- system2(iconv, c("-f", encodage.in, "-t", "UTF-8", shQuote(nom), "-c", "-o", "temp"))
 if (! err)  err <- system2("mv", c("temp", shQuote(nom))) else stop("Erreur d'encodage avec iconv")
 if (err) stop("Erreur de copie fichier après encodage avec iconv")
}

#' Conversion d'un fichier en ISO-8859-15
#'
#' Conversion d'un fichier de UTF-8 en ISO-8859-15
#'
#' @param nom Chemin du fichier à encoder
#' @param encodage.in (= encodage.entrée) Encodage du fichier de lecture
#' @return Lancement dun appel système à iconv -t ISO-8859-15
#' @export

file2Latin <- function(nom, encodage.in = "UTF-8")  {

  err <- system2(iconv, c("-f", encodage.in, "-t", "ISO-8859-15", shQuote(nom), "-c", "-o", "temp"))
  if (! err)  err <- system2("mv", c("temp", shQuote(nom))) else stop("Erreur d'encodage Latin avec iconv")
  if (err)  stop("Erreur de copie fichier après encodage Latin avec iconv")
}

#' Lecture d'une base CSV
#'
#' Lecture d'un fichier CSV et conversion en data.table.
#' Si sécuriser.types.sortie = TRUE, forçage des types en sortie.
#'
#' @param encodage Encodage de la base lue. Valeur par défaut : encodage.entrée
#' @param classes Les classes ("character", "numeric") des variables en colonnes
#' @param drop  Rang de la colonne à supprimer
#' @param skip  Nombre de lignes à sauter en début de fichier (défaut aucune).
#' @param rapide Booléen (= FALSE). Si TRUE, et si convertir.encodage est TRUE, convertir en UTF-8 avant lecture.
#' @param séparateur.liste = séparateur.liste.entrée,
#' @param séparateur.décimal = séparateur.décimal.entrée,
#' @param convertir.encodage (= TRUE) convertir en encodage UTF-8 avant lecture
#' @return Une base data.table
#' @examples
#' read.csv.skip(Base, séparateur.décimal = ",")
#' @export

read.csv.skip <- function(x,
                          encodage = encodage.entrée,
                          classes = NA,
                          drop = NULL,
                          skip = 0,
                          rapide = FALSE,
                          séparateur.liste = séparateur.liste.entrée,
                          séparateur.décimal = séparateur.décimal.entrée,
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

#' Sauv.base
#'
#' Sauvegarde d'une base data.table sous forme de fichier CSV
#' Si sécuriser.types.sortie = TRUE, forçage des types en sortie.
#'
#' @param chemin.dossier Chemin du dossier dans lequel la base sera sauvegardée
#' @param nom Nom de l'objet à sauvegarder
#' @param nom.sauv  Chaine de caractères du nom du fichier .csv sans l'extension
#' @param Latin (= convertir.latin) Convertir en encodage latin ISO-8859-15
#' @param sep (= séparateur.liste.sortie)
#' @param dec (= séparateur.décimal.sortie),
#' @param environment (= .GlobalEnv) environnement,
#' @return Valeur booléenne de file.exists(file.path(chemin.dossier, nom.sauv %+% ".csv"))
#' @examples
#' Sauv.base("données", Base, "BaseDonnée", sep = ";", dec = ",")
#' @export
#'

Sauv.base <- function(chemin.dossier,
                      nom,
                      nom.sauv,
                      Latin = TRUE,
                      sep = séparateur.liste.sortie,
                      dec = séparateur.décimal.sortie,
                      environment = .GlobalEnv)
{
  message("Sauvegarde de ", nom.sauv)

  filepath <- file.path(chemin.dossier, nom.sauv) %+% ".csv"

  L <- get(nom, envir = environment)

  if (! is.list(L) || (is.data.frame(L) && nrow(L) == 0)) L <- list(0)

  data.table::fwrite(L,
             filepath,
             sep = sep,
             dec = dec)

  if (Latin) {
    file2Latin(filepath)
    message("Conversion de ", nom.sauv, " en encodage ISO-8859-15")
  }

  file.exists(file.path(chemin.dossier, nom.sauv %+% ".csv"))
}

#' sauv.bases
#'
#' Sauvegarde d'une base data.table sous forme de fichier CSV
#'
#' @param chemin.dossier Chemin du dossier dans lequel la base sera sauvegardée
#' @param env  Environnement
#' @param nom Nom de l'objet à sauvegarder
#' @param ... Autres noms d'objets à sauvegarder
#' @return Liste de booléens résultant de l'application de \link{Sauv.base}
#' @examples
#' envir <- environment()
#' # Générer Bulletins.paie et Paie dans envir
#' sauv.bases("données",  env = envir, c("Bulletins.paie", "Paie"))
#' @export
#'

sauv.bases <- function(chemin.dossier, env, ...)
{
  if (! dir.exists(chemin.dossier))
  {
    stop("Pas de dossier de travail spécifié")
  }

  skiplist <- 2


  tmp <- as.list(match.call())
  tmp[1] <- NULL

  message("Dans le dossier ", chemin.dossier," :")
  invisible(lapply(tmp[-c(1:skiplist)], function(x) {
    if (exists(x, where = env))
    {
        Sauv.base(chemin.dossier,
                               x,
                               x,
                           FALSE,
               environment = env)
    }  else {
      cat("Pas de base", x)
    }

  }))
}

# Utiliser une assignation globale
# car la fonction anonyme ne comporte que de variables locales

#' Lecture d'une série de bases CSV
#'
#' Appelle \link{read.csv.skip} sur chaque base d'une série de chemins et empile les retours en lignes
#'
#' @param base.string Vecteur de caractères du nom de l'objet data.table retourné
#' @param fichiers    Vecteur de chemins de fichiers
#' @param charger     Booléen : TRUE pour charger les bases, FALSE sinon (sans effet)
#' @param colClasses  Vecteur de classes ("numeric" ou "character", etc.) caractérisant les colonnes
#' @param skip        Sauter les N premières lignes
#' @param drop        Rang de la colonne à supprimer
#' @param séparateur.liste  Séparateur des champs CSV
#' @param séparateur.décimal  Séparateur décimal
#' @param rapide      Accélération parallèle ou pas
#' @param convertir.encodage  convertir d'encodage (basculer entre Latin-1 et UTF-8)
#' @param encodage    Encodage de la base d'entrée.
#' @return Objet \code{data.table} résultant de l'empilement des bases lues.
#' @examples
#' test <- data.table(datasets::cars)
#' res  <- try(Read.csv("base",
#'                      "test.csv",
#'                       colClasses = c("integer", "integer"),
#'                       séparateur.liste = ";",
#'                       séparateur.décimal = ",",
#'                       convertir.encodage = FALSE,
#'                       encodage = "UTF-8",
#'                       rapide = TRUE),
#'           silent = FALSE)
#' if (inherits(res, 'try-error'))
#'   stop("Problème de lecture de la base de la table bulletins-lignes de Paie")
#' @export

Read.csv <- function(base.string, fichiers,
                     charger = TRUE,
                     colClasses = NA,
                     skip = 0,
                     drop = NULL,
                     séparateur.liste = séparateur.liste.entrée,
                     séparateur.décimal = séparateur.décimal.entrée,
                     rapide = FALSE,
                     convertir.encodage = TRUE,
                     encodage = encodage.entrée) {

  Read.csv_(base.string,
            fichiers,
            charger,
            colClasses,
            skip,
            drop,
            séparateur.liste,
            séparateur.décimal,
            rapide,
            convertir.encodage,
            encodage)
}

Read.csv_ <- function(base.string, fichiers,
                      charger = TRUE,
                      colClasses = NA,
                      skip = 0,
                      drop = NULL,
                      séparateur.liste = séparateur.liste.entrée,
                      séparateur.décimal = séparateur.décimal.entrée,
                      rapide = FALSE,
                      convertir.encodage = TRUE,
                      encodage = encodage.entrée) {

    if (charger) {

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

#' Statistiques descriptives de base (minimum, maximum et quartiles)
#'
#' Lecture d'un fichier CSV et conversion en data.table.
#' Si sécuriser.types.sortie = TRUE, forçage des types en sortie.
#'
#' @param x Encodage de la base lue. Valeur par défaut : encodage.entrée
#' @param y Les classes ("character", "numeric") des variables en colonnes
#' @param align  Rang de la colonne à supprimer
#' @param extra  Nombre de lignes à sauter en début de fichier (défaut aucune).
#' @param type Booléen (= FALSE). Si TRUE, et si convertir.encodage est TRUE, convertir en UTF-8 avant lecture.
#' @return Une base data.table
#' @examples
#' read.csv.skip(Base, séparateur.décimal = ",")
#' @export

Résumé <- function(x,
                   y,
                   align = 'r',
                   extra = 0,
                   type = "pond")
{
  essayer(label = "+quartiles", Résumé_(x,y, align, extra, type), "La distribution des quartiles n'a pas pu être déterminée.")
}

Résumé_ <- function(x,y, align = 'r', extra = 0, type = "pond")  {

      Y <- na.omit(y)


      if (! is.list(Y) || is.data.frame(Y)) {

        if (type == "pond") {
          T <- sapply(Y, function(x) {
            q <- bigvis::weighted.quantile(x, Y$quotité.moyenne, na.rm = TRUE)
            q <- append(q, weighted.mean(x, Y$quotité.moyenne, na.rm = TRUE), 3)
            formatC(q, big.mark = " ", format = "fg", digits = 2)
          })

          S <- cbind(c("Minimum", "1er quartile", "Médiane", "Moyenne", "3ème quartile", "Maximum"), T)

        } else if (type == "standard") {

          S <- cbind(c("Minimum", "1er quartile", "Médiane", "Moyenne", "3ème quartile", "Maximum"),
                     sapply(Y, function(x) formatC(summary(x), big.mark = " ", digits = 1, format = "f")))
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

                                 formatC(q, big.mark = " ", format = "fg", digits = 2)
                       }))
        } else if (type == "standard") {

          S <-cbind(c("Minimum", "1er quartile", "Médiane", "Moyenne", "3ème quartile", "Maximum"),
                    sapply(Y, function(x) formatC(summary(x), big.mark = " ", digits = 1, format = "f")))
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

#' Tableau
#'
#' Présentation de tables sous forme de tableau d'une seule ligne de données.
#' Le tableau ne peut pas présenter plus d'une seule ligne.
#'
#' @param x Vecteur de nom de lignes pour le tableau
#' @param ...  paramètres. Si sep.milliers est un paramètre, sa valeur est utiisée comme séparateur des milliers. Par défault, le séparateur blanc.
#' @return Base de données data.table mise en forme de tableau par la fonction knitr::kable comportant l'ensemble des paramètres mis en colonnes centrées, avec x comme noms de lignes.
#' @examples
#' Tableau(c("a", "b", "c", "d"), 1, 2, 3, 4)
#'
#' | a | b | c | d |
#' |:-:|:-:|:-:|:-:|
#' | 1 | 2 | 3 | 4 |
#'
#' @export
#'

Tableau <- function(x, ...) {

  V <- c(...)

  if ("sep.milliers" %in% names(V)) {

    sep.milliers <- V["sep.milliers"]
    V$sep.milliers <- NULL

  }  else {

  sep.milliers <- " "
  }

  T <- t(prettyNum(V, big.mark = sep.milliers))

  T <- data.table(T)

  names(T) <- x
  kable(T, row.names = FALSE, align = "c", booktabs= TRUE)
}


#' Tableau de plusieurs lignes
#'
#' Présentation de tables sous forme de tableau de plusieurs lignes de données.
#'
#' @param colnames Vecteur de nom de colonnes pour le tableau
#' @param rownames Vecteur de nom de lignes pour le tableau
#' @param extra (= "") Si la valeur de ce paramètre est "variation", la variation relative est calculée entre le début et la fin de la période en lignes
#' @param ...  paramètres fonctionnels uniquement. Si un paramètre n'est pas une fonction, le tableau est vide dans son ensemble
#' @return Base de données data.table mise en forme de tableau par la fonction knitr::kable comportant l'ensemble des paramètres mis en colonnes centrées, avec x comme noms de lignes.
#' @examples
#' Tableau(c("a", "b", "c", "d"), 1, 2, 3, 4)
#'
#' | a | b | c | d |
#' |:-:|:-:|:-:|:-:|
#' | 1 | 2 | 3 | 4 |
#'
#' @export
#'

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
        a <- as.character(prettyNum((h(f(rownames[lr]))/h(f(rownames[1])) - 1) * 100, digits = 3))
        if  (length(a) > 0)  S[ceiling(lr/2)] <- a else S[ceiling(lr/2)] <- ""

        S
    }


    if (! missing(extra) && is.character(extra) && extra == "variation") {
      T <- data.table(rownames)
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

      M <- as.data.table(lapply(tmp, function(f) sapply(rownames, f)))

      if (nrow(M) == lr) {

        T <- cbind(rownames, M)
       # names(T) <- colnames

      } else {
        cat("Il manque une ligne au moins dans la table. Annulation. [extra != variation]")
        return("")
      }
    }

    kable(T, row.names = FALSE, align = "c", booktabs= TRUE)
    return(invisible("kable"))
}

#' Tableau vertical 2
#'
#' @export

Tableau.vertical2 <- function(colnames, rownames, ...)
{
  tmp <- list(...)
  N <- names(tmp[1])
  if (!is.null(N) && N == "digits") {
    digits = tmp[[1]]
    tmp <- tmp[-1]
  } else {
    digits = 1
  }

  res <- try(T <- data.frame(rownames,
                  lapply(tmp, function(y) formatC(y,
                                                          big.mark = " ",
                                                          width = "12",
                                                          format = "f",
                                                          digits = digits,
                                                          decimal.mark =",",
                                                          preserve.width ="common"))))
  if (inherits(res, 'try-error')) return("")

  names(T) <- colnames

  kable(T, row.names = FALSE, align = NULL, booktabs= TRUE)
}



v.jmois  <-  c(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
v.jmois.leap  <-  c(31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)

#' Calcul du nombre de jours dans le mois
#' @export

calcul.nb.jours.mois <- function(Mois, année)   if ((année - 2008) %% 4 == 0) {
    return(sum(v.jmois.leap[Mois]))
    } else {
    return(sum(v.jmois[Mois]))
  }

#' Sélectionne les éléments positifs d'un vecteur
#' @export
positive <- function(X) X[!is.na(X) & X > 0]

#' Sélectionne les éléments non nuls d'un vecteur
#' @export
non.null <- function(X) X[!is.na(X) & X != 0]

#' Teste si, dans une base, la proportion d'enregistrements Noms-Prénoms dont les matricules ne sont pas identiques
#'  reste inférieure à une marge de tolérance fixée (taux.tolérance.homonymie)
#'
#' @param Base Base à tester
#' @export

tester.homogeneite.matricules <- function(Base) {

  message("Contrôle sur la cohérence de l'association Nom-Prénom-Matricule (homonymies et changements de matricule)")

    convertir.nom.prénom.majuscules <- function(S)
  {
    S[ , c("Nom", "Prénom")] <- apply(S[ , c("Nom", "Prénom")],
                                      2,
                                      function(x)
                                        toupper(chartr("éèôâçë","eeoaice", x)))
  }

  S <- convertir.nom.prénom.majuscules(Base[ , c("Nom", "Prénom", "Matricule")])

  with.matr    <-   nrow(unique(S))
  without.matr <-   nrow(unique(S[ , c("Nom", "Prénom")]))

  message("Matricules distincts: ", with.matr)
  message("Noms-Prénoms distincs: ", without.matr)

  if (with.matr  >   (1 + taux.tolérance.homonymie / 100) * without.matr)
  {
     msg <- paste0("Résultats trop différents (", taux.tolérance.homonymie, " % de marge tolérée). Changement de régime de matricule.")
     message(msg)
     cat(msg)
  }
}

#' Longueur d'un vecteur, ou nombre de lignes d'une table, lorsque l'on a retiré les éléments ou les lignes NA
#' @param v  Vecteur ou data.frame/data.table
#' @return Nombre d'éléments ou de lignes.
#' @export

longueur.non.na <- function(v) if (is.vector(v)) length(v[!is.na(v)]) else if (is.data.frame(v)) nrow(na.omit(v))

#' Saut de page dans les rapports d'analyse
#'
#'@return Aucun
#'@export

newpage <- function() {
  if (PDF == TRUE) {
  cat("\n\\newpage\n")
  } else {
  cat("<p style=\"page-break-after:always;\"></p>")
  }
}

#' Saut de ligne dans les rapports d'analyse
#'
#' @return Aucun
#' @export

newline <- function() {
  if (PDF == TRUE) {
    return(" \\newline ")
  } else {
    return("<br>")
  }
}

# numérotation des tableaux

#' Incrémente le numéro de tableau courant
#'
#' @return Valeur incrémentée de numéro.tableau
#' @export

incrément <- function() {
  numéro.tableau <<- numéro.tableau + 1
  numéro.tableau
}

#' Incrémente le numéro de chapitre courant
#'
#' @return Valeur incrémentée de chapitre
#' @export

incrémenter.chapitre <- function() {
  chapitre <<- chapitre + 1
  invisible(chapitre)
}

#' Ajoute le séparateur des milliers et une virgule des décimales en français
#'
#' @param  x Vecteur de valeurs numériques
#' @return Vecteur de chaînes de caractères modifiées
#' @examples FR(1235345.23) #1 235 345,2
#' @export

FR <- function(x) formatC(x, big.mark = " ", format = "f", decimal.mark = ",", digits = 1)

#' Essaye d'exécuter une portion de code et en cas d'erreur continue l'exécution du script en renvoyant un message d'erreur non bloquant.
#'
#' @param code Portion de code.
#' @param message Message d'erreur.
#' @param abort Arrêt du programme en cas d'erreur.
#' @param prof Profiler ou pas.
#' @param times Nombre de tests de benchmark.
#' @return Valeur retournée par X en cas de succès, sinon objet de classe "try-error" retourné par try(code)
#' @examples
#' if (exists("e")) rm(e)
#' essayer({ a <- 1/e}, "division par inconnu")  # affichage du message d'erreur
#' @export

essayer <- function(X, Y, abort = FALSE, prof = profiler, times = 5, label = "") {

  if (prof) {
        DT <- system.time({res <- try(X, silent = FALSE)})
        if (label == "") label <- substr(paste(deparse(substitute(X)), collapse = " "), start = 1, stop = 20)
  } else {

  res <- try(X, silent = FALSE)
  }

  if (inherits(res, 'try-error')) {
      cat(Y)
      if (abort) {

        stop("Arrêt de programme.")
      }
  } else {
      if (class(res) == "knitr_kable")
        print (res)
  }
  
  if (prof) {

      assign("PROF", rbind(get("PROF", envir = .GlobalEnv), data.table(expr = label, median = median(DT["elapsed"]))), envir = .GlobalEnv)
      return(invisible(NULL))

  } else {

      return(invisible(res))
  }
}

#' Prise en compte du pluriel
#'
#' @param Mot Mot à pluraliser éventuellement
#' @param N   Entier : pluriel si N > 1.
#' @examples
#' cat("patient" %s% 2)
#' @return Concaténation du mot et de "s" si N > 1
#' @export

'%s%' <- function(mot, N) if (N > 1) mot %+% "s"  else mot

#' Sélection du filtre correspondant à une chaîne de caractères associée
#'
#' @param x  Chaîne de caractères associée figurant dans la colonne \code{type} de la table \code{codes}
#' @return Cette fonction renvoie un ensemble de valeurs fixes ou une expression régulière, sauf si x n'est pas dans la colonne \code{type} de la table \code{codes} (renvoie NULL)
#' @export

filtre <- function(x) {

  if (! x %in% codes$type) return(NULL)

  if (is.na(codes[x, valeur])) {
    return(codes[x, expression])
  } else {
    return(unlist(codes[x, valeur]))
  }

}

#' Filtrage d'une base de paye
#'
#' Filtre la base par une expression régulière sur libellés de paye ou par valeurs fixes sur une variable donnée.
#' Si le filtrage a une portée, l'ensemble des lignes de la portée (exemple "Mois") est conservé.
#' @export

filtrer_Paie <- function(x,
                         portée = NULL,
                         Base = Paie,
                         Var = "Code",
                         indic = FALSE) {
  P_ <- NULL

  essayer(label = "filtre paie", {

  filtre_ <- filtre(x)

  # En i, la variable Var ne peut pas être évaluée mais peut filtrer sur Paie[get(Var)...]
  # en by, utiliser dans c(...) directement

  if (is.null(portée)) {

    if (is.na(codes[x, valeur])) {

      P_  <- Base[grepl(filtre_, Libellé, ignore.case=TRUE, perl=TRUE)]

    } else {

      P_  <- Base[get(Var) %chin% filtre_]
    }

  } else {

    if (is.na(codes[x, valeur])) {

      if (indic) {

        P_  <- Base[ , indic := grepl(filtre_, Libellé, ignore.case=TRUE, perl=TRUE)
                   ][ , indic0 := any(indic),
                           by = c("Matricule", "Année", portée)][indic0 == TRUE][, indic0 := NULL]
      } else {

        P_  <- Base[ , indic0 := any(grepl(filtre_, Libellé, ignore.case=TRUE, perl=TRUE)),
                     by = c("Matricule", "Année", portée)
                   ][indic0 == TRUE][, indic0 := NULL]
      }

    } else {

      if (indic) {

        P_  <- Base[ , indic := get(Var) %chin% filtre_,

                   ][ , indic0 := any(indic),
                            by = c("Matricule", "Année", portée)
                   ][indic0 == TRUE][, indic0 := NULL]
      } else {

        P_  <- Base[ , indic0 := any(get(Var) %chin% filtre_),
                     by = c("Matricule", "Année", portée)
                 ][indic0 == TRUE][, indic0 := NULL]
      }
    }
  }
  # Redéfinition de la quotité nécessaire en raison du fait
  # que la quotité précédemment définie est une quotité statistique
  # Pour les vérifications de liquidation il faut une quotité réelle

  if (all(c("Temps.de.travail", "quotité.moyenne.orig") %chin% names(Base))) {
       P_[ , `:=`(quotité = Temps.de.travail / 100,
                  quotité.moyenne = quotité.moyenne.orig)]
  }

  },  "Le filtre n'a pas pu être appliqué ( " %+% x %+%" ).")

  P_
}


extraire_paye_ <- function(an, L, out) {

  if (! is.null(L)) {
    assign(out, unique(Bulletins.paie[Année == an
                                                 & Mois == 12
                                                 & Statut != "ELU"
                                                 & Statut %chin% L,
                                                 .(Matricule, Nir)]), .GlobalEnv)
  } else {
  assign(out, unique(Bulletins.paie[Année == an
                               & Mois == 12
                               & Statut != "ELU",
                               .(Matricule, Nir)]), .GlobalEnv)
  }
}

#' Extraire les matricules et Nir des mois de décembre, sauf pour les élus
#' Pour les statuts listés dans L si L non null
#' @param an  Année
#' @param L   Vecteur des statuts considérés
#' @param out  Data.table extraite de \code{Bulletins.paie} de deux colonnes (\code{Matricule} et \code{Nir}), filtrée des doublons.
#' @examples extraire_paye(2012, c("TITULAIRE", "STAGIAIRE"), "Bulletins.début.psr")
#' @export
#'
extraire_paye <- function(an, L, out) {
  essayer({extraire_paye_(an, L, out)}, "Les informations de base sur l'âge des personnels n'ont pas pu être extraites des champs Nir.", abort = TRUE)
}

#' Insérer un script auxiliaire, indexé par une variable globale
#' @param chemin  Chemin du script R
#' @param index   Vecteur numérique contenant les valeurs de la variable globale.
#' @param variable Vecteur de caractères contenant le nom de la variable globale dans le script auxiliaire.
#' @param gen  Si \code{FALSE} alors se contente de sourcer le script auxiliaire selon \code{encodage.code.source}. Sinon intègre le rapport auxiliaire au format du rapport principal.
#' @return Valeur de la dernière variable globale \code{variable} instanciée. Effets de bord en sortie.
#' @export

insérer_script <- function(chemin, index, variable = "année", gen = générer.rapport) {

invisible(lapply(index, function(x) {
  assign(variable, x, .GlobalEnv)
  incrémenter.chapitre()
  if (! gen) {

    source(chemin, encoding = encodage.code.source)

  } else {

    cat(knit_child(text = readLines(spin(chemin, knit = FALSE), encoding = encodage.code.source), quiet = TRUE), sep = '\n')
  }
}))

get(variable)
}

