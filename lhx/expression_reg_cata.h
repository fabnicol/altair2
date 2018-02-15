#ifndef EXPRESSION_REG_CATA_HPP
#define EXPRESSION_REG_CATA_HPP

/// \file expression_reg_cata.h
/// \author Fabrice Nicol
/// \brief Ce fichier contient les expressions rationnelles permettant d'identifier les catégories statutaires A et B
/// à partir des grades des agents

/*
Décret n° 2011-746 du 27 juin 2011 portant statuts particuliers des corps des personnels de rééducation de la catégorie B de la fonction publique hospitalière
Sont classés dans la catégorie B les corps des personnels de rééducation des établissements mentionnés à l'article 2 de la loi du 9 janvier 1986 susvisée, ci-dessous énumérés :
1° Le corps des pédicures-podologues ;
2° Le corps des masseurs-kinésithérapeutes ;
3° Le corps des ergothérapeutes ;
4° Le corps des psychomotriciens ;
5° Le corps des orthophonistes ;
6° Le corps des orthoptistes ;
7° Le corps des diététiciens.  */

/* Passage en A des ergothérapeutes
Décret n° 2015-1048 du 21 août 2015 portant dispositions statutaires relatives aux ergothérapeutes de la fonction publique hospitalière  */

/* Nouveau corps, classe sup
 *       IB  IN
 * 1	449	394	1 an	1 824,34 €
   2	465	407	2 ans	1 884,53 €
   3	491	424	2 ans	1 963,25 €
   4	517	444	2 ans	2 055,85 €
   5	546	464	2 ans	2 148,46 €
   6	577	487	3 ans	2 254,96 €
   7	607	510	3 ans	2 361,45 €
   8	637	533	4 ans	2 467,95 €
   9	667	556	4 ans	2 574,45 €
   10	701	582	4 ans	2 694,83 €
   11	736	608	-	2 815,22 €

Corps : Ergothérapeute (nouveau corps au 01.09.2015), classe N
Categorie : A


1	385	353	1 an	1 634,50 €
2	408	367	2 ans	1 699,32 €
3	438	386	3 ans	1 787,30 €
4	464	406	3 ans	1 879,90 €
5	497	428	3 ans	1 981,77 €
6	542	461	3 ans	2 134,57 €
7	582	492	3 ans	2 278,11 €
8	611	513	4 ans	2 375,34 €
9	637	533	4 ans	2 467,95 €
10	663	553	4 ans	2 560,56 €
11	685	570	-	2 639 */

// Les infirmières FPH sont passées en A en 2012 sauf un corps en extinction IDE; situation à évaluer pour l'éducation nationale sur les situations d'extinction

#include "expression_reg_adjoints.h"

/// Expression régulière tendant à capturer les vacataires
static constexpr auto EXPRESSION_REG_VACATIONS = ".*\\bvacat.*|.*\\bvac\\.?\\b.*";                 // vac.* peut être vérifié par 'vacances'

/// Expression régulière tendant à capturer les officiers subalternes de catégorie B
/// Sous-lieutenant, lieutenant, major
#define OFFICIER_SUB     "(?:sous-?|\\b)lieut[^c]*\\b|major"

/// Expression régulière tendant à capturer les agents officiers (subalternes et supérieurs) de catégorie A
/// Capitaine, commandant, lieutenant-colonel, colonel, général
#define OFFICIER         "lieut.*col|capit|com.*d.*t|colon|g.?.?n.?.?ral"

/// Administrateur
#define ADMINISTRATEUR "adminis|a.*\\bh.*\\bu"

/// Attaché
#define ATTACHE "attach"

/// Biologiste
#define BIOLOGISTE "biol"

/// Bibliothécaire
#define BIBLIOTHECAIRE "biblio"

/// Cadre socio-éducatif ou de santé
#define CADRE_SOCIO_ED_SANTE "cad.*(?:\\bsoc.*ed|\\bsan)"

/// Conseiller
#define CONSEILLER "cons\\.?"

/// Directeur
#define DIRECTEUR "dir(?:ect|.*\\bet.*b|.*\\bsoi)"

/// Ingénieur
#define INGENIEUR "ingen"

/// Médecin
#define MEDECIN "mede"

/// Maître de conférences
#define MAITRE_DE_CONF "ma.t.*conf"

/// Praticien hospitalier
#define PRATICIEN_HOSP "prat.*hos"

/// Pharmacien
#define PHARMACIEN "pharm"

/// Professeur
#define PROFESSEUR "prof"

/// Psychologue
#define PSYCHOLOGUE "psy.*(?:l|m.*c.*\\bs)"

/// Sage-femme
#define SAGE_FEMME "sage.*f"

/// secrétaire de mairie
#define SECRETAIRE_MAIRIE "secr.*mai"

/// Vétérinaire
#define VETERINAIRE "v[eé]t[eé]r"

/// Interne
#define INTERNE "int.*(?:med|phar|od)"

/// Ergothérapeute
#define ERGOTHERAPEUTE "ergo.*sant"

/// Infirmière de catégorie A
#define INFIRMIERE_A  "infi?r?m?i?.*(?:\\b(?!i)|anes|bloc|i\\.?a\\.?d\\.?|i\\.?b\\.?o\\.?d\\.?|s\\.?\\s*\\bg\\.?|soi|enc.*s)"

/// Radiophysicien
#define RADIOPHYSICIEN "radiophys"

/// Technicien, masseur kinésithérapeute, diététicien, informaticien, manipulateur cadre de santé (technicien supérieur cat. A)
#define TECHNICIEN_CADRE_SANTE "(?:tec.*l|mass.*kin|diet|inf|manip).*\\bc(?:\\.|a).*\\bs"

/// Expression régulière tendant à capturer les agents de catégorie A
static constexpr auto   EXPRESSION_REG_CAT_A = AUCUN_MOT
        SOIT
        ADMINISTRATEUR
        OU
        "c.*\\b(?:cl|tr).*\\bu"
        OU
        ATTACHE
        OU
        BIOLOGISTE
        OU
        BIBLIOTHECAIRE
        OU
        CADRE_SOCIO_ED_SANTE
        OU
        OFFICIER
        OU
        CONSEILLER
        OU
        "d\\S*\\.?\\s*g\\S*\\.?"
        OU
        DIRECTEUR
        OU
        INGENIEUR
        OU
        MEDECIN
        OU
        MAITRE_DE_CONF
        OU
        PRATICIEN_HOSP
        OU
        PHARMACIEN
        OU
        "ped.*p.*c.*\\bs"
        OU
        "prep.*c.*\\bs"
        OU
        PROFESSEUR
        OU
        PSYCHOLOGUE
        OU
        PUERICULTRICE
        OU
        SAGE_FEMME
        OU
        SECRETAIRE_MAIRIE
        OU
        VETERINAIRE
        OU
        "i\\.?a\\.?d\\.?e\\.?|i\\.?b\\.?o\\.?d\\.?e\\.?|I\\.?S\\.?G\\.?(?:\\b|S)"
        OU
        INTERNE
        OU
        ERGOTHERAPEUTE
        OU
        INFIRMIERE_A
        OU
        RADIOPHYSICIEN
        OU
        TECHNICIEN_CADRE_SANTE
        FIN_SOIT
        ETC;

// A cause du cas problématique des infirmiers et diététiciens, ex B recatégorisés en A, il faut d'abord tester A puis si échec B

#define REDACTEUR "redac"
#define TECHNICIEN "tech"
#define TECHNICIEN_SUP_HOSP "T\\.?S\\.?H\\.?"
#define ANIMATEUR "anim"
#define EDUCATEUR "educ"
#define AS  "a\\.?\\s?s\\.?\\s?(?:e|\\s)"
#define ASSISTANT "a\\.?\\s?s\\.?\\s?(?:e|\\s)|ast\\.?|assi?s?t?\\.?.*"
#define SPECIALISE "spec"
#define ENSEIGNEMENT "ens"
#define CONSERVATION "cons"
#define BIBLIOTHEQUES "bib"
#define SOCIO_EDUC "soc.*ed"
#define MEDICO_TECH "med.*t"
#define MEDICO_ADM "med.*adm"
#define MONITEUR "monit"
#define CONTROLEUR "contro.*t(?:er|ra)"
#define CHEF_SERVICE_PM "ch.*(?:s.*po|S.*P.*M).*"
#define IDE  "I\\.?D\\.?E\\.?|inf.*\\bi\\.?d\\.?e"
#define REEDUCATEUR  "r[eé][eé]duc"
#define ADJOINT_CADRE "adj.*cadr"
#define  ANALYSTE "analyst"
#define  DIETETICIEN "diet"
#define  PREPARATEUR "prep.*ph"
#define  PED "ped.po"
#define  PROGRAMMEUR "programm"
#define  ORTHOPEDISTE "orthop"
#define  KINE "mass.*kin"
#define  MANIPULATEUR "manip"
#define  SECRETAIRE_MED "secr.*med"
#define  AMA  "\\ba\\.?m\\.?a\\.?\\b"

/// Expression régulière tendant à capturer les agents de catégorie B
/// \warning Ne tester cette expression qu'après échec de l'expression régulière de catégorie A  EXPRESSION_REG_CAT_A
static constexpr auto   EXPRESSION_REG_CAT_B = AUCUN_MOT
        SOIT
        REDACTEUR
        OU
        TECHNICIEN
        OU
        TECHNICIEN_SUP_HOSP
        OU
        ANIMATEUR
        OU
        EDUCATEUR
        OU
        AS
        OU
        ASSISTANT
        SOIT
        SPECIALISE
        OU
        ENSEIGNEMENT
        OU
        CONSERVATION
        OU
        PATRIMOINE
        OU
        BIBLIOTHEQUES
        OU
        SOCIAL
        OU
        SOCIO_EDUC
        OU
        MEDICO_TECH
        OU
        MEDICO_ADM
        FIN_SOIT
        OU
        MONITEUR
        OU
        CONTROLEUR
        OU
        CHEF_SERVICE_PM
        OU
        IDE
        OU
        REEDUCATEUR
        OU
        ADJOINT_CADRE
        OU
        ANALYSTE
        OU
        DIETETICIEN
        OU
        PREPARATEUR
        OU
        PED
        OU
        PROGRAMMEUR
        OU
        ORTHOPEDISTE
        OU
        KINE
        OU
        MANIPULATEUR
        OU
        SECRETAIRE_MAIRIE
        OU
        AMA
        OU
        OFFICIER_SUB
        FIN_SOIT
        ETC;


#endif // EXPRESSION_REG_CATA_HPP
