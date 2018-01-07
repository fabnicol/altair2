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
        if (dossier->getText().isEmpty())
        {
            dossier->setText(dirpath);
            updateProject(true);
        }
    }

public slots:

    /// Connecteur qui rajoute une ligne de saisie de matricules au dialogue <b>Exportation des bulletins</b>

    void ajouterLigneMatricules();

private:

    QGridLayout* layout;                 ///< Disposition graphique du dialogue (\em layout).
    QDialogButtonBox *closeButton; ///< Bouton Quitter.
    FLineEdit *matrLineEdit;       ///< Première ligne de matricules.
    FLineEdit *matrLineEdit2;      ///< Deuxième ligne de matricules.
    FLineEdit *matrLineEdit3;      ///< Troisième ligne de matricules.
    QList<FLineEdit*> matrLineEditList; ///< Liste de lignes de matricules ajoutées.
    QList<QToolButton*> eraseButtonList; ///< Liste des boutons d'effacement de ligne.
    FLineFrame* dossier;           ///< Dossier d'export des bulletins extraits
    QString dirpath;         ///< Chemin du dossier par défaut.

    /// Vérifie l'input (format de saisie).
    /// Pour extraire le bulletin d'un agent, rentrer son matricule, suivi d'un tiret \n
    /// puis le numéro du mois (de 1 à 12), suivi d'un tiret, et l'année.\n
    /// Utiliser ... pour indiquer une plage de valeurs.\n
    /// Insérer un point-vigule avant la demande suivante, sans espace.\n
    /// Lorsque les années sont données sur deux chiffres au lieu de 4, elles sont\n
    /// réputées être postérieures à l'an 2000.\n
    /// Il est possible de regrouper plusieurs matricules, pour une même valeur\n
    /// du ou des mois et années. Les matricules sont alors séparés par des virgules.\n
    /// Les séquences peuvent être ajoutées les unes aux autres, séparées par un point-virgule.
    /// \param l Chaîne de caractères formatée.
    /// \return Booléen : \em true si la formet est correct, \em false sinon.
    /// \remarks
    /// Par exemple : \n\n
    ///  1058 N-3-2010;1010 B-7...9-2012...2014\n\n
    ///  pour extraire les bulletins des agents de matricule :\n\n
    ///  1058 N en mars 2010\n
    ///  1010 B en juillet, août, septembre 2012 à 2014.\n\n
    /// Autre exemple : \n\n
    ///  1059,1092,1011-1...3-15...17\n\n
    ///  pour extraire les bulletins des agents de matricule :\n\n
    ///  1059, 1092 et 1011 en janvier, février et mars 2015, 2016 et 2017.

    bool checkInput (FLineEdit* l);

signals:
    /// Signal envoyé pour forcer l'enregistrement du projet .alt.
    void updateProject(bool);
};


#endif // MATRICULES_H
