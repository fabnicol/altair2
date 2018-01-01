// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser
// les fichiers de paye produits au format spécifié par l'annexe de la
// convention-cadre nationale de dématérialisation en vigueur à compter de
// l'année 2008.
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
// //////////////////////////////////////////////////////////////////////////

/// \file altair.h
/// \author Fabrice Nicol
/// \brief Implémentation de la classe  common qui comprend des utilitaires communs

#include "common.h"
#include "gui_enums.h"

void common::exporter_identification_controle (QString &file_str, const QString &subdir)
{
    const QString &employeur = Hash::aplatir (Hash::Employeur, ", ", subdir);
    const QString &budget = Hash::aplatir (Hash::Budget, ", ", subdir);
    const QString &siret = Hash::aplatir (Hash::Siret, " - ", subdir);
    const QString &etablissement = Hash::aplatir (Hash::Etablissement, " - ", subdir);

    substituer ("controle<-c\\(\"\",\"\",\"\",\"\"\\)", "controle<-c(\""
                + employeur + "\",\""
                + siret + "\",\""
                + etablissement + "\",\""
                + budget + "\")",
                file_str);
}

void common::exporter_identification_controle (const QString &subdir)
{
    const QString &prologue_code_path = path_access (SCRIPT_DIR "prologue_codes.R");
    QString file_str = readFile (prologue_code_path);
    exporter_identification_controle (file_str, subdir);

    exportpath = (subdir.isEmpty()) ? prologue_code_path  : common::path_access (DONNEES_SORTIE) + QDir::separator() + subdir + "/prologue_codes.R";
    renommer(dump (file_str), exportpath);
}





