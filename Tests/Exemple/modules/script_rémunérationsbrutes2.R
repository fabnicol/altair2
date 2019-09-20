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

# Conversion en Rmd: spin(..., knit = FALSE)  dans l'encodage du système (convertir ce fichier en UTF-8 sous linux)

# On revient à une analyse des rémunérations qui réinclut tous les personnels (vacataires, élus, inactifs, annexes)

année <- fin.période.sous.revue

Analyse.remunerations.exercice <- Analyse.remunerations[Annee == année]     


#'# 3. Rémunérations brutes : analyse pour l'exercice `r année`      
#'## 3.1 Masse salariale brute de l'ensemble des agents     
  
masses.personnels <- Analyse.remunerations.exercice[Statut != "ELU",
                                                    .(Montant.brut.annuel = sum(Montant.brut.annuel, na.rm = TRUE),
                                                      rémunération.indemnitaire.imposable = sum(rémunération.indemnitaire.imposable, na.rm = TRUE),
                                                      indemnités.élu = sum(indemnités.élu, na.rm = TRUE),
                                                      total.lignes.paie = sum(total.lignes.paie, na.rm = TRUE),
                                                      acomptes = sum(acomptes, na.rm = TRUE))]

#'### Cumuls des rémunérations brutes pour l'exercice `r année`      
#'  
#'*Personnels (hors élus)*     
#'  
#'&nbsp;*Tableau 3.1.1*   
#'    
essayer({
print(Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes) :",
                    "\\ dont \\ Primes :",
                    "\\ dont \\ Autres rémunérations",
                    "Part de primes en %"),
                  c(masses.personnels$Montant.brut.annuel,
                    masses.personnels$total.lignes.paie,
                    masses.personnels$rémunération.indemnitaire.imposable,
                    masses.personnels$acomptes,
                    masses.personnels$rémunération.indemnitaire.imposable/masses.personnels$Montant.brut.annuel * 100)))
}, "Le tableau des rémunérations brutes pour l'ensemble des personnels n'a pas pu être généré.")

#'  
#'**Définitions :**
#'
#'  *Brut annuel (bulletins)*   : somme du champ *Brut*    
#'  *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
#'  *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
#'  *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
#'  

#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :  
#'  
#'  
#'&nbsp;*Tableau 3.1.2*   
#'    
essayer({
print(Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Bulletins de paie ",
                    "Lignes de paie ",
                    "Différence "),
                  c(masses.personnels$Montant.brut.annuel,
                    masses.personnels$total.lignes.paie,
                    masses.personnels$Montant.brut.annuel -
                      masses.personnels$total.lignes.paie)))
}, "Le tableau de cohérence pour l'ensemble des personnels n'a pas pu être généré.")

#'
#'à comparer aux soldes des comptes 641 et 648 du compte de gestion.
#'

#'   
#'## 3.2 Masse salariale brute des fonctionnaires
#'
#'*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*  
#'
filtre.fonctionnaire <<- function (X) X[ !is.na(X)  & X > minimum.positif ]

AR <- Analyse.remunerations.exercice[Statut == "TITULAIRE" | Statut == "STAGIAIRE", 
                                     ..colonnes.sélectionnées]

attach(AR, warn.conflicts = FALSE)
source("histogrammes.R", encoding = encodage.code.source)

detach(AR)
#'    
#'**Nota :**   
#'*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio rémunération / quotite*  
#'    
#'**Effectif : `r nrow(AR)`**
#'
#'**Tests de cohérence**

if (nrow(AR) > 0) {
  masses.fonct <- AR[ , lapply(.(Montant.brut.annuel, rémunération.indemnitaire.imposable, total.lignes.paie, acomptes), sum, na.rm = TRUE)]
  
} else {
  masses.fonct <- c(0,0) 
}

#'Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :
#'
#'  
#'&nbsp;*Tableau 3.2.1*   
#'    
essayer({
print(Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes) : ",
                    "\\ dont \\ \\ primes :",
                    "\\ dont \\ autres rémunérations :",
                    "Part de primes en %"),
                  c(masses.fonct[[1]],  
                    masses.fonct[[3]],
                    masses.fonct[[2]],
                    masses.fonct[[4]],
                    masses.fonct[[2]]/masses.fonct[[1]] * 100)))
}, "Le tableau des rémunérations brutes pour les focntionnaires n'a pas pu être généré.")

#'
#'**Définitions :**
#'
#'  *Brut annuel (bulletins)*   : somme du champ *Brut*   
#'  *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
#'  *Primes*                    : indemnités sauf remboursements, certaines IJSS, Supplément familial de traitement et Indemnité de résidence       
#'  *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
#'
#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (fonctionnaires) :
#'
#'  
#'&nbsp;*Tableau 3.2.2*   
#'    
essayer({
print(Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Bulletins de paie ",
                    "Lignes de paie ",
                    "Différence "),
                  c(masses.fonct[[1]],  # Brut
                    masses.fonct[[3]],  # lignes
                    masses.fonct[[1]] -
                      masses.fonct[[3]])))

}, "Le tableau de cohérence pour les fonctionnaires n'a pas pu être généré.")

#'
#'A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.
#'
#'**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année `r année`**     
#'  
#'&nbsp;*Tableau 3.2.3*   
#'    

essayer({
print(Résumé(c("Traitement indiciaire",
         "Primes",
         "Autres rémunérations",
         "Quotité",
         "Effectif"),
       AR[Grade != "V" & Grade != "A" & Statut != "ELU"
          & Filtre_actif == TRUE
          & Filtre_annexe == FALSE,
             .(traitement.indiciaire,
               rémunération.indemnitaire.imposable,
               acomptes,
               quotite.moyenne)],
       
       extra = "length"))
  
}, "Le tableau des quartiles du SMPT n'a pas pu être généré.")

#'  
#'&nbsp;*Tableau 3.2.4*   
#'    
essayer({
  print(Résumé(c("Total lignes hors rappels",
           "Total brut",
           "SMPT brut en EQTP",
           "Part indemnitaire",
           "Quotité",
           "Effectif"),
         AR[Grade != "V" & Grade != "A" & Statut != "ELU"
            & Filtre_actif == TRUE
            & Filtre_annexe == FALSE,
            .(total.lignes.paie,
               Montant.brut.annuel,
               Montant.brut.annuel.eqtp,
               part.rémunération.indemnitaire,
               quotite.moyenne)],
         extra = "length"))
}, "Le tableau des quartiles du SMPT n'a pas pu être généré.")

#'   
#'*Hors vacataires identifiés, assistantes maternelles, élus locaux et pour les postes actifs non annexes*  
#'
#'**Categorie A**
#'

ARA <- data.table::data.table(NULL)
ARB <- data.table::data.table(NULL)
ARC <- data.table::data.table(NULL)

#'  
#'&nbsp;*Tableau 3.2.5*   
#'    


if (analyse.par.catégorie) {
  
essayer({
  ARA <- AR[Categorie == "A" & Grade != "V" & Grade != "A" & Statut != "ELU" 
            & Filtre_actif == TRUE
            & Filtre_annexe == FALSE]
  
  print(Résumé(c("Traitement indiciaire",
           "Primes",
           "Autres rémunérations",
           "Quotité"),
         ARA[ , .(traitement.indiciaire,
                  rémunération.indemnitaire.imposable,
                  acomptes,
                  quotite.moyenne)]))
}, "Le tableau des quartiles pour la catégorie A n'a pas pu être généré.")
  
} else {
  cat("Pas de statistiques par catégorie.\n")
}
#'


#'  
#'&nbsp;*Tableau 3.2.6*   
#'    
if (analyse.par.catégorie) {  
  
  essayer({
    print(Résumé(c("Total rémunérations", 
             "Total rémunérations EQTP", 
             "Part indemnitaire",
             "Quotité"),
           ARA[ , .(Montant.brut.annuel,
                    Montant.brut.annuel.eqtp,
                    part.rémunération.indemnitaire,
                    quotite.moyenne)]))
  }, "Le tableau des quartiles pour la catégorie A n'a pas pu être généré.")
  
} else {
  cat("Pas de statistiques par catégorie.\n")
}


#'
#'**Effectif : `r nrow(ARA)`**  
#'
#'**Categorie B**
#'
#'  
#'&nbsp;*Tableau 3.2.7*   
#'    

if (analyse.par.catégorie) {
  
  essayer({  
    ARB <- AR[Categorie == "B" & Grade != "V" & Grade != "A" & Statut != "ELU"
              & Filtre_actif == TRUE
              & Filtre_annexe == FALSE]
    
    print(Résumé(c("Traitement indiciaire",
             "Primes",
             "Autres rémunérations",
             "Quotité"),
           ARB[, .(traitement.indiciaire,
                   rémunération.indemnitaire.imposable,
                   acomptes,
                   quotite.moyenne)]))
  }, "Le tableau des quartiles pour la catégorie B n'a pas pu être généré.")
  
} else {
  cat("Pas de statistiques par catégorie.\n")
}
#'
#'  
#'&nbsp;*Tableau 3.2.8*   
#'    

if (analyse.par.catégorie) {
  
  essayer({  
    print(Résumé(c("Total rémunérations",
             "Total rémunérations EQTP",
             "Part de la rémunération indemnitaire",
             "Quotité"),
           ARB[, .(Montant.brut.annuel,
                   Montant.brut.annuel.eqtp,
                   part.rémunération.indemnitaire,
                   quotite.moyenne)]))
  }, "Le tableau des quartiles pour la catégorie B n'a pas pu être généré.")
  
} else {
  cat("Pas de statistiques par catégorie.\n")
}

#'
#'**Effectif : `r nrow(ARB)`**
#'
#'**Categorie C**
#'

#'  
#'&nbsp;*Tableau 3.2.9*   
#'    

if (analyse.par.catégorie) {
  
  essayer({  
    ARC <- AR[Categorie == "C" & Grade != "V" & Grade != "A" & Statut != "ELU" 
              & Filtre_actif == TRUE
              & Filtre_annexe == FALSE, ]
    
    print(Résumé(c("Traitement indiciaire",
             "Primes",
             "Autres rémunérations",
             "Quotité"),
           ARC[ , .(traitement.indiciaire,
                    rémunération.indemnitaire.imposable,
                    acomptes,
                    quotite.moyenne)]))
    
  }, "Le tableau des quartiles pour la catégorie C n'a pas pu être généré.")

} else {
  cat("Pas de statistique par catégorie.\n")
}

#'
#'  
#'&nbsp;*Tableau 3.2.10*   
#'    

if (analyse.par.catégorie) {
  
  essayer({
    
    print(Résumé(c("Total rémunérations",
             "Total rémunérations EQTP",
             "Part de la rémunération indemnitaire",
             "Quotité"),
           ARC[ , .(Montant.brut.annuel,
                    Montant.brut.annuel.eqtp,
                    part.rémunération.indemnitaire,
                    quotite.moyenne) ]))
  
    }, "Le tableau des quartiles pour la catégorie C n'a pas pu être généré.")
  
} else {
  
  cat("Pas de statistiques par catégorie.\n")
}

#'**Effectif : `r nrow(ARC)`**
#'
#'
#'######      <br>
#'
#'## 3.3 Contractuels, vacataires et stagiaires inclus   
#'   
#'*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   
#'
attach(Analyse.remunerations.exercice, warn.conflicts=FALSE)

essayer({
temp <- rémunération.indemnitaire.imposable.eqtp[Statut != "ELU"
                                                 & Statut != "TITULAIRE"
                                                 & Statut != "STAGIAIRE"
                                                 & Filtre_actif == TRUE
                                                 & Filtre_annexe == FALSE
                                                 & rémunération.indemnitaire.imposable.eqtp > 1000] / 1000

if (longueur.non.na(temp) > 0)
  hist(temp,
       xlab = "Rémunération indemnitaire brute imposable en milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(0, 40),
       main = "Rémunération annuelle totale des contractuels en " %+% année,
       col = "red",
       nclass = 50)
}, "Le graphique de la rémunération indemnitaire des contractuels n'a pas pu être généré.")

#'   
#'**Nota :**
#'Ne sont retenues que les rémunérations supérieures à 1 000 k&euro;.
#'Les élus ne sont pas pris en compte.
#'   

temp <- positive(acomptes)

detach(Analyse.remunerations.exercice)

if (longueur.non.na(temp))
  hist(temp,
       xlab = "En euros :\n divers",
       ylab = "Effectif",
       xlim = c(0, 5000),
       main = "Autres rémunérations en " %+% année,
       nclass = 50,
       col = "grey")

#'

AR <- Analyse.remunerations.exercice[Statut != "ELU"
                                             &  Statut != "TITULAIRE"
                                             &  Statut != "STAGIAIRE"
                                             & Filtre_actif == TRUE
                                             & Filtre_annexe == FALSE,
                                             ..colonnes.sélectionnées]

#'   
#'**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année `r année`**     
#'   
#'  
#'&nbsp;*Tableau 3.3.1*   
#'    
essayer({
    print(Résumé(c("Primes",
             "Autres rémunérations",
             "Quotité",
             "Effectif"),
           AR[ , .(rémunération.indemnitaire.imposable,
                   acomptes,
                   quotite.moyenne)],
           extra = "length"))

  
}, "Le tableau des quartiles pour la catégorie C n'a pas pu être généré.")

#'  
#'&nbsp;*Tableau 3.3.2*   
#'    

essayer({
  print(Résumé(c("Total rémunérations",
           "Total rémunérations EQTP",
           "Quotité",
           "Effectif"),
         AR[ , .(Montant.brut.annuel, Montant.brut.annuel.eqtp, quotite.moyenne)],
         extra = "length"))
}, "Le tableau des quartiles pour la catégorie C n'a pas pu être généré.")

#'


# pour année fin #

rm(Analyse.remunerations.exercice)



#'  
conditionnel("Lien vers la base des rémunérations", "Bases/Remunerations/Analyse.remunerations.csv")  
#'   




newpage()





