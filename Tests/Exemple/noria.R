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


noria <- function(Tableau = tableau.effectifs, Base = Analyse.variations.par.exercice, champ = "brut", filtre = "") { 

ES <- fread("~/Desktop/SDIS/Bases/ES.csv", dec= ",", sep=";", encoding="UTF-8")
ES$Matricule <- as.character(ES$Matricule)

ES <- ES[ , .(Matricule, Entrée, Sortie)][ , `:=`(Entrée = as.Date(Entrée, format = "%d/%m/%Y"),
                                                  Sortie = as.Date(Sortie, format = "%d/%m/%Y"))
                                        ][ , `:=`(Année.entrée = as.numeric(format(Entrée, "%Y")),
                                                  Mois.entrée  = as.numeric(format(Entrée, "%m")),
                                                  Jour.entrée  = as.numeric(format(Entrée, "%d")),
                                                  Année.sortie = as.numeric(format(Sortie, "%Y")),
                                                  Mois.sortie  = as.numeric(format(Sortie, "%m")),
                                                  Jour.sortie  = as.numeric(format(Sortie, "%d")))]

if (filtre != "") {
  if (! filtre %chin% c("A", "B", "C")) {
    Base <- Base[Statut %chin% filtre]
  } else {
    Base <- Base[Catégorie == filtre]
  } 
}

tot.etpt <- Base[ , .(etpt = sum(quotité.moyenne, na.rm = TRUE)), by = Année]  
  
if (champ == "brut") salaire.moyen <- "Montant.brut.annuel.eqtp" else salaire.moyen <- "Montant.net.annuel.eqtp"

présents.bulletins <- list()
matricules.présents <- list()

transl <- function(année) année - début.période.sous.revue + 1

filtrage <- function(année) {
  
    if (filtre != "") {
      
      if (! filtre %chin% c("A", "B", "C")) {
        présents.bulletins[[transl(année)]] <<- Bulletins.paie[Statut %chin% filtre & Année == année & quotité.moyenne != 0 & Grade != "A" & Grade != "V", .(Matricule, Mois, quotité)]
      } else {
        présents.bulletins[[transl(année)]] <<- Bulletins.paie[Catégorie == filtre & Année == année & quotité.moyenne != 0 & Grade != "A" & Grade != "V", .(Matricule, Mois, quotité)]
      } 
      
    } else {
      
      présents.bulletins[[transl(année)]] <<- Bulletins.paie[Année == année & quotité.moyenne != 0 & Grade != "A" & Grade != "V", .(Matricule, Mois, quotité)]
    }
    
    présents.bulletins[[transl(année)]] <<- merge(présents.bulletins[[transl(année)]], ES[ , .(Matricule, Année.entrée, Mois.entrée, Jour.entrée,
                                                                                                          Année.sortie, Mois.sortie, Jour.sortie)], by = "Matricule", all.x = TRUE)
  
    matricules.présents.base <- Base[Année == année, unique(Matricule)]
    
    matricules.présents[[transl(année)]] <<- intersect(unique(présents.bulletins[[transl(année)]]$Matricule), matricules.présents.base)
}

for (année in période) filtrage(année)

exclure.présents <- function(année, mois) {
  
    matricules.présents <- matricules.présents[[transl(année)]]
    
if (noria.sur.base.de.paie) {
  
     matricules.mois <- présents.bulletins[[transl(année)]][Mois == mois & quotité != 0, unique(Matricule)]
    
     matricules.mois <- présents.bulletins[[transl(année)]][Mois == mois & quotité != 0, unique(Matricule)]

     matricules.mois <- intersect(matricules.mois, matricules.présents)
    
     matricules.sélectionnés <- setdiff(matricules.présents, matricules.mois)
     
     date <- -1 
  
} else {    
  
    if (mois == 12) {
      
      matricules.sélectionnés <- présents.bulletins[[transl(année)]][Année.sortie == année, unique(Matricule)]
      
    } else {
      
      matricules.sélectionnés <- présents.bulletins[[transl(année)]][Année.entrée == année, unique(Matricule)]
      
    }
    
    matricules.sélectionnés   <- intersect(matricules.sélectionnés, matricules.présents)
    
    if (mois == 12) {
      
      date <- présents.bulletins[[transl(année)]][Matricule %chin% matricules.sélectionnés, mean(Mois.sortie - 1 + Jour.sortie / 30, na.rm = TRUE)] / 12
      
    } else {
      
      date <- présents.bulletins[[transl(année)]][Matricule %chin% matricules.sélectionnés, mean(Mois.entrée - 1 + Jour.entrée / 30, na.rm = TRUE)] / 12
    }
}
    
    etpt.sélectionnés <- unique(Base[Année == année 
                                 & quotité.moyenne != 0 
                                 & Grade != "A" 
                                 & Grade != "V"
                                 & Matricule %chin% matricules.sélectionnés, 
                                 .(Année, quotité.moyenne, Matricule)], by = NULL)[ , sum(quotité.moyenne, na.rm=TRUE)]
    
    list(matricules = matricules.sélectionnés, etpt = etpt.sélectionnés,  n = length(matricules.sélectionnés), date = date)
}

entrants <- function(année)   exclure.présents(année, mois = 1)
sortants <- function(année)   exclure.présents(année, mois = 12)

période.translatée <- 1:durée.sous.revue

ent   <- lapply(période, entrants)
sort  <- lapply(période, sortants)

nsort <- sapply(période.translatée, function(année) sort[[année]][["n"]])
nent  <- sapply(période.translatée, function(année) ent[[année]][["n"]])

if (noria.sur.base.de.paie) {
  qsortants <- sapply(période.translatée, function(année) 1 - sort[[année]][["etpt"]] / nsort[année])
  qentrants <- sapply(période.translatée,  function(année) ent[[année]][["etpt"]]) / nent
} else {
  qsortants <- sapply(période.translatée, function(année)  1 - sort[[année]][["date"]]) 
  qentrants <- sapply(période.translatée,  function(année) ent[[année]][["date"]]) 
}

salaire.entrants <- sapply(période, function(année) {
                                          B <- Base[Année == année 
                                                     & Matricule %chin% as.character(ent[[transl(année)]][["matricules"]]), 
                                                     c(salaire.moyen, "quotité.moyenne"), 
                                                     with = FALSE]
                                          
                                          weighted.mean(B[[1]],  B[[2]], na.rm = TRUE)
})

salaire.sortants <- sapply(période, function(année) {
                                           B <- Base[Année == année
                                                 & Matricule %chin% as.character(sort[[transl(année)]][["matricules"]]), 
                                                 c(salaire.moyen, "quotité.moyenne"),
                                                 with = FALSE]
  
                                           weighted.mean(B[[1]], B[[2]], na.rm = TRUE)
})


tot.etpt.stables <- sapply(période, function(année) Base[Année == année 
                         & ! Matricule %chin% as.character(ent[[transl(année)]][["matricules"]]) 
                         & ! Matricule %chin% as.character(sort[[transl(année)]][["matricules"]]), 
                         sum(quotité.moyenne)]) 

rmpp <- sapply(période, function(année) {
  B <- Base[Année == année 
            & ! Matricule %chin% as.character(ent[[transl(année)]][["matricules"]]) 
            & ! Matricule %chin% as.character(sort[[transl(année)]][["matricules"]]), 
            c(salaire.moyen,  "quotité.moyenne"),
            with = FALSE] 
  
  weighted.mean(B[[1]], B[[2]], na.rm = TRUE)
}) 

smtp <- sapply(période, function(année) {
  B <- Base[Année == année, 
            c(salaire.moyen,  "quotité.moyenne"),
            with = FALSE] 
  
  weighted.mean(B[[1]], B[[2]], na.rm = TRUE)
}) 

delta <- rmpp - smtp +
effet.total.entrées.sorties / tot.etpt$etpt

# tot.etpt.stable <- sum(C[[2]], na.rm = TRUE)

# GVT négatif = (masse salariale moyenne unitaire des entrants - masse salariale moyenne unitaire des sortants) x nombre sortants
# ici exprimée en rémunérations

form <- function(V) formatC(c(as.numeric(V), sum(as.numeric(V), na.rm = TRUE)),
                            big.mark = " ",
                            digits = 1,
                            format = "f")

effet.noria <- (salaire.entrants - salaire.sortants) * nsort * qsortants

effet.noria.form <- form(effet.noria)

# L'effet de la variation d'effectifs (schéma d'emploi, vacances infra-annuelles incluses) est : 
#  se = (nombre entrants - nombre sortants) x masse salariale moyenne unitaire des entrants

effet.variation.effectifs <- salaire.entrants * (nent - nsort) * qentrants

effet.variation.effectifs.form <- form(effet.variation.effectifs)

effet.total.entrées.sorties <- salaire.entrants * nent * qentrants - salaire.sortants * nsort * qsortants 
 
effet.total.entrées.sorties.form <- form(effet.total.entrées.sorties)

effet.vacances <- nsort * (qentrants - qsortants) * salaire.entrants
    
effet.vacances.form <- form(effet.vacances)

variation.effectifs <- nent - nsort

taux.rotation <- 1/2 * 100 / (tot.etpt$etpt/(nent + nsort) - 1)

taux.rotation.form <- form(taux.rotation)
taux.rotation.form[durée.sous.revue + 1] <- ""
                        
print(
  Tableau.vertical2(colnames = c(étiquette.année, 
                                "Entrants",
                                "Sortants",
                                "Var. effectifs",
                                "Taux de rotation %"),
                    rownames = c(as.character(période), "Total"),
                    form(nent),
                    form(nsort),
                    form(variation.effectifs),
                    taux.rotation.form))

print(  
      Tableau.vertical2(colnames = c(étiquette.année, 
                                    "Effet noria",
                                    "Effet var. effectifs",
                                    "Effet vacances",
                                    "Total"),
                      rownames = c(as.character(période), "Total"),
                      effet.noria.form,
                      effet.variation.effectifs.form,
                      effet.vacances.form,
                      effet.total.entrées.sorties.form))
  
data.table(période,
            effet.noria,
            nent,
            nsort,
            variation.effectifs,
            taux.rotation,
            effet.variation.effectifs,
            effet.vacances,
            effet.total.entrées.sorties)

}
