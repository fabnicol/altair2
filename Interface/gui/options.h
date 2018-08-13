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

/// Classe de l'onglet Traitement du dialgue d'options
/// 
/// Cette classe précise les modalités d'exécution de l'application-noyau \em lhx (nombre de fils d'exécution, consommation mémoire, log, mode d'exécution cumulatif, distributif ou standard, etc.;) et le type de rapports d'analyse produits (version avancée ou standard).

class processPage :  public QDialog, public common
{
    Q_OBJECT

public :
    processPage();                 ///< Constructeur de l'onglet Traitement.
    FLineFrame *logFrame;          ///< composant fonctionnelde type QLineEdit augmenté de fonctionnalités spéciales. Enregistre le chemin du log d'exécution.
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
    FComboBox  *baseTypeWidget;   ///< Type de base en sortie : "Standard", "Par année", "Par agent", "Toutes catégories", "Traitement" etc.

    FLineEdit *maxNLigneLineEdit; ///< Nombre maximal de lignes de la base

    QGroupBox  *baseTypeBox;      ///< Boîte de présentation Type de base

    FCheckBox
      *etabCheckBox,              ///< Exporter l'établissement ou pas
      *rangCheckBox;              ///< Numéroter les lignes ou pas

    QLabel *maxNLigneLabel;       ///< Titre de  maxNLigneLineEdit

private slots:
    void substituer_versant();    ///< Exporter le versant FPH ou FPT dans le script R pour générer les tests adéquats

};


/// Classe de l'onglet Codes permettant d'exporter des codes de paye manuellement spécifiés vers les scripts R.

class codePage :  public QDialog, public common
{
    Q_OBJECT

public :
    /// Constructeur de l'onglet "Code de paye des tests"

    codePage();

    /// Réinitialisation de l'élément de texte variable de l'onglet.
    
    void resetLabel()
    {
        label->setText ("") ;
    }

     static QString prologue_codes_path;  ///< Chemin initial de \em prologue_codes.R sous #SCRIPT_DIR. Ce fichier contient les exportations de valeurs de l'onglet \b Codes de l'interface graphique.
     
private:
    QList<FLineEdit*> listeCodes; ///< Liste des pointeurs vers des composants fonctionnels de classe FLineEdit, qui rassemble l'ensemble des lignes correspondant à variables.
    QGridLayout *vLayout = new QGridLayout;    ///< Disposition secondaire.
    QVBoxLayout *mainLayout = new QVBoxLayout; ///< Disposition principale.
    QGroupBox *baseBox;                        ///< Boite regroupant les codes.    
    /// Crée une ligne de codes pour un type donné d'éléments de paye
    /// \param nom Nom de l'élément de paye
    /// \return Nombre d'éléments de paye empilés

    int ajouterVariable (const QString& nom);
protected : 
   
    QStringList variables;        ///< Libellés des éléments de paye faisant l'objet d'une ligne dans l'onglet.
    QList<QString> listeLabels;   ///< Elements de variables dont les espaces ont été retirées.
    QList<QLabel*> listeDialogueLabels;        ///< Mise en forme des éléments de listeLabels au format QLabel.
    QLabel *label;                             ///< Elément de texte variable servant à afficher des messaes d'erreur ou de réussite de l'exportation des codes.
    QString init_label_text ;                  ///< Message "Appuyer pour exporter..."
    QToolButton* appliquerCodes ;              ///< Bouton "Exporter" (flèche verte) .
  
    
private slots:
    
    /// Substitue certaines valeurs caractéristiques du contrôle dans le fichier \em prologue_codes.R dans common::exportpath, notamment le Siret, le budget et l'employeur
    
    void substituer_valeurs_dans_script_R();
    
    /// Active les contrôles réglementaires sur la fonction publique hospitalière (FPH) même s'ils n'ont pas été activés par reconnaissnce automatique.
    
    void activer_fph (bool);
};

class rapportPage :  public  QDialog, public common
{
    Q_OBJECT

public :
    /// Constructeur de l'onglet "Code de paye des tests"

    rapportPage();

    
private:
    QList<FCheckBox*> listeCB; ///< Liste des pointeurs vers des composants fonctionnels de classe FCheckBox, qui rassemble l'ensemble des cases à cocher pour sélectionner les parties du rapport.
    QGridLayout *vLayout = new QGridLayout;    ///< Disposition secondaire.
    QVBoxLayout *mainLayout = new QVBoxLayout; ///< Disposition principale.
    QGroupBox *baseBox;                        ///< Boite regroupant les codes.    
    /// Crée une ligne de codes pour un type donné d'éléments de paye
    /// \param nom Nom de l'élément de paye
    /// \return Nombre d'éléments de paye empilés

    QStringList variables;        ///< Libellés des éléments de paye faisant l'objet d'une ligne dans l'onglet.
     QLabel *label;                             ///< Elément de texte variable servant à afficher des messaes d'erreur ou de réussite de l'exportation des codes.
    QString init_label_text ;                  ///< Message "Appuyer pour exporter..."
    QToolButton* appliquerCodes ;              ///< Bouton "Exporter" (flèche verte) .

    /// Crée une ligne de codes pour un type donné d'éléments de paye
    /// \param nom Nom de l'élément de paye
    /// \return Nombre d'éléments de paye empilés

    int ajouterVariable (const QString& nom);

private slots:
    
    /// Substitue certaines valeurs caractéristiques du contrôle dans le fichier \em prologue_codes.R dans common::exportpath, notamment le Siret, le budget et l'employeur
    
    void substituer_valeurs_dans_script_R();
    
};

/// Classe de l'onglet Extra permettant d'importer les fichiers auxiliaires optionnels (correspondance budgétaire, fichier des catégories statutaires rectifié, concessions de logement, plafonds IFSE)     

class extraPage: public QDialog, public common
{

Q_OBJECT

public:
    /// Constructeur de la classe extraPage
    
    extraPage();
    
    FLineFrame *budgetFrame, ///< Groupe ligne de chemin + navigateur de fichiers pour la correspondance entre codes, libellés de paye et sous-comptes du 64 (données externes à importer)
               *gradesFrame, ///< Groupe ligne de chemin + navigateur de fichiers pour la correspondance entre grades et catégorie statutaire (données externes à importer)
               *logtFrame,   ///< Groupe ligne de chemin + navigateur de fichiers pour la correspondance entre matricule et concession de logement, par année et mois (données externes à importer)
               *ifseFrame;   ///< Groupe ligne de chemin + navigateur de fichiers pour la correspondance entre grade, groupe fonctionnel et plafond en euros pour l'IFSE (RIFSEEP)  
    
    void do_copies();        ///< Réalise les copies des fichiers auxiliaires paye_budget.csv, grades.catégories.csv et logements.csv dans Tests/Exemple pour importation par le script R.  
   
private:   
    
    FCheckBox *budgetCheckBox,  ///< Case à cocher de contrôle de \link budgetFrame
              *gradesCheckBox,  ///< Case à cocher de contrôle de \link gradesFrame
              *logtCheckBox,    ///< Case à cocher de contrôle de \link logtFrame
              *ifseCheckBox;    ///< Case à cocher de contrôle de \link ifsetFrame
};


/// Classe de l'onglet Codes, qui permet de saisir manuellement les codes de paye de certaines catégories de lignes de paye.
/// 
/// Les codes différents, pour la même ligne, sont séparés par des points-virgules. Ils doivent être exportés dans le fichier common::exportpath en cliquant sur la flèche verte de l'onglet.

class options :   public QDialog, public common
{
    Q_OBJECT

public:

    options (Altair* parent = 0);      ///< Constructeur de l'onglet correspondant à un pointeur \em parent vers une instance de la classe Altair
    standardPage* standardTab;         ///< Onglet d'accueil Format : type de base en sortie, modalité d'extraction des données, exportation/archivage
    processPage* processTab;           ///< Onglet de Traitement : nombre de fils, utilisation de la mémoire, log et enchainement avec la production des rapports
    codePage* codeTab;                 ///< Onglet des codes de paye : renseignement manuel des codes de paye utilisés pour certains types d'éléments de paye
    extraPage* extraTab;               ///< Onglet des fichiers auxiliaires : importation de fichiers externes optionnels permetant de préciser/corriger la génération des rapports d'analyse.  
    rapportPage* rapportTab;           ///< Onglet des parties des rapports d'analyse : choix du contenu des rapports.  
    static std::uint16_t RefreshFlag;  ///< Drapeau indiquant si si l'interface a été actualisée ou doit l'être
    QListWidget *optionWidget;         ///< composant du dialogue d'options
    QDialogButtonBox *closeButton;     ///< Bouton "Quitter"    
    
    /// Efface les données de  Hash::wrapper et de  Hash::Reference
    
    void clearOptionData();            

private:

    QStackedWidget *pagesWidget;       ///< composant permettant d'empiler les onglets du dialogue d'options (standardPage,  processPage,  codePage)

    /// Méthode permettant d'ajouter les icônes.
    /// 
    /// Appelle options::createIcons sur une liste prédéterminée d'arguments.
    
    void createIcons();                        
    
    /// Méthode permettant d'ajouter une icône particulière
    ///
    /// \param path  Chemin de l'icône
    /// \param text  Libellé de  l'icône
    
    void createIcon (const char* path, const char* text);   

private slots:
    
    /// Changement d'onglet du dialogue d'options
    
    void changePage(QListWidgetItem *current, QListWidgetItem *previous); 
    
    /// Coche la case "Enchainer les rapports" qui permet de lancer R pour produire des rapports d'analyse à partir de l'interface sans lancer RStudio
    
    void enchainerRapports (int index);                                   
};
#endif // OPTIONS_H
