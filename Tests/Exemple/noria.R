
noria <- function() { }

if (0) {
entrants <- function(x)   {
  
  A <- setdiff(Analyse.variations[Année == x, Matricule], 
               Analyse.variations[Année == x -1, Matricule])
  
  
  B <- unique(Bulletins.paie[Année == x 
                             & Matricule %chin% A, 
                             .(Année, quotité, Matricule, Mois, Statut)], by = NULL)
  
  eqtp.agent <- B[ , sum(quotité, na.rm=TRUE)] / 12
  eqtp.fonct <- B[Statut == "TITULAIRE" | Statut == "STAGIAIRE", sum(quotité, na.rm=TRUE)] / 12
  
  list(A, eqtp.agent, eqtp.fonct)
}

sortants <- function(x)   {
  
  A <- setdiff(Analyse.variations[Année == x-1, Matricule], 
               Analyse.variations[Année == x, Matricule])
  
  B <- unique(Bulletins.paie[Année == x - 1
                             & Matricule %chin% A,
                             .(Année, quotité, Matricule, Mois, Statut)], by = NULL)
  
  eqtp.agent <- B[ , sum(quotité, na.rm=TRUE)] / 12
  eqtp.fonct <- B[Statut == "TITULAIRE" | Statut == "STAGIAIRE", sum(quotité, na.rm=TRUE)] / 12
  
  list(A, eqtp.agent, eqtp.fonct)
}


s <- list(0)
e <- list(0)
noria <- rep(0, durée.sous.revue)
remplacements <- rep(0, durée.sous.revue)

f <- function(x) {
  y <- x - début.période.sous.revue
  
  s[[y]] <<- sortants(x)
  e[[y]] <<- entrants(x)
  
  noria[y] <<- mean.default(Analyse.variations[Année == x 
                                               & Matricule %chin% e[[y]][[1]], 
                                               Montant.net.annuel.eqtp],
                            na.rm = TRUE) - mean.default(Analyse.variations[Année == x- 1 
                                                                            & Matricule %chin% s[[y]][[1]], 
                                                                            Montant.net.annuel.eqtp],
                                                         na.rm = TRUE)
  
  prettyNum(noria[y],
            big.mark = " ",
            digits = 5,
            format = "fg")
}

g <- function(x) {
  
  y <- x - début.période.sous.revue
  
  remplacements[y] <<- min(e[[y]][[2]], s[[y]][[2]], na.rm=TRUE)
  
  prettyNum(noria[y] * remplacements[y] / (masse.salariale.nette[y] * 10),
            big.mark = " ",
            digits = 3,
            format = "fg")
}



# B

if (durée.sous.revue > 1) {
  Tableau.vertical(c(étiquette.année,  "Noria EQTP (&euro;)", "En % de la MS N-1", "Remplacements EQTP", "Taux de remplacements (%)"),
                   période[2:durée.sous.revue],
                   extra = "no",
                   f,
                   g,
                   function(x) prettyNum(remplacements[x - début.période.sous.revue], 
                                         digits=0,
                                         format="f"),
                   function(x) prettyNum(remplacements[x - début.période.sous.revue] / effectifs[[as.character(x)]]["ETPT"] * 100,
                                         digits=2,
                                         format="f"))
} else {
  cat("L'effet de noria ne peut être calculé que pour des durées sous revue supérieures à un exercice.")
}


# C


f <- function(x) {
  y <- x - début.période.sous.revue
  
  noria[y] <<- sum(Analyse.variations[Année == x 
                                      & (Statut == "TITULAIRE" | Statut == "STAGIAIRE") 
                                      & Matricule %chin% e[[y]][[1]], 
                                      Montant.net.annuel.eqtp],
                   na.rm = TRUE) / e[[y]][[3]] -  sum(Analyse.variations[Année == x- 1
                                                                         & (Statut == "TITULAIRE" | Statut == "STAGIAIRE") 
                                                                         & Matricule %chin% s[[y]][[1]],
                                                                         Montant.net.annuel.eqtp],
                                                      na.rm = TRUE) / s[[y]][[3]]
  
  prettyNum(noria[y],
            big.mark = " ",
            digits = 5,
            format = "fg")
}

g <- function(x) {
  
  y <- x - début.période.sous.revue
  
  remplacements[y] <<- min(e[[y]][[3]], s[[y]][[3]], na.rm=TRUE)
  
  prettyNum(noria[y] * remplacements[y] / (masse.salariale.nette[y] * 10),
            big.mark = " ",
            digits = 3,
            format = "fg")
}

# D

if (durée.sous.revue > 1) {
  Tableau.vertical(c(étiquette.année,  "Noria EQTP (&euro;)", "En % de la  MSN N-1", "Remplacements EQTP", "Taux de remplacements (%)"),
                   période[2:length(période)],
                   extra = "no",
                   f,
                   g,
                   function(x) prettyNum(remplacements[x - début.période.sous.revue], digits=0, format="f"),
                   function(x) prettyNum(remplacements[x - début.période.sous.revue]/ effectifs[[as.character(x)]]["ETPT_fonct"] * 100, digits=2, format="f"))
} else {
  cat("L'effet de noria ne peut être calculé que pour des durées sous revue supérieures à un exercice.")
}

}



