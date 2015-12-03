---
title: false
author: false
date: false
output:
html_document:
css: style.css
---
  
![Image_Altair](Altair.png)
  
  
# D�monstrateur Alta�r version 15.02   

**** C:\Users\Public\Dev\altair\R\bin\x64;C:\Program Files (x86)\Microsoft DirectX SDK (June 2010);C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64;C:\Python27\;C:\Python27\Scripts;C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.0\bin;C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v7.0\libnvvp;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;C:\Program Files (x86)\Microsoft SDKs\TypeScript\1.0\;C:\Program Files (x86)\NVIDIA Corporation\DDS Utilities;C:\Program Files (x86)\Common Files\Acronis\SnapAPI\;C:\Strawberry\c\bin;C:\Strawberry\perl\site\bin;C:\Strawberry\perl\bin;C:\Users\Fab\.dnx\bin;C:\Program Files\Microsoft DNX\Dnvm\;C:\Program Files (x86)\Calibre2\;C:\Program Files (x86)\QuickTime\QTSystem\;c:\users\public\dev\altair\r\bin;c:\users\public\dev\altair\r\bin\x64;C:\Users\Fab\AppData\Roaming\Python\Scripts;c:\Program Files (x86)\Git;E:\Users\Public\Altair\Git\bin;c:\Users\Public\Dev\altair\texlive\bin\win32;c:\Users\Public\Dev\altair\texlive\bin\win32;c:\Users\Public\Dev\altair\texlive\bin\win32;


<p class = "centered"><b>Exercices 2012 � 2014 </b></p>
<p class = "author">Fabrice Nicol</h1>




sam. 14 nov. 2015
   



# 1. Statistiques de population

### 1.1 Effectifs



 



&nbsp;*Tableau 1*   


|                                                             | 2012  | 2013  | 2014  |
|:------------------------------------------------------------|:-----:|:-----:|:-----:|
|Total effectifs (a)                                          | 354,0 | 360,0 | 403,0 |
|&nbsp;&nbsp;&nbsp;dont pr�sents 12 mois                      | 252,0 | 266,0 | 269,0 |
|&nbsp;&nbsp;&nbsp;dont fonctionnaires (b)                    | 223,0 | 245,0 | 261,0 |
|&nbsp;&nbsp;&nbsp;dont fonct. pr�sents 12 mois               | 215,0 | 230,0 | 245,0 |
|&nbsp;&nbsp;&nbsp;dont non titulaires                        | 72,0  | 70,0  | 80,0  |
|&nbsp;&nbsp;&nbsp;dont �lus                                  |  0,0  |  0,0  |  0,0  |
|&nbsp;&nbsp;&nbsp;dont �lus pr�sents 12 mois                 |  0,0  |  0,0  |  0,0  |
|&nbsp;&nbsp;&nbsp;dont vacataires d�tect�s (c)               |  0,0  |  0,0  |  0,0  |
|&nbsp;&nbsp;&nbsp;dont assistantes maternelles d�tect�es (c) |  0,0  |  0,0  |  0,0  |
|Postes non actifs (g)                                        | 14,0  | 15,0  | 33,0  |
|Postes annexes (g)                                           | 58,0  | 42,0  | 63,0  |
|Postes actifs non annexes (g)                                | 296,0 | 318,0 | 340,0 |
|Total ETP/ann�e (d)                                          | 320,3 | 329,1 | 340,4 |
|Total ETPT/ann�e (e)                                         | 270,8 | 294,7 | 295,1 |
|Total ETPT/ann�e personnes en place (f)(g)                   |  0,0  | 212,0 | 188,8 |
|Total ETPT/ann�e fonctionnaires (g)                          | 213,1 | 234,4 | 226,0 |
|Total ETPT/ann�e titulaires � temps complet (g)              | 184,5 | 184,0 | 161,8 |
|Total ETPT non titulaires (g)                                | 46,8  | 44,6  | 44,6  |
|Total ETPT autre statut                                      |  6,6  |  5,9  |  2,9  |
|Total ETPT postes non actifs (g)                             |  0,0  |  0,0  |  0,0  |
|Total ETPT postes annexes (g)                                |  5,2  |  5,8  |  3,7  |
|Total ETPT postes actifs non annexes (g)                     | 265,6 | 288,9 | 291,4 |


**Nota:**   
*(a) Nombre de matricules distincts ayant eu au moins un bulletin de paie dans l'ann�e, en fonction ou non.Peut correspondre � des r�gularisations, des personnels hors position d'activit� ou des ayants droit (reversion, etc.)*   
*(b) Titulaires ou stagiaires*   
*(c) Sur la base des libell�s d'emploi et des libell�s de lignes de paie. La d�tection peut �tre lacunaire*   
*(d) ETP  : Equivalent temps plein = r�mun�ration . quotit�*  
*(e) ETPT : Equivalent temps plein travaill� = ETP . 12/nombre de mois travaill�s dans l'ann�e*  
*(f) Personnes en place : pr�sentes en N et N-1 avec la m�me quotit�, postes actifs et non annexes uniquement.*     
*(g) Postes actifs et non annexes :* voir [Compl�ments m�thodologiques](Docs/m�thodologie.pdf)    
*&nbsp;&nbsp;&nbsp;Un poste actif est d�fini par au moins un bulletin de paie comportant un traitement positif pour un volume d'heures de travail mensuel non nul.*             
*&nbsp;&nbsp;&nbsp;Un poste non annexe est d�fini comme la conjonction de crit�res horaires et de revenu sur une ann�e. La p�riode minimale de r�f�rence est le mois.*   
*Les dix derni�res lignes du tableau sont calcul�es en ne tenant pas compte des �lus.*      
  
[Lien vers la base des effectifs](Bases/Effectifs/tableau.effectifs.csv)





### 1.2 Pyramide des �ges, personnels non �lus
 
&nbsp;*Tableau 2*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2012 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  16                  |          |
| 1er quartile  |                  39                  |          |
|    M�diane    |                  47                  |          |
|    Moyenne    |                  46                  |   279    |
| 3�me quartile |                  54                  |          |
|    Maximum    |                  77                  |          |

 

![plot of chunk unnamed-chunk-9](figure/unnamed-chunk-9-1.png) 

 
&nbsp;*Tableau 3*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2014 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  18                  |          |
| 1er quartile  |                  38                  |          |
|    M�diane    |                  47                  |          |
|    Moyenne    |                44,96                 |   309    |
| 3�me quartile |                  53                  |          |
|    Maximum    |                  70                  |          |

 
 

![plot of chunk unnamed-chunk-11](figure/unnamed-chunk-11-1.png) 


### 1.3 Pyramide des �ges, personnels non titulaires
 
&nbsp;*Tableau 4*   
   


|  Statistique  | �ge des personnels non titulaires<br>au 31/12/2012 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         20                         |          |
| 1er quartile  |                       30,25                        |          |
|    M�diane    |                        39,5                        |          |
|    Moyenne    |                       39,68                        |    38    |
| 3�me quartile |                         46                         |          |
|    Maximum    |                         63                         |          |

 

![plot of chunk unnamed-chunk-13](figure/unnamed-chunk-13-1.png) 

 
&nbsp;*Tableau 5*   
   


|  Statistique  | �ge des personnels non titulaires<br>au 31/12/2014 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         23                         |          |
| 1er quartile  |                       31,75                        |          |
|    M�diane    |                         42                         |          |
|    Moyenne    |                       42,71                        |    24    |
| 3�me quartile |                         49                         |          |
|    Maximum    |                         70                         |          |

![plot of chunk unnamed-chunk-14](figure/unnamed-chunk-14-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.nontit.csv)  
 


### 1.4 Pyramide des �ges, personnels fonctionnaires stagiaires et titulaires
 
&nbsp;*Tableau 6*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2012 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  21                  |          |
| 1er quartile  |                  41                  |          |
|    M�diane    |                  48                  |          |
|    Moyenne    |                46,92                 |   219    |
| 3�me quartile |                  53                  |          |
|    Maximum    |                  64                  |          |

 


![plot of chunk unnamed-chunk-16](figure/unnamed-chunk-16-1.png) 

 
&nbsp;*Tableau 7*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2014 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  21                  |          |
| 1er quartile  |                  39                  |          |
|    M�diane    |                  47                  |          |
|    Moyenne    |                45,64                 |   256    |
| 3�me quartile |                  53                  |          |
|    Maximum    |                  65                  |          |



![plot of chunk unnamed-chunk-18](figure/unnamed-chunk-18-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.fonctionnaires.csv)  
 
### 1.5 Pyramide des �ges, personnels permanents (titulaires, stagiaires et non titulaires)
 
&nbsp;*Tableau 8*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2012 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  20                  |          |
| 1er quartile  |                  40                  |          |
|    M�diane    |                  47                  |          |
|    Moyenne    |                45,85                 |   257    |
| 3�me quartile |                  53                  |          |
|    Maximum    |                  64                  |          |

 

![plot of chunk unnamed-chunk-20](figure/unnamed-chunk-20-1.png) 

&nbsp;*Tableau 9*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2014 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  21                  |          |
| 1er quartile  |                38,75                 |          |
|    M�diane    |                  47                  |          |
|    Moyenne    |                45,39                 |   280    |
| 3�me quartile |                  53                  |          |
|    Maximum    |                  70                  |          |



![plot of chunk unnamed-chunk-22](figure/unnamed-chunk-22-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.permanents.csv)  
 
### 1.6 Effectifs des personnels par dur�e de service

**Personnels en fonction (hors �lus) des exercices 2012 � 2014 inclus :**

 
&nbsp;*Tableau 10*   
   


| Plus de 2 ans | Moins de 2 ans | Moins d'un an | Moins de six mois |
|:-------------:|:--------------:|:-------------:|:-----------------:|
|      264      |      113       |      52       |        22         |



![plot of chunk unnamed-chunk-24](figure/unnamed-chunk-24-1.png) 


**Effectifs (hors �lus)**   




 
&nbsp;*Tableau 11*   
   


|                  | 2012 | 2013 | 2014 |
|:-----------------|:----:|:----:|:----:|
|Plus de deux ans  | 257  | 264  | 258  |
|Moins de deux ans |  39  |  54  |  82  |
|Total             | 296  | 318  | 340  |



**Nota :**
*Personnels en place : ayant servi au moins deux ann�es cons�cutives pendant la p�riode.*     
*Plus/moins de deux ans : plus/mois de 730 jours sur la p�riode sous revue.*   



  
  



   
# 2. R�mun�rations brutes : analyse pour l'exercice 2012    
   
## 2.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des r�mun�rations brutes pour l'exercice 2012   
 
*Personnels (hors �lus)*     
 
&nbsp;*Tableau 12*   
   


|           Agr�gats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 6 538 348,5 |
| Brut annuel (lignes), dont : | 6 938 814,0 |
|         \ \ Primes :         |  839 098,1  |
|   \ \ Autres r�mun�rations   |     0,0     |
|     Part de primes en %      |    12,8     |

   
*Elus*    
 
 
&nbsp;*Tableau 13*   
   


|           Agr�gats           | k&euro; |
|------------------------------|---------|
|   Brut annuel (bulletins)    |   0,0   |
| Brut annuel (lignes), dont : |   0,0   |
|         \ \ Primes :         |   0,0   |
|   \ \ Autres r�mun�rations   |   0,0   |

    
**D�finitions :**   

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lu*          : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
    
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   
 
 
&nbsp;*Tableau 14*    
   


|     Agr�gats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 6 538 348,5 |
|  Lignes de paie   | 6 938 814,0 |
|    Diff�rence     | -400 465,5  |

   
� comparer aux soldes des comptes 641 et 648 du compte de gestion.   

Somme des r�mun�rations brutes vers�es (�lus) :     
 
 
&nbsp;*Tableau 15*   
   


|     Agr�gats      | k&euro; |
|-------------------|---------|
| Bulletins de paie |   0,0   |
|  Lignes de paie   |   0,0   |
|    Diff�rence     |   0,0   |

  
## 2.2 Masse salariale brute des fonctionnaires    

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*   


![plot of chunk unnamed-chunk-126](figure/unnamed-chunk-126-1.png) 
![plot of chunk unnamed-chunk-127](figure/unnamed-chunk-127-1.png) 
![plot of chunk unnamed-chunk-128](figure/unnamed-chunk-128-1.png) 
![plot of chunk unnamed-chunk-129](figure/unnamed-chunk-129-1.png) 
![plot of chunk unnamed-chunk-130](figure/unnamed-chunk-130-1.png) 

![plot of chunk unnamed-chunk-131](figure/unnamed-chunk-131-1.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio r�mun�ration / quotit�*   
   
**Effectif : 219**   
   
**Tests de coh�rence**   


   
Somme des r�mun�rations brutes vers�es aux personnels titulaires et stagiaires :    

 
&nbsp;*Tableau 16*   
   


|           Agr�gats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 5 357 770,6 |
| Brut annuel (lignes), dont : | 5 612 180,3 |
|         \ \ Primes :         |  733 963,3  |
|   \ \ Autres r�mun�rations   |     0,0     |
|     Part de primes en %      |    13,7     |

    
**D�finitions :**     
   
 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lus*         : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
   
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   

 
&nbsp;*Tableau 17*      
   


|     Agr�gats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 5 357 770,6 |
|  Lignes de paie   | 5 612 180,3 |
|    Diff�rence     | -254 409,7  |

   
A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.    

**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2012**      
 
&nbsp;*Tableau 18*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres r�mun�rations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 3 013|    703,8|                    0|         |
|  1er quartile|                19 191|  1 072,3|                    0|         |
|       M�diane|                21 281|  1 773,6|                    0|         |
|       Moyenne|                22 364|  3 365,7|                    0|      218|
| 3�me quartile|                24 352|  3 740,5|                    0|         |
|       Maximum|                64 570| 48 702,4|                    0|         |

 
&nbsp;*Tableau 19*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la r�m. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     3 966|      3 966|             4 013|                        3,614|         |
|  1er quartile|                    20 630|     19 795|            19 886|                        5,439|         |
|       M�diane|                    23 505|     21 940|            22 078|                        8,353|         |
|       Moyenne|                    25 730|     24 563|            24 759|                       11,257|      218|
| 3�me quartile|                    27 692|     26 165|            26 013|                       14,839|         |
|       Maximum|                   113 272|    110 823|           102 637|                       58,727|         |

   
*Hors vacataires identifi�s, assistantes maternelles, �lus locaux et pour les postes actifs non annexes*   

**Cat�gorie A**    


Pas de statistique en l'absence de fichier des cat�gories.

    
**Effectif : 0**  

**Cat�gorie B**    
    

Pas de statistique en l'absence de fichier des cat�gories.


**Effectif : 0**    

**Cat�gorie C**    


Pas de statistique en l'absence de fichier des cat�gories.

**Effectif : 0**    

## 2.3 Contractuels, vacataires et stagiaires inclus     
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   



![plot of chunk unnamed-chunk-140](figure/unnamed-chunk-140-1.png) 

  
**Nota :**   
*Ne sont retenues que les r�mun�rations sup�rieures � 1 000 k&euro;.*  
*Les �lus ne sont pas pris en compte.*  
  



  
**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2012**     
  
 
&nbsp;*Tableau 20*   
   


|   Statistique|   Primes| Autres r�mun�rations| Effectif|
|-------------:|--------:|--------------------:|--------:|
|       Minimum|      0,0|                    0|         |
|  1er quartile|    138,1|                    0|         |
|       M�diane|  1 060,1|                    0|         |
|       Moyenne|  1 289,9|                    0|       78|
| 3�me quartile|  1 533,9|                    0|         |
|       Maximum| 12 207,9|                    0|         |

   
&nbsp;*Tableau 21*   
   
   

|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               4 071|                    4 868|         |
|  1er quartile|               8 318|                   16 543|         |
|       M�diane|              12 393|                   19 188|         |
|       Moyenne|              14 224|                   20 179|       78|
| 3�me quartile|              16 499|                   21 910|         |
|       Maximum|              86 781|                   86 544|         |


  
  



   
# 3. R�mun�rations brutes : analyse pour l'exercice 2014    
   
## 3.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des r�mun�rations brutes pour l'exercice 2014   
 
*Personnels (hors �lus)*     
 
&nbsp;*Tableau 22*   
   


|           Agr�gats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 7 824 528,3 |
| Brut annuel (lignes), dont : | 7 662 638,3 |
|         \ \ Primes :         | 1 319 834,2 |
|   \ \ Autres r�mun�rations   |  121 494,4  |
|     Part de primes en %      |    16,9     |

   
*Elus*    
 
 
&nbsp;*Tableau 23*   
   


|           Agr�gats           | k&euro; |
|------------------------------|---------|
|   Brut annuel (bulletins)    |   0,0   |
| Brut annuel (lignes), dont : |   0,0   |
|         \ \ Primes :         |   0,0   |
|   \ \ Autres r�mun�rations   |   0,0   |

    
**D�finitions :**   

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lu*          : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
    
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   
 
 
&nbsp;*Tableau 24*    
   


|     Agr�gats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 7 824 528,3 |
|  Lignes de paie   | 7 662 638,3 |
|    Diff�rence     |  161 890,0  |

   
� comparer aux soldes des comptes 641 et 648 du compte de gestion.   

Somme des r�mun�rations brutes vers�es (�lus) :     
 
 
&nbsp;*Tableau 25*   
   


|     Agr�gats      | k&euro; |
|-------------------|---------|
| Bulletins de paie |   0,0   |
|  Lignes de paie   |   0,0   |
|    Diff�rence     |   0,0   |

  
## 3.2 Masse salariale brute des fonctionnaires    

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*   


![plot of chunk unnamed-chunk-150](figure/unnamed-chunk-150-1.png) 
![plot of chunk unnamed-chunk-151](figure/unnamed-chunk-151-1.png) 
![plot of chunk unnamed-chunk-152](figure/unnamed-chunk-152-1.png) 
![plot of chunk unnamed-chunk-153](figure/unnamed-chunk-153-1.png) 
![plot of chunk unnamed-chunk-154](figure/unnamed-chunk-154-1.png) 

![plot of chunk unnamed-chunk-155](figure/unnamed-chunk-155-1.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio r�mun�ration / quotit�*   
   
**Effectif : 249**   
   
**Tests de coh�rence**   


   
Somme des r�mun�rations brutes vers�es aux personnels titulaires et stagiaires :    

 
&nbsp;*Tableau 26*   
   


|           Agr�gats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 6 273 012,3 |
| Brut annuel (lignes), dont : | 6 073 430,5 |
|         \ \ Primes :         | 1 055 207,0 |
|   \ \ Autres r�mun�rations   |  31 871,8   |
|     Part de primes en %      |    16,8     |

    
**D�finitions :**     
   
 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lus*         : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
   
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   

 
&nbsp;*Tableau 27*      
   


|     Agr�gats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 6 273 012,3 |
|  Lignes de paie   | 6 073 430,5 |
|    Diff�rence     |  199 581,8  |

   
A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.    

**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2014**      
 
&nbsp;*Tableau 28*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres r�mun�rations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 5 871|    738,5|                  0,0|         |
|  1er quartile|                17 784|  1 674,6|                  0,0|         |
|       M�diane|                19 395|  2 784,9|                195,4|         |
|       Moyenne|                20 537|  4 322,7|                130,6|      244|
| 3�me quartile|                22 159|  4 970,3|                195,4|         |
|       Maximum|                48 066| 57 490,0|                295,4|         |

 
&nbsp;*Tableau 29*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la r�m. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     7 301|      7 301|                 0|                         3,75|         |
|  1er quartile|                    20 052|     20 843|            21 079|                         7,84|         |
|       M�diane|                    22 856|     23 640|            23 840|                        12,22|         |
|       Moyenne|                    24 860|     25 678|            28 635|                        14,63|      244|
| 3�me quartile|                    26 262|     27 321|            29 866|                        18,53|         |
|       Maximum|                    98 163|     99 718|           122 778|                        57,65|         |

   
*Hors vacataires identifi�s, assistantes maternelles, �lus locaux et pour les postes actifs non annexes*   

**Cat�gorie A**    


Pas de statistique en l'absence de fichier des cat�gories.

    
**Effectif : 0**  

**Cat�gorie B**    
    

Pas de statistique en l'absence de fichier des cat�gories.


**Effectif : 0**    

**Cat�gorie C**    


Pas de statistique en l'absence de fichier des cat�gories.

**Effectif : 0**    

## 3.3 Contractuels, vacataires et stagiaires inclus     
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   



![plot of chunk unnamed-chunk-164](figure/unnamed-chunk-164-1.png) 

  
**Nota :**   
*Ne sont retenues que les r�mun�rations sup�rieures � 1 000 k&euro;.*  
*Les �lus ne sont pas pris en compte.*  
  

![plot of chunk unnamed-chunk-165](figure/unnamed-chunk-165-1.png) 

  
**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2014**     
  
 
&nbsp;*Tableau 30*   
   


|   Statistique|   Primes| Autres r�mun�rations| Effectif|
|-------------:|--------:|--------------------:|--------:|
|       Minimum|      0,0|                 0,00|         |
|  1er quartile|    448,2|                 0,00|         |
|       M�diane|  1 261,2|                 0,00|         |
|       Moyenne|  1 832,4|                34,17|       96|
| 3�me quartile|  2 335,6|                 0,00|         |
|       Maximum| 13 491,6|               615,76|         |

   
&nbsp;*Tableau 31*   
   
   

|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               4 368|                    3 061|         |
|  1er quartile|               7 936|                   17 344|         |
|       M�diane|              13 060|                   19 872|         |
|       Moyenne|              14 488|                   26 517|       96|
| 3�me quartile|              18 968|                   23 603|         |
|       Maximum|              55 012|                  242 185|         |

 
[Lien vers la base des r�mun�rations](Bases/R�mun�rations/Analyse.r�mun�rations.csv)  
  



  
## 3.4 Comparaisons source INSEE/DGCL   
  
*Salaires annnuels moyens 2011 en EQTP (hors assistantes maternelles)*   
 
&nbsp;*Tableau 32*   
   


|     Agr�gat     | Salaires bruts 2011 (&euro;) | Salaires bruts 2012 (&euro;) |
|-----------------|------------------------------|------------------------------|
|    Ensemble     |           25 908,0           |           26 340,0           |
|   Titulaires    |           26 676,0           |           27 108,0           |
| Autres salari�s |           22 836,0           |              NA              |

  
**El�ments de la r�mun�ration brute pour les titulaires de la FPT entre 2010 et 2012**      
  
 
&nbsp;*Tableau 33*   
   


|         R�m. annuelles          | 2010 (&euro;) | Primes (%) | 2011 (&euro;) | Primes (%) | 2012 (&euro;) | Primes (%) |
|---------------------------------|---------------|------------|---------------|------------|---------------|------------|
|          Salaire brut           |     26305     |            |     26660     |            |     27108     |            |
|         Traitement brut         |     20350     |    22,6    |     20562     |    22,9    |     20724     |    23,6    |
| Primes et r�mun�rations annexes |               |            |               |            |               |            |
|       y compris IR et SFT       |     5955      |            |     6098      |            |     6384      |            |

  
*Champ : France. Salari�s en �quivalent-temps plein (EQTP) des collectivit�s territoriales (y compris b�n�ficiaires de contrats aid�s, hors assistantes maternelles).*   												
*Les primes sont cumul�es au suppl�ment familial de traitement (SFT) et � l'indemnit� de r�sidence (IR). Le cumul est rapport� � la r�mun�ration brute totale.*    
[Source INSEE](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)    
[Source DGCL](http://www.fonction-publique.gouv.fr/files/files/statistiques/rapports_annuels/2012-2013/xls/Vue3_1_Remunerations.xls)    
[Source DGFIP PLF 2015](http://www.performance-publique.budget.gouv.fr/sites/performance_publique/files/farandole/ressources/2015/pap/pdf/jaunes/jaune2015_fonction_publique.pdf)   
  




# 4. R�mun�rations nettes : �volutions sur la p�riode 2012 - 2014    

Nombre d'exercices: 3   
 
**Les donn�es pr�sent�es dans cette section sont toutes relatives � des r�mun�rations nettes en �quivalent temps plein (EQTP)**   
Les �lus, les vacataires et les assistantes maternelles ont �t� retir�s de la population �tudi�e       
Seuls sont consid�r�s les postes actifs et non annexes   
   
*Nota :*   
*EQTP = Equivalent temps plein  = 12 . moyenne du ratio r�mun�ration / quotit�*    
   
## 4.1 Distribution de la r�mun�ration nette moyenne sur la p�riode    

![plot of chunk unnamed-chunk-32](figure/unnamed-chunk-32-1.png) 



![plot of chunk unnamed-chunk-33](figure/unnamed-chunk-33-1.png) 


[Lien vers la base de donn�es synth�tique](Bases/R�mun�rations/Analyse.variations.synth�se.csv)
[Lien vers la base de donn�es d�taill�e par ann�e](Bases/R�mun�rations/Analyse.variations.par.exercice.csv)

## 4.2 Evolutions des r�mun�rations nettes sur la p�riode 2012 - 2014   

### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors �lus)





   
**Salaire net moyen par t�te (SMPT net) en EQTP, hors �lus**         
      
 
&nbsp;*Tableau 34*   
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-----------------:|:-------------:|
| 2012  |           5 620,2           |               |      18 987       |               |
| 2013  |           6 061,3           |     35,9      |      19 181       |      19       |
| 2014  |           7 637,1           |               |      22 595       |               |

  
**Effet de noria sur salaires nets et taux de remplacements**       
  
**Effet de noria** : *diff�rence entre la r�mun�ration annuelle des entrants � l'ann�e N et des sortants � l'ann�e N-1*.  
*Usuellement calcul�e sur les r�mun�rations brutes, ici sur les r�mun�rations nettes EQTP*  
*afin d'appr�cier l'impact de cet effet sur l'�volution des r�mun�rations nette moyennes calcul�e au tableau pr�c�dent.*               
 
&nbsp;*Tableau 35*   
   


| Ann�e | Noria EQTP (&euro;) | En % de la MS N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-----------------:|:------------------:|:-------------------------:|
| 2013  |       349,59        |      0,0 685      |         11         |            3,7            |
| 2014  |      -2 196,2       |      -0,493       |         14         |            4,6            |


*MS N-1 : masse salariale nette de l'ann�e n-1.*   
      
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) en EQTP**         
      
 
&nbsp;*Tableau 36*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          3 335|
|  1er quartile|         15 370|
|       M�diane|         17 250|
|       Moyenne|         18 990|
| 3�me quartile|         20 330|
|       Maximum|         85 750|



|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|          2 820|
|  1er quartile|         16 070|
|       M�diane|         18 050|
|       Moyenne|         22 590|
| 3�me quartile|         22 290|
|       Maximum|        201 900|

 
*Nota :*  La population retenue est constitu�e des agents qui :   
&nbsp;&nbsp;- ne font pas partie des 2 centiles extr�maux   
&nbsp;&nbsp;- sont au moins pr�sents 1 jour(s) la premi�re et la derni�re ann�e d'activit�  
Les �lus, vacataires et assistantes maternelles sont retir�s du p�rim�tre.   
Seuls sont pris en compte les agents ayant connu au moins un mois actif et ayant eu, sur l'ann�e, des r�mun�rations non annexes.  
[Compl�ments m�thodologiques](Docs/m�thodologie.pdf)     
     
**Comparaisons source INSEE/DGCL**   

**Salaires annuels moyens 2011 et 2012 en EQTP (hors assistantes maternelles)**   
 
&nbsp;*Tableau 37*       


|     Agr�gat     | Salaires nets 2011 (&euro;) | Salaires nets 2012 (&euro;) |
|-----------------|-----------------------------|-----------------------------|
|    Ensemble     |          21 876,0           |          22 176,0           |
|   Titulaires    |          22 632,0           |          22 920,0           |
| Autres salari�s |          18 864,0           |             NA              |

*Champ : France. Salari�s en �quivalent-temps plein (EQTP) des collectivit�s territoriales (y compris b�n�ficiaires de contrats aid�s, hors assistantes maternelles).*     			



**Distribution des salaires nets annuels en EQTP dans la fonction publique par versant en 2011**   

 
&nbsp;*Tableau 38*   
   


| D�cile (k&euro;) |   FPE    |   FPT    |   FPH    | Secteur priv� |
|------------------|----------|----------|----------|---------------|
|        D1        | 17 496,0 | 15 288,0 | 16 584,0 |   14 040,0    |
|        D2        | 20 916,0 | 16 512,0 | 18 168,0 |   15 660,0    |
|        D3        | 23 052,0 | 17 508,0 | 19 620,0 |   17 136,0    |
|        D4        | 24 912,0 | 18 480,0 | 21 048,0 |   18 708,0    |
|        D5        | 26 832,0 | 19 632,0 | 22 596,0 |   20 544,0    |
|        D6        | 28 944,0 | 21 012,0 | 24 504,0 |   22 824,0    |
|        D7        | 31 632,0 | 22 860,0 | 27 216,0 |   25 872,0    |
|        D8        | 35 592,0 | 25 596,0 | 30 996,0 |   30 828,0    |
|        D9        | 42 456,0 | 30 876,0 | 37 812,0 |   40 800,0    |

**Distribution des salaires nets annuels en EQTP dans la fonction publique territoriale par cat�gorie en 2011**   
 
&nbsp;*Tableau 39*   
   


| D�cile (k&euro;) | Cat�gorie A | Cat�gorie B | Cat�gorie C | Autres salari�s |
|------------------|-------------|-------------|-------------|-----------------|
|        D1        |  26 040,0   |  20 580,0   |  15 972,0   |    13 620,0     |
|        D2        |  28 992,0   |  22 272,0   |  16 896,0   |    14 340,0     |
|        D3        |  31 272,0   |  23 652,0   |  17 652,0   |    15 024,0     |
|        D4        |  33 468,0   |  24 960,0   |  18 360,0   |    15 684,0     |
|        D5        |  35 820,0   |  26 244,0   |  19 164,0   |    16 368,0     |
|        D6        |  38 664,0   |  27 636,0   |  20 100,0   |    17 232,0     |
|        D7        |  42 276,0   |  29 160,0   |  21 216,0   |    18 480,0     |
|        D8        |  47 124,0   |  30 984,0   |  22 680,0   |    20 784,0     |
|        D9        |  54 840,0   |  33 804,0   |  24 996,0   |    26 916,0     |
|     Moyenne      |  21 876,0   |  38 700,0   |  26 928,0   |    20 016,0     |

[Source INSEE, onglets Figure3, F1web et F3web](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)   
  
[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)
  
### 4.2.2 Fonctionnaires

**Titulaires et stagiaires**      



**Salaire net moyen par t�te (SMPT net) en EQTP**       
&nbsp;*Tableau 40*   
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2012  |           4 327,1           |               |          19 849           |               |
| 2013  |           4 810,1           |     28,5      |          20 382           |     15,7      |
| 2014  |           5 560,1           |               |          22 975           |               |





  
**Effet de noria sur salaires nets et taux de remplacements**       
 
&nbsp;*Tableau 41*   
   


| Ann�e | Noria EQTP (&euro;) | En % de la  MSN N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-------------------:|:------------------:|:-------------------------:|
| 2013  |      -4 213,4       |       -0,472        |         5          |            2,1            |
| 2014  |       -14 738       |        -2,34        |         8          |            3,4            |

    
*MS N-1 : masse salariale nette de l'ann�e n-1.*   
      
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) en EQTP**         
 
&nbsp;*Tableau 42*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          3 335|
|  1er quartile|         15 930|
|       M�diane|         17 820|
|       Moyenne|         19 850|
| 3�me quartile|         20 750|
|       Maximum|         85 750|



|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|          3 526|
|  1er quartile|         16 760|
|       M�diane|         18 670|
|       Moyenne|         22 980|
| 3�me quartile|         24 190|
|       Maximum|        101 400|

   



  
**Evolution du SMPT net des titulaires � temps complet**     
  
 
&nbsp;*Tableau 43*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | SMPT (&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:-------------:|:-------------:|
| 2012  |               3 581,7               |               |    19 256     |               |
| 2013  |               3 750,5               |     -9,53     |    20 383     |     3,24      |
| 2014  |               3 240,5               |               |    19 880     |               |

   
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) des titulaires � temps complet**         
      
 
&nbsp;*Tableau 44*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          8 792|
|  1er quartile|         15 890|
|       M�diane|         17 530|
|       Moyenne|         19 260|
| 3�me quartile|         20 450|
|       Maximum|         81 430|

  


|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|         13 070|
|  1er quartile|         16 410|
|       M�diane|         18 110|
|       Moyenne|         19 880|
| 3�me quartile|         20 690|
|       Maximum|         86 830|


[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)


## 4.3 Glissement vieillesse-technicit� (GVT)   

### 4.3.1 Ensemble des personnels   
  
*Cette section est consacr�e � la r�mun�ration moyenne des personnes en place (RMPP), d�finies comme pr�sentes deux ann�es enti�res cons�cutives avec la m�me quotit�*   
*L'�volution de la RMPP permet d'�tudier le glissement vieillesse-technicit� "positif", � effectifs constants sur deux ann�es*      
*Le GVT positif est d� aux mesures statutaires et individuelles, � l'avancement et aux changements d'activit�*  

![plot of chunk unnamed-chunk-51](figure/unnamed-chunk-51-1.png) 






  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 45*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2013  |               4 262,7               |     -12,5     |        19 919        |     -1,47     |
| 2014  |               3 729,1               |               |        19 627        |               |

   
**Distribution et variation sur la p�riode de la r�mun�ration nette des personnes en place**                
 
&nbsp;*Tableau 46*   
   



 


|   Statistique| Premi�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          4 966|         |
|  1er quartile|         15 440|         |
|       M�diane|         17 250|         |
|       Moyenne|         19 100|      268|
| 3�me quartile|         20 420|         |
|       Maximum|         85 750|         |

 
&nbsp;*Tableau 47*   
   


|   Statistique| Derni�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          6 535|         |
|  1er quartile|         16 390|         |
|       M�diane|         18 510|         |
|       Moyenne|         22 020|      289|
| 3�me quartile|         22 600|         |
|       Maximum|        101 400|         |


*Variation individuelle de r�mun�ration nette en EQTP pour les personnels pr�sents la premi�re et la derni�re ann�e*   
 
&nbsp;*Tableau 48*   
   


|   Statistique| Variation normalis�e (%)| Variation annuelle moyenne normalis�e (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -27,117|                                   -14,628|         |
|  1er quartile|                    2,906|                                     1,442|         |
|       M�diane|                    7,727|                                     3,792|         |
|       Moyenne|                   19,150|                                     8,429|      252|
| 3�me quartile|                   19,778|                                     9,443|         |
|       Maximum|                  124,577|                                    49,859|         |

   
### 4.3.2 Titulaires et stagiaires     
  



  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 49*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2013  |               3 957,9               |     -10,7     |        19 789        |    -0,227     |
| 2014  |               3 534,2               |               |        19 744        |               |

   
**Distribution et variation sur la p�riode de la r�mun�ration nette des fonctionnaires en place**                
 
&nbsp;*Tableau 50*   
   
 


|   Statistique| Premi�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          8 792|         |
|  1er quartile|         15 680|         |
|       M�diane|         17 420|         |
|       Moyenne|         19 570|      228|
| 3�me quartile|         20 510|         |
|       Maximum|         85 750|         |

 
&nbsp;*Tableau 51*   
   


|   Statistique| Derni�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|         10 030|         |
|  1er quartile|         16 910|         |
|       M�diane|         18 800|         |
|       Moyenne|         23 320|      249|
| 3�me quartile|         24 210|         |
|       Maximum|        101 400|         |


*Variation individuelle de r�mun�ration nette en EQTP pour les personnels pr�sents la premi�re et la derni�re ann�e*   
 
&nbsp;*Tableau 52*   
   


|   Statistique| Variation normalis�e (%)| Variation annuelle moyenne normalis�e (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -18,003|                                    -9,448|         |
|  1er quartile|                    3,415|                                     1,693|         |
|       M�diane|                    8,331|                                     4,082|         |
|       Moyenne|                   20,799|                                     9,193|      218|
| 3�me quartile|                   20,438|                                     9,744|         |
|       Maximum|                  124,577|                                    49,859|         |



[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)

**Nota**   
*Personnes en place :* en fonction au moins deux ann�es cons�cutives sur la p�riode 2012 � 2014    
*Variation sur la p�riode d'activit� :* entre l'arriv�e et le d�part de la personne      
*Variation normalis�e :* conforme � la d�finition INSEE (pr�sente en d�but et en fin de p�riode avec la m�me quotit�)  
 
**Commentaire**       
Les diff�rences �ventuelles constat�es entre l'�volution de la RMPP au tableau 50 sont dues soit � l'effet de noria soit � l'effet p�rim�tre.    
     
[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)


### 4.4 Comparaisons avec la situation nationale des r�mun�rations   
 
**�volution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    
 
&nbsp;*Tableau 53*   
   


|   Ann�e    | 2008-2009 | 2009-2010 | 2010-2011 | 2011-1012 | 2008-2012 | Moy. 2008-2012 | M�diane 2007-2011 |
|------------|-----------|-----------|-----------|-----------|-----------|----------------|-------------------|
| SMPT brut  |    2,5    |    1,3    |    1,5    |    1,7    |    7,2    |      1,8       |                   |
|  SMPT net  |    3,0    |    1,4    |    1,3    |    1,4    |    7,3    |      1,8       |       13,4        |
| RMPP brute |    3,3    |    2,5    |    2,5    |    2,7    |   11,5    |      2,8       |                   |
| RMPP nette |    3,3    |    2,5    |    2,3    |    2,4    |   10,9    |      2,6       |                   |

*Source : fichier g�n�ral de l'�tat (FGE), DADS, SIASP, Insee, Drees. Traitement Insee, Drees, DGCL*    
Hors assistants maternels et familiaux, y compris b�n�ficiaires de contrats aid�s.   
SMPT : Salaire moyen par t�te en EQTP.   
RMPP : Agents pr�sents 24 mois cons�cutifs chez le m�me employeur avec la m�me quotit� de travail.  
Moyenne des variations g�om�triques annuelles pour les agents du champ.  
La derni�re colonne pr�sente la m�diane des augmentations du SMPT net pour les agents pr�sents en 2007 et 2011.   
 
**Salaires nets annuels et �volution moyenne type de collectivit� en &euro; courants  EQTP**    
  
 
&nbsp;*Tableau 54*   
   


|       Type de collectivit�        | SMPT net 2011 (&euro;) | SMPT net 2012 (&euro;) | �volution annuelle moy. 2007-2011 (%) |
|-----------------------------------|------------------------|------------------------|---------------------------------------|
|             Communes              |        20 784,0        |        21 120,0        |                  2,5                  |
|    CCAS et caisses des �coles     |        19 415,0        |        19 716,0        |                  2,4                  |
|      EPCI � fiscalit� propre      |        22 882,0        |        23 088,0        |                  3,1                  |
| Autres structures intercommunales |        21 299,0        |        21 684,0        |                  3,0                  |
|           D�partements            |        24 487,0        |        24 744,0        |                  3,9                  |
|               SDIS                |        29 811,0        |        29 940,0        |                  3,4                  |
|              R�gions              |        22 432,0        |        22 836,0        |                  3,8                  |
|   Autres collectivit�s locales    |        24 680,0        |        24 696,0        |                  3,2                  |
|        Ensemble (moyenne)         |        21 873,0        |        22 176,0        |                  2,9                  |


*Champ : France. Salari�s en �quivalent-temps plein (EQTP) des collectivit�s territoriales (y compris b�n�ficiaires de contrats aid�s, hors assistantes maternelles).*     			
Conversion en euros courants, calcul CRC.  
[Source INSEE donn�es 2011 obsol�tes](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)   
[Source DGAFP](http://infos.emploipublic.fr/dossiers/la-fonction-publique-en-chiffres/la-fonction-publique-en-chiffre-2013/non-defini-08/apm-71444/)  
[Source PLF 2014 donn�es 2011 r�vis�es p.151](http://www.fonction-publique.gouv.fr/files/files/statistiques/jaunes/jaune2014_FP.pdf)   
[Source PLF 2015 donn�es 2012 p.130](http://www.performance-publique.budget.gouv.fr/sites/performance_publique/files/farandole/ressources/2015/pap/pdf/jaunes/jaune2015_fonction_publique.pdf)   
   











# 5. Tests r�glementaires   
## 5.1 Contr�le des heures suppl�mentaires, des NBI et primes informatiques   
  
**Dans cette partie, l'ensemble de la base de paie est �tudi�.**  
Les agents non actifs ou dont le poste est annexe sont r�introduits dans le p�rim�tre.   



Primes informatiques potentielles : aucune
 
&nbsp;*Tableau 55*   
   


| Nombre de lignes NBI pour non titulaires | Nombre de b�n�ficiaires de PFI |
|:----------------------------------------:|:------------------------------:|
|                    0                     |               0                |

  
[Lien vers la base de donn�es NBI aux non titulaires](Bases/R�glementation/NBI.aux.non.titulaires.csv)   
[Lien vers la base de donn�es Primes informatiques](Bases/R�glementation/personnels.prime.informatique.csv)   
  
**Nota :**   
NBI: nouvelle bonification indiciaire   
PFI: prime de fonctions informatiques   




 
&nbsp;*Tableau 56*   
   


| R�mun�rations de NBI anormales, par agent et par exercice | Montants correspondants |
|:---------------------------------------------------------:|:-----------------------:|
|                            99                             |         74 921          |

  
[Lien vers la base de donn�es NBI anormales](Bases/Fiabilit�/lignes.nbi.anormales.csv)   
  
**Nota :**   
*Est consid�r� comme manifestement anormal un total annuel de r�mun�rations NBI correspondant � un point d'indice net mensuel inf�rieur � 4 euros ou sup�rieur � 6 euros.*    
*Les rappels ne sont pas pris en compte dans les montants vers�s. Certains �carts peuvent �tre r�gularis�s en les prenant en compte*     
 
&nbsp;*Tableau 57*   
   


| Ann�e | Cumuls des NBI | Montants vers�s (a) | Point d'INM apparent | Point d'INM moyen | Contr�le |
|-------|----------------|---------------------|----------------------|-------------------|----------|
| 2012  |    32 742,0    |      37 381,4       |         1,1          |        4,6        |  Rouge   |
| 2013  |    33 061,0    |      34 598,0       |         1,0          |        4,6        |  Rouge   |
| 2014  |    6 820,0     |      31 858,7       |         4,7          |        4,6        |   Vert   |


*(a) Hors rappels sur r�mun�rations*   




  
[Lien vers la base de donn�es des cumuls annuels de NBI](Bases/Fiabilit�/cumuls.nbi.csv)   
  
 
## 5.2 Contr�le des vacations pour les fonctionnaires





Il y a  0 fonctionnaire(s) effectuant des vacations pour son propre �tablissement. Les bulletins concern�s sont donn�s en lien.


[Lien vers les matricules des fonctionnaires concern�s](Bases/R�glementation/matricules.fonctionnaires.et.vacations.csv)
[Lien vers les bulletins de paie correspondants](Bases/R�glementation/lignes.fonctionnaires.et.vacations.csv)

## 5.3 Contr�les sur les cumuls traitement indiciaire, indemnit�s et vacations des contractuels    




**Contractuels effectuant des vacations (CEV)**

 
&nbsp;*Tableau 58*   
   


| Nombre de CEV | Nombre de lignes | Nombre de lignes indemnitaires | Nombre de lignes de traitement |
|:-------------:|:----------------:|:------------------------------:|:------------------------------:|
|       0       |        0         |               0                |               0                |

 
[Lien vers le bulletins des CEV](Bases/R�glementation/lignes.contractuels.et.vacations.csv)   
[Lien vers la base de donn�es Matricules des CEV](Bases/R�glementation/matricules.contractuels.et.vacations.csv)  
[Lien vers la base de donn�es Cumul r�gime indemnitaire et vacations de CEV](Bases/R�glementation/RI.et.vacations.csv)  
[Lien vers la base de donn�es Lignes de traitement indiciaire pour CEV](Bases/R�glementation/traitement.et.vacations.csv)  
 




## 5.4 Contr�le sur les indemnit�s IAT et IFTS      




 
&nbsp;*Tableau 59*   
     

Tests IAT/IFTS sans r�sultat positif.

  
[Codes IFTS retenus](Bases/R�glementation/codes.ifts.csv)   
[Lien vers la base de donn�es cumuls iat/ifts](Bases/R�glementation/personnels.iat.ifts.csv)    

### Contr�le sur les IFTS pour cat�gories B et contractuels



 



 




 
&nbsp;*Tableau 60*   
   


| Nombre de lignes de paie de contractuels percevant des IFTS | Nombre de lignes IFTS pour IB < 380 |
|:-----------------------------------------------------------:|:-----------------------------------:|
|                             41                              |                  0                  |


[Lien vers la base de donn�es Lignes IFTS pour contractuels](Bases/R�glementation/ifts.et.contractuel.csv)    
[Lien vers la base de donn�es Lignes IFTS pour IB < 380](Bases/R�glementation/lignes.ifts.anormales.csv)     

**Nota :**
IB < 380 : fonctionnaire percevant un indice brut inf�rieur � 380





## 5.5 Contr�le de la prime de fonctions et de r�sultats (PFR)   
  




 
&nbsp;*Tableau 61*   
     


|   Codes IFTS   |      Codes PFR      | Agents cumulant PFR et IFTS |
|:--------------:|:-------------------:|:---------------------------:|
| 3040 304A 3049 | 3046 3111 3112 325A |              0              |

  
[Lien vers la base de donn�es cumuls pfr/ifts](Bases/R�glementation/personnels.pfr.ifts.csv)    




 
&nbsp;*Tableau 62 : rappel des plafonds annuels de la PFR*   
     


| Adm. g�n�ral | Adm. HC |  Adm.  | Direct./Attach� princ. | Secr. mairie/Attach� |
|:------------:|:-------:|:------:|:----------------------:|:--------------------:|
|    58 800    | 55 200  | 49 800 |         25 800         |        20 100        |


Les plafonds annuels de la PFR de sont pas d�pass�s.

 
&nbsp;*Tableau 63* : Valeurs de l'agr�gat (PFR ou IFTS) pour les b�n�ficiaires de la PFR   
         


|            Nom| Matricule| Ann�e| Cumul PFR ou IFTS|                    Grade|
|--------------:|---------:|-----:|-----------------:|------------------------:|
|          ALARY|      3184|  2012|             1 867|  Technicien ppal 1�re Cl|
|          ALARY|      3184|  2013|             2 800|  Technicien ppal 1�re Cl|
|          ALARY|      3184|  2014|             1 400|  TECHNICIEN PPAL 1ERE CL|
|       ARTIGUES|      0404|  2013|             2 817|      Ing�nieur Principal|
|       ARTIGUES|      0404|  2014|             1 408|      INGENIEUR PRINCIPAL|
|          BARBE|     08659|  2014|             1 000|        ATTACHE PRINCIPAL|
|          BENTZ|      0369|  2012|            34 270|  ADMINISTRATEUR TER.HCL.|
|          BENTZ|      0369|  2013|            12 573|  Administrateur Ter.HCl.|
|         CARALP|      0039|  2012|            11 769|    DIRECTEUR TERRITORIAL|
|         CARALP|      0039|  2013|            13 585|    Directeur territorial|
|         CARALP|      0039|  2014|            24 000|    DIRECTEUR TERRITORIAL|
|       CASTILLO|      0051|  2013|             2 869|      Ing�nieur Principal|
|       CASTILLO|      0051|  2014|             1 434|  INGENIEUR EN CHEF DE CN|
|         CONDOU|      3002|  2012|             966,8| Technicien ppal 2�me cla|
|         CONDOU|      3002|  2013|             1 289| Technicien ppal 2�me cla|
|         CONDOU|      3002|  2014|             644,5|   TECHNICIEN PRINCIP 2CL|
|         D IVRY|      0514|  2012|             1 061|      ATTACHE TERRITORIAL|
|         D IVRY|      0514|  2013|             2 417|      ATTACHE TERRITORIAL|
|         D IVRY|      0514|  2014|            14 500|      ATTACHE TERRITORIAL|
|         D'IVRY|      0514|  2012|             5 304|      Attach� Territorial|
|         D'IVRY|      0514|  2013|             5 307|      Attach� Territorial|
|       ERNANDEZ|      0095|  2012|            45 650|  DGA. 40-150 MILLES HAB.|
|       ERNANDEZ|      0095|  2013|            46 342|  DGA. 40-150 milles Hab.|
|        FOURNIL|      4153|  2012|             4 908|      ATTACHE TERRITORIAL|
|        FOURNIL|      4153|  2013|             5 776|      Attach� Territorial|
|        FOURNIL|      4153|  2014|             9 965|      ATTACHE TERRITORIAL|
|         GARDEY|     05319|  2014|             7 500|        ATTACHE PRINCIPAL|
|       LABOURIE|      0579|  2013|             583,3|      Attach� Territorial|
|       LABOURIE|      0579|  2014|             1 750|      ATTACHE TERRITORIAL|
|       LACRAMPE|      0332|  2012|             6 863|   EDUC.TER.PRINC 1CL APS|
|       LACRAMPE|      0332|  2013|             6 863|   Educ.Ter.Princ 1CL APS|
|       LACRAMPE|      0332|  2014|             6 875|   EDUC.TER.PRINC 1CL APS|
|         MARTIN|      0155|  2012|             8 630|      ATTACHE TERRITORIAL|
|         MARTIN|      0155|  2013|             9 735|      Attach� Territorial|
|         MARTIN|      0155|  2014|            18 000|      ATTACHE TERRITORIAL|
|      MAYNADIER|      0201|  2013|             1 422|                Ing�nieur|
|      MAYNADIER|      0201|  2014|               711|                INGENIEUR|
|       MENGELLE|      0569|  2012|             6 865|     ASS.CONS.DE 1eCLASSE|
|       MENGELLE|      0569|  2013|             7 369|      Attach� Territorial|
|       MENGELLE|      0569|  2014|            11 800|      ATTACHE TERRITORIAL|
|       PEYREGNE|      0183|  2013|             985,9|   Technicien Territorial|
|       PEYREGNE|      0183|  2014|               493|   TECHNICIEN TERRITORIAL|
|            REY|     03148|  2014|            13 250|  CONTRACTUEL CDD (CAT.A)|
| ROUSSEL-DEVAUX|      0582|  2012|            24 900|           Administrateur|
| ROUSSEL-DEVAUX|      0582|  2013|            41 500|  D.G.S. 40 a 80.000 hab.|
| ROUSSEL-DEVAUX|      0582|  2014|            49 800|  D.G.S. 40 A 80.000 HAB.|
|       SOMMACAL|      0313|  2012|            11 769|    DIRECTEUR TERRITORIAL|
|       SOMMACAL|      0313|  2013|            13 585|    Directeur territorial|
|       SOMMACAL|      0313|  2014|            23 100|    DIRECTEUR TERRITORIAL|
|          VABRE|      0330|  2012|             7 119|   ATTACHE TER. PRINCIPAL|
|          VABRE|      0330|  2013|             7 040|   Attach� Ter. Principal|
|          VABRE|      0330|  2014|            10 440|   ATTACHE TER. PRINCIPAL|

 
&nbsp;*Tableau 64* : Variations de l'agr�gat (PFR ou IFTS) pour les b�n�ficiaires de la PFR
         


|            Nom| Matricule|           Ann�es| Variation (%)| Moyenne g�om�trique annuelle(%)|
|--------------:|---------:|----------------:|-------------:|-------------------------------:|
|          ALARY|      3184| 2012, 2013, 2014|         -25,0|                           -13,4|
|       ARTIGUES|      0404|       2013, 2014|         -50,0|                           -50,0|
|          BENTZ|      0369|       2012, 2013|         -63,3|                           -63,3|
|         CARALP|      0039| 2012, 2013, 2014|         103,9|                            42,8|
|       CASTILLO|      0051|       2013, 2014|         -50,0|                           -50,0|
|         CONDOU|      3002| 2012, 2013, 2014|         -33,3|                           -18,4|
|         D IVRY|      0514| 2012, 2013, 2014|        1267,0|                           269,7|
|         D'IVRY|      0514|       2012, 2013|           0,1|                             0,1|
|       ERNANDEZ|      0095|       2012, 2013|           1,5|                             1,5|
|        FOURNIL|      4153| 2012, 2013, 2014|         103,0|                            42,5|
|       LABOURIE|      0579|       2013, 2014|         200,0|                           200,0|
|       LACRAMPE|      0332| 2012, 2013, 2014|           0,2|                             0,1|
|         MARTIN|      0155| 2012, 2013, 2014|         108,6|                            44,4|
|      MAYNADIER|      0201|       2013, 2014|         -50,0|                           -50,0|
|       MENGELLE|      0569| 2012, 2013, 2014|          71,9|                            31,1|
|       PEYREGNE|      0183|       2013, 2014|         -50,0|                           -50,0|
| ROUSSEL-DEVAUX|      0582| 2012, 2013, 2014|         100,0|                            41,4|
|       SOMMACAL|      0313| 2012, 2013, 2014|          96,3|                            40,1|
|          VABRE|      0330| 2012, 2013, 2014|          46,6|                            21,1|

        
  
[Lien vers la base de donn�es agr�gat PFR-IFTS](Bases/R�mun�rations/b�n�ficiaires.PFR.csv)    

  
[Lien vers la base de donn�es variations agr�gat PFR-IFTS](Bases/R�mun�rations/b�n�ficiaires.PFR.Variation.csv)    





## 5.6 Contr�le sur les heures suppl�mentaires

Le seuil de 180 heures suppl�mentaires maximum est d�pass� par  6  agents.
 Le seuil de 220 heures suppl�mentaires maximum est d�pass� par  6  agents.


 
&nbsp;*Tableau 65*   
   


| Nombre de lignes HS en exc�s | Nombre de lignes IHTS anormales |
|:----------------------------:|:-------------------------------:|
|             305              |               NA                |


[Lien vers la base de donn�es Heures suppl�mentaires en exc�s du seuil de 25h/mois: matricules](Bases/R�glementation/HS.sup.25.csv)     
[Lien vers la base de donn�es cumuls en exc�s des seuils annuels](Bases/R�glementation/D�passement.seuil.180h.csv)    
[Lien vers la base de donn�es IHTS anormales](Bases/R�glementation/ihts.anormales.csv)      

**Nota :**
HS en exc�s : au-del� de 25 heures par mois
IHTS anormales : non attribu�es � des fonctionnaires de cat�gorie B ou C.




## 5.7 Contr�le sur les indemnit�s des �lus
  



  

Tableau des indemnit�s d'�lu : pas de donn�es.

[Lien vers la base de donn�es R�mun�rations des �lus](Bases/R�glementation/r�mun�rations.�lu.csv)




## 5.8 Lien avec le compte de gestion



 
Cumul des lignes de paie par exercice et cat�gorie de ligne de paie   
 


Tableau  66  Ann�e  2012

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s|   787 204,93|
|             Rappels|    31 174,49|
| Suppl�ment familial|    51 893,21|
|         Traitements| 6 099 715,82|

Tableau  67  Ann�e  2013

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s|   916 358,60|
|             Rappels|    25 864,32|
|       R�m. diverses|     5 388,68|
| Suppl�ment familial|    61 098,55|
|         Traitements| 6 483 796,03|

Tableau  68  Ann�e  2014

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 128 582,07|
|             Rappels|    44 237,73|
|       R�m. diverses|   121 494,44|
| Suppl�ment familial|    69 757,72|
|         Traitements| 6 342 804,12|

 
[Lien vers la base d�taill�e des cumuls des lignes de paie](Bases/R�glementation/cumul.lignes.paie.csv)
 
[Lien vers la base agr�g�e des cumuls des lignes de paie](Bases/R�glementation/cumul.total.lignes.paie.csv)
 
 
*Avertissement : les rappels comprennent �galement les rappels de cotisations et d�ductions diverses.*    
  




## 5.9 Contr�le du suppl�ment familial de traitement   
 


La dur�e du travail prise en compte dans la base de donn�es est de  303  h par mois.

Attention !
Le temps de travail hebdomadaire s'�carte significativement de la dur�e l�gale :  69,9  h par semaine.

Pour les agents n'ayant pas d'enfant signal� en base, il a �t� d�tect� 3 bulletins de paie pr�sentant un paiement du SFT apparemment anormal.

 
[Lien vers la base des paiements de SFT � agents sans enfant signal�](Bases/R�glementation/Paie.sans.enfant.r�duit.csv)
 


Pour les agents ayant au moins un enfant, il n'a �t� d�tect� aucun �cart de paiement sur SFT sup�rieur �  1  euro.

 
[Lien vers la base des �carts de paiement sur SFT](Bases/R�glementation/controle.sft.csv)
 



# Annexe
## Liens compl�mentaires

[Lien vers le fichier des personnels](Bases/Effectifs/Cat�gories des personnels.csv)
 
## Fiabilit� du traitement statistique  
### Eliminations des doublons  
 

Attention : Alta�r a d�tect� des lignes dupliqu�es alors qu'aucun retraitement des lignes dupliqu�es n'est pr�vu par d�faut.

 



 
*Tests de fiabilit� sur le renseignement des heures et des quotit�s*    
  



 


Pourcentage d'heures renseign�es : 95 %

  


Pourcentage de quotit�s renseign�es : 100 %

  


Nombre de bulletins � heures et quotit�s :  10815 [ 95 %]

  


Nombre de bulletins � heures sans quotit�s :  2 [ 0 %]

  


Nombre de bulletins � quotit�s sans heures :  573 [ 5 %]

  


Nombre de bulletins apparemment inactifs :  0 [ 0 %]

  


Nombre de bulletins non renseign�s :  0 [ 0 %]

  



 

Nombre de bulletins de paie de salaires (net ou brut) vers�s pour un champ Heures = 0 :  573

  



  
[Lien vers la base de donn�es des salaires vers�s pour Heures=0](Bases/Fiabilit�/base.heures.nulles.salaire.nonnull.csv)   
[Lien vers la base de donn�es des salaires vers�s � quotit� ind�finie](Bases/Fiabilit�/base.quotit�.ind�finie.salaire.nonnull.csv)   

# Tableau des personnels : renseigner la cat�gorie

Utiliser les codes : A, B, C, ELU, AUTRES

En cas de changement de cat�gorie en cours de p�riode, utiliser la cat�gorie AUTRES
Cela peut conduire � modifier manuellement le fichier Cat�gories des personnels.csv



Non g�n�r�  [anonymisation]


