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




<p class = "centered"><b>Exercices 2010 � 2013 </b></p>
<p class = "author">Fabrice Nicol</h1>




dim. 06 d�c. 2015      
     
P�riode sous revue : 2010 - 2013    
Nombre d'exercices : 4        
    



# 1. Statistiques de population

### 1.1 Effectifs



 



&nbsp;*Tableau 1*   


|                                                             | 2010  | 2011  | 2012  | 2013  |
|:------------------------------------------------------------|:-----:|:-----:|:-----:|:-----:|
|Total effectifs (a)                                          | 386,0 | 391,0 | 407,0 | 448,0 |
|&nbsp;&nbsp;&nbsp;dont pr�sents 12 mois                      | 224,0 | 227,0 | 237,0 | 260,0 |
|&nbsp;&nbsp;&nbsp;dont fonctionnaires (b)                    | 150,0 | 163,0 | 171,0 | 179,0 |
|&nbsp;&nbsp;&nbsp;dont fonct. pr�sents 12 mois               | 137,0 | 146,0 | 154,0 | 165,0 |
|&nbsp;&nbsp;&nbsp;dont non titulaires                        | 64,0  | 61,0  | 83,0  | 80,0  |
|&nbsp;&nbsp;&nbsp;dont �lus                                  | 17,0  | 17,0  | 17,0  | 14,0  |
|&nbsp;&nbsp;&nbsp;dont �lus pr�sents 12 mois                 | 17,0  | 17,0  | 14,0  | 14,0  |
|&nbsp;&nbsp;&nbsp;dont vacataires d�tect�s (c)               | 33,0  | 34,0  | 40,0  | 56,0  |
|&nbsp;&nbsp;&nbsp;dont assistantes maternelles d�tect�es (c) |  0,0  | 13,0  | 15,0  | 14,0  |
|Postes non actifs (g)                                        | 42,0  | 77,0  | 66,0  | 83,0  |
|Postes annexes (g)                                           | 148,0 | 160,0 | 139,0 | 182,0 |
|Postes actifs non annexes (g)                                | 221,0 | 214,0 | 251,0 | 252,0 |
|Total ETP/ann�e (d)                                          | 254,9 | 221,0 | 252,3 | 266,1 |
|Total ETPT/ann�e (e)                                         | 211,0 | 183,2 | 219,4 | 224,5 |
|Total ETPT/ann�e personnes en place (f)(g)                   |  0,0  |  0,0  | 125,6 | 142,9 |
|Total ETPT/ann�e fonctionnaires (g)                          | 126,4 | 131,2 | 141,8 | 142,0 |
|Total ETPT/ann�e titulaires � temps complet (g)              | 86,0  | 82,5  | 82,5  | 85,0  |
|Total ETPT non titulaires (g)                                | 47,1  | 35,4  | 55,4  | 57,7  |
|Total ETPT autre statut                                      |  0,0  |  0,0  |  1,5  |  3,2  |
|Total ETPT postes non actifs (g)                             |  0,0  |  0,0  |  0,0  |  0,0  |
|Total ETPT postes annexes (g)                                | 24,4  | 13,1  | 14,4  | 13,2  |
|Total ETPT postes actifs non annexes (g)                     | 186,6 | 170,1 | 205,1 | 211,3 |


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


La dur�e du travail prise en compte dans la base de donn�es est de  151,67  h par mois.

     
  
[Lien vers la base des effectifs](Bases/Effectifs/tableau.effectifs.csv)





### 1.2 Pyramide des �ges, personnels non �lus
 
&nbsp;*Tableau 2*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2010 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  21                  |          |
| 1er quartile  |                  35                  |          |
|    M�diane    |                  42                  |          |
|    Moyenne    |                 43,2                 |          |
| 3�me quartile |                  51                  |          |
|    Maximum    |                  77                  |          |

 

![plot of chunk unnamed-chunk-10](figure/unnamed-chunk-10-1.png) 

 
&nbsp;*Tableau 3*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  19                  |          |
| 1er quartile  |                  36                  |          |
|    M�diane    |                  44                  |          |
|    Moyenne    |                43,97                 |          |
| 3�me quartile |                  53                  |          |
|    Maximum    |                  79                  |          |

 
 

![plot of chunk unnamed-chunk-12](figure/unnamed-chunk-12-1.png) 


### 1.3 Pyramide des �ges, personnels non titulaires
 
&nbsp;*Tableau 4*   
   


|  Statistique  | �ge des personnels non titulaires<br>au 31/12/2010 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         21                         |          |
| 1er quartile  |                         33                         |          |
|    M�diane    |                         44                         |          |
|    Moyenne    |                       44,13                        |          |
| 3�me quartile |                         53                         |          |
|    Maximum    |                         77                         |          |

 

![plot of chunk unnamed-chunk-14](figure/unnamed-chunk-14-1.png) 

 
&nbsp;*Tableau 5*   
   


|  Statistique  | �ge des personnels non titulaires<br>au 31/12/2013 | Effectif |
|:-------------:|:--------------------------------------------------:|:--------:|
|    Minimum    |                         19                         |          |
| 1er quartile  |                         33                         |          |
|    M�diane    |                         42                         |          |
|    Moyenne    |                       43,81                        |          |
| 3�me quartile |                         55                         |          |
|    Maximum    |                         79                         |          |

![plot of chunk unnamed-chunk-15](figure/unnamed-chunk-15-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.nontit.csv)  
 


### 1.4 Pyramide des �ges, personnels fonctionnaires stagiaires et titulaires
 
&nbsp;*Tableau 6*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2010 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  21                  |          |
| 1er quartile  |                  35                  |          |
|    M�diane    |                  42                  |          |
|    Moyenne    |                42,47                 |   144    |
| 3�me quartile |                  49                  |          |
|    Maximum    |                  62                  |          |

 


![plot of chunk unnamed-chunk-17](figure/unnamed-chunk-17-1.png) 

 
&nbsp;*Tableau 7*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  23                  |          |
| 1er quartile  |                  38                  |          |
|    M�diane    |                  45                  |          |
|    Moyenne    |                44,59                 |   169    |
| 3�me quartile |                  51                  |          |
|    Maximum    |                  65                  |          |



![plot of chunk unnamed-chunk-19](figure/unnamed-chunk-19-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.fonctionnaires.csv)  
 
### 1.5 Pyramide des �ges, personnels permanents (titulaires, stagiaires et non titulaires)
 
&nbsp;*Tableau 8*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2010 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  21                  |          |
| 1er quartile  |                  35                  |          |
|    M�diane    |                  42                  |          |
|    Moyenne    |                 43,2                 |          |
| 3�me quartile |                  51                  |          |
|    Maximum    |                  77                  |          |

 

![plot of chunk unnamed-chunk-21](figure/unnamed-chunk-21-1.png) 

&nbsp;*Tableau 9*   
   


|  Statistique  | �ge des personnels <br>au 31/12/2013 | Effectif |
|:-------------:|:------------------------------------:|:--------:|
|    Minimum    |                  19                  |          |
| 1er quartile  |                  36                  |          |
|    M�diane    |                  44                  |          |
|    Moyenne    |                44,28                 |          |
| 3�me quartile |                  53                  |          |
|    Maximum    |                  79                  |          |



![plot of chunk unnamed-chunk-23](figure/unnamed-chunk-23-1.png) 

 
[Lien vers la base des �ges](Bases/Effectifs/Bulletins.paie.nir.permanents.csv)  
 
### 1.6 Effectifs des personnels par dur�e de service

**Personnels en fonction (hors �lus) des exercices 2010 � 2013 inclus :**

 
&nbsp;*Tableau 10*   
   


| Plus de 2 ans | Moins de 2 ans | Moins d'un an | Moins de six mois |
|:-------------:|:--------------:|:-------------:|:-----------------:|
|      194      |      330       |      103      |        29         |



![plot of chunk unnamed-chunk-25](figure/unnamed-chunk-25-1.png) 


**Effectifs (hors �lus)**   




 
&nbsp;*Tableau 11*   
   


|                  | 2010 | 2011 | 2012 | 2013 |
|:-----------------|:----:|:----:|:----:|:----:|
|Plus de deux ans  |  0   | 177  | 194  | 190  |
|Moins de deux ans | 221  |  37  |  55  |  62  |
|Total             | 221  | 214  | 249  | 252  |



**Nota :**
*Personnels en place : ayant servi au moins deux ann�es cons�cutives pendant la p�riode.*     
*Plus/moins de deux ans : plus/mois de 730 jours sur la p�riode sous revue.*   



  
  



   
# 2. R�mun�rations brutes : analyse pour l'exercice 2010    
   
## 2.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des r�mun�rations brutes pour l'exercice 2010
 
*Personnels (hors �lus)*     
 
&nbsp;*Tableau 12*   
   


|           Agr�gats            |   k&euro;    |
|-------------------------------|--------------|
|    Brut annuel (bulletins)    | 5 307 936,4  |
|    Brut annuel (lignes) :     | 10 485 230,7 |
|       \ dont \ Primes :       | 1 028 260,9  |
| \ dont \ Autres r�mun�rations |   50 427,2   |
|      Part de primes en %      |     19,4     |

 
**D�finitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
 
**Tests de coh�rence**

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :  
 
 
&nbsp;*Tableau 13*   
   


|     Agr�gats      |   k&euro;    |
|-------------------|--------------|
| Bulletins de paie | 5 307 936,4  |
|  Lignes de paie   | 10 485 230,7 |
|    Diff�rence     | -5 177 294,3 |


� comparer aux soldes des comptes 641 et 648 du compte de gestion.

  
## 2.2 Masse salariale brute des fonctionnaires

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*  


![plot of chunk unnamed-chunk-133](figure/unnamed-chunk-133-1.png) ![plot of chunk unnamed-chunk-133](figure/unnamed-chunk-133-2.png) ![plot of chunk unnamed-chunk-133](figure/unnamed-chunk-133-3.png) ![plot of chunk unnamed-chunk-133](figure/unnamed-chunk-133-4.png) ![plot of chunk unnamed-chunk-133](figure/unnamed-chunk-133-5.png) ![plot of chunk unnamed-chunk-133](figure/unnamed-chunk-133-6.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio r�mun�ration / quotit�*  
   
**Effectif : 149**

**Tests de coh�rence**



Somme des r�mun�rations brutes vers�es aux personnels titulaires et stagiaires :

 
&nbsp;*Tableau 14*   
   


|            Agr�gats             |   k&euro;   |
|---------------------------------|-------------|
|     Brut annuel (bulletins)     | 3 455 355,0 |
|     Brut annuel (lignes) :      | 6 868 030,9 |
|       \ dont \ \ primes :       |  668 731,8  |
| \ dont \ autres r�mun�rations : |  29 346,0   |
|       Part de primes en %       |    19,4     |


**D�finitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, Suppl�ment familial de traitement et Indemnit� de r�sidence       
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   

**Tests de coh�rence**

Somme des r�mun�rations brutes vers�es aux personnels (fonctionnaires) :

 
&nbsp;*Tableau 15*   
   


|     Agr�gats      |   k&euro;    |
|-------------------|--------------|
| Bulletins de paie | 3 455 355,0  |
|  Lignes de paie   | 6 868 030,9  |
|    Diff�rence     | -3 412 675,9 |


A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.

**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2010**     
 
&nbsp;*Tableau 16*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres r�mun�rations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 7 249|    567,2|                  0,0|         |
|  1er quartile|                35 888|  2 771,3|                  0,0|         |
|       M�diane|                39 177|  3 791,3|                  0,0|         |
|       Moyenne|                43 141|  4 702,7|                209,6|      140|
| 3�me quartile|                48 885|  4 689,8|                123,6|         |
|       Maximum|               155 501| 60 297,8|              1 475,8|         |

 
&nbsp;*Tableau 17*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la r�m. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     8 735|      4 379|                 0|                        4,744|         |
|  1er quartile|                    38 820|     19 430|            20 489|                       13,633|         |
|       M�diane|                    43 503|     21 751|            22 857|                       17,401|         |
|       Moyenne|                    47 843|     23 950|            25 227|                       18,106|      140|
| 3�me quartile|                    52 698|     26 349|            27 508|                       20,969|         |
|       Maximum|                   215 799|    109 277|           109 277|                       55,179|         |

  
*Hors vacataires identifi�s, assistantes maternelles, �lus locaux et pour les postes actifs non annexes*  

**Cat�gorie A**




 
&nbsp;*Tableau 18*   
   


|   Statistique| Traitement indiciaire| Primes| Autres r�mun�rations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                50 007|  2 160|                    0|
|  1er quartile|                65 824|  7 045|                    0|
|       M�diane|                70 007|  8 501|                    0|
|       Moyenne|                76 348| 12 564|                    0|
| 3�me quartile|                79 517| 12 601|                    0|
|       Maximum|               155 501| 60 298|                    0|


 
&nbsp;*Tableau 19*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              29 064|                   34 026|                                5,722|
|  1er quartile|              35 885|                   37 560|                               17,789|
|       M�diane|              38 812|                   41 290|                               25,612|
|       Moyenne|              44 548|                   46 646|                               24,773|
| 3�me quartile|              44 522|                   46 884|                               28,480|
|       Maximum|             109 277|                  109 277|                               55,179|


**Effectif : 15**  

**Cat�gorie B**

 
&nbsp;*Tableau 20*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres r�mun�rations|
|-------------:|---------------------:|--------:|--------------------:|
|       Minimum|                22 138|    567,2|                0,000|
|  1er quartile|                38 081|  2 866,7|                0,000|
|       M�diane|                45 236|  3 715,1|                0,000|
|       Moyenne|                43 971|  4 086,4|                2,555|
| 3�me quartile|                49 434|  4 694,7|                0,000|
|       Maximum|                64 623| 11 656,4|              120,070|


 
&nbsp;*Tableau 21*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              11 956|                        0|                                4,744|
|  1er quartile|              20 856|                   23 002|                               12,693|
|       M�diane|              24 342|                   25 926|                               16,023|
|       Moyenne|              24 057|                   24 823|                               16,581|
| 3�me quartile|              28 464|                   28 923|                               20,426|
|       Maximum|              39 280|                   36 015|                               29,675|


**Effectif : 47**

**Cat�gorie C**

 
&nbsp;*Tableau 22*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres r�mun�rations|
|-------------:|---------------------:|--------:|--------------------:|
|       Minimum|                 7 249|    770,5|                 0,00|
|  1er quartile|                35 263|  2 624,8|                 0,00|
|       M�diane|                37 156|  3 543,8|                29,43|
|       Moyenne|                36 255|  3 562,2|               374,69|
| 3�me quartile|                39 622|  4 330,9|               878,82|
|       Maximum|                50 554| 10 426,6|             1 475,76|


 
&nbsp;*Tableau 23*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               4 379|                   15 825|                                 8,33|
|  1er quartile|              18 966|                   19 752|                                14,66|
|       M�diane|              20 265|                   20 916|                                17,34|
|       Moyenne|              19 925|                   21 351|                                17,74|
| 3�me quartile|              21 915|                   22 694|                                19,81|
|       Maximum|              30 102|                   30 102|                                34,64|

**Effectif : 78**


######      <br>

## 2.3 Contractuels, vacataires et stagiaires inclus   
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   


![plot of chunk unnamed-chunk-146](figure/unnamed-chunk-146-1.png) 

  
**Nota :**
Ne sont retenues que les r�mun�rations sup�rieures � 1 000 k&euro;.
Les �lus ne sont pas pris en compte.
  

![plot of chunk unnamed-chunk-147](figure/unnamed-chunk-147-1.png) 





  
**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2010**     
  
 
&nbsp;*Tableau 24*   
   


|   Statistique|   Primes| Autres r�mun�rations| Effectif|
|-------------:|--------:|--------------------:|--------:|
|       Minimum|   -898,1|                  0,0|         |
|  1er quartile|    675,5|                  0,0|         |
|       M�diane|  2 443,9|                  0,0|         |
|       Moyenne|  4 072,1|                254,5|       81|
| 3�me quartile|  4 898,0|                428,4|         |
|       Maximum| 43 409,2|              1 970,5|         |

 
&nbsp;*Tableau 25*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               4 143|                    9 870|         |
|  1er quartile|               9 148|                   18 793|         |
|       M�diane|              16 948|                   22 470|         |
|       Moyenne|              20 137|                   25 939|       81|
| 3�me quartile|              22 650|                   25 412|         |
|       Maximum|             103 993|                  103 993|         |






  
  



   
# 3. R�mun�rations brutes : analyse pour l'exercice 2013    
   
## 3.1 Masse salariale brute de l'ensemble des agents     
    
 



### Cumuls des r�mun�rations brutes pour l'exercice 2013
 
*Personnels (hors �lus)*     
 
&nbsp;*Tableau 26*   
   


|           Agr�gats            |   k&euro;   |
|-------------------------------|-------------|
|    Brut annuel (bulletins)    | 6 606 466,5 |
|    Brut annuel (lignes) :     | 6 372 701,1 |
|       \ dont \ Primes :       | 1 145 163,5 |
| \ dont \ Autres r�mun�rations |  17 876,2   |
|      Part de primes en %      |    17,3     |

 
**D�finitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*    
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, indemnit�s d'�lu le cas �ch�ant, Suppl�ment familial de traitement et Indemnit� de r�sidence        
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   
 
**Tests de coh�rence**

Somme des r�mun�rations brutes vers�es aux personnels (non �lus) :  
 
 
&nbsp;*Tableau 27*   
   


|     Agr�gats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 6 606 466,5 |
|  Lignes de paie   | 6 372 701,1 |
|    Diff�rence     |  233 765,4  |


� comparer aux soldes des comptes 641 et 648 du compte de gestion.

  
## 3.2 Masse salariale brute des fonctionnaires

*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*  


![plot of chunk unnamed-chunk-156](figure/unnamed-chunk-156-1.png) ![plot of chunk unnamed-chunk-156](figure/unnamed-chunk-156-2.png) ![plot of chunk unnamed-chunk-156](figure/unnamed-chunk-156-3.png) ![plot of chunk unnamed-chunk-156](figure/unnamed-chunk-156-4.png) ![plot of chunk unnamed-chunk-156](figure/unnamed-chunk-156-5.png) ![plot of chunk unnamed-chunk-156](figure/unnamed-chunk-156-6.png) 

   
**Nota :**   
*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio r�mun�ration / quotit�*  
   
**Effectif : 172**

**Tests de coh�rence**



Somme des r�mun�rations brutes vers�es aux personnels titulaires et stagiaires :

 
&nbsp;*Tableau 28*   
   


|            Agr�gats             |   k&euro;   |
|---------------------------------|-------------|
|     Brut annuel (bulletins)     | 4 339 731,2 |
|     Brut annuel (lignes) :      | 4 295 767,1 |
|       \ dont \ \ primes :       |  823 806,8  |
| \ dont \ autres r�mun�rations : |  16 452,9   |
|       Part de primes en %       |    19,0     |


**D�finitions :**

 *Brut annuel (bulletins)*   : somme du champ *Brut*   
 *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
 *Primes*                    : indemnit�s sauf remboursements, certaines IJSS, Suppl�ment familial de traitement et Indemnit� de r�sidence       
 *Autres r�mun�rations*      : acomptes, retenues sur brut, r�mun�rations diverses, rappels   

**Tests de coh�rence**

Somme des r�mun�rations brutes vers�es aux personnels (fonctionnaires) :

 
&nbsp;*Tableau 29*   
   


|     Agr�gats      |   k&euro;   |
|-------------------|-------------|
| Bulletins de paie | 4 339 731,2 |
|  Lignes de paie   | 4 295 767,1 |
|    Diff�rence     |  43 964,1   |


A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.

**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2013**     
 
&nbsp;*Tableau 30*   
   


|   Statistique| Traitement indiciaire|   Primes| Autres r�mun�rations| Effectif|
|-------------:|---------------------:|--------:|--------------------:|--------:|
|       Minimum|                 4 570|    589,6|                  0,0|         |
|  1er quartile|                17 866|  3 006,4|                  0,0|         |
|       M�diane|                19 313|  3 633,7|                  0,0|         |
|       Moyenne|                21 545|  5 167,9|                104,1|      158|
| 3�me quartile|                24 577|  5 418,1|                  0,0|         |
|       Maximum|                53 327| 62 963,3|              6 600,0|         |

 
&nbsp;*Tableau 31*   
   


|   Statistique| Total lignes hors rappels| Total brut| SMPT brut en EQTP| Part de la r�m. indemnitaire| Effectif|
|-------------:|-------------------------:|----------:|-----------------:|----------------------------:|--------:|
|       Minimum|                     5 160|      5 324|                 0|                        5,262|         |
|  1er quartile|                    21 207|     20 939|            22 198|                       13,999|         |
|       M�diane|                    24 087|     24 592|            25 255|                       15,876|         |
|       Moyenne|                    26 713|     26 968|            28 027|                       17,430|      158|
| 3�me quartile|                    28 202|     28 494|            30 757|                       20,153|         |
|       Maximum|                   116 290|    119 238|           119 238|                       52,805|         |

  
*Hors vacataires identifi�s, assistantes maternelles, �lus locaux et pour les postes actifs non annexes*  

**Cat�gorie A**




 
&nbsp;*Tableau 32*   
   


|   Statistique| Traitement indiciaire| Primes| Autres r�mun�rations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                18 883|  2 165|                  0,0|
|  1er quartile|                28 282|  7 724|                  0,0|
|       M�diane|                36 172| 10 613|                  0,0|
|       Moyenne|                35 386| 13 839|                381,7|
| 3�me quartile|                41 964| 15 384|                159,7|
|       Maximum|                53 327| 62 963|              6 600,0|


 
&nbsp;*Tableau 33*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              23 415|                        0|                                5,262|
|  1er quartile|              36 968|                   39 149|                               17,919|
|       M�diane|              44 561|                   46 703|                               25,904|
|       Moyenne|              49 133|                   49 806|                               25,537|
| 3�me quartile|              50 646|                   56 122|                               32,711|
|       Maximum|             119 238|                  119 238|                               52,805|


**Effectif : 21**  

**Cat�gorie B**

 
&nbsp;*Tableau 34*   
   


|   Statistique| Traitement indiciaire| Primes| Autres r�mun�rations|
|-------------:|---------------------:|------:|--------------------:|
|       Minimum|                12 486|  1 318|                 0,00|
|  1er quartile|                19 672|  2 289|                 0,00|
|       M�diane|                23 480|  4 135|                 0,00|
|       Moyenne|                22 591|  4 221|                52,68|
| 3�me quartile|                25 088|  5 808|                28,93|
|       Maximum|                31 521|  8 761|               297,00|


 
&nbsp;*Tableau 35*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|              14 657|                        0|                                6,333|
|  1er quartile|              25 433|                   26 037|                                9,507|
|       M�diane|              27 333|                   28 341|                               15,113|
|       Moyenne|              27 045|                   27 582|                               15,458|
| 3�me quartile|              30 362|                   31 707|                               20,243|
|       Maximum|              35 844|                   38 826|                               29,097|


**Effectif : 55**

**Cat�gorie C**

 
&nbsp;*Tableau 36*   
   


|   Statistique| Traitement indiciaire|  Primes| Autres r�mun�rations|
|-------------:|---------------------:|-------:|--------------------:|
|       Minimum|                 4 570|   589,6|                 0,00|
|  1er quartile|                17 196| 3 006,4|                 0,00|
|       M�diane|                18 012| 3 537,8|                 0,00|
|       Moyenne|                17 298| 3 582,1|                67,55|
| 3�me quartile|                18 780| 3 906,1|                 0,00|
|       Maximum|                23 826| 8 866,6|             3 506,44|


 
&nbsp;*Tableau 37*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Part de la r�mun�ration indemnitaire|
|-------------:|-------------------:|------------------------:|------------------------------------:|
|       Minimum|               5 324|                        0|                                10,85|
|  1er quartile|              20 088|                   21 013|                                14,68|
|       M�diane|              21 659|                   23 015|                                15,58|
|       Moyenne|              21 240|                   22 748|                                16,68|
| 3�me quartile|              23 532|                   24 347|                                17,77|
|       Maximum|              29 811|                   39 118|                                36,70|

**Effectif : 82**


######      <br>

## 3.3 Contractuels, vacataires et stagiaires inclus   
  
*Les assistantes maternelles et les vacataires sont ici inclus, pour les postes non annexes*   


![plot of chunk unnamed-chunk-169](figure/unnamed-chunk-169-1.png) 

  
**Nota :**
Ne sont retenues que les r�mun�rations sup�rieures � 1 000 k&euro;.
Les �lus ne sont pas pris en compte.
  

![plot of chunk unnamed-chunk-170](figure/unnamed-chunk-170-1.png) 





  
**Formation et distribution du salaire brut moyen par t�te (SMPT) en EQTP pour l'ann�e 2013**     
  
 
&nbsp;*Tableau 38*   
   


|   Statistique|   Primes| Autres r�mun�rations| Effectif|
|-------------:|--------:|--------------------:|--------:|
|       Minimum|      0,0|                0,000|         |
|  1er quartile|    178,2|                0,000|         |
|       M�diane|  2 156,5|                0,000|         |
|       Moyenne|  2 990,9|                2,298|       94|
| 3�me quartile|  5 116,3|                0,000|         |
|       Maximum| 13 876,1|              216,000|         |

 
&nbsp;*Tableau 39*   
   


|   Statistique| Total r�mun�rations| Total r�mun�rations EQTP| Effectif|
|-------------:|-------------------:|------------------------:|--------:|
|       Minimum|               4 859|                        0|         |
|  1er quartile|              12 359|                   19 193|         |
|       M�diane|              20 084|                   22 995|         |
|       Moyenne|              19 528|                   24 705|       94|
| 3�me quartile|              23 746|                   25 397|         |
|       Maximum|              71 487|                  127 800|         |

 
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
  




# 4. R�mun�rations nettes : �volutions sur la p�riode 2010 - 2013    

Nombre d'exercices: 4   
 
**Les donn�es pr�sent�es dans cette section sont toutes relatives � des r�mun�rations nettes en �quivalent temps plein (EQTP)**   
Les �lus, les vacataires et les assistantes maternelles ont �t� retir�s de la population �tudi�e       
Seuls sont consid�r�s les postes actifs et non annexes   
   
*Nota :*   
*EQTP = Equivalent temps plein  = 12 . moyenne du ratio r�mun�ration / quotit�*    
   
## 4.1 Distribution de la r�mun�ration nette moyenne sur la p�riode    

![plot of chunk unnamed-chunk-33](figure/unnamed-chunk-33-1.png) 



![plot of chunk unnamed-chunk-34](figure/unnamed-chunk-34-1.png) 


[Lien vers la base de donn�es synth�tique](Bases/R�mun�rations/Analyse.variations.synth�se.csv)
[Lien vers la base de donn�es d�taill�e par ann�e](Bases/R�mun�rations/Analyse.variations.par.exercice.csv)

## 4.2 Evolutions des r�mun�rations nettes sur la p�riode 2010 - 2013   

### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors �lus)





   
**Salaire net moyen par t�te (SMPT net) en EQTP, hors �lus**         
      
 
&nbsp;*Tableau 42*   
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-----------------:|:-------------:|
| 2010  |           4 600,2           |               |      21 102       |               |
| 2011  |           4 507,5           |     21,8      |      21 775       |     9,26      |
| 2012  |           5 148,2           |               |      21 362       |               |
| 2013  |           5 602,5           |               |      23 056       |               |

  
**Effet de noria sur salaires nets et taux de remplacements**       
  
**Effet de noria** : *diff�rence entre la r�mun�ration annuelle des entrants � l'ann�e N et des sortants � l'ann�e N-1*.  
*Usuellement calcul�e sur les r�mun�rations brutes, ici sur les r�mun�rations nettes EQTP*  
*afin d'appr�cier l'impact de cet effet sur l'�volution des r�mun�rations nette moyennes calcul�e au tableau pr�c�dent.*               
 
&nbsp;*Tableau 43*   
   


| Ann�e | Noria EQTP (&euro;) | En % de la MS N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-----------------:|:------------------:|:-------------------------:|
| 2011  |       673,47        |       2,49        |        170         |            93             |
| 2012  |      -3 294,4       |       -1,02       |         14         |            6,3            |
| 2013  |       -600,14       |      -0,162       |         14         |            6,2            |


*MS N-1 : masse salariale nette de l'ann�e n-1.*   
      
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) en EQTP**         
      
 
&nbsp;*Tableau 44*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|         10 080|
|  1er quartile|         16 040|
|       M�diane|         18 540|
|       Moyenne|         21 100|
| 3�me quartile|         21 880|
|       Maximum|         92 590|



|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|          8 658|
|  1er quartile|         17 160|
|       M�diane|         19 740|
|       Moyenne|         23 060|
| 3�me quartile|         23 280|
|       Maximum|        275 900|

 
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
**Ensemble**  
   
&nbsp;*Tableau 48*   
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2010  |           2 875,1           |               |          20 986           |               |
| 2011  |           3 216,7           |     24,3      |          22 653           |     11,3      |
| 2012  |           3 473,5           |               |          22 702           |               |
| 2013  |           3 574,6           |               |          23 363           |               |

  
**Cat�gorie A**  

&nbsp;*Tableau 49*   
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2010  |           578,02            |               |          38 534           |               |
| 2011  |           812,31            |     47,6      |          42 753           |     10,7      |
| 2012  |           791,75            |               |          39 588           |               |
| 2013  |            853,4            |               |          42 670           |               |

  
**Cat�gorie B**    
    
&nbsp;*Tableau 50*   
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2010  |           952,38            |               |          21 645           |               |
| 2011  |            1 095            |      28       |          22 346           |     8,34      |
| 2012  |           1 225,7           |               |          23 127           |               |
| 2013  |           1 219,5           |               |          23 451           |               |

  
**Cat�gorie C**     
   
&nbsp;*Tableau 51*   
   


| Ann�e | R�m. nette totale (k&euro;) | Variation (%) | SMPT net en EQTP (&euro;) | Variation (%) |
|:-----:|:---------------------------:|:-------------:|:-------------------------:|:-------------:|
| 2010  |           1 344,7           |               |          17 240           |               |
| 2011  |           1 309,5           |     11,7      |          17 695           |     7,54      |
| 2012  |            1 456            |               |          18 200           |               |
| 2013  |           1 501,8           |               |          18 540           |               |

   



  
**Effet de noria sur salaires nets et taux de remplacements**       
 
&nbsp;*Tableau 52*   
   


| Ann�e | Noria EQTP (&euro;) | En % de la  MSN N-1 | Remplacements EQTP | Taux de remplacements (%) |
|:-----:|:-------------------:|:-------------------:|:------------------:|:-------------------------:|
| 2011  |       1 786,2       |        16,8         |        127         |            96             |
| 2012  |       -57 491       |        -11,4        |         3          |            1,8            |
| 2013  |       7 951,4       |        1,37         |         3          |            1,8            |

    
*MS N-1 : masse salariale nette de l'ann�e n-1.*   
      
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) en EQTP**         
 
&nbsp;*Tableau 53*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|         11 930|
|  1er quartile|         16 540|
|       M�diane|         18 840|
|       Moyenne|         20 990|
| 3�me quartile|         22 350|
|       Maximum|         92 590|



|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|          8 658|
|  1er quartile|         17 840|
|       M�diane|         20 640|
|       Moyenne|         23 360|
| 3�me quartile|         24 560|
|       Maximum|         98 580|

   



  
**Evolution du SMPT net des titulaires � temps complet**     
  
 
&nbsp;*Tableau 54*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | SMPT (&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:-------------:|:-------------:|
| 2010  |                1 825                |               |    21 221     |               |
| 2011  |               1 843,9               |     11,1      |    22 215     |     12,4      |
| 2012  |               1 947,7               |               |    23 467     |               |
| 2013  |               2 027,3               |               |    23 851     |               |

   
**Distribution et variation sur la p�riode du salaire moyen net par t�te (SMPT net) des titulaires � temps complet**         
      
 
&nbsp;*Tableau 55*   
   


|   Statistique| Premi�re ann�e|
|-------------:|--------------:|
|       Minimum|         11 930|
|  1er quartile|         16 500|
|       M�diane|         18 880|
|       Moyenne|         21 220|
| 3�me quartile|         22 530|
|       Maximum|         92 590|

  


|   Statistique| Derni�re ann�e|
|-------------:|--------------:|
|       Minimum|         14 190|
|  1er quartile|         17 820|
|       M�diane|         20 450|
|       Moyenne|         23 850|
| 3�me quartile|         23 720|
|       Maximum|         98 580|


[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)


## 4.3 Glissement vieillesse-technicit� (GVT)   

### 4.3.1 Ensemble des personnels   
  
*Cette section est consacr�e � la r�mun�ration moyenne des personnes en place (RMPP), d�finies comme pr�sentes deux ann�es enti�res cons�cutives avec la m�me quotit�*   
*L'�volution de la RMPP permet d'�tudier le glissement vieillesse-technicit� "positif", � effectifs constants sur deux ann�es*      
*Le GVT positif est d� aux mesures statutaires et individuelles, � l'avancement et aux changements d'activit�*  

![plot of chunk unnamed-chunk-55](figure/unnamed-chunk-55-1.png) 






  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 56*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2011  |                  0                  |               |         NaN          |               |
| 2012  |               2 979,3               |      Inf      |        22 570        |      NaN      |
| 2013  |               3 464,8               |               |        23 098        |               |

   
**Distribution et variation sur la p�riode de la r�mun�ration nette des personnes en place**                
 
&nbsp;*Tableau 57*   
   



 


|   Statistique| Premi�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|             NA|         |
|  1er quartile|             NA|         |
|       M�diane|             NA|         |
|       Moyenne|            NaN|        0|
| 3�me quartile|             NA|         |
|       Maximum|             NA|         |

 
&nbsp;*Tableau 58*   
   


|   Statistique| Derni�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|         11 120|         |
|  1er quartile|         17 710|         |
|       M�diane|         20 230|         |
|       Moyenne|         22 500|      206|
| 3�me quartile|         23 660|         |
|       Maximum|         98 580|         |


*Variation individuelle de r�mun�ration nette en EQTP pour les personnels pr�sents la premi�re et la derni�re ann�e*   
 
&nbsp;*Tableau 59*   
   


|  Statistique| Variation normalis�e (%)| Variation annuelle moyenne normalis�e (%)| Effectif|
|------------:|------------------------:|-----------------------------------------:|--------:|
|      Minimum|                  logical|                                   logical|         |
| 1er quartile|                   NA's:0|                                    NA's:0|         |

   
### 4.3.2 Titulaires et stagiaires     
  



  
**Evolution de la RMPP nette en EQTP**     
  
 
&nbsp;*Tableau 60*   
   


| Ann�e | R�mun�ration nette totale (k&euro;) | Variation (%) | RMPP nette (k&euro;) | Variation (%) |
|:-----:|:-----------------------------------:|:-------------:|:--------------------:|:-------------:|
| 2011  |                  0                  |               |         NaN          |               |
| 2012  |               2 682,3               |      Inf      |        22 925        |      NaN      |
| 2013  |               2 726,8               |               |        23 306        |               |

   
**Distribution et variation sur la p�riode de la r�mun�ration nette des fonctionnaires en place**                
 
&nbsp;*Tableau 61*   
   
 


|   Statistique| Premi�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|             NA|         |
|  1er quartile|             NA|         |
|       M�diane|             NA|         |
|       Moyenne|            NaN|        0|
| 3�me quartile|             NA|         |
|       Maximum|             NA|         |

 
&nbsp;*Tableau 62*   
   


|   Statistique| Derni�re ann�e| Effectif|
|-------------:|--------------:|--------:|
|       Minimum|         11 930|         |
|  1er quartile|         17 350|         |
|       M�diane|         20 020|         |
|       Moyenne|         22 690|      288|
| 3�me quartile|         23 690|         |
|       Maximum|         98 580|         |


*Variation individuelle de r�mun�ration nette en EQTP pour les personnels pr�sents la premi�re et la derni�re ann�e*   
 
&nbsp;*Tableau 63*   
   


|  Statistique| Variation normalis�e (%)| Variation annuelle moyenne normalis�e (%)| Effectif|
|------------:|------------------------:|-----------------------------------------:|--------:|
|      Minimum|                  logical|                                   logical|         |
| 1er quartile|                   NA's:0|                                    NA's:0|         |



[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)

**Nota**   
*Personnes en place :* en fonction au moins deux ann�es cons�cutives sur la p�riode 2010 � 2013    
*Variation sur la p�riode d'activit� :* entre l'arriv�e et le d�part de la personne      
*Variation normalis�e :* conforme � la d�finition INSEE (pr�sente en d�but et en fin de p�riode avec la m�me quotit�)  
 
**Commentaire**       
Les diff�rences �ventuelles constat�es entre l'�volution de la RMPP au tableau 61 sont dues soit � l'effet de noria soit � l'effet p�rim�tre.    
     
[Lien vers la base de donn�es](Bases/R�mun�rations/Analyse.variations.synth�se.csv)


### 4.4 Comparaisons avec la situation nationale des r�mun�rations   
 
**�volution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    
 
&nbsp;*Tableau 64*   
   


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
  
 
&nbsp;*Tableau 65*   
   


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

Il existe  3 non titulaire s  percevant une NBI.

Primes informatiques potentielles : PRIME FONCTION INFORMATIQUE
 
&nbsp;*Tableau 66*   
   


| Nombre de lignes NBI pour non titulaires | Nombre de b�n�ficiaires de PFI |
|:----------------------------------------:|:------------------------------:|
|                    36                    |               12               |

  
[Lien vers la base de donn�es NBI aux non titulaires](Bases/R�glementation/NBI.aux.non.titulaires.csv)   
[Lien vers la base de donn�es Primes informatiques](Bases/R�glementation/personnels.prime.informatique.csv)   
  
**Nota :**   
NBI: nouvelle bonification indiciaire   
PFI: prime de fonctions informatiques   




 
&nbsp;*Tableau 67*   
   


| R�mun�rations de NBI anormales, par agent et par exercice | Montants correspondants |
|:---------------------------------------------------------:|:-----------------------:|
|                            27                             |         15 598          |

  
[Lien vers la base de donn�es NBI anormales](Bases/Fiabilit�/lignes.nbi.anormales.csv)   
  
**Nota :**   
*Est consid�r� comme manifestement anormal un total annuel de r�mun�rations NBI correspondant � un point d'indice net mensuel inf�rieur � 4 euros ou sup�rieur � 6 euros.*    
*Les rappels ne sont pas pris en compte dans les montants vers�s. Certains �carts peuvent �tre r�gularis�s en les prenant en compte*     
 
&nbsp;*Tableau 68*   
   


| Ann�e | Cumuls des NBI | Montants vers�s (a) | Point d'INM apparent | Point d'INM moyen | Contr�le |
|-------|----------------|---------------------|----------------------|-------------------|----------|
| 2010  |    7 650,0     |      34 760,4       |         4,5          |        4,6        |   Vert   |
| 2011  |    7 315,0     |      32 979,2       |         4,5          |        4,6        |   Vert   |
| 2012  |    7 230,0     |      32 487,1       |         4,5          |        4,6        |   Vert   |
| 2013  |    6 335,0     |      28 193,9       |         4,5          |        4,6        |  Orange  |


*(a) Hors rappels sur r�mun�rations*   




  
[Lien vers la base de donn�es des cumuls annuels de NBI](Bases/Fiabilit�/cumuls.nbi.csv)   
  
 
## 5.2 Contr�le des vacations pour les fonctionnaires





Il y a  0 fonctionnaire(s) effectuant des vacations pour son propre �tablissement. Les bulletins concern�s sont donn�s en lien.


[Lien vers les matricules des fonctionnaires concern�s](Bases/R�glementation/matricules.fonctionnaires.et.vacations.csv)
[Lien vers les bulletins de paie correspondants](Bases/R�glementation/lignes.fonctionnaires.et.vacations.csv)

## 5.3 Contr�les sur les cumuls traitement indiciaire, indemnit�s et vacations des contractuels    




**Contractuels effectuant des vacations (CEV)**

 
&nbsp;*Tableau 69*   
   


| Nombre de CEV | Nombre de lignes | Nombre de lignes indemnitaires | Nombre de lignes de traitement |
|:-------------:|:----------------:|:------------------------------:|:------------------------------:|
|      101      |      17 182      |              220               |              690               |

 
[Lien vers le bulletins des CEV](Bases/R�glementation/lignes.contractuels.et.vacations.csv)   
[Lien vers la base de donn�es Matricules des CEV](Bases/R�glementation/matricules.contractuels.et.vacations.csv)  
[Lien vers la base de donn�es Cumul r�gime indemnitaire et vacations de CEV](Bases/R�glementation/RI.et.vacations.csv)  
[Lien vers la base de donn�es Lignes de traitement indiciaire pour CEV](Bases/R�glementation/traitement.et.vacations.csv)  
 




## 5.4 Contr�le sur les indemnit�s IAT et IFTS      




 
&nbsp;*Tableau 70*   
     


|                                                                    Codes IFTS                                                                    |   |
|:------------------------------------------------------------------------------------------------------------------------------------------------:|:-:|
| 160001550 160001425 160001500 160008200 160008150 160001400 160001375 3046 160001250 160001600 160001775 160001300 160001200 160001700 160001525 |   |



| Nombre de personnels percevant IAT et IFTS |   |
|:------------------------------------------:|:-:|
|                     1                      |   |

  
[Codes IFTS retenus](Bases/R�glementation/codes.ifts.csv)   
[Lien vers la base de donn�es cumuls iat/ifts](Bases/R�glementation/personnels.iat.ifts.csv)    

### Contr�le sur les IFTS pour cat�gories B et contractuels



 



 




 
&nbsp;*Tableau 71*   
   


| Nombre de lignes de paie de contractuels percevant des IFTS | Nombre de lignes IFTS pour IB < 380 |
|:-----------------------------------------------------------:|:-----------------------------------:|
|                             178                             |                  0                  |


[Lien vers la base de donn�es Lignes IFTS pour contractuels](Bases/R�glementation/ifts.et.contractuel.csv)    
[Lien vers la base de donn�es Lignes IFTS pour IB < 380](Bases/R�glementation/lignes.ifts.anormales.csv)     

**Nota :**
IB < 380 : fonctionnaire percevant un indice brut inf�rieur � 380





## 5.5 Contr�le de la prime de fonctions et de r�sultats (PFR)   
  

Tous les attributaires de la PFR sont identifi�s en cat�gorie A.

  
  




 
&nbsp;*Tableau 72*   
     


| Codes PFR | Agents cumulant PFR et IFTS |
|:---------:|:---------------------------:|
|           |              0              |

  
[Lien vers la base de donn�es cumuls pfr/ifts](Bases/R�glementation/personnels.pfr.ifts.csv)    




 
&nbsp;*Tableau 73 : rappel des plafonds annuels de la PFR*   
     


| Adm. g�n�ral | Adm. HC |  Adm.  | Direct./Attach� princ. | Secr. mairie/Attach� |
|:------------:|:-------:|:------:|:----------------------:|:--------------------:|
|    58 800    | 55 200  | 49 800 |         25 800         |        20 100        |


Les plafonds annuels de la PFR de sont pas d�pass�s.

 
&nbsp;*Tableau 74* : Valeurs de l'agr�gat (PFR ou IFTS) pour les b�n�ficiaires de la PFR   
         


Aucun b�n�ficiaire de la PFR d�tect�.

 
&nbsp;*Tableau 75* : Variations de l'agr�gat (PFR ou IFTS) pour les b�n�ficiaires de la PFR
         


Aucun tableau de variation.

        
  
[Lien vers la base de donn�es agr�gat PFR-IFTS](Bases/R�mun�rations/b�n�ficiaires.PFR.csv)    

  
[Lien vers la base de donn�es variations agr�gat PFR-IFTS](Bases/R�mun�rations/b�n�ficiaires.PFR.Variation.csv)    





## 5.6 Contr�le sur les heures suppl�mentaires

Le seuil de 180 heures suppl�mentaires maximum est d�pass� par  17  agents.
 Le seuil de 220 heures suppl�mentaires maximum est d�pass� par  17  agents.


 
&nbsp;*Tableau 76*   
   


| Nombre de lignes HS en exc�s | Nombre de lignes IHTS anormales |
|:----------------------------:|:-------------------------------:|
|             271              |               230               |


[Lien vers la base de donn�es Heures suppl�mentaires en exc�s du seuil de 25h/mois](Bases/R�glementation/HS.sup.25.csv)     
[Lien vers la base de donn�es cumuls en exc�s des seuils annuels](Bases/R�glementation/D�passement.seuil.180h.csv)    
[Lien vers la base de donn�es IHTS anormales](Bases/R�glementation/ihts.anormales.csv)      

**Nota :**   
HS en exc�s : au-del� de 25 heures par mois     
IHTS anormales : attribu�es � des fonctionnaires ou non-titulaires de cat�gorie A ou assimil�s.     




## 5.7 Contr�le sur les indemnit�s des �lus
  



  


|Matricule |Nom       | Ann�e|Emploi | Indemnit�s | Autres |   Total |
|:---------|:---------|-----:|:------|-----------:|-------:|--------:|
|173001    |DEFIX     |  2010|Elus   |    36721,80|       0| 36721,80|
|173101    |WAUQUIEZ  |  2010|Elus   |    36721,80|       0| 36721,80|
|173201    |DECOLIN   |  2010|Elus   |    36721,80|       0| 36721,80|
|173301    |CHASSAING |  2010|Elus   |    36721,80|       0| 36721,80|
|173401    |GONCALVES |  2010|Elus   |    36721,80|       0| 36721,80|
|173501    |MAURIN    |  2010|Elus   |    36721,80|       0| 36721,80|
|173601    |BAY       |  2010|Elus   |    36721,80|       0| 36721,80|
|173701    |COURIOL   |  2010|Elus   |    36721,80|       0| 36721,80|
|173801    |GUIEAU    |  2010|Elus   |    36721,80|       0| 36721,80|
|173901    |CONVERT   |  2010|Elus   |    36721,80|       0| 36721,80|
|174001    |PEYRET    |  2010|Elus   |    36721,80|       0| 36721,80|
|174201    |GIRBON    |  2010|Elus   |    36721,80|       0| 36721,80|
|174301    |GROS      |  2010|Elus   |    36721,80|       0| 36721,80|
|174401    |RIGAUD    |  2010|Elus   |    36721,80|       0| 36721,80|
|182101    |REYNAUD   |  2010|Elus   |    36721,80|       0| 36721,80|
|301754    |JOUBERT   |  2011|ELU    |    36580,80|       0| 36580,80|
|301754    |JOUBERT   |  2012|ELU    |    36580,80|       0| 36580,80|
|301754    |JOUBERT   |  2013|ELU    |    36580,80|       0| 36580,80|
|301786    |PIERRE    |  2011|ELU    |    18406,68|       0| 18406,68|
|301786    |PIERRE    |  2012|ELU    |    18406,68|       0| 18406,68|
|301786    |PIERRE    |  2013|ELU    |    18406,68|       0| 18406,68|
|301943    |DEFIX     |  2011|ELU    |    18406,68|       0| 18406,68|
|301943    |DEFIX     |  2012|ELU    |    18406,68|       0| 18406,68|
|301943    |DEFIX     |  2013|ELU    |    18406,68|       0| 18406,68|
|301944    |WAUQUIEZ  |  2011|ELU    |    18406,68|       0| 18406,68|
|301944    |WAUQUIEZ  |  2012|ELU    |    18406,69|       0| 18406,69|
|301944    |WAUQUIEZ  |  2013|ELU    |    18406,68|       0| 18406,68|
|301945    |DECOLIN   |  2011|ELU    |    18406,68|       0| 18406,68|
|301945    |DECOLIN   |  2012|ELU    |    18406,68|       0| 18406,68|
|301945    |DECOLIN   |  2013|ELU    |    18406,68|       0| 18406,68|
|301946    |CHASSAING |  2011|ELU    |    18406,68|       0| 18406,68|
|301946    |CHASSAING |  2012|ELU    |    18406,68|       0| 18406,68|
|301946    |CHASSAING |  2013|ELU    |    18406,68|       0| 18406,68|
|301947    |GONCALVES |  2011|ELU    |    18406,68|       0| 18406,68|
|301947    |GONCALVES |  2012|ELU    |    18406,68|       0| 18406,68|
|301947    |GONCALVES |  2013|ELU    |    18406,68|       0| 18406,68|
|301948    |MAURIN    |  2011|ELU    |    18406,68|       0| 18406,68|
|301948    |MAURIN    |  2012|ELU    |     7669,45|       0|  7669,45|
|301949    |BAY       |  2011|ELU    |    18406,68|       0| 18406,68|
|301949    |BAY       |  2012|ELU    |    18406,68|       0| 18406,68|
|301949    |BAY       |  2013|ELU    |    18406,68|       0| 18406,68|
|301950    |COURIOL   |  2011|ELU    |    18406,68|       0| 18406,68|
|301950    |COURIOL   |  2012|ELU    |    18406,68|       0| 18406,68|
|301950    |COURIOL   |  2013|ELU    |    18406,68|       0| 18406,68|
|301951    |GUIEAU    |  2011|ELU    |    18406,68|       0| 18406,68|
|301951    |GUIEAU    |  2012|ELU    |    18406,68|       0| 18406,68|
|301951    |GUIEAU    |  2013|ELU    |    18406,68|       0| 18406,68|
|301952    |CONVERT   |  2011|ELU    |    18406,68|       0| 18406,68|
|301952    |CONVERT   |  2012|ELU    |     9203,34|       0|  9203,34|
|301953    |PEYRET    |  2011|ELU    |    18406,68|       0| 18406,68|
|301953    |PEYRET    |  2012|ELU    |    18406,68|       0| 18406,68|
|301953    |PEYRET    |  2013|ELU    |    18406,68|       0| 18406,68|
|301954    |GIRBON    |  2011|ELU    |    18406,68|       0| 18406,68|
|301954    |GIRBON    |  2012|ELU    |     9203,34|       0|  9203,34|
|301955    |GROS      |  2011|ELU    |    18406,68|       0| 18406,68|
|301955    |GROS      |  2012|ELU    |    18406,68|       0| 18406,68|
|301955    |GROS      |  2013|ELU    |    18406,68|       0| 18406,68|
|301956    |RIGAUD    |  2011|ELU    |    18406,68|       0| 18406,68|
|301956    |RIGAUD    |  2012|ELU    |    18406,68|       0| 18406,68|
|301956    |RIGAUD    |  2013|ELU    |    18406,68|       0| 18406,68|
|301991    |REYNAUD   |  2011|ELU    |    18406,68|       0| 18406,68|
|301991    |REYNAUD   |  2012|ELU    |    18406,68|       0| 18406,68|
|301991    |REYNAUD   |  2013|ELU    |    18406,68|       0| 18406,68|
|60302     |JOUBERT   |  2010|Elus   |    72979,44|       0| 72979,44|
|64502     |PIERRE    |  2010|Elus   |    36721,80|       0| 36721,80|

[Lien vers la base de donn�es R�mun�rations des �lus](Bases/R�glementation/r�mun�rations.�lu.csv)




## 5.8 Lien avec le compte de gestion



 
Cumul des lignes de paie par exercice et cat�gorie de ligne de paie   
 


Tableau  77  Ann�e  2010

|           Cat�gorie|  Cumul annuel|
|-------------------:|-------------:|
|          Indemnit�s|    901 790,04|
|             Rappels|     94 463,01|
|       R�m. diverses|     50 427,17|
| Suppl�ment familial|     76 043,69|
|         Traitements| 10 117 498,07|

Tableau  78  Ann�e  2011

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s|   859 785,12|
|             Rappels|   312 030,48|
|       R�m. diverses|    11 119,69|
| Suppl�ment familial|    78 235,58|
|         Traitements| 4 760 956,27|

Tableau  79  Ann�e  2012

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s|   916 467,75|
|             Rappels|   265 409,09|
|       R�m. diverses|    15 004,19|
| Suppl�ment familial|    82 448,19|
|         Traitements| 5 213 570,52|

Tableau  80  Ann�e  2013

|           Cat�gorie| Cumul annuel|
|-------------------:|------------:|
|          Indemnit�s| 1 038 625,40|
|             Rappels|   287 307,26|
|       R�m. diverses|    17 876,24|
| Suppl�ment familial|    88 661,84|
|         Traitements| 5 503 405,22|

 
[Lien vers la base d�taill�e des cumuls des lignes de paie](Bases/R�glementation/cumul.lignes.paie.csv)
 
[Lien vers la base agr�g�e des cumuls des lignes de paie](Bases/R�glementation/cumul.total.lignes.paie.csv)
 
 
*Avertissement : les rappels comprennent �galement les rappels de cotisations et d�ductions diverses.*    
  




## 5.9 Contr�le du suppl�ment familial de traitement   
 


Pour les agents n'ayant pas d'enfant signal� en base, il a �t� d�tect� 48 bulletins de paie pr�sentant un paiement du SFT apparemment anormal.

 
[Lien vers la base des paiements de SFT � agents sans enfant signal�](Bases/R�glementation/Paie.sans.enfant.r�duit.csv)
 


Pour les agents ayant au moins un enfant, il a �t� d�tect� 46 bulletins de paie pr�sentant un �cart de paiement du SFT sup�rieur � 1 euro.

 
[Lien vers la base des �carts de paiement sur SFT](Bases/R�glementation/controle.sft.csv)
 



# Annexe
## Liens compl�mentaires
 
## Fiabilit� du traitement statistique   
*Doublons*      

Attention : Alta�r a d�tect� des lignes dupliqu�es alors qu'aucun retraitement des lignes dupliqu�es n'est pr�vu par d�faut.

 



 
*Tests de fiabilit� sur le renseignement des heures et des quotit�s*    
  



    

Nombre de bulletins :  14 260

    

Les heures de travail ont �t� redress�es avec la m�thode  des quotit�s.

   

 Nombre de bulletins de paie redress�s : 148

   

 Pourcentage de redressements : 1,04 % des bulletins de paie.

 


Pourcentage d'heures renseign�es (apr�s redressement �ventuel): 88,1 %

  


Pourcentage de quotit�s renseign�es : 98,2 %

  


Nombre de bulletins � heures et quotit�s :  12347 [ 86,6 %]

  


Nombre de bulletins � heures sans quotit�s :  211 [ 1,5 %]

  


Nombre de bulletins � quotit�s sans heures :  1651 [ 11,6 %]

  


Nombre de bulletins apparemment inactifs :  51 [ 0,4 %]

  



 

Nombre de bulletins de paie de salaires vers�s pour un champ Heures = 0 :  1 646 [ 11,5 %]

  



  
[Lien vers la base de donn�es des salaires vers�s pour Heures=0](Bases/Fiabilit�/base.heures.nulles.salaire.nonnull.csv)   
[Lien vers la base de donn�es des salaires vers�s � quotit� ind�finie](Bases/Fiabilit�/base.quotit�.ind�finie.salaire.nonnull.csv)   

# Tableau des personnels  






[Lien vers la base des grades et cat�gories](Bases/Effectifs/grades.cat�gories.csv)        
  

[Lien vers la base des personnels](Bases/Effectifs/matricules.csv)        
  

# Divergences lignes-bulletins de paie     
  
*Pour exclure certains codes de paie de l'analyse, renseigner le fichier liste.exclusions.txt*  
  

   Codes exclus :  890A 319N 413R 409R 860A 876A 877A  

  
[Divergences lignes-bulletins de paie](Bases/Fiabilit�/Delta.csv)     
  



