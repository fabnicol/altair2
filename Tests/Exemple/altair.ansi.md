---
title: false
author: false
date: false
output:
html_document:
css: style.css
---
  
![Image_Altair](Altair.png)
  
  
# D�monstrateur Alta�r version 15.10   


```
## Error in importer.bases.via.xhl2csv("Paie", colClasses = colonnes.classes.input): Chargement de la table bulletins-lignes de paie en �chec.
```


<p class = "centered"><b>Exercices 2013 � 2014 </b></p>
<p class = "author">Fabrice Nicol</h1>




mar. 08 d�c. 2015
   


La dur�e du travail prise en compte dans la base de donn�es est de  151,67  h par mois.

```
## Error in eval(expr, envir, enclos): could not find function "."
```

# 1. Statistiques de population

### 1.1 Effectifs



 



&nbsp;*Tableau 1*   


|                                                             | 2009  | 2010  | 2011  | 2012  | 2013  |
|:------------------------------------------------------------|:-----:|:-----:|:-----:|:-----:|:-----:|
|Total effectifs (a)                                          | 709,0 | 769,0 | 698,0 | 714,0 | 703,0 |
|&nbsp;&nbsp;&nbsp;dont pr�sents 12 mois                      | 465,0 | 462,0 | 444,0 | 428,0 | 433,0 |
|&nbsp;&nbsp;&nbsp;dont fonctionnaires (b)                    | 438,0 | 431,0 | 423,0 | 427,0 | 417,0 |
|&nbsp;&nbsp;&nbsp;dont fonct. pr�sents 12 mois               | 400,0 | 403,0 | 402,0 | 384,0 | 391,0 |
|&nbsp;&nbsp;&nbsp;dont non titulaires                        | 58,0  | 52,0  | 56,0  | 57,0  | 73,0  |
|&nbsp;&nbsp;&nbsp;dont �lus                                  | 14,0  | 14,0  | 14,0  | 25,0  | 14,0  |
|&nbsp;&nbsp;&nbsp;dont �lus pr�sents 12 mois                 | 13,0  | 14,0  | 13,0  | 14,0  | 14,0  |
|&nbsp;&nbsp;&nbsp;dont vacataires d�tect�s (c)               | 39,0  | 48,0  | 48,0  | 45,0  | 46,0  |
|&nbsp;&nbsp;&nbsp;dont assistantes maternelles d�tect�es (c) | 16,0  | 15,0  |  4,0  |  0,0  |  0,0  |
|Postes non actifs (g)                                        | 89,0  | 131,0 | 64,0  | 78,0  | 65,0  |
|Postes annexes (g)                                           | 218,0 | 276,0 | 208,0 | 215,0 | 209,0 |
|Postes actifs non annexes (g)                                | 477,0 | 479,0 | 476,0 | 474,0 | 480,0 |
|Total ETP/ann�e (d)                                          | 511,7 | 537,3 | 535,0 | 518,0 | 523,1 |
|Total ETPT/ann�e (e)                                         | 443,8 | 457,1 | 448,1 | 437,3 | 440,8 |
|Total ETPT/ann�e personnes en place (f)(g)                   |  0,0  | 354,0 | 357,4 | 327,6 | 331,9 |
|Total ETPT/ann�e fonctionnaires (g)                          | 384,9 | 387,5 | 381,3 | 372,4 | 368,1 |
|Total ETPT/ann�e titulaires � temps complet (g)              | 288,5 | 301,2 | 294,0 | 262,6 | 277,1 |
|Total ETPT non titulaires (g)                                | 36,7  | 32,3  | 32,5  | 34,6  | 40,5  |
|Total ETPT autre statut                                      |  2,8  |  4,2  |  4,8  |  6,4  |  5,6  |
|Total ETPT postes non actifs (g)                             |  0,0  |  0,0  |  0,0  |  0,0  |  0,0  |
|Total ETPT postes annexes (g)                                | 18,9  | 25,7  | 24,3  | 23,5  | 22,3  |
|Total ETPT postes actifs non annexes (g)                     | 424,9 | 431,4 | 423,8 | 413,8 | 418,4 |


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
   


|  Statistique  | �ge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  16                  |          |
| 1er quartile  |                  35                  |          |
|    M�diane    |                  43                  |          |
|    Moyenne    |                42,45                 |   542    |
| 3�me quartile |                  51                  |          |
|    Maximum    |                  66                  |          |

 

![plot of chunk unnamed-chunk-9](figure/unnamed-chunk-9-1.png) 

 
&nbsp;*Tableau 3*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2014 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  17                  |          |
| 1er quartile  |                  36                  |          |
|    M�diane    |                  45                  |          |
|    Moyenne    |                43,35                 |   528    |
| 3�me quartile |                  52                  |          |
|    Maximum    |                  70                  |          |

 
 

![plot of chunk unnamed-chunk-11](figure/unnamed-chunk-11-1.png) 


### 1.3 Pyramide des �ges, personnels non titulaires
 
&nbsp;*Tableau 4*   
   


|  Statistique  | �ge des personnels non titulaires<br>au 31/12/2013 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         17                         |          |
| 1er quartile  |                         24                         |          |
|    M�diane    |                        37,5                        |          |
|    Moyenne    |                       36,97                        |   100    |
| 3�me quartile |                       46,25                        |          |
|    Maximum    |                         66                         |          |

 

![plot of chunk unnamed-chunk-13](figure/unnamed-chunk-13-1.png) 

 
&nbsp;*Tableau 5*   
   


|  Statistique  | �ge des personnels non titulaires<br>au 31/12/2014 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         17                         |          |
| 1er quartile  |                         23                         |          |
|    M�diane    |                         35                         |          |
|    Moyenne    |                       36,09                        |   108    |
| 3�me quartile |                         46                         |          |
|    Maximum    |                         70                         |          |

![plot of chunk unnamed-chunk-14](figure/unnamed-chunk-14-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.nontit.csv)  
 


### 1.4 Pyramide des �ges, personnels fonctionnaires stagiaires et titulaires
 
&nbsp;*Tableau 6*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  24                  |          |
| 1er quartile  |                  38                  |          |
|    M�diane    |                  45                  |          |
|    Moyenne    |                44,18                 |   410    |
| 3�me quartile |                  51                  |          |
|    Maximum    |                  62                  |          |

 


![plot of chunk unnamed-chunk-16](figure/unnamed-chunk-16-1.png) 

 
&nbsp;*Tableau 7*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2014 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  22                  |          |
| 1er quartile  |                  40                  |          |
|    M�diane    |                  47                  |          |
|    Moyenne    |                  46                  |   400    |
| 3�me quartile |                  53                  |          |
|    Maximum    |                  63                  |          |



![plot of chunk unnamed-chunk-18](figure/unnamed-chunk-18-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.fonctionnaires.csv)  
 
### 1.5 Pyramide des �ges, personnels permanents (titulaires, stagiaires et non titulaires)
 
&nbsp;*Tableau 8*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  17                  |          |
| 1er quartile  |                  36                  |          |
|    M�diane    |                 43,5                 |          |
|    Moyenne    |                42,76                 |   510    |
| 3�me quartile |                  51                  |          |
|    Maximum    |                  66                  |          |

 

![plot of chunk unnamed-chunk-20](figure/unnamed-chunk-20-1.png) 

&nbsp;*Tableau 9*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2014 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  17                  |          |
| 1er quartile  |                  37                  |          |
|    M�diane    |                  45                  |          |
|    Moyenne    |                43,89                 |   508    |
| 3�me quartile |                  52                  |          |
|    Maximum    |                  70                  |          |



![plot of chunk unnamed-chunk-22](figure/unnamed-chunk-22-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.permanents.csv)  
 
### 1.6 Effectifs des personnels par dur�e de service

**Personnels en fonction (hors �lus) des exercices 2013 � 2014 inclus :**

 
&nbsp;*Tableau 10*   
   


| Plus de 2 ans | Moins de 2 ans | Moins d'un an | Moins de six mois |
|:-------------:|:--------------:|:-------------:|:-----------------:|
|      453      |      140       |      84       |        23         |



![plot of chunk unnamed-chunk-24](figure/unnamed-chunk-24-1.png) 


**Effectifs (hors �lus)**   




 
&nbsp;*Tableau 11*   
   


|                  | 2009 | 2010 | 2011 | 2012 | 2013 |
|:-----------------|:----:|:----:|:----:|:----:|:----:|
|Plus de deux ans  | 436  | 448  | 448  | 428  | 408  |
|Moins de deux ans |  35  |  24  |  24  |  42  |  68  |
|Total             | 471  | 472  | 472  | 470  | 476  |



**Nota :**
*Personnels en place : ayant servi au moins deux ann�es cons�cutives pendant la p�riode.*     
*Plus/moins de deux ans : plus/mois de 730 jours sur la p�riode sous revue.*   



  
  

```
## Error in eval(expr, envir, enclos): could not find function "s�lectionner.exercice.analyse.r�mun�rations"
```

   
# 2. R�mun�rations brutes : analyse pour l'exercice 2009    
   
## 2.1 Masse salariale brute de l'ensemble des agents     
    
 

```
## Error in is.data.frame(x): object 'Analyse.r�mun�rations.exercice' not found
```

### Cumuls des r�mun�rations brutes pour l'exercice 2009   
 
*Personnels (hors �lus)*     
 
&nbsp;*Tableau 12*   
   

```
## Error in `[.data.table`(masses.personnels, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```

   

**D�finitions :**   

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lu*          : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
    
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   
 
 
&nbsp;*Tableau 13*    
   

```
## Error in `[.data.table`(masses.personnels, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```

   
� comparer aux soldes des comptes 641 et 648 du compte de gestion.   


  
## 2.2 Masse salariale brute des fonctionnaires    

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*   

```
## Error in eval(expr, envir, enclos): object 'Analyse.r�mun�rations.exercice' not found
```

![plot of chunk unnamed-chunk-127](figure/unnamed-chunk-127-1.png) 
![plot of chunk unnamed-chunk-128](figure/unnamed-chunk-128-1.png) 
![plot of chunk unnamed-chunk-129](figure/unnamed-chunk-129-1.png) 
![plot of chunk unnamed-chunk-130](figure/unnamed-chunk-130-1.png) 
![plot of chunk unnamed-chunk-131](figure/unnamed-chunk-131-1.png) 

![plot of chunk unnamed-chunk-132](figure/unnamed-chunk-132-1.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio r�mun�ration / quotit�*   
   
**Effectif : 84**   
   
**Tests de coh�rence**   


   
Somme des r�mun�rations brutes vers�es aux personnels titulaires et stagiaires :    

 
&nbsp;*Tableau 14*   
   


|           Agr�gats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 1 049 352,5 |
| Brut annuel (lignes), dont : | 1 027 334,0 |
|         \ \ Primes :         |  112 593,4  |
|   \ \ Autres r�mun�rations   |   2 947,6   |
|     Part de primes en %      |    10,7     |

    
**D�finitions :**     
   
 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lus*         : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
   
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   

 
&nbsp;*Tableau 15*      
   


|     Agr�gats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 1 049 352,5 |
|  Lignes de paie   | 1 027 334,0 |
|    Diff�rence     |  22 018,5   |

   
A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.    

**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2009**      
 
&nbsp;*Tableau 16*   
   


|   Statistique| Traitement indiciaire|    Primes| Autres r�mun�rations| Effectif|
|-------------:|---------------------:|---------:|--------------------:|--------:|
|       Minimum|                 3 039|    -22,25|                 0,00|         |
|  1er quartile|                 6 244|      0,00|                 0,00|         |
|       M�diane|                 8 859|    286,40|                 0,00|         |
|       Moyenne|                10 882|  1 353,82|                35,51|       83|
| 3�me quartile|                13 138|  1 112,86|                 0,00|         |
|       Maximum|                44 200| 17 494,04|             1 100,00|         |

 
&nbsp;*Tableau 17*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la r�m. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     3 246|      3 830|             4 806|                     -0,2 483|         |
|  1er quartile|                     6 533|      6 872|            17 524|                      0,0 000|         |
|       M�diane|                     8 985|      9 699|            19 070|                      3,0 647|         |
|       Moyenne|                    12 236|     12 472|            19 898|                      5,8 142|       83|
| 3�me quartile|                    13 995|     14 436|            20 391|                      8,0 952|         |
|       Maximum|                    61 694|     58 944|            58 944|                     36,9 267|         |

   
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

```
## Error in attach(Analyse.r�mun�rations.exercice, warn.conflicts = FALSE): object 'Analyse.r�mun�rations.exercice' not found
```

```
## Error in eval(expr, envir, enclos): object 'r�mun�ration.indemnitaire.imposable.eqtp' not found
```



![plot of chunk unnamed-chunk-141](figure/unnamed-chunk-141-1.png) 

  
**Nota :**   
*Ne sont retenues que les r�mun�rations sup�rieures � 1 000 k&euro;.*  
*Les �lus ne sont pas pris en compte.*  
  

```
## Error in positive(autres.r�mun�rations): object 'autres.r�mun�rations' not found
```

![plot of chunk unnamed-chunk-142](figure/unnamed-chunk-142-1.png) 

```
## Error in eval(expr, envir, enclos): object 'Analyse.r�mun�rations.exercice' not found
```

  
**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2009**     
  
 
&nbsp;*Tableau 18*   
   


|   Statistique|    Primes| Autres r�mun�rations| Effectif|
|-------------:|---------:|--------------------:|--------:|
|       Minimum|    -22,25|                 0,00|         |
|  1er quartile|      0,00|                 0,00|         |
|       M�diane|    264,70|                 0,00|         |
|       Moyenne|  1 340,40|                35,09|       84|
| 3�me quartile|  1 087,85|                 0,00|         |
|       Maximum| 17 494,04|             1 100,00|         |

   
&nbsp;*Tableau 19*   
   
   

|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               3 830|                    4 806|         |
|  1er quartile|               6 877|                   17 484|         |
|       M�diane|               9 708|                   19 009|         |
|       Moyenne|              12 492|                   19 835|       84|
| 3�me quartile|              14 380|                   20 368|         |
|       Maximum|              58 944|                   58 944|         |


  
  

```
## Error in eval(expr, envir, enclos): could not find function "s�lectionner.exercice.analyse.r�mun�rations"
```

   
# 3. R�mun�rations brutes : analyse pour l'exercice 2013    
   
## 3.1 Masse salariale brute de l'ensemble des agents     
    
 

```
## Error in is.data.frame(x): object 'Analyse.r�mun�rations.exercice' not found
```

### Cumuls des r�mun�rations brutes pour l'exercice 2013   
 
*Personnels (hors �lus)*     
 
&nbsp;*Tableau 20*   
   

```
## Error in `[.data.table`(masses.personnels, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```

   

**D�finitions :**   

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lu*          : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
    
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   
 
 
&nbsp;*Tableau 21*    
   

```
## Error in `[.data.table`(masses.personnels, "Montant.brut.annuel"): When i is a data.table (or character vector), the columns to join by must be specified either using 'on=' argument (see ?data.table) or by keying x (i.e. sorted, and, marked as sorted, see ?setkey). Keyed joins might have further speed benefits on very large data due to x being sorted in RAM.
```

   
� comparer aux soldes des comptes 641 et 648 du compte de gestion.   


  
## 3.2 Masse salariale brute des fonctionnaires    

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*   

```
## Error in eval(expr, envir, enclos): object 'Analyse.r�mun�rations.exercice' not found
```

![plot of chunk unnamed-chunk-149](figure/unnamed-chunk-149-1.png) 
![plot of chunk unnamed-chunk-150](figure/unnamed-chunk-150-1.png) 
![plot of chunk unnamed-chunk-151](figure/unnamed-chunk-151-1.png) 
![plot of chunk unnamed-chunk-152](figure/unnamed-chunk-152-1.png) 
![plot of chunk unnamed-chunk-153](figure/unnamed-chunk-153-1.png) 

![plot of chunk unnamed-chunk-154](figure/unnamed-chunk-154-1.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio r�mun�ration / quotit�*   
   
**Effectif : 84**   
   
**Tests de coh�rence**   


   
Somme des r�mun�rations brutes vers�es aux personnels titulaires et stagiaires :    

 
&nbsp;*Tableau 22*   
   


|           Agr�gats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 1 049 352,5 |
| Brut annuel (lignes), dont : | 1 027 334,0 |
|         \ \ Primes :         |  112 593,4  |
|   \ \ Autres r�mun�rations   |   2 947,6   |
|     Part de primes en %      |    10,7     |

    
**D�finitions :**     
   
 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lus*         : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
   
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   

 
&nbsp;*Tableau 23*      
   


|     Agr�gats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 1 049 352,5 |
|  Lignes de paie   | 1 027 334,0 |
|    Diff�rence     |  22 018,5   |

   
A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.    

**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2013**      
 
&nbsp;*Tableau 24*   
   


|   Statistique| Traitement indiciaire|    Primes| Autres r�mun�rations| Effectif|
|-------------:|---------------------:|---------:|--------------------:|--------:|
|       Minimum|                 3 039|    -22,25|                 0,00|         |
|  1er quartile|                 6 244|      0,00|                 0,00|         |
|       M�diane|                 8 859|    286,40|                 0,00|         |
|       Moyenne|                10 882|  1 353,82|                35,51|       83|
| 3�me quartile|                13 138|  1 112,86|                 0,00|         |
|       Maximum|                44 200| 17 494,04|             1 100,00|         |

 
&nbsp;*Tableau 25*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la r�m. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     3 246|      3 830|             4 806|                     -0,2 483|         |
|  1er quartile|                     6 533|      6 872|            17 524|                      0,0 000|         |
|       M�diane|                     8 985|      9 699|            19 070|                      3,0 647|         |
|       Moyenne|                    12 236|     12 472|            19 898|                      5,8 142|       83|
| 3�me quartile|                    13 995|     14 436|            20 391|                      8,0 952|         |
|       Maximum|                    61 694|     58 944|            58 944|                     36,9 267|         |

   
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

```
## Error in attach(Analyse.r�mun�rations.exercice, warn.conflicts = FALSE): object 'Analyse.r�mun�rations.exercice' not found
```

```
## Error in eval(expr, envir, enclos): object 'r�mun�ration.indemnitaire.imposable.eqtp' not found
```



![plot of chunk unnamed-chunk-163](figure/unnamed-chunk-163-1.png) 

  
**Nota :**   
*Ne sont retenues que les r�mun�rations sup�rieures � 1 000 k&euro;.*  
*Les �lus ne sont pas pris en compte.*  
  

```
## Error in positive(autres.r�mun�rations): object 'autres.r�mun�rations' not found
```

![plot of chunk unnamed-chunk-164](figure/unnamed-chunk-164-1.png) 

```
## Error in eval(expr, envir, enclos): object 'Analyse.r�mun�rations.exercice' not found
```

  
**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2013**     
  
 
&nbsp;*Tableau 26*   
   


|   Statistique|    Primes| Autres r�mun�rations| Effectif|
|-------------:|---------:|--------------------:|--------:|
|       Minimum|    -22,25|                 0,00|         |
|  1er quartile|      0,00|                 0,00|         |
|       M�diane|    264,70|                 0,00|         |
|       Moyenne|  1 340,40|                35,09|       84|
| 3�me quartile|  1 087,85|                 0,00|         |
|       Maximum| 17 494,04|             1 100,00|         |

   
&nbsp;*Tableau 27*   
   
   

|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               3 830|                    4 806|         |
|  1er quartile|               6 877|                   17 484|         |
|       M�diane|               9 708|                   19 009|         |
|       Moyenne|              12 492|                   19 835|       84|
| 3�me quartile|              14 380|                   20 368|         |
|       Maximum|              58 944|                   58 944|         |

 
[Lien vers la base des r�mun�rations](Bases/R�mun�rations/Analyse.r�mun�rations.csv)  
  



  
## 3.4 Comparaisons source INSEE/DGCL   
  
*Salaires annnuels moyens 2011 en EQTP (hors assistantes maternelles)*   
 
&nbsp;*Tableau 28*   
   


| Agr�gat (&euro;) | Salaires bruts 2011 | Salaires bruts 2012 | Salaires bruts 2013 |
|------------------|---------------------|---------------------|---------------------|
|     Ensemble     |      25 908,0       |      26 340,0       |      26 616,0       |
|    Titulaires    |      26 676,0       |      27 108,0       |      27 444,0       |
| Autres salari�s  |      22 836,0       |         NA          |      24 360,0       |

  
**El�ments de la r�mun�ration brute pour les titulaires de la FPT entre 2010 et 2012**      
  
 
&nbsp;*Tableau 29*   
   


|         R�m. annuelles          | 2010  | Primes | 2011  | Primes | 2012  | Primes | 2013  | Primes |
|---------------------------------|-------|--------|-------|--------|-------|--------|-------|--------|
|          Salaire brut           | 26305 |        | 26660 |        | 27108 |        | 27444 |        |
|         Traitement brut         | 20350 | 22,6 % | 20562 | 22,9 % | 20724 | 23,6 % | 21060 | 23,6 % |
| Primes et r�mun�rations annexes |       |        |       |        |       |        |       |        |
|       y compris IR et SFT       | 5955  |        | 6098  |        | 6384  |        | 6384  |        |

  
*Champ : France. Salari�s en �quivalent-temps plein (EQTP) des collectivit�s territoriales (y compris b�n�ficiaires de contrats aid�s, hors assistantes maternelles).*   												
*Les primes sont cumul�es au suppl�ment familial de traitement (SFT) et � l'indemnit� de r�sidence (IR). Le cumul est rapport� � la r�mun�ration brute totale.*    
[Source INSEE](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)    
[Source DGCL](http://www.fonction-publique.gouv.fr/files/files/statistiques/rapports_annuels/2012-2013/xls/Vue3_1_Remunerations.xls)    
[Source DGFIP PLF 2015](http://www.performance-publique.budget.gouv.fr/sites/performance_publique/files/farandole/ressources/2015/pap/pdf/jaunes/jaune2015_fonction_publique.pdf)   
  




# 4. R�mun�rations nettes : �volutions sur la p�riode 2013 - 2014    

Nombre d'exercices: 5   
 
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

## 4.2 Evolutions des r�mun�rations nettes sur la p�riode 2013 - 2014   

### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors �lus)





   
**Salaire net moyen par t�te (SMPT net) en EQTP, hors �lus**         
      
 
&nbsp;*Tableau 30*   
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-----------------:|:-------------:|
| 2009  |            9 036            |               |      19 185       |               |
| 2010  |            8 992            |               |      19 051       |               |
| 2011  |           9 099,4           |     5,63      |      19 278       |     4,52      |
| 2012  |           9 256,7           |               |      19 695       |               |
| 2013  |           9 544,7           |               |      20 052       |               |

  
**Effet de noria sur salaires nets et taux de remplacements**       
  
**Effet de noria** : *diff�rence entre la r�mun�ration annuelle des entrants � l'ann�e N et des sortants � l'ann�e N-1*.  
*Usuellement calcul�e sur les r�mun�rations brutes, ici sur les r�mun�rations nettes EQTP*  
*afin d'appr�cier l'impact de cet effet sur l'�volution des r�mun�rations nette moyennes calcul�e au tableau pr�c�dent.*               
 
&nbsp;*Tableau 31*   
   


```
## Error in FUN(X[[i]], ...): attempt to select less than one element
```


*MS N-1 : masse salariale nette de l'ann�e n-1.*   
      
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) en EQTP**         
      
 
&nbsp;*Tableau 32*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          4 794|
|  1er quartile|         16 990|
|       M�diane|         18 670|
|       Moyenne|         20 050|
| 3�me quartile|         22 030|
|       Maximum|         54 530|



|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|             NA|
|  1er quartile|             NA|
|       M�diane|             NA|
|       Moyenne|            NaN|
| 3�me quartile|             NA|
|       Maximum|             NA|

 
*Nota :*  La population retenue est constitu�e des agents qui :   
&nbsp;&nbsp;- ne font pas partie des 2 centiles extr�maux   
&nbsp;&nbsp;- sont au moins pr�sents 1 jour(s) la premi�re et la derni�re ann�e d'activit�  
Les �lus, vacataires et assistantes maternelles sont retir�s du p�rim�tre.   
Seuls sont pris en compte les agents ayant connu au moins un mois actif et ayant eu, sur l'ann�e, des r�mun�rations non annexes.  
[Compl�ments m�thodologiques](Docs/m�thodologie.pdf)     
     
**Comparaisons source INSEE/DGCL**   

**Salaires annuels moyens 2011 et 2012 en EQTP (hors assistantes maternelles)**   
 
&nbsp;*Tableau 33*       


|     Agr�gat     | Salaires nets 2011 (&euro;) | Salaires nets 2012 (&euro;) | Salaires nets 2013 (&euro;) |
|-----------------|-----------------------------|-----------------------------|-----------------------------|
|    Ensemble     |          21 876,0           |          22 176,0           |          22 224,0           |
|   Titulaires    |          22 632,0           |          22 920,0           |          22 920,0           |
| Autres salari�s |          18 864,0           |             NA              |             NA              |

*Champ : France. Salari�s en �quivalent-temps plein (EQTP) des collectivit�s territoriales (y compris b�n�ficiaires de contrats aid�s, hors assistantes maternelles).*     			



**Distribution des salaires nets annuels en EQTP dans la fonction publique par versant en 2011**   

 
&nbsp;*Tableau 34*   
   


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
 
&nbsp;*Tableau 35*   
   


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
&nbsp;*Tableau 36*   
   


```
## Error in data.frame(c("7 985,2", "7 985,2", "7 985,2", "7 985,2"), c("7 944,9", : les arguments impliquent des nombres de lignes diff�rents : 4, 0, 1
```





  
**Effet de noria sur salaires nets et taux de remplacements**       
 
&nbsp;*Tableau 37*   
   


```
## Error in e[[y]]: attempt to select less than one element
```

    
*MS N-1 : masse salariale nette de l'ann�e n-1.*   
      
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) en EQTP**         
 
&nbsp;*Tableau 38*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          8 883|
|  1er quartile|         17 480|
|       M�diane|         19 420|
|       Moyenne|         20 820|
| 3�me quartile|         22 730|
|       Maximum|         54 530|



|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|             NA|
|  1er quartile|             NA|
|       M�diane|             NA|
|       Moyenne|            NaN|
| 3�me quartile|             NA|
|       Maximum|             NA|

   



  
**Evolution du SMPT net des titulaires � temps complet**     
  
 
&nbsp;*Tableau 39*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | SMPT (&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:-------------:|:-------------:|
| 2009  |               5 703,4               |               |    19 667     |               |
| 2010  |                6 052                |               |    19 974     |               |
| 2011  |               5 934,6               |     1,35      |    20 049     |     5,34      |
| 2012  |               5 356,5               |               |    20 290     |               |
| 2013  |               5 780,4               |               |    20 718     |               |

   
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) des titulaires � temps complet**         
      
 
&nbsp;*Tableau 40*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          8 883|
|  1er quartile|         17 400|
|       M�diane|         19 280|
|       Moyenne|         20 720|
| 3�me quartile|         22 210|
|       Maximum|         54 530|

  


|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|             NA|
|  1er quartile|             NA|
|       M�diane|             NA|
|       Moyenne|            NaN|
| 3�me quartile|             NA|
|       Maximum|             NA|


[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)


## 4.3 Glissement vieillesse-technicit� (GVT)   

### 4.3.1 Ensemble des personnels   
  
*Cette section est consacr�e � la r�mun�ration moyenne des personnes en place (RMPP), d�finies comme pr�sentes deux ann�es enti�res cons�cutives avec la m�me quotit�*   
*L'�volution de la RMPP permet d'�tudier le glissement vieillesse-technicit� "positif", � effectifs constants sur deux ann�es*      
*Le GVT positif est d� aux mesures statutaires et individuelles, � l'avancement et aux changements d'activit�*  

![plot of chunk unnamed-chunk-51](figure/unnamed-chunk-51-1.png) 






  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 41*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2010  |               7 240,1               |               |        19 890        |               |
| 2011  |               7 350,9               |    -0,551     |        19 867        |     4,93      |
| 2012  |               6 955,7               |               |        20 398        |               |
| 2013  |               7 200,2               |               |        20 870        |               |

   
**Distribution et variation sur la p�riode de la r�mun�ration nette des personnes en place**                
 
&nbsp;*Tableau 42*   
   



 


|   Statistique| Premi�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          7 081|         |
|  1er quartile|         15 860|         |
|       M�diane|         17 480|         |
|       Moyenne|         19 190|      417|
| 3�me quartile|         20 730|         |
|       Maximum|         56 150|         |

 
&nbsp;*Tableau 43*   
   


|   Statistique| Derni�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|         10 140|         |
|  1er quartile|         17 240|         |
|       M�diane|         19 040|         |
|       Moyenne|         20 460|      416|
| 3�me quartile|         22 150|         |
|       Maximum|         54 530|         |


*Variation individuelle de r�mun�ration nette en EQTP pour les personnels pr�sents la premi�re et la derni�re ann�e*   
 
&nbsp;*Tableau 44*   
   


|   Statistique| Variation normalis�e (%)| Variation annuelle moyenne normalis�e (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -36,523|                                   -10,741|         |
|  1er quartile|                    5,698|                                     1,395|         |
|       M�diane|                   10,318|                                     2,485|         |
|       Moyenne|                   10,853|                                     2,530|      359|
| 3�me quartile|                   16,113|                                     3,805|         |
|       Maximum|                   38,583|                                     8,499|         |

   
### 4.3.2 Titulaires et stagiaires     
  



  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 45*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2010  |               6 896,8               |               |        19 818        |               |
| 2011  |               7 009,9               |     0,365     |        19 858        |     4,89      |
| 2012  |               6 728,9               |               |        20 329        |               |
| 2013  |                6 922                |               |        20 787        |               |

   
**Distribution et variation sur la p�riode de la r�mun�ration nette des fonctionnaires en place**                
 
&nbsp;*Tableau 46*   
   
 


|   Statistique| Premi�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          7 081|         |
|  1er quartile|         15 980|         |
|       M�diane|         17 630|         |
|       Moyenne|         19 350|      384|
| 3�me quartile|         20 820|         |
|       Maximum|         56 150|         |

 
&nbsp;*Tableau 47*   
   


|   Statistique| Derni�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          4 743|         |
|  1er quartile|         17 400|         |
|       M�diane|         19 430|         |
|       Moyenne|         21 120|      428|
| 3�me quartile|         23 040|         |
|       Maximum|         63 190|         |


*Variation individuelle de r�mun�ration nette en EQTP pour les personnels pr�sents la premi�re et la derni�re ann�e*   
 
&nbsp;*Tableau 48*   
   


|   Statistique| Variation normalis�e (%)| Variation annuelle moyenne normalis�e (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -36,523|                                   -10,741|         |
|  1er quartile|                    5,885|                                     1,440|         |
|       M�diane|                   10,354|                                     2,494|         |
|       Moyenne|                   10,960|                                     2,558|      345|
| 3�me quartile|                   16,096|                                     3,802|         |
|       Maximum|                   38,583|                                     8,499|         |



[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)

**Nota**   
*Personnes en place :* en fonction au moins deux ann�es cons�cutives sur la p�riode 2013 � 2014    
*Variation sur la p�riode d'activit� :* entre l'arriv�e et le d�part de la personne      
*Variation normalis�e :* conforme � la d�finition INSEE (pr�sente en d�but et en fin de p�riode avec la m�me quotit�)  
 
**Commentaire**       
Les diff�rences �ventuelles constat�es entre l'�volution de la RMPP au tableau 46 sont dues soit � l'effet de noria soit � l'effet p�rim�tre.    
     
[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)


### 4.4 Comparaisons avec la situation nationale des r�mun�rations   
 
**�volution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    
 
&nbsp;*Tableau 49*   
   


|   Ann�e    | 2008-09 | 2009-10 | 2010-11 | 2011-12 | 2012-13 | 2008-12 | Moy. 2008-12 | M�d. 2007-11 |
|------------|---------|---------|---------|---------|---------|---------|--------------|--------------|
| SMPT brut  |   2,5   |   1,3   |   1,5   |   1,7   |   1,1   |   7,2   |     1,8      |              |
|  SMPT net  |   3,0   |   1,4   |   1,3   |   1,4   |   0,8   |   7,3   |     1,8      |     13,4     |
| RMPP brute |   3,3   |   2,5   |   2,5   |   2,7   |   1,9   |  11,5   |     2,8      |              |
| RMPP nette |   3,3   |   2,5   |   2,3   |   2,4   |   1,6   |  10,9   |     2,6      |              |

*Source : fichier g�n�ral de l'�tat (FGE), DADS, SIASP, Insee, Drees. Traitement Insee, Drees, DGCL*    
Hors assistants maternels et familiaux, y compris b�n�ficiaires de contrats aid�s.   
SMPT : Salaire moyen par t�te en EQTP.   
RMPP : Agents pr�sents 24 mois cons�cutifs chez le m�me employeur avec la m�me quotit� de travail.  
Moyenne des variations g�om�triques annuelles pour les agents du champ.  
La derni�re colonne pr�sente la m�diane des augmentations du SMPT net pour les agents pr�sents en 2007 et 2011.   
 
**Salaires nets annuels et �volution moyenne type de collectivit� en &euro; courants  EQTP**    
  
 
&nbsp;*Tableau 50*   
   


|           Collectivit�            | SMPT net 2011 | SMPT net 2012 | SMPT net 2013 | Evol. Moy. 2007-2011 (%) |
|-----------------------------------|---------------|---------------|---------------|--------------------------|
|             Communes              |   20 784,0    |   21 120,0    |   21 096,0    |           2,5            |
|    CCAS et caisses des �coles     |   19 415,0    |   19 716,0    |   19 788,0    |           2,4            |
|      EPCI � fiscalit� propre      |   22 882,0    |   23 088,0    |   23 184,0    |           3,1            |
| Autres structures intercommunales |   21 299,0    |   21 684,0    |   21 828,0    |           3,0            |
|           D�partements            |   24 487,0    |   24 744,0    |   24 852,0    |           3,9            |
|               SDIS                |   29 811,0    |   29 940,0    |   30 180,0    |           3,4            |
|              R�gions              |   22 432,0    |   22 836,0    |   23 004,0    |           3,8            |
|   Autres collectivit�s locales    |   24 680,0    |   24 696,0    |   24 828,0    |           3,2            |
|        Ensemble (moyenne)         |   21 873,0    |   22 176,0    |   22 212,0    |           2,9            |


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


```
## Error in uniqueN(Paie[Statut != "TITULAIRE" & Statut != "STAGIAIRE" & : object 'Statut' not found
```

```
## Error in eval(expr, envir, enclos): object 'Statut' not found
```

```
## Error in grep(expression.r�g.pfi, Libell�, ignore.case = TRUE, perl = TRUE): object 'Libell�' not found
```

```
## Error in unique(Libell�[filtre], by = NULL): object 'Libell�' not found
```

Primes informatiques potentielles : PRIME FONCTION INFORMATIQUE
 
&nbsp;*Tableau 51*   
   


```
## Error in Tableau(c("Nombre de lignes NBI pour non titulaires", "Nombre de b�n�ficiaires de PFI"), : 'names' attribute [2] must be the same length as the vector [1]
```

  
[Lien vers la base de donn�es NBI aux non titulaires](Bases/R�glementation/NBI.aux.non.titulaires.csv)   
[Lien vers la base de donn�es Primes informatiques](Bases/R�glementation/personnels.prime.informatique.csv)   
  
**Nota :**   
NBI: nouvelle bonification indiciaire   
PFI: prime de fonctions informatiques   



```
## Error in grepl(expression.r�g.nbi, Libell�, perl = TRUE, ignore.case = TRUE): object 'Libell�' not found
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
   


| R�mun�rations de NBI anormales, par agent et par exercice | Montants correspondants |
|:---------------------------------------------------------:|:-----------------------:|
|                            82                             |         37 139          |

  
[Lien vers la base de donn�es NBI anormales](Bases/Fiabilit�/lignes.nbi.anormales.csv)   
  
**Nota :**   
*Est consid�r� comme manifestement anormal un total annuel de r�mun�rations NBI correspondant � un point d'indice net mensuel inf�rieur � 4 euros ou sup�rieur � 6 euros.*    
*Les rappels ne sont pas pris en compte dans les montants vers�s. Certains �carts peuvent �tre r�gularis�s en les prenant en compte*     
 
&nbsp;*Tableau 53*   
   


| Ann�e | Cumuls des NBI | Montants vers�s (a) | Point d'INM apparent | Point d'INM moyen | Contr�le |
|-------|----------------|---------------------|----------------------|-------------------|----------|
| 2009  |    20 840,0    |      92 353,7       |         4,4          |        4,6        |  Orange  |
| 2010  |    19 840,0    |      88 811,3       |         4,5          |        4,6        |   Vert   |
| 2011  |    17 905,0    |      80 552,2       |         4,5          |        4,6        |   Vert   |
| 2012  |    16 230,0    |      72 636,3       |         4,5          |        4,6        |  Orange  |
| 2013  |    15 235,0    |      67 438,4       |         4,4          |        4,6        |  Orange  |


*(a) Hors rappels sur r�mun�rations*   




  
[Lien vers la base de donn�es des cumuls annuels de NBI](Bases/Fiabilit�/cumuls.nbi.csv)   
  
 
## 5.2 Contr�le des vacations pour les fonctionnaires


```
## Error in eval(expr, envir, enclos): object 'Statut' not found
```



Il y a  0 fonctionnaire(s) effectuant des vacations pour son propre �tablissement. Les bulletins concern�s sont donn�s en lien.


[Lien vers les matricules des fonctionnaires concern�s](Bases/R�glementation/matricules.fonctionnaires.et.vacations.csv)
[Lien vers les bulletins de paie correspondants](Bases/R�glementation/lignes.fonctionnaires.et.vacations.csv)

## 5.3 Contr�les sur les cumuls traitement indiciaire, indemnit�s et vacations des contractuels    


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

 
[Lien vers le bulletins des CEV](Bases/R�glementation/lignes.contractuels.et.vacations.csv)   
[Lien vers la base de donn�es Matricules des CEV](Bases/R�glementation/matricules.contractuels.et.vacations.csv)  
[Lien vers la base de donn�es Cumul r�gime indemnitaire et vacations de CEV](Bases/R�glementation/RI.et.vacations.csv)  
[Lien vers la base de donn�es Lignes de traitement indiciaire pour CEV](Bases/R�glementation/traitement.et.vacations.csv)  
 




## 5.4 Contr�le sur les indemnit�s IAT et IFTS      


```
## Error in `:=`(ifts.logical = grepl(expression.r�g.ifts, Paie$Libell�, : Check that is.data.table(DT) == TRUE. Otherwise, := and `:=`(...) are defined for use in j, once only and in particular ways. See help(":=").
```

```
## Error in unique(Paie[ifts.logical == TRUE][, Code]): object 'ifts.logical' not found
```

Il n'a pas �t� possible d'identifier les IAT par expression r�guli�re.


 
&nbsp;*Tableau 55*   
     

Tests IAT/IFTS sans r�sultat positif.

  
[Codes IFTS retenus](Bases/R�glementation/codes.ifts.csv)   
[Lien vers la base de donn�es cumuls iat/ifts](Bases/R�glementation/personnels.iat.ifts.csv)    

### Contr�le sur les IFTS pour cat�gories B et contractuels



 


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


[Lien vers la base de donn�es Lignes IFTS pour contractuels](Bases/R�glementation/ifts.et.contractuel.csv)    
[Lien vers la base de donn�es Lignes IFTS pour IB < 380](Bases/R�glementation/lignes.ifts.anormales.csv)     

**Nota :**
IB < 380 : fonctionnaire percevant un indice brut inf�rieur � 380





## 5.5 Contr�le de la prime de fonctions et de r�sultats (PFR)   
  


```
## Error in `:=`(pfr.logical, grepl(expression.r�g.pfr, Paie$Libell�, ignore.case = TRUE, : Check that is.data.table(DT) == TRUE. Otherwise, := and `:=`(...) are defined for use in j, once only and in particular ways. See help(":=").
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

  
[Lien vers la base de donn�es cumuls pfr/ifts](Bases/R�glementation/personnels.pfr.ifts.csv)    



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
     


| Adm. g�n�ral | Adm. HC |  Adm.  | Direct./Attach� princ. | Secr. mairie/Attach� |
|:------------:|:-------:|:------:|:----------------------:|:--------------------:|
|    58 800    | 55 200  | 49 800 |         25 800         |        20 100        |


Les plafonds annuels de la PFR de sont pas d�pass�s.

 
&nbsp;*Tableau 59* : Valeurs de l'agr�gat (PFR ou IFTS) pour les b�n�ficiaires de la PFR   
         


Aucun b�n�ficiaire de la PFR d�tect�.

 
&nbsp;*Tableau 60* : Variations de l'agr�gat (PFR ou IFTS) pour les b�n�ficiaires de la PFR
         


Aucun tableau de variation.

        
  
[Lien vers la base de donn�es agr�gat PFR-IFTS](Bases/R�mun�rations/b�n�ficiaires.PFR.csv)    

  
[Lien vers la base de donn�es variations agr�gat PFR-IFTS](Bases/R�mun�rations/b�n�ficiaires.PFR.Variation.csv)    





## 5.6 Contr�le sur les heures suppl�mentaires

Le seuil de 180 heures suppl�mentaires maximum est d�pass� par  59  agents.
 Le seuil de 220 heures suppl�mentaires maximum est d�pass� par  59  agents.

```
## Error in eval(expr, envir, enclos): object 'Heures.Sup.' not found
```


 
&nbsp;*Tableau 61*   
   


| Nombre de lignes HS en exc�s | Nombre de lignes IHTS anormales |
|:----------------------------:|:-------------------------------:|
|             303              |               NA                |


[Lien vers la base de donn�es Heures suppl�mentaires en exc�s du seuil de 25h/mois](Bases/R�glementation/HS.sup.25.csv)     
[Lien vers la base de donn�es cumuls en exc�s des seuils annuels](Bases/R�glementation/D�passement.seuil.180h.csv)    
[Lien vers la base de donn�es IHTS anormales](Bases/R�glementation/ihts.anormales.csv)      

**Nota :**   
HS en exc�s : au-del� de 25 heures par mois     
IHTS anormales : non attribu�es � des fonctionnaires de cat�gorie B ou C.     




## 5.7 Contr�le sur les indemnit�s des �lus
  



  


|Matricule |Nom | Ann�e|Emploi               | Indemnit�s | Autres |   Total |
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

[Lien vers la base de donn�es R�mun�rations des �lus](Bases/R�glementation/r�mun�rations.�lu.csv)




## 5.8 Lien avec le compte de gestion


```
## Error in Type %chin% c("T", "I", "R", "IR", "S", "A", "AC"): object 'Type' not found
```

```
## Error in sum(Total, na.rm = TRUE): invalid 'type' (character) of argument
```

 
Cumul des lignes de paie par exercice et cat�gorie de ligne de paie   
 


Tableau  62  Ann�e  2013

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 334 263,04|
|             Rappels|   244 938,75|
|       R�m. diverses|    27 860,93|
| Suppl�ment familial|   146 331,98|
|         Traitements| 9 127 148,03|

Tableau  63  Ann�e  2014

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 334 245,36|
|             Rappels|   165 096,19|
|       R�m. diverses|    29 337,19|
| Suppl�ment familial|   148 838,36|
|         Traitements| 9 315 116,31|

Tableau  64  Ann�e  2015

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 325 516,15|
|             Rappels|   156 348,33|
|       R�m. diverses|    17 323,77|
| Suppl�ment familial|   140 162,84|
|         Traitements| 9 070 866,33|

Tableau  65  Ann�e  2016

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 532 916,11|
|             Rappels|   156 613,18|
|       R�m. diverses|    35 882,81|
| Suppl�ment familial|   138 565,19|
|         Traitements| 8 861 048,17|

Tableau  66  Ann�e  2017

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 586 973,59|
|             Rappels|   198 605,80|
|       R�m. diverses|    51 331,04|
| Suppl�ment familial|   141 995,23|
|         Traitements| 9 069 382,34|

 
[Lien vers la base d�taill�e des cumuls des lignes de paie](Bases/R�glementation/cumul.lignes.paie.csv)
 
[Lien vers la base agr�g�e des cumuls des lignes de paie](Bases/R�glementation/cumul.total.lignes.paie.csv)
 
 
*Avertissement : les rappels comprennent �galement les rappels de cotisations et d�ductions diverses.*    
  




## 5.9 Contr�le du suppl�ment familial de traitement   
 


```
## Error in eval(expr, envir, enclos): object 'NbEnfants' not found
```

```
## Error in eval(expr, envir, enclos): object 'Paie.sans.enfant' not found
```


Pour les agents n'ayant pas d'enfant signal� en base, il a �t� d�tect� 187 bulletins de paie pr�sentant un paiement du SFT apparemment anormal.

 
[Lien vers la base des paiements de SFT � agents sans enfant signal�](Bases/R�glementation/Paie.sans.enfant.r�duit.csv)
 


```
## Error in eval(expr, envir, enclos): object 'NbEnfants' not found
```

```
## Error in eval(expr, envir, enclos): object 'Paie.enfants' not found
```


Pour les agents ayant au moins un enfant, il a �t� d�tect� 49 bulletins de paie pr�sentant un �cart de paiement du SFT sup�rieur � 1 euro.

 
[Lien vers la base des �carts de paiement sur SFT](Bases/R�glementation/controle.sft.csv)
 



# Annexe
## Liens compl�mentaires

[Lien vers le fichier des personnels](Bases/Effectifs/Cat�gories des personnels.csv)
 
## Fiabilit� du traitement statistique   
*Doublons*      

Attention : Alta�r a d�tect� des lignes dupliqu�es alors qu'aucun retraitement des lignes dupliqu�es n'est pr�vu par d�faut.

 



 
*Tests de fiabilit� sur le renseignement des heures et des quotit�s*    
  



    

Nombre de bulletins :  32 107

    

Les heures de travail ont �t� redress�es avec la m�thode  des quotit�s.

   

 Nombre de bulletins de paie redress�s : 525

   

 Pourcentage de redressements : 1,64 % des bulletins de paie.

 


Pourcentage d'heures renseign�es (apr�s redressement �ventuel): 95,4 %

  


Pourcentage de quotit�s renseign�es : 99,4 %

  


Nombre de bulletins � heures et quotit�s :  30579 [ 95,2 %]

  


Nombre de bulletins � heures sans quotit�s :  53 [ 0,2 %]

  


Nombre de bulletins � quotit�s sans heures :  1334 [ 4,2 %]

  


Nombre de bulletins apparemment inactifs :  141 [ 0,4 %]

  



 

Nombre de bulletins de paie de salaires vers�s pour un champ Heures = 0 :  1 398 [ 4,4 %]

  



  
[Lien vers la base de donn�es des salaires vers�s pour Heures=0](Bases/Fiabilit�/base.heures.nulles.salaire.nonnull.csv)   
[Lien vers la base de donn�es des salaires vers�s � quotit� ind�finie](Bases/Fiabilit�/base.quotit�.ind�finie.salaire.nonnull.csv)   

# Tableau des personnels : renseigner la cat�gorie

Utiliser les codes : A, B, C, ELU, AUTRES

En cas de changement de cat�gorie en cours de p�riode, utiliser la cat�gorie AUTRES
Cela peut conduire � modifier manuellement le fichier Cat�gories des personnels.csv



Non g�n�r�  [anonymisation]


