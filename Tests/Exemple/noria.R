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

# Modélisation : en première année sous revue, est réputé absent le Matricule ayant une quotité non uniformément nulle sur l'année, qui n'a pas de quotité positive dans les trois premiers mois
# Modélisation : en dernière année sous revue, est réputé absent le Matricule ayant une quotité non uniformément nulle sur l'année, qui n'a pas de quotité positive dans les trois derniers mois
# Sont exclus les vacataires et assistantes maternelles détectées.

noria <- function(Tableau = tableau.effectifs, Base = Analyse.variations.par.exercice, champ = "brut", filtre = Filtre_neutre) { 
  
s <- list(0)
ent <- list(0)
nor <- rep(0, durée.sous.revue)
se  <- rep(0, durée.sous.revue)
de  <- rep(0, durée.sous.revue)

tot.etpt <- as.numeric(sub(",", ".", Tableau[Effectifs == "Total ETPT/année (e)"]))[-1]

if (any(deparse(filtre) != deparse(Filtre_neutre)))
    Base <- Base[filtre() == TRUE]

if (champ == "brut") masse.salariale.unitaire <<- "Montant.brut.annuel.eqtp" else masse.salariale.unitaire <<- "Montant.net.annuel.eqtp"


entrants <- function(x)   {
  
  if (x == début.période.sous.revue) {
    
      A <- intersect(setdiff(unique(Bulletins.paie[Année == x  & Grade != "A" & Grade != "V" & quotité.moyenne != 0, Matricule]),
                        unique(Bulletins.paie[Année == x & (Mois <= 3) & quotité != 0 & Grade != "A" & Grade != "V", Matricule])),
                Analyse.variations.par.exercice[Année == x, unique(Matricule)])


  } else {
    
      A <- setdiff(unique(Base[Année == x & quotité.moyenne != 0 & Grade != "A" & Grade != "V", Matricule]), 
                   unique(Base[Année == x -1  & quotité.moyenne != 0 & Grade != "A" & Grade != "V", Matricule]))
  
  }
  
  LA <- uniqueN(A)
    
  B <- unique(Bulletins.paie[Année == x 
                             & filtre() == TRUE
                             & Matricule %chin% A, 
                             .(Année, quotité, Matricule, Mois, Statut)], by = NULL)
  
  eqtp.agent <- B[ , sum(quotité, na.rm=TRUE)] / 12
  
  list(A, eqtp.agent,  LA)
}

sortants <- function(x)   {
  
  if (x == fin.période.sous.revue) {
    
    A <- intersect(setdiff(unique(Bulletins.paie[Année == x  & Grade != "A" & Grade != "V" & quotité.moyenne != 0, Matricule]),
                      unique(Bulletins.paie[Année == x & (Mois >= 10) & quotité != 0 & Grade != "A" & Grade != "V", Matricule])),
              Analyse.variations.par.exercice[Année == x, unique(Matricule)])
    
  } else {
    
    A <- setdiff(unique(Base[Année == x  & quotité.moyenne != 0 & Grade != "A" & Grade != "V", Matricule]), 
                 unique(Base[Année == x + 1  & quotité.moyenne != 0 & Grade != "A" & Grade != "V", Matricule]))
  }
  
  LA <- uniqueN(A)
  
  B <- unique(Bulletins.paie[Année == x 
                             & filtre() == TRUE
                             & Matricule %chin% A,
                             .(Année, quotité, Matricule, Mois, Statut)], by = NULL)
  
  eqtp.agent <- B[ , sum(quotité, na.rm=TRUE)] / 12
  
  list(A, eqtp.agent, LA)
}

# GVT négatif = (masse salariale moyenne unitaire des entrants - masse salariale moyenne unitaire des sortants) x nombre sortants
# ici exprimée en rémunérations

f <- function(x) {
  y <- x - début.période.sous.revue + 1
  
  s[[y]] <<- sortants(x)
  ent[[y]] <<- entrants(x)
  nsort <- s[[y]][[3]]
  
  B1 <- Base[Année == x 
             & Matricule %chin% as.character(ent[[y]][[1]]), 
                c("quotité.moyenne", masse.salariale.unitaire), 
                  with = FALSE]
  
  B2 <- Base[Année == x
             & Matricule %chin% as.character(s[[y]][[1]]), 
               c("quotité.moyenne", masse.salariale.unitaire),
                   with = FALSE]
  
  nor[y] <<- (weighted.mean(B1[[2]],  B1[[1]], na.rm = TRUE)
              - weighted.mean(B2[[2]], B2[[1]], na.rm = TRUE)) * nsort / tot.etpt[y] 
  
  formatC(as.numeric(nor[y]),
            big.mark = " ",
            digits = 1,
            format = "f")
}


# L'effet de la variation d'effectifs (schéma d'emploi, vacances infra-annuelles incluses) est : 
#  se = (nombre entrants - nombre sortants) x masse salariale moyenne unitaire des entrants

h <- function(x) {
  y <- x - début.période.sous.revue + 1
  
  nent <- ent[[y]][[3]]
  nsort <- s[[y]][[3]]

  variation.effectifs <- nent - nsort
  
  B <- Base[Année == x & Matricule %chin% as.character(ent[[y]][[1]]), 
                                              c(masse.salariale.unitaire,  "quotité.moyenne"),
                                                 with = FALSE] 
  
  C <- Base[Année == x & ! Matricule %chin% as.character(ent[[y]][[1]]) & ! Matricule %chin% as.character(s[[y]][[1]]), 
            c(masse.salariale.unitaire,  "quotité.moyenne"),
            with = FALSE] 
  
  masse.salariale.stable <- weighted.mean(C[[1]], C[[2]], na.rm = TRUE)
  tot.etpt.stable <- sum(C[[2]], na.rm = TRUE)
  
  se[y] <<- (weighted.mean(B[[1]], B[[2]], na.rm = TRUE) * variation.effectifs + masse.salariale.stable) / (tot.etpt.stable + variation.effectifs) - masse.salariale.stable/tot.etpt.stable

  
  formatC(as.numeric(se[y]),
            big.mark = " ",
            digits = 1,
            format = "f")
}


t <- function(x) {
  y <- x - début.période.sous.revue + 1
  
  nent <- ent[[y]][[3]]
  nsort <- s[[y]][[3]]
 
  B <- Base[Année == x & Matricule %chin% as.character(ent[[y]][[1]]), 
            c(masse.salariale.unitaire,  "quotité.moyenne"),
            with = FALSE] 
  
  C <- Base[Année == x & ! Matricule %chin% as.character(ent[[y]][[1]]) & ! Matricule %chin% as.character(s[[y]][[1]]), 
            c(masse.salariale.unitaire,  "quotité.moyenne"),
            with = FALSE] 
  
  masse.salariale.stable <- weighted.mean(C[[1]], C[[2]], na.rm = TRUE)
  tot.etpt.stable <- sum(C[[2]], na.rm = TRUE)
  
  D <- Base[Année == x & Matricule %chin% as.character(s[[y]][[1]]), 
          c(masse.salariale.unitaire,  "quotité.moyenne"),
          with = FALSE] 

  de[y] <<- (weighted.mean(B[[1]], B[[2]], na.rm = TRUE) * nent - weighted.mean(D[[1]], D[[2]], na.rm = TRUE) * nsort +  masse.salariale.stable) / tot.etpt[y] - masse.salariale.stable/tot.etpt.stable
  
  formatC(as.numeric(de[y]),
          big.mark = " ",
          digits = 1,
          format = "f")
}


# B

k <- function(x) {
  y <- x - début.période.sous.revue + 1
  formatC(as.numeric(de[y]-se[y]-nor[y]),
            big.mark = " ",
            digits = 1,
            format = "f")
  }

sous.période <- période[1:durée.sous.revue]

if (durée.sous.revue > 1) {

  noria <- c(sapply(sous.période, f),
    formatC(sum(unlist(nor), na.rm = TRUE),
            big.mark = "", digits = 1, format = "f"))
  
  entrants <- c(v <- sapply(1:durée.sous.revue, function(x) round(ent[[x]][[3]], 1)),
    formatC(round(sum(v, na.rm = TRUE), 1),
            big.mark = "", digits = 1, format = "f"))
  
  sortants <- c(v <- sapply(1:durée.sous.revue, function(x) round(s[[x]][[3]], 1)),
    formatC(round(sum(v, na.rm = TRUE), 1),
            big.mark = "", digits = 1, format = "f"))
  
  var.effectifs <- c(v <- sapply(1:durée.sous.revue, function(x) round(ent[[x]][[3]] - s[[x]][[3]], 1)),
    formatC(round(sum(v, na.rm = TRUE), 1),
            big.mark = "", digits = 1, format = "f"))
  
  effet.var <- c(sapply(sous.période, h),
    formatC(sum(unlist(se), na.rm = TRUE),
            big.mark = "", digits = 1, format = "f"))
  
  total.es <- c(sapply(sous.période, t),
    formatC(sum(unlist(de), na.rm = TRUE), 
            big.mark = "", digits = 1, format = "f"))
  
  vacances <- c(sapply(sous.période, k),
    formatC(sum(unlist(de) - unlist(se) - unlist(nor), na.rm = TRUE), 
            big.mark = "", digits = 1, format = "f"))

  print(
    Tableau.vertical2(colnames = c(étiquette.année, 
                                    "Effet de noria",
                                    "Entrants",
                                    "Sortants",
                                    "Variation effectifs",
                                    "Effet variation effectifs",
                                    "Effet de vacances",
                                    "Total effet entrées-sorties"),
                      
                      rownames = c(as.character(sous.période), "Total"),
                      noria,
                      entrants,
                      sortants,
                      var.effectifs,
                      effet.var,
                      vacances,
                      total.es))
  
} else {
  cat("L'effet de noria ne peut être calculé que pour des durées sous revue supérieures à un exercice.")
}


# C

}

