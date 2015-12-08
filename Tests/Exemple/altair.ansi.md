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


```
## Error in importer.bases.via.xhl2csv("Paie", colClasses = colonnes.classes.input): Chargement de la table bulletins-lignes de paie en échec.
```


<p class = "centered"><b>Exercices 2013 à 2014 </b></p>
<p class = "author">Fabrice Nicol</h1>




mar. 08 déc. 2015
   


La durée du travail prise en compte dans la base de données est de  151,67  h par mois.

```
## Error in eval(expr, envir, enclos): could not find function "."
```

# 1. Statistiques de population

### 1.1 Effectifs



 



&nbsp;*Tableau 1*   


|                                                             | 2009  | 2010  | 2011  | 2012  | 2013  |
|:------------------------------------------------------------|:-----:|:-----:|:-----:|:-----:|:-----:|
|Total effectifs (a)                                          | 709,0 | 769,0 | 698,0 | 714,0 | 703,0 |
|&nbsp;&nbsp;&nbsp;dont présents 12 mois                      | 465,0 | 462,0 | 444,0 | 428,0 | 433,0 |
|&nbsp;&nbsp;&nbsp;dont fonctionnaires (b)                    | 438,0 | 431,0 | 423,0 | 427,0 | 417,0 |
|&nbsp;&nbsp;&nbsp;dont fonct. présents 12 mois               | 400,0 | 403,0 | 402,0 | 384,0 | 391,0 |
|&nbsp;&nbsp;&nbsp;dont non titulaires                        | 58,0  | 52,0  | 56,0  | 57,0  | 73,0  |
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
   


|  Statistique  | Âge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  16                  |          |
| 1er quartile  |                  35                  |          |
|    Médiane    |                  43                  |          |
|    Moyenne    |                42,45                 |   542    |
| 3ème quartile |                  51                  |          |
|    Maximum    |                  66                  |          |

 

![plot of chunk unnamed-chunk-9](figure/unnamed-chunk-9-1.png) 

 
&nbsp;*Tableau 3*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2014 | Effectif |
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
   


|  Statistique  | Âge des personnels non titulaires<br>au 31/12/2013 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         17                         |          |
| 1er quartile  |                         24                         |          |
|    Médiane    |                        37,5                        |          |
|    Moyenne    |                       36,97                        |   100    |
| 3ème quartile |                       46,25                        |          |
|    Maximum    |                         66                         |          |

 

![plot of chunk unnamed-chunk-13](figure/unnamed-chunk-13-1.png) 

 
&nbsp;*Tableau 5*   
   


|  Statistique  | Âge des personnels non titulaires<br>au 31/12/2014 | Effectif |
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
   


|  Statistique  | Âge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  24                  |          |
| 1er quartile  |                  38                  |          |
|    Médiane    |                  45                  |          |
|    Moyenne    |                44,18                 |   410    |
| 3ème quartile |                  51                  |          |
|    Maximum    |                  62                  |          |

 


![plot of chunk unnamed-chunk-16](figure/unnamed-chunk-16-1.png) 

 
&nbsp;*Tableau 7*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2014 | Effectif |
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
   


|  Statistique  | Âge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  17                  |          |
| 1er quartile  |                  36                  |          |
|    Médiane    |                 43,5                 |          |
|    Moyenne    |                42,76                 |   510    |
| 3ème quartile |                  51                  |          |
|    Maximum    |                  66                  |          |

 

![plot of chunk unnamed-chunk-20](figure/unnamed-chunk-20-1.png) 

&nbsp;*Tableau 9*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2014 | Effectif |
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

**Personnels en fonction (hors élus) des exercices 2013 à 2014 inclus :**

 
&nbsp;*Tableau 10*   
   


| Plus de 2 ans | Moins de 2 ans | Moins d'un an | Moins de six mois |
|:-------------:|:--------------:|:-------------:|:-----------------:|
|      453      |      140       |      84       |        23         |



![plot of chunk unnamed-chunk-24](figure/unnamed-chunk-24-1.png) 


**Effectifs (hors élus)**   




 
&nbsp;*Tableau 11*   
   


|                  | 2009 | 2010 | 2011 | 2012 | 2013 |
|:-----------------|:----:|:----:|:----:|:----:|:----:|
|Plus de deux ans  | 436  | 448  | 448  | 428  | 408  |
|Moins de deux ans |  35  |  24  |  24  |  42  |  68  |
|Total             | 471  | 472  | 472  | 470  | 476  |



**Nota :**
*Personnels en place : ayant servi au moins deux années consécutives pendant la période.*     
*Plus/moins de deux ans : plus/mois de 730 jours sur la période sous revue.*   



  
  

```
## Error in eval(expr, envir, enclos): could not find function "sélectionner.exercice.analyse.rémunérations"
```

   
# 2. Rémunérations brutes : analyse pour l'exercice 2009    
   
## 2.1 Masse salariale brute de l'ensemble des agents     
    
 

```
## Error in is.data.frame(x): object 'Analyse.rémunérations.exercice' not found
```

### Cumuls des rémunérations brutes pour l'exercice 2009   
 
*Personnels (hors élus)*     
 
&nbsp;*Tableau 12*   
   

```
## Error in `[.data.table`(masses.personnels, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```

   

**Définitions :**   

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
 *Indemnités d'élu*          : toutes rémunérations indemnitaires des élus    
 *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
    
**Tests de cohérence**   

Somme des rémunérations brutes versées aux personnels (non élus) :   
 
 
&nbsp;*Tableau 13*    
   

```
## Error in `[.data.table`(masses.personnels, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```

   
à comparer aux soldes des comptes 641 et 648 du compte de gestion.   


  
## 2.2 Masse salariale brute des fonctionnaires    

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*   

```
## Error in eval(expr, envir, enclos): object 'Analyse.rémunérations.exercice' not found
```

![plot of chunk unnamed-chunk-127](figure/unnamed-chunk-127-1.png) 
![plot of chunk unnamed-chunk-128](figure/unnamed-chunk-128-1.png) 
![plot of chunk unnamed-chunk-129](figure/unnamed-chunk-129-1.png) 
![plot of chunk unnamed-chunk-130](figure/unnamed-chunk-130-1.png) 
![plot of chunk unnamed-chunk-131](figure/unnamed-chunk-131-1.png) 

![plot of chunk unnamed-chunk-132](figure/unnamed-chunk-132-1.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio rémunération / quotité*   
   
**Effectif : 84**   
   
**Tests de cohérence**   


   
Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :    

 
&nbsp;*Tableau 14*   
   


|           Agrégats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 1 049 352,5 |
| Brut annuel (lignes), dont : | 1 027 334,0 |
|         \ \ Primes :         |  112 593,4  |
|   \ \ Autres rémunérations   |   2 947,6   |
|     Part de primes en %      |    10,7     |

    
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
| Bulletins de paie | 1 049 352,5 |
|  Lignes de paie   | 1 027 334,0 |
|    Différence     |  22 018,5   |

   
A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.    

**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2009**      
 
&nbsp;*Tableau 16*   
   


|   Statistique| Traitement indiciaire|    Primes| Autres rémunérations| Effectif|
|-------------:|---------------------:|---------:|--------------------:|--------:|
|       Minimum|                 3 039|    -22,25|                 0,00|         |
|  1er quartile|                 6 244|      0,00|                 0,00|         |
|       Médiane|                 8 859|    286,40|                 0,00|         |
|       Moyenne|                10 882|  1 353,82|                35,51|       83|
| 3ème quartile|                13 138|  1 112,86|                 0,00|         |
|       Maximum|                44 200| 17 494,04|             1 100,00|         |

 
&nbsp;*Tableau 17*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la rém. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     3 246|      3 830|             4 806|                     -0,2 483|         |
|  1er quartile|                     6 533|      6 872|            17 524|                      0,0 000|         |
|       Médiane|                     8 985|      9 699|            19 070|                      3,0 647|         |
|       Moyenne|                    12 236|     12 472|            19 898|                      5,8 142|       83|
| 3ème quartile|                    13 995|     14 436|            20 391|                      8,0 952|         |
|       Maximum|                    61 694|     58 944|            58 944|                     36,9 267|         |

   
*Hors vacataires identifiés, assistantes maternelles, élus locaux et pour les postes actifs non annexes*   

**Catégorie A**    


Pas de statistique en l'absence de fichier des catégories.

    
**Effectif : 0**  

**Catégorie B**    
    

Pas de statistique en l'absence de fichier des catégories.


**Effectif : 0**    

**Catégorie C**    


Pas de statistique en l'absence de fichier des catégories.

**Effectif : 0**    

## 2.3 Contractuels, vacataires et stagiaires inclus     
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   

```
## Error in attach(Analyse.rémunérations.exercice, warn.conflicts = FALSE): object 'Analyse.rémunérations.exercice' not found
```

```
## Error in eval(expr, envir, enclos): object 'rémunération.indemnitaire.imposable.eqtp' not found
```



![plot of chunk unnamed-chunk-141](figure/unnamed-chunk-141-1.png) 

  
**Nota :**   
*Ne sont retenues que les rémunérations supérieures à 1 000 k&euro;.*  
*Les élus ne sont pas pris en compte.*  
  

```
## Error in positive(autres.rémunérations): object 'autres.rémunérations' not found
```

![plot of chunk unnamed-chunk-142](figure/unnamed-chunk-142-1.png) 

```
## Error in eval(expr, envir, enclos): object 'Analyse.rémunérations.exercice' not found
```

  
**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2009**     
  
 
&nbsp;*Tableau 18*   
   


|   Statistique|    Primes| Autres rémunérations| Effectif|
|-------------:|---------:|--------------------:|--------:|
|       Minimum|    -22,25|                 0,00|         |
|  1er quartile|      0,00|                 0,00|         |
|       Médiane|    264,70|                 0,00|         |
|       Moyenne|  1 340,40|                35,09|       84|
| 3ème quartile|  1 087,85|                 0,00|         |
|       Maximum| 17 494,04|             1 100,00|         |

   
&nbsp;*Tableau 19*   
   
   

|   Statistique| Total rémunérations| Total rémunérations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               3 830|                    4 806|         |
|  1er quartile|               6 877|                   17 484|         |
|       Médiane|               9 708|                   19 009|         |
|       Moyenne|              12 492|                   19 835|       84|
| 3ème quartile|              14 380|                   20 368|         |
|       Maximum|              58 944|                   58 944|         |


  
  

```
## Error in eval(expr, envir, enclos): could not find function "sélectionner.exercice.analyse.rémunérations"
```

   
# 3. Rémunérations brutes : analyse pour l'exercice 2013    
   
## 3.1 Masse salariale brute de l'ensemble des agents     
    
 

```
## Error in is.data.frame(x): object 'Analyse.rémunérations.exercice' not found
```

### Cumuls des rémunérations brutes pour l'exercice 2013   
 
*Personnels (hors élus)*     
 
&nbsp;*Tableau 20*   
   

```
## Error in `[.data.table`(masses.personnels, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```

   

**Définitions :**   

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
 *Indemnités d'élu*          : toutes rémunérations indemnitaires des élus    
 *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
    
**Tests de cohérence**   

Somme des rémunérations brutes versées aux personnels (non élus) :   
 
 
&nbsp;*Tableau 21*    
   

```
## Error in `[.data.table`(masses.personnels, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```

   
à comparer aux soldes des comptes 641 et 648 du compte de gestion.   


  
## 3.2 Masse salariale brute des fonctionnaires    

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*   

```
## Error in eval(expr, envir, enclos): object 'Analyse.rémunérations.exercice' not found
```

![plot of chunk unnamed-chunk-149](figure/unnamed-chunk-149-1.png) 
![plot of chunk unnamed-chunk-150](figure/unnamed-chunk-150-1.png) 
![plot of chunk unnamed-chunk-151](figure/unnamed-chunk-151-1.png) 
![plot of chunk unnamed-chunk-152](figure/unnamed-chunk-152-1.png) 
![plot of chunk unnamed-chunk-153](figure/unnamed-chunk-153-1.png) 

![plot of chunk unnamed-chunk-154](figure/unnamed-chunk-154-1.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio rémunération / quotité*   
   
**Effectif : 84**   
   
**Tests de cohérence**   


   
Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :    

 
&nbsp;*Tableau 22*   
   


|           Agrégats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 1 049 352,5 |
| Brut annuel (lignes), dont : | 1 027 334,0 |
|         \ \ Primes :         |  112 593,4  |
|   \ \ Autres rémunérations   |   2 947,6   |
|     Part de primes en %      |    10,7     |

    
**Définitions :**     
   
 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
 *Indemnités d'élus*         : toutes rémunérations indemnitaires des élus    
 *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
   
**Tests de cohérence**   

Somme des rémunérations brutes versées aux personnels (non élus) :   

 
&nbsp;*Tableau 23*      
   


|     Agrégats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 1 049 352,5 |
|  Lignes de paie   | 1 027 334,0 |
|    Différence     |  22 018,5   |

   
A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.    

**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2013**      
 
&nbsp;*Tableau 24*   
   


|   Statistique| Traitement indiciaire|    Primes| Autres rémunérations| Effectif|
|-------------:|---------------------:|---------:|--------------------:|--------:|
|       Minimum|                 3 039|    -22,25|                 0,00|         |
|  1er quartile|                 6 244|      0,00|                 0,00|         |
|       Médiane|                 8 859|    286,40|                 0,00|         |
|       Moyenne|                10 882|  1 353,82|                35,51|       83|
| 3ème quartile|                13 138|  1 112,86|                 0,00|         |
|       Maximum|                44 200| 17 494,04|             1 100,00|         |

 
&nbsp;*Tableau 25*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la rém. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     3 246|      3 830|             4 806|                     -0,2 483|         |
|  1er quartile|                     6 533|      6 872|            17 524|                      0,0 000|         |
|       Médiane|                     8 985|      9 699|            19 070|                      3,0 647|         |
|       Moyenne|                    12 236|     12 472|            19 898|                      5,8 142|       83|
| 3ème quartile|                    13 995|     14 436|            20 391|                      8,0 952|         |
|       Maximum|                    61 694|     58 944|            58 944|                     36,9 267|         |

   
*Hors vacataires identifiés, assistantes maternelles, élus locaux et pour les postes actifs non annexes*   

**Catégorie A**    


Pas de statistique en l'absence de fichier des catégories.

    
**Effectif : 0**  

**Catégorie B**    
    

Pas de statistique en l'absence de fichier des catégories.


**Effectif : 0**    

**Catégorie C**    


Pas de statistique en l'absence de fichier des catégories.

**Effectif : 0**    

## 3.3 Contractuels, vacataires et stagiaires inclus     
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   

```
## Error in attach(Analyse.rémunérations.exercice, warn.conflicts = FALSE): object 'Analyse.rémunérations.exercice' not found
```

```
## Error in eval(expr, envir, enclos): object 'rémunération.indemnitaire.imposable.eqtp' not found
```



![plot of chunk unnamed-chunk-163](figure/unnamed-chunk-163-1.png) 

  
**Nota :**   
*Ne sont retenues que les rémunérations supérieures à 1 000 k&euro;.*  
*Les élus ne sont pas pris en compte.*  
  

```
## Error in positive(autres.rémunérations): object 'autres.rémunérations' not found
```

![plot of chunk unnamed-chunk-164](figure/unnamed-chunk-164-1.png) 

```
## Error in eval(expr, envir, enclos): object 'Analyse.rémunérations.exercice' not found
```

  
**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2013**     
  
 
&nbsp;*Tableau 26*   
   


|   Statistique|    Primes| Autres rémunérations| Effectif|
|-------------:|---------:|--------------------:|--------:|
|       Minimum|    -22,25|                 0,00|         |
|  1er quartile|      0,00|                 0,00|         |
|       Médiane|    264,70|                 0,00|         |
|       Moyenne|  1 340,40|                35,09|       84|
| 3ème quartile|  1 087,85|                 0,00|         |
|       Maximum| 17 494,04|             1 100,00|         |

   
&nbsp;*Tableau 27*   
   
   

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
 
&nbsp;*Tableau 28*   
   


| Agrégat (&euro;) | Salaires bruts 2011 | Salaires bruts 2012 | Salaires bruts 2013 |
|------------------|---------------------|---------------------|---------------------|
|     Ensemble     |      25 908,0       |      26 340,0       |      26 616,0       |
|    Titulaires    |      26 676,0       |      27 108,0       |      27 444,0       |
| Autres salariés  |      22 836,0       |         NA          |      24 360,0       |

  
**Eléments de la rémunération brute pour les titulaires de la FPT entre 2010 et 2012**      
  
 
&nbsp;*Tableau 29*   
   


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
  




# 4. Rémunérations nettes : évolutions sur la période 2013 - 2014    

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

## 4.2 Evolutions des rémunérations nettes sur la période 2013 - 2014   

### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors élus)





   
**Salaire net moyen par tête (SMPT net) en EQTP, hors élus**         
      
 
&nbsp;*Tableau 30*   
   


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
 
&nbsp;*Tableau 31*   
   


```
## Error in FUN(X[[i]], ...): attempt to select less than one element
```


*MS N-1 : masse salariale nette de l'année n-1.*   
      
**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
      
 
&nbsp;*Tableau 32*   
   


|   Statistique| Première année|
|-------------:|--------------:|
|       Minimum|          4 794|
|  1er quartile|         16 990|
|       Médiane|         18 670|
|       Moyenne|         20 050|
| 3ème quartile|         22 030|
|       Maximum|         54 530|



|   Statistique| Dernière année|
|-------------:|--------------:|
|       Minimum|             NA|
|  1er quartile|             NA|
|       Médiane|             NA|
|       Moyenne|            NaN|
| 3ème quartile|             NA|
|       Maximum|             NA|

 
*Nota :*  La population retenue est constituée des agents qui :   
&nbsp;&nbsp;- ne font pas partie des 2 centiles extrêmaux   
&nbsp;&nbsp;- sont au moins présents 1 jour(s) la première et la dernière année d'activité  
Les élus, vacataires et assistantes maternelles sont retirés du périmètre.   
Seuls sont pris en compte les agents ayant connu au moins un mois actif et ayant eu, sur l'année, des rémunérations non annexes.  
[Compléments méthodologiques](Docs/méthodologie.pdf)     
     
**Comparaisons source INSEE/DGCL**   

**Salaires annuels moyens 2011 et 2012 en EQTP (hors assistantes maternelles)**   
 
&nbsp;*Tableau 33*       


|     Agrégat     | Salaires nets 2011 (&euro;) | Salaires nets 2012 (&euro;) | Salaires nets 2013 (&euro;) |
|-----------------|-----------------------------|-----------------------------|-----------------------------|
|    Ensemble     |          21 876,0           |          22 176,0           |          22 224,0           |
|   Titulaires    |          22 632,0           |          22 920,0           |          22 920,0           |
| Autres salariés |          18 864,0           |             NA              |             NA              |

*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			



**Distribution des salaires nets annuels en EQTP dans la fonction publique par versant en 2011**   

 
&nbsp;*Tableau 34*   
   


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
 
&nbsp;*Tableau 35*   
   


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
&nbsp;*Tableau 36*   
   


```
## Error in data.frame(c("7 985,2", "7 985,2", "7 985,2", "7 985,2"), c("7 944,9", : les arguments impliquent des nombres de lignes différents : 4, 0, 1
```





  
**Effet de noria sur salaires nets et taux de remplacements**       
 
&nbsp;*Tableau 37*   
   


```
## Error in e[[y]]: attempt to select less than one element
```

    
*MS N-1 : masse salariale nette de l'année n-1.*   
      
**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
 
&nbsp;*Tableau 38*   
   


|   Statistique| Première année|
|-------------:|--------------:|
|       Minimum|          8 883|
|  1er quartile|         17 480|
|       Médiane|         19 420|
|       Moyenne|         20 820|
| 3ème quartile|         22 730|
|       Maximum|         54 530|



|   Statistique| Dernière année|
|-------------:|--------------:|
|       Minimum|             NA|
|  1er quartile|             NA|
|       Médiane|             NA|
|       Moyenne|            NaN|
| 3ème quartile|             NA|
|       Maximum|             NA|

   



  
**Evolution du SMPT net des titulaires à temps complet**     
  
 
&nbsp;*Tableau 39*   
   


| Année | Rémunération nette totale (k&euro;) | Variation (%) | SMPT (&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:-------------:|:-------------:|
| 2009  |               5 703,4               |               |    19 667     |               |
| 2010  |                6 052                |               |    19 974     |               |
| 2011  |               5 934,6               |     1,35      |    20 049     |     5,34      |
| 2012  |               5 356,5               |               |    20 290     |               |
| 2013  |               5 780,4               |               |    20 718     |               |

   
**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) des titulaires à temps complet**         
      
 
&nbsp;*Tableau 40*   
   


|   Statistique| Première année|
|-------------:|--------------:|
|       Minimum|          8 883|
|  1er quartile|         17 400|
|       Médiane|         19 280|
|       Moyenne|         20 720|
| 3ème quartile|         22 210|
|       Maximum|         54 530|

  


|   Statistique| Dernière année|
|-------------:|--------------:|
|       Minimum|             NA|
|  1er quartile|             NA|
|       Médiane|             NA|
|       Moyenne|            NaN|
| 3ème quartile|             NA|
|       Maximum|             NA|


[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)


## 4.3 Glissement vieillesse-technicité (GVT)   

### 4.3.1 Ensemble des personnels   
  
*Cette section est consacrée à la rémunération moyenne des personnes en place (RMPP), définies comme présentes deux années entières consécutives avec la même quotité*   
*L'évolution de la RMPP permet d'étudier le glissement vieillesse-technicité "positif", à effectifs constants sur deux années*      
*Le GVT positif est dû aux mesures statutaires et individuelles, à l'avancement et aux changements d'activité*  

![plot of chunk unnamed-chunk-51](figure/unnamed-chunk-51-1.png) 






  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 41*   
   


| Année | Rémunération nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2010  |               7 240,1               |               |        19 890        |               |
| 2011  |               7 350,9               |    -0,551     |        19 867        |     4,93      |
| 2012  |               6 955,7               |               |        20 398        |               |
| 2013  |               7 200,2               |               |        20 870        |               |

   
**Distribution et variation sur la période de la rémunération nette des personnes en place**                
 
&nbsp;*Tableau 42*   
   



 


|   Statistique| Première année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          7 081|         |
|  1er quartile|         15 860|         |
|       Médiane|         17 480|         |
|       Moyenne|         19 190|      417|
| 3ème quartile|         20 730|         |
|       Maximum|         56 150|         |

 
&nbsp;*Tableau 43*   
   


|   Statistique| Dernière année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|         10 140|         |
|  1er quartile|         17 240|         |
|       Médiane|         19 040|         |
|       Moyenne|         20 460|      416|
| 3ème quartile|         22 150|         |
|       Maximum|         54 530|         |


*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
 
&nbsp;*Tableau 44*   
   


|   Statistique| Variation normalisée (%)| Variation annuelle moyenne normalisée (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -36,523|                                   -10,741|         |
|  1er quartile|                    5,698|                                     1,395|         |
|       Médiane|                   10,318|                                     2,485|         |
|       Moyenne|                   10,853|                                     2,530|      359|
| 3ème quartile|                   16,113|                                     3,805|         |
|       Maximum|                   38,583|                                     8,499|         |

   
### 4.3.2 Titulaires et stagiaires     
  



  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 45*   
   


| Année | Rémunération nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2010  |               6 896,8               |               |        19 818        |               |
| 2011  |               7 009,9               |     0,365     |        19 858        |     4,89      |
| 2012  |               6 728,9               |               |        20 329        |               |
| 2013  |                6 922                |               |        20 787        |               |

   
**Distribution et variation sur la période de la rémunération nette des fonctionnaires en place**                
 
&nbsp;*Tableau 46*   
   
 


|   Statistique| Première année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          7 081|         |
|  1er quartile|         15 980|         |
|       Médiane|         17 630|         |
|       Moyenne|         19 350|      384|
| 3ème quartile|         20 820|         |
|       Maximum|         56 150|         |

 
&nbsp;*Tableau 47*   
   


|   Statistique| Dernière année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          4 743|         |
|  1er quartile|         17 400|         |
|       Médiane|         19 430|         |
|       Moyenne|         21 120|      428|
| 3ème quartile|         23 040|         |
|       Maximum|         63 190|         |


*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
 
&nbsp;*Tableau 48*   
   


|   Statistique| Variation normalisée (%)| Variation annuelle moyenne normalisée (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -36,523|                                   -10,741|         |
|  1er quartile|                    5,885|                                     1,440|         |
|       Médiane|                   10,354|                                     2,494|         |
|       Moyenne|                   10,960|                                     2,558|      345|
| 3ème quartile|                   16,096|                                     3,802|         |
|       Maximum|                   38,583|                                     8,499|         |



[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)

**Nota**   
*Personnes en place :* en fonction au moins deux années consécutives sur la période 2013 à 2014    
*Variation sur la période d'activité :* entre l'arrivée et le départ de la personne      
*Variation normalisée :* conforme à la définition INSEE (présente en début et en fin de période avec la même quotité)  
 
**Commentaire**       
Les différences éventuelles constatées entre l'évolution de la RMPP au tableau 46 sont dues soit à l'effet de noria soit à l'effet périmètre.    
     
[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)


### 4.4 Comparaisons avec la situation nationale des rémunérations   
 
**Évolution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    
 
&nbsp;*Tableau 49*   
   


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
  
 
&nbsp;*Tableau 50*   
   


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
## 5.1 Contrôle des heures supplémentaires, des NBI et primes informatiques   
  
**Dans cette partie, l'ensemble de la base de paie est étudié.**  
Les agents non actifs ou dont le poste est annexe sont réintroduits dans le périmètre.   


```
## Error in uniqueN(Paie[Statut != "TITULAIRE" & Statut != "STAGIAIRE" & : object 'Statut' not found
```

```
## Error in eval(expr, envir, enclos): object 'Statut' not found
```

```
## Error in grep(expression.rég.pfi, Libellé, ignore.case = TRUE, perl = TRUE): object 'Libellé' not found
```

```
## Error in unique(Libellé[filtre], by = NULL): object 'Libellé' not found
```

Primes informatiques potentielles : PRIME FONCTION INFORMATIQUE
 
&nbsp;*Tableau 51*   
   


```
## Error in Tableau(c("Nombre de lignes NBI pour non titulaires", "Nombre de bénéficiaires de PFI"), : 'names' attribute [2] must be the same length as the vector [1]
```

  
[Lien vers la base de données NBI aux non titulaires](Bases/Réglementation/NBI.aux.non.titulaires.csv)   
[Lien vers la base de données Primes informatiques](Bases/Réglementation/personnels.prime.informatique.csv)   
  
**Nota :**   
NBI: nouvelle bonification indiciaire   
PFI: prime de fonctions informatiques   



```
## Error in grepl(expression.rég.nbi, Libellé, perl = TRUE, ignore.case = TRUE): object 'Libellé' not found
```

```
## Error in eval(expr, envir, enclos): object 'T2' not found
```

```
## Error in is.data.table(y): object 'T2' not found
```

```
## Error in eval(expr, envir, enclos): could not find function "."
```

```
## Error in `:=`(ratio, nbi.cumul.montants/nbi.cumul.indiciaire): Check that is.data.table(DT) == TRUE. Otherwise, := and `:=`(...) are defined for use in j, once only and in particular ways. See help(":=").
```

```
## Error in `:=`(nbi.anormale, (abs(ratio) < 4 | abs(ratio) > 6)): Check that is.data.table(DT) == TRUE. Otherwise, := and `:=`(...) are defined for use in j, once only and in particular ways. See help(":=").
```

```
## Error in eval(expr, envir, enclos): object 'nbi.anormale' not found
```

 
&nbsp;*Tableau 52*   
   


| Rémunérations de NBI anormales, par agent et par exercice | Montants correspondants |
|:---------------------------------------------------------:|:-----------------------:|
|                            82                             |         37 139          |

  
[Lien vers la base de données NBI anormales](Bases/Fiabilité/lignes.nbi.anormales.csv)   
  
**Nota :**   
*Est considéré comme manifestement anormal un total annuel de rémunérations NBI correspondant à un point d'indice net mensuel inférieur à 4 euros ou supérieur à 6 euros.*    
*Les rappels ne sont pas pris en compte dans les montants versés. Certains écarts peuvent être régularisés en les prenant en compte*     
 
&nbsp;*Tableau 53*   
   


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


```
## Error in eval(expr, envir, enclos): object 'Statut' not found
```



Il y a  0 fonctionnaire(s) effectuant des vacations pour son propre établissement. Les bulletins concernés sont donnés en lien.


[Lien vers les matricules des fonctionnaires concernés](Bases/Réglementation/matricules.fonctionnaires.et.vacations.csv)
[Lien vers les bulletins de paie correspondants](Bases/Réglementation/lignes.fonctionnaires.et.vacations.csv)

## 5.3 Contrôles sur les cumuls traitement indiciaire, indemnités et vacations des contractuels    


```
## Error in eval(expr, envir, enclos): object 'Statut' not found
```

```
## Error in eval(expr, envir, enclos): object 'Type' not found
```


**Contractuels effectuant des vacations (CEV)**

 
&nbsp;*Tableau 54*   
   


| Nombre de CEV | Nombre de lignes | Nombre de lignes indemnitaires | Nombre de lignes de traitement |
|:-------------:|:----------------:|:------------------------------:|:------------------------------:|
|      94       |      26 307      |               0                |               0                |

 
[Lien vers le bulletins des CEV](Bases/Réglementation/lignes.contractuels.et.vacations.csv)   
[Lien vers la base de données Matricules des CEV](Bases/Réglementation/matricules.contractuels.et.vacations.csv)  
[Lien vers la base de données Cumul régime indemnitaire et vacations de CEV](Bases/Réglementation/RI.et.vacations.csv)  
[Lien vers la base de données Lignes de traitement indiciaire pour CEV](Bases/Réglementation/traitement.et.vacations.csv)  
 




## 5.4 Contrôle sur les indemnités IAT et IFTS      


```
## Error in `:=`(ifts.logical = grepl(expression.rég.ifts, Paie$Libellé, : Check that is.data.table(DT) == TRUE. Otherwise, := and `:=`(...) are defined for use in j, once only and in particular ways. See help(":=").
```

```
## Error in unique(Paie[ifts.logical == TRUE][, Code]): object 'ifts.logical' not found
```

Il n'a pas été possible d'identifier les IAT par expression régulière.


 
&nbsp;*Tableau 55*   
     

Tests IAT/IFTS sans résultat positif.

  
[Codes IFTS retenus](Bases/Réglementation/codes.ifts.csv)   
[Lien vers la base de données cumuls iat/ifts](Bases/Réglementation/personnels.iat.ifts.csv)    

### Contrôle sur les IFTS pour catégories B et contractuels



 


```
## Error in na.omit(Paie[as.integer(Indice) < 350 & ifts.logical == TRUE, : object 'Indice' not found
```

 


```
## Error in eval(expr, envir, enclos): object 'Statut' not found
```


 
&nbsp;*Tableau 56*   
   


```
## Error in Tableau(c("Nombre de lignes de paie de contractuels percevant des IFTS", : 'names' attribute [2] must be the same length as the vector [1]
```


[Lien vers la base de données Lignes IFTS pour contractuels](Bases/Réglementation/ifts.et.contractuel.csv)    
[Lien vers la base de données Lignes IFTS pour IB < 380](Bases/Réglementation/lignes.ifts.anormales.csv)     

**Nota :**
IB < 380 : fonctionnaire percevant un indice brut inférieur à 380





## 5.5 Contrôle de la prime de fonctions et de résultats (PFR)   
  


```
## Error in `:=`(pfr.logical, grepl(expression.rég.pfr, Paie$Libellé, ignore.case = TRUE, : Check that is.data.table(DT) == TRUE. Otherwise, := and `:=`(...) are defined for use in j, once only and in particular ways. See help(":=").
```

```
## Error in unique(Paie[pfr.logical == TRUE, Code]): object 'pfr.logical' not found
```

```
## Error in `:=`(cumul.pfr.ifts, (any(pfr.logical[Type == "I"]) & any(ifts.logical[Type == : Check that is.data.table(DT) == TRUE. Otherwise, := and `:=`(...) are defined for use in j, once only and in particular ways. See help(":=").
```


 
&nbsp;*Tableau 57*   
     


| Codes PFR | Agents cumulant PFR et IFTS |
|:---------:|:---------------------------:|
|           |              0              |

  
[Lien vers la base de données cumuls pfr/ifts](Bases/Réglementation/personnels.pfr.ifts.csv)    



```
## Error in Code %chin% union(unlist(codes.pfr), unlist(codes.ifts)): object 'Code' not found
```

```
## Error in eval(expr, envir, enclos): object 'P' not found
```

```
## Error in merge(P, P.any, by = c("Nom", "Matricule")): object 'P' not found
```

```
## Error in eval(expr, envir, enclos): object 'P' not found
```

```
## Error in eval(expr, envir, enclos): object 'P' not found
```

```
## Error in eval(expr, envir, enclos): object 'P' not found
```

 
&nbsp;*Tableau 58 : rappel des plafonds annuels de la PFR*   
     


| Adm. général | Adm. HC |  Adm.  | Direct./Attaché princ. | Secr. mairie/Attaché |
|:------------:|:-------:|:------:|:----------------------:|:--------------------:|
|    58 800    | 55 200  | 49 800 |         25 800         |        20 100        |


Les plafonds annuels de la PFR de sont pas dépassés.

 
&nbsp;*Tableau 59* : Valeurs de l'agrégat (PFR ou IFTS) pour les bénéficiaires de la PFR   
         


Aucun bénéficiaire de la PFR détecté.

 
&nbsp;*Tableau 60* : Variations de l'agrégat (PFR ou IFTS) pour les bénéficiaires de la PFR
         


Aucun tableau de variation.

        
  
[Lien vers la base de données agrégat PFR-IFTS](Bases/Rémunérations/bénéficiaires.PFR.csv)    

  
[Lien vers la base de données variations agrégat PFR-IFTS](Bases/Rémunérations/bénéficiaires.PFR.Variation.csv)    





## 5.6 Contrôle sur les heures supplémentaires

Le seuil de 180 heures supplémentaires maximum est dépassé par  59  agents.
 Le seuil de 220 heures supplémentaires maximum est dépassé par  59  agents.

```
## Error in eval(expr, envir, enclos): object 'Heures.Sup.' not found
```


 
&nbsp;*Tableau 61*   
   


| Nombre de lignes HS en excès | Nombre de lignes IHTS anormales |
|:----------------------------:|:-------------------------------:|
|             303              |               NA                |


[Lien vers la base de données Heures supplémentaires en excès du seuil de 25h/mois](Bases/Réglementation/HS.sup.25.csv)     
[Lien vers la base de données cumuls en excès des seuils annuels](Bases/Réglementation/Dépassement.seuil.180h.csv)    
[Lien vers la base de données IHTS anormales](Bases/Réglementation/ihts.anormales.csv)      

**Nota :**   
HS en excès : au-delà de 25 heures par mois     
IHTS anormales : non attribuées à des fonctionnaires de catégorie B ou C.     




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


```
## Error in Type %chin% c("T", "I", "R", "IR", "S", "A", "AC"): object 'Type' not found
```

```
## Error in sum(Total, na.rm = TRUE): invalid 'type' (character) of argument
```

 
Cumul des lignes de paie par exercice et catégorie de ligne de paie   
 


Tableau  62  Année  2013

|           Catégorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnités| 1 334 263,04|
|             Rappels|   244 938,75|
|       Rém. diverses|    27 860,93|
| Supplément familial|   146 331,98|
|         Traitements| 9 127 148,03|

Tableau  63  Année  2014

|           Catégorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnités| 1 334 245,36|
|             Rappels|   165 096,19|
|       Rém. diverses|    29 337,19|
| Supplément familial|   148 838,36|
|         Traitements| 9 315 116,31|

Tableau  64  Année  2015

|           Catégorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnités| 1 325 516,15|
|             Rappels|   156 348,33|
|       Rém. diverses|    17 323,77|
| Supplément familial|   140 162,84|
|         Traitements| 9 070 866,33|

Tableau  65  Année  2016

|           Catégorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnités| 1 532 916,11|
|             Rappels|   156 613,18|
|       Rém. diverses|    35 882,81|
| Supplément familial|   138 565,19|
|         Traitements| 8 861 048,17|

Tableau  66  Année  2017

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
 


```
## Error in eval(expr, envir, enclos): object 'NbEnfants' not found
```

```
## Error in eval(expr, envir, enclos): object 'Paie.sans.enfant' not found
```


Pour les agents n'ayant pas d'enfant signalé en base, il a été détecté 187 bulletins de paie présentant un paiement du SFT apparemment anormal.

 
[Lien vers la base des paiements de SFT à agents sans enfant signalé](Bases/Réglementation/Paie.sans.enfant.réduit.csv)
 


```
## Error in eval(expr, envir, enclos): object 'NbEnfants' not found
```

```
## Error in eval(expr, envir, enclos): object 'Paie.enfants' not found
```


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


