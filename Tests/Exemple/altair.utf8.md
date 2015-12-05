---
title: false
author: false
date: false
output:
html_document:
css: style.css
---
  
![Image_Altair](Altair.png)
  
  
# Démonstrateur Altaïr version 15.10   




<p class = "centered"><b>Exercices 2009 à 2013 </b></p>
<p class = "author">Fabrice Nicol</h1>




dim. 06 déc. 2015
   


La durée du travail prise en compte dans la base de données est de  151,67  h par mois.

# 1. Statistiques de population

### 1.1 Effectifs



 



&nbsp;*Tableau 1*   


|                                                             | 2009  | 2010  | 2011  | 2012  | 2013  |
|:------------------------------------------------------------|:-----:|:-----:|:-----:|:-----:|:-----:|
|Total effectifs (a)                                          | 709,0 | 769,0 | 698,0 | 714,0 | 703,0 |
|&nbsp;&nbsp;&nbsp;dont présents 12 mois                      | 465,0 | 462,0 | 444,0 | 428,0 | 433,0 |
|&nbsp;&nbsp;&nbsp;dont fonctionnaires (b)                    | 438,0 | 431,0 | 423,0 | 427,0 | 417,0 |
|&nbsp;&nbsp;&nbsp;dont fonct. présents 12 mois               | 400,0 | 403,0 | 402,0 | 384,0 | 391,0 |
|&nbsp;&nbsp;&nbsp;dont non titulaires                        | 60,0  | 54,0  | 56,0  | 57,0  | 73,0  |
|&nbsp;&nbsp;&nbsp;dont élus                                  | 14,0  | 14,0  | 14,0  | 25,0  | 14,0  |
|&nbsp;&nbsp;&nbsp;dont élus présents 12 mois                 | 13,0  | 14,0  | 13,0  | 14,0  | 14,0  |
|&nbsp;&nbsp;&nbsp;dont vacataires détectés (c)               | 39,0  | 48,0  | 48,0  | 45,0  | 46,0  |
|&nbsp;&nbsp;&nbsp;dont assistantes maternelles détectées (c) | 16,0  | 15,0  |  4,0  |  0,0  |  0,0  |
|Postes non actifs (g)                                        | 89,0  | 131,0 | 64,0  | 78,0  | 65,0  |
|Postes annexes (g)                                           | 218,0 | 276,0 | 208,0 | 215,0 | 209,0 |
|Postes actifs non annexes (g)                                | 477,0 | 479,0 | 476,0 | 474,0 | 480,0 |
|Total ETP/année (d)                                          | 511,7 | 537,3 | 535,0 | 518,0 | 523,1 |
|Total ETPT/année (e)                                         | 443,8 | 457,1 | 448,1 | 437,3 | 440,8 |
|Total ETPT/année personnes en place (f)(g)                   |  0,0  | 354,0 | 357,4 | 327,6 | 331,9 |
|Total ETPT/année fonctionnaires (g)                          | 384,9 | 387,5 | 381,3 | 372,4 | 368,1 |
|Total ETPT/année titulaires à temps complet (g)              | 288,5 | 301,2 | 294,0 | 262,6 | 277,1 |
|Total ETPT non titulaires (g)                                | 36,7  | 32,3  | 32,5  | 34,6  | 40,5  |
|Total ETPT autre statut                                      |  2,8  |  4,2  |  4,8  |  6,4  |  5,6  |
|Total ETPT postes non actifs (g)                             |  0,0  |  0,0  |  0,0  |  0,0  |  0,0  |
|Total ETPT postes annexes (g)                                | 18,9  | 25,7  | 24,3  | 23,5  | 22,3  |
|Total ETPT postes actifs non annexes (g)                     | 424,9 | 431,4 | 423,8 | 413,8 | 418,4 |


**Nota:**   
*(a) Nombre de matricules distincts ayant eu au moins un bulletin de paie dans l'année, en fonction ou non.Peut correspondre à des régularisations, des personnels hors position d'activité ou des ayants droit (reversion, etc.)*   
*(b) Titulaires ou stagiaires*   
*(c) Sur la base des libellés d'emploi et des libellés de lignes de paie. La détection peut être lacunaire*   
*(d) ETP  : Equivalent temps plein = rémunération . quotité*  
*(e) ETPT : Equivalent temps plein travaillé = ETP . 12/nombre de mois travaillés dans l'année*  
*(f) Personnes en place : présentes en N et N-1 avec la même quotité, postes actifs et non annexes uniquement.*     
*(g) Postes actifs et non annexes :* voir [Compléments méthodologiques](Docs/méthodologie.pdf)    
*&nbsp;&nbsp;&nbsp;Un poste actif est défini par au moins un bulletin de paie comportant un traitement positif pour un volume d'heures de travail mensuel non nul.*             
*&nbsp;&nbsp;&nbsp;Un poste non annexe est défini comme la conjonction de critères horaires et de revenu sur une année. La période minimale de référence est le mois.*   
*Les dix dernières lignes du tableau sont calculées en ne tenant pas compte des élus.*      
  
[Lien vers la base des effectifs](Bases/Effectifs/tableau.effectifs.csv)





### 1.2 Pyramide des âges, personnels non élus
 
&nbsp;*Tableau 2*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2009 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  16                  |          |
| 1er quartile  |                  35                  |          |
|    Médiane    |                  43                  |          |
|    Moyenne    |                42,45                 |   542    |
| 3ème quartile |                  51                  |          |
|    Maximum    |                  66                  |          |

 

![plot of chunk unnamed-chunk-9](figure/unnamed-chunk-9-1.png) 

 
&nbsp;*Tableau 3*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  17                  |          |
| 1er quartile  |                  36                  |          |
|    Médiane    |                  45                  |          |
|    Moyenne    |                43,35                 |   528    |
| 3ème quartile |                  52                  |          |
|    Maximum    |                  70                  |          |

 
 

![plot of chunk unnamed-chunk-11](figure/unnamed-chunk-11-1.png) 


### 1.3 Pyramide des âges, personnels non titulaires
 
&nbsp;*Tableau 4*   
   


|  Statistique  | Âge des personnels non titulaires<br>au 31/12/2009 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         17                         |          |
| 1er quartile  |                         24                         |          |
|    Médiane    |                        37,5                        |          |
|    Moyenne    |                       36,97                        |   100    |
| 3ème quartile |                       46,25                        |          |
|    Maximum    |                         66                         |          |

 

![plot of chunk unnamed-chunk-13](figure/unnamed-chunk-13-1.png) 

 
&nbsp;*Tableau 5*   
   


|  Statistique  | Âge des personnels non titulaires<br>au 31/12/2013 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         17                         |          |
| 1er quartile  |                         23                         |          |
|    Médiane    |                         35                         |          |
|    Moyenne    |                       36,09                        |   108    |
| 3ème quartile |                         46                         |          |
|    Maximum    |                         70                         |          |

![plot of chunk unnamed-chunk-14](figure/unnamed-chunk-14-1.png) 

 
[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.nontit.csv)  
 


### 1.4 Pyramide des âges, personnels fonctionnaires stagiaires et titulaires
 
&nbsp;*Tableau 6*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2009 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  24                  |          |
| 1er quartile  |                  38                  |          |
|    Médiane    |                  45                  |          |
|    Moyenne    |                44,18                 |   410    |
| 3ème quartile |                  51                  |          |
|    Maximum    |                  62                  |          |

 


![plot of chunk unnamed-chunk-16](figure/unnamed-chunk-16-1.png) 

 
&nbsp;*Tableau 7*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  22                  |          |
| 1er quartile  |                  40                  |          |
|    Médiane    |                  47                  |          |
|    Moyenne    |                  46                  |   400    |
| 3ème quartile |                  53                  |          |
|    Maximum    |                  63                  |          |



![plot of chunk unnamed-chunk-18](figure/unnamed-chunk-18-1.png) 

 
[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.fonctionnaires.csv)  
 
### 1.5 Pyramide des âges, personnels permanents (titulaires, stagiaires et non titulaires)
 
&nbsp;*Tableau 8*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2009 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  17                  |          |
| 1er quartile  |                  36                  |          |
|    Médiane    |                 43,5                 |          |
|    Moyenne    |                42,76                 |   510    |
| 3ème quartile |                  51                  |          |
|    Maximum    |                  66                  |          |

 

![plot of chunk unnamed-chunk-20](figure/unnamed-chunk-20-1.png) 

&nbsp;*Tableau 9*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  17                  |          |
| 1er quartile  |                  37                  |          |
|    Médiane    |                  45                  |          |
|    Moyenne    |                43,89                 |   508    |
| 3ème quartile |                  52                  |          |
|    Maximum    |                  70                  |          |



![plot of chunk unnamed-chunk-22](figure/unnamed-chunk-22-1.png) 

 
[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.permanents.csv)  
 
### 1.6 Effectifs des personnels par durée de service

**Personnels en fonction (hors élus) des exercices 2009 à 2013 inclus :**

 
&nbsp;*Tableau 10*   
   


| Plus de 2 ans | Moins de 2 ans | Moins d'un an | Moins de six mois |
|:-------------:|:--------------:|:-------------:|:-----------------:|
|      472      |      146       |      86       |        24         |



![plot of chunk unnamed-chunk-24](figure/unnamed-chunk-24-1.png) 


**Effectifs (hors élus)**   




 
&nbsp;*Tableau 11*   
   


|                  | 2009 | 2010 | 2011 | 2012 | 2013 |
|:-----------------|:----:|:----:|:----:|:----:|:----:|
|Plus de deux ans  | 439  | 451  | 451  | 431  | 411  |
|Moins de deux ans |  38  |  27  |  24  |  42  |  68  |
|Total             | 477  | 478  | 475  | 473  | 479  |



**Nota :**
*Personnels en place : ayant servi au moins deux années consécutives pendant la période.*     
*Plus/moins de deux ans : plus/mois de 730 jours sur la période sous revue.*   



  
  



   
# 2. Rémunérations brutes : analyse pour l'exercice 2009    
   
## 2.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des rémunérations brutes pour l'exercice 2009
 
*Personnels (hors élus)*     
 
&nbsp;*Tableau 12*   
   


|           Agrégats           |   k&euro;    |
|------------------------------|--------------|
|   Brut annuel (bulletins)    | 10 647 917,6 |
| Brut annuel (lignes), dont : | 10 433 576,9 |
|         \ \ Primes :         | 1 508 455,9  |
|   \ \ Autres rémunérations   |   28 394,4   |
|     Part de primes en %      |     14,2     |

 
**Définitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
 *Indemnités d'élu*          : toutes rémunérations indemnitaires des élus    
 *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
 
**Tests de cohérence**

Somme des rémunérations brutes versées aux personnels (non élus) :  
 
 
&nbsp;*Tableau 13*   
   


|     Agrégats      |   k&euro;    |
|-------------------|--------------|
| Bulletins de paie | 10 647 917,6 |
|  Lignes de paie   | 10 433 576,9 |
|    Différence     |  214 340,8   |


à comparer aux soldes des comptes 641 et 648 du compte de gestion.

  
## 2.2 Masse salariale brute des fonctionnaires

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*  


![plot of chunk unnamed-chunk-131](figure/unnamed-chunk-131-1.png) ![plot of chunk unnamed-chunk-131](figure/unnamed-chunk-131-2.png) ![plot of chunk unnamed-chunk-131](figure/unnamed-chunk-131-3.png) ![plot of chunk unnamed-chunk-131](figure/unnamed-chunk-131-4.png) ![plot of chunk unnamed-chunk-131](figure/unnamed-chunk-131-5.png) ![plot of chunk unnamed-chunk-131](figure/unnamed-chunk-131-6.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio rémunération / quotité*  
   
**Effectif : 433**

**Tests de cohérence**

       
       
       
Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :

 
&nbsp;*Tableau 14*   
   


|           Agrégats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 9 264 307,7 |
| Brut annuel (lignes), dont : | 9 173 037,4 |
|         \ \ Primes :         | 1 419 599,3 |
|   \ \ Autres rémunérations   |  24 890,5   |
|     Part de primes en %      |    15,3     |


**Définitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
 *Indemnités d'élus*         : toutes rémunérations indemnitaires des élus    
 *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   

**Tests de cohérence**

Somme des rémunérations brutes versées aux personnels (non élus) :

 
&nbsp;*Tableau 15*   
   


|     Agrégats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 9 264 307,7 |
|  Lignes de paie   | 9 173 037,4 |
|    Différence     |  91 270,3   |


A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.

**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2009**     
 
&nbsp;*Tableau 16*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres rémunérations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 2 855|    778,3|                 0,00|         |
|  1er quartile|                16 203|  2 018,8|                 0,00|         |
|       Médiane|                17 446|  2 821,5|                 0,00|         |
|       Moyenne|                18 795|  3 441,3|                60,41|      412|
| 3ème quartile|                20 745|  4 179,9|                15,25|         |
|       Maximum|                52 990| 25 332,3|             2 876,80|         |

 
&nbsp;*Tableau 17*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la rém. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     3 640|      3 820|                 0|                        4,794|         |
|  1er quartile|                    18 432|     18 631|            19 551|                       10,600|         |
|       Médiane|                    20 398|     20 634|            21 722|                       13,639|         |
|       Moyenne|                    22 237|     22 445|            23 674|                       14,644|      412|
| 3ème quartile|                    24 278|     24 571|            25 821|                       17,557|         |
|       Maximum|                    70 613|     70 580|            70 580|                       36,630|         |

  
*Hors vacataires identifiés, assistantes maternelles, élus locaux et pour les postes actifs non annexes*  

**Catégorie A**




 
&nbsp;*Tableau 18*   
   


|   Statistique| Traitement indiciaire| Primes| Autres rémunérations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                10 229|  3 254|                0,000|
|  1er quartile|                28 835|  5 805|                0,000|
|       Médiane|                33 703|  7 884|                7,625|
|       Moyenne|                34 174|  9 492|              176,649|
| 3ème quartile|                43 191| 10 020|              202,838|
|       Maximum|                52 990| 25 332|            1 278,990|


 
&nbsp;*Tableau 19*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              14 052|                        0|                                13,66|
|  1er quartile|              36 996|                   38 281|                                17,02|
|       Médiane|              41 300|                   42 679|                                18,63|
|       Moyenne|              43 687|                   43 267|                                21,07|
| 3ème quartile|              52 350|                   52 350|                                25,04|
|       Maximum|              70 580|                   70 580|                                35,89|


**Effectif : 24**  

**Catégorie B**

 
&nbsp;*Tableau 20*   
   


|   Statistique| Traitement indiciaire| Primes| Autres rémunérations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                 9 150|  2 825|                 0,00|
|  1er quartile|                19 793|  4 256|                 0,00|
|       Médiane|                22 136|  4 878|                 0,00|
|       Moyenne|                22 248|  5 428|                68,19|
| 3ème quartile|                24 834|  6 496|                38,40|
|       Maximum|                29 384| 10 775|               633,74|


 
&nbsp;*Tableau 21*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              12 838|                        0|                                12,76|
|  1er quartile|              24 365|                   26 071|                                15,97|
|       Médiane|              27 429|                   28 321|                                18,93|
|       Moyenne|              27 641|                   28 393|                                19,63|
| 3ème quartile|              30 558|                   31 031|                                22,92|
|       Maximum|              36 959|                   36 959|                                31,03|


**Effectif : 53**

**Catégorie C**

 
&nbsp;*Tableau 22*   
   


|   Statistique| Traitement indiciaire|  Primes| Autres rémunérations|
|-------------:|---------------------:|-------:|--------------------:|
|       Minimum|                 2 855|   778,3|                 0,00|
|  1er quartile|                16 132| 2 018,1|                 0,00|
|       Médiane|                16 976| 2 208,3|                 0,00|
|       Moyenne|                17 147| 2 693,5|                50,86|
| 3ème quartile|                18 984| 3 094,6|                 0,00|
|       Maximum|                25 843| 7 199,8|             2 876,80|


 
&nbsp;*Tableau 23*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               3 820|                        0|                                4,794|
|  1er quartile|              18 312|                   19 245|                               10,334|
|       Médiane|              19 860|                   20 645|                               11,661|
|       Moyenne|              20 101|                   21 524|                               13,396|
| 3ème quartile|              22 343|                   23 439|                               15,288|
|       Maximum|              33 991|                   40 925|                               36,630|

**Effectif : 335**


######      <br>

## 2.3 Contractuels, vacataires et stagiaires inclus   
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   


![plot of chunk unnamed-chunk-144](figure/unnamed-chunk-144-1.png) 

  
**Nota :**
Ne sont retenues que les rémunérations supérieures à 1 000 k&euro;.
Les élus ne sont pas pris en compte.
  

![plot of chunk unnamed-chunk-145](figure/unnamed-chunk-145-1.png) 





  
**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2009**     
  
 
&nbsp;*Tableau 24*   
   


|   Statistique|  Primes| Autres rémunérations| Effectif|
|-------------:|-------:|--------------------:|--------:|
|       Minimum|   -79,6|                 0,00|         |
|  1er quartile|     0,0|                 0,00|         |
|       Médiane|   497,7|                 0,00|         |
|       Moyenne| 1 108,2|                43,65|       65|
| 3ème quartile| 1 485,5|                 0,00|         |
|       Maximum| 6 358,0|             1 054,40|         |

 
&nbsp;*Tableau 25*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               3 443|                        0|         |
|  1er quartile|               7 803|                   16 655|         |
|       Médiane|              12 239|                   18 115|         |
|       Moyenne|              14 323|                   19 330|       65|
| 3ème quartile|              17 372|                   20 234|         |
|       Maximum|              52 654|                   52 654|         |






  
  



   
# 3. Rémunérations brutes : analyse pour l'exercice 2013    
   
## 3.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des rémunérations brutes pour l'exercice 2013
 
*Personnels (hors élus)*     
 
&nbsp;*Tableau 26*   
   


|           Agrégats           |   k&euro;    |
|------------------------------|--------------|
|   Brut annuel (bulletins)    | 10 785 759,3 |
| Brut annuel (lignes), dont : | 10 657 373,2 |
|         \ \ Primes :         | 1 780 299,9  |
|   \ \ Autres rémunérations   |   51 331,0   |
|     Part de primes en %      |     16,5     |

 
**Définitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
 *Indemnités d'élu*          : toutes rémunérations indemnitaires des élus    
 *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
 
**Tests de cohérence**

Somme des rémunérations brutes versées aux personnels (non élus) :  
 
 
&nbsp;*Tableau 27*   
   


|     Agrégats      |   k&euro;    |
|-------------------|--------------|
| Bulletins de paie | 10 785 759,3 |
|  Lignes de paie   | 10 657 373,2 |
|    Différence     |  128 386,1   |


à comparer aux soldes des comptes 641 et 648 du compte de gestion.

  
## 3.2 Masse salariale brute des fonctionnaires

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*  


![plot of chunk unnamed-chunk-154](figure/unnamed-chunk-154-1.png) ![plot of chunk unnamed-chunk-154](figure/unnamed-chunk-154-2.png) ![plot of chunk unnamed-chunk-154](figure/unnamed-chunk-154-3.png) ![plot of chunk unnamed-chunk-154](figure/unnamed-chunk-154-4.png) ![plot of chunk unnamed-chunk-154](figure/unnamed-chunk-154-5.png) ![plot of chunk unnamed-chunk-154](figure/unnamed-chunk-154-6.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio rémunération / quotité*  
   
**Effectif : 411**

**Tests de cohérence**

       
       
       
Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :

 
&nbsp;*Tableau 28*   
   


|           Agrégats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 9 511 343,6 |
| Brut annuel (lignes), dont : | 9 438 778,3 |
|         \ \ Primes :         | 1 662 312,8 |
|   \ \ Autres rémunérations   |  48 383,4   |
|     Part de primes en %      |    17,5     |


**Définitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
 *Indemnités d'élus*         : toutes rémunérations indemnitaires des élus    
 *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   

**Tests de cohérence**

Somme des rémunérations brutes versées aux personnels (non élus) :

 
&nbsp;*Tableau 29*   
   


|     Agrégats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 9 511 343,6 |
|  Lignes de paie   | 9 438 778,3 |
|    Différence     |  72 565,4   |


A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.

**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2013**     
 
&nbsp;*Tableau 30*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres rémunérations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 2 880|    539,3|                  0,0|         |
|  1er quartile|                17 396|  3 022,3|                  0,0|         |
|       Médiane|                18 415|  3 384,5|                  0,0|         |
|       Moyenne|                19 619|  4 196,6|                122,2|      396|
| 3ème quartile|                21 846|  4 913,2|                170,2|         |
|       Maximum|                47 256| 25 307,4|              3 805,3|         |

 
&nbsp;*Tableau 31*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la rém. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     3 419|      3 774|                 0|                        9,463|         |
|  1er quartile|                    20 612|     20 686|            21 363|                       14,172|         |
|       Médiane|                    22 340|     22 607|            23 878|                       15,675|         |
|       Moyenne|                    23 816|     24 008|            25 450|                       17,296|      396|
| 3ème quartile|                    26 223|     26 791|            27 953|                       19,717|         |
|       Maximum|                    70 925|     70 925|            70 925|                       51,003|         |

  
*Hors vacataires identifiés, assistantes maternelles, élus locaux et pour les postes actifs non annexes*  

**Catégorie A**




 
&nbsp;*Tableau 32*   
   


|   Statistique| Traitement indiciaire| Primes| Autres rémunérations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                18 870|  4 945|                  0,0|
|  1er quartile|                26 300|  7 123|                  0,0|
|       Médiane|                29 699|  8 449|                  0,0|
|       Moyenne|                31 911|  9 939|                 58,5|
| 3ème quartile|                38 659| 10 346|                  0,0|
|       Maximum|                47 256| 25 307|                297,0|


 
&nbsp;*Tableau 33*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              24 301|                   27 989|                                17,24|
|  1er quartile|              35 104|                   35 578|                                19,67|
|       Médiane|              37 354|                   39 231|                                22,01|
|       Moyenne|              41 395|                   43 144|                                23,40|
| 3ème quartile|              48 895|                   48 895|                                25,84|
|       Maximum|              70 925|                   70 925|                                35,68|


**Effectif : 18**  

**Catégorie B**

 
&nbsp;*Tableau 34*   
   


|   Statistique| Traitement indiciaire| Primes| Autres rémunérations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                10 705|  3 075|                  0,0|
|  1er quartile|                19 942|  5 033|                  0,0|
|       Médiane|                23 190|  5 409|                  0,0|
|       Moyenne|                22 860|  5 826|                155,8|
| 3ème quartile|                25 497|  6 319|                210,0|
|       Maximum|                33 338| 12 899|              3 805,3|


 
&nbsp;*Tableau 35*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              13 951|                        0|                                13,12|
|  1er quartile|              25 784|                   27 601|                                16,86|
|       Médiane|              29 013|                   30 755|                                19,53|
|       Moyenne|              29 029|                   30 609|                                20,36|
| 3ème quartile|              32 904|                   33 883|                                22,44|
|       Maximum|              42 627|                   42 627|                                51,00|


**Effectif : 62**

**Catégorie C**

 
&nbsp;*Tableau 36*   
   


|   Statistique| Traitement indiciaire|  Primes| Autres rémunérations|
|-------------:|---------------------:|-------:|--------------------:|
|       Minimum|                 2 880|   539,3|                  0,0|
|  1er quartile|                17 385| 3 006,8|                  0,0|
|       Médiane|                18 086| 3 200,1|                  0,0|
|       Moyenne|                18 281| 3 546,4|                118,9|
| 3ème quartile|                20 297| 3 950,9|                139,2|
|       Maximum|                26 147| 8 435,4|              1 828,4|


 
&nbsp;*Tableau 37*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               3 774|                        0|                                9,463|
|  1er quartile|              20 435|                   21 119|                               13,941|
|       Médiane|              21 615|                   22 572|                               14,775|
|       Moyenne|              22 026|                   23 411|                               16,337|
| 3ème quartile|              24 710|                   25 421|                               18,014|
|       Maximum|              37 515|                   48 958|                               43,177|

**Effectif : 315**


######      <br>

## 3.3 Contractuels, vacataires et stagiaires inclus   
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   


![plot of chunk unnamed-chunk-167](figure/unnamed-chunk-167-1.png) 

  
**Nota :**
Ne sont retenues que les rémunérations supérieures à 1 000 k&euro;.
Les élus ne sont pas pris en compte.
  

![plot of chunk unnamed-chunk-168](figure/unnamed-chunk-168-1.png) 





  
**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2013**     
  
 
&nbsp;*Tableau 38*   
   


|   Statistique|    Primes| Autres rémunérations| Effectif|
|-------------:|---------:|--------------------:|--------:|
|       Minimum|    -22,25|                 0,00|         |
|  1er quartile|      0,00|                 0,00|         |
|       Médiane|    264,70|                 0,00|         |
|       Moyenne|  1 340,40|                35,09|       84|
| 3ème quartile|  1 087,85|                 0,00|         |
|       Maximum| 17 494,04|             1 100,00|         |

 
&nbsp;*Tableau 39*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               3 830|                    4 806|         |
|  1er quartile|               6 877|                   17 484|         |
|       Médiane|               9 708|                   19 009|         |
|       Moyenne|              12 492|                   19 835|       84|
| 3ème quartile|              14 380|                   20 368|         |
|       Maximum|              58 944|                   58 944|         |

 
[Lien vers la base des rémunérations](Bases/Rémunérations/Analyse.rémunérations.csv)  
  



  
## 3.4 Comparaisons source INSEE/DGCL   
  
*Salaires annnuels moyens 2011 en EQTP (hors assistantes maternelles)*   
 
&nbsp;*Tableau 40*   
   


| Agrégat (&euro;) | Salaires bruts 2011 | Salaires bruts 2012 | Salaires bruts 2013 |
|------------------|---------------------|---------------------|---------------------|
|     Ensemble     |      25 908,0       |      26 340,0       |      26 616,0       |
|    Titulaires    |      26 676,0       |      27 108,0       |      27 444,0       |
| Autres salariés  |      22 836,0       |         NA          |      24 360,0       |

  
**Eléments de la rémunération brute pour les titulaires de la FPT entre 2010 et 2012**      
  
 
&nbsp;*Tableau 41*   
   


|         Rém. annuelles          | 2010  | Primes | 2011  | Primes | 2012  | Primes | 2013  | Primes |
|---------------------------------|-------|--------|-------|--------|-------|--------|-------|--------|
|          Salaire brut           | 26305 |        | 26660 |        | 27108 |        | 27444 |        |
|         Traitement brut         | 20350 | 22,6 % | 20562 | 22,9 % | 20724 | 23,6 % | 21060 | 23,6 % |
| Primes et rémunérations annexes |       |        |       |        |       |        |       |        |
|       y compris IR et SFT       | 5955  |        | 6098  |        | 6384  |        | 6384  |        |

  
*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*   												
*Les primes sont cumulées au supplément familial de traitement (SFT) et à l'indemnité de résidence (IR). Le cumul est rapporté à la rémunération brute totale.*    
[Source INSEE](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)    
[Source DGCL](http://www.fonction-publique.gouv.fr/files/files/statistiques/rapports_annuels/2012-2013/xls/Vue3_1_Remunerations.xls)    
[Source DGFIP PLF 2015](http://www.performance-publique.budget.gouv.fr/sites/performance_publique/files/farandole/ressources/2015/pap/pdf/jaunes/jaune2015_fonction_publique.pdf)   
  




# 4. Rémunérations nettes : évolutions sur la période 2009 - 2013    

Nombre d'exercices: 5   
 
**Les données présentées dans cette section sont toutes relatives à des rémunérations nettes en équivalent temps plein (EQTP)**   
Les élus, les vacataires et les assistantes maternelles ont été retirés de la population étudiée       
Seuls sont considérés les postes actifs et non annexes   
   
*Nota :*   
*EQTP = Equivalent temps plein  = 12 . moyenne du ratio rémunération / quotité*    
   
## 4.1 Distribution de la rémunération nette moyenne sur la période    

![plot of chunk unnamed-chunk-32](figure/unnamed-chunk-32-1.png) 



![plot of chunk unnamed-chunk-33](figure/unnamed-chunk-33-1.png) 


[Lien vers la base de données synthétique](Bases/Rémunérations/Analyse.variations.synthèse.csv)
[Lien vers la base de données détaillée par année](Bases/Rémunérations/Analyse.variations.par.exercice.csv)

## 4.2 Evolutions des rémunérations nettes sur la période 2009 - 2013   

### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors élus)





   
**Salaire net moyen par tête (SMPT net) en EQTP, hors élus**         
      
 
&nbsp;*Tableau 42*   
   


| Année | Rém. nette totale (k&euro;) | Variation (%) | SMPT net (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-----------------:|:-------------:|
| 2009  |            9 036            |               |      19 185       |               |
| 2010  |            8 992            |               |      19 051       |               |
| 2011  |           9 099,4           |     5,63      |      19 278       |     4,52      |
| 2012  |           9 256,7           |               |      19 695       |               |
| 2013  |           9 544,7           |               |      20 052       |               |

  
**Effet de noria sur salaires nets et taux de remplacements**       
  
**Effet de noria** : *différence entre la rémunération annuelle des entrants à l'année N et des sortants à l'année N-1*.  
*Usuellement calculée sur les rémunérations brutes, ici sur les rémunérations nettes EQTP*  
*afin d'apprécier l'impact de cet effet sur l'évolution des rémunérations nette moyennes calculée au tableau précédent.*               
 
&nbsp;*Tableau 43*   
   


| Année | Noria EQTP (&euro;) | En % de la MS N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-----------------:|:------------------:|:-------------------------:|
| 2010  |      -3 563,2       |      -0,593       |         15         |            3,3            |
| 2011  |       -304,68       |     -0,0 569      |         17         |            3,7            |
| 2012  |      -2 134,6       |       -0,54       |         23         |            5,3            |
| 2013  |      -3 399,9       |      -0,651       |         18         |             4             |


*MS N-1 : masse salariale nette de l'année n-1.*   
      
**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
      
 
&nbsp;*Tableau 44*   
   


|   Statistique| Première année|
|-------------:|--------------:|
|       Minimum|          7 081|
|  1er quartile|         15 750|
|       Médiane|         17 410|
|       Moyenne|         19 180|
| 3ème quartile|         20 790|
|       Maximum|         56 150|



|   Statistique| Dernière année|
|-------------:|--------------:|
|       Minimum|          4 794|
|  1er quartile|         16 990|
|       Médiane|         18 670|
|       Moyenne|         20 050|
| 3ème quartile|         22 030|
|       Maximum|         54 530|

 
*Nota :*  La population retenue est constituée des agents qui :   
&nbsp;&nbsp;- ne font pas partie des 2 centiles extrêmaux   
&nbsp;&nbsp;- sont au moins présents 1 jour(s) la première et la dernière année d'activité  
Les élus, vacataires et assistantes maternelles sont retirés du périmètre.   
Seuls sont pris en compte les agents ayant connu au moins un mois actif et ayant eu, sur l'année, des rémunérations non annexes.  
[Compléments méthodologiques](Docs/méthodologie.pdf)     
     
**Comparaisons source INSEE/DGCL**   

**Salaires annuels moyens 2011 et 2012 en EQTP (hors assistantes maternelles)**   
 
&nbsp;*Tableau 45*       


|     Agrégat     | Salaires nets 2011 (&euro;) | Salaires nets 2012 (&euro;) | Salaires nets 2013 (&euro;) |
|-----------------|-----------------------------|-----------------------------|-----------------------------|
|    Ensemble     |          21 876,0           |          22 176,0           |          22 224,0           |
|   Titulaires    |          22 632,0           |          22 920,0           |          22 920,0           |
| Autres salariés |          18 864,0           |             NA              |             NA              |

*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			



**Distribution des salaires nets annuels en EQTP dans la fonction publique par versant en 2011**   

 
&nbsp;*Tableau 46*   
   


| Décile (k&euro;) |   FPE    |   FPT    |   FPH    | Secteur privé |
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

**Distribution des salaires nets annuels en EQTP dans la fonction publique territoriale par catégorie en 2011**   
 
&nbsp;*Tableau 47*   
   


| Décile (k&euro;) | Catégorie A | Catégorie B | Catégorie C | Autres salariés |
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
  
[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)
  
### 4.2.2 Fonctionnaires

**Titulaires et stagiaires**      



**Salaire net moyen par tête (SMPT net) en EQTP**       
**Ensemble**  
   
&nbsp;*Tableau 48*   
   


| Année | Rém. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2009  |           7 985,2           |               |          19 571           |               |
| 2010  |           7 944,9           |               |          19 473           |               |
| 2011  |           8 001,2           |     2,48      |          19 854           |     6,39      |
| 2012  |           8 218,6           |               |          20 343           |               |
| 2013  |           8 183,2           |               |          20 822           |               |

  
**Catégorie A**  

&nbsp;*Tableau 49*   
   


| Année | Rém. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2009  |           854,31            |               |          38 832           |               |
| 2010  |           803,43            |               |          38 259           |               |
| 2011  |           880,87            |     -25,7     |          38 299           |     -9,18     |
| 2012  |           763,84            |               |          36 373           |               |
| 2013  |           634,81            |               |          35 267           |               |

  
**Catégorie B**    
    
&nbsp;*Tableau 50*   
   


| Année | Rém. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2009  |           1 239,8           |               |          23 843           |               |
| 2010  |           1 332,9           |               |          23 384           |               |
| 2011  |           1 390,6           |     24,3      |          23 975           |     5,99      |
| 2012  |           1 574,9           |               |          23 862           |               |
| 2013  |           1 541,6           |               |          25 272           |               |

  
**Catégorie C**     
   
&nbsp;*Tableau 51*   
   


| Année | Rém. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2009  |            5 891            |               |          17 638           |               |
| 2010  |           5 808,6           |               |          17 602           |               |
| 2011  |           5 729,8           |     1,57      |          17 794           |     8,38      |
| 2012  |           5 879,9           |               |          18 549           |               |
| 2013  |           5 983,2           |               |          19 116           |               |

   



  
**Effet de noria sur salaires nets et taux de remplacements**       
 
&nbsp;*Tableau 52*   
   


| Année | Noria EQTP (&euro;) | En % de la  MSN N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-------------------:|:------------------:|:-------------------------:|
| 2010  |      -6 634,7       |       -0,638        |         6          |            1,5            |
| 2011  |       13 280        |        1,15         |         5          |            1,3            |
| 2012  |        5 704        |        0,686        |         7          |            1,9            |
| 2013  |       -12 989       |       -0,495        |         2          |           0,61            |

    
*MS N-1 : masse salariale nette de l'année n-1.*   
      
**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
 
&nbsp;*Tableau 53*   
   


|   Statistique| Première année|
|-------------:|--------------:|
|       Minimum|          7 081|
|  1er quartile|         15 980|
|       Médiane|         17 790|
|       Moyenne|         19 570|
| 3ème quartile|         21 210|
|       Maximum|         56 150|



|   Statistique| Dernière année|
|-------------:|--------------:|
|       Minimum|          8 883|
|  1er quartile|         17 480|
|       Médiane|         19 420|
|       Moyenne|         20 820|
| 3ème quartile|         22 730|
|       Maximum|         54 530|

   



  
**Evolution du SMPT net des titulaires à temps complet**     
  
 
&nbsp;*Tableau 54*   
   


| Année | Rémunération nette totale (k&euro;) | Variation (%) | SMPT (&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:-------------:|:-------------:|
| 2009  |               5 703,4               |               |    19 667     |               |
| 2010  |                6 052                |               |    19 974     |               |
| 2011  |               5 934,6               |     1,35      |    20 049     |     5,34      |
| 2012  |               5 356,5               |               |    20 290     |               |
| 2013  |               5 780,4               |               |    20 718     |               |

   
**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) des titulaires à temps complet**         
      
 
&nbsp;*Tableau 55*   
   


|   Statistique| Première année|
|-------------:|--------------:|
|       Minimum|          7 081|
|  1er quartile|         16 050|
|       Médiane|         17 800|
|       Moyenne|         19 670|
| 3ème quartile|         20 800|
|       Maximum|         56 150|

  


|   Statistique| Dernière année|
|-------------:|--------------:|
|       Minimum|          8 883|
|  1er quartile|         17 400|
|       Médiane|         19 280|
|       Moyenne|         20 720|
| 3ème quartile|         22 210|
|       Maximum|         54 530|


[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)


## 4.3 Glissement vieillesse-technicité (GVT)   

### 4.3.1 Ensemble des personnels   
  
*Cette section est consacrée à la rémunération moyenne des personnes en place (RMPP), définies comme présentes deux années entières consécutives avec la même quotité*   
*L'évolution de la RMPP permet d'étudier le glissement vieillesse-technicité "positif", à effectifs constants sur deux années*      
*Le GVT positif est dû aux mesures statutaires et individuelles, à l'avancement et aux changements d'activité*  

![plot of chunk unnamed-chunk-54](figure/unnamed-chunk-54-1.png) 






  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 56*   
   


| Année | Rémunération nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2010  |               7 240,1               |               |        19 890        |               |
| 2011  |               7 350,9               |    -0,551     |        19 867        |     4,93      |
| 2012  |               6 955,7               |               |        20 398        |               |
| 2013  |               7 200,2               |               |        20 870        |               |

   
**Distribution et variation sur la période de la rémunération nette des personnes en place**                
 
&nbsp;*Tableau 57*   
   



 


|   Statistique| Première année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          7 081|         |
|  1er quartile|         15 820|         |
|       Médiane|         17 430|         |
|       Moyenne|         19 130|      428|
| 3ème quartile|         20 630|         |
|       Maximum|         56 150|         |

 
&nbsp;*Tableau 58*   
   


|   Statistique| Dernière année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          7 137|         |
|  1er quartile|         17 200|         |
|       Médiane|         19 000|         |
|       Moyenne|         20 350|      427|
| 3ème quartile|         22 140|         |
|       Maximum|         54 530|         |


*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
 
&nbsp;*Tableau 59*   
   


|   Statistique| Variation normalisée (%)| Variation annuelle moyenne normalisée (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -47,174|                                   -14,747|         |
|  1er quartile|                    5,612|                                     1,374|         |
|       Médiane|                   10,352|                                     2,493|         |
|       Moyenne|                   10,769|                                     2,462|      369|
| 3ème quartile|                   16,157|                                     3,815|         |
|       Maximum|                   45,049|                                     9,743|         |

   
### 4.3.2 Titulaires et stagiaires     
  



  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 60*   
   


| Année | Rémunération nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2010  |               6 896,8               |               |        19 818        |               |
| 2011  |               7 009,9               |     0,365     |        19 858        |     4,89      |
| 2012  |               6 728,9               |               |        20 329        |               |
| 2013  |                6 922                |               |        20 787        |               |

   
**Distribution et variation sur la période de la rémunération nette des fonctionnaires en place**                
 
&nbsp;*Tableau 61*   
   
 


|   Statistique| Première année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          7 081|         |
|  1er quartile|         15 940|         |
|       Médiane|         17 580|         |
|       Moyenne|         19 280|      395|
| 3ème quartile|         20 750|         |
|       Maximum|         56 150|         |

 
&nbsp;*Tableau 62*   
   


|   Statistique| Dernière année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          4 743|         |
|  1er quartile|         17 370|         |
|       Médiane|         19 370|         |
|       Moyenne|         21 000|      439|
| 3ème quartile|         22 980|         |
|       Maximum|         63 190|         |


*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
 
&nbsp;*Tableau 63*   
   


|   Statistique| Variation normalisée (%)| Variation annuelle moyenne normalisée (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -47,174|                                   -14,747|         |
|  1er quartile|                    5,778|                                     1,414|         |
|       Médiane|                   10,398|                                     2,504|         |
|       Moyenne|                   10,870|                                     2,486|      355|
| 3ème quartile|                   16,151|                                     3,814|         |
|       Maximum|                   45,049|                                     9,743|         |



[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)

**Nota**   
*Personnes en place :* en fonction au moins deux années consécutives sur la période 2009 à 2013    
*Variation sur la période d'activité :* entre l'arrivée et le départ de la personne      
*Variation normalisée :* conforme à la définition INSEE (présente en début et en fin de période avec la même quotité)  
 
**Commentaire**       
Les différences éventuelles constatées entre l'évolution de la RMPP au tableau 61 sont dues soit à l'effet de noria soit à l'effet périmètre.    
     
[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)


### 4.4 Comparaisons avec la situation nationale des rémunérations   
 
**Évolution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    
 
&nbsp;*Tableau 64*   
   


|   Année    | 2008-09 | 2009-10 | 2010-11 | 2011-12 | 2012-13 | 2008-12 | Moy. 2008-12 | Méd. 2007-11 |
|------------|---------|---------|---------|---------|---------|---------|--------------|--------------|
| SMPT brut  |   2,5   |   1,3   |   1,5   |   1,7   |   1,1   |   7,2   |     1,8      |              |
|  SMPT net  |   3,0   |   1,4   |   1,3   |   1,4   |   0,8   |   7,3   |     1,8      |     13,4     |
| RMPP brute |   3,3   |   2,5   |   2,5   |   2,7   |   1,9   |  11,5   |     2,8      |              |
| RMPP nette |   3,3   |   2,5   |   2,3   |   2,4   |   1,6   |  10,9   |     2,6      |              |

*Source : fichier général de l'État (FGE), DADS, SIASP, Insee, Drees. Traitement Insee, Drees, DGCL*    
Hors assistants maternels et familiaux, y compris bénéficiaires de contrats aidés.   
SMPT : Salaire moyen par tête en EQTP.   
RMPP : Agents présents 24 mois consécutifs chez le même employeur avec la même quotité de travail.  
Moyenne des variations géométriques annuelles pour les agents du champ.  
La dernière colonne présente la médiane des augmentations du SMPT net pour les agents présents en 2007 et 2011.   
 
**Salaires nets annuels et évolution moyenne type de collectivité en &euro; courants  EQTP**    
  
 
&nbsp;*Tableau 65*   
   


|           Collectivité            | SMPT net 2011 | SMPT net 2012 | SMPT net 2013 | Evol. Moy. 2007-2011 (%) |
|-----------------------------------|---------------|---------------|---------------|--------------------------|
|             Communes              |   20 784,0    |   21 120,0    |   21 096,0    |           2,5            |
|    CCAS et caisses des écoles     |   19 415,0    |   19 716,0    |   19 788,0    |           2,4            |
|      EPCI à fiscalité propre      |   22 882,0    |   23 088,0    |   23 184,0    |           3,1            |
| Autres structures intercommunales |   21 299,0    |   21 684,0    |   21 828,0    |           3,0            |
|           Départements            |   24 487,0    |   24 744,0    |   24 852,0    |           3,9            |
|               SDIS                |   29 811,0    |   29 940,0    |   30 180,0    |           3,4            |
|              Régions              |   22 432,0    |   22 836,0    |   23 004,0    |           3,8            |
|   Autres collectivités locales    |   24 680,0    |   24 696,0    |   24 828,0    |           3,2            |
|        Ensemble (moyenne)         |   21 873,0    |   22 176,0    |   22 212,0    |           2,9            |


*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			
Conversion en euros courants, calcul CRC.  
[Source INSEE données 2011 obsolètes](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)   
[Source DGAFP](http://infos.emploipublic.fr/dossiers/la-fonction-publique-en-chiffres/la-fonction-publique-en-chiffre-2013/non-defini-08/apm-71444/)  
[Source PLF 2014 données 2011 révisées p.151](http://www.fonction-publique.gouv.fr/files/files/statistiques/jaunes/jaune2014_FP.pdf)   
[Source PLF 2015 données 2012 p.130](http://www.performance-publique.budget.gouv.fr/sites/performance_publique/files/farandole/ressources/2015/pap/pdf/jaunes/jaune2015_fonction_publique.pdf)   
   











# 5. Tests réglementaires   
## 5.1 Contrôle des NBI et primes informatiques   
  
**Dans cette partie, l'ensemble de la base de paie est étudié.**  
Les agents non actifs ou dont le poste est annexe sont réintroduits dans le périmètre.   

Il existe  1 non titulaire   percevant une NBI.

Primes informatiques potentielles : PRIME FONCTION INFORMATIQUE
 
&nbsp;*Tableau 66*   
   


| Nombre de lignes NBI pour non titulaires | Nombre de bénéficiaires de PFI |
|:----------------------------------------:|:------------------------------:|
|                    24                    |              163               |

  
[Lien vers la base de données NBI aux non titulaires](Bases/Réglementation/NBI.aux.non.titulaires.csv)   
[Lien vers la base de données Primes informatiques](Bases/Réglementation/personnels.prime.informatique.csv)   
  
**Nota :**   
NBI: nouvelle bonification indiciaire   
PFI: prime de fonctions informatiques   




 
&nbsp;*Tableau 67*   
   


| Rémunérations de NBI anormales, par agent et par exercice | Montants correspondants |
|:---------------------------------------------------------:|:-----------------------:|
|                            82                             |         37 139          |

  
[Lien vers la base de données NBI anormales](Bases/Fiabilité/lignes.nbi.anormales.csv)   
  
**Nota :**   
*Est considéré comme manifestement anormal un total annuel de rémunérations NBI correspondant à un point d'indice net mensuel inférieur à 4 euros ou supérieur à 6 euros.*    
*Les rappels ne sont pas pris en compte dans les montants versés. Certains écarts peuvent être régularisés en les prenant en compte*     
 
&nbsp;*Tableau 68*   
   


| Année | Cumuls des NBI | Montants versés (a) | Point d'INM apparent | Point d'INM moyen | Contrôle |
|-------|----------------|---------------------|----------------------|-------------------|----------|
| 2009  |    20 840,0    |      92 353,7       |         4,4          |        4,6        |  Orange  |
| 2010  |    19 840,0    |      88 811,3       |         4,5          |        4,6        |   Vert   |
| 2011  |    17 905,0    |      80 552,2       |         4,5          |        4,6        |   Vert   |
| 2012  |    16 230,0    |      72 636,3       |         4,5          |        4,6        |  Orange  |
| 2013  |    15 235,0    |      67 438,4       |         4,4          |        4,6        |  Orange  |


*(a) Hors rappels sur rémunérations*   




  
[Lien vers la base de données des cumuls annuels de NBI](Bases/Fiabilité/cumuls.nbi.csv)   
  
 
## 5.2 Contrôle des vacations pour les fonctionnaires





Il y a  0 fonctionnaire(s) effectuant des vacations pour son propre établissement. Les bulletins concernés sont donnés en lien.


[Lien vers les matricules des fonctionnaires concernés](Bases/Réglementation/matricules.fonctionnaires.et.vacations.csv)
[Lien vers les bulletins de paie correspondants](Bases/Réglementation/lignes.fonctionnaires.et.vacations.csv)

## 5.3 Contrôles sur les cumuls traitement indiciaire, indemnités et vacations des contractuels    




**Contractuels effectuant des vacations (CEV)**

 
&nbsp;*Tableau 69*   
   


| Nombre de CEV | Nombre de lignes | Nombre de lignes indemnitaires | Nombre de lignes de traitement |
|:-------------:|:----------------:|:------------------------------:|:------------------------------:|
|      94       |      26 307      |              133               |              937               |

 
[Lien vers le bulletins des CEV](Bases/Réglementation/lignes.contractuels.et.vacations.csv)   
[Lien vers la base de données Matricules des CEV](Bases/Réglementation/matricules.contractuels.et.vacations.csv)  
[Lien vers la base de données Cumul régime indemnitaire et vacations de CEV](Bases/Réglementation/RI.et.vacations.csv)  
[Lien vers la base de données Lignes de traitement indiciaire pour CEV](Bases/Réglementation/traitement.et.vacations.csv)  
 




## 5.4 Contrôle sur les indemnités IAT et IFTS      




 
&nbsp;*Tableau 70*   
     


|   Codes IFTS   |   |
|:--------------:|:-:|
| 3046 3052 401A |   |



| Nombre de personnels percevant IAT et IFTS |   |
|:------------------------------------------:|:-:|
|                     11                     |   |

  
[Codes IFTS retenus](Bases/Réglementation/codes.ifts.csv)   
[Lien vers la base de données cumuls iat/ifts](Bases/Réglementation/personnels.iat.ifts.csv)    

### Contrôle sur les IFTS pour catégories B et contractuels



 



 




 
&nbsp;*Tableau 71*   
   


| Nombre de lignes de paie de contractuels percevant des IFTS | Nombre de lignes IFTS pour IB < 380 |
|:-----------------------------------------------------------:|:-----------------------------------:|
|                             67                              |                  0                  |


[Lien vers la base de données Lignes IFTS pour contractuels](Bases/Réglementation/ifts.et.contractuel.csv)    
[Lien vers la base de données Lignes IFTS pour IB < 380](Bases/Réglementation/lignes.ifts.anormales.csv)     

**Nota :**
IB < 380 : fonctionnaire percevant un indice brut inférieur à 380





## 5.5 Contrôle de la prime de fonctions et de résultats (PFR)   
  

Tous les attributaires de la PFR sont identifiés en catégorie A.

  
  




 
&nbsp;*Tableau 72*   
     


| Codes PFR | Agents cumulant PFR et IFTS |
|:---------:|:---------------------------:|
|           |              0              |

  
[Lien vers la base de données cumuls pfr/ifts](Bases/Réglementation/personnels.pfr.ifts.csv)    




 
&nbsp;*Tableau 73 : rappel des plafonds annuels de la PFR*   
     


| Adm. général | Adm. HC |  Adm.  | Direct./Attaché princ. | Secr. mairie/Attaché |
|:------------:|:-------:|:------:|:----------------------:|:--------------------:|
|    58 800    | 55 200  | 49 800 |         25 800         |        20 100        |


Les plafonds annuels de la PFR de sont pas dépassés.

 
&nbsp;*Tableau 74* : Valeurs de l'agrégat (PFR ou IFTS) pour les bénéficiaires de la PFR   
         


Aucun bénéficiaire de la PFR détecté.

 
&nbsp;*Tableau 75* : Variations de l'agrégat (PFR ou IFTS) pour les bénéficiaires de la PFR
         


Aucun tableau de variation.

        
  
[Lien vers la base de données agrégat PFR-IFTS](Bases/Rémunérations/bénéficiaires.PFR.csv)    

  
[Lien vers la base de données variations agrégat PFR-IFTS](Bases/Rémunérations/bénéficiaires.PFR.Variation.csv)    





## 5.6 Contrôle sur les heures supplémentaires

Le seuil de 180 heures supplémentaires maximum est dépassé par  59  agents.
 Le seuil de 220 heures supplémentaires maximum est dépassé par  59  agents.


 
&nbsp;*Tableau 76*   
   


| Nombre de lignes HS en excès | Nombre de lignes IHTS anormales |
|:----------------------------:|:-------------------------------:|
|            1 699             |               108               |


[Lien vers la base de données Heures supplémentaires en excès du seuil de 25h/mois](Bases/Réglementation/HS.sup.25.csv)     
[Lien vers la base de données cumuls en excès des seuils annuels](Bases/Réglementation/Dépassement.seuil.180h.csv)    
[Lien vers la base de données IHTS anormales](Bases/Réglementation/ihts.anormales.csv)      

**Nota :**   
HS en excès : au-delà de 25 heures par mois     
IHTS anormales : attribuées à des fonctionnaires ou non-titulaires de catégorie A ou assimilés.     




## 5.7 Contrôle sur les indemnités des élus
  



  


|Matricule |Nom | Année|Emploi               | Indemnités | Autres |   Total |
|:---------|:---|-----:|:--------------------|-----------:|-------:|--------:|
|010108    |YYY |  2009|MAIRE ADJOINT        |    13835,25|       0| 13835,25|
|010108    |YYY |  2010|MAIRE ADJOINT        |    13935,66|       0| 13935,66|
|010108    |YYY |  2011|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|010108    |YYY |  2012|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|010108    |YYY |  2013|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|010183    |YYY |  2009|MAIRE ADJOINT        |    13835,25|       0| 13835,25|
|010183    |YYY |  2010|MAIRE ADJOINT        |    13935,66|       0| 13935,66|
|010183    |YYY |  2011|CONSEILLER MUNICIPAL |     4892,47|       0|  4892,47|
|010183    |YYY |  2012|CONSEILLER MUNICIPAL |     5337,24|       0|  5337,24|
|010183    |YYY |  2013|CONSEILLER MUNICIPAL |     5337,24|       0|  5337,24|
|300918    |YYY |  2009|MAIRE ADJOINT        |    13835,25|       0| 13835,25|
|300918    |YYY |  2010|MAIRE ADJOINT        |    13935,66|       0| 13935,66|
|300918    |YYY |  2011|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300918    |YYY |  2012|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300918    |YYY |  2013|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300919    |YYY |  2009|MAIRE ADJOINT        |    13835,25|       0| 13835,25|
|300919    |YYY |  2010|MAIRE ADJOINT        |    13935,66|       0| 13935,66|
|300919    |YYY |  2011|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300919    |YYY |  2012|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300919    |YYY |  2013|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300920    |YYY |  2009|MAIRE ADJOINT        |    13835,25|       0| 13835,25|
|300920    |YYY |  2010|MAIRE ADJOINT        |    13935,66|       0| 13935,66|
|300920    |YYY |  2011|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300920    |YYY |  2012|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300920    |YYY |  2013|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300921    |YYY |  2009|MAIRE ADJOINT        |    13835,25|       0| 13835,25|
|300921    |YYY |  2010|MAIRE ADJOINT        |    13935,66|       0| 13935,66|
|300921    |YYY |  2011|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300921    |YYY |  2012|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300921    |YYY |  2013|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300922    |YYY |  2009|MAIRE ADJOINT        |    13835,25|       0| 13835,25|
|300922    |YYY |  2010|MAIRE ADJOINT        |    13935,66|       0| 13935,66|
|300922    |YYY |  2011|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300922    |YYY |  2012|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300922    |YYY |  2013|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300923    |YYY |  2009|MAIRE ADJOINT        |    13835,25|       0| 13835,25|
|300923    |YYY |  2010|MAIRE ADJOINT        |    13935,66|       0| 13935,66|
|300923    |YYY |  2011|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300923    |YYY |  2012|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300923    |YYY |  2013|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300924    |YYY |  2009|MAIRE ADJOINT        |    13835,25|       0| 13835,25|
|300924    |YYY |  2010|MAIRE ADJOINT        |    13935,66|       0| 13935,66|
|300924    |YYY |  2011|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300924    |YYY |  2012|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300924    |YYY |  2013|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300929    |YYY |  2009|MAIRE                |    47435,16|       0| 47435,16|
|300929    |YYY |  2010|MAIRE                |    47779,38|       0| 47779,38|
|300929    |YYY |  2011|MAIRE                |    34056,38|       0| 34056,38|
|300929    |YYY |  2012|MAIRE                |    34099,07|       0| 34099,07|
|300929    |YYY |  2013|MAIRE                |    39658,80|       0| 39658,80|
|300930    |YYY |  2009|MAIRE ADJOINT        |    13835,25|       0| 13835,25|
|300930    |YYY |  2010|MAIRE ADJOINT        |    13935,66|       0| 13935,66|
|300930    |YYY |  2011|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300930    |YYY |  2012|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300930    |YYY |  2013|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|300932    |YYY |  2009|CONSEILLER MUNICIPAL |     3767,73|       0|  3767,73|
|300932    |YYY |  2010|CONSEILLER MUNICIPAL |     3795,06|       0|  3795,06|
|300932    |YYY |  2011|CONSEILLER MUNICIPAL |     3804,48|       0|  3804,48|
|300932    |YYY |  2012|CONSEILLER MUNICIPAL |     3804,48|       0|  3804,48|
|300932    |YYY |  2013|CONSEILLER MUNICIPAL |     3804,48|       0|  3804,48|
|300935    |YYY |  2009|CONSEILLER MUNICIPAL |     3767,73|       0|  3767,73|
|300935    |YYY |  2010|CONSEILLER MUNICIPAL |     3795,06|       0|  3795,06|
|300935    |YYY |  2011|CONSEILLER MUNICIPAL |     3804,48|       0|  3804,48|
|300935    |YYY |  2012|CONSEILLER MUNICIPAL |     3804,48|       0|  3804,48|
|300935    |YYY |  2013|CONSEILLER MUNICIPAL |     3804,48|       0|  3804,48|
|301116    |YYY |  2009|CONSEILLER MUNICIPAL |     9237,50|       0|  9237,50|
|301116    |YYY |  2010|CONSEILLER MUNICIPAL |    10147,44|       0| 10147,44|
|301116    |YYY |  2011|CONSEILLER MUNICIPAL |    13653,93|       0| 13653,93|
|301116    |YYY |  2012|MAIRE ADJOINT        |    13970,40|       0| 13970,40|
|301116    |YYY |  2013|MAIRE ADJOINT        |    13970,40|       0| 13970,40|

[Lien vers la base de données Rémunérations des élus](Bases/Réglementation/rémunérations.élu.csv)




## 5.8 Lien avec le compte de gestion



 
Cumul des lignes de paie par exercice et catégorie de ligne de paie   
 


Tableau  77  Année  2009

|           Catégorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnités| 1 334 263,04|
|             Rappels|   244 938,75|
|       Rém. diverses|    27 860,93|
| Supplément familial|   146 331,98|
|         Traitements| 9 127 148,03|

Tableau  78  Année  2010

|           Catégorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnités| 1 334 245,36|
|             Rappels|   165 096,19|
|       Rém. diverses|    29 337,19|
| Supplément familial|   148 838,36|
|         Traitements| 9 315 116,31|

Tableau  79  Année  2011

|           Catégorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnités| 1 325 516,15|
|             Rappels|   156 348,33|
|       Rém. diverses|    17 323,77|
| Supplément familial|   140 162,84|
|         Traitements| 9 070 866,33|

Tableau  80  Année  2012

|           Catégorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnités| 1 532 916,11|
|             Rappels|   156 613,18|
|       Rém. diverses|    35 882,81|
| Supplément familial|   138 565,19|
|         Traitements| 8 861 048,17|

Tableau  81  Année  2013

|           Catégorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnités| 1 586 973,59|
|             Rappels|   198 605,80|
|       Rém. diverses|    51 331,04|
| Supplément familial|   141 995,23|
|         Traitements| 9 069 382,34|

 
[Lien vers la base détaillée des cumuls des lignes de paie](Bases/Réglementation/cumul.lignes.paie.csv)
 
[Lien vers la base agrégée des cumuls des lignes de paie](Bases/Réglementation/cumul.total.lignes.paie.csv)
 
 
*Avertissement : les rappels comprennent également les rappels de cotisations et déductions diverses.*    
  




## 5.9 Contrôle du supplément familial de traitement   
 


Pour les agents n'ayant pas d'enfant signalé en base, il a été détecté 187 bulletins de paie présentant un paiement du SFT apparemment anormal.

 
[Lien vers la base des paiements de SFT à agents sans enfant signalé](Bases/Réglementation/Paie.sans.enfant.réduit.csv)
 


Pour les agents ayant au moins un enfant, il a été détecté 49 bulletins de paie présentant un écart de paiement du SFT supérieur à 1 euro.

 
[Lien vers la base des écarts de paiement sur SFT](Bases/Réglementation/controle.sft.csv)
 



# Annexe
## Liens complémentaires

[Lien vers le fichier des personnels](Bases/Effectifs/Catégories des personnels.csv)
 
## Fiabilité du traitement statistique   
*Doublons*      

Attention : Altaïr a détecté des lignes dupliquées alors qu'aucun retraitement des lignes dupliquées n'est prévu par défaut.

 



 
*Tests de fiabilité sur le renseignement des heures et des quotités*    
  



    

Nombre de bulletins :  32 107

    

Les heures de travail ont été redressées avec la méthode  des quotités.

   

 Nombre de bulletins de paie redressés : 525

   

 Pourcentage de redressements : 1,64 % des bulletins de paie.

 


Pourcentage d'heures renseignées (après redressement éventuel): 95,4 %

  


Pourcentage de quotités renseignées : 99,4 %

  


Nombre de bulletins à heures et quotités :  30579 [ 95,2 %]

  


Nombre de bulletins à heures sans quotités :  53 [ 0,2 %]

  


Nombre de bulletins à quotités sans heures :  1334 [ 4,2 %]

  


Nombre de bulletins apparemment inactifs :  141 [ 0,4 %]

  



 

Nombre de bulletins de paie de salaires versés pour un champ Heures = 0 :  1 398 [ 4,4 %]

  



  
[Lien vers la base de données des salaires versés pour Heures=0](Bases/Fiabilité/base.heures.nulles.salaire.nonnull.csv)   
[Lien vers la base de données des salaires versés à quotité indéfinie](Bases/Fiabilité/base.quotité.indéfinie.salaire.nonnull.csv)   

# Tableau des personnels : renseigner la catégorie

Utiliser les codes : A, B, C, ELU, AUTRES

En cas de changement de catégorie en cours de période, utiliser la catégorie AUTRES
Cela peut conduire à modifier manuellement le fichier Catégories des personnels.csv



Non généré  [anonymisation]


