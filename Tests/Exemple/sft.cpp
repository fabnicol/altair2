
// [[Rcpp::plugins(cpp11)]]

#include <Rcpp.h>

/*  
 *  Utiliser boost/regex.hpp, inclus dans le paquet BH
 *  Les résultats sont 50 fois plus rapides qu'avec la bibliothèque standard de C++14 (G++)...sous linux.
 *  Sous windows, non, utilisation de la bibliothèque standard regex C++1 = même performances que sous linux.
 *  Ecart de performances beaucoup plus grand sous Windows entre R et Rcpp (x15) que sous linux (x3). 
 *  Gain de 1 s par million de lignes sous Windows, de 150 ms seulement sous linux.
 *  
 */


#include <regex>
#include <string>

using namespace std;
using namespace Rcpp;


/*
*  on prévoit 15 enfants...
*
*  limitations : pas de vérification des cas de divorce etc., ni des cas de cumuls
*                pas de vérification non plus de la licéité des versements à des contractuels exclus par l'article 1er 
*                du décret n°85-1148 du 24 octobre 1985 modifié relatif à la rémunération des personnels civils et militaires
*                de l'Etat, des personnels des collectivités territoriales et des personnels des établissements publics d'hospitalisation. 
*/


//  part fixe mensuelle

constexpr double  sft_fixe[15]= {2.29, 10.67, 15.24,
                                         15.24 + 4.57,
                                         15.24 + 4.57 * 2, 
                                         15.24 + 4.57 * 3,
                                         15.24 + 4.57 * 4,
                                         15.24 + 4.57 * 5,
                                         15.24 + 4.57 * 6,
                                         15.24 + 4.57 * 7,
                                         15.24 + 4.57 * 8,
                                         15.24 + 4.57 * 9,
                                         15.24 + 4.57 * 10,
                                         15.24 + 4.57 * 11,
                                         15.24 + 4.57 * 12};
  
// part variable en proportion du traitement indiciaire
  
constexpr double  sft_prop[15] =  {0, 3, 8, 8 + 6,
                                                    8 + 6 * 2,
                                                    8 + 6 * 3,
                                                    8 + 6 * 4,
                                                    8 + 6 * 5,
                                                    8 + 6 * 6,
                                                    8 + 6 * 7,
                                                    8 + 6 * 8,
                                                    8 + 6 * 9,
                                                    8 + 6 * 10,
                                                    8 + 6 * 11,
                                                    8 + 6 * 12};
  
constexpr double PointMensuelIM[8][12] = {
                  { 4.534275,4.534275,4.556950,4.556950,4.556950,4.556950,4.556950,4.556950,4.556950,4.570625,4.570625,4.570625 },
                  { 4.570625,4.570625,4.570625,4.570625,4.570625,4.570625,4.593475,4.593475,4.593475,4.607258,4.607258,4.607258 },
                  { 4.607258,4.607258,4.607258,4.607258,4.607258,4.607258,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 },
                  { 4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 },
                  { 4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 },
                  { 4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 },
                  { 4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 },
                  { 4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 }};
 
 constexpr double part_proportionnelle_minimale(int Annee, int Mois, int Prop)
 {
   return PointMensuelIM[Annee - 2008][Mois - 1] * sft_prop[Prop - 1] * 449 / 100;
 }
 
 // [[Rcpp::export]]
  
 double sft(int prop, const string& indice, double nbi, double duree, int annee, int mois)   
 {
   if (duree  == 0 || indice.empty() || (indice.at(0) == 'N' && indice.at(1) == 'A')) return(0);  
    
   const char* ECHELLE_LETTRE_PATTERN = "H.*(E|é).*[A-F]";
   int indice_entier = 0;
    
   static const regex echelle_lettre {ECHELLE_LETTRE_PATTERN, regex::icase};
    
   indice_entier = (regex_match(indice, echelle_lettre))? 717 :  stoi(indice);

   indice_entier = stoi(indice);
   
   // Filtrer les nbi == NA
   
   indice_entier = indice_entier + nbi;
   
   double part_proportionnelle = 0;
   
   if (prop)
     part_proportionnelle =  sft_prop[prop - 1] * static_cast<double>(max(449, min(indice_entier, 717))) 
                                                * PointMensuelIM[annee - 2008][mois - 1];  
   
   // on prend en compte les quotités spécifiques de temps partiel
   // 0.91429  =  32/35 ; 0.85714 = 6/7
   
   double coef = (duree == 90)?  0.91429 : ((duree == 80)? 0.85714 : duree/100);
   
   double valeur = 
     (prop != 1)? 
     coef * (part_proportionnelle + sft_fixe[prop - 1])
       : coef * part_proportionnelle + sft_fixe[prop - 1];
   
   
   // vérification du plancher des attributions minimales à temps plein
   
   if (prop != 1) 
     valeur = max(valeur, part_proportionnelle_minimale(annee, mois, prop) + sft_fixe[prop - 1]);
     
     
     return(valeur);
     
 }
 
 
 
 
 
 /*
  double sft(...)  
  {
  if (is.na(durée) || is.na(x) || is.na(indice)) return(0);
  if (x > 15) return(-1);
  }
  */  
 
