#ifndef EXPRESSION_REG_CATA_HPP
#define EXPRESSION_REG_CATA_HPP



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

#include "expression_reg_adjoints.hpp"

/// Expression régulière tendant à capturer les vacataires
static constexpr auto EXPRESSION_REG_VACATIONS = ".*\\bvacat.*|.*\\bvac\\.?\\b.*";                 // vac.* peut être vérifié par 'vacances'

/// Expression régulière tendant à capturer les officiers subalternes de catégorie B
#define OFFICIER_SUB     "(?:sous-?|\\b)lieut[^c]*\\b|major"

/// Expression régulière tendant à capturer les agents officiers (subalternes et supérieurs) de catégorie A
#define OFFICIER         "lieut.*col|capit|com.*d.*t|colon|g.?.?n.?.?ral"

/// Expression régulière tendant à capturer les agents de catégorie A
static constexpr auto   EXPRESSION_REG_CAT_A = "\\W*\
(?:adminis|a.*\\bh.*\\bu|c.*\\b(?:cl|tr).*\\bu|attach|biol|biblio|cad.*(?:\\bsoc.*ed|\\bsan)|" OFFICIER "|cons\\.?|d\\S*\\.?\\s*g\\S*\\.?|\
dir(?:ect|.*\\bet.*b|.*\\bsoi)|ingen|mede|ma.t.*conf|prat.*hos|pharm|ped.*p.*c.*\\bs|prep.*c.*\\bs|prof|psy.*(?:l|m.*c.*\\bs)|puer.*cad.*sa|puericultr|sage.*f|secr.*mai|v[eé]t[eé]r|\
i\\.?a\\.?d\\.?e\\.?|i\\.?b\\.?o\\.?d\\.?e\\.?|I\\.?S\\.?G\\.?(?:\\b|S)|int.*(?:med|phar|od)|ergo.*sant|infi?r?m?i?.*(?:\\b(?!i)|anes|bloc|i\\.?a\\.?d\\.?|i\\.?b\\.?o\\.?d\\.?|s\\.?\\s*\\bg\\.?|soi|enc.*s)|\
radiophys|(?:tec.*l|mass.*kin|diet|inf|manip).*\\bc(?:\\.|a).*\\bs).*";

 // A cause du cas problématique des infirmiers et diététiciens, ex B recatégorisés en A, il faut d'abord tester A puis si échec B

/// Expression régulière tendant à capturer les agents de catégorie C
static constexpr auto   EXPRESSION_REG_CAT_B = "\\W*\
(?:redac|tech|T\\.?S\\.?H\\.?|anim|educ|a\\.?\\s?s\\.?\\s?(?:e|\\s)|ast\\.?|assi?s?t?\\.?.*(?:spec|ens|cons|pat|bib|social|soc.*ed|med.*t|med.*adm)|monit|contro.*t(?:er|ra)|\
ch.*(?:s.*po|S.*P.*M).*|I\\.?D\\.?E\\.?|ergo|inf.*\\bi\\.?d\\.?e|reeduc|adj.*cadr|analyst|diet|prep.*ph|ped.*po|programmeu|orthop|mass.*kin|manip|secr.*med|\\ba\\.?m\\.?a\\.?\\b|" OFFICIER_SUB ").*";

/*
 * REDACTEUR
 * TECHNICIEN
 * TECHNICIEN_SUP_HOSP
 * ANIMATEUR
 * AS
 * EDUCATEUR
 * ASSISTANT
 * SPECIALISE
 * ENSEIGNEMENT
 * CONSERVATION
 * PATRIMOINE
 * BIBLIOTHEQUES
 * SOCIO_EDUC
 * MEDICO_TECH
 * MEDICO_ADM
 * MONITEUR
 * CONTROLEUR
 * LIEUTENANT
 */

#endif // EXPRESSION_REG_CATA_HPP
