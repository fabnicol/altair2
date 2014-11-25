# Méthodologie de calcul des rémunérations individuelles

# Les convergences avec les statistiques de l'INSEE
 
Les périmètres retenus sont autant que possible ajustés pour être proches de ceux du système
d'information sur les agents du service public (SIASP).

L’étude des salaires est réalisée à partir des postes, restreints aux postes actifs et non annexes.

Un certain nombre de différences méthodologiques avec les calculs de l'INSEE et de la DGAFP doivent
toutefois être relevées.

## La caractérisation des postes actifs non annexes

Dans le SIASP, les postes actifs et non annexes se définissent par trois variables :
rémunération, nombre d'heures, durée d'emploi.  

On définit tout d'abord une période non annexe.  

Une période est dite non annexe sous les conditions (C) suivantes :   

1. si le salaire net du poste, indépendamment de sa durée, est supérieur à 3 fois le Smic
    mensuel net; ou :  
2. si les trois conditions suivantes sont simultanément remplies :   
    a) la durée du poste est supérieure à 30 jours ;   
    b) le nombre d’heures du poste est supérieur à 120 heures ;  
    c) le nombre d’heures travaillées par jour au cours du poste est supérieur à 1,5 heure.   

Un poste est constitué d’une ou plusieurs périodes de travail d’un salarié dans un même 
établissement.   

On qualifie une période d'active si elle correspond à un temps complet, à un temps partiel ou à un forfait, à une 
cessation progressive d’activité, à un congé de maladie ordinaire (CMO) ou à un congé formation (CF).   

Sont considérées comme périodes inactives, les rappels, les périodes non rémunérées, les périodes de chômage,
les congés de fin d’activité (CFA) et les congés longue maladie (CLM).  

Un poste est actif s'il est constitué d'au moins une période active.  

On qualifie un poste de non annexe sous les conditions suivantes :  

* si le salarié a été rémunéré pour au moins une période non annexe ;   
* sinon, si les conditions (C) sont vérifiées pour l'ensemble du poste, à défaut de l'être pour 
  une période donnée.   
        
Dans le cas contraire, le poste est dit annexe.   

Pour les calculs réalisés par Altair, les périodes sont systématiquement annuelles ou 
infra-annuelles et les filtres (C) sont appréciés sur une seule période globalisée.
Le filtrage par la condition d'activité et les conditions (C) est réalisé, 
moyennant quelques approximations qui sont précisées *infra*.   

Comme dans le SIASP, les élus locaux sont exclus du calcul des rémunérations moyennes et taux de primes.  
Ils ne sont inclus que pour les calculs de masses de rémunérations globales et dans ce cas leur 
inclusion est signalée.  

## Le calcul des quotités   

Les quotités sont calculées à partir des postes actifs et non annexes et de la
variable Heures de travail. 
 
Sauf indication contraire, les données de salaire sont exprimées en « équivalent temps plein annualisé ».
En matière d’effectifs, cette notion est équivalente à la notion budgétaire d’ETPT 
(équivalents temps plein travaillés) ou à celle d’EQTP utilisée dans le secteur privé.  
 
Pour chaque poste, le salaire est transformé en un salaire en équivalent temps plein annualisé (EQTP 
annualisé), correspondant au salaire qui aurait été perçu par un poste à temps plein (notion de 
quotité) toute l’année (notion de durée).     

Pour chaque poste, ce salaire est pondéré par son poids en « équivalent temps plein annualisé », 
autrement dit au prorata de la durée rémunérée et de la quotité travaillée. Par exemple, un poste
occupé durant 6 mois à temps plein et rémunéré 10 000 euros compte pour 0,5 « équivalent temps
plein annualisé », rémunéré par an 20 000 euros. Un poste occupé toute l’année avec une quotité
travaillée de 60 % et rémunéré 12 000 euros compte pour 0,6 « équivalent temps plein annualisé » 
rémunéré 20 000 euros par an.  

Le salaire net annuel moyen en « équivalent temps plein annualisé » est obtenu en pondérant 
les salaires annualisés des postes par le nombre d’« équivalents temps plein annualisés ».  

Le calcul de la quotité consiste à déterminer une durée médiane annuelle de travail pour les postes actifs
à temps complet, par strate. La durée estimée au sein d’une strate sert ensuite de pondération pour ramener, au 
sein de ces strates, les postes à temps partiel ou à temps non complet à des « équivalent temps plein ».   

## L'unicité du SIREN   

Dans les fichiers SIASP, l’unité d’intérêt est le poste, ce qui correspond à une ou plusieurs 
périodes de travail d’un salarié dans un même établissement, défini dans la FPT par son SIREN.   

Pour les traitements réalisés pour une seule collectivité simultanément, il convient donc de n'utiliser, en entrée
du logiciel, que des fichiers de paye comportant des codes SIRET correspondant au même SIREN 
(les 9 premiers chiffres du SIRET). Cette condition n'est pas vérifiée par les filtres d'importation des fichiers .xhl
du logiciel à ce jour ( version 14.10) mais l'est en pratique. En effet, l'ouverture d'un contrôle étant conditionnée à
l'unicité de la personne morale vérifiée, les structures rattachées à une même collectivité 
(régies non personnalisées, services diversement autonomes,...) ont des numéros SIRET correspondant au même SIREN.

# Les approximations retenues par le logiciel Altair     

## La caractérisation des postes actifs  

Les postes actifs sont caractéristés de manière plus rudimentaire : les agents actifs sont ceux qui
ont simultanément un nombre d'heures travaillées non nul et un traitement strictement positif.   

En principe ce double critère permet d'exclure les CFA et CLM. Il peut aussi conduire à exclure, à 
tort, des CMO ou des CF de plus d'un mois. Il est estimé que l'impact de cette aproximation reste 
raisonnable à l'échelle de la précision visée. Cette approximation devrait faire l'objet d'un examen 
spécifique.  

## L'utilisation de données brutes  

La principale différence avec les traitements réalisés par les services statistiques ministériels
(SSM) a trait a l'absence de redressements pour un certain nombre de variables connues pour leur 
qualité insatisfaisante (grade et statut, nombre d'heures, entre autres). L'impact de l'utilisation
de données brutes, par rapport aux données redressées des SSM, n'est pas évaluable à ce stade.  

## Autres différences et approximations  

Pour la version 14.10 (novembre 2014), tout mois rémunéré est réputé correspondre à 30 jours de travail, pour 
les critères de caractérisation des périodes et postes annexes du 1.1, plus précisément pour le calcul 
du nombre de jours travaillés et du nombre d’heures travaillées par jour au cours de la période.

Cette approximation est provisoire et sera ultérieurement levée.  

Elle a pour conséquence deux effets contraires : un nombre plus élevé de postes et périodes annexes 
est pris en compte au titre du critère a). Inversement un nombre plus faible de ces postes et périodes
est pris en compte au titre du critère c).

Par ailleurs, la définition des strates de calcul des quotités a dû être revue pour éviter que les effectifs des
strates ne soient trop faibles, notamment pour les collectivités à effectifs limités. Là où l'INSEE retient comme
critères définitoires d'une strate "la catégorie hiérarchique, le statut, le sexe et la tranche d’âge", 
seuls sont retenus par Altair le statut modifié, l'emploi modifié et le sexe.

Par statut modifié, on entend une variable comprenant la variable statut incluse dans le SIASP (avec les mêmes
modalités) augmentée des modalités suivantes : statut de vacataire (lorsque détecté), d'élu ou d'assistante
maternelle. Ces trois catégories statutaires sont exclues de l'analyse d'office. En effet, pour les vacataires, 
les faibles effectifs et les problèmes de qualité sur les variables heures et temps de travail conduisent à des
valeurs de rémunérations EQTP aberrantes dans un nombre excessif de cas, compte tenu de l'absence de redressement.
Les assistantes maternelles, par ailleurs, ne sont pas comprises dans les périmètres des statistiques publiées 
dans le cadre du Rapport annuel sur l'état de la fonction publique. Enfin les élus locaux sont exclus
des calculs par l'INSEE.

Par emploi modifié on entend soit l'emploi, s'il y a au moins 5 emplois à temps complet 
dans la strate, soit, à défaut, la médiane du même statut.

Il est conjecturé qu'en l'absence de catégorie hiérarchique, l'emploi modifié peut représenter une variable 
suffisamment bien liée à la catégorie hiérarchique d'une part, à la tranche d'âge d'autre part, pour se substituer à ces
deux variables dans la détermination des strates.

# Algorithmes

Ces considérations méthodologiques permettent de dégager les algorithmes dont l'exposé suit en pseudo-code.  

## Filtrage des périodes 

Pour chaque Matricule et chaque année :  

`````
   Si il existe un bulletin mensuel dans l'année tel que pour la première ligne de paie vérifiant :
        Type == "TRAITEMENT" [le traitement indiciaire]  
      on a  : Montant > minimum.positif  
   et si Heures > minimum.positif  pour ce mois
   alors   
      Filtre_actif = vrai.  
   sinon  
      Filtre_actif = faux.  
      
   Pour année:
    2013 :
      si Rémunération.annuelle.nette  > 3361
    2012 :
      si Rémunération.annuelle.nette  > 3322
    2011 :
      si Rémunération.annuelle.nette  > 3222
    2010 :
      si Rémunération.annuelle.nette  > 3169
    2009 :
      si Rémunération.annuelle.nette  > 3132
    2008 :
      si Rémunération.annuelle.nette  > 3076  
      
   ou si :
    nombre.mois > 1 et   
    somme(Heures) > 120 heures et  
    somme(Heures)/nombre(jours) > 1,5.   
   
   alors :
    Filtre_non_annexe = vrai
    
   Pour les distributions, moyennes et médianes de rémunérations :   
   Retenir le périmètre combiné : 
      Filtre_non_annexe == vrai et Filtre_actif == vrai
    
`````   
## Calcul des quotités     

`````````````      
Pour chacun des statuts S dans {TITULAIRE, STAGIAIRE, AUTRE_STATUT},    
     et chacun des sexes G dans {HOMME, FEMME} :  

  calculer la médiane M0[S, G] des Heures des agents tels que :  
  
     Temps.de.travail == 100   
     et Heures est renseigné   
     et Heures > minimum.positif.  
`````````````      
````````````` 
Pour chaque Emploi et Sexe dans la période calculer l'effectif E des agents tels que :
   
     Temps.de.travail == 100   
     et Filtre_actif == vrai
     et Heures est renseigné   
     et Heures > minimum.positif.  
`````````````    
`````````````    
  Si E > 4  
  alors 
     calculer M = médiane(Heures) sur cet effectif
  sinon
     M = M0[S, G], pour S le statut de l'agent et G son sexe.   

`````````````    
```````
Alors si M est non-manquante et M > minimum.positif :  

         Quotité = Heures / M

      sinon M = M0[S, G]   
```````          

# Références   

Nouveaux compléments méthodologiques suite à l’introduction du système d’information SIASP, INSEE-
DGAFP mars 2013.  

Décompte des emplois et mesure des évolutions de salaires dans les trois versants de la fonction
publique, INSEE-DGAFP novembre 2011.  
 
 
