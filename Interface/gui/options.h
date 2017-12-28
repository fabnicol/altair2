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


/// \file options.h
/// \author Fabrice Nicol
/// \date 2015-2017
/// \brief Code des classes du dialogue d'options


#ifndef OPTIONS_H
#define OPTIONS_H

#include "flineframe.h"
#include "common.h"


class options;
class Altair;
class RAltairPage;
class QToolDirButton;
class FListFrame;
class FLineFrame;


class processPage :  public QDialog, public common
{
    Q_OBJECT

public :
    processPage();                 ///< Constructeur de l'onglet Traitement.
    FLineFrame *logFrame;          ///< fwidget de type QLineEdit augmenté de fonctionnalités spéciales. Enregistre le chemin du log.
    FCheckBox  *rapportEntier;     ///< Case à cocher indiquant si le rapport doit être dans sa version longue (ou courte si non cochée).
    FCheckBox  *enchainerRapports; ///< Cas à cocher indiquant si la génération des rapports est automatiquement lancée par l'interface sans passer par RStudio.

private:
    FComboBox
        *processTypeWidget,    ///< Sélectionne le nombre de fils d'exécution.
        *memoryUseWidget,      ///< Sélectionne le pourcentage d'utilisation de la RAM disponible.
        *rapportTypeWidget;    ///< Sélectionne le type de rapport en sortie (PDF, DOCX et ODT ou les trois formats).

    QGroupBox
        *processTypeBox;       ///< Groupe "Mode d'exécution"

    FCheckBox
        *logCheckBox,          ///< Case à cocher permettant d'activer/de désactiver logFrame.
        *consoleCheckBox;      ///< Case à cocher permettant d'activer la console (cochée par défaut).

    QLabel
        *nLineLabel,
        *NLineLabel;
};


/// Classe de l'onglet "Format des bases" du dialogue d'options

class standardPage :  public QDialog, public common
{
    Q_OBJECT

public :

    /// Constructeur de l'onglet "Format des bases"

    standardPage();

    FCheckBox
      *tableCheckBox,         ///< Exporter la table ou pas
      *FPHCheckBox;           ///< Données Fonction publique hospitalière ou pas
    FComboBox* exportWidget;  ///< Modalités d'exportation (standard, cumulative, distributive etc.)

private:
    FComboBox  *baseTypeWidget;   ///< Type de base en sortie : "Standard", "Par année", "Toutes catégories", "Traitement" etc.

    FLineEdit *maxNLigneLineEdit; ///< Nombre maximal de lignes de la base

    QGroupBox  *baseTypeBox;      ///< Boîte de présentation Type de base

    FCheckBox
      *etabCheckBox,              ///< Exporter l'établissement ou pas
      *rangCheckBox;              ///< Numéroter les lignes ou pas

    QLabel *maxNLigneLabel;       ///< Titre de  maxNLigneLineEdit

private slots:
    void substituer_versant();    ///< Exporter le versant FPH ou FPT dans le script R pour générer les tests adéquats

};


class codePage :  public QDialog, public common
{
    Q_OBJECT

public :
    /// Constructeur du dialogue "Code de paye des tests"

    codePage();

    void resetLabel()
    {
        label->setText ("") ;
    }

private:
    QString prologue_codes_path;
    QStringList variables;
    QList<FLineEdit*> listeCodes;
    QList<QString> listeLabels;
    QList<QLabel*> listeDialogueLabels;
    QLabel *label;
    QString init_label_text ;
    QToolButton* appliquerCodes ;
    QGridLayout *vLayout = new QGridLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QGroupBox *baseBox;


    /// Crée une ligne de codes pour un type donné d'éléments de paye
    /// \param nom Nom de l'élément de paye
    /// \return Nombre d'éléments de paye empilés

    int ajouterVariable (const QString& nom);

    /// Réinitialise l'exportation des codes d'éléments de paye
    /// Ecrase prologue_codes.R ( prologue_codes_path) par sa valeur d'initialisation prologue_init.R
    /// \return \e true si la réinitialisation par écrasement a réussi, \e false sinon.

    bool reinitialiser_prologue();

private slots:
    void substituer_valeurs_dans_script_R();
    void activer_fph (bool);
};


class options :   public QDialog, public common
{
    Q_OBJECT

public:

    options (Altair* parent = 0);
    standardPage* standardTab;         ///< Onglet d'accueil Format : type de base en sortie, modalité d'extraction des données, exportation/archivage
    processPage* processTab;           ///< Onglet de Traitement : nombre de fils, utilisation de la mémoire, log et enchainement avec la production des rapports
    codePage* codeTab;                 ///< Onglet des codes de paye : renseignement manuel des codes de paye utilisés pour certains types d'éléments de paye
    static std::uint16_t RefreshFlag;  ///< Drapeau indiquant si si l'interface a été actualisée ou doit l'être
    QListWidget *optionWidget;         ///< composant du dialogue d'options
    void clearOptionData();            ///< Efface les données de  Hash::wrapper et de  Hash::Reference

private:

    QDialogButtonBox *closeButton;     ///< Bouton "Quitter"
    QStackedWidget *pagesWidget;       ///< composant permettant d'empiler les onglets du dialogue d'options (standardPage,  processPage,  codePage)

    void createIcons();                                     ///< Méthode permettant d'ajouter les icones
    void createIcon (const char* path, const char* text);

private slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous); ///< Changement de page
    void enchainerRapports (int index);                                   ///< Code la case "Enchainer les rapports" qui permet de lancer R pour produire des rapports d'analyse à partir de l'interface sans lancer RStudio
};
#endif // OPTIONS_H
