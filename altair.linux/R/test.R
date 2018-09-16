# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2018
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

# prime$nom
# prime$catégorie
# prime$restreint_fonctionnaire
# si non null Paie_B doit avoir indic_B

#' Quotité administrative
#' @param quotité  quotité formelle (Temps.de.travail / 100)
#' @export
adm <- function(quotité) ifelse(quotité == 0.8,  6/7, ifelse (quotité == 0.9,  32/35, quotité))


#' Sauvegarde une base dans le dossier des bases
#' 
#' Sauvegarde paramétrée par environnement
#' 
#' @param x  Objet à sauvegarder (vecteur de caractères)    
#' @param y  Nom du fichier de sauvegarde CSV.   
#' @param z  Nom du sous-dossier du dossier des bases.   
#' @param env  Environnement

sauvebase <- function(x, y, z, env) {
  Sauv.base(file.path(chemin.dossier.bases, z),
            x,
            y,
            FALSE,
            sep = ";",
            dec = ",",
            environment = env)
  
}

#' Affichage du tableau des cumuls de primes
#' 
#' @param résultat  Résultat retourné par la fonction \link{test_prime}
#' @examples
#' tableau_cumuls(test_prime(prime_IAT, prime_B = prime_IFTS, Paie_I, verbeux = FALSE))
#' 
#' |Matricule |Année |       Grade          |                  Régime                     |
#' |----------|------|----------------------|---------------------------------------------|
#' |010843    |2009  |ANIMATEUR TERRITORIAL |   IFTS 1 mois-IAT 10 mois-Cumul 1 mois      |
#' |010843    |2010  |ANIMATEUR TERRITORIAL |   IFTS 11 mois-IAT 0 mois-Cumul 1 mois      |
#' |010854    |2009  |REDACTEUR TERRITORIAL |   IFTS 9 mois-IAT 2 mois-Cumul 1 mois       | 
#' 
#' @export

tableau_cumuls <- function(résultat) {
  
  if (! is.null(résultat) && ! is.null(résultat$cumuls) && nrow(résultat$cumuls[c != 0]) > 0) {
    kable(résultat$cumuls[c != 0, .(Matricule, Année, Grade, Régime)])
  } else cat("Pas de cumuls.")
}

#' Affichage du tableau des cumuls de primes et du logement par NAS
#' 
#' @param résultat  Résultat retourné par la fonction \link{test_prime}
#' @examples
#' tableau_NAS(test_prime(prime_IFTS, Paie_I, verbeux = FALSE, NAS = "non"))
#' 
#' |Matricule |Année | Mois |       Grade            |       Emploi            |    Montant |
#' |----------|------|-------------------------------|-------------------------|------------|
#' |010843    |2009  |   8  |  ATTACHE PRINCIPAL     |   CHEF DE DIVISION      |    785,25  |
#' |010843    |2009  |   9  |  ATTACHE PRINCIPAL     |   CHEF DE DIVISION      |    785,25  |
#' |010854    |2009  |   9  |  ADMINISTRATEUR        |   CHEF DE SERVICE       |    995,20  |
#' 
#' @export

tableau_NAS <- function(résultat) {
  
  if (! is.null(résultat) && ! is.null(résultat$NAS) && nrow(résultat$NAS) > 0) {
    kable(résultat$NAS[ , .(Matricule, Année, Mois, Grade, Emploi, Montant)])
  } else cat("Pas de cumuls prime-NAS.")
}

#' Affichage du tableau des agrégats des primes A et B, pour chaque année de période 
#' 
#' @param résultat  Résultat retourné par la fonction \link{test_prime}
#' @param verbeux   [FALSE] Le résultat n'est affiché que si  \code{verbeux} vaut  \code{TRUE}
#' @examples 
#' agrégat_annuel(test_prime(prime_IAT, prime_B = prime_IFTS, Paie_I, verbeux = FALSE), verbeux = TRUE)
#' @export

agrégat_annuel<- function(résultat, verbeux) {
  
  beneficiaires <- résultat$cumuls[, .(Matricule, Année, nb.mois, Grade, Agrégat)]
  
  if (verbeux) {
    
    if (nrow(beneficiaires)) {
      
      beneficiaires$Agrégat <- formatC(beneficiaires$Agrégat, big.mark = " ", format="fg")
      
      kable(beneficiaires, align = 'r', row.names = FALSE)
      
    } else {
      cat("\nAucun bénéficiaire détecté.\n")
    }
  }
  
}

#' Affichage du tableau des variations des agrégats des primes A et B sur l'ensemble de la période
#' 
#' @param résultat  Résultat retourné par la fonction \link{test_prime}
#' @param verbeux   [FALSE] Le résultat n'est affiché que si  \code{verbeux} vaut  \code{TRUE}
#' @export

évolution_agrégat <- function(résultat, verbeux) {
  
  if (verbeux) {
    
    if (nrow(résultat$variations)) {
      
      kable(résultat$variations, align = 'r', row.names = FALSE)
      
    } else {
      cat("\nAucun tableau de variation.\n")
    }
  }
}

#' Analyse des contraintes principales associées à une indemnité 
#' @param prime     Prime au format liste comportant les arguments :
#'   \describe{
#'   \item{nom}{Nom de la prime en majuscules. Une expression régulière en décrivant le libellé doit être enregistrée dans l'espace global sous le nom : expression.rég.nom}
#'   \item{catégorie}{"A", "B", "C" ou tout vecteur d'une à deux lettres comprises dans ces trois valeurs. Décrit les catégories statutaires auxquelles la prime est attribuable.}
#'   \item{restreint_fonctionnaire}{Booléen. Par défaut FALSE. Préciser TRUE si la prime est uniquement attrubuable aux fonctionnaires. Dans certains cas (mais pas pour tous), la prime peut aussi être attribuable aux non-titulaires, sous réserve d'un acte réglementaire interne à l'organisme.}
#'   \item{dossier}{Chaîne de caractères. Sous-dossier du dossier Bases dans lequel le fichier auxiliaire CSV doit être généré. Par exemple : "Reglementation".}
#'   \item{expr.rég.}{Chaîne de caractères. Expression régulière filtrant sur champs \code{Grade}, décriuvant une contrainte limitant l'accès de la prime à un certain sous-ensemble de grades.}
#'   \item{indice}{Liste. Couple d'un caractère "+" ou "-" et d'un entier, ou triplet correspondant au couple augmenté d'un vecteur d'une ou deux lettres statutaires. Exemple : list("+", 350, c("A","B)). La liste décrit un critère limitatif pour la prime : 
#'   elle ne peut être attribuée qu'aux indices supérieurs ("+") ou inférieurs ("-") au nombre donné en deuxième position pour les fonctionnaires de catégorie précisée en troisième prosition.}}
#'   
#' @param Paie_I    Base data.table des indemnités comportant les colonnes :
#' \itemize{
#'   \item{Nom} 
#'   \item{Prénom}
#'   \item{Matricule} 
#'   \item{Année} 
#'   \item{Mois} 
#'   \item{Début}
#'   \item{Fin}
#'   \item{Code}
#'   \item{Libellé}
#'   \item{Montant}
#'   \item{Type}
#'   \item{Emploi}
#'   \item{Grade} 
#'   \item{Indice}
#'   \item{Statut}
#'   \item{Catégorie}}
#' @param verbeux   [FALSE] Le résultat des tableaux "non titulaires" et "catégories" n'est affiché que si \code{verbeux} vaut \code{TRUE}
#' @return  Liste constituée de :
#'  \describe{
#'   \item{Paye}{La base data.table de paye correspondant à la prime en premier argument, toutes primes confondues.}
#'   \item{Lignes}{Les lignes de paye correspondant à la prime en premier argument seulement.}
#'   \item{K}{Codes de paye correspondant à la prime.}
#'   \item{manquant}{Booléen. TRUE si absence de résultat, FALSE sinon.}}
#' @note  Sauvegarde deux fichiers dans le sous-dossier prime$dossier : 
#' \itemize{
#' {prime$nom.non.tit.csv} {Recense les attributaires non titulaires}
#' {prime$nom.cat.A (ou AB ou B ou BC...)} {Recense les attributaires de catégorie A, B, C ou toute combinaison de ces lettres.}
#' }   
#' @export

analyser <- function(prime, Paie_I, verbeux) {

  Paie_A <- NULL
  Lignes_A <- NULL
  résultat.manquant <- FALSE
  lignes.indice.anormal <- NULL
  
  essayer({  Paie_A   <- filtrer_Paie(prime$nom, 
                             portée = "Mois",
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
        
        lignes.indice.anormal <-lignes.indice.anormal[Catégorie %chin% prime$indice[3]]
          
      } 
          
      nr <- nrow(lignes.indice.anormal)
      
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
      
    }, 
      "La vérification de la borne indiciaire n'a pas pu êre réalisée. ")
  }
  
  # Questions de compatibilité statutaire
  
  essayer({ if (prime$restreint_fonctionnaire) {
      
      A.non.tit  <- Lignes_A[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]
      
      if ((N.A.non.tit <- uniqueN(A.non.tit$Matricule)) > 0) {
        
        cat(N.A.non.tit,
            "attributaire" %s% N.A.non.tit,
            prime$nom,
            "sont des non-titulaires. ")
        
        if (verbeux) print(kable(A.non.tit,
                                 align = 'r',
                                 row.names = FALSE))
        
      } else {
        
        cat("Tous les attributaires de",
            prime$nom,
            "sont titulaires ou stagiaires. ")
      }
    }
    
    if (!is.null(prime$catégorie)){
      
      if (! is.null(prime$expr.rég)) {
        
        A.non.cat <- Lignes_A[! Catégorie %chin% prime$catégorie 
                              | ! grepl(prime$expr.rég, Grade, ignore.case = TRUE, perl = TRUE)] 
        
      } else {
        
        A.non.cat <- Lignes_A[! Catégorie %chin% prime$catégorie]
      }
      
      if ((N.A.non.cat <<- uniqueN(A.non.cat$Matricule)) > 0) {
        
        cat(N.A.non.cat, 
            "attributaires de",
            prime$nom,
            "ne sont pas identifiés en catégorie",
            prime$catégorie,
            ". ")
        
        if (verbeux)  print(kable(A.non.cat, align = 'r', row.names = FALSE))
        
      } else {
        
        cat("Tous les attributaires de",
            prime$nom,
            "sont identifiés en catégorie",
            prime$catégorie, ". ")
      }
      
    } else {
      
      if (! is.null(prime$expr.rég)) {
        
        A.non.cat <- Lignes_A[! grepl(prime$expr.rég, Grade, ignore.case = TRUE, perl = TRUE)] 
        
        if ((N.A.non.cat <<- uniqueN(A.non.cat$Matricule)) > 0) {
          
          cat(N.A.non.cat,
              "attributaires de",
              prime$nom,
              "ne sont pas identifiés comme relevant de grades conformes. ")
          
          if (verbeux)  print(kable(A.non.cat, align = 'r', row.names = FALSE))
          
        }
        
      } else {
       
        cat("La détection des incompatibilités statutaires n'a pas pu être réalisée. ")
        stop("La prime " %+% prime$nom %+% " doit être renseignée soit pour sa catagorie statutaire (prime$catégorie) soit par une expression régulière sur libellé (prime$expr.rég).")
        
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
    sauvebase("A.non.cat", prime$nom %+% ".non.cat" %+% paste0("", prime$catégorie, collapse = ""), prime$dossier, env)
    sauvebase("A.non.tit", prime$nom %+% ".non.tit", prime$dossier, env)
    if (! is.null(prime$indice)) {
      sauvebase("lignes.indice.anormal", prime$nom %+% ".indice.anormal", prime$dossier, env)
    }
 }
  
  list(Paye = Paie_A, Lignes = Lignes_A, K = get(K), manquant = résultat.manquant)
}

#' Teste les primes et indemnités   
#' 
#' Analyse les contraintes relatives aux non titulaires, à la catégorie statutaire, au grade, à l'indice, aux cumuls avec d'autres indemnités.   
#' 
#' @param prime     Prime au format liste comportant les arguments :
#'   \describe{
#'   \item{nom}{Nom de la prime en majuscules. Une expression régulière en décrivant le libellé doit être enregistrée dans l'espace global sous le nom : expression.rég.nom}
#'   \item{catégorie}{"A", "B", "C" ou tout vecteur d'une à deux lettres comprises dans ces trois valeurs. Décrit les catégories statutaires auxquelles la prime est attribuable.}
#'   \item{restreint_fonctionnaire}{Booléen. Par défaut FALSE. Préciser TRUE si la prime est uniquement attrubuable aux fonctionnaires. Dans certains cas (mais pas pour tous), la prime peut aussi être attribuable aux non-titulaires, sous réserve d'un acte réglementaire interne à l'organisme.}
#'   \item{dossier}{Chaîne de caractères. Sous-dossier du dossier Bases dans lequel le fichier auxiliaire CSV doit être généré. Par exemple : "Reglementation".}
#'   \item{expr.rég.}{Chaîne de caractères. Expression régulière filtrant sur champs \code{Grade}, décriuvant une contrainte limitant l'accès de la prime à un certain sous-ensemble de grades.}
#'   \item{indice}{Liste. Couple d'un caractère "+" ou "-" et d'un entier, ou triplet correspondant au couple augmenté d'un vecteur d'une ou deux lettres statutaires. Exemple : list("+", 350, c("A","B)). La liste décrit un critère limitatif pour la prime : 
#'   elle ne peut être attribuée qu'aux indices supérieurs ("+") ou inférieurs ("-") au nombre donné en deuxième position pour les fonctionnaires de catégorie précisée en troisième prosition.}
#'   \item{NAS}{Si vaut "non", la prime est incompatible avec le logement par nécessité absolue de service (NAS). Si vaut un nombre, la prime doit être inférieure à ce seuil pour bénéficier d'un logement par NAS.}}
#' @param prime_B     Prime au format liste comportant les mêmes types d'arguments. Les cumuls de \code{prime} et de \code{prime_B} seront analysés.   
#' @param Paie_I    Base data.table des indemnités comportant les colonnes :
#' \itemize{
#'   \item{Nom} 
#'   \item{Prénom}
#'   \item{Matricule} 
#'   \item{Année} 
#'   \item{Mois} 
#'   \item{Début}
#'   \item{Fin}
#'   \item{Code}
#'   \item{Libellé}
#'   \item{Montant}
#'   \item{Type}
#'   \item{Emploi}
#'   \item{Grade} 
#'   \item{Indice}
#'   \item{Statut}
#'   \item{Catégorie}}
#' @param verbeux   [FALSE] Le résultat des tableaux "non titulaires" et "catégories" n'est affiché que si \code{verbeux} vaut \code{TRUE}
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
#' {prime$nom.cat.A (ou AB ou B ou BC...)} {Recense les attributaires de catégorie A, B, C ou toute combinaison de ces lettres.}
#' }   
#' @export

test_prime <- function(prime, prime_B, Paie_I = NULL, Paie_B = NULL, Lignes_B = NULL, verbeux = FALSE) {

if (is.null(Paie_B) || is.null(Lignes_B)) {
  
  if (is.null(prime_B)) stop("prime_B ne doit pas être NULL si Paie_B ou Lignes_B est NULL.")
  
  res      <- analyser(prime_B, Paie_I, verbeux)
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

résultat <- analyser(prime, Paie_I, verbeux)

Paie_A   <- résultat$Paye
Lignes_A <- résultat$Lignes
K        <- résultat$K
résultat.manquant <- résultat$manquant

essayer({ if (! is.null(Paie_B) && ! résultat.manquant) {
  
  # on exclut les rappels !
  
    indic_B <- "indic_"  %+% prime_B$nom
    
    NAMES <- names(Paie_B)
    
    if (! indic_B %chin% NAMES && "indic" %chin% NAMES) setnames(Paie_B, "indic", indic_B)
    
    période.fusion <- merge(Paie_A[indic == TRUE],
                            Paie_B[get(indic_B) == TRUE],
                            by = c("Nom", "Prénom", "Matricule",
                                   "Année", "Mois", "Emploi", "Grade",
                                   "Indice", "Statut",
                                   "Catégorie"))[ , .(Matricule, Année, Mois)]
  
    période.fusion <- unique(période.fusion)
    
    A_ <- merge(Paie_A, période.fusion)
    B_ <- merge(Paie_B, période.fusion)
    B_$indic <- A_$indic
    
    personnels.A.B <- B_[indic == TRUE | get(indic_B) == TRUE
                        ][ , indic := NULL
                        ][ , indic_B := NULL
                             , with = FALSE]
    
    nombre.mois.cumuls <- uniqueN(personnels.A.B[ , .(Matricule, Année, Mois)], by = NULL)
    
    nombre.agents.cumulant.A.B <- uniqueN(personnels.A.B$Matricule)
    
    setkey(personnels.A.B, Matricule,Année, Mois)
  
}
 },
   "La détection des cumuls d'indemnités " %+% ident_prime %+% " et " %+% prime_B$nom %+% " n'a pas pu être réalisée. ")


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

if(sauvegarder.bases.analyse) {
  sauvebase("personnels.A.B", "personnels." %+% tolower(ident_prime) %+% "." %+% tolower(prime_B$nom), prime$dossier, environment())
}

indic <- "indic_"  %+% prime$nom
indic_B <- "indic_"  %+% prime_B$nom

Lignes_A[ , indic := TRUE, with = FALSE]
Lignes_B[ , indic_B := TRUE, with = FALSE]

beneficiaires.A <- merge(Lignes_A, Lignes_B, all = TRUE)

beneficiaires.A[ , Régime := if (all(is.na(get(indic)))) { if (any(get(indic_B))) "I" else NA } else { if (all(is.na(get(indic_B)))) "P" else "C" },
                   by = .(Matricule, Année, Mois)][ , indic := NULL, with = FALSE][ , indic_B := NULL, with = FALSE]

matricules.A <- unique(Lignes_A$Matricule)

beneficiaires.A <- beneficiaires.A[Matricule %chin% matricules.A,
                                       .(Agrégat = sum(Montant, na.rm = TRUE),
                                         c = uniqueN(Mois[Régime == "C"]),
                                         nb.mois = uniqueN(Mois),
                                         Grade = Grade[1],
                                         Mois,
                                         Régime),
                                       keyby= .(Matricule, Année),
                                  ][ , 
                                      .(Agrégat,
                                        c,
                                        Grade,
                                        nb.mois,
                                        Régime = {
                                       
                                       prime_B$nom %+% " " %+% uniqueN(Mois[Régime == "I"]) %+% " mois-" %+% ident_prime %+% " " %+% uniqueN(Mois[Régime == "P"]) %+% " mois" %+% "-Cumul " %+% c %+% " mois"
                                     }),
                                      keyby= .(Matricule, Année)]

beneficiaires.A <- unique(beneficiaires.A)

beneficiaires.A.Variation <- beneficiaires.A[ , 
                                                { 
                                                  L <- length(Année)
                                                  q <- Agrégat[L]/Agrégat[1] * nb.mois[1]/nb.mois[L]                   
                                                  .(Années = paste(Année, collapse = ", "), 
                                                    `Variation (%)` = round((q - 1) * 100, 1),
                                                    `Moyenne géométrique annuelle(%)` = round((q^(1/(L - 1)) - 1) * 100, 1)) 
                                                }, by = "Matricule"]

beneficiaires.A.Variation <- beneficiaires.A.Variation[`Variation (%)` != 0.00]

cumul.prime.NAS <- NULL

if ((! is.null(prime$NAS) && prime$NAS == "non") || (! is.null(prime_B$NAS) && prime_B$NAS == "non")) {
  
  if (! is.null(base.logements)) {
    essayer({ if (! is.null(prime$NAS) && prime$NAS == "non" && is.null(prime_B$NAS)) {
      
      Lignes_C <-  Lignes_A 
      prime_NAS <- prime$nom
      
    } else {
        if (is.null(prime$NAS)) {
          
          Lignes_C <- Lignes_B 
          prime_NAS <- prime_B$nom
          
        } else { 
            
          if (prime$NAS == "non" && prime_B$NAS == "non") {
            
            Lignes_C <- merge(Lignes_A, Lignes_B, all = TRUE)
            prime_NAS <- prime$nom %+% "-" %+% prime_B$nom
            
          }
        }
    }
      
    cumul.prime.NAS <- merge(unique(base.logements[Logement == "NAS", .(Matricule, Année, Mois)]),
                             Lignes_C[ ,  .(Matricule, Nom, Prénom, Statut, Grade, Emploi, Année, Mois, Code, Libellé, Montant)])
    
    }, "Le test des cumuls de " %+% prime$nom %+% " ou " %+% prime_B$nom %+% " et du logement par NAS n'a pas pu être réalisé.")
  }
}

env <- environment()

if (sauvegarder.bases.analyse) {
    sauvebase("beneficiaires.A", "beneficiaires." %+% ident_prime %+% "." %+% prime_B$nom, "Remunerations", env)
    sauvebase("beneficiaires.A.Variation", "beneficiaires." %+% ident_prime %+% "." %+% prime_B$nom %+% ".Variation", "Remunerations", env)
    if (! is.null(cumul.prime.NAS)) sauvebase("cumul.prime.NAS", "cumul." %+% prime_NAS %+% ".NAS", "Reglementation", env)
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
     manquant = résultat.manquant,
     NAS = cumul.prime.NAS)
}

#' Teste les logements par NAS
#' 
#' @param avantage Vecteur de caractères indiquant le type de logement (actuellement seul "NAS" est actif)
#' @param Paie Base de Paye principale comportant les variables \code{Matricule, Année, Mois, Statut, Grade, Emploi, Type, Code, Libellé, Montant}
#' @param logements La base de logements facultative importée par l'onglet Extra de l'interface graphique 
#' @return base de type \code{data.table} comportant les enregistrements identifiés comme problématiques.  
#' @export

test_avn <- function(avantage, Paie, logements = NULL) {
  
  val <- codes[type == avantage, valeur]
  
  essayer({  if (! is.na(val)) {
      Paie_AV <- Paie[Code %chin% val, .(Matricule, Année, Mois, Statut, Grade, Emploi, Type, Code, Libellé, Montant)]
  } else {
      Paie_AV <- Paie[grepl(codes[type == avantage, expression], Libellé, ignore.case = TRUE, perl = TRUE), .(Matricule, Année, Mois, Statut, Grade, Emploi, Code, Type, Libellé, Montant)]
  }
  
  if ((n1 <- nrow(Paie_AV[Type == "AV"])) < (n2 <- nrow(Paie_AV))) {
    newline()
    cat("Tous les avantages en nature pour logement par NAS (", n2, " lignes) ne sont pas déclarés comme type \"Avantage en nature\", soit ", n1, " lignes."  )
    newline()
  }
  
  NAS.non.importes      <- NULL
  NAS.non.declares.paye <- NULL
  newline()  
  
  if (is.null(logements) || is.null(logements[Logement == "NAS"]) || nrow(logements[Logement == "NAS"]) == 0) {
    
    logements <- Paie_AV[ , .(Matricule, Année, Mois)]   # on prend ceux de la base de paye à défaut de déclarations explicites dans fichier auxiliaire importé
    logements[ , Logement := "NAS"]
    cat("En l'absence de fichier auxiliaire importé des logements par NAS, il a été trouvé ", nrow(logements), " lignes d'avantage en nature de logement, supposé par NAS.")

  } else {
    
    if (is.null(Paie_AV) || nrow(Paie_AV) == 0)  {  # il y a donc des logements par NAS non déclarés en base de paye...
      
      cat("Aucun des logements par NAS déclarés dans le fichier auxiliaire importé n'est mentionné comme avantage en nature (Type == \"AV\") en base de paye. ")
      cat("Il peut en résulter des anomalies dans les déclarations fiscales ou de cotisations sociales. ")

    } else {
      
      cat("Il existe à la fois des avantages en nature déclarés en base de paye (", nrow(Paie_AV), " lignes) et un fichier de déclaration de logements par NAS importé (", n1, " lignes. ")
      
      logements <- merge(Paie_AV, base.logements[Logement == "NAS"], by = c("Matricule", "Année", "Mois"), all = TRUE)
      NAS.non.importes <- logements[is.na(Logement)]
      NAS.non.declares.paye <- logements[! is.na(Logement) & is.na(Code)]
    }      
  }
  }, "Il n'a pas été possible d'extraire les logements par NAS déclarés en base de paye. ")

  newline()
  
  env <- environment()
  
  if (sauvegarder.bases.analyse) {
    sauvebase("NAS.non.importes", "NAS.non.importes", "Reglementation", env)
    sauvebase("NAS.non.declares.paye", "NAS.non.declares.paye", "Reglementation", env)
  }

  if (! is.null(logements) && nrow(logements) > 0) {
    
    message("Détection d'avantages en nature liés à un logement par NAS.")
    return(logements[, .(Matricule, Année, Mois, Logement)])
    
  } else {
    
    message("Pas de détection d'avantage en nature lié à un logement par NAS.")
    return(NULL)
  }
}

#' Teste les plafonds d'une prime
#' 
#' @param plafonds Base \code{data.table} comportant les colonnes caractères \code{Grade, Groupe} et \code{Logement} suivies de la colonne numérique \code{Plafond}. Logement doit contenir le codage \code{NAS} pour les personnels logés par nécessité absolue de service.    
#' @param Lignes Lignes de paye limitées à des montants indemnitaires fléchés (ex: IFSE) et comportant les variables \code{Matricule, Année, Mois, Statut, Grade, Emploi, Type, Code, Libellé, Montant}
#' @param logements La base de logements importée par l'onglet Extra de l'interface graphique, comportant la variable Logement et le codage \code{NAS} pour les personnels logés par nécessité absolue de service. A défaut tous les agents sont considérés non logés.       
#' @return Liste constituée du coût des dépassements par année et d'une base de type \code{data.table} comportant les bulletins de paye comportant une ligne IFSE identifiée comme problématique.  
#' @export

test_plafonds <- function(plafonds, Lignes, logements = NULL) {
  
  essayer({ 

  if (is.null(plafonds)) {
    cat("Le fichier des plafonds de l'IFSE n'est pas importé. Le test du respect des plafonds ne peut donc pas être effectué. Le lien ci-après est inactif.")
    newline()
    return(rep.int(0, uniqueN(Lignes$Année)))
  }
  
  if (is.null(logements)) logements <- data.table(Logement = rep_len("", nrow(Lignes)))
  
  essayer({ if (ncol(logements) == 1) Paie_NAS <- NULL else {
      
       Paie_NAS <- merge(Lignes, logements, by = c("Matricule", "Année", "Mois"), all.x = TRUE)
    
       Paie_NAS <- merge(Paie_NAS[Logement == "NAS", 
                                     .(Matricule, 
                                       Année, 
                                       Mois, 
                                       Statut, 
                                       Grade, 
                                       Emploi, 
                                       Logement, 
                                       Type, 
                                       Code, 
                                       Libellé, 
                                       Montant)
                                  ][ , Logement := NULL], 
                           plafonds[Logement == "NAS"
                                  ][ , Logement := NULL],
                         by = "Grade")
    }
    
    if (ncol(logements) == 1) Paie_NO_NAS <- Lignes else {
      
       Paie_NO_NAS <- merge(Lignes, logements, by = c("Matricule", "Année", "Mois"), all.x = TRUE)
    
       Paie_NO_NAS <- merge(Paie_NO_NAS[Logement != "NAS", 
                                        .(Matricule,
                                          Année,
                                          Mois,
                                          Statut,
                                          Grade,
                                          Emploi,
                                          Logement,
                                          Type,
                                          Code,
                                          Libellé,
                                          Montant)
                                        ][ , Logement := NULL],
                            plafonds[Logement != "NAS"
                                        ][ , Logement := NULL],
                            by = "Grade")
    }
    
    newline()  
    
  }, "Il n'a pas été possible de contrôler les plafonds en base de paye. ")
  
  newline()
  
  P <- rbind(Paie_NAS, Paie_NO_NAS)
  
  dépassements <- P[Montant / adm(Temps.de.travail/100) > Plafond]
  couts.dépassements <- dépassements[ , sum(Montant, na.rm = TRUE), by = Année]
  bulletins.dépassements <- merge(P, dépassements[, .(Matricule, Année, Mois)])
  
  env <- environment()
  
  if (sauvegarder.bases.analyse)  
      sauvebase("bulletins.dépassements", "bulletins.depassements.ifse", "Reglementation", env)

  couts.dépassements
  
 }, "Les plafonds n'ont pas pu être testés.")  
  
}
