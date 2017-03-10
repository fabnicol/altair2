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



