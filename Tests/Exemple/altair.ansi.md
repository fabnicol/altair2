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




<p class = "centered"><b>Exercices 2008 � 2012 </b></p>
<p class = "author">Fabrice Nicol</h1>




mar. 24 nov. 2015
   


La dur�e du travail prise en compte dans la base de donn�es est de  151,67  h par mois.

# 1. Statistiques de population

### 1.1 Effectifs



 



&nbsp;*Tableau 1*   


|                                                             |  2008   |  2009   |  2010   |  2011   |  2012   |
|:------------------------------------------------------------|:-------:|:-------:|:-------:|:-------:|:-------:|
|Total effectifs (a)                                          | 1 119,0 | 1 146,0 | 1 221,0 | 1 215,0 | 1 260,0 |
|&nbsp;&nbsp;&nbsp;dont pr�sents 12 mois                      |  870,0  |  953,0  |  922,0  |  991,0  | 1 029,0 |
|&nbsp;&nbsp;&nbsp;dont fonctionnaires (b)                    |  704,0  |  706,0  |  733,0  |  735,0  |  744,0  |
|&nbsp;&nbsp;&nbsp;dont fonct. pr�sents 12 mois               |  644,0  |  661,0  |  667,0  |  698,0  |  690,0  |
|&nbsp;&nbsp;&nbsp;dont non titulaires                        |  192,0  |  210,0  |  231,0  |  244,0  |  261,0  |
|&nbsp;&nbsp;&nbsp;dont �lus                                  |  100,0  |  72,0   |  75,0   |  73,0   |  76,0   |
|&nbsp;&nbsp;&nbsp;dont �lus pr�sents 12 mois                 |  37,0   |  67,0   |  69,0   |  72,0   |  71,0   |
|&nbsp;&nbsp;&nbsp;dont vacataires d�tect�s (c)               |   0,0   |   0,0   |   0,0   |   0,0   |   0,0   |
|&nbsp;&nbsp;&nbsp;dont assistantes maternelles d�tect�es (c) |   0,0   |   0,0   |   0,0   |   0,0   |   0,0   |
|Postes non actifs (g)                                        |  58,0   |  89,0   |  118,0  |  66,0   |  92,0   |
|Postes annexes (g)                                           |  138,0  |  178,0  |  198,0  |  175,0  |  180,0  |
|Postes actifs non annexes (g)                                |  875,0  |  891,0  |  937,0  |  959,0  |  995,0  |
|Total ETP/ann�e (d)                                          |  891,7  |  914,6  |  946,5  |  994,1  | 1 005,3 |
|Total ETPT/ann�e (e)                                         |  819,8  |  846,6  |  860,6  |  908,8  |  914,8  |
|Total ETPT/ann�e personnes en place (f)(g)                   |   0,0   |  621,1  |  627,5  |  632,6  |  655,5  |
|Total ETPT/ann�e fonctionnaires (g)                          |  640,2  |  649,6  |  655,8  |  678,6  |  678,0  |
|Total ETPT/ann�e titulaires � temps complet (g)              |  449,6  |  477,0  |  466,2  |  461,4  |  477,1  |
|Total ETPT non titulaires (g)                                |  141,8  |  158,3  |  172,3  |  181,4  |  202,1  |
|Total ETPT autre statut                                      |   0,7   |   0,1   |   4,7   |   6,3   |  10,1   |
|Total ETPT postes non actifs (g)                             |   0,5   |   0,1   |   0,2   |   0,1   |   0,0   |
|Total ETPT postes annexes (g)                                |  38,0   |  38,7   |  31,3   |  46,5   |  28,5   |
|Total ETPT postes actifs non annexes (g)                     |  782,3  |  807,9  |  829,1  |  862,2  |  886,3  |


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
   


|  Statistique  | �ge des personnels <br>au 31/12/2008 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  18                  |          |
| 1er quartile  |                  30                  |          |
|    M�diane    |                  38                  |          |
|    Moyenne    |                38,65                 |   941    |
| 3�me quartile |                  48                  |          |
|    Maximum    |                  64                  |          |

 

![plot of chunk unnamed-chunk-9](figure/unnamed-chunk-9-1.png) 

 
&nbsp;*Tableau 3*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2012 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  18                  |          |
| 1er quartile  |                  31                  |          |
|    M�diane    |                  40                  |          |
|    Moyenne    |                39,99                 |          |
| 3�me quartile |                  49                  |          |
|    Maximum    |                  66                  |          |

 
 

![plot of chunk unnamed-chunk-11](figure/unnamed-chunk-11-1.png) 


### 1.3 Pyramide des �ges, personnels non titulaires
 
&nbsp;*Tableau 4*   
   


|  Statistique  | �ge des personnels non titulaires<br>au 31/12/2008 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         18                         |          |
| 1er quartile  |                         21                         |          |
|    M�diane    |                        26,5                        |          |
|    Moyenne    |                       31,72                        |   260    |
| 3�me quartile |                         40                         |          |
|    Maximum    |                         63                         |          |

 

![plot of chunk unnamed-chunk-13](figure/unnamed-chunk-13-1.png) 

 
&nbsp;*Tableau 5*   
   


|  Statistique  | �ge des personnels non titulaires<br>au 31/12/2012 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         18                         |          |
| 1er quartile  |                         23                         |          |
|    M�diane    |                         27                         |          |
|    Moyenne    |                        31,7                        |   315    |
| 3�me quartile |                         39                         |          |
|    Maximum    |                         62                         |          |

![plot of chunk unnamed-chunk-14](figure/unnamed-chunk-14-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.nontit.csv)  
 


### 1.4 Pyramide des �ges, personnels fonctionnaires stagiaires et titulaires
 
&nbsp;*Tableau 6*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2008 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  22                  |          |
| 1er quartile  |                  34                  |          |
|    M�diane    |                  41                  |          |
|    Moyenne    |                41,42                 |   674    |
| 3�me quartile |                  49                  |          |
|    Maximum    |                  64                  |          |

 


![plot of chunk unnamed-chunk-16](figure/unnamed-chunk-16-1.png) 

 
&nbsp;*Tableau 7*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2012 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  23                  |          |
| 1er quartile  |                  37                  |          |
|    M�diane    |                  43                  |          |
|    Moyenne    |                43,78                 |   723    |
| 3�me quartile |                  51                  |          |
|    Maximum    |                  66                  |          |



![plot of chunk unnamed-chunk-18](figure/unnamed-chunk-18-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.fonctionnaires.csv)  
 
### 1.5 Pyramide des �ges, personnels permanents (titulaires, stagiaires et non titulaires)
 
&nbsp;*Tableau 8*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2008 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  18                  |          |
| 1er quartile  |                  30                  |          |
|    M�diane    |                  38                  |          |
|    Moyenne    |                38,72                 |   934    |
| 3�me quartile |                  48                  |          |
|    Maximum    |                  64                  |          |

 

![plot of chunk unnamed-chunk-20](figure/unnamed-chunk-20-1.png) 

&nbsp;*Tableau 9*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2012 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  18                  |          |
| 1er quartile  |                  31                  |          |
|    M�diane    |                  40                  |          |
|    Moyenne    |                40,11                 |   1038   |
| 3�me quartile |                  49                  |          |
|    Maximum    |                  66                  |          |



![plot of chunk unnamed-chunk-22](figure/unnamed-chunk-22-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.permanents.csv)  
 
### 1.6 Effectifs des personnels par dur�e de service

**Personnels en fonction (hors �lus) des exercices 2008 � 2012 inclus :**

 
&nbsp;*Tableau 10*   
   


| Plus de 2 ans | Moins de 2 ans | Moins d'un an | Moins de six mois |
|:-------------:|:--------------:|:-------------:|:-----------------:|
|      945      |      268       |      137      |        57         |



![plot of chunk unnamed-chunk-24](figure/unnamed-chunk-24-1.png) 


**Effectifs (hors �lus)**   




 
&nbsp;*Tableau 11*   
   


|                  | 2008 | 2009 | 2010 | 2011 | 2012 |
|:-----------------|:----:|:----:|:----:|:----:|:----:|
|Plus de deux ans  | 792  | 844  | 910  | 886  | 861  |
|Moins de deux ans |  83  |  47  |  27  |  73  | 134  |
|Total             | 875  | 891  | 937  | 959  | 995  |



**Nota :**
*Personnels en place : ayant servi au moins deux ann�es cons�cutives pendant la p�riode.*     
*Plus/moins de deux ans : plus/mois de 730 jours sur la p�riode sous revue.*   



  
  



   
# 2. R�mun�rations brutes : analyse pour l'exercice 2008    
   
## 2.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des r�mun�rations brutes pour l'exercice 2008   
 
*Personnels (hors �lus)*     
 
&nbsp;*Tableau 12*   
   


|           Agr�gats           |   k&euro;    |
|------------------------------|--------------|
|   Brut annuel (bulletins)    | 23 318 144,6 |
| Brut annuel (lignes), dont : | 23 365 224,6 |
|         \ \ Primes :         | 6 347 169,0  |
|   \ \ Autres r�mun�rations   |    797,4     |
|     Part de primes en %      |     27,2     |

   

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
| Bulletins de paie | 23 318 144,6 |
|  Lignes de paie   | 23 365 224,6 |
|    Diff�rence     |  -47 079,9   |

   
� comparer aux soldes des comptes 641 et 648 du compte de gestion.   


  
## 2.2 Masse salariale brute des fonctionnaires    

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*   


![plot of chunk unnamed-chunk-127](figure/unnamed-chunk-127-1.png) 
![plot of chunk unnamed-chunk-128](figure/unnamed-chunk-128-1.png) 
![plot of chunk unnamed-chunk-129](figure/unnamed-chunk-129-1.png) 
![plot of chunk unnamed-chunk-130](figure/unnamed-chunk-130-1.png) 
![plot of chunk unnamed-chunk-131](figure/unnamed-chunk-131-1.png) 

![plot of chunk unnamed-chunk-132](figure/unnamed-chunk-132-1.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio r�mun�ration / quotit�*   
   
**Effectif : 693**   
   
**Tests de coh�rence**   


   
Somme des r�mun�rations brutes vers�es aux personnels titulaires et stagiaires :    

 
&nbsp;*Tableau 14*   
   


|           Agr�gats           |   k&euro;    |
|------------------------------|--------------|
|   Brut annuel (bulletins)    | 18 267 594,8 |
| Brut annuel (lignes), dont : | 18 388 645,3 |
|         \ \ Primes :         | 4 969 970,9  |
|   \ \ Autres r�mun�rations   |     0,0      |
|     Part de primes en %      |     27,2     |

    
**D�finitions :**     
   
 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lus*         : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
   
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   

 
&nbsp;*Tableau 15*      
   


|     Agr�gats      |   k&euro;    |
|-------------------|--------------|
| Bulletins de paie | 18 267 594,8 |
|  Lignes de paie   | 18 388 645,3 |
|    Diff�rence     |  -121 050,5  |

   
A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.    

**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2008**      
 
&nbsp;*Tableau 16*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres r�mun�rations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 3 164|    763,5|                    0|         |
|  1er quartile|                16 221|  4 830,1|                    0|         |
|       M�diane|                17 915|  5 927,4|                    0|         |
|       Moyenne|                19 639|  7 270,6|                    0|      683|
| 3�me quartile|                22 428|  8 281,7|                    0|         |
|       Maximum|                80 378| 42 553,4|                    0|         |

 
&nbsp;*Tableau 17*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la r�m. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     4 024|      4 063|                 0|                        9,158|         |
|  1er quartile|                    21 543|     21 360|            21 639|                       21,751|         |
|       M�diane|                    24 533|     24 382|            24 780|                       24,628|         |
|       Moyenne|                    26 909|     26 724|            27 559|                       26,210|      683|
| 3�me quartile|                    29 412|     29 256|            29 757|                       29,084|         |
|       Maximum|                   122 739|    119 918|           119 591|                       50,764|         |

   
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



![plot of chunk unnamed-chunk-141](figure/unnamed-chunk-141-1.png) 

  
**Nota :**   
*Ne sont retenues que les r�mun�rations sup�rieures � 1 000 k&euro;.*  
*Les �lus ne sont pas pris en compte.*  
  

![plot of chunk unnamed-chunk-142](figure/unnamed-chunk-142-1.png) 

  
**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2008**     
  
 
&nbsp;*Tableau 18*   
   


|   Statistique| Primes| Autres r�mun�rations| Effectif|
|-------------:|------:|--------------------:|--------:|
|       Minimum|      0|                0,000|         |
|  1er quartile|  2 775|                0,000|         |
|       M�diane|  5 223|                0,000|         |
|       Moyenne|  6 718|                4,153|      192|
| 3�me quartile|  7 731|                0,000|         |
|       Maximum| 43 361|              797,420|         |

   
&nbsp;*Tableau 19*   
   
   

|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               3 768|                    2 182|         |
|  1er quartile|              12 035|                   21 651|         |
|       M�diane|              22 813|                   25 140|         |
|       Moyenne|              25 191|                   30 220|      192|
| 3�me quartile|              30 996|                   35 658|         |
|       Maximum|              97 650|                   97 383|         |


  
  



   
# 3. R�mun�rations brutes : analyse pour l'exercice 2012    
   
## 3.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des r�mun�rations brutes pour l'exercice 2012   
 
*Personnels (hors �lus)*     
 
&nbsp;*Tableau 20*   
   


|           Agr�gats           |   k&euro;    |
|------------------------------|--------------|
|   Brut annuel (bulletins)    | 27 525 369,9 |
| Brut annuel (lignes), dont : | 27 637 098,5 |
|         \ \ Primes :         | 7 668 892,9  |
|   \ \ Autres r�mun�rations   |   -3 271,1   |
|     Part de primes en %      |     27,9     |

   

**D�finitions :**   

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lu*          : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
    
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   
 
 
&nbsp;*Tableau 21*    
   


|     Agr�gats      |   k&euro;    |
|-------------------|--------------|
| Bulletins de paie | 27 525 369,9 |
|  Lignes de paie   | 27 637 098,5 |
|    Diff�rence     |  -111 728,6  |

   
� comparer aux soldes des comptes 641 et 648 du compte de gestion.   


  
## 3.2 Masse salariale brute des fonctionnaires    

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*   


![plot of chunk unnamed-chunk-149](figure/unnamed-chunk-149-1.png) 
![plot of chunk unnamed-chunk-150](figure/unnamed-chunk-150-1.png) 
![plot of chunk unnamed-chunk-151](figure/unnamed-chunk-151-1.png) 
![plot of chunk unnamed-chunk-152](figure/unnamed-chunk-152-1.png) 
![plot of chunk unnamed-chunk-153](figure/unnamed-chunk-153-1.png) 

![plot of chunk unnamed-chunk-154](figure/unnamed-chunk-154-1.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio r�mun�ration / quotit�*   
   
**Effectif : 739**   
   
**Tests de coh�rence**   


   
Somme des r�mun�rations brutes vers�es aux personnels titulaires et stagiaires :    

 
&nbsp;*Tableau 22*   
   


|           Agr�gats           |   k&euro;    |
|------------------------------|--------------|
|   Brut annuel (bulletins)    | 20 973 059,8 |
| Brut annuel (lignes), dont : | 21 062 379,7 |
|         \ \ Primes :         | 5 780 629,0  |
|   \ \ Autres r�mun�rations   |  -16 669,1   |
|     Part de primes en %      |     27,6     |

    
**D�finitions :**     
   
 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Indemnit�s d'�lus*         : toutes r�mun�rations indemnitaires des �lus    
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
   
**Tests de coh�rence**   

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :   

 
&nbsp;*Tableau 23*      
   


|     Agr�gats      |   k&euro;    |
|-------------------|--------------|
| Bulletins de paie | 20 973 059,8 |
|  Lignes de paie   | 21 062 379,7 |
|    Diff�rence     |  -89 319,9   |

   
A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.    

**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2012**      
 
&nbsp;*Tableau 24*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres r�mun�rations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 3 342|    874,3|              -329,58|         |
|  1er quartile|                17 280|  4 941,4|               -54,77|         |
|       M�diane|                19 189|  6 271,7|                 0,00|         |
|       Moyenne|                20 978|  7 695,3|               -22,90|      728|
| 3�me quartile|                23 905|  8 747,9|                 0,00|         |
|       Maximum|                54 217| 42 719,1|                69,35|         |

 
&nbsp;*Tableau 25*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la r�m. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     4 217|      4 234|                 0|                        11,88|         |
|  1er quartile|                    22 677|     22 667|            23 251|                        21,66|         |
|       M�diane|                    25 992|     26 017|            26 944|                        24,32|         |
|       Moyenne|                    28 673|     28 547|            30 320|                        26,04|      728|
| 3�me quartile|                    31 744|     31 592|            32 636|                        28,95|         |
|       Maximum|                    94 940|    108 055|           348 542|                        64,86|         |

   
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



![plot of chunk unnamed-chunk-163](figure/unnamed-chunk-163-1.png) 

  
**Nota :**   
*Ne sont retenues que les r�mun�rations sup�rieures � 1 000 k&euro;.*  
*Les �lus ne sont pas pris en compte.*  
  

![plot of chunk unnamed-chunk-164](figure/unnamed-chunk-164-1.png) 

  
**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2012**     
  
 
&nbsp;*Tableau 26*   
   


|   Statistique| Primes| Autres r�mun�rations| Effectif|
|-------------:|------:|--------------------:|--------:|
|       Minimum|      0|               -162,9|         |
|  1er quartile|  3 187|                  0,0|         |
|       M�diane|  5 700|                  0,0|         |
|       Moyenne|  6 778|                 -2,2|      267|
| 3�me quartile|  7 259|                  0,0|         |
|       Maximum| 60 095|                678,1|         |

   
&nbsp;*Tableau 27*   
   
   

|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|              -2 780|                   -5 305|         |
|  1er quartile|              12 933|                   21 798|         |
|       M�diane|              21 721|                   24 335|         |
|       Moyenne|              23 776|                   28 681|      267|
| 3�me quartile|              26 269|                   33 345|         |
|       Maximum|             130 660|                  130 303|         |

 
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
  




# 4. R�mun�rations nettes : �volutions sur la p�riode 2008 - 2012    

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

## 4.2 Evolutions des r�mun�rations nettes sur la p�riode 2008 - 2012   

### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors �lus)





   
**Salaire net moyen par t�te (SMPT net) en EQTP, hors �lus**         
      
 
&nbsp;*Tableau 30*   
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-----------------:|:-------------:|
| 2008  |           20 245            |               |      23 164       |               |
| 2009  |           20 791            |               |      23 762       |               |
| 2010  |           22 255            |      19       |      24 164       |     6,03      |
| 2011  |           23 492            |               |      24 833       |               |
| 2012  |           24 094            |               |      24 561       |               |

  
**Effet de noria sur salaires nets et taux de remplacements**       
  
**Effet de noria** : *diff�rence entre la r�mun�ration annuelle des entrants � l'ann�e N et des sortants � l'ann�e N-1*.  
*Usuellement calcul�e sur les r�mun�rations brutes, ici sur les r�mun�rations nettes EQTP*  
*afin d'appr�cier l'impact de cet effet sur l'�volution des r�mun�rations nette moyennes calcul�e au tableau pr�c�dent.*               
 
&nbsp;*Tableau 31*   
   


| Ann�e | Noria EQTP (&euro;) | En % de la MS N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-----------------:|:------------------:|:-------------------------:|
| 2009  |       -944,73       |      -0,176       |         38         |            4,5            |
| 2010  |       2 108,6       |       0,364       |         36         |            4,2            |
| 2011  |      -1 003,2       |      -0,181       |         40         |            4,4            |
| 2012  |      -9 553,9       |       -1,36       |         33         |            3,7            |


*MS N-1 : masse salariale nette de l'ann�e n-1.*   
      
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) en EQTP**         
      
 
&nbsp;*Tableau 32*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          1 368|
|  1er quartile|         18 040|
|       M�diane|         20 460|
|       Moyenne|         23 160|
| 3�me quartile|         24 880|
|       Maximum|         97 670|



|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|          2 868|
|  1er quartile|         18 730|
|       M�diane|         21 260|
|       Moyenne|         24 560|
| 3�me quartile|         26 410|
|       Maximum|        285 900|

 
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
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2008  |           15 533            |               |          22 776           |               |
| 2009  |           15 758            |               |          23 555           |               |
| 2010  |           16 721            |     15,1      |          24 163           |     8,84      |
| 2011  |           17 154            |               |          24 332           |               |
| 2012  |           17 874            |               |          24 790           |               |





  
**Effet de noria sur salaires nets et taux de remplacements**       
 
&nbsp;*Tableau 37*   
   


| Ann�e | Noria EQTP (&euro;) | En % de la  MSN N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-------------------:|:------------------:|:-------------------------:|
| 2009  |      -7 088,8       |       -0,487        |         11         |            1,6            |
| 2010  |       10 702        |        1,16         |         17         |            2,6            |
| 2011  |      -8 228,9       |       -0,914        |         19         |            2,7            |
| 2012  |       7 910,8       |        0,556        |         12         |            1,8            |

    
*MS N-1 : masse salariale nette de l'ann�e n-1.*   
      
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) en EQTP**         
 
&nbsp;*Tableau 38*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|          1 368|
|  1er quartile|         18 070|
|       M�diane|         20 460|
|       Moyenne|         22 780|
| 3�me quartile|         24 580|
|       Maximum|         97 670|



|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|          3 506|
|  1er quartile|         18 870|
|       M�diane|         21 760|
|       Moyenne|         24 790|
| 3�me quartile|         26 320|
|       Maximum|        285 900|

   



  
**Evolution du SMPT net des titulaires � temps complet**     
  
 
&nbsp;*Tableau 39*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | SMPT (&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:-------------:|:-------------:|
| 2008  |               10 641                |               |    23 183     |               |
| 2009  |               11 632                |               |    24 233     |               |
| 2010  |               11 517                |     12,7      |    24 504     |     7,36      |
| 2011  |               11 548                |               |    24 781     |               |
| 2012  |               11 997                |               |    24 890     |               |

   
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) des titulaires � temps complet**         
      
 
&nbsp;*Tableau 40*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|         15 260|
|  1er quartile|         18 100|
|       M�diane|         20 460|
|       Moyenne|         23 180|
| 3�me quartile|         24 630|
|       Maximum|         97 670|

  


|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|          9 460|
|  1er quartile|         19 110|
|       M�diane|         21 830|
|       Moyenne|         24 890|
| 3�me quartile|         27 070|
|       Maximum|         76 170|


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
| 2009  |               15 538                |               |        24 742        |               |
| 2010  |               15 937                |     8,66      |        24 979        |     1,24      |
| 2011  |               16 218                |               |        25 144        |               |
| 2012  |               16 883                |               |        25 050        |               |

   
**Distribution et variation sur la p�riode de la r�mun�ration nette des personnes en place**                
 
&nbsp;*Tableau 42*   
   



 


|   Statistique| Premi�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          4 617|         |
|  1er quartile|         18 170|         |
|       M�diane|         20 590|         |
|       Moyenne|         23 450|      786|
| 3�me quartile|         24 830|         |
|       Maximum|         97 670|         |

 
&nbsp;*Tableau 43*   
   


|   Statistique| Derni�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          8 850|         |
|  1er quartile|         18 990|         |
|       M�diane|         21 610|         |
|       Moyenne|         24 730|      877|
| 3�me quartile|         26 490|         |
|       Maximum|        142 500|         |


*Variation individuelle de r�mun�ration nette en EQTP pour les personnels pr�sents la premi�re et la derni�re ann�e*   
 
&nbsp;*Tableau 44*   
   


|   Statistique| Variation normalis�e (%)| Variation annuelle moyenne normalis�e (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -39,818|                                 -11,9 220|         |
|  1er quartile|                    1,530|                                   0,3 803|         |
|       M�diane|                    6,118|                                   1,4 956|         |
|       Moyenne|                    7,458|                                   1,6 741|      682|
| 3�me quartile|                   11,494|                                   2,7 573|         |
|       Maximum|                  101,377|                                  19,1 249|         |

   
### 4.3.2 Titulaires et stagiaires     
  



  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 45*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2009  |               12 932                |               |        23 948        |               |
| 2010  |               13 199                |     9,11      |        24 263        |     2,11      |
| 2011  |               13 562                |               |        24 570        |               |
| 2012  |               14 110                |               |        24 454        |               |

   
**Distribution et variation sur la p�riode de la r�mun�ration nette des fonctionnaires en place**                
 
&nbsp;*Tableau 46*   
   
 


|   Statistique| Premi�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|         10 590|         |
|  1er quartile|         18 110|         |
|       M�diane|         20 430|         |
|       Moyenne|         22 760|      677|
| 3�me quartile|         24 320|         |
|       Maximum|         97 670|         |

 
&nbsp;*Tableau 47*   
   


|   Statistique| Derni�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|          9 570|         |
|  1er quartile|         18 790|         |
|       M�diane|         21 740|         |
|       Moyenne|         24 650|      792|
| 3�me quartile|         26 320|         |
|       Maximum|        142 500|         |


*Variation individuelle de r�mun�ration nette en EQTP pour les personnels pr�sents la premi�re et la derni�re ann�e*   
 
&nbsp;*Tableau 48*   
   


|   Statistique| Variation normalis�e (%)| Variation annuelle moyenne normalis�e (%)| Effectif|
|-------------:|------------------------:|-----------------------------------------:|--------:|
|       Minimum|                  -39,818|                                 -11,9 220|         |
|  1er quartile|                    1,648|                                   0,4 094|         |
|       M�diane|                    6,590|                                   1,6 083|         |
|       Moyenne|                    7,404|                                   1,6 695|      611|
| 3�me quartile|                   11,474|                                   2,7 527|         |
|       Maximum|                  101,377|                                  19,1 249|         |



[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)

**Nota**   
*Personnes en place :* en fonction au moins deux ann�es cons�cutives sur la p�riode 2008 � 2012    
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

Il existe  1 non titulaire   percevant une NBI.

Primes informatiques potentielles : aucune
 
&nbsp;*Tableau 51*   
   


| Nombre de lignes NBI pour non titulaires | Nombre de b�n�ficiaires de PFI |
|:----------------------------------------:|:------------------------------:|
|                    12                    |               0                |

  
[Lien vers la base de donn�es NBI aux non titulaires](Bases/R�glementation/NBI.aux.non.titulaires.csv)   
[Lien vers la base de donn�es Primes informatiques](Bases/R�glementation/personnels.prime.informatique.csv)   
  
**Nota :**   
NBI: nouvelle bonification indiciaire   
PFI: prime de fonctions informatiques   




 
&nbsp;*Tableau 52*   
   


| R�mun�rations de NBI anormales, par agent et par exercice | Montants correspondants |
|:---------------------------------------------------------:|:-----------------------:|
|                            71                             |         33 543          |

  
[Lien vers la base de donn�es NBI anormales](Bases/Fiabilit�/lignes.nbi.anormales.csv)   
  
**Nota :**   
*Est consid�r� comme manifestement anormal un total annuel de r�mun�rations NBI correspondant � un point d'indice net mensuel inf�rieur � 4 euros ou sup�rieur � 6 euros.*    
*Les rappels ne sont pas pris en compte dans les montants vers�s. Certains �carts peuvent �tre r�gularis�s en les prenant en compte*     
 
&nbsp;*Tableau 53*   
   


| Ann�e | Cumuls des NBI | Montants vers�s (a) | Point d'INM apparent | Point d'INM moyen | Contr�le |
|-------|----------------|---------------------|----------------------|-------------------|----------|
| 2008  |    25 255,0    |      113 463,9      |         4,5          |        4,6        |   Vert   |
| 2009  |    28 280,0    |      127 491,1      |         4,5          |        4,6        |   Vert   |
| 2010  |    29 740,0    |      134 382,9      |         4,5          |        4,6        |   Vert   |
| 2011  |    28 820,0    |      130 786,8      |         4,5          |        4,6        |   Vert   |
| 2012  |    29 730,0    |      134 716,0      |         4,5          |        4,6        |   Vert   |


*(a) Hors rappels sur r�mun�rations*   




  
[Lien vers la base de donn�es des cumuls annuels de NBI](Bases/Fiabilit�/cumuls.nbi.csv)   
  
 
## 5.2 Contr�le des vacations pour les fonctionnaires





Il y a  0 fonctionnaire(s) effectuant des vacations pour son propre �tablissement. Les bulletins concern�s sont donn�s en lien.


[Lien vers les matricules des fonctionnaires concern�s](Bases/R�glementation/matricules.fonctionnaires.et.vacations.csv)
[Lien vers les bulletins de paie correspondants](Bases/R�glementation/lignes.fonctionnaires.et.vacations.csv)

## 5.3 Contr�les sur les cumuls traitement indiciaire, indemnit�s et vacations des contractuels    




**Contractuels effectuant des vacations (CEV)**

 
&nbsp;*Tableau 54*   
   


| Nombre de CEV | Nombre de lignes | Nombre de lignes indemnitaires | Nombre de lignes de traitement |
|:-------------:|:----------------:|:------------------------------:|:------------------------------:|
|       0       |        0         |               0                |               0                |

 
[Lien vers le bulletins des CEV](Bases/R�glementation/lignes.contractuels.et.vacations.csv)   
[Lien vers la base de donn�es Matricules des CEV](Bases/R�glementation/matricules.contractuels.et.vacations.csv)  
[Lien vers la base de donn�es Cumul r�gime indemnitaire et vacations de CEV](Bases/R�glementation/RI.et.vacations.csv)  
[Lien vers la base de donn�es Lignes de traitement indiciaire pour CEV](Bases/R�glementation/traitement.et.vacations.csv)  
 




## 5.4 Contr�le sur les indemnit�s IAT et IFTS      




 
&nbsp;*Tableau 55*   
     


|             Codes IFTS             |   |
|:----------------------------------:|:-:|
| 502N 502A 202N 202R 202A 502R 502V |   |



| Nombre de personnels percevant IAT et IFTS |   |
|:------------------------------------------:|:-:|
|                     11                     |   |

  
[Codes IFTS retenus](Bases/R�glementation/codes.ifts.csv)   
[Lien vers la base de donn�es cumuls iat/ifts](Bases/R�glementation/personnels.iat.ifts.csv)    

### Contr�le sur les IFTS pour cat�gories B et contractuels



 



 




 
&nbsp;*Tableau 56*   
   


| Nombre de lignes de paie de contractuels percevant des IFTS | Nombre de lignes IFTS pour IB < 380 |
|:-----------------------------------------------------------:|:-----------------------------------:|
|                            2 448                            |                 318                 |


[Lien vers la base de donn�es Lignes IFTS pour contractuels](Bases/R�glementation/ifts.et.contractuel.csv)    
[Lien vers la base de donn�es Lignes IFTS pour IB < 380](Bases/R�glementation/lignes.ifts.anormales.csv)     

**Nota :**
IB < 380 : fonctionnaire percevant un indice brut inf�rieur � 380





## 5.5 Contr�le de la prime de fonctions et de r�sultats (PFR)   
  




 
&nbsp;*Tableau 57*   
     


| Codes PFR | Agents cumulant PFR et IFTS |
|:---------:|:---------------------------:|
|           |              0              |

  
[Lien vers la base de donn�es cumuls pfr/ifts](Bases/R�glementation/personnels.pfr.ifts.csv)    




 
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

Le seuil de 180 heures suppl�mentaires maximum est d�pass� par  50  agents.
 Le seuil de 220 heures suppl�mentaires maximum est d�pass� par  50  agents.


 
&nbsp;*Tableau 61*   
   


| Nombre de lignes HS en exc�s | Nombre de lignes IHTS anormales |
|:----------------------------:|:-------------------------------:|
|             388              |               NA                |


[Lien vers la base de donn�es Heures suppl�mentaires en exc�s du seuil de 25h/mois](Bases/R�glementation/HS.sup.25.csv)     
[Lien vers la base de donn�es cumuls en exc�s des seuils annuels](Bases/R�glementation/D�passement.seuil.180h.csv)    
[Lien vers la base de donn�es IHTS anormales](Bases/R�glementation/ihts.anormales.csv)      

**Nota :**   
HS en exc�s : au-del� de 25 heures par mois     
IHTS anormales : non attribu�es � des fonctionnaires de cat�gorie B ou C.     




## 5.7 Contr�le sur les indemnit�s des �lus
  



  


|Matricule |Nom | Ann�e|Emploi                       | Indemnit�s | Autres |    Total |
|:---------|:---|-----:|:----------------------------|-----------:|-------:|---------:|
|1002 P    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|1002 P    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1002 P    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1002 P    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1002 P    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|108 T     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|108 T     |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|108 T     |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    16841,07|       0|  16841,07|
|108 T     |YYY |  2011|12EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|108 T     |YYY |  2012|12EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1163 P    |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1163 P    |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1163 P    |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1163 P    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1212 T    |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|1223 E    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|1319 J    |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     7490,91|       0|   7490,91|
|1319 J    |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1319 J    |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1319 J    |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1319 J    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1320 K    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1320 K    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1320 K    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1320 K    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1320 K    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1321 L    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1321 L    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1322 M    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1322 M    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1322 M    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1322 M    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1322 M    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1323 N    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1323 N    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1323 N    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1323 N    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     7070,76|       0|   7070,76|
|1323 N    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1324 P    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1324 P    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1324 P    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1324 P    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1324 P    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1325 R    |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     7490,91|       0|   7490,91|
|1325 R    |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1325 R    |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1325 R    |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1325 R    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1326 S    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1326 S    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1326 S    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1326 S    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1326 S    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1327 T    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1327 T    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1327 T    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1327 T    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1327 T    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1328 U    |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    11686,51|       0|  11686,51|
|1328 U    |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    18522,30|       0|  18522,30|
|1328 U    |YYY |  2010|11EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|1328 U    |YYY |  2011|11EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1328 U    |YYY |  2012|11EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1329 V    |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     7490,91|       0|   7490,91|
|1329 V    |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |     3752,48|       0|   3752,48|
|1330 W    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1330 W    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1330 W    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     9219,99|       0|   9219,99|
|1330 W    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |    11404,44|       0|  11404,44|
|1330 W    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1331 X    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1331 X    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1331 X    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1331 X    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1331 X    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1332 Y    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1332 Y    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1332 Y    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1332 Y    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1332 Y    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1333 Z    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1333 Z    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1333 Z    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1333 Z    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1333 Z    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1334 A    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1334 A    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1334 A    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1334 A    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1334 A    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1335 B    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1335 B    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1335 B    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1335 B    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1335 B    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1336 C    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1336 C    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1336 C    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1336 C    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1336 C    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1337 D    |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     7490,91|       0|   7490,91|
|1337 D    |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1337 D    |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1337 D    |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1337 D    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1338 E    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1338 E    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1338 E    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1338 E    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1338 E    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1339 F    |YYY |  2008|11EME VICE PRESIDENT         |    12285,11|       0|  12285,11|
|1339 F    |YYY |  2009|11EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|1339 F    |YYY |  2010|10EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|1339 F    |YYY |  2011|10EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1339 F    |YYY |  2012|10EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1340 G    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     7490,91|       0|   7490,91|
|1340 G    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |    11294,07|       0|  11294,07|
|1340 G    |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1340 G    |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1340 G    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1341 H    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1341 H    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1341 H    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1341 H    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1341 H    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1342 J    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1797,85|       0|   1797,85|
|1342 J    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1342 J    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |      907,80|       0|    907,80|
|1349 S    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1573,37|       0|   1573,37|
|1349 S    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1349 S    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1349 S    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1349 S    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1350 T    |YYY |  2008|14EME VICE PRESIDENT         |    10751,20|       0|  10751,20|
|1350 T    |YYY |  2009|14EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|1350 T    |YYY |  2010|13EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|1350 T    |YYY |  2011|13EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1350 T    |YYY |  2012|13EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1351 U    |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     6555,60|       0|   6555,60|
|1351 U    |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1351 U    |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1351 U    |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1351 U    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1352 V    |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     1573,37|       0|   1573,37|
|1352 V    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|1352 V    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1352 V    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1352 V    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1353 W    |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     6555,60|       0|   6555,60|
|1353 W    |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1353 W    |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    13200,75|       0|  13200,75|
|1353 W    |YYY |  2011|19EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1353 W    |YYY |  2012|19EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|1362 F    |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     5620,29|       0|   5620,29|
|1362 F    |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|1362 F    |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1362 F    |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1362 F    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1476 E    |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |     7541,59|       0|   7541,59|
|1476 E    |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|1476 E    |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1476 E    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1532 R    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |      907,12|       0|    907,12|
|1532 R    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1532 R    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1532 R    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1535 U    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |      907,12|       0|    907,12|
|1535 U    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1535 U    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1535 U    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1536 V    |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |      907,12|       0|    907,12|
|1536 V    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     9219,99|       0|   9219,99|
|1536 V    |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1536 V    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1570 G    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|1570 G    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1570 G    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1577 P    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2503,29|       0|   2503,29|
|1577 P    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1577 P    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1580 T    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2503,29|       0|   2503,29|
|1580 T    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1580 T    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1596 K    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2049,39|       0|   2049,39|
|1596 K    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     7793,04|       0|   7793,04|
|1596 K    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|1614 E    |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     1595,49|       0|   1595,49|
|1614 E    |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|1614 E    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|163 C     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|163 C     |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|163 C     |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|163 C     |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|163 C     |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|166 F     |YYY |  2008|9EME VICE PRESIDENT          |    16818,66|       0|  16818,66|
|166 F     |YYY |  2009|10EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|166 F     |YYY |  2010|9EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|166 F     |YYY |  2011|9EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|166 F     |YYY |  2012|9EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|179 V     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|1867 E    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2508,99|       0|   2508,99|
|1868 F    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     3736,87|       0|   3736,87|
|1978 A    |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2946,14|       0|   2946,14|
|1988 L    |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |      950,37|       0|    950,37|
|232 C     |YYY |  2008|2EME PREMIER VICE PRESIDENT  |    16818,66|       0|  16818,66|
|232 C     |YYY |  2009|2EME PREMIER VICE PRESIDENT  |    18522,30|       0|  18522,30|
|232 C     |YYY |  2010|2EME PREMIER VICE PRESIDENT  |    18656,70|       0|  18656,70|
|232 C     |YYY |  2011|2EME PREMIER VICE PRESIDENT  |    18703,20|       0|  18703,20|
|232 C     |YYY |  2012|2EME PREMIER VICE PRESIDENT  |    18703,20|       0|  18703,20|
|251 Y     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|252 Z     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|252 Z     |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|252 Z     |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|252 Z     |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|252 Z     |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|255 C     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|256 D     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|257 E     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|257 E     |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|257 E     |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|257 E     |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|257 E     |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|258 F     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|259 G     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     7994,50|       0|   7994,50|
|259 G     |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|259 G     |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|259 G     |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|259 G     |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|260 H     |YYY |  2008|10EME VICE PRESIDENT         |     4533,55|       0|   4533,55|
|261 J     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|261 J     |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|261 J     |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|261 J     |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2052,81|       0|   2052,81|
|263 L     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|264 M     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|264 M     |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|264 M     |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|264 M     |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|264 M     |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|265 N     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|265 N     |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|265 N     |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|265 N     |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|265 N     |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|266 P     |YYY |  2008|17EME VICE PRESIDENT         |    27012,82|       0|  36351,61|
|266 P     |YYY |  2009|17EME VICE PRESIDENT         |    27871,87|       0|  37221,44|
|266 P     |YYY |  2010|18EME VICE PRESIDENT         |    28067,29|       0|  37477,88|
|266 P     |YYY |  2011|18EME VICE PRESIDENT         |    28124,20|       0|  37545,20|
|266 P     |YYY |  2012|18EME VICE PRESIDENT         |    28179,92|       0|  37656,64|
|267 R     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|267 R     |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|267 R     |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|267 R     |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|267 R     |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    12438,43|       0|  12438,43|
|268 S     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|269 T     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|271 V     |YYY |  2008|11EME VICE PRESIDENT         |    12024,46|       0|  12024,46|
|271 V     |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|271 V     |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|271 V     |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|271 V     |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|272 W     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|272 W     |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|272 W     |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|272 W     |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|272 W     |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|273 X     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|274 Y     |YYY |  2008|4EME VICE PRESIDENT          |    28051,02|       0|  38428,01|
|274 Y     |YYY |  2009|3EME VICE PRESIDENT          |    29747,41|       0|  40972,52|
|274 Y     |YYY |  2010|2EME VICE PRESIDENT          |    29951,37|       0|  41246,04|
|274 Y     |YYY |  2011|2EME VICE PRESIDENT          |    30066,41|       0|  41429,62|
|274 Y     |YYY |  2012|2EME VICE PRESIDENT          |    24108,64|       0|  32631,28|
|276 A     |YYY |  2008|20EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|276 A     |YYY |  2009|17EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|276 A     |YYY |  2010|16EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|276 A     |YYY |  2011|16EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|276 A     |YYY |  2012|16EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|277 B     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|278 C     |YYY |  2008|1ER PREMIER VICE PRESIDENT   |    16818,66|       0|  16818,66|
|278 C     |YYY |  2009|1ER PREMIER VICE PRESIDENT   |    18522,30|       0|  18522,30|
|278 C     |YYY |  2010|1ER PREMIER VICE PRESIDENT   |    18656,70|       0|  18656,70|
|278 C     |YYY |  2011|1ER PREMIER VICE PRESIDENT   |    18703,20|       0|  18703,20|
|278 C     |YYY |  2012|1ER PREMIER VICE PRESIDENT   |    10054,88|       0|  10054,88|
|279 D     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|279 D     |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|279 D     |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|279 D     |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|279 D     |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|280 E     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|281 F     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|283 H     |YYY |  2008|6EME VICE PRESIDENT          |    12285,11|       0|  12285,11|
|283 H     |YYY |  2009|6EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|283 H     |YYY |  2010|5EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|283 H     |YYY |  2011|5EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|283 H     |YYY |  2012|5EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|284 J     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     2301,44|       0|   2301,44|
|284 J     |YYY |  2009|CONSEILLER COMMUNAUTAIRE     |     2710,56|       0|   2710,56|
|284 J     |YYY |  2010|CONSEILLER COMMUNAUTAIRE     |     2730,24|       0|   2730,24|
|284 J     |YYY |  2011|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|284 J     |YYY |  2012|CONSEILLER COMMUNAUTAIRE     |     2737,08|       0|   2737,08|
|285 K     |YYY |  2008|12EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|285 K     |YYY |  2009|8EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|285 K     |YYY |  2010|7EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|285 K     |YYY |  2011|4EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|285 K     |YYY |  2012|4EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|286 L     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|287 M     |YYY |  2008|14EME VICE PRESIDENT         |     4533,55|       0|   4533,55|
|288 N     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|289 P     |YYY |  2008|13EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|289 P     |YYY |  2009|5EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|289 P     |YYY |  2010|4EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|289 P     |YYY |  2011|3EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|289 P     |YYY |  2012|3EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|290 R     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|301 C     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     7994,50|       0|   7994,50|
|301 C     |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|301 C     |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|301 C     |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|301 C     |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|354 K     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|356 M     |YYY |  2008|16EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|356 M     |YYY |  2009|15EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|356 M     |YYY |  2010|14EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|356 M     |YYY |  2011|14EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|356 M     |YYY |  2012|14EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|371 D     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|461 B     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|507 B     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|508 C     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|509 D     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|510 E     |YYY |  2008|19EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|510 E     |YYY |  2009|16EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|510 E     |YYY |  2010|15EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|510 E     |YYY |  2011|15EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|510 E     |YYY |  2012|15EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|511 F     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    14970,08|       0|  14970,08|
|511 F     |YYY |  2009|18EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|511 F     |YYY |  2010|17EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|511 F     |YYY |  2011|17EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|511 F     |YYY |  2012|17EME VICE PRESIDENT         |    18703,20|       0|  18703,20|
|513 H     |YYY |  2008|18EME VICE PRESIDENT         |     4533,55|       0|   4533,55|
|518 N     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |     7994,50|       0|   7994,50|
|518 N     |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|518 N     |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|518 N     |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|518 N     |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|54 J      |YYY |  2008|3EME VICE PRESIDENT          |     4533,55|       0|   4533,55|
|563 M     |YYY |  2008|15EME VICE PRESIDENT         |    16818,66|       0|  16818,66|
|563 M     |YYY |  2009|13EME VICE PRESIDENT         |    18522,30|       0|  18522,30|
|563 M     |YYY |  2010|12EME VICE PRESIDENT         |    18656,70|       0|  18656,70|
|563 M     |YYY |  2011|7EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|563 M     |YYY |  2012|7EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|564 N     |YYY |  2008|5EME VICE PRESIDENT          |    16818,66|       0|  16818,66|
|564 N     |YYY |  2009|9EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|564 N     |YYY |  2010|8EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|564 N     |YYY |  2011|8EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|564 N     |YYY |  2012|8EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|565 P     |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|566 R     |YYY |  2008|7EME VICE PRESIDENT          |    16818,66|       0|  16818,66|
|566 R     |YYY |  2009|4EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|566 R     |YYY |  2010|3EME VICE PRESIDENT          |    74738,49|       0|  95297,61|
|566 R     |YYY |  2011|PRESIDENT                    |    84892,58|       0| 103639,72|
|566 R     |YYY |  2012|PRESIDENT                    |    78577,44|       0|  91009,44|
|58 N      |YYY |  2008|8EME VICE PRESIDENT          |    16818,66|       0|  16818,66|
|58 N      |YYY |  2009|7EME VICE PRESIDENT          |    18522,30|       0|  18522,30|
|58 N      |YYY |  2010|6EME VICE PRESIDENT          |    18656,70|       0|  18656,70|
|58 N      |YYY |  2011|6EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|58 N      |YYY |  2012|6EME VICE PRESIDENT          |    18703,20|       0|  18703,20|
|59 P      |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|62 T      |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|62 T      |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|62 T      |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|62 T      |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|62 T      |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|63 U      |YYY |  2008|PRESIDENT                    |    88657,58|       0| 121937,40|
|63 U      |YYY |  2009|PRESIDENT                    |    98805,00|       0| 132104,40|
|63 U      |YYY |  2010|PRESIDENT                    |    16173,42|       0|  21377,44|
|64 V      |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|
|65 W      |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|65 W      |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|65 W      |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|65 W      |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|65 W      |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|68 Z      |YYY |  2008|6EME VICE PRESIDENT          |    12024,46|       0|  12024,46|
|68 Z      |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|68 Z      |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|68 Z      |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|68 Z      |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|69 A      |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |     2684,97|       0|   2684,97|
|70 B      |YYY |  2008|CONSEILLER COMMUNAUT DELEGUE |    10175,88|       0|  10175,88|
|70 B      |YYY |  2009|CONSEILLER COMMUNAUT DELEGUE |    11294,07|       0|  11294,07|
|70 B      |YYY |  2010|CONSEILLER COMMUNAUT DELEGUE |    11376,06|       0|  11376,06|
|70 B      |YYY |  2011|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|70 B      |YYY |  2012|CONSEILLER COMMUNAUT DELEGUE |    11404,44|       0|  11404,44|
|924 E     |YYY |  2008|CONSEILLER COMMUNAUTAIRE     |      503,59|       0|    503,59|

[Lien vers la base de donn�es R�mun�rations des �lus](Bases/R�glementation/r�mun�rations.�lu.csv)




## 5.8 Lien avec le compte de gestion



 
Cumul des lignes de paie par exercice et cat�gorie de ligne de paie   
 


Tableau  62  Ann�e  2008

|           Cat�gorie|  Cumul annuel|
|-------------------:|-------------:|
|          Indemnit�s|  6 049 892,01|
|             Rappels|    288 659,23|
|       R�m. diverses|        797,42|
| Suppl�ment familial|    349 475,20|
|         Traitements| 17 674 314,76|

Tableau  63  Ann�e  2009

|           Cat�gorie|  Cumul annuel|
|-------------------:|-------------:|
|    Indem. R�sidence|      3 095,76|
|          Indemnit�s|  6 246 021,01|
|             Rappels|    448 471,90|
|       R�m. diverses|        534,22|
| Suppl�ment familial|    372 638,69|
|         Traitements| 18 492 771,17|

Tableau  64  Ann�e  2010

|           Cat�gorie|  Cumul annuel|
|-------------------:|-------------:|
|          Indemnit�s|  6 549 869,92|
|             Rappels|    461 846,07|
|       R�m. diverses|     19 660,33|
| Suppl�ment familial|    379 623,84|
|         Traitements| 19 150 025,62|

Tableau  65  Ann�e  2011

|           Cat�gorie|  Cumul annuel|
|-------------------:|-------------:|
|          Indemnit�s|  6 942 832,60|
|             Rappels|    385 376,55|
|       R�m. diverses|     17 154,00|
| Suppl�ment familial|    389 664,80|
|         Traitements| 20 045 292,29|

Tableau  66  Ann�e  2012

|           Cat�gorie|  Cumul annuel|
|-------------------:|-------------:|
|          Indemnit�s|  7 313 817,20|
|             Rappels|    289 683,65|
|       R�m. diverses|     -3 271,09|
| Suppl�ment familial|    388 778,18|
|         Traitements| 20 772 768,29|

 
[Lien vers la base d�taill�e des cumuls des lignes de paie](Bases/R�glementation/cumul.lignes.paie.csv)
 
[Lien vers la base agr�g�e des cumuls des lignes de paie](Bases/R�glementation/cumul.total.lignes.paie.csv)
 
 
*Avertissement : les rappels comprennent �galement les rappels de cotisations et d�ductions diverses.*    
  




## 5.9 Contr�le du suppl�ment familial de traitement   
 


Pour les agents n'ayant pas d'enfant signal� en base, il a �t� d�tect� 13 bulletins de paie pr�sentant un paiement du SFT apparemment anormal.

 
[Lien vers la base des paiements de SFT � agents sans enfant signal�](Bases/R�glementation/Paie.sans.enfant.r�duit.csv)
 


Pour les agents ayant au moins un enfant, il a �t� d�tect� 70 bulletins de paie pr�sentant un �cart de paiement du SFT sup�rieur � 1 euro.

 
[Lien vers la base des �carts de paiement sur SFT](Bases/R�glementation/controle.sft.csv)
 



# Annexe
## Liens compl�mentaires

[Lien vers le fichier des personnels](Bases/Effectifs/Cat�gories des personnels.csv)
 
## Fiabilit� du traitement statistique   
*Doublons*      

Attention : Alta�r a d�tect� des lignes dupliqu�es alors qu'aucun retraitement des lignes dupliqu�es n'est pr�vu par d�faut.

 



 
*Tests de fiabilit� sur le renseignement des heures et des quotit�s*    
  



    

Nombre de bulletins :  63 961

    

Les heures de travail ont �t� redress�es avec la m�thode  des quotit�s.

   

 Nombre de bulletins de paie redress�s : 5 983

   

 Pourcentage de redressements : 9,35 % des bulletins de paie.

 


Pourcentage d'heures renseign�es (apr�s redressement �ventuel): 92,9 %

  


Pourcentage de quotit�s renseign�es : 99,2 %

  


Nombre de bulletins � heures et quotit�s :  59416 [ 92,9 %]

  


Nombre de bulletins � heures sans quotit�s :  0 [ 0 %]

  


Nombre de bulletins � quotit�s sans heures :  4005 [ 6,3 %]

  


Nombre de bulletins apparemment inactifs :  540 [ 0,8 %]

  



 

Nombre de bulletins de paie de salaires vers�s pour un champ Heures = 0 :  4 003 [ 6,3 %]

  



  
[Lien vers la base de donn�es des salaires vers�s pour Heures=0](Bases/Fiabilit�/base.heures.nulles.salaire.nonnull.csv)   
[Lien vers la base de donn�es des salaires vers�s � quotit� ind�finie](Bases/Fiabilit�/base.quotit�.ind�finie.salaire.nonnull.csv)   

# Tableau des personnels : renseigner la cat�gorie

Utiliser les codes : A, B, C, ELU, AUTRES

En cas de changement de cat�gorie en cours de p�riode, utiliser la cat�gorie AUTRES
Cela peut conduire � modifier manuellement le fichier Cat�gories des personnels.csv



Non g�n�r�  [anonymisation]


