/*  
 *  Sous linux : Utiliser boost/regex.hpp
 *  
 *  Les r�sultats sont 15 % plus rapides qu'avec la biblioth�que standard de C++14 (G++) sous linux.
 *  Sous windows, m�me performances que sous linux � 7-8 % pr�s, mais probl�me de compilation sous boost.
 *  Ecart de performances beaucoup plus grand sous Windows entre R et Rcpp (x15) que sous linux (x3) : 
 *  gain de 1 s par million de lignes sous Windows, de 150 ms seulement sous linux.
 *  
 *  Sous linux, la compilation sera automatiquement r�alis�e avec boost, sauf si la directive
 *  FORCE_STL_BUILD est donn�e au compilateur.
 *  
 *  Les compilations boost n�cessitent de pr�ciser la variable d'environnement  PKG_LIBS="-lboost_regex" pour R CMD build et R CMD INSTALL.
 *  
 *  La directive d'exportation Rcpp::plugins(cpp11) peut �tre retir�e si constexpr et std::regex ne sont pas
 *  utilis�s. S'il est laiss�, la compilation peut impliquer l'exportation de la variable d'environnement PKG_CXXFLAGS="-std=c++11"
 *  pour un compilateur G++ de version inf�rieure � 5.
 */

// [[Rcpp::depends(BH)]]
#include <Rcpp.h>

#if defined(__linux__) && ! defined(FORCE_STL_BUILD)
  #include <boost/regex.hpp>
  #define reglib boost
#else
#ifdef USE_REGEX
#include <boost/regex.hpp>
  #define reglib boost
#endif
#endif

#include <string>

using namespace Rcpp;


// [[Rcpp::plugins(cpp11)]]


/* Ne jamais utiliser using namespace std, mais pr�fixer std::, sous peine de probl�mes d'exportation des symboles */

/*
*  on pr�voit 15 enfants...
*
*  limitations : pas de v�rification des cas de divorce etc., ni des cas de cumuls
*                pas de v�rification non plus de la lic�it� des versements � des contractuels exclus par l'article 1er 
*                du d�cret n�85-1148 du 24 octobre 1985 modifi� relatif � la r�mun�ration des personnels civils et militaires
*                de l'Etat, des personnels des collectivit�s territoriales et des personnels des �tablissements publics d'hospitalisation. 
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
  
const double  sft_prop[15] =  {0, 3, 8, 8 + 6,
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
  
const double PointMensuelIM[8][12] = {
                  { 4.534275,4.534275,4.556950,4.556950,4.556950,4.556950,4.556950,4.556950,4.556950,4.570625,4.570625,4.570625 },
                  { 4.570625,4.570625,4.570625,4.570625,4.570625,4.570625,4.593475,4.593475,4.593475,4.607258,4.607258,4.607258 },
                  { 4.607258,4.607258,4.607258,4.607258,4.607258,4.607258,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 },
                  { 4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 },
                  { 4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 },
                  { 4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 },
                  { 4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 },
                  { 4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292,4.630292 }};
 
 const double part_proportionnelle_minimale(int Annee, int Mois, int Prop)
 {
   return PointMensuelIM[Annee - 2008][Mois - 1] * sft_prop[Prop - 1] * 449 / 100;
 }
 
 // [[Rcpp::export]]  
 double sft(int prop, const std::string& indice, double nbi, double duree, int annee, int mois)   
 {
   if (duree  == 0 || indice.empty() || (indice.at(0) == 'N' && indice.at(1) == 'A')) return(0);  
   int indice_entier = 0;
   
  #ifdef USE_REGEX 

     const char* ECHELLE_LETTRE_PATTERN = "H.*(E|�).*[A-F]";
   
     static const reglib::regex echelle_lettre {ECHELLE_LETTRE_PATTERN, reglib::regex::icase};
    
     indice_entier =  reglib::regex_match(indice, echelle_lettre)? 717 : std::stoi(indice);
     
  #else
     
     for (const char c: {'A', 'B', 'C', 'D', 'E', 'F'}) 
     {
       if (indice.find(c) != std::string::npos)
       {
         indice_entier = 717;
         break;
       }
     }
     
     if (indice_entier == 0) indice_entier = std::stoi(indice);
     
   #endif

   indice_entier += nbi;
   
   double part_proportionnelle = 0;
   
   if (prop)
     part_proportionnelle =  sft_prop[prop - 1] * static_cast<double>(std::max(449, std::min(indice_entier, 717))) 
                                                * PointMensuelIM[annee - 2008][mois - 1];  
   
   // on prend en compte les quotit�s sp�cifiques de temps partiel
   // 0.91429  =  32/35 ; 0.85714 = 6/7
   
   double coef = (duree == 90)?  0.91429 : ((duree == 80)? 0.85714 : duree/100);
   
   double valeur = 
     (prop != 1)? 
     coef * (part_proportionnelle + sft_fixe[prop - 1])
       : coef * part_proportionnelle + sft_fixe[prop - 1];
   
   
   // v�rification du plancher des attributions minimales � temps plein
   
   if (prop != 1) 
     valeur = std::max(valeur, part_proportionnelle_minimale(annee, mois, prop) + sft_fixe[prop - 1]);
     
     
     return(valeur);
     
 }
 
 
 /*
  double sft(...)  
  {
  if (is.na(dur�e) || is.na(x) || is.na(indice)) return(0);
  if (x > 15) return(-1);
  }

  
  */  
 
