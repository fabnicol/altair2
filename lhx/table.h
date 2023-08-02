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
#ifndef TABLE_HPP_INCLUDED
#define TABLE_HPP_INCLUDED
#include "validator.h"
#include "tags.h"
#include <vector>
#include <string>
#include <iostream>

void generer_table_bulletins (info_t* Info);
int64_t generer_table_standard (const char* chemin_table, std::vector<info_t> &info);
extern bool verbeux;
std::pair<uint64_t, uint32_t>  boucle_ecriture (std::vector<info_t> &info, int segment);

#if defined(__WIN32__) && defined(USE_ICONV)
static inline void convertir (const std::string& filepath)
{

    if (verbeux) std::cerr << PROCESSING_HTML_TAG "Conversion au format Latin-1..."  ENDL;

    std::system ((std::string ("set PATH=%PATH%; C:/Users/Public/Dev/altair/Outils && CALL iconv.exe -c -s -f UTF-8 -t ISO-8859-1 " )
                  + filepath + ".temp" + CSV
                  + std::string (" > ")
                  + filepath + CSV + " 2>/dev/null").c_str());

    if (verbeux) std::cerr << PROCESSING_HTML_TAG "Conversion terminée"  ENDL;
}
#endif

#if !defined SANITIZING_QUOTES && !defined NO_SANITIZING_QUOTES
  #define  QUOTE(X) { if (VAR(X) == nullptr || VAR(X)[0] == 0) base << sep ; else base << '"' << VAR(X) << '"' << sep;}
  #define  QUOTE2(X, Y) { if (VAR(X) == nullptr || VAR(X)[0] == 0) \
                           { \
                                if (VAR(Y) != nullptr && VAR(Y)[0] != 0) \
                                  base << " - " << VAR(Y) << '"' << sep; \
                                else \
                                  base << sep; \
                           } else { \
                                base << '"' << VAR(X) << " - " ; \
                                if (VAR(Y) != nullptr) \
                                    base << VAR(Y) << '"' << sep; \
                                else \
                                    base << '"' << sep; \
                          }\
                        }

  #define  QUOTE_EOL(X) { if (VAR(X) == nullptr || VAR(X)[0] == 0) base << sep; else base << '"' << VAR(X) << '"' << "\n";}
#else
  #define  QUOTE(X) base << VAR(X) << sep;
  #define  QUOTE2(X, Y) base << VAR(X) << " - " << VAR(Y) << sep ;
  #define  QUOTE_EOL(X) base << VAR(X) << "\n";
#endif

#define NO_QUOTE(X) { if (VAR(X) == nullptr || VAR(X)[0] == 0) base << sep ; else base << VAR(X) << sep; }

#endif // TABLE_HPP_INCLUDED
