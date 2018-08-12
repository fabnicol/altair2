#+ pyramides-des-âges

message("Statistiques de démographie réalisées.")

e <- new.env()

fichiers.pyr <- list.files(path= file.path(currentDir, "data"), pattern = "*.csv", full.names = TRUE)

# Lecture des fichiers de référence des pyramides (fichiers listés dans fichiers.pyr), comportant les statistiques INSEE

for (f in fichiers.pyr) {
  base <- basename(f)
  assign(substr(base, 1, attr(regexec("(.*)\\.csv", base)[[1]], "match.length")[2]),
         fread(f, 
               sep = ";",
               header = TRUE,
               encoding = "Latin-1",
               dec = ",",
               colClasses = c("integer", "numeric", "numeric", "integer", "character")),
         envir = .GlobalEnv)
}


########### 1.2 Pyramides ########################

#'## `r chapitre`.2 Pyramide des âges, ensemble des personnels  &nbsp; 
#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_2.odt)    
#' 
#+fig.height=8, fig.width=7

essayer(produire_pyramides(NULL, 
                           "Pyramide des âges des personnels",
                           versant = VERSANT_FP, 
                           e),
        "La pyramide des âges de l'ensemble des personnels n'a pas pu être générée.")

#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_3.odt)     
#'

newpage()


#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

print(e$res)
#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_1.odt)   
#'[Lien vers la base des âges - début de période](Bases/Effectifs/`r e$nom.fichier.avant`.csv)  
#'    
#'[Lien vers la base des âges - fin de période](Bases/Effectifs/`r e$nom.fichier.après`.csv)  
#'     


########### 1.3 Pyramides fonctionnaires ########################

#'
#'## `r chapitre`.3 Pyramide des âges des fonctionnaires  &nbsp;
#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_2.odt)    

#' 
#+fig.height=8, fig.width=7
essayer(produire_pyramides(c("TITULAIRE", "STAGIAIRE"), 
                           "Pyramide des âges des fonctionnaires",
                           versant = "TIT_" %+% VERSANT_FP,
                           e),
        "La pyramide des âges des fonctionnaires n'a pas pu être générée.")


#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_3.odt)     
#'  

newpage()

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

print(e$res)

#'  
#'[Lien vers la base des âges - début de période](Bases/Effectifs/`r e$nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](Bases/Effectifs/`r e$nom.fichier.après`.csv)  
#'[![Notice](Notice.png)](Docs/Notices/fiche_1.odt)      

#'   
#'   
#'   
#'   
#'   
#'   


########### 1.4 Pyramides non Tit ########################

#'## `r chapitre`.4 Pyramide des âges, personnels non titulaires   &nbsp; 
#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_2.odt)     
#'  
#+fig.height=8, fig.width=7
essayer(produire_pyramides(c("NON_TITULAIRE"), 
                           "Pyramide des âges des non titulaires", 
                           versant = "NONTIT_" %+% VERSANT_FP,
                           e),
        "La pyramide des âges des non titulaires n'a pas pu être générée." )


#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_3.odt)     
#'  

newpage()

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

print(e$res)


#'  
#'[Lien vers la base des âges - début de période](Bases/Effectifs/`r e$nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](Bases/Effectifs/`r e$nom.fichier.après`.csv)  
#'[![Notice](Notice.png)](Docs/Notices/fiche_1.odt)      

#'   
#'   
#'   
#'   
#'   
#'   


########### 1.5 Pyramides Autres statut ########################

#'## `r chapitre`.5 Pyramide des âges, autres statuts  &nbsp; 

#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_2.odt)   
#' 
#+fig.height=8, fig.width=7
Filtre_bulletins <<- setdiff(unique(Bulletins.paie$Statut), c("TITULAIRE", "NON_TITULAIRE", "STAGIAIRE")) 

essayer(produire_pyramides(Filtre_bulletins,
                           "Pyramide des âges des autres personnels",
                           versant = "",
                           e),
        "La pyramide des âges des autres personnels n'a pas pu être générée.")

#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_3.odt)     
#'  

newpage()

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

print(e$res)

#'  
#'[Lien vers la base des âges - début de période](Bases/Effectifs/`r e$nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](Bases/Effectifs/`r e$nom.fichier.après`.csv)  
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

