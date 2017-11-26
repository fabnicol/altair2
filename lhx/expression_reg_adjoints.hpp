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
#ifndef EXPRESSION_REG_ADJOINTS_HPP
#define EXPRESSION_REG_ADJOINTS_HPP

/// \file expression_reg_adjoints
/// \brief Expressions régulières de niveau adjoints / auxiliaires / aides (catégories C)

#include "expression_reg_commun.hpp"

/// administratif
#define ADMINISTRATIFS   "adm.*"

/// animation
#define ANIMATION        "ani.*"

/// technique
#define TECHNIQUES       "tech.*"

/// patrimoine
#define PATRIMOINE       "pat.*"

/// opérateur
#define OPERATEUR        "op(?:e|é).*"

/// aide de laboratoire
#define AIDE_LABO        "aide.*\\blab.*"

/// auxiliaire
#define AUXILIAIRE       "aux.*"

/// puericultrice
#define PUERICULTRICE    "puer.*"

#define SOIN             "soin"

/// gardien, garde-champêtre
#define GARDIEN          "gard(?:ien|.*ch.*)"

/// brigadier, sapeur, caporal, sous-oficier, adjudant, sergent
#define SOUS_OFF         "brigadier.*|sapeur.*|capor.*|sous.*off.*|adju.*|serg.*"

/// receveur
#define RECEVEUR         "receveur"

/// adjoint
#define ADJOINT          "adj.*"

//  SOIT A OU B OU ...  FIN_SOIT  OU ... OU ...

static constexpr auto EXPRESSION_REG_ADJOINTS = AUCUN_MOT
                                                  SOIT
                                                    ADJOINT
                                                      SOIT
                                                        ADMINISTRATIFS
                                                        OU ANIMATION
                                                        OU TECHNIQUES
                                                        OU PATRIMOINE
                                                      FIN_SOIT
                                                    OU OPERATEUR
                                                      SOIT
                                                        "a\\.?p\\.?s\\.?|act"
                                                      FIN_SOIT
                                                    OU AIDE_LABO
                                                    OU AUXILIAIRE
                                                      SOIT
                                                        PUERICULTRICE OU SOIN
                                                      FIN_SOIT
                                                    OU GARDIEN
                                                    OU SOUS_OFF
                                                    OU RECEVEUR
                                                  FIN_SOIT
                                                ETC;

#endif // EXPRESSION_REG_ADJOINTS_HPP
