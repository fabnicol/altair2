/* 
   on prévoit 15 enfants...
 
   limitations : pas de vérification des cas de divorce etc., ni des cas de cumuls
                 pas de vérification non plus de la licéité des versements à des contractuels exclus par l'article 1er 
                 du décret n°85-1148 du 24 octobre 1985 modifié relatif à la rémunération des personnels civils et militaires
                 de l'Etat, des personnels des collectivités territoriales et des personnels des établissements publics d'hospitalisation. 
*/ 

#include "sft2.h"
#include <R.h>
#include <Rinternals.h>

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

//                          cxxargs = "-std=c++11 -DUSE_REGEX"

SEXP  sft_(SEXP Prop, SEXP Indice, SEXP Nbi, SEXP Duree, SEXP Annee, SEXP Mois)
{
//          rop = "integer", Indice = "character", Nbi = "numeric", Duree = "numeric",
//            Annee = "integer", Mois = "integer"))
// corps de la fonction C++
                    
   int prop = asInteger(Prop);
   const std::string indice = std::string(CHAR(asChar(Indice)));
   int nbi = asInteger(Nbi);
   double duree = asReal(Duree);
   int annee = asInteger(Annee);
   int mois = asInteger(Mois);   

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

   double valeur;    
   double coef;
   double part_proportionnelle = 0;
   int indice_entier = 0;

 #ifdef USE_REGEX
   const char* ECHELLE_LETTRE_PATTERN = "H.*(E|é).*[A-F]";
   
   static const std::regex echelle_lettre {ECHELLE_LETTRE_PATTERN, std::regex::icase};
 #endif

   if (duree  == 0 || indice.empty() || (indice.at(0) == 'N' && indice.at(1) == 'A')) 
   {
     valeur = 0;
     goto fin;
   } 

  #ifdef USE_REGEX 

    
    
     indice_entier =  std::regex_match(indice, echelle_lettre)? 717 : std::stoi(indice);
     
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
   

   if (prop)
     part_proportionnelle =  sft_prop[prop - 1] * static_cast<double>(std::max(449, std::min(indice_entier, 717))) 
                                                * PointMensuelIM[annee - 2008][mois - 1];  
   
   // on prend en compte les quotités spécifiques de temps partiel
   // 0.91429  =  32/35 ; 0.85714 = 6/7
   
   coef = (duree == 90)?  0.91429 : ((duree == 80)? 0.85714 : duree/100);
   
   valeur = 
     (prop != 1)? 
     coef * (part_proportionnelle + sft_fixe[prop - 1])
       : coef * part_proportionnelle + sft_fixe[prop - 1];
   
   
   // vérification du plancher des attributions minimales à temps plein
   
   if (prop != 1) 
     valeur = std::max(valeur, part_proportionnelle_minimale(annee, mois, prop) + sft_fixe[prop - 1]);
     
fin:
   //SEXP out = PROTECT(allocVector(REALSXP, 1));
   //REAL(out)[0] = valeur;

   return(ScalarReal(valeur));
   
}    
 
 
 

