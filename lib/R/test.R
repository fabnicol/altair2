# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2018
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'annee 2008.
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

# prime$nom
# prime$categorie
# prime$restreint_fonctionnaire
# si non null Paie_B doit avoir indic_B

#' Quotité administrative
#' @param quotite  quotite formelle (Temps.de.travail / 100)
#' @export
adm <- function(quotite) ifelse(quotite == 0.8,  6/7, ifelse (quotite == 0.9,  32/35, quotite))


#' Sauvegarde une base dans le dossier des bases
#' 
#' Sauvegarde paramétrée par environnement
#' 
#' @param x  Objet à sauvegarder (vecteur de caractères)    
#' @param y  Nom du fichier de sauvegarde CSV.   
#' @param z  Nom du sous-dossier du dossier des bases.   
#' @param env  Environnement

sauvebase <- function(x, y, z, env) {
  Sauv.base(z,
            x,
            y,
            environment = env)
  
}

#' Affichage du tableau des cumuls de primes
#' 
#' @param résultat  Résultat retourné par la fonction \link{test_prime}
#' @examples
#' tableau_cumuls(test_prime(prime_IAT, prime_B = prime_IFTS, Paie_I, verbeux = FALSE))
#' 
#' |Matricule |Annee |       Grade          |                  Régime                     |
#' |----------|------|----------------------|---------------------------------------------|
#' |010843    |2009  |ANIMATEUR TERRITORIAL |   IFTS 1 mois-IAT 10 mois-Cumul 1 mois      |
#' |010843    |2010  |ANIMATEUR TERRITORIAL |   IFTS 11 mois-IAT 0 mois-Cumul 1 mois      |
#' |010854    |2009  |REDACTEUR TERRITORIAL |   IFTS 9 mois-IAT 2 mois-Cumul 1 mois       | 
#' 
#' @export

tableau_cumuls <- function(résultat, e = NULL) {
  
  if (is.null(e)) return(NULL)
  
  if (! is.null(résultat)
     && ! is.null(résultat$cumuls)
     && nrow(résultat$cumuls[c != 0]) > 0) {
    
    res <- résultat$cumuls[Régime != "", .(Matricule, Annee, Grade, Régime)]

    if (nrow(res) > 0) {
        e$tableau <- kable(res, format = "simple")
        e$res <- TRUE
    } else {
        e$tableau <- ""
        e$res <- FALSE
    }
    
  } else {
    
    e$res <- FALSE
  }
  
  e
}


#' Affichage du tableau des agrégats des primes A et B, pour chaque année de période 
#' 
#' @param résultat  Résultat retourné par la fonction \link{test_prime}
#' @param verbeux   [FALSE] Le résultat n'est affiché que si  \code{verbeux} vaut  \code{TRUE}
#' @examples 
#' agrégat_annuel(test_prime(prime_IAT, prime_B = prime_IFTS, Paie_I, verbeux = FALSE), verbeux = TRUE)
#' @export

agrégat_annuel<- function(résultat, verbeux) {
  
  beneficiaires <- résultat$cumuls[, .(Matricule, Annee, nb.mois, Grade, Agrégat)]
  
  if (verbeux) {
    
    if (nrow(beneficiaires)) {
      
      beneficiaires$Agrégat <- formatC(beneficiaires$Agrégat, big.mark = " ", format="fg")
      
      kable(beneficiaires, align = 'r', format = "simple", row.names = FALSE)
      
    } else {
      cat("\nAucun bénéficiaire détecté.\n")
    }
  }
  
}

#' Affichage du tableau des variations des agrégats des primes A et B sur l'ensemble de la periode
#' 
#' @param résultat  Résultat retourné par la fonction \link{test_prime}
#' @param verbeux   [FALSE] Le résultat n'est affiché que si  \code{verbeux} vaut  \code{TRUE}
#' @export

évolution_agrégat <- function(résultat, verbeux) {
  
  if (verbeux) {
    
    if (nrow(résultat$variations)) {
      
      kable(résultat$variations, align = 'r', format = "simple", row.names = FALSE)
      
    } else {
      cat("\nAucun tableau de variation.\n")
    }
  }
}

#' Analyse des contraintes principales associées à une indemnité 
#' @param prime     Prime au format liste comportant les arguments :
#'   \describe{
#'   \item{nom}{Nom de la prime en majuscules. Une expression régulière en décrivant le libellé doit être enregistrée dans l'espace global sous le nom : expression.rég.nom}
#'   \item{categorie}{"A", "B", "C" ou tout vecteur d'une à deux lettres comprises dans ces trois valeurs. Décrit les categories statutaires auxquelles la prime est attribuable.}
#'   \item{restreint_fonctionnaire}{Booléen. Par défaut FALSE. Préciser TRUE si la prime est uniquement attrubuable aux fonctionnaires. Dans certains cas (mais pas pour tous), la prime peut aussi être attribuable aux non-titulaires, sous réserve d'un acte réglementaire interne à l'organisme.}
#'   \item{dossier}{Chaîne de caractères. Sous-dossier du dossier Bases dans lequel le fichier auxiliaire CSV doit être généré. Par exemple : "Reglementation".}
#'   \item{expr.reg.}{Chaîne de caractères. Expression régulière filtrant sur champs \code{Grade}, décriuvant une contrainte limitant l'accès de la prime à un certain sous-ensemble de grades.}
#'   \item{indice}{Liste. Couple d'un caractère "+" ou "-" et d'un entier, ou triplet correspondant au couple augmenté d'un vecteur d'une ou deux lettres statutaires. Exemple : list("+", 350, c("A","B)). La liste décrit un critère limitatif pour la prime : 
#'   elle ne peut être attribuée qu'aux indices supérieurs ("+") ou inférieurs ("-") au nombre donné en deuxième position pour les fonctionnaires de categorie précisée en troisième prosition.}}
#'   
#' @param Paie_I    Base data.table des indemnités comportant les colonnes :
#' \itemize{
#'   \item{Nom} 
#'   \item{Prenom}
#'   \item{Matricule} 
#'   \item{Annee} 
#'   \item{Mois} 
#'   \item{Debut}
#'   \item{Fin}
#'   \item{Code}
#'   \item{Libelle}
#'   \item{Montant}
#'   \item{Type}
#'   \item{Emploi}
#'   \item{Grade} 
#'   \item{Indice}
#'   \item{Statut}
#'   \item{Categorie}}
#' @param verbeux   [FALSE] Le résultat des tableaux "non titulaires" et "categories" n'est affiché que si \code{verbeux} vaut \code{TRUE}
#' @return  Liste constituée de :
#'  \describe{
#'   \item{Paye}{La base data.table de paye correspondant à la prime en premier argument, toutes primes confondues.}
#'   \item{Lignes}{Les lignes de paye correspondant à la prime en premier argument seulement.}
#'   \item{K}{Codes de paye correspondant à la prime.}
#'   \item{manquant}{Booléen. TRUE si absence de résultat, FALSE sinon.}}
#' @note  Sauvegarde deux fichiers dans le sous-dossier prime$dossier : 
#' \itemize{
#' {prime$nom.non.tit.csv} {Recense les attributaires non titulaires}
#' {prime$nom.cat.A (ou AB ou B ou BC...)} {Recense les attributaires de categorie A, B, C ou toute combinaison de ces lettres.}
#' }   
#' @export

analyser <- function(prime, Paie_I, verbeux, echo = TRUE) {

  Paie_A <- NULL
  Lignes_A <- NULL
  résultat.manquant <- FALSE
  lignes.indice.anormal <- NULL
  
  essayer({  Paie_A   <- filtrer_Paie(prime$nom, 
                             portee = "Mois",
                             Base = Paie_I,
                             indic = TRUE)
    
    Lignes_A <- Paie_A[indic == TRUE
                      ][ , indic := NULL]
  }, 
    "Le filtrage des idemnités " %+% prime$nom %+% " n'a pas pu être réalisé. ")
  
  if (! is.null(prime$indice)) {
    
    essayer({ lignes.indice.anormal <- if (prime$indice[1] == "+"){
        
        Lignes_A[Indice < prime$indice[2]] } else  { Lignes_A[Indice >= prime$indice[2]]
      } 
    
      if (length(prime$indice) >= 3 && prime$indice[3] %chin% c("A", "B", "C")) {
        
        lignes.indice.anormal <-lignes.indice.anormal[Categorie %chin% prime$indice[3]]
          
      } 
          
      nr <- nrow(lignes.indice.anormal)
      
      if (echo) {
        if (! is.null(nr) && nr > 0) {
          cat(nr,
              "attributaires de",
              prime$nom,
              "ne satisfont pas au critère de la borne indiciaire (INM", prime$indice[2] %+% "). " )
        } else {
          cat("Les attributaires de",
              prime$nom,
              "satisfont tous au critère de la borne indiciaire (INM", prime$indice[2] %+% "). ") 
        }
      }
      
    }, 
      "La vérification de la borne indiciaire n'a pas pu êre réalisée. ")
  }
  
  # Questions de compatibilité statutaire
  
  essayer({ if (prime$restreint_fonctionnaire) {
      
      A.non.tit  <- Lignes_A[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]
      
      if (echo) {
        
        if ((N.A.non.tit <- uniqueN(A.non.tit$Matricule)) > 0) {
        
          cat(N.A.non.tit,
              "attributaire" %s% N.A.non.tit,
              prime$nom,
              "sont des non-titulaires. ")
          
          if (verbeux) print(kable(A.non.tit,
                                   align = 'r',
                                   format = "simple",
                                   row.names = FALSE))
        
        } else {
        
          cat("Tous les attributaires de",
              prime$nom,
              "sont titulaires ou stagiaires. ")
        }
      }
    }
    
    if (!is.null(prime$categorie)){
      
      if (! is.null(prime$expr.reg)) {
        
        A.non.cat <- Lignes_A[! Categorie %chin% prime$categorie 
                              | ! grepl(prime$expr.reg, Grade, ignore.case = TRUE, perl = TRUE)] 
        
      } else {
        
        A.non.cat <- Lignes_A[! Categorie %chin% prime$categorie]
      }
      
      if (echo) {
        if ((N.A.non.cat <<- uniqueN(A.non.cat$Matricule)) > 0) {
          
          cat(N.A.non.cat, 
              "attributaires de",
              prime$nom,
              "ne sont pas identifiés en categorie",
              prime$categorie,
              ". ")
          
          if (verbeux)  print(kable(A.non.cat, align = 'r', format = "simple", row.names = FALSE))
          
        } else {
          
          if (echo) cat("Tous les attributaires de",
                        prime$nom,
                        "sont identifiés en categorie",
                        prime$categorie, ". ")
        }
      }
      
    } else {
      
      if (! is.null(prime$expr.reg)) {
        
        A.non.cat <- Lignes_A[! grepl(prime$expr.reg, Grade, ignore.case = TRUE, perl = TRUE)] 
        
        if (echo) {
          if ((N.A.non.cat <<- uniqueN(A.non.cat$Matricule)) > 0) {
          
          cat(N.A.non.cat,
              "attributaires de",
              prime$nom,
              "ne sont pas identifiés comme relevant de grades conformes. ")
          
          if (verbeux)  print(kable(A.non.cat, align = 'r', format = "simple", row.names = FALSE))
          
          }
        }
        
      } else {
       
        cat("La détection des incompatibilités statutaires n'a pas pu être réalisée. ")
        stop("La prime " %+% prime$nom %+% " doit être renseignée soit pour sa catagorie statutaire (prime$categorie) soit par une expression régulière sur libellé (prime$expr.reg).")
        
      }
    }
  },
    "La détection des incompatibilités statutaires n'a pas pu être réalisée. ")
  
  essayer({   K <- "codes." %+% tolower(prime$nom)
    
    assign(K, list(K = unique(Lignes_A$Code)))
      
    L <- length(get(K))
      
    if (L == 0) {
      cat("Il n'a pas été possible d'identifier", 
          prime$nom,
          "par méthode heuristique. Renseigner les codes de paye correspondants dans l'interface graphique. ")
 
      résultat.manquant <- TRUE

    }
    }, "Pas de génération des codes " %+% prime$nom %+% ". ")
  
  env <- environment()
 
  if (sauvegarder.bases.analyse) { 
    sauvebase("A.non.cat", prime$nom %+% ".non.cat" %+% paste0("", prime$categorie, collapse = ""), prime$dossier, env)
    sauvebase("A.non.tit", prime$nom %+% ".non.tit", prime$dossier, env)
    if (! is.null(prime$indice)) {
      sauvebase("lignes.indice.anormal", prime$nom %+% ".indice.anormal", prime$dossier, env)
    }
 }
  
  list(Paye = Paie_A, Lignes = Lignes_A, K = get(K), manquant = résultat.manquant)
}

#' Teste les primes et indemnités   
#' 
#' Analyse les contraintes relatives aux non titulaires, à la categorie statutaire, au grade, à l'indice, aux cumuls avec d'autres indemnités.   
#' 
#' @param prime     Prime au format liste comportant les arguments :
#'   \describe{
#'   \item{nom}{Nom de la prime en majuscules. Une expression régulière en décrivant le libellé doit être enregistrée dans l'espace global sous le nom : expression.rég.nom}
#'   \item{categorie}{"A", "B", "C" ou tout vecteur d'une à deux lettres comprises dans ces trois valeurs. Décrit les categories statutaires auxquelles la prime est attribuable.}
#'   \item{restreint_fonctionnaire}{Booléen. Par défaut FALSE. Préciser TRUE si la prime est uniquement attrubuable aux fonctionnaires. Dans certains cas (mais pas pour tous), la prime peut aussi être attribuable aux non-titulaires, sous réserve d'un acte réglementaire interne à l'organisme.}
#'   \item{dossier}{Chaîne de caractères. Sous-dossier du dossier Bases dans lequel le fichier auxiliaire CSV doit être généré. Par exemple : "Reglementation".}
#'   \item{expr.reg.}{Chaîne de caractères. Expression régulière filtrant sur champs \code{Grade}, décriuvant une contrainte limitant l'accès de la prime à un certain sous-ensemble de grades.}
#'   \item{indice}{Liste. Couple d'un caractère "+" ou "-" et d'un entier, ou triplet correspondant au couple augmenté d'un vecteur d'une ou deux lettres statutaires. Exemple : list("+", 350, c("A","B)). La liste décrit un critère limitatif pour la prime : 
#'   elle ne peut être attribuée qu'aux indices supérieurs ("+") ou inférieurs ("-") au nombre donné en deuxième position pour les fonctionnaires de categorie précisée en troisième prosition.}
#'   \item{NAS}{Si vaut "non", la prime est incompatible avec le logement par nécessité absolue de service (NAS). Si vaut un nombre, la prime doit être inférieure à ce seuil pour bénéficier d'un logement par NAS.}}
#' @param prime_B     Prime au format liste comportant les mêmes types d'arguments. Les cumuls de \code{prime} et de \code{prime_B} seront analysés.   
#' @param Paie_I    Base data.table des indemnités comportant les colonnes :
#' \itemize{
#'   \item{Nom} 
#'   \item{Prenom}
#'   \item{Matricule} 
#'   \item{Annee} 
#'   \item{Mois} 
#'   \item{Debut}
#'   \item{Fin}
#'   \item{Code}
#'   \item{Libelle}
#'   \item{Montant}
#'   \item{Type}
#'   \item{Emploi}
#'   \item{Grade} 
#'   \item{Indice}
#'   \item{Statut}
#'   \item{Categorie}}
#' @param verbeux   [FALSE] Le résultat des tableaux "non titulaires" et "categories" n'est affiché que si \code{verbeux} vaut \code{TRUE}
#' @return  Liste constituée de :
#'  \describe{
#'   \item{Paye}{La base data.table de paye correspondant à la prime en premier argument, toutes primes confondues.}
#'   \item{Lignes}{Les lignes de paye correspondant à la prime en premier argument seulement.}
#'   \item{K}{Codes de paye correspondant à la prime.}
#'   \item{manquant}{Booléen. TRUE si absence de résultat, FALSE sinon.}}
#'   \item{NAS}{Base des cumuls irréguliers de \code{prime} et d'un logement par NAS, si \code{NAS} vaut "non", sinon NULL}
#' @note  Sauvegarde deux fichiers dans le sous-dossier prime$dossier : 
#' \itemize{
#' {prime$nom.non.tit.csv} {Recense les attributaires non titulaires}
#' {prime$nom.cat.A (ou AB ou B ou BC...)} {Recense les attributaires de categorie A, B, C ou toute combinaison de ces lettres.}
#' }   
#' @export

test_prime <- function(prime, prime_B, Paie_I = NULL, Paie_B = NULL, Lignes_B = NULL, verbeux = FALSE, echo = TRUE) {

if (is.null(prime_B)) return(NULL);  
  
if (is.null(Paie_B) || is.null(Lignes_B)) {
  
  res      <- analyser(prime_B, Paie_I, verbeux, echo)
  Paie_B   <- res$Paye
  Lignes_B <- res$Lignes
}
  
# Initialisation
  
Paie_A <- NULL 
Lignes_A <- NULL 
personnels.A.B <- NULL 
A.non.cat <- NULL 
lignes.indice.anormal <- NULL
nombre.mois.cumuls <- 0 
nombre.agents.cumulant.A.B <- 0
ident_prime <- prime$nom

nombre.agents.cumulant.A.B <- 0

résultat <- analyser(prime, Paie_I, verbeux, echo)

Paie_A   <- résultat$Paye
Lignes_A <- résultat$Lignes
K        <- résultat$K
résultat.manquant <- résultat$manquant

essayer({ if (! is.null(Paie_B) && ! résultat.manquant) {
  
  # on exclut les rappels !
     
    indic_B <- "indic_"  %+% prime_B$nom
    
    if (exists(indic_B, envir = .GlobalEnv)) {
      
      INDIC_B <- get(indic_B, envir = .GlobalEnv)
      
      NAMES <- names(Paie_B)
      
      if (! indic_B %chin% NAMES && "indic" %chin% NAMES) setnames(Paie_B, "indic", indic_B)
      
        periode.fusion <- merge(unique(Paie_A[indic == TRUE]),
                                unique(Paie_B[INDIC_B == TRUE]),
                                by = c("Nom", "Prenom", "Matricule",
                                       "Annee", "Mois", "Emploi", "Grade",
                                       "Indice", "Statut",
                                       "Categorie"))[ , .(Matricule, Annee, Mois)]
      
        periode.fusion <- unique(periode.fusion)
        
        A_ <- merge(unique(Paie_A), periode.fusion, by = c("Matricule", "Annee", "Mois"))
        B_ <- merge(unique(Paie_B), periode.fusion, by = c("Matricule", "Annee", "Mois"))
        B_$indic <- A_$indic
        
        personnels.A.B <- B_[indic == TRUE | INDIC_B  == TRUE
                            ][ , indic := NULL
                            ][ , (indic_B) := NULL]
        
        nombre.mois.cumuls <- uniqueN(personnels.A.B[ , .(Matricule, Annee, Mois)], by = NULL)
        
        nombre.agents.cumulant.A.B <- uniqueN(personnels.A.B$Matricule)
        
        setkey(personnels.A.B, Matricule,Annee, Mois)

    }
  
}
 },
   "La détection des cumuls d'indemnités " %+% ident_prime %+% " et " %+% prime_B$nom %+% " n'a pas pu être réalisée. ")

if (exists("nombre.agents.cumulant.A.B") && nombre.agents.cumulant.A.B > 0) {
  essayer(label = "Tableau cumuls", {
    
    L <- length(K)
      
    if (L < 6) {
      
      print(Tableau(c("Codes " %+% ident_prime,
                      "Agents cumulant " %+% ident_prime %+% " et " %+% prime_B$nom),
              sep.milliers = "",
              paste(unlist(K), collapse = " "),
              nombre.agents.cumulant.A.B))
      
    } else {
      
      cat("Codes " %+% ident_prime %+% " : ", paste(unlist(K), collapse = " "))
      
    }
  }, "Le tableau des cumuls ne peut pas être généré. ")
}

if(sauvegarder.bases.analyse) {
  sauvebase("personnels.A.B",
  "personnels." %+% tolower(ident_prime) %+% "." %+% tolower(prime_B$nom),
                             prime$dossier, environment())
}

indic <<- "indic_"  %+% prime$nom
indic_B <<- "indic_"  %+% prime_B$nom
assign(indic_B, NULL, envir = .GlobalEnv)

Lignes_A[ , (indic) := TRUE]
Lignes_B[ , (indic_B) := TRUE]

beneficiaires.A <- merge(unique(Lignes_A),
                         unique(Lignes_B),
                         all = TRUE,
                         by = c("Nom", "Prenom", "Matricule", "Annee", "Mois", "Debut",
                                "Fin", "Grade", "Emploi", "Temps.de.travail", "Indice",
                                "Categorie", "Statut", "Type", "Montant"))

beneficiaires.A[ , Régime := if (all(is.na(get(indic)))) {
                                if (any(get(indic_B))) "I" else NA
                             } else {
                                if (all(is.na(get(indic_B)))) "P" else "C"
                             },
                   by = .(Matricule, Annee, Mois)
               ][ , (indic) := NULL
               ][ , (indic_B) := NULL]

matricules.A <- unique(Lignes_A$Matricule)

beneficiaires.A <- beneficiaires.A[Matricule %chin% matricules.A,
                                       .(Agrégat = sum(Montant, na.rm = TRUE),
                                         c = uniqueN(Mois[Régime == "C"]),
                                         i = uniqueN(Mois[Régime == "I"]),
                                         p = uniqueN(Mois[Régime == "P"]),
                                         nb.mois = uniqueN(Mois),
                                         Grade = Grade[1],
                                         Mois,
                                         Régime),
                                       keyby= .(Matricule, Annee),
                                  ]
if (nrow(beneficiaires.A) > 0) {
    beneficiaires.A <- beneficiaires.A[ ,
                                        .(Agrégat,
                                            c,
                                            i,
                                            p,
                                            Grade,
                                            nb.mois,

                                            Régime = {

                                            if (c != 0 & p != 0 & i != 0) {
                                                prime_B$nom %+% " " %+% i %+% " mois-" %+% ident_prime %+% " " %+% p %+% " mois" %+% "-Cumul " %+% c %+% " mois"
                                            } else ""
                                        }),
                                keyby = .(Matricule, Annee)]

    beneficiaires.A <- unique(beneficiaires.A)

    beneficiaires.A.Variation <- beneficiaires.A[ ,
                                                    {
                                                    L <- length(Annee)
                                                    q <- Agrégat[L]/Agrégat[1] * nb.mois[1]/nb.mois[L]
                                                    .(Annees = paste(Annee, collapse = ", "),
                                                        `Variation (%)` = round((q - 1) * 100, 1),
                                                        `Moyenne géométrique annuelle(%)` = round((q^(1/(L - 1)) - 1) * 100, 1))
                                                    },
                                                by = "Matricule"]

    beneficiaires.A.Variation <- beneficiaires.A.Variation[`Variation (%)` != 0.00]
} else {
   beneficiaires.A <- NULL
   beneficiaires.A.Variation <- NULL
}

cumul.prime.NAS <- NULL

env <- environment()

if (sauvegarder.bases.analyse) {
    sauvebase("Lignes_A", "Lignes_A." %+% ident_prime %+% "." %+% prime_B$nom, "Remunerations", env)
    sauvebase("Lignes_B", "Lignes_B." %+% ident_prime %+% "." %+% prime_B$nom, "Remunerations", env)
    sauvebase("beneficiaires.A", "beneficiaires." %+% ident_prime %+% "." %+% prime_B$nom, "Remunerations", env)
    sauvebase("beneficiaires.A.Variation", "beneficiaires." %+% ident_prime %+% "." %+% prime_B$nom %+% ".Variation", "Remunerations", env)
}


list(Paie = Paie_A, 
     Lignes = Lignes_A, 
     Paie_B = Paie_B,
     Lignes_B = Lignes_B,
     personnels = personnels.A.B, 
     non.cat = A.non.cat, 
     mois = nombre.mois.cumuls, 
     N = nombre.agents.cumulant.A.B,
     cumuls = beneficiaires.A,
     variations = beneficiaires.A.Variation,
     matricules = matricules.A,
     indices = lignes.indice.anormal,
     manquant = résultat.manquant)
}
