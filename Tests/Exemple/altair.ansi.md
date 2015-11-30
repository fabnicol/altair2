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




<p class = "centered"><b>Exercices 2009 � 2013 </b></p>
<p class = "author">Fabrice Nicol</h1>




mar. 01 d�c. 2015
   


La dur�e du travail prise en compte dans la base de donn�es est de  151,67  h par mois.

# 1. Statistiques de population

### 1.1 Effectifs



 



&nbsp;*Tableau 1*   


|                                                             | 2009  | 2010  | 2011  | 2012  | 2013  |
|:------------------------------------------------------------|:-----:|:-----:|:-----:|:-----:|:-----:|
|Total effectifs (a)                                          | 709,0 | 769,0 | 698,0 | 714,0 | 703,0 |
|&nbsp;&nbsp;&nbsp;dont pr�sents 12 mois                      | 465,0 | 462,0 | 444,0 | 428,0 | 433,0 |
|&nbsp;&nbsp;&nbsp;dont fonctionnaires (b)                    | 438,0 | 431,0 | 423,0 | 427,0 | 417,0 |
|&nbsp;&nbsp;&nbsp;dont fonct. pr�sents 12 mois               | 400,0 | 403,0 | 402,0 | 384,0 | 391,0 |
|&nbsp;&nbsp;&nbsp;dont non titulaires                        | 60,0  | 54,0  | 56,0  | 57,0  | 73,0  |
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
   


|  Statistique  | �ge des personnels <br>au 31/12/2009 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  16                  |          |
| 1er quartile  |                  35                  |          |
|    M�diane    |                  43                  |          |
|    Moyenne    |                42,45                 |   542    |
| 3�me quartile |                  51                  |          |
|    Maximum    |                  66                  |          |

 

![plot of chunk unnamed-chunk-9](figure/unnamed-chunk-9-1.png) 

 
&nbsp;*Tableau 3*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2013 | Effectif |
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
   


|  Statistique  | �ge des personnels non titulaires<br>au 31/12/2009 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         17                         |          |
| 1er quartile  |                         24                         |          |
|    M�diane    |                        37,5                        |          |
|    Moyenne    |                       36,97                        |   100    |
| 3�me quartile |                       46,25                        |          |
|    Maximum    |                         66                         |          |

 

![plot of chunk unnamed-chunk-13](figure/unnamed-chunk-13-1.png) 

 
&nbsp;*Tableau 5*   
   


|  Statistique  | �ge des personnels non titulaires<br>au 31/12/2013 | Effectif |
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
   


|  Statistique  | �ge des personnels <br>au 31/12/2009 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  24                  |          |
| 1er quartile  |                  38                  |          |
|    M�diane    |                  45                  |          |
|    Moyenne    |                44,18                 |   410    |
| 3�me quartile |                  51                  |          |
|    Maximum    |                  62                  |          |

 


![plot of chunk unnamed-chunk-16](figure/unnamed-chunk-16-1.png) 

 
&nbsp;*Tableau 7*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2013 | Effectif |
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
   


|  Statistique  | �ge des personnels <br>au 31/12/2009 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  17                  |          |
| 1er quartile  |                  36                  |          |
|    M�diane    |                 43,5                 |          |
|    Moyenne    |                42,76                 |   510    |
| 3�me quartile |                  51                  |          |
|    Maximum    |                  66                  |          |

 

![plot of chunk unnamed-chunk-20](figure/unnamed-chunk-20-1.png) 

&nbsp;*Tableau 9*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2013 | Effectif |
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

**Personnels en fonction (hors �lus) des exercices 2009 � 2013 inclus :**

 
&nbsp;*Tableau 10*   
   


| Plus de 2 ans | Moins de 2 ans | Moins d'un an | Moins de six mois |
|:-------------:|:--------------:|:-------------:|:-----------------:|
|      472      |      146       |      86       |        24         |



![plot of chunk unnamed-chunk-24](figure/unnamed-chunk-24-1.png) 


**Effectifs (hors �lus)**   




 
&nbsp;*Tableau 11*   
   


|                  | 2009 | 2010 | 2011 | 2012 | 2013 |
|:-----------------|:----:|:----:|:----:|:----:|:----:|
|Plus de deux ans  | 439  | 451  | 451  | 431  | 411  |
|Moins de deux ans |  38  |  27  |  24  |  42  |  68  |
|Total             | 477  | 478  | 475  | 473  | 479  |



**Nota :**
*Personnels en place : ayant servi au moins deux ann�es cons�cutives pendant la p�riode.*     
*Plus/moins de deux ans : plus/mois de 730 jours sur la p�riode sous revue.*   



  
  



   
# 2. R�mun�rations brutes : analyse pour l'exercice 2009    
   
## 2.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des r�mun�rations brutes pour l'exercice 2009
 
*Personnels (hors �lus)*     
 
&nbsp;*Tableau 12*   
   


|           Agr�gats           |   k&euro;    |
|------------------------------|--------------|
|   Brut annuel (bulletins)    | 10 647 917,6 |
| Brut annuel (lignes), dont : | 10 433 576,9 |
|         \ \ Primes :         | 1 508 455,9  |
|   \ \ Autres r�mun�rations   |   28 394,4   |
|     Part de primes en %      |     14,2     |

 
**D�finitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lu*          : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
 
**Tests de coh�rence**

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :  
 
 
&nbsp;*Tableau 13*   
   


|     Agr�gats      |   k&euro;    |
|-------------------|--------------|
| Bulletins de paie | 10 647 917,6 |
|  Lignes de paie   | 10 433 576,9 |
|    Diff�rence     |  214 340,8   |


� comparer aux soldes des comptes 641 et 648 du compte de gestion.

  
## 2.2 Masse salariale brute des fonctionnaires

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*  


![plot of chunk unnamed-chunk-127](figure/unnamed-chunk-127-1.png) ![plot of chunk unnamed-chunk-127](figure/unnamed-chunk-127-2.png) ![plot of chunk unnamed-chunk-127](figure/unnamed-chunk-127-3.png) ![plot of chunk unnamed-chunk-127](figure/unnamed-chunk-127-4.png) ![plot of chunk unnamed-chunk-127](figure/unnamed-chunk-127-5.png) ![plot of chunk unnamed-chunk-127](figure/unnamed-chunk-127-6.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio r�mun�ration / quotit�*  
   
**Effectif : 433**

**Tests de coh�rence**



Somme des r�mun�rations brutes vers�es aux personnels titulaires et stagiaires :

 
&nbsp;*Tableau 14*   
   


|           Agr�gats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 9 264 307,7 |
| Brut annuel (lignes), dont : | 9 173 037,4 |
|         \ \ Primes :         | 1 419 599,3 |
|   \ \ Autres r�mun�rations   |  24 890,5   |
|     Part de primes en %      |    15,3     |


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
| Bulletins de paie | 9 264 307,7 |
|  Lignes de paie   | 9 173 037,4 |
|    Diff�rence     |  91 270,3   |


A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.

**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2009**     
 
&nbsp;*Tableau 16*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres r�mun�rations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 2 855|    778,3|                 0,00|         |
|  1er quartile|                16 203|  2 018,8|                 0,00|         |
|       M�diane|                17 446|  2 821,5|                 0,00|         |
|       Moyenne|                18 795|  3 441,3|                60,41|      412|
| 3�me quartile|                20 745|  4 179,9|                15,25|         |
|       Maximum|                52 990| 25 332,3|             2 876,80|         |

 
&nbsp;*Tableau 17*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la r�m. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     3 640|      3 820|                 0|                        4,794|         |
|  1er quartile|                    18 432|     18 631|            19 551|                       10,600|         |
|       M�diane|                    20 398|     20 634|            21 722|                       13,639|         |
|       Moyenne|                    22 237|     22 445|            23 674|                       14,644|      412|
| 3�me quartile|                    24 278|     24 571|            25 821|                       17,557|         |
|       Maximum|                    70 613|     70 580|            70 580|                       36,630|         |

  
*Hors vacataires identifi�s, assistantes maternelles, �lus locaux et pour les postes actifs non annexes*  

**Cat�gorie A**




 
&nbsp;*Tableau 18*   
   


|   Statistique| Traitement indiciaire| Primes| Autres r�mun�rations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                10 229|  3 254|                0,000|
|  1er quartile|                28 835|  5 805|                0,000|
|       M�diane|                33 703|  7 884|                7,625|
|       Moyenne|                34 174|  9 492|              176,649|
| 3�me quartile|                43 191| 10 020|              202,838|
|       Maximum|                52 990| 25 332|            1 278,990|


 
&nbsp;*Tableau 19*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              14 052|                        0|                                13,66|
|  1er quartile|              36 996|                   38 281|                                17,02|
|       M�diane|              41 300|                   42 679|                                18,63|
|       Moyenne|              43 687|                   43 267|                                21,07|
| 3�me quartile|              52 350|                   52 350|                                25,04|
|       Maximum|              70 580|                   70 580|                                35,89|


**Effectif : 24**  

**Cat�gorie B**

 
&nbsp;*Tableau 20*   
   


|   Statistique| Traitement indiciaire| Primes| Autres r�mun�rations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                 9 150|  2 825|                 0,00|
|  1er quartile|                19 793|  4 256|                 0,00|
|       M�diane|                22 136|  4 878|                 0,00|
|       Moyenne|                22 248|  5 428|                68,19|
| 3�me quartile|                24 834|  6 496|                38,40|
|       Maximum|                29 384| 10 775|               633,74|


 
&nbsp;*Tableau 21*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              12 838|                        0|                                12,76|
|  1er quartile|              24 365|                   26 071|                                15,97|
|       M�diane|              27 429|                   28 321|                                18,93|
|       Moyenne|              27 641|                   28 393|                                19,63|
| 3�me quartile|              30 558|                   31 031|                                22,92|
|       Maximum|              36 959|                   36 959|                                31,03|


**Effectif : 53**

**Cat�gorie C**

 
&nbsp;*Tableau 22*   
   


|   Statistique| Traitement indiciaire|  Primes| Autres r�mun�rations|
|-------------:|---------------------:|-------:|--------------------:|
|       Minimum|                 2 855|   778,3|                 0,00|
|  1er quartile|                16 132| 2 018,1|                 0,00|
|       M�diane|                16 976| 2 208,3|                 0,00|
|       Moyenne|                17 147| 2 693,5|                50,86|
| 3�me quartile|                18 984| 3 094,6|                 0,00|
|       Maximum|                25 843| 7 199,8|             2 876,80|


 
&nbsp;*Tableau 23*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               3 820|                        0|                                4,794|
|  1er quartile|              18 312|                   19 245|                               10,334|
|       M�diane|              19 860|                   20 645|                               11,661|
|       Moyenne|              20 101|                   21 524|                               13,396|
| 3�me quartile|              22 343|                   23 439|                               15,288|
|       Maximum|              33 991|                   40 925|                               36,630|

**Effectif : 335**


######      <br>

## 2.3 Contractuels, vacataires et stagiaires inclus   
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   


![plot of chunk unnamed-chunk-140](figure/unnamed-chunk-140-1.png) 

  
**Nota :**
Ne sont retenues que les r�mun�rations sup�rieures � 1 000 k&euro;.
Les �lus ne sont pas pris en compte.
  

![plot of chunk unnamed-chunk-141](figure/unnamed-chunk-141-1.png) 





  
**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2009**     
  
 
&nbsp;*Tableau 24*   
   


|   Statistique|  Primes| Autres r�mun�rations| Effectif|
|-------------:|-------:|--------------------:|--------:|
|       Minimum|   -79,6|                 0,00|         |
|  1er quartile|     0,0|                 0,00|         |
|       M�diane|   497,7|                 0,00|         |
|       Moyenne| 1 108,2|                43,65|       65|
| 3�me quartile| 1 485,5|                 0,00|         |
|       Maximum| 6 358,0|             1 054,40|         |

 
&nbsp;*Tableau 25*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               3 443|                        0|         |
|  1er quartile|               7 803|                   16 655|         |
|       M�diane|              12 239|                   18 115|         |
|       Moyenne|              14 323|                   19 330|       65|
| 3�me quartile|              17 372|                   20 234|         |
|       Maximum|              52 654|                   52 654|         |






  
  



   
# 3. R�mun�rations brutes : analyse pour l'exercice 2013    
   
## 3.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des r�mun�rations brutes pour l'exercice 2013
 
*Personnels (hors �lus)*     
 
&nbsp;*Tableau 26*   
   


|           Agr�gats           |   k&euro;    |
|------------------------------|--------------|
|   Brut annuel (bulletins)    | 10 785 759,3 |
| Brut annuel (lignes), dont : | 10 657 373,2 |
|         \ \ Primes :         | 1 780 299,9  |
|   \ \ Autres r�mun�rations   |   51 331,0   |
|     Part de primes en %      |     16,5     |

 
**D�finitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lu*          : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
 
**Tests de coh�rence**

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :  
 
 
&nbsp;*Tableau 27*   
   


|     Agr�gats      |   k&euro;    |
|-------------------|--------------|
| Bulletins de paie | 10 785 759,3 |
|  Lignes de paie   | 10 657 373,2 |
|    Diff�rence     |  128 386,1   |


� comparer aux soldes des comptes 641 et 648 du compte de gestion.

  
## 3.2 Masse salariale brute des fonctionnaires

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*  


![plot of chunk unnamed-chunk-150](figure/unnamed-chunk-150-1.png) ![plot of chunk unnamed-chunk-150](figure/unnamed-chunk-150-2.png) ![plot of chunk unnamed-chunk-150](figure/unnamed-chunk-150-3.png) ![plot of chunk unnamed-chunk-150](figure/unnamed-chunk-150-4.png) ![plot of chunk unnamed-chunk-150](figure/unnamed-chunk-150-5.png) ![plot of chunk unnamed-chunk-150](figure/unnamed-chunk-150-6.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio r�mun�ration / quotit�*  
   
**Effectif : 411**

**Tests de coh�rence**



Somme des r�mun�rations brutes vers�es aux personnels titulaires et stagiaires :

 
&nbsp;*Tableau 28*   
   


|           Agr�gats           |   k&euro;   |
|------------------------------|-------------|
|   Brut annuel (bulletins)    | 9 511 343,6 |
| Brut annuel (lignes), dont : | 9 438 778,3 |
|         \ \ Primes :         | 1 662 312,8 |
|   \ \ Autres r�mun�rations   |  48 383,4   |
|     Part de primes en %      |    17,5     |


**D�finitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lus*         : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   

**Tests de coh�rence**

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :

 
&nbsp;*Tableau 29*   
   


|     Agr�gats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 9 511 343,6 |
|  Lignes de paie   | 9 438 778,3 |
|    Diff�rence     |  72 565,4   |


A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.

**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2013**     
 
&nbsp;*Tableau 30*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres r�mun�rations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 2 880|    539,3|                  0,0|         |
|  1er quartile|                17 396|  3 022,3|                  0,0|         |
|       M�diane|                18 415|  3 384,5|                  0,0|         |
|       Moyenne|                19 619|  4 196,6|                122,2|      396|
| 3�me quartile|                21 846|  4 913,2|                170,2|         |
|       Maximum|                47 256| 25 307,4|              3 805,3|         |

 
&nbsp;*Tableau 31*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la r�m. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     3 419|      3 774|                 0|                        9,463|         |
|  1er quartile|                    20 612|     20 686|            21 363|                       14,172|         |
|       M�diane|                    22 340|     22 607|            23 878|                       15,675|         |
|       Moyenne|                    23 816|     24 008|            25 450|                       17,296|      396|
| 3�me quartile|                    26 223|     26 791|            27 953|                       19,717|         |
|       Maximum|                    70 925|     70 925|            70 925|                       51,003|         |

  
*Hors vacataires identifi�s, assistantes maternelles, �lus locaux et pour les postes actifs non annexes*  

**Cat�gorie A**




 
&nbsp;*Tableau 32*   
   


|   Statistique| Traitement indiciaire| Primes| Autres r�mun�rations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                18 870|  4 945|                  0,0|
|  1er quartile|                26 300|  7 123|                  0,0|
|       M�diane|                29 699|  8 449|                  0,0|
|       Moyenne|                31 911|  9 939|                 58,5|
| 3�me quartile|                38 659| 10 346|                  0,0|
|       Maximum|                47 256| 25 307|                297,0|


 
&nbsp;*Tableau 33*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              24 301|                   27 989|                                17,24|
|  1er quartile|              35 104|                   35 578|                                19,67|
|       M�diane|              37 354|                   39 231|                                22,01|
|       Moyenne|              41 395|                   43 144|                                23,40|
| 3�me quartile|              48 895|                   48 895|                                25,84|
|       Maximum|              70 925|                   70 925|                                35,68|


**Effectif : 18**  

**Cat�gorie B**

 
&nbsp;*Tableau 34*   
   


|   Statistique| Traitement indiciaire| Primes| Autres r�mun�rations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                10 705|  3 075|                  0,0|
|  1er quartile|                19 942|  5 033|                  0,0|
|       M�diane|                23 190|  5 409|                  0,0|
|       Moyenne|                22 860|  5 826|                155,8|
| 3�me quartile|                25 497|  6 319|                210,0|
|       Maximum|                33 338| 12 899|              3 805,3|


 
&nbsp;*Tableau 35*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              13 951|                        0|                                13,12|
|  1er quartile|              25 784|                   27 601|                                16,86|
|       M�diane|              29 013|                   30 755|                                19,53|
|       Moyenne|              29 029|                   30 609|                                20,36|
| 3�me quartile|              32 904|                   33 883|                                22,44|
|       Maximum|              42 627|                   42 627|                                51,00|


**Effectif : 62**

**Cat�gorie C**

 
&nbsp;*Tableau 36*   
   


|   Statistique| Traitement indiciaire|  Primes| Autres r�mun�rations|
|-------------:|---------------------:|-------:|--------------------:|
|       Minimum|                 2 880|   539,3|                  0,0|
|  1er quartile|                17 385| 3 006,8|                  0,0|
|       M�diane|                18 086| 3 200,1|                  0,0|
|       Moyenne|                18 281| 3 546,4|                118,9|
| 3�me quartile|                20 297| 3 950,9|                139,2|
|       Maximum|                26 147| 8 435,4|              1 828,4|


 
&nbsp;*Tableau 37*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               3 774|                        0|                                9,463|
|  1er quartile|              20 435|                   21 119|                               13,941|
|       M�diane|              21 615|                   22 572|                               14,775|
|       Moyenne|              22 026|                   23 411|                               16,337|
| 3�me quartile|              24 710|                   25 421|                               18,014|
|       Maximum|              37 515|                   48 958|                               43,177|

**Effectif : 315**


######      <br>

## 3.3 Contractuels, vacataires et stagiaires inclus   
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   


![plot of chunk unnamed-chunk-163](figure/unnamed-chunk-163-1.png) 

  
**Nota :**
Ne sont retenues que les r�mun�rations sup�rieures � 1 000 k&euro;.
Les �lus ne sont pas pris en compte.
  

![plot of chunk unnamed-chunk-164](figure/unnamed-chunk-164-1.png) 





  
**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2013**     
  
 
&nbsp;*Tableau 38*   
   


|   Statistique|    Primes| Autres r�mun�rations| Effectif|
|-------------:|---------:|--------------------:|--------:|
|       Minimum|    -22,25|                 0,00|         |
|  1er quartile|      0,00|                 0,00|         |
|       M�diane|    264,70|                 0,00|         |
|       Moyenne|  1 340,40|                35,09|       84|
| 3�me quartile|  1 087,85|                 0,00|         |
|       Maximum| 17 494,04|             1 100,00|         |

 
&nbsp;*Tableau 39*   
   


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
 
&nbsp;*Tableau 40*   
   


| Agr�gat (&euro;) | Salaires bruts 2011 | Salaires bruts 2012 | Salaires bruts 2013 |
|------------------|---------------------|---------------------|---------------------|
|     Ensemble     |      25 908,0       |      26 340,0       |      26 616,0       |
|    Titulaires    |      26 676,0       |      27 108,0       |      27 444,0       |
| Autres salari�s  |      22 836,0       |         NA          |      24 360,0       |

  
**El�ments de la r�mun�ration brute pour les titulaires de la FPT entre 2010 et 2012**      
  
 
&nbsp;*Tableau 41*   
   


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
  




# 4. R�mun�rations nettes : �volutions sur la p�riode 2009 - 2013    

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

## 4.2 Evolutions des r�mun�rations nettes sur la p�riode 2009 - 2013   

### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors �lus)





   
**Salaire net moyen par t�te (SMPT net) en EQTP, hors �lus**         
      
 
&nbsp;*Tableau 42*   
   


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
 
&nbsp;*Tableau 43*   
   


| Ann�e | Noria EQTP (&euro;) | En % de la MS N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-----------------:|:------------------:|:-------------------------:|
| 2010  |      -3 563,2       |      -0,593       |         15         |            3,3            |
| 2011  |       -304,68       |     -0,0 569      |         17         |            3,7            |
| 2012  |      -2 134,6       |       -0,54       |         23         |            5,3            |
| 2013  |      -3 399,9       |      -0,651       |         18         |             4             |


*MS N-1 : masse salariale nette de l'ann�e n-1.*   
      
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) en EQTP**         
      
 
&nbsp;*Tableau 44*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          7 081|
|  1er quartile|         15 750|
|       M�diane|         17 410|
|       Moyenne|         19 180|
| 3�me quartile|         20 790|
|       Maximum|         56 150|



|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|          4 794|
|  1er quartile|         16 990|
|       M�diane|         18 670|
|       Moyenne|         20 050|
| 3�me quartile|         22 030|
|       Maximum|         54 530|

 
*Nota :*  La population retenue est constitu�e des agents qui :   
&nbsp;&nbsp;- ne font pas partie des 2 centiles extr�maux   
&nbsp;&nbsp;- sont au moins pr�sents 1 jour(s) la premi�re et la derni�re ann�e d'activit�  
Les �lus, vacataires et assistantes maternelles sont retir�s du p�rim�tre.   
Seuls sont pris en compte les agents ayant connu au moins un mois actif et ayant eu, sur l'ann�e, des r�mun�rations non annexes.  
[Compl�ments m�thodologiques](Docs/m�thodologie.pdf)     
     
**Comparaisons source INSEE/DGCL**   

**Salaires annuels moyens 2011 et 2012 en EQTP (hors assistantes maternelles)**   
 
&nbsp;*Tableau 45*       


|     Agr�gat     | Salaires nets 2011 (&euro;) | Salaires nets 2012 (&euro;) | Salaires nets 2013 (&euro;) |
|-----------------|-----------------------------|-----------------------------|-----------------------------|
|    Ensemble     |          21 876,0           |          22 176,0           |          22 224,0           |
|   Titulaires    |          22 632,0           |          22 920,0           |          22 920,0           |
| Autres salari�s |          18 864,0           |             NA              |             NA              |

*Champ : France. Salari�s en �quivalent-temps plein (EQTP) des collectivit�s territoriales (y compris b�n�ficiaires de contrats aid�s, hors assistantes maternelles).*     			



**Distribution des salaires nets annuels en EQTP dans la fonction publique par versant en 2011**   

 
&nbsp;*Tableau 46*   
   


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
 
&nbsp;*Tableau 47*   
   


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
&nbsp;*Tableau 48*   
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2009  |           7 985,2           |               |          19 571           |               |
| 2010  |           7 944,9           |               |          19 473           |               |
| 2011  |           8 001,2           |     2,48      |          19 854           |     6,39      |
| 2012  |           8 218,6           |               |          20 343           |               |
| 2013  |           8 183,2           |               |          20 822           |               |





  
**Effet de noria sur salaires nets et taux de remplacements**       
 
&nbsp;*Tableau 49*   
   


| Ann�e | Noria EQTP (&euro;) | En % de la  MSN N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-------------------:|:------------------:|:-------------------------:|
| 2010  |      -6 634,7       |       -0,471        |         6          |            1,5            |
| 2011  |       13 280        |        0,842        |         5          |            1,3            |
| 2012  |        5 704        |        0,491        |         7          |            1,9            |
| 2013  |       -12 989       |       -0,354        |         2          |           0,61            |

    
*MS N-1 : masse salariale nette de l'ann�e n-1.*   
      
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) en EQTP**         
 
&nbsp;*Tableau 50*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          7 081|
|  1er quartile|         15 980|
|       M�diane|         17 790|
|       Moyenne|         19 570|
| 3�me quartile|         21 210|
|       Maximum|         56 150|



|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|          8 883|
|  1er quartile|         17 480|
|       M�diane|         19 420|
|       Moyenne|         20 820|
| 3�me quartile|         22 730|
|       Maximum|         54 530|

   



  
**Evolution du SMPT net des titulaires � temps complet**     
  
 
&nbsp;*Tableau 51*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | SMPT (&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:-------------:|:-------------:|
| 2009  |               5 703,4               |               |    19 667     |               |
| 2010  |                6 052                |               |    19 974     |               |
| 2011  |               5 934,6               |     1,35      |    20 049     |     5,34      |
| 2012  |               5 356,5               |               |    20 290     |               |
| 2013  |               5 780,4               |               |    20 718     |               |

   
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) des titulaires � temps complet**         
      
 
&nbsp;*Tableau 52*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          7 081|
|  1er quartile|         16 050|
|       M�diane|         17 800|
|       Moyenne|         19 670|
| 3�me quartile|         20 800|
|       Maximum|         56 150|

  


|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|          8 883|
|  1er quartile|         17 400|
|       M�diane|         19 280|
|       Moyenne|         20 720|
| 3�me quartile|         22 210|
|       Maximum|         54 530|


[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)


## 4.3 Glissement vieillesse-technicit� (GVT)   

### 4.3.1 Ensemble des personnels   
  
*Cette section est consacr�e � la r�mun�ration moyenne des personnes en place (RMPP), d�finies comme pr�sentes deux ann�es enti�res cons�cutives avec la m�me quotit�*   
*L'�volution de la RMPP permet d'�tudier le glissement vieillesse-technicit� "positif", � effectifs constants sur deux ann�es*      
*Le GVT positif est d� aux mesures statutaires et individuelles, � l'avancement et aux changements d'activit�*  

![plot of chunk unnamed-chunk-51](figure/unnamed-chunk-51-1.png) 






  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 53*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2010  |               7 240,1               |               |        19 890        |               |
| 2011  |               7 350,9               |    -0,551     |        19 867        |     4,93      |
| 2012  |               6 955,7               |               |        20 398        |               |
| 2013  |               7 200,2               |               |        20 870        |               |

   
**Distribution et variation sur la p�riode de la r�mun�ration nette des personnes en place**                
 
&nbsp;*Tableau 54*   
   



 


|   Statistique| Premi�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          7 081|         |
|  1er quartile|         15 820|         |
|       M�diane|         17 430|         |
|       Moyenne|         19 130|      428|
| 3�me quartile|         20 630|         |
|       Maximum|         56 150|         |

 
&nbsp;*Tableau 55*   
   


|   Statistique| Derni�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          7 137|         |
|  1er quartile|         17 200|         |
|       M�diane|         19 000|         |
|       Moyenne|         20 350|      427|
| 3�me quartile|         22 140|         |
|       Maximum|         54 530|         |


*Variation individuelle de r�mun�ration nette en EQTP pour les personnels pr�sents la premi�re et la derni�re ann�e*   
 
&nbsp;*Tableau 56*   
   


|   Statistique| Variation normalis�e (%)| Variation annuelle moyenne normalis�e (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -47,174|                                   -14,747|         |
|  1er quartile|                    5,612|                                     1,374|         |
|       M�diane|                   10,352|                                     2,493|         |
|       Moyenne|                   10,769|                                     2,462|      369|
| 3�me quartile|                   16,157|                                     3,815|         |
|       Maximum|                   45,049|                                     9,743|         |

   
### 4.3.2 Titulaires et stagiaires     
  



  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 57*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2010  |               6 896,8               |               |        19 818        |               |
| 2011  |               7 009,9               |     0,365     |        19 858        |     4,89      |
| 2012  |               6 728,9               |               |        20 329        |               |
| 2013  |                6 922                |               |        20 787        |               |

   
**Distribution et variation sur la p�riode de la r�mun�ration nette des fonctionnaires en place**                
 
&nbsp;*Tableau 58*   
   
 


|   Statistique| Premi�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          7 081|         |
|  1er quartile|         15 940|         |
|       M�diane|         17 580|         |
|       Moyenne|         19 280|      395|
| 3�me quartile|         20 750|         |
|       Maximum|         56 150|         |

 
&nbsp;*Tableau 59*   
   


|   Statistique| Derni�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          4 743|         |
|  1er quartile|         17 370|         |
|       M�diane|         19 370|         |
|       Moyenne|         21 000|      439|
| 3�me quartile|         22 980|         |
|       Maximum|         63 190|         |


*Variation individuelle de r�mun�ration nette en EQTP pour les personnels pr�sents la premi�re et la derni�re ann�e*   
 
&nbsp;*Tableau 60*   
   


|   Statistique| Variation normalis�e (%)| Variation annuelle moyenne normalis�e (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -47,174|                                   -14,747|         |
|  1er quartile|                    5,778|                                     1,414|         |
|       M�diane|                   10,398|                                     2,504|         |
|       Moyenne|                   10,870|                                     2,486|      355|
| 3�me quartile|                   16,151|                                     3,814|         |
|       Maximum|                   45,049|                                     9,743|         |



[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)

**Nota**   
*Personnes en place :* en fonction au moins deux ann�es cons�cutives sur la p�riode 2009 � 2013    
*Variation sur la p�riode d'activit� :* entre l'arriv�e et le d�part de la personne      
*Variation normalis�e :* conforme � la d�finition INSEE (pr�sente en d�but et en fin de p�riode avec la m�me quotit�)  
 
**Commentaire**       
Les diff�rences �ventuelles constat�es entre l'�volution de la RMPP au tableau 58 sont dues soit � l'effet de noria soit � l'effet p�rim�tre.    
     
[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)


### 4.4 Comparaisons avec la situation nationale des r�mun�rations   
 
**�volution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    
 
&nbsp;*Tableau 61*   
   


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
  
 
&nbsp;*Tableau 62*   
   


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
## 5.1 Contr�le des NBI et primes informatiques   
  
**Dans cette partie, l'ensemble de la base de paie est �tudi�.**  
Les agents non actifs ou dont le poste est annexe sont r�introduits dans le p�rim�tre.   

Il existe  1 non titulaire   percevant une NBI.

Primes informatiques potentielles : PRIME FONCTION INFORMATIQUE
 
&nbsp;*Tableau 63*   
   


| Nombre de lignes NBI pour non titulaires | Nombre de b�n�ficiaires de PFI |
|:----------------------------------------:|:------------------------------:|
|                    24                    |              163               |

  
[Lien vers la base de donn�es NBI aux non titulaires](Bases/R�glementation/NBI.aux.non.titulaires.csv)   
[Lien vers la base de donn�es Primes informatiques](Bases/R�glementation/personnels.prime.informatique.csv)   
  
**Nota :**   
NBI: nouvelle bonification indiciaire   
PFI: prime de fonctions informatiques   




 
&nbsp;*Tableau 64*   
   


| R�mun�rations de NBI anormales, par agent et par exercice | Montants correspondants |
|:---------------------------------------------------------:|:-----------------------:|
|                            82                             |         37 139          |

  
[Lien vers la base de donn�es NBI anormales](Bases/Fiabilit�/lignes.nbi.anormales.csv)   
  
**Nota :**   
*Est consid�r� comme manifestement anormal un total annuel de r�mun�rations NBI correspondant � un point d'indice net mensuel inf�rieur � 4 euros ou sup�rieur � 6 euros.*    
*Les rappels ne sont pas pris en compte dans les montants vers�s. Certains �carts peuvent �tre r�gularis�s en les prenant en compte*     
 
&nbsp;*Tableau 65*   
   


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





Il y a  0 fonctionnaire(s) effectuant des vacations pour son propre �tablissement. Les bulletins concern�s sont donn�s en lien.


[Lien vers les matricules des fonctionnaires concern�s](Bases/R�glementation/matricules.fonctionnaires.et.vacations.csv)
[Lien vers les bulletins de paie correspondants](Bases/R�glementation/lignes.fonctionnaires.et.vacations.csv)

## 5.3 Contr�les sur les cumuls traitement indiciaire, indemnit�s et vacations des contractuels    




**Contractuels effectuant des vacations (CEV)**

 
&nbsp;*Tableau 66*   
   


| Nombre de CEV | Nombre de lignes | Nombre de lignes indemnitaires | Nombre de lignes de traitement |
|:-------------:|:----------------:|:------------------------------:|:------------------------------:|
|      94       |      26 307      |              133               |              937               |

 
[Lien vers le bulletins des CEV](Bases/R�glementation/lignes.contractuels.et.vacations.csv)   
[Lien vers la base de donn�es Matricules des CEV](Bases/R�glementation/matricules.contractuels.et.vacations.csv)  
[Lien vers la base de donn�es Cumul r�gime indemnitaire et vacations de CEV](Bases/R�glementation/RI.et.vacations.csv)  
[Lien vers la base de donn�es Lignes de traitement indiciaire pour CEV](Bases/R�glementation/traitement.et.vacations.csv)  
 




## 5.4 Contr�le sur les indemnit�s IAT et IFTS      




 
&nbsp;*Tableau 67*   
     


|   Codes IFTS   |   |
|:--------------:|:-:|
| 3046 3052 401A |   |



| Nombre de personnels percevant IAT et IFTS |   |
|:------------------------------------------:|:-:|
|                     11                     |   |

  
[Codes IFTS retenus](Bases/R�glementation/codes.ifts.csv)   
[Lien vers la base de donn�es cumuls iat/ifts](Bases/R�glementation/personnels.iat.ifts.csv)    

### Contr�le sur les IFTS pour cat�gories B et contractuels



 



 




 
&nbsp;*Tableau 68*   
   


| Nombre de lignes de paie de contractuels percevant des IFTS | Nombre de lignes IFTS pour IB < 380 |
|:-----------------------------------------------------------:|:-----------------------------------:|
|                             67                              |                 18                  |


[Lien vers la base de donn�es Lignes IFTS pour contractuels](Bases/R�glementation/ifts.et.contractuel.csv)    
[Lien vers la base de donn�es Lignes IFTS pour IB < 380](Bases/R�glementation/lignes.ifts.anormales.csv)     

**Nota :**
IB < 380 : fonctionnaire percevant un indice brut inf�rieur � 380





## 5.5 Contr�le de la prime de fonctions et de r�sultats (PFR)   
  




 
&nbsp;*Tableau 69*   
     


| Codes PFR | Agents cumulant PFR et IFTS |
|:---------:|:---------------------------:|
|           |              0              |

  
[Lien vers la base de donn�es cumuls pfr/ifts](Bases/R�glementation/personnels.pfr.ifts.csv)    




 
&nbsp;*Tableau 70 : rappel des plafonds annuels de la PFR*   
     


| Adm. g�n�ral | Adm. HC |  Adm.  | Direct./Attach� princ. | Secr. mairie/Attach� |
|:------------:|:-------:|:------:|:----------------------:|:--------------------:|
|    58 800    | 55 200  | 49 800 |         25 800         |        20 100        |


Les plafonds annuels de la PFR de sont pas d�pass�s.

 
&nbsp;*Tableau 71* : Valeurs de l'agr�gat (PFR ou IFTS) pour les b�n�ficiaires de la PFR   
         


Aucun b�n�ficiaire de la PFR d�tect�.

 
&nbsp;*Tableau 72* : Variations de l'agr�gat (PFR ou IFTS) pour les b�n�ficiaires de la PFR
         


Aucun tableau de variation.

        
  
[Lien vers la base de donn�es agr�gat PFR-IFTS](Bases/R�mun�rations/b�n�ficiaires.PFR.csv)    

  
[Lien vers la base de donn�es variations agr�gat PFR-IFTS](Bases/R�mun�rations/b�n�ficiaires.PFR.Variation.csv)    





## 5.6 Contr�le sur les heures suppl�mentaires

Le seuil de 180 heures suppl�mentaires maximum est d�pass� par  59  agents.
 Le seuil de 220 heures suppl�mentaires maximum est d�pass� par  59  agents.


 
&nbsp;*Tableau 73*   
   


| Nombre de lignes HS en exc�s | Nombre de lignes IHTS anormales |
|:----------------------------:|:-------------------------------:|
|            1 699             |               108               |


[Lien vers la base de donn�es Heures suppl�mentaires en exc�s du seuil de 25h/mois](Bases/R�glementation/HS.sup.25.csv)     
[Lien vers la base de donn�es cumuls en exc�s des seuils annuels](Bases/R�glementation/D�passement.seuil.180h.csv)    
[Lien vers la base de donn�es IHTS anormales](Bases/R�glementation/ihts.anormales.csv)      

**Nota :**   
HS en exc�s : au-del� de 25 heures par mois     
IHTS anormales : attribu�es � des fonctionnaires ou non-titulaires de cat�gorie A ou assimil�s.     




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



 
Cumul des lignes de paie par exercice et cat�gorie de ligne de paie   
 


Tableau  74  Ann�e  2009

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 334 263,04|
|             Rappels|   244 938,75|
|       R�m. diverses|    27 860,93|
| Suppl�ment familial|   146 331,98|
|         Traitements| 9 127 148,03|

Tableau  75  Ann�e  2010

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 334 245,36|
|             Rappels|   165 096,19|
|       R�m. diverses|    29 337,19|
| Suppl�ment familial|   148 838,36|
|         Traitements| 9 315 116,31|

Tableau  76  Ann�e  2011

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 325 516,15|
|             Rappels|   156 348,33|
|       R�m. diverses|    17 323,77|
| Suppl�ment familial|   140 162,84|
|         Traitements| 9 070 866,33|

Tableau  77  Ann�e  2012

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 532 916,11|
|             Rappels|   156 613,18|
|       R�m. diverses|    35 882,81|
| Suppl�ment familial|   138 565,19|
|         Traitements| 8 861 048,17|

Tableau  78  Ann�e  2013

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
 


Pour les agents n'ayant pas d'enfant signal� en base, il a �t� d�tect� 187 bulletins de paie pr�sentant un paiement du SFT apparemment anormal.

 
[Lien vers la base des paiements de SFT � agents sans enfant signal�](Bases/R�glementation/Paie.sans.enfant.r�duit.csv)
 


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


