// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie,
// de modification et de redistribution accordés par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
// seule une responsabilité restreinte pèse sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les concédants successifs.
//
// A cet égard l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels
// avertis possédant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invités à charger et tester l'adéquation du
// logiciel à leurs besoins dans des conditions permettant d'assurer la
// sécurité de leurs systèmes et ou de leurs données et, plus généralement,
// à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accepté les
// termes.
//
//
/* 
   on pr�voit 15 enfants...
 
   limitations : pas de v�rification des cas de divorce etc., ni des cas de cumuls
                 pas de v�rification non plus de la lic�it� des versements � des contractuels exclus par l'article 1er 
                 du d�cret n�85-1148 du 24 octobre 1985 modifi� relatif � la r�mun�ration des personnels civils et militaires
                 de l'Etat, des personnels des collectivit�s territoriales et des personnels des �tablissements publics d'hospitalisation. 
*/ 


#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include "sft.h"


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

SEXP  sft_C(SEXP Prop, SEXP Indice, SEXP Echelon, SEXP Nbi, SEXP Duree, SEXP Annee, SEXP Mois)
{
//          Prop = "integer", Indice = "character", Nbi = "numeric", Duree = "numeric",
//            Annee = "integer", Mois = "integer"))
// corps de la fonction C++
                    
   int prop = asInteger(Prop);
   const char* echelon = CHAR(asChar(Echelon));
   int indice = asInteger(Indice);
   int nbi = asInteger(Nbi);
   double duree = asReal(Duree);
   int annee = asInteger(Annee);
   int mois = asInteger(Mois);   

   const double  sft_fixe[15]= {2.29, 10.67, 15.24,
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

   double valeur = 0;    
   double coef = 0;
   double part_proportionnelle = 0;
   int indice_entier = 0;

 #ifdef USE_REGEX
   const char* ECHELLE_LETTRE_PATTERN = "H.*(E|�).*[A-F]";
   
   static const std::regex echelle_lettre {ECHELLE_LETTRE_PATTERN, std::regex::icase};
 #endif

// on a en g�n�ral indice = 0 pour les hors �chelle lettres qui de mani�re absurde voient leur HE indiqu�e dans l'�chelon...

   
  #ifdef USE_REGEX 

     indice_entier =  std::regex_match(echelon, echelle_lettre)? 717 : indice;
     
  #else
     
     const char letters[] = {'A', 'B', 'C', 'D', 'E', 'F'};
     
     for (int j = 0; echelon[j] != 0; ++j)
       for (int i = 0; i < 6; ++i) 
       {
  		   if (echelon[j] == letters[i] && (echelon[j + 1] == '1' || echelon[j + 1] == '2' || echelon[j + 1] == '3'))
  		   {
  			   indice_entier = 717;
  			   goto out;
  		   }
       }
     
	 out : 
	 
       if (indice_entier == 0) indice_entier = indice;
     
  #endif

   indice_entier += nbi;
   
   // "Pour les personnels non r�mun�r�s par un traitement �tabli en application de l'article 2 pr�cit�,
   // l'�l�ment proportionnel est calcul� en pourcentage du traitement aff�rent � l'indice major� 449 (indice brut 524)." art. 10 bis d�cretn�85-1148
   
   if (indice_entier > 717) indice_entier = 717;
   if (indice_entier < 449) indice_entier = 449;
   
   if (prop)
     part_proportionnelle =  sft_prop[prop - 1]/100 * (double) indice_entier
                                                * PointMensuelIM[annee - 2008][mois - 1];  
   
   // on prend en compte les quotit�s sp�cifiques de temps partiel
   // 0.91429  =  32/35 ; 0.85714 = 6/7
   
   coef = (duree == 90)?  0.91429 : ((duree == 80)? 0.85714 : duree/100);
   
   valeur = 
     (prop != 1)? 
     coef * (part_proportionnelle + sft_fixe[prop - 1])
       : coef * part_proportionnelle + sft_fixe[prop - 1];
  
   // v�rification du plancher des attributions minimales � temps plein
   
   double test = part_proportionnelle_minimale(annee, mois, prop) + sft_fixe[prop - 1];
   
   if (prop != 1 && test > valeur) 
     valeur = test;
     

   return(ScalarReal(valeur));
   
}    
 
static const R_CallMethodDef callMethods[]  = {
   {"sft_C", (DL_FUNC) &sft_C, 7},
   {NULL}
 };
 
 void R_init_sft(DllInfo *info) 
{
   R_registerRoutines(info, NULL, callMethods, NULL, NULL);
}


 

