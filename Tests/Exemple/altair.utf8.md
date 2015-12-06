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




<p class = "centered"><b>Exercices 2008 à 2012 </b></p>
<p class = "author">Fabrice Nicol</h1>




dim. 06 déc. 2015
   


La durée du travail prise en compte dans la base de données est de  151,67  h par mois.

# 1. Statistiques de population

### 1.1 Effectifs



 



&nbsp;*Tableau 1*   


|                                                             |  2008   |  2009   |  2010   |  2011   |  2012   |
|:------------------------------------------------------------|:-------:|:-------:|:-------:|:-------:|:-------:|
|Total effectifs (a)                                          | 1 119,0 | 1 146,0 | 1 221,0 | 1 215,0 | 1 260,0 |
|&nbsp;&nbsp;&nbsp;dont présents 12 mois                      |  870,0  |  953,0  |  922,0  |  991,0  | 1 037,0 |
|&nbsp;&nbsp;&nbsp;dont fonctionnaires (b)                    |  704,0  |  706,0  |  733,0  |  735,0  |  744,0  |
|&nbsp;&nbsp;&nbsp;dont fonct. présents 12 mois               |  644,0  |  661,0  |  667,0  |  698,0  |  690,0  |
|&nbsp;&nbsp;&nbsp;dont non titulaires                        |  192,0  |  210,0  |  231,0  |  244,0  |  261,0  |
|&nbsp;&nbsp;&nbsp;dont élus                                  |  100,0  |  72,0   |  75,0   |  73,0   |  76,0   |
|&nbsp;&nbsp;&nbsp;dont élus présents 12 mois                 |  37,0   |  67,0   |  69,0   |  72,0   |  71,0   |
|&nbsp;&nbsp;&nbsp;dont vacataires détectés (c)               |   0,0   |   0,0   |   0,0   |   0,0   |   0,0   |
|&nbsp;&nbsp;&nbsp;dont assistantes maternelles détectées (c) |   0,0   |   0,0   |   0,0   |   0,0   |   0,0   |
|Postes non actifs (g)                                        |  58,0   |  89,0   |  118,0  |  66,0   |  92,0   |
|Postes annexes (g)                                           |  138,0  |  178,0  |  198,0  |  175,0  |  180,0  |
|Postes actifs non annexes (g)                                |  875,0  |  891,0  |  937,0  |  959,0  |  995,0  |
|Total ETP/année (d)                                          |  891,7  |  914,6  |  946,5  |  994,1  | 1 005,3 |
|Total ETPT/année (e)                                         |  819,8  |  846,6  |  860,6  |  908,8  |  915,5  |
|Total ETPT/année personnes en place (f)(g)                   |   0,0   |  621,1  |  627,5  |  632,6  |  662,5  |
|Total ETPT/année fonctionnaires (g)                          |  640,2  |  649,6  |  655,8  |  678,6  |  678,0  |
|Total ETPT/année titulaires à temps complet (g)              |  449,6  |  477,0  |  466,2  |  461,4  |  477,1  |
|Total ETPT non titulaires (g)                                |  141,8  |  158,3  |  172,3  |  181,4  |  202,8  |
|Total ETPT autre statut                                      |   0,7   |   0,1   |   4,7   |   6,3   |  10,1   |
|Total ETPT postes non actifs (g)                             |   0,5   |   0,1   |   0,2   |   0,1   |   0,0   |
|Total ETPT postes annexes (g)                                |  38,0   |  38,7   |  31,3   |  46,5   |  28,5   |
|Total ETPT postes actifs non annexes (g)                     |  782,3  |  807,9  |  829,1  |  862,2  |  887,0  |


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
   


|  Statistique  | Âge des personnels <br>au 31/12/2008 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  18                  |          |
| 1er quartile  |                  30                  |          |
|    Médiane    |                  38                  |          |
|    Moyenne    |                38,65                 |   941    |
| 3ème quartile |                  48                  |          |
|    Maximum    |                  64                  |          |

 

![plot of chunk unnamed-chunk-9](figure/unnamed-chunk-9-1.png) 

 
&nbsp;*Tableau 3*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2012 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  18                  |          |
| 1er quartile  |                  31                  |          |
|    Médiane    |                  40                  |          |
|    Moyenne    |                40,08                 |          |
| 3ème quartile |                  49                  |          |
|    Maximum    |                  66                  |          |

 
 

![plot of chunk unnamed-chunk-11](figure/unnamed-chunk-11-1.png) 


### 1.3 Pyramide des âges, personnels non titulaires
 
&nbsp;*Tableau 4*   
   


|  Statistique  | Âge des personnels non titulaires<br>au 31/12/2008 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         18                         |          |
| 1er quartile  |                         21                         |          |
|    Médiane    |                        26,5                        |          |
|    Moyenne    |                       31,72                        |   260    |
| 3ème quartile |                         40                         |          |
|    Maximum    |                         63                         |          |

 

![plot of chunk unnamed-chunk-13](figure/unnamed-chunk-13-1.png) 

 
&nbsp;*Tableau 5*   
   


|  Statistique  | Âge des personnels non titulaires<br>au 31/12/2012 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         18                         |          |
| 1er quartile  |                         23                         |          |
|    Médiane    |                         27                         |          |
|    Moyenne    |                       32,23                        |   324    |
| 3ème quartile |                       40,25                        |          |
|    Maximum    |                         62                         |          |

![plot of chunk unnamed-chunk-14](figure/unnamed-chunk-14-1.png) 

 
[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.nontit.csv)  
 


### 1.4 Pyramide des âges, personnels fonctionnaires stagiaires et titulaires
 
&nbsp;*Tableau 6*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2008 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  22                  |          |
| 1er quartile  |                  34                  |          |
|    Médiane    |                  41                  |          |
|    Moyenne    |                41,42                 |   674    |
| 3ème quartile |                  49                  |          |
|    Maximum    |                  64                  |          |

 


![plot of chunk unnamed-chunk-16](figure/unnamed-chunk-16-1.png) 

 
&nbsp;*Tableau 7*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2012 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  23                  |          |
| 1er quartile  |                  37                  |          |
|    Médiane    |                  43                  |          |
|    Moyenne    |                43,78                 |   723    |
| 3ème quartile |                  51                  |          |
|    Maximum    |                  66                  |          |



![plot of chunk unnamed-chunk-18](figure/unnamed-chunk-18-1.png) 

 
[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.fonctionnaires.csv)  
 
### 1.5 Pyramide des âges, personnels permanents (titulaires, stagiaires et non titulaires)
 
&nbsp;*Tableau 8*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2008 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  18                  |          |
| 1er quartile  |                  30                  |          |
|    Médiane    |                  38                  |          |
|    Moyenne    |                38,72                 |   934    |
| 3ème quartile |                  48                  |          |
|    Maximum    |                  64                  |          |

 

![plot of chunk unnamed-chunk-20](figure/unnamed-chunk-20-1.png) 

&nbsp;*Tableau 9*   
   


|  Statistique  | Âge des personnels <br>au 31/12/2012 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  18                  |          |
| 1er quartile  |                  32                  |          |
|    Médiane    |                  40                  |          |
|    Moyenne    |                 40,2                 |   1047   |
| 3ème quartile |                  49                  |          |
|    Maximum    |                  66                  |          |



![plot of chunk unnamed-chunk-22](figure/unnamed-chunk-22-1.png) 

 
[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.permanents.csv)  
 
### 1.6 Effectifs des personnels par durée de service

**Personnels en fonction (hors élus) des exercices 2008 à 2012 inclus :**

 
&nbsp;*Tableau 10*   
   


| Plus de 2 ans | Moins de 2 ans | Moins d'un an | Moins de six mois |
|:-------------:|:--------------:|:-------------:|:-----------------:|
|      945      |      268       |      137      |        57         |



![plot of chunk unnamed-chunk-24](figure/unnamed-chunk-24-1.png) 


**Effectifs (hors élus)**   




 
&nbsp;*Tableau 11*   
   


|                  | 2008 | 2009 | 2010 | 2011 | 2012 |
|:-----------------|:----:|:----:|:----:|:----:|:----:|
|Plus de deux ans  | 792  | 844  | 910  | 886  | 861  |
|Moins de deux ans |  83  |  47  |  27  |  73  | 134  |
|Total             | 875  | 891  | 937  | 959  | 995  |



**Nota :**
*Personnels en place : ayant servi au moins deux années consécutives pendant la période.*     
*Plus/moins de deux ans : plus/mois de 730 jours sur la période sous revue.*   



  
  



   
# 2. Rémunérations brutes : analyse pour l'exercice 2008    
   
## 2.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des rémunérations brutes pour l'exercice 2008
 
*Personnels (hors élus)*     
 
&nbsp;*Tableau 12*   
   


|           Agrégats           |   k&euro;    |
|------------------------------|--------------|
|   Brut annuel (bulletins)    | 23 318 144,6 |
| Brut annuel (lignes), dont : | 23 365 224,6 |
|         \ \ Primes :         | 6 347 169,0  |
|   \ \ Autres rémunérations   |    797,4     |
|     Part de primes en %      |     27,2     |

 
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
| Bulletins de paie | 23 318 144,6 |
|  Lignes de paie   | 23 365 224,6 |
|    Différence     |  -47 079,9   |


à comparer aux soldes des comptes 641 et 648 du compte de gestion.

  
## 2.2 Masse salariale brute des fonctionnaires

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*  


![plot of chunk unnamed-chunk-132](figure/unnamed-chunk-132-1.png) ![plot of chunk unnamed-chunk-132](figure/unnamed-chunk-132-2.png) ![plot of chunk unnamed-chunk-132](figure/unnamed-chunk-132-3.png) ![plot of chunk unnamed-chunk-132](figure/unnamed-chunk-132-4.png) ![plot of chunk unnamed-chunk-132](figure/unnamed-chunk-132-5.png) ![plot of chunk unnamed-chunk-132](figure/unnamed-chunk-132-6.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio rémunération / quotité*  
   
**Effectif : 693**

**Tests de cohérence**

       
       
       
Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :

 
&nbsp;*Tableau 14*   
   

```
## Error in `[.data.table`(masses.premier, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```


**Définitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
 *Indemnités d'élus*         : toutes rémunérations indemnitaires des élus    
 *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   

**Tests de cohérence**

Somme des rémunérations brutes versées aux personnels (non élus) :

 
&nbsp;*Tableau 15*   
   

```
## Error in `[.data.table`(masses.premier, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```


A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.

**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2008**     
 
&nbsp;*Tableau 16*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres rémunérations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 3 164|    763,5|                    0|         |
|  1er quartile|                16 221|  4 830,1|                    0|         |
|       Médiane|                17 915|  5 927,4|                    0|         |
|       Moyenne|                19 639|  7 270,6|                    0|      683|
| 3ème quartile|                22 428|  8 281,7|                    0|         |
|       Maximum|                80 378| 42 553,4|                    0|         |

 
&nbsp;*Tableau 17*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la rém. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     4 024|      4 063|                 0|                        9,158|         |
|  1er quartile|                    21 543|     21 360|            21 639|                       21,751|         |
|       Médiane|                    24 533|     24 382|            24 780|                       24,628|         |
|       Moyenne|                    26 909|     26 724|            27 559|                       26,210|      683|
| 3ème quartile|                    29 412|     29 256|            29 757|                       29,084|         |
|       Maximum|                   122 739|    119 918|           119 591|                       50,764|         |

  
*Hors vacataires identifiés, assistantes maternelles, élus locaux et pour les postes actifs non annexes*  

**Catégorie A**




 
&nbsp;*Tableau 18*   
   


|   Statistique| Traitement indiciaire| Primes| Autres rémunérations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                 6 227|  2 069|                    0|
|  1er quartile|                21 733|  8 236|                    0|
|       Médiane|                26 248| 11 276|                    0|
|       Moyenne|                27 776| 13 447|                    0|
| 3ème quartile|                31 878| 15 831|                    0|
|       Maximum|                80 378| 42 553|                    0|


 
&nbsp;*Tableau 19*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               8 322|                    2 406|                                21,89|
|  1er quartile|              31 465|                   32 031|                                25,67|
|       Médiane|              36 748|                   36 791|                                31,82|
|       Moyenne|              40 901|                   41 876|                                31,75|
| 3ème quartile|              45 234|                   50 424|                                36,43|
|       Maximum|             119 918|                  119 591|                                47,79|


**Effectif : 107**  

**Catégorie B**

 
&nbsp;*Tableau 20*   
   


|   Statistique| Traitement indiciaire| Primes| Autres rémunérations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                 3 821|  1 165|                    0|
|  1er quartile|                18 646|  5 899|                    0|
|       Médiane|                21 130|  6 982|                    0|
|       Moyenne|                21 400|  7 443|                    0|
| 3ème quartile|                24 827|  8 677|                    0|
|       Maximum|                32 596| 13 055|                    0|


 
&nbsp;*Tableau 21*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               4 986|                        0|                                16,65|
|  1er quartile|              24 730|                   25 011|                                23,52|
|       Médiane|              28 754|                   29 756|                                24,92|
|       Moyenne|              28 783|                   29 209|                                25,68|
| 3ème quartile|              33 568|                   34 283|                                26,67|
|       Maximum|              43 051|                   42 933|                                46,29|


**Effectif : 91**

**Catégorie C**

 
&nbsp;*Tableau 22*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres rémunérations|
|-------------:|---------------------:|--------:|--------------------:|
|       Minimum|                 3 164|    763,5|                    0|
|  1er quartile|                16 010|  4 511,2|                    0|
|       Médiane|                16 895|  5 402,6|                    0|
|       Moyenne|                17 513|  5 875,5|                    0|
| 3ème quartile|                19 374|  6 610,3|                    0|
|       Maximum|                26 268| 15 826,5|                    0|


 
&nbsp;*Tableau 23*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               4 063|                    1 745|                                9,158|
|  1er quartile|              20 845|                   21 265|                               21,412|
|       Médiane|              22 764|                   23 202|                               23,489|
|       Moyenne|              23 210|                   24 091|                               25,089|
| 3ème quartile|              25 949|                   26 646|                               27,582|
|       Maximum|              38 240|                   42 197|                               50,764|

**Effectif : 485**


######      <br>

## 2.3 Contractuels, vacataires et stagiaires inclus   
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   


![plot of chunk unnamed-chunk-145](figure/unnamed-chunk-145-1.png) 

  
**Nota :**
Ne sont retenues que les rémunérations supérieures à 1 000 k&euro;.
Les élus ne sont pas pris en compte.
  

![plot of chunk unnamed-chunk-146](figure/unnamed-chunk-146-1.png) 





  
**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2008**     
  
 
&nbsp;*Tableau 24*   
   


|   Statistique| Primes| Autres rémunérations| Effectif|
|-------------:|------:|--------------------:|--------:|
|       Minimum|      0|                0,000|         |
|  1er quartile|  2 775|                0,000|         |
|       Médiane|  5 223|                0,000|         |
|       Moyenne|  6 718|                4,153|      192|
| 3ème quartile|  7 731|                0,000|         |
|       Maximum| 43 361|              797,420|         |

 
&nbsp;*Tableau 25*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               3 768|                    2 182|         |
|  1er quartile|              12 035|                   21 651|         |
|       Médiane|              22 813|                   25 140|         |
|       Moyenne|              25 191|                   30 220|      192|
| 3ème quartile|              30 996|                   35 658|         |
|       Maximum|              97 650|                   97 383|         |






  
  



   
# 3. Rémunérations brutes : analyse pour l'exercice 2012    
   
## 3.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des rémunérations brutes pour l'exercice 2012
 
*Personnels (hors élus)*     
 
&nbsp;*Tableau 26*   
   


|           Agrégats           |   k&euro;    |
|------------------------------|--------------|
|   Brut annuel (bulletins)    | 27 545 129,5 |
| Brut annuel (lignes), dont : | 27 658 590,4 |
|         \ \ Primes :         | 7 673 002,4  |
|   \ \ Autres rémunérations   |   -3 271,1   |
|     Part de primes en %      |     27,9     |

 
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
| Bulletins de paie | 27 545 129,5 |
|  Lignes de paie   | 27 658 590,4 |
|    Différence     |  -113 460,9  |


à comparer aux soldes des comptes 641 et 648 du compte de gestion.

  
## 3.2 Masse salariale brute des fonctionnaires

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*  


![plot of chunk unnamed-chunk-155](figure/unnamed-chunk-155-1.png) ![plot of chunk unnamed-chunk-155](figure/unnamed-chunk-155-2.png) ![plot of chunk unnamed-chunk-155](figure/unnamed-chunk-155-3.png) ![plot of chunk unnamed-chunk-155](figure/unnamed-chunk-155-4.png) ![plot of chunk unnamed-chunk-155](figure/unnamed-chunk-155-5.png) ![plot of chunk unnamed-chunk-155](figure/unnamed-chunk-155-6.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio rémunération / quotité*  
   
**Effectif : 739**

**Tests de cohérence**

       
       
       
Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :

 
&nbsp;*Tableau 28*   
   

```
## Error in `[.data.table`(masses.premier, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```


**Définitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
 *Indemnités d'élus*         : toutes rémunérations indemnitaires des élus    
 *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   

**Tests de cohérence**

Somme des rémunérations brutes versées aux personnels (non élus) :

 
&nbsp;*Tableau 29*   
   

```
## Error in `[.data.table`(masses.premier, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```


A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.

**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2012**     
 
&nbsp;*Tableau 30*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres rémunérations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 3 342|    874,3|              -329,58|         |
|  1er quartile|                17 280|  4 941,4|               -54,77|         |
|       Médiane|                19 189|  6 271,7|                 0,00|         |
|       Moyenne|                20 978|  7 695,3|               -22,90|      728|
| 3ème quartile|                23 905|  8 747,9|                 0,00|         |
|       Maximum|                54 217| 42 719,1|                69,35|         |

 
&nbsp;*Tableau 31*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la rém. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     4 217|      4 234|                 0|                        11,88|         |
|  1er quartile|                    22 677|     22 667|            23 251|                        21,66|         |
|       Médiane|                    25 992|     26 017|            26 944|                        24,32|         |
|       Moyenne|                    28 673|     28 547|            30 320|                        26,04|      728|
| 3ème quartile|                    31 744|     31 592|            32 636|                        28,95|         |
|       Maximum|                    94 940|    108 055|           348 542|                        64,86|         |

  
*Hors vacataires identifiés, assistantes maternelles, élus locaux et pour les postes actifs non annexes*  

**Catégorie A**




 
&nbsp;*Tableau 32*   
   


|   Statistique| Traitement indiciaire| Primes| Autres rémunérations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                 5 350|  2 256|              -194,76|
|  1er quartile|                23 399|  8 374|                 0,00|
|       Médiane|                27 654| 11 621|                 0,00|
|       Moyenne|                29 030| 13 603|               -15,57|
| 3ème quartile|                35 120| 16 412|                 0,00|
|       Maximum|                54 217| 42 719|                 0,06|


 
&nbsp;*Tableau 33*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               8 085|                        0|                                21,90|
|  1er quartile|              32 860|                   35 631|                                25,60|
|       Médiane|              39 172|                   42 085|                                30,16|
|       Moyenne|              42 318|                   44 656|                                31,25|
| 3ème quartile|              48 565|                   50 278|                                37,24|
|       Maximum|             108 055|                  171 722|                                47,61|


**Effectif : 142**  

**Catégorie B**

 
&nbsp;*Tableau 34*   
   


|   Statistique| Traitement indiciaire| Primes| Autres rémunérations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                 4 687|  1 606|              -177,42|
|  1er quartile|                19 725|  6 126|                 0,00|
|       Médiane|                22 416|  7 085|                 0,00|
|       Moyenne|                22 608|  7 589|               -15,22|
| 3ème quartile|                26 323|  8 569|                 0,00|
|       Maximum|                34 213| 14 177|                 0,01|


 
&nbsp;*Tableau 35*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               6 293|                        0|                                16,54|
|  1er quartile|              26 002|                   26 896|                                23,00|
|       Médiane|              30 101|                   31 012|                                24,38|
|       Moyenne|              30 111|                   30 932|                                25,07|
| 3ème quartile|              34 538|                   35 290|                                27,09|
|       Maximum|              47 706|                   47 576|                                35,06|


**Effectif : 101**

**Catégorie C**

 
&nbsp;*Tableau 36*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres rémunérations|
|-------------:|---------------------:|--------:|--------------------:|
|       Minimum|                 3 342|    874,3|              -329,58|
|  1er quartile|                17 180|  4 670,2|               -56,24|
|       Médiane|                18 134|  5 525,1|                 0,00|
|       Moyenne|                18 281|  5 988,0|               -26,64|
| 3ème quartile|                20 100|  6 817,5|                 0,00|
|       Maximum|                27 254| 13 199,8|                69,35|


 
&nbsp;*Tableau 37*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Part de la rémunération indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               4 234|                        0|                                11,88|
|  1er quartile|              22 042|                   22 603|                                20,56|
|       Médiane|              23 859|                   24 293|                                23,31|
|       Moyenne|              24 190|                   25 996|                                24,72|
| 3ème quartile|              27 564|                   27 963|                                27,42|
|       Maximum|              46 796|                  348 542|                                64,86|

**Effectif : 485**


######      <br>

## 3.3 Contractuels, vacataires et stagiaires inclus   
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   


![plot of chunk unnamed-chunk-168](figure/unnamed-chunk-168-1.png) 

  
**Nota :**
Ne sont retenues que les rémunérations supérieures à 1 000 k&euro;.
Les élus ne sont pas pris en compte.
  

![plot of chunk unnamed-chunk-169](figure/unnamed-chunk-169-1.png) 





  
**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année 2012**     
  
 
&nbsp;*Tableau 38*   
   


|   Statistique| Primes| Autres rémunérations| Effectif|
|-------------:|------:|--------------------:|--------:|
|       Minimum|      0|               -162,9|         |
|  1er quartile|  3 187|                  0,0|         |
|       Médiane|  5 700|                  0,0|         |
|       Moyenne|  6 794|                 -2,2|      267|
| 3ème quartile|  7 259|                  0,0|         |
|       Maximum| 60 095|                678,1|         |

 
&nbsp;*Tableau 39*   
   


|   Statistique| Total rémunérations| Total rémunérations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|              -2 780|                   -5 305|         |
|  1er quartile|              12 933|                   21 798|         |
|       Médiane|              21 721|                   24 335|         |
|       Moyenne|              23 850|                   28 644|      267|
| 3ème quartile|              26 482|                   33 345|         |
|       Maximum|             130 660|                  130 303|         |

 
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
  




# 4. Rémunérations nettes : évolutions sur la période 2008 - 2012    

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

## 4.2 Evolutions des rémunérations nettes sur la période 2008 - 2012   

### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors élus)





   
**Salaire net moyen par tête (SMPT net) en EQTP, hors élus**         
      
 
&nbsp;*Tableau 42*   
   


| Année | Rém. nette totale (k&euro;) | Variation (%) | SMPT net (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-----------------:|:-------------:|
| 2008  |           20 245            |               |      23 164       |               |
| 2009  |           20 791            |               |      23 762       |               |
| 2010  |           22 255            |      19       |      24 164       |       6       |
| 2011  |           23 492            |               |      24 833       |               |
| 2012  |           24 087            |               |      24 554       |               |

  
**Effet de noria sur salaires nets et taux de remplacements**       
  
**Effet de noria** : *différence entre la rémunération annuelle des entrants à l'année N et des sortants à l'année N-1*.  
*Usuellement calculée sur les rémunérations brutes, ici sur les rémunérations nettes EQTP*  
*afin d'apprécier l'impact de cet effet sur l'évolution des rémunérations nette moyennes calculée au tableau précédent.*               
 
&nbsp;*Tableau 43*   
   


| Année | Noria EQTP (&euro;) | En % de la MS N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-----------------:|:------------------:|:-------------------------:|
| 2009  |       -944,73       |      -0,176       |         38         |            4,5            |
| 2010  |       2 108,6       |       0,364       |         36         |            4,2            |
| 2011  |      -1 003,2       |      -0,181       |         40         |            4,4            |
| 2012  |      -9 553,9       |       -1,36       |         33         |            3,7            |


*MS N-1 : masse salariale nette de l'année n-1.*   
      
**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
      
 
&nbsp;*Tableau 44*   
   


|   Statistique| Première année|
|-------------:|--------------:|
|       Minimum|          1 368|
|  1er quartile|         18 040|
|       Médiane|         20 460|
|       Moyenne|         23 160|
| 3ème quartile|         24 880|
|       Maximum|         97 670|



|   Statistique| Dernière année|
|-------------:|--------------:|
|       Minimum|          2 868|
|  1er quartile|         18 730|
|       Médiane|         21 260|
|       Moyenne|         24 550|
| 3ème quartile|         26 400|
|       Maximum|        285 900|

 
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
| 2008  |           15 533            |               |          22 776           |               |
| 2009  |           15 758            |               |          23 555           |               |
| 2010  |           16 721            |     15,1      |          24 163           |     8,84      |
| 2011  |           17 154            |               |          24 332           |               |
| 2012  |           17 874            |               |          24 790           |               |

  
**Catégorie A**  

&nbsp;*Tableau 49*   
   


| Année | Rém. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2008  |           3 710,1           |               |          34 674           |               |
| 2009  |           3 740,5           |               |          37 034           |               |
| 2010  |           4 467,5           |     40,8      |          37 860           |     9,17      |
| 2011  |           4 729,4           |               |          37 239           |               |
| 2012  |           5 223,6           |               |          37 852           |               |

  
**Catégorie B**    
    
&nbsp;*Tableau 50*   
   


| Année | Rém. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2008  |           2 187,3           |               |          24 303           |               |
| 2009  |           2 309,5           |               |          25 103           |               |
| 2010  |           2 365,3           |     15,3      |          25 709           |     4,84      |
| 2011  |           2 367,9           |               |          25 738           |               |
| 2012  |           2 522,4           |               |          25 479           |               |

  
**Catégorie C**     
   
&nbsp;*Tableau 51*   
   


| Année | Rém. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2008  |           9 635,7           |               |          19 867           |               |
| 2009  |           9 708,5           |               |          20 396           |               |
| 2010  |           9 888,1           |     5,11      |          20 515           |     5,33      |
| 2011  |           10 056            |               |          20 692           |               |
| 2012  |           10 128            |               |          20 925           |               |

   



  
**Effet de noria sur salaires nets et taux de remplacements**       
 
&nbsp;*Tableau 52*   
   


| Année | Noria EQTP (&euro;) | En % de la  MSN N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-------------------:|:------------------:|:-------------------------:|
| 2009  |      -7 088,8       |       -0,785        |         11         |            1,6            |
| 2010  |       10 702        |        1,89         |         17         |            2,6            |
| 2011  |      -8 228,9       |        -1,55        |         19         |            2,7            |
| 2012  |       7 910,8       |        0,949        |         12         |            1,8            |

    
*MS N-1 : masse salariale nette de l'année n-1.*   
      
**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
 
&nbsp;*Tableau 53*   
   


|   Statistique| Première année|
|-------------:|--------------:|
|       Minimum|          1 368|
|  1er quartile|         18 070|
|       Médiane|         20 460|
|       Moyenne|         22 780|
| 3ème quartile|         24 580|
|       Maximum|         97 670|



|   Statistique| Dernière année|
|-------------:|--------------:|
|       Minimum|          3 506|
|  1er quartile|         18 870|
|       Médiane|         21 760|
|       Moyenne|         24 790|
| 3ème quartile|         26 320|
|       Maximum|        285 900|

   



  
**Evolution du SMPT net des titulaires à temps complet**     
  
 
&nbsp;*Tableau 54*   
   


| Année | Rémunération nette totale (k&euro;) | Variation (%) | SMPT (&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:-------------:|:-------------:|
| 2008  |               10 641                |               |    23 183     |               |
| 2009  |               11 632                |               |    24 233     |               |
| 2010  |               11 517                |     12,7      |    24 504     |     7,36      |
| 2011  |               11 548                |               |    24 781     |               |
| 2012  |               11 997                |               |    24 890     |               |

   
**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) des titulaires à temps complet**         
      
 
&nbsp;*Tableau 55*   
   


|   Statistique| Première année|
|-------------:|--------------:|
|       Minimum|         15 260|
|  1er quartile|         18 100|
|       Médiane|         20 460|
|       Moyenne|         23 180|
| 3ème quartile|         24 630|
|       Maximum|         97 670|

  


|   Statistique| Dernière année|
|-------------:|--------------:|
|       Minimum|          9 460|
|  1er quartile|         19 110|
|       Médiane|         21 830|
|       Moyenne|         24 890|
| 3ème quartile|         27 070|
|       Maximum|         76 170|


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
| 2009  |               15 538                |               |        24 742        |               |
| 2010  |               15 937                |     9,86      |        24 979        |     1,31      |
| 2011  |               16 218                |               |        25 144        |               |
| 2012  |               17 070                |               |        25 067        |               |

   
**Distribution et variation sur la période de la rémunération nette des personnes en place**                
 
&nbsp;*Tableau 57*   
   



 


|   Statistique| Première année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          4 617|         |
|  1er quartile|         18 170|         |
|       Médiane|         20 590|         |
|       Moyenne|         23 450|      786|
| 3ème quartile|         24 830|         |
|       Maximum|         97 670|         |

 
&nbsp;*Tableau 58*   
   


|   Statistique| Dernière année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          8 850|         |
|  1er quartile|         18 990|         |
|       Médiane|         21 610|         |
|       Moyenne|         24 720|      877|
| 3ème quartile|         26 490|         |
|       Maximum|        142 500|         |


*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
 
&nbsp;*Tableau 59*   
   


|   Statistique| Variation normalisée (%)| Variation annuelle moyenne normalisée (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -39,818|                                 -11,9 220|         |
|  1er quartile|                    1,502|                                   0,3 734|         |
|       Médiane|                    6,118|                                   1,4 956|         |
|       Moyenne|                    7,426|                                   1,6 669|      682|
| 3ème quartile|                   11,473|                                   2,7 524|         |
|       Maximum|                  101,377|                                  19,1 249|         |

   
### 4.3.2 Titulaires et stagiaires     
  



  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 60*   
   


| Année | Rémunération nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2009  |               12 932                |               |        23 948        |               |
| 2010  |               13 199                |     9,11      |        24 263        |     2,11      |
| 2011  |               13 562                |               |        24 570        |               |
| 2012  |               14 110                |               |        24 454        |               |

   
**Distribution et variation sur la période de la rémunération nette des fonctionnaires en place**                
 
&nbsp;*Tableau 61*   
   
 


|   Statistique| Première année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|         10 590|         |
|  1er quartile|         18 110|         |
|       Médiane|         20 430|         |
|       Moyenne|         22 760|      677|
| 3ème quartile|         24 320|         |
|       Maximum|         97 670|         |

 
&nbsp;*Tableau 62*   
   


|   Statistique| Dernière année| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          9 570|         |
|  1er quartile|         18 790|         |
|       Médiane|         21 740|         |
|       Moyenne|         24 650|      792|
| 3ème quartile|         26 320|         |
|       Maximum|        142 500|         |


*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
 
&nbsp;*Tableau 63*   
   


|   Statistique| Variation normalisée (%)| Variation annuelle moyenne normalisée (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -39,818|                                 -11,9 220|         |
|  1er quartile|                    1,648|                                   0,4 094|         |
|       Médiane|                    6,590|                                   1,6 083|         |
|       Moyenne|                    7,404|                                   1,6 695|      611|
| 3ème quartile|                   11,474|                                   2,7 527|         |
|       Maximum|                  101,377|                                  19,1 249|         |



[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)

**Nota**   
*Personnes en place :* en fonction au moins deux années consécutives sur la période 2008 à 2012    
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

Primes informatiques potentielles : aucune
 
&nbsp;*Tableau 66*   
   


| Nombre de lignes NBI pour non titulaires | Nombre de bénéficiaires de PFI |
|:----------------------------------------:|:------------------------------:|
|                    12                    |               0                |

  
[Lien vers la base de données NBI aux non titulaires](Bases/Réglementation/NBI.aux.non.titulaires.csv)   
[Lien vers la base de données Primes informatiques](Bases/Réglementation/personnels.prime.informatique.csv)   
  
**Nota :**   
NBI: nouvelle bonification indiciaire   
PFI: prime de fonctions informatiques   




 
&nbsp;*Tableau 67*   
   


| Rémunérations de NBI anormales, par agent et par exercice | Montants correspondants |
|:---------------------------------------------------------:|:-----------------------:|
|                            71                             |         33 543          |

  
[Lien vers la base de données NBI anormales](Bases/Fiabilité/lignes.nbi.anormales.csv)   
  
**Nota :**   
*Est considéré comme manifestement anormal un total annuel de rémunérations NBI correspondant à un point d'indice net mensuel inférieur à 4 euros ou supérieur à 6 euros.*    
*Les rappels ne sont pas pris en compte dans les montants versés. Certains écarts peuvent être régularisés en les prenant en compte*     
 
&nbsp;*Tableau 68*   
   


| Année | Cumuls des NBI | Montants versés (a) | Point d'INM apparent | Point d'INM moyen | Contrôle |
|-------|----------------|---------------------|----------------------|-------------------|----------|
| 2008  |    25 255,0    |      113 463,9      |         4,5          |        4,6        |   Vert   |
| 2009  |    28 280,0    |      127 491,1      |         4,5          |        4,6        |   Vert   |
| 2010  |    29 740,0    |      134 382,9      |         4,5          |        4,6        |   Vert   |
| 2011  |    28 820,0    |      130 786,8      |         4,5          |        4,6        |   Vert   |
| 2012  |    29 730,0    |      134 716,0      |         4,5          |        4,6        |   Vert   |


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
|       0       |        0         |               0                |               0                |

 
[Lien vers le bulletins des CEV](Bases/Réglementation/lignes.contractuels.et.vacations.csv)   
[Lien vers la base de données Matricules des CEV](Bases/Réglementation/matricules.contractuels.et.vacations.csv)  
[Lien vers la base de données Cumul régime indemnitaire et vacations de CEV](Bases/Réglementation/RI.et.vacations.csv)  
[Lien vers la base de données Lignes de traitement indiciaire pour CEV](Bases/Réglementation/traitement.et.vacations.csv)  
 




## 5.4 Contrôle sur les indemnités IAT et IFTS      




 
&nbsp;*Tableau 70*   
     


|             Codes IFTS             |   |
|:----------------------------------:|:-:|
| 502N 502A 202N 202R 202A 502R 502V |   |



| Nombre de personnels percevant IAT et IFTS |   |
|:------------------------------------------:|:-:|
|                     11                     |   |

  
[Codes IFTS retenus](Bases/Réglementation/codes.ifts.csv)   
[Lien vers la base de données cumuls iat/ifts](Bases/Réglementation/personnels.iat.ifts.csv)    

### Contrôle sur les IFTS pour catégories B et contractuels



 



 




 
&nbsp;*Tableau 71*   
   


| Nombre de lignes de paie de contractuels percevant des IFTS | Nombre de lignes IFTS pour IB < 380 |
|:-----------------------------------------------------------:|:-----------------------------------:|
|                            2 448                            |                  3                  |


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

Le seuil de 180 heures supplémentaires maximum est dépassé par  50  agents.
 Le seuil de 220 heures supplémentaires maximum est dépassé par  50  agents.


 
&nbsp;*Tableau 76*   
   


| Nombre de lignes HS en excès | Nombre de lignes IHTS anormales |
|:----------------------------:|:-------------------------------:|
|             388              |               75                |


[Lien vers la base de données Heures supplémentaires en excès du seuil de 25h/mois](Bases/Réglementation/HS.sup.25.csv)     
[Lien vers la base de données cumuls en excès des seuils annuels](Bases/Réglementation/Dépassement.seuil.180h.csv)    
[Lien vers la base de données IHTS anormales](Bases/Réglementation/ihts.anormales.csv)      

**Nota :**   
HS en excès : au-delà de 25 heures par mois     
IHTS anormales : attribuées à des fonctionnaires ou non-titulaires de catégorie A ou assimilés.     




## 5.7 Contrôle sur les indemnités des élus
  



  


|Matricule |Nom                   | Année|Emploi                       | Indemnités | Autres |    Total |
|:---------|:---------------------|-----:|:----------------------------|-----------:|-------:|---------:|
|1002 P    |AMPHOUX               |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|1002 P    |AMPHOUX               |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1002 P    |AMPHOUX               |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1002 P    |AMPHOUX               |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1002 P    |AMPHOUX               |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|108 T     |BAFFERT               |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|108 T     |BAFFERT               |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|108 T     |BAFFERT               |  2010|CONSEILLER COMMUNAUT DELEGUE |    16841,07|       0|  16841,07|
|108 T     |BAFFERT               |  2011|12EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|108 T     |BAFFERT               |  2012|12EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1163 P    |QUEIROS               |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1163 P    |QUEIROS               |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1163 P    |QUEIROS               |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1163 P    |QUEIROS               |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1212 T    |HANFF                 |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|1223 E    |MEUNIER RIVIERE       |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|1319 J    |SIMARD                |  2008|CONSEILLER COMMUNAUT DELEGUE |     7490,91|       0|   7490,91|
|1319 J    |SIMARD                |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1319 J    |SIMARD                |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1319 J    |SIMARD                |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1319 J    |SIMARD                |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1320 K    |JOIRE                 |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1320 K    |JOIRE                 |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1320 K    |JOIRE                 |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1320 K    |JOIRE                 |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1320 K    |JOIRE                 |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1321 L    |JOLY                  |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1321 L    |JOLY                  |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1322 M    |LETEXIER              |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1322 M    |LETEXIER              |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1322 M    |LETEXIER              |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1322 M    |LETEXIER              |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1322 M    |LETEXIER              |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1323 N    |MOUREY                |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1323 N    |MOUREY                |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1323 N    |MOUREY                |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1323 N    |MOUREY                |  2011|CONSEILLER COMMUNAUTAIRE     |     7070,76|       0|   7070,76|
|1323 N    |MOUREY                |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1324 P    |PERRIER               |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1324 P    |PERRIER               |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1324 P    |PERRIER               |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1324 P    |PERRIER               |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1324 P    |PERRIER               |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1325 R    |BARACETTI             |  2008|CONSEILLER COMMUNAUT DELEGUE |     7490,91|       0|   7490,91|
|1325 R    |BARACETTI             |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1325 R    |BARACETTI             |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1325 R    |BARACETTI             |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1325 R    |BARACETTI             |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1326 S    |TASCA                 |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1326 S    |TASCA                 |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1326 S    |TASCA                 |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1326 S    |TASCA                 |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1326 S    |TASCA                 |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1327 T    |YTOURNEL              |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1327 T    |YTOURNEL              |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1327 T    |YTOURNEL              |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1327 T    |YTOURNEL              |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1327 T    |YTOURNEL              |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1328 U    |BACHIR CHERIF         |  2008|CONSEILLER COMMUNAUT DELEGUE |    11686,51|       0|  11686,51|
|1328 U    |BACHIR CHERIF         |  2009|CONSEILLER COMMUNAUT DELEGUE |    18522,30|       0|  18522,30|
|1328 U    |BACHIR CHERIF         |  2010|11EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|1328 U    |BACHIR CHERIF         |  2011|11EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1328 U    |BACHIR CHERIF         |  2012|11EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1329 V    |ECHEVET               |  2008|CONSEILLER COMMUNAUT DELEGUE |     7490,91|       0|   7490,91|
|1329 V    |ECHEVET               |  2009|CONSEILLER COMMUNAUT DELEGUE |     3752,48|       0|   3752,48|
|1330 W    |BELLET                |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1330 W    |BELLET                |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1330 W    |BELLET                |  2010|CONSEILLER COMMUNAUTAIRE     |     9219,99|       0|   9219,99|
|1330 W    |BELLET                |  2011|CONSEILLER COMMUNAUTAIRE     |    11404,44|       0|  11404,44|
|1330 W    |BELLET                |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1331 X    |BLANC TAILLEUR        |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1331 X    |BLANC TAILLEUR        |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1331 X    |BLANC TAILLEUR        |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1331 X    |BLANC TAILLEUR        |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1331 X    |BLANC TAILLEUR        |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1332 Y    |CUSTOT                |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1332 Y    |CUSTOT                |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1332 Y    |CUSTOT                |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1332 Y    |CUSTOT                |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1332 Y    |CUSTOT                |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1333 Z    |DUPONT FERRIER        |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1333 Z    |DUPONT FERRIER        |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1333 Z    |DUPONT FERRIER        |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1333 Z    |DUPONT FERRIER        |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1333 Z    |DUPONT FERRIER        |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1334 A    |BOISSET               |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1334 A    |BOISSET               |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1334 A    |BOISSET               |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1334 A    |BOISSET               |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1334 A    |BOISSET               |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1335 B    |FILIPPI               |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1335 B    |FILIPPI               |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1335 B    |FILIPPI               |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1335 B    |FILIPPI               |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1335 B    |FILIPPI               |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1336 C    |GADUEL                |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1336 C    |GADUEL                |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1336 C    |GADUEL                |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1336 C    |GADUEL                |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1336 C    |GADUEL                |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1337 D    |GRASSET               |  2008|CONSEILLER COMMUNAUT DELEGUE |     7490,91|       0|   7490,91|
|1337 D    |GRASSET               |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1337 D    |GRASSET               |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1337 D    |GRASSET               |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1337 D    |GRASSET               |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1338 E    |BERTRAND POUGNAND     |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1338 E    |BERTRAND POUGNAND     |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1338 E    |BERTRAND POUGNAND     |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1338 E    |BERTRAND POUGNAND     |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1338 E    |BERTRAND POUGNAND     |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1339 F    |FERRARI               |  2008|11EME VICE PRESIDENT         |    12285,11|       0|  12285,11|
|1339 F    |FERRARI               |  2009|11EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|1339 F    |FERRARI               |  2010|10EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|1339 F    |FERRARI               |  2011|10EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1339 F    |FERRARI               |  2012|10EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1340 G    |SEMANAZ               |  2008|CONSEILLER COMMUNAUTAIRE     |     7490,91|       0|   7490,91|
|1340 G    |SEMANAZ               |  2009|CONSEILLER COMMUNAUTAIRE     |    11294,07|       0|  11294,07|
|1340 G    |SEMANAZ               |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1340 G    |SEMANAZ               |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1340 G    |SEMANAZ               |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1341 H    |FILLOL                |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1341 H    |FILLOL                |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1341 H    |FILLOL                |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1341 H    |FILLOL                |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1341 H    |FILLOL                |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1342 J    |MAGNIN                |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1342 J    |MAGNIN                |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1342 J    |MAGNIN                |  2010|CONSEILLER COMMUNAUTAIRE     |      907,80|       0|    907,80|
|1349 S    |VOIR                  |  2008|CONSEILLER COMMUNAUTAIRE     |     1573,37|       0|   1573,37|
|1349 S    |VOIR                  |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1349 S    |VOIR                  |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1349 S    |VOIR                  |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1349 S    |VOIR                  |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1350 T    |GRASSET               |  2008|14EME VICE PRESIDENT         |    10751,20|       0|  10751,20|
|1350 T    |GRASSET               |  2009|14EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|1350 T    |GRASSET               |  2010|13EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|1350 T    |GRASSET               |  2011|13EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1350 T    |GRASSET               |  2012|13EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1351 U    |FALCON DE LONGEVIALLE |  2008|CONSEILLER COMMUNAUT DELEGUE |     6555,60|       0|   6555,60|
|1351 U    |FALCON DE LONGEVIALLE |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1351 U    |FALCON DE LONGEVIALLE |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1351 U    |FALCON DE LONGEVIALLE |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1351 U    |FALCON DE LONGEVIALLE |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1352 V    |BOMBINO               |  2008|CONSEILLER COMMUNAUTAIRE     |     1573,37|       0|   1573,37|
|1352 V    |BOMBINO               |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1352 V    |BOMBINO               |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1352 V    |BOMBINO               |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1352 V    |BOMBINO               |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1353 W    |NOBLECOURT            |  2008|CONSEILLER COMMUNAUT DELEGUE |     6555,60|       0|   6555,60|
|1353 W    |NOBLECOURT            |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1353 W    |NOBLECOURT            |  2010|CONSEILLER COMMUNAUT DELEGUE |    13200,75|       0|  13200,75|
|1353 W    |NOBLECOURT            |  2011|19EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1353 W    |NOBLECOURT            |  2012|19EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1362 F    |BLACHE                |  2008|CONSEILLER COMMUNAUT DELEGUE |     5620,29|       0|   5620,29|
|1362 F    |BLACHE                |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1362 F    |BLACHE                |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1362 F    |BLACHE                |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1362 F    |BLACHE                |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1476 E    |TOSCANO               |  2009|CONSEILLER COMMUNAUT DELEGUE |     7541,59|       0|   7541,59|
|1476 E    |TOSCANO               |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1476 E    |TOSCANO               |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1476 E    |TOSCANO               |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1532 R    |KUNTZ                 |  2009|CONSEILLER COMMUNAUTAIRE     |      907,12|       0|    907,12|
|1532 R    |KUNTZ                 |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1532 R    |KUNTZ                 |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1532 R    |KUNTZ                 |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1535 U    |VUAILLAT              |  2009|CONSEILLER COMMUNAUTAIRE     |      907,12|       0|    907,12|
|1535 U    |VUAILLAT              |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1535 U    |VUAILLAT              |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1535 U    |VUAILLAT              |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1536 V    |CHAMUSSY              |  2009|CONSEILLER COMMUNAUTAIRE     |      907,12|       0|    907,12|
|1536 V    |CHAMUSSY              |  2010|CONSEILLER COMMUNAUTAIRE     |     9219,99|       0|   9219,99|
|1536 V    |CHAMUSSY              |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1536 V    |CHAMUSSY              |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1570 G    |CRISTINI              |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1570 G    |CRISTINI              |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1570 G    |CRISTINI              |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1577 P    |ODDON                 |  2010|CONSEILLER COMMUNAUTAIRE     |     2503,29|       0|   2503,29|
|1577 P    |ODDON                 |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1577 P    |ODDON                 |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1580 T    |GERBIER               |  2010|CONSEILLER COMMUNAUTAIRE     |     2503,29|       0|   2503,29|
|1580 T    |GERBIER               |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1580 T    |GERBIER               |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1596 K    |HUGELE                |  2010|CONSEILLER COMMUNAUTAIRE     |     2049,39|       0|   2049,39|
|1596 K    |HUGELE                |  2011|CONSEILLER COMMUNAUTAIRE     |     7793,04|       0|   7793,04|
|1596 K    |HUGELE                |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1614 E    |MOREAU                |  2010|CONSEILLER COMMUNAUTAIRE     |     1595,49|       0|   1595,49|
|1614 E    |MOREAU                |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1614 E    |MOREAU                |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|163 C     |ROUX                  |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|163 C     |ROUX                  |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|163 C     |ROUX                  |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|163 C     |ROUX                  |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|163 C     |ROUX                  |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|166 F     |PROBY                 |  2008|9EME VICE PRESIDENT          |    16818,66|       0|  16818,66|
|166 F     |PROBY                 |  2009|10EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|166 F     |PROBY                 |  2010|9EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|166 F     |PROBY                 |  2011|9EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|166 F     |PROBY                 |  2012|9EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|179 V     |BLONDE                |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|1867 E    |GAUTHIER              |  2012|CONSEILLER COMMUNAUTAIRE     |     2508,99|       0|   2508,99|
|1868 F    |PUISSAT               |  2012|CONSEILLER COMMUNAUTAIRE     |     3736,87|       0|   3736,87|
|1978 A    |SAFAR                 |  2012|CONSEILLER COMMUNAUTAIRE     |     2946,14|       0|   2946,14|
|1988 L    |VERRI                 |  2012|CONSEILLER COMMUNAUT DELEGUE |      950,37|       0|    950,37|
|232 C     |SULLI                 |  2008|2EME PREMIER VICE PRESIDENT  |    16818,66|       0|  16818,66|
|232 C     |SULLI                 |  2009|2EME PREMIER VICE PRESIDENT  |    18522,30|       0|  18522,30|
|232 C     |SULLI                 |  2010|2EME PREMIER VICE PRESIDENT  |    18656,70|       0|  18656,70|
|232 C     |SULLI                 |  2011|2EME PREMIER VICE PRESIDENT  |    18703,20|       0|  18703,20|
|232 C     |SULLI                 |  2012|2EME PREMIER VICE PRESIDENT  |    18703,20|       0|  18703,20|
|251 Y     |ALBOUY                |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|252 Z     |ARIAS                 |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|252 Z     |ARIAS                 |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|252 Z     |ARIAS                 |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|252 Z     |ARIAS                 |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|252 Z     |ARIAS                 |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|255 C     |BAUDEAU               |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|256 D     |BETTO                 |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|257 E     |DESSARTS              |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|257 E     |DESSARTS              |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|257 E     |DESSARTS              |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|257 E     |DESSARTS              |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|257 E     |DESSARTS              |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|258 F     |BONZI                 |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|259 G     |BOUHET                |  2008|CONSEILLER COMMUNAUTAIRE     |     7994,50|       0|   7994,50|
|259 G     |BOUHET                |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|259 G     |BOUHET                |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|259 G     |BOUHET                |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|259 G     |BOUHET                |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|260 H     |CAUNE                 |  2008|10EME VICE PRESIDENT         |     4533,55|       0|   4533,55|
|261 J     |CHIRON                |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|261 J     |CHIRON                |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|261 J     |CHIRON                |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|261 J     |CHIRON                |  2011|CONSEILLER COMMUNAUTAIRE     |     2052,81|       0|   2052,81|
|263 L     |CHUMIATCHER           |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|264 M     |COEUR                 |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|264 M     |COEUR                 |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|264 M     |COEUR                 |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|264 M     |COEUR                 |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|264 M     |COEUR                 |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|265 N     |CONTRERAS             |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|265 N     |CONTRERAS             |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|265 N     |CONTRERAS             |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|265 N     |CONTRERAS             |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|265 N     |CONTRERAS             |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|266 P     |DESTOT                |  2008|17EME VICE PRESIDENT         |    27012,82|       0|  36351,61|
|266 P     |DESTOT                |  2009|17EME VICE PRESIDENT         |    27871,87|       0|  37221,44|
|266 P     |DESTOT                |  2010|18EME VICE PRESIDENT         |    28067,29|       0|  37477,88|
|266 P     |DESTOT                |  2011|18EME VICE PRESIDENT         |    28124,20|       0|  37545,20|
|266 P     |DESTOT                |  2012|18EME VICE PRESIDENT         |    28179,92|       0|  37656,64|
|267 R     |DJELLAL               |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|267 R     |DJELLAL               |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|267 R     |DJELLAL               |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|267 R     |DJELLAL               |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|267 R     |DJELLAL               |  2012|CONSEILLER COMMUNAUT DELEGUE |    12438,43|       0|  12438,43|
|268 S     |DOUTRE-ROUSSEL        |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|269 T     |GARNIER               |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|271 V     |TARDY                 |  2008|11EME VICE PRESIDENT         |    12024,46|       0|  12024,46|
|271 V     |TARDY                 |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|271 V     |TARDY                 |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|271 V     |TARDY                 |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|271 V     |TARDY                 |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|272 W     |GUIGUET               |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|272 W     |GUIGUET               |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|272 W     |GUIGUET               |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|272 W     |GUIGUET               |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|272 W     |GUIGUET               |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|273 X     |GUILLET REVOL         |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|274 Y     |ISSINDOU              |  2008|4EME VICE PRESIDENT          |    28051,02|       0|  38428,01|
|274 Y     |ISSINDOU              |  2009|3EME VICE PRESIDENT          |    29747,41|       0|  40972,52|
|274 Y     |ISSINDOU              |  2010|2EME VICE PRESIDENT          |    29951,37|       0|  41246,04|
|274 Y     |ISSINDOU              |  2011|2EME VICE PRESIDENT          |    30066,41|       0|  41429,62|
|274 Y     |ISSINDOU              |  2012|2EME VICE PRESIDENT          |    24108,64|       0|  32631,28|
|276 A     |JULLIEN               |  2008|20EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|276 A     |JULLIEN               |  2009|17EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|276 A     |JULLIEN               |  2010|16EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|276 A     |JULLIEN               |  2011|16EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|276 A     |JULLIEN               |  2012|16EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|277 B     |LACROIX               |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|278 C     |FIORASO               |  2008|1ER PREMIER VICE PRESIDENT   |    16818,66|       0|  16818,66|
|278 C     |FIORASO               |  2009|1ER PREMIER VICE PRESIDENT   |    18522,30|       0|  18522,30|
|278 C     |FIORASO               |  2010|1ER PREMIER VICE PRESIDENT   |    18656,70|       0|  18656,70|
|278 C     |FIORASO               |  2011|1ER PREMIER VICE PRESIDENT   |    18703,20|       0|  18703,20|
|278 C     |FIORASO               |  2012|1ER PREMIER VICE PRESIDENT   |    10054,88|       0|  10054,88|
|279 D     |LOPPE                 |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|279 D     |LOPPE                 |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|279 D     |LOPPE                 |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|279 D     |LOPPE                 |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|279 D     |LOPPE                 |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|280 E     |MATRAIRE              |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|281 F     |MEITE                 |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|283 H     |MOTTE                 |  2008|6EME VICE PRESIDENT          |    12285,11|       0|  12285,11|
|283 H     |MOTTE                 |  2009|6EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|283 H     |MOTTE                 |  2010|5EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|283 H     |MOTTE                 |  2011|5EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|283 H     |MOTTE                 |  2012|5EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|284 J     |OCTRU                 |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|284 J     |OCTRU                 |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|284 J     |OCTRU                 |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|284 J     |OCTRU                 |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|284 J     |OCTRU                 |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|285 K     |OLLIVIER              |  2008|12EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|285 K     |OLLIVIER              |  2009|8EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|285 K     |OLLIVIER              |  2010|7EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|285 K     |OLLIVIER              |  2011|4EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|285 K     |OLLIVIER              |  2012|4EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|286 L     |GRANGE                |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|287 M     |REMANDE               |  2008|14EME VICE PRESIDENT         |     4533,55|       0|   4533,55|
|288 N     |ROUGEMONT             |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|289 P     |UHRY                  |  2008|13EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|289 P     |UHRY                  |  2009|5EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|289 P     |UHRY                  |  2010|4EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|289 P     |UHRY                  |  2011|3EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|289 P     |UHRY                  |  2012|3EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|290 R     |VIAL                  |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|301 C     |COIGNE                |  2008|CONSEILLER COMMUNAUTAIRE     |     7994,50|       0|   7994,50|
|301 C     |COIGNE                |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|301 C     |COIGNE                |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|301 C     |COIGNE                |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|301 C     |COIGNE                |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|354 K     |FRISTOT               |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|356 M     |KAMOWSKI              |  2008|16EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|356 M     |KAMOWSKI              |  2009|15EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|356 M     |KAMOWSKI              |  2010|14EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|356 M     |KAMOWSKI              |  2011|14EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|356 M     |KAMOWSKI              |  2012|14EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|371 D     |PUECH                 |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|461 B     |ROUVEYRE              |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|507 B     |BEAUGRAND             |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|508 C     |BECHET                |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|509 D     |ESTEVE                |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|510 E     |DIAZ                  |  2008|19EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|510 E     |DIAZ                  |  2009|16EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|510 E     |DIAZ                  |  2010|15EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|510 E     |DIAZ                  |  2011|15EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|510 E     |DIAZ                  |  2012|15EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|511 F     |MAYOUSSIER            |  2008|CONSEILLER COMMUNAUT DELEGUE |    14970,08|       0|  14970,08|
|511 F     |MAYOUSSIER            |  2009|18EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|511 F     |MAYOUSSIER            |  2010|17EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|511 F     |MAYOUSSIER            |  2011|17EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|511 F     |MAYOUSSIER            |  2012|17EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|513 H     |PERILLIE              |  2008|18EME VICE PRESIDENT         |     4533,55|       0|   4533,55|
|518 N     |JULIOT                |  2008|CONSEILLER COMMUNAUTAIRE     |     7994,50|       0|   7994,50|
|518 N     |JULIOT                |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|518 N     |JULIOT                |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|518 N     |JULIOT                |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|518 N     |JULIOT                |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|54 J      |AVRILLIER             |  2008|3EME VICE PRESIDENT          |     4533,55|       0|   4533,55|
|563 M     |MOULIN                |  2008|15EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|563 M     |MOULIN                |  2009|13EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|563 M     |MOULIN                |  2010|12EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|563 M     |MOULIN                |  2011|7EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|563 M     |MOULIN                |  2012|7EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|564 N     |REPELLIN              |  2008|5EME VICE PRESIDENT          |    16818,66|       0|  16818,66|
|564 N     |REPELLIN              |  2009|9EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|564 N     |REPELLIN              |  2010|8EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|564 N     |REPELLIN              |  2011|8EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|564 N     |REPELLIN              |  2012|8EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|565 P     |DESCHAMPS             |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|566 R     |BAIETTO               |  2008|7EME VICE PRESIDENT          |    16818,66|       0|  16818,66|
|566 R     |BAIETTO               |  2009|4EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|566 R     |BAIETTO               |  2010|3EME VICE PRESIDENT          |    74738,49|       0|  95297,61|
|566 R     |BAIETTO               |  2011|PRESIDENT                    |    84892,58|       0| 103639,72|
|566 R     |BAIETTO               |  2012|PRESIDENT                    |    78577,44|       0|  91009,44|
|58 N      |BOULARD               |  2008|8EME VICE PRESIDENT          |    16818,66|       0|  16818,66|
|58 N      |BOULARD               |  2009|7EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|58 N      |BOULARD               |  2010|6EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|58 N      |BOULARD               |  2011|6EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|58 N      |BOULARD               |  2012|6EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|59 P      |CUPANI                |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|62 T      |LACHAT                |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|62 T      |LACHAT                |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|62 T      |LACHAT                |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|62 T      |LACHAT                |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|62 T      |LACHAT                |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|63 U      |MIGAUD                |  2008|PRESIDENT                    |    88657,58|       0| 121937,40|
|63 U      |MIGAUD                |  2009|PRESIDENT                    |    98805,00|       0| 132104,40|
|63 U      |MIGAUD                |  2010|PRESIDENT                    |    16173,42|       0|  21377,44|
|64 V      |BALLUET               |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|65 W      |POIRIER               |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|65 W      |POIRIER               |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|65 W      |POIRIER               |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|65 W      |POIRIER               |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|65 W      |POIRIER               |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|68 Z      |SAVIN                 |  2008|6EME VICE PRESIDENT          |    12024,46|       0|  12024,46|
|68 Z      |SAVIN                 |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|68 Z      |SAVIN                 |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|68 Z      |SAVIN                 |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|68 Z      |SAVIN                 |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|69 A      |VANNIER               |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|70 B      |VICARIO               |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|70 B      |VICARIO               |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|70 B      |VICARIO               |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|70 B      |VICARIO               |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|70 B      |VICARIO               |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|924 E     |FAUBERT               |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|

[Lien vers la base de données Rémunérations des élus](Bases/Réglementation/rémunérations.élu.csv)




## 5.8 Lien avec le compte de gestion



 
Cumul des lignes de paie par exercice et catégorie de ligne de paie   
 


Tableau  77  Année  2008

|           Catégorie|  Cumul annuel|
|-------------------:|-------------:|
|          Indemnités|  6 049 892,01|
|             Rappels|    288 659,23|
|       Rém. diverses|        797,42|
| Supplément familial|    349 475,20|
|         Traitements| 17 674 314,76|

Tableau  78  Année  2009

|           Catégorie|  Cumul annuel|
|-------------------:|-------------:|
|    Indem. Résidence|      3 095,76|
|          Indemnités|  6 246 021,01|
|             Rappels|    448 471,90|
|       Rém. diverses|        534,22|
| Supplément familial|    372 638,69|
|         Traitements| 18 492 771,17|

Tableau  79  Année  2010

|           Catégorie|  Cumul annuel|
|-------------------:|-------------:|
|          Indemnités|  6 549 869,92|
|             Rappels|    461 846,07|
|       Rém. diverses|     19 660,33|
| Supplément familial|    379 623,84|
|         Traitements| 19 150 025,62|

Tableau  80  Année  2011

|           Catégorie|  Cumul annuel|
|-------------------:|-------------:|
|          Indemnités|  6 942 832,60|
|             Rappels|    385 376,55|
|       Rém. diverses|     17 154,00|
| Supplément familial|    389 664,80|
|         Traitements| 20 045 292,29|

Tableau  81  Année  2012

|           Catégorie|  Cumul annuel|
|-------------------:|-------------:|
|          Indemnités|  7 317 926,65|
|             Rappels|    289 032,45|
|       Rém. diverses|     -3 271,09|
| Supplément familial|    388 778,18|
|         Traitements| 20 790 150,77|

 
[Lien vers la base détaillée des cumuls des lignes de paie](Bases/Réglementation/cumul.lignes.paie.csv)
 
[Lien vers la base agrégée des cumuls des lignes de paie](Bases/Réglementation/cumul.total.lignes.paie.csv)
 
 
*Avertissement : les rappels comprennent également les rappels de cotisations et déductions diverses.*    
  




## 5.9 Contrôle du supplément familial de traitement   
 


Pour les agents n'ayant pas d'enfant signalé en base, il a été détecté 13 bulletins de paie présentant un paiement du SFT apparemment anormal.

 
[Lien vers la base des paiements de SFT à agents sans enfant signalé](Bases/Réglementation/Paie.sans.enfant.réduit.csv)
 


Pour les agents ayant au moins un enfant, il a été détecté 70 bulletins de paie présentant un écart de paiement du SFT supérieur à 1 euro.

 
[Lien vers la base des écarts de paiement sur SFT](Bases/Réglementation/controle.sft.csv)
 



# Annexe
## Liens complémentaires

[Lien vers le fichier des personnels](Bases/Effectifs/Catégories des personnels.csv)
 
## Fiabilité du traitement statistique   
*Doublons*      

Attention : Altaïr a détecté des lignes dupliquées alors qu'aucun retraitement des lignes dupliquées n'est prévu par défaut.

 



 
*Tests de fiabilité sur le renseignement des heures et des quotités*    
  



    

Nombre de bulletins :  63 970

    

Les heures de travail ont été redressées avec la méthode  des quotités.

   

 Nombre de bulletins de paie redressés : 5 984

   

 Pourcentage de redressements : 9,35 % des bulletins de paie.

 


Pourcentage d'heures renseignées (après redressement éventuel): 92,9 %

  


Pourcentage de quotités renseignées : 99,2 %

  


Nombre de bulletins à heures et quotités :  59425 [ 92,9 %]

  


Nombre de bulletins à heures sans quotités :  0 [ 0 %]

  


Nombre de bulletins à quotités sans heures :  4005 [ 6,3 %]

  


Nombre de bulletins apparemment inactifs :  540 [ 0,8 %]

  



 

Nombre de bulletins de paie de salaires versés pour un champ Heures = 0 :  4 003 [ 6,3 %]

  



  
[Lien vers la base de données des salaires versés pour Heures=0](Bases/Fiabilité/base.heures.nulles.salaire.nonnull.csv)   
[Lien vers la base de données des salaires versés à quotité indéfinie](Bases/Fiabilité/base.quotité.indéfinie.salaire.nonnull.csv)   

# Tableau des personnels  




Non généré


# Divergences lignes-bulletins de paie     
  
*Pour exclure certains codes de paie de l'analyse, renseigner le fichier liste.exclusions.txt*  
  

   Codes exclus :  890A 319N 413R 409R 860A 876A 877A  

  
[Divergences lignes-bulletins de paie](Bases/Fiabilité/Delta.csv)     
  



