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
// /////////////////////////////////////////////////////////////////////////

/// \file common.h
/// \author Fabrice Nicol
/// \brief Code de la classe common, qui regroupe des utilitaires communs à l'interface graphique  MainWindow, à son agent  Altair et à d'autres classes

#ifndef COMMON_H
#define COMMON_H
#include <QtWidgets>
#include "fwidgets.h"
#include "tools.h"


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


class common : public tools
{

public:

    common() {}

    /// Exporter les caractéristiques du contrôle dans le rapport : Siret, Budget, Employeur, Etablissement
    /// \param  file_str Chaîne de caractères dans laquelle insérer ces informations
    /// \param  subdir Répertoire du rapport

    void exporter_identification_controle (QString & file_str, const QString &subdir = "");

    /// Exporter les caractéristiques du contrôle dans le rapport : Siret, Budget, Employeur, Etablissement
    /// \note utilise le fichier #SCRIPT_DIR "prologue_codes.R"
    /// \param subdir Répertoire du rapport

    void exporter_identification_controle (const QString &subdir = "");

#ifdef LOCAL_BINPATH

   QString execPath = path_access (System); ///< Chemin du répertoire lhx, soit ~/Dev/altair/linux/lhx sous linux et ~/Dev/altair/win/lhx sous Windows
   const QString sharedir = generateDatadirPath(); ///< Chemin ~/.local/share/Altair
   QString altairCommandStr = execPath + QDir::separator() + ("lhx" + QString (systemSuffix)); ///< Ligne de commande permettant de lancer lhx


#  ifdef MINIMAL

     QString RAltairDirStr = path_access ("R/bin/x64");
     // Passer les '/' soit avec QDir::toNativeSeparators() soit utiliser QDir::separator() sous Windows.
     QString RAltairCommandStr = RAltairDirStr + QDir::separator() + "Rscript" + QString (systemSuffix);

#  else

#    ifdef __WIN32__

        /// Ligne de commande permettant de lancer RStudio
        QString RAltairDirStr = path_access ("RStudio");
        QString RAltairCommandStr = RAltairDirStr + QDir::separator() + "bin" + QDir::separator() + "rstudio" + QString (systemSuffix) ;

#    else

        /// Ligne de commande permettant de lancer RStudio
        QString RAltairCommandStr = QString ("/usr/bin/rstudio");

#    endif
#  endif

#else

    QString execPath = PREFIX + QString ("/bin"); ///< Chemin du répertoire contenant lhx
    QString sharedir = PREFIX + QString ("/share/applications/altair");
    /// Ligne de commande permettant de lancer RStudio
    QString RAltairDirStr = QString ("/lib/rstudio/bin");
    QString RAltairCommandStr = PREFIX + RAltairDirStr + QString ("rstudio"); ///< Ligne de commande permettant de lancer RStudio
    QString altairCommandStr = PREFIX + QString ("/bin/lhx"); ///< Ligne de commande permettant de lancer lhx

#endif

};

#endif // COMMON_H
