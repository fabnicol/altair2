
#'## 1.2 Pyramide des âges, ensemble des personnels      
#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_2.odt)    
#' 
#+fig.height=8, fig.width=7

produire_pyramides(NULL, 
                   "Pyramide des âges des personnels",
                   versant = VERSANT_FP, 
                   e)

#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_3.odt)     
#'

newpage()


#'  
#'&nbsp;*Tableau 1.2.1*   
#'    

print(e$res)
#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_1.odt)   
conditionnel("Lien vers la base des âges - début de période", "Bases/Effectifs/" %+%  e$nom.fichier.avant %+% ".csv")  
#'    
conditionnel("Lien vers la base des âges - fin de période", "Bases/Effectifs/" %+%  e$nom.fichier.après %+% ".csv")  
#'     
#'   
#'   
#'   
#'   
#'   
#' 

########### 1.3 Pyramides fonctionnaires ########################

#'
#'## 1.3 Pyramide des âges des fonctionnaires  &nbsp;
#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_2.odt)    

#' 
#+fig.height=8, fig.width=7

produire_pyramides(c("TITULAIRE", "STAGIAIRE"), 
                   "Pyramide des âges des fonctionnaires",
                   versant = "TIT_" %+% VERSANT_FP,
                   e)

#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_3.odt)     
#'  

newpage()

#'  
#'&nbsp;*Tableau 1.3.1*   
#'    

print(e$res)

#'  
conditionnel("Lien vers la base des âges - début de période", "Bases/Effectifs/" %+%  e$nom.fichier.avant %+% ".csv")  
#'  
conditionnel("Lien vers la base des âges - fin de période", "Bases/Effectifs/" %+%  e$nom.fichier.après %+% ".csv")  
#'[![Notice](Notice.png)](Docs/Notices/fiche_1.odt)      
#'   
#'   

#'   
#'   
#'   
#'   
#'   
#' 

########### 1.4 Pyramides non Tit ########################

#'## 1.4 Pyramide des âges, personnels non titulaires   &nbsp; 
#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_2.odt)     
#'  
#+fig.height=8, fig.width=7

essayer(produire_pyramides(c("NON_TITULAIRE"), 
                           "Pyramide des âges des non titulaires", 
                           versant = "NONTIT_" %+% VERSANT_FP,
                           e))


#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_3.odt)     
#'  

newpage()

#'  
#'&nbsp;*Tableau 1.4.1*   
#'    

print(e$res)


#'  
conditionnel("Lien vers la base des âges - début de période", "Bases/Effectifs/" %+% e$nom.fichier.avant %+% ".csv")  
#'  
conditionnel("Lien vers la base des âges - fin de période", "Bases/Effectifs/" %+% e$nom.fichier.après %+% ".csv")  
#'[![Notice](Notice.png)](Docs/Notices/fiche_1.odt)      

#'   
#'   
#'   
#'   
#'   
#'   


########### 1.5 Pyramides Autres statut ########################

#'## 1.5 Pyramide des âges, autres statuts  &nbsp; 

#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_2.odt)   
#' 
#+fig.height=8, fig.width=7
Filtre_bulletins <<- setdiff(unique(Bulletins.paie$Statut), c("TITULAIRE", "NON_TITULAIRE", "STAGIAIRE")) 

essayer(produire_pyramides(Filtre_bulletins,
                           "Pyramide des âges des autres personnels",
                           versant = "",
                           e))

#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_3.odt)     
#'  

#'   
#'   
#'   
#'   
#'   
#' 
newpage()

#'  
#'&nbsp;*Tableau 1.5.1*   
#'    

print(e$res)

#'  
conditionnel("Lien vers la base des âges - début de période", "Bases/Effectifs/" %+% e$nom.fichier.avant %+% ".csv")  
#'  
conditionnel("Lien vers la base des âges - fin de période", "Bases/Effectifs/" %+% e$nom.fichier.après %+% ".csv")  
#'[![Notice](Notice.png)](Docs/Notices/fiche_1.odt)    
#'  
#'*Source des comparaisons avec les données nationales*      
#'         
#'Rapport annuel sur l'état de la fonction publique pour 2016      
#'[Pyramide 2013 FPH](Docs/insee_pyramide_fph_2013.csv)   
#'[Pyramide 2013 FPT](Docs/insee_pyramide_fpt_2013.csv)     


#'*Toutes les pyramides des âges sont établies au 31 décembre de l'année considérée.*   
#'*Les élus ne sont pas compris dans le périmètre statistique.*     

#'   
#'   
#'   
#'   
#'   
#'   

