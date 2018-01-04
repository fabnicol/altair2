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

/// \file matricules.h
/// \author Fabrice Nicol
/// \brief Code de la classe MatriculeInput, qui construit un dialogue d'extraction des bulletins


#ifndef MATRICULES_H
#define MATRICULES_H

#include <QDialog>
#include "common.h"
#include "flineframe.h"
#include "gui_enums.h"

/// Classe permettant de construire un dialogue d'extraction des bulletins de paye par matricule, année(s) et mois
class MatriculeInput : public QDialog
{
Q_OBJECT

public:

    /// Constructeur de la classe MatriculeInput
    /// \param  largeur du dialogue
    /// \param  hauteur en paramètres

    MatriculeInput (int largeur, int hauteur);

    QString matricules;  ///< Liste des séquences Matricule-Mois-Année (avec leurs plages éventuelles), séparées par des points virgules.

    /// Réinitialisation de tous les champs
    /// @return Booléen \em true sur les dialogue d'extraction était rempli, \em false sinon

    bool clear()
    {
        bool filled = ! matricules.isEmpty();
        matricules.clear();
        dossier->setText("");
        matrLineEdit->setText("");
        matrLineEdit2->setText("");
        matrLineEdit3->setText("");
        updateProject(true);
        return filled;
    }

    /// Vérifie qu'à l'ouverture du dialogue, le répertoire par défaut dirpath est bien renseigné.

    void checkDefaultFolder()
    {
        Q(dirpath)
        if (dossier->getText().isEmpty())
        {
            dossier->setText(dirpath);
            updateProject(true);
        }
    }

private:
    QDialogButtonBox *closeButton; ///< Bouton Quitter
    FLineEdit *matrLineEdit;       ///< Première ligne de matricules
    FLineEdit *matrLineEdit2;      ///< Deuxième ligne de matricules
    FLineEdit *matrLineEdit3;      ///< Troisième ligne de matricules
    FLineFrame* dossier;           ///< Dossier d'export des bulletins extraits
    QString dirpath;         ///< Chemin du dossier par défaut.

    /// Vérifie l'input (format de saisie)
    /// \param l CHaîne de caractères formatée selon l'usage :\n
    /// <ul><li>Pour un seul matricule : Matricule-Mois-Année</li>
    ///  <li>Pour une plage de mois et d'années : Matricule-Mois.début...Mois.fin-Année.début...Année.fin</li>
    /// </ul>\n
    /// Les séquences de ce type peuvent être ajoutées les unes aux autres séparées par un point-virgule.

    bool checkInput (FLineEdit* l);

signals:
    void updateProject(bool);
};


#endif // MATRICULES_H
