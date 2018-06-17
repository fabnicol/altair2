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
#ifndef EXPRESSION_REG_ELUS_HPP
#define EXPRESSION_REG_ELUS_HPP

#include "expression_reg_commun.h"


/// \file expression_reg_elus.h
/// \author Fabrice Nicol
/// \brief Expressions régulières représentant les élus


/// maire
#define MAIRE                                    "maire"

/// président
#define PRESIDENT                                "pr..?sident"

/// élu
#define ELUS                                     "[eé]lus?"

/// adjoint au maire
#define ADJOINT_MAIRE                            "adj.*\\bmaire\\b"

/// vice-président
#define VICE_PRESIDENT                           "vi.*\\bpr..?sident\\b"

/// conseiller municipal
#define CONSEILLER_MUNIC                         "cons.*\\bmuni"

/// conseiller communautaire
#define CONSEILLER_COMMUN                        "cons.*\\bcomm"

/// conseiller délégué
#define CONSEILLER_DELEGUE                       "(?:cons.*\\bd\\S*|d..?..?gu)"

static constexpr auto EXPRESSION_REG_ELUS = DEBUT
        SOIT
        MAIRE
        OU PRESIDENT
        OU ELUS
        OU ADJOINT_MAIRE
        OU VICE_PRESIDENT
        OU CONSEILLER_MUNIC
        OU CONSEILLER_COMMUN
        OU CONSEILLER_DELEGUE
        FIN_SOIT
        ETC;

#endif // EXPRESSION_REG_ELUS_HPP
