e <- new.env()

#'## 1.2 Pyramide des âges, ensemble des personnels      
#'   
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_2.odt)    
#' 

produire_pyramides(NULL, 
                   "Pyramide des âges des personnels",
                   versant = VERSANT_FP, 
                   e)

#'   
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_3.odt)     
#'

newpage()


#'  
#'&nbsp;*Tableau 1.2.1*   
#'    
#'   
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_1.odt)   
conditionnel("Lien vers la base des âges - début de periode", "Bases/Effectifs/" %+%  e$nom.fichier.avant %+% ".csv")  
#'    
conditionnel("Lien vers la base des âges - fin de periode", "Bases/Effectifs/" %+%  e$nom.fichier.apres %+% ".csv")  
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
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_2.odt)    

#' 


essayer(produire_pyramides(c("TITULAIRE", "STAGIAIRE"), 
                   "Pyramide des âges des fonctionnaires",
                   versant = "",  #"TIT_" %+% VERSANT_FP : à réactiver lorsque les statistiques de pyramides des âges par versant ET statut seront dispo
                   e),  "Impossible de produire la pyramide des fonctionnaires")

#'   
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_3.odt)     
#'  

newpage()

#'  
#'&nbsp;*Tableau 1.3.1*   
#'    

print(e$res)

#'  
conditionnel("Lien vers la base des âges - début de periode", "Bases/Effectifs/" %+%  e$nom.fichier.avant %+% ".csv")  
#'  
conditionnel("Lien vers la base des âges - fin de periode", "Bases/Effectifs/" %+%  e$nom.fichier.apres %+% ".csv")  
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_1.odt)      
#'   
#'   

#'   
#'   
#'   
#'   
#'   
#' 

########### 1.4 Pyramides des personnels contractuels ########################

#'## 1.4 Pyramide des âges, personnels non titulaires   &nbsp; 
#'  
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_2.odt)     
#'  
##fig.height=8, fig.width=7

essayer(produire_pyramides(c("NON_TITULAIRE"), 
                           "Pyramide des âges des non titulaires", 
                           versant = "", #"NONTIT_" %+% VERSANT_FP, à réactiver lorsque les statistiques de pyramides des âges par versant ET statut seront dispo
                           e), "Impossible de produire la pyramide des contractuels")


#'   
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_3.odt)     
#'  

newpage()

#'  
#'&nbsp;*Tableau 1.4.1*   
#'    

print(e$res)


#'  
conditionnel("Lien vers la base des âges - début de periode", "Bases/Effectifs/" %+% e$nom.fichier.avant %+% ".csv")  
#'  
conditionnel("Lien vers la base des âges - fin de periode", "Bases/Effectifs/" %+% e$nom.fichier.apres %+% ".csv")  
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_1.odt)      

#'   
#'   
#'   
#'   
#'   
#'   


########### 1.5 Pyramides Autres statut ########################

#'## 1.5 Pyramide des âges, autres statuts  &nbsp; 

#'  
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_2.odt)   
#' 
##fig.height=8, fig.width=7

Filtre_bulletins <<- setdiff(unique(Bulletins.paie$Statut), c("TITULAIRE", "NON_TITULAIRE", "STAGIAIRE")) 

essayer(produire_pyramides(Filtre_bulletins,
                           "Pyramide des âges des autres personnels",
                           versant = "",
                           e),  "Impossible de produire la pyramide des autres statuts")

#'   
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_3.odt)     
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
conditionnel("Lien vers la base des âges - début de periode", "Bases/Effectifs/" %+% e$nom.fichier.avant %+% ".csv")  
#'  
conditionnel("Lien vers la base des âges - fin de periode", "Bases/Effectifs/" %+% e$nom.fichier.apres %+% ".csv")  
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_1.odt)    
#'  



#'*Toutes les pyramides des âges sont établies au 31 décembre de l'annee considérée.*   
#'*Les élus ne sont pas compris dans le périmètre statistique.*     



#'   
#'   
#'   
#'   
#'   
#'   

