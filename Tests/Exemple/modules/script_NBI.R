#'
########### 5. TESTS STATUTAIRES ########################
#'
#'   

#### 5.1 NBI ####  

#'# 5. Tests réglementaires   
#'    
#'**Dans cette partie, l'ensemble de la base de paie est étudié.**  
#'Les agents non actifs ou dont le poste est annexe sont réintroduits dans le périmètre.   
#'    
#'    
#'## 5.1 Contrôle des nouvelles bonifications indiciaires (NBI)

#+ tests-statutaires-nbi
#'   
#'Il est conseillé, pour ce test, de saisir les codes de NBI dans l'onglet Codes de l'interface graphique  &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_onglet_codes.odt)     
#'A défaut, des lignes de paye de rappels de cotisations sur NBI peuvent être agrégées, dans certains cas, aux rappels de rémunération brute.   
#'       

insérer_script(fonction = "calcul_NBI")

#'   
conditionnel("Lien vers la base de données NBI aux non titulaires", "Bases/Reglementation/NBI.aux.non.titulaires.csv")
#'[![Notice](Notice.png)]("Docs/Notices/fiche_NBI_nt.odt")       
#'     

#'  
#'&nbsp;*Tableau 5.1.1 : Contrôle de liquidation de la NBI*    [![Notice](Notice.png)](Docs/Notices/fiche_NBI_liq.odt)       
#'    

Tableau(
  c("Lignes de NBI concernées",
    "Coûts correspondants",
    "Rappels (montants bruts)"),
  nrow(lignes.nbi.anormales),
  round(couts.nbi.anormales, 1),
  round(rappels.nbi, 1))

#'  
#'&nbsp;*Tableau 5.1.2 : Contrôle de liquidation de la NBI, hors rappels*     &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_NBI_liq.odt)       
#'    

Tableau(
  c("Lignes de NBI concernées (hors rappels)",
    "Coûts correspondants"),
  nrow(lignes.nbi.anormales.hors.rappels),
  round(couts.nbi.anormales.hors.rappels, 1))

#'       
conditionnel("Lien vers la base de données des anomalies de NBI", "Bases/Fiabilite/lignes.nbi.anormales.csv")     
conditionnel("Lien vers la base de données des anomalies de NBI hors rappels", "Bases/Fiabilite/lignes.nbi.anormales.hors.rappels.csv")          
#'   
#'**Nota :**   
#'*Est considéré comme anomalie manifeste un total annuel de rémunérations NBI correspondant à un point d'indice net mensuel inférieur à la moyenne de l'année moins 1 euro ou supérieur à cette moyenne plus 1 euro.*    
#'*Les rappels ne sont pas pris en compte dans les montants versés. Certains écarts peuvent être régularisés en les prenant en compte*     
#'  
#'    
#'&nbsp;*Tableau 5.1.3 : Contrôle global de la liquidation des NBI*     &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_NBI_glob.odt)       
#'    

with(cumuls.nbi,
     
     Tableau.vertical2(c("Année", "Cumuls des NBI", "Montants versés (a)", "Point d'INM apparent", "Point d'INM moyen", "Contrôle"), 
                       Année, 
                       cumul.annuel.indiciaire,
                       cumul.annuel.montants,
                       a <- cumul.annuel.montants/cumul.annuel.indiciaire,
                       b <- PointMensuelIMMoyen[Année - 2007],
                       ifelse(abs(b - a) > 0.3, "Rouge", ifelse(abs(b - a) > 0.15, "Orange", "Vert")))
     
)

#'   
conditionnel("Lien vers la base de données des cumuls annuels de NBI", "Bases/Fiabilite/cumuls.nbi.csv")   
#'   

# --- Test Proratisation NBI

#'  
#'&nbsp;*Tableau 5.1.4 : Contrôle de proratisation/liquidation de la NBI*     &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_NBI_prorat.odt)           
#'  

# Calcul plus exact de liquidation, attention à exclure les rappels

insérer_script(fonction = "proratisation_NBI")

#'   
conditionnel("Lien vers les bulletins anormaux du contrôle de proratisation/liquidation de la NBI", "Bases/Fiabilite/lignes.nbi.anormales.mensuel.csv")   
#'   
#'   
conditionnel("Lien vers les lignes de paye du contrôle de proratisation/liquidation de la NBI", "Bases/Fiabilite/lignes.paie.nbi.anormales.mensuel.csv")   
#'   
#'  
#'&nbsp;*Tableau 5.1.5 : Contrôle d'attribution de NBI par catégorie statutaire*  &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_plafonds_NBI.odt)           
#'  

insérer_script(fonction = "catégories_NBI")

#'   
#'**Nota :**   
#'Coût annuel calculé pour la quotité de travail observée, limité aux dépassements des maxima ci-après.   
#'Dépassements de NBI :     
#' - plus de 50 points pour la catégorie A;   
#' - plus de 30 points pour la catégorie B;   
#' - plus de 20 points pour la catégorie C.     
#'Directeurs généraux adjoints : plus de 80 points.   
#'Directeurs généraux adjoints : plus de 120 points.           
#'  

#'   
conditionnel("Lien vers les NBI dépassant les seuils par catégorie statutaire", "Bases/Reglementation/NBI.cat.irreg.csv")   
#'   
