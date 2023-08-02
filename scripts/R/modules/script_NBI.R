#'
########### 5. TESTS STATUTAIRES ########################
#'
#'   

#### 5.1 NBI ####  

#'# 5. Tests réglementaires   
#'    
#'**Dans cette partie, l'ensemble de la base de paie est étudié.**  
#'    
#'    
#'## 5.1 Contrôle des nouvelles bonifications indiciaires (NBI) &nbsp;  [![Notice](icones/Notice.png)](Docs/Notices/fiche_NBI_nt.odt)

#+ tests-statutaires-nbi
#'   
#'Il est fortement conseillé, pour ce test, de saisir les codes de NBI dans l'onglet Codes de l'interface graphique  &nbsp; [![Notice](icones/Notice.png)](Docs/Notices/fiche_onglet_codes.odt)     
#'A défaut, des lignes de paye de rappels de cotisations sur NBI peuvent être agrégées, dans certains cas, aux rappels de rémunération brute.   

essayer( {
inserer_script(fonction = "calcul_NBI")
}, "Le contrôle de la NBI n'a pas pu être réalisé.")

#'   
conditionnel("Lien vers la base de données NBI aux non titulaires", "Bases/Reglementation/NBI.aux.non.titulaires.csv")
#'     

#'
#'    
#'&nbsp;*Tableau 5.1.3 : Contrôle global de la liquidation des NBI*     &nbsp; [![Notice](icones/Notice.png)](Docs/Notices/fiche_NBI_glob.odt)       
#'    

attach(cumuls.nbi)

essayer ({

# Ne pas oublier print

print(Tableau.vertical2(c("Annee", "Cumuls des NBI", "Montants versés (a)", "Point d'INM apparent", "Point d'INM moyen", "Contrôle"),
                Annee,
                cumul.annuel.indiciaire,
                cumul.annuel.montants,
                a <- cumul.annuel.montants/cumul.annuel.indiciaire,
                b <- PointMensuelIMMoyen[Annee - 2007],
                ifelse(abs(b - a) > 0.3, "Rouge", ifelse(abs(b - a) > 0.15, "Orange", "Vert"))))

                },
"Le tableau de contrôle global de la liquidation des NBI n'a pas pu être produit")
     
detach(cumuls.nbi)

#'   
conditionnel("Lien vers la base de données des cumuls annuels de NBI", "Bases/Fiabilite/cumuls.nbi.csv")   
#'   

#'
#'  
#'&nbsp;*Tableau 5.1.5 : Contrôle d'attribution de NBI par categorie statutaire*  &nbsp; [![Notice](icones/Notice.png)](Docs/Notices/fiche_plafonds_NBI.odt)           
#'  

essayer({
inserer_script(fonction = "categories_NBI")
}, "Le contrôle du lien entre NBI et categorie statutaire n'a pas pu être réalisé.")

#'   
#'**Nota :**   
#'Coût annuel calcule pour la quotite de travail observée, limité aux dépassements des maxima ci-après.   
#'Dépassements de NBI :     
#' - plus de 50 points pour la categorie A;   
#' - plus de 30 points pour la categorie B;   
#' - plus de 20 points pour la categorie C.     
#'Directeurs généraux adjoints : plus de 80 points.   
#'Directeurs généraux adjoints : plus de 120 points.           
#'  

#'   
conditionnel("Lien vers les NBI dépassant les seuils par categorie statutaire", "Bases/Reglementation/NBI.cat.irreg.csv")   
#'   
