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




/// Classe de l'onglet Codes permettant d'exporter des codes de paye manuellement spécifiés vers les scripts R.

class codePage :  public FDialogConnections, public common
{
    Q_OBJECT

public :
    /// Constructeur de l'onglet "Code de paye des tests"

    codePage();

private:
    int nbVar; ///< Nombre de champs de codes de l'onglet (hors FPH)
    QList<FLineEdit*> listeCodes; ///< Liste des pointeurs vers des composants fonctionnels de classe FLineEdit, qui rassemble l'ensemble des lignes correspondant à variables.
    QGridLayout *vLayout = new QGridLayout;    ///< Disposition secondaire (codes).
    QGridLayout *v2Layout = new QGridLayout;    ///< Disposition secondaire (messages d'importation).
    QVBoxLayout *mainLayout = new QVBoxLayout; ///< Disposition principale.
    QGroupBox *baseBox = new QGroupBox;                        ///< Boite regroupant les codes.
    QGroupBox *importBox = new QGroupBox;                      ///< Boite regroupant les messages et dialogues d'importation des bases CSV.

    /// Crée une ligne de codes pour un type donné d'éléments de paye
    /// \param nom Nom de l'élément de paye
    /// \return Nombre d'éléments de paye empilés

    void ajouterVariable (const QString& nom);

protected : 
   
    QStringList variables;        ///< Libellés des éléments de paye faisant l'objet d'une ligne dans l'onglet.
    QList<QString> listeLabels;   ///< Elements de variables dont les espaces ont été retirées.
    QList<QLabel*> listeDialogueLabels;        ///< Mise en forme des éléments de listeLabels au format QLabel.
    QLabel *label = new QLabel;                             ///< Elément de texte variable servant à afficher des messaes d'erreur ou de réussite de l'exportation des codes.
    FLineFrame *codesFrame;                    ///< Elément de texte et chemin du fichier pour l'importation du fichier de codes de paye
    FLineFrame *codesLibellesFrame;            ///< Elément de texte et chemin de la table Codes-Libellés générée par le logiciel dans l'annexe
    QString init_label_text ;                  ///< Message "Appuyer pour exporter..."
    QToolButton* appliquerCodes ;              ///< Bouton "Exporter" (flèche verte) .

public slots:
    /// Substitue certaines valeurs caractéristiques du contrôle dans le fichier \em prologue_codes.R dans common::exportpath, notamment le Siret, le budget et l'employeur

    void substituer_valeurs_dans_script_R();

    /// Réinitialise les champs de code et les chemins, surcharge

    void reinit(int );

    /// Réinitialise les champs de code et les chemins
    ///
    void reinit(flags::update value) { reinit(static_cast<int>(value)); }

private slots:

    /// Active les contrôles réglementaires sur la fonction publique hospitalière (FPH) même s'ils n'ont pas été activés par reconnaissnce automatique.
    
    void activer_fph (bool);

    /// Importation du fichier CSV des codes de paye
    /// \param chemin chemin du fichier
    /// \return true en cas de réussite, false sinon.

    bool importCodesCSV(const QString& chemin);

};

/// Classe de l'onglet Traitement du dialgue d'options
/// 
/// Cette classe précise les modalités d'exécution de l'application-noyau \em lhx (nombre de fils d'exécution, consommation mémoire, log, mode d'exécution cumulatif, distributif ou standard, etc.;) et le type de rapports d'analyse produits (version avancée ou standard).

class processPage :  public FDialogConnections, public common
{
    Q_OBJECT

public :
    processPage();                 ///< Constructeur de l'onglet Traitement.
    FLineFrame *logFrame;          ///< composant fonctionnelde type QLineEdit augmenté de fonctionnalités spéciales. Enregistre le chemin du log d'exécution.
    FCheckBox  *enchainerRapports; ///< Cas à cocher indiquant si la génération des rapports est automatiquement lancée par l'interface.
    FCheckBox *openCheckBox,         ///< Case à cocher permettant d'ouvrir les rapports à la fin de l'exécution (cochée par défaut).
              *consoleCheckBox;      ///< Case à cocher permettant d'activer la console (cochée par défaut).

    /// Réimplémentation de la fonction virtuelle de la classe common.
    /// Exporte les valeurs de parallelCheckBox et openCheckBox  dans prologue_init.R

   void substituer_valeurs_dans_script_R();

private:
    FComboBox
        *processTypeWidget,    ///< Sélectionne le nombre de fils d'exécution.
        *memoryUseWidget,      ///< Sélectionne le pourcentage d'utilisation de la RAM disponible.
        *rapportTypeWidget;    ///< Sélectionne le type de rapport en sortie (PDF, DOCX et ODT ou les trois formats, plus HTML).

    QGroupBox
        *processTypeBox;       ///< Groupe "Mode d'exécution"

    FCheckBox
#ifndef Q_OS_WIN
        *parallelCheckBox,     ///< Case à cocher permettant de produire les rapports en parallèle (cochée par défaut).
#endif
    *logCheckBox;          ///< Case à cocher permettant d'activer/de désactiver logFrame.

    QString    file_str; ///< Contenu du fichier prologue.codes.R
    const QString prologue_options_path = common::path_access (SCRIPT_DIR "prologue_codes.R");
    

};


/// Classe de l'onglet "Format des bases" du dialogue d'options

class standardPage :  public FDialogConnections, public common
{
    Q_OBJECT

public :

    /// Constructeur de l'onglet "Format des bases"

    standardPage();

    FCheckBox
      *tableCheckBox,         ///< Exporter la table ou pas
      *FPHCheckBox;           ///< Données Fonction publique hospitalière ou pas
    FComboBox* exportWidget;  ///< Modalités d'exportation (standard, cumulative, distributive etc.)

     /// Réimplémentation de la fonction virtuelle de la classe common. Renvoie à substituer_versant.

    void substituer_valeurs_dans_script_R()
    {
        substituer_versant();
    }

private:
    FComboBox  *baseWidthWidget;   ///< Largeur de la base (nombre de colonnes) : "Standard", "Etendu", "Maximal"

    FComboBox  *baseTypeWidget;   ///< Format de base en sortie : "Standard", "Par année", "Par agent", "Toutes catégories", "Traitement" etc.

    FLineEdit *maxNLigneLineEdit; ///< Nombre maximal de lignes de la base

    QGroupBox  *baseTypeBox;      ///< Boîte de présentation Type de base

    FCheckBox
      *etabCheckBox,              ///< Exporter l'établissement ou pas
      *rangCheckBox,              ///< Numéroter les lignes ou pas
      *bomCheckBox;               ///< Ne pas forcer la lecture en encodage UTF-8 sous tableur en n'insérant pas de BOM

    QLabel *maxNLigneLabel;       ///< Titre de  maxNLigneLineEdit

private slots:
    void substituer_versant();    ///< Exporter le versant FPH ou FPT dans le script R pour générer les tests adéquats

};



class rapportPage :  public  FDialogConnections, public common
{
    Q_OBJECT

public :
    /// Constructeur de l'onglet "Code de paye des tests"

    rapportPage();

    
private:
    QList<FCheckBox*> listeCB; ///< Liste des pointeurs vers des composants fonctionnels de classe FCheckBox, qui rassemble l'ensemble des cases à cocher pour sélectionner les parties du rapport.
    QGridLayout *vLayout = new QGridLayout;    ///< Disposition secondaire (parties du rapport).

    QVBoxLayout *mainLayout = new QVBoxLayout; ///< Disposition principale.
    QGroupBox *baseBox;                        ///< Boite regroupant les codes.    
    /// Crée une ligne de codes pour un type donné d'éléments de paye
    /// \param nom Nom de l'élément de paye
    /// \return Nombre d'éléments de paye empilés
    /// 
    
    FComboBox *profils; ///< Sélectionne les profils de niveau de détail souhaité pour le rapport.

    QStringList variables;        ///< Libellés des éléments de paye faisant l'objet d'une ligne dans l'onglet.
    QLabel *label;                             ///< Elément de texte variable servant à afficher des messaes d'erreur ou de réussite de l'exportation des codes.
    QString init_label_text ;                  ///< Message "Appuyer pour exporter..."
    const QString prologue_options_path = path_access(SCRIPT_DIR "prologue_codes.R");
    QToolButton* appliquerCodes ;              ///< Bouton "Exporter" (flèche verte) .
    
    QString file_str; ///< Lecture dans QString du contenu du fichier prologue.codes.R
    
    QString liste_cb; ///< Message récapitulatif des parties du rapport générées.
    
    /// Crée une ligne de codes pour un type donné d'éléments de paye
    /// \param nom Nom de l'élément de paye
    /// \return Nombre d'éléments de paye empilés

    int ajouterVariable (const QString& nom);
    
    void message(int r, QIcon& icon); ///< Affichage de la liste des parties du rapport qui seront générées.
    

public slots:
    
    /// Substitue certaines valeurs caractéristiques du contrôle dans le fichier \em prologue_codes.R dans common::exportpath, notamment le Siret, le budget et l'employeur
    
    void substituer_valeurs_dans_script_R();

private slots:

    /// Active/désactive certaines cases liées à d'autres. 
    /// \param s Libellé (#special_tag) de la case dans \link listeCB

    void ajusterDependances(const QString& s, bool checked);
    
};

/// Classe de l'onglet Extra permettant d'importer les fichiers auxiliaires optionnels (correspondance budgétaire, fichier des catégories statutaires rectifié, concessions de logement, plafonds IFSE)     

class extraPage: public FDialogConnections, public common
{

Q_OBJECT

public:
    /// Constructeur de la classe extraPage
    
    extraPage();
    
    FLineFrame *budgetFrame, ///< Groupe ligne de chemin + navigateur de fichiers pour la correspondance entre codes, libellés de paye et sous-comptes du 64 (données externes à importer)
               *gradesFrame; ///< Groupe ligne de chemin + navigateur de fichiers pour la correspondance entre grades et catégorie statutaire (données externes à importer)

#ifdef DEBUG
    FLineFrame *logtFrame,   ///< Groupe ligne de chemin + navigateur de fichiers pour la correspondance entre matricule et concession de logement, par année et mois (données externes à importer)
               *ifseFrame;   ///< Groupe ligne de chemin + navigateur de fichiers pour la correspondance entre grade, groupe fonctionnel et plafond en euros pour l'IFSE (RIFSEEP)
#endif

    FCheckBox *budgetCheckBox;  ///< Case à cocher de contrôle de \link budgetFrame
     
    void do_copies();        ///< Réalise les copies des fichiers auxiliaires paye_budget.csv, grades.catégories.csv et logements.csv dans scripts/R pour importation par le script R.  
    void substituer_valeurs_dans_script_R();  ///< Ajuste la valeur du paramètre paye.budget.existe dans le fichier prologue_codes.R
private:   
    
    FCheckBox
              *gradesCheckBox;  ///< Case à cocher de contrôle de \link gradesFrame

#ifdef DEBUG
              *logtCheckBox,    ///< Case à cocher de contrôle de \link logtFrame
              *ifseCheckBox;    ///< Case à cocher de contrôle de \link ifsetFrame
#endif
};


/// Classe de l'onglet Codes, qui permet de saisir manuellement les codes de paye de certaines catégories de lignes de paye.
/// 
/// Les codes différents, pour la même ligne, sont séparés par des points-virgules. Ils doivent être exportés dans le fichier common::exportpath en cliquant sur la flèche verte de l'onglet.

class options :   public QDialog, public common
{
    Q_OBJECT

public:

    options (Altair* parent = nullptr);///< Constructeur de l'onglet correspondant à un pointeur \em parent vers une instance de la classe Altair
    standardPage* standardTab;         ///< Onglet d'accueil Format : type de base en sortie, modalité d'extraction des données, exportation/archivage
    processPage* processTab;           ///< Onglet de Traitement : nombre de fils, utilisation de la mémoire, log et enchainement avec la production des rapports
    codePage* codeTab;                 ///< Onglet des codes de paye : renseignement manuel des codes de paye utilisés pour certains types d'éléments de paye
    extraPage* extraTab;               ///< Onglet des fichiers auxiliaires : importation de fichiers externes optionnels permetant de préciser/corriger la génération des rapports d'analyse.  
    rapportPage* rapportTab;           ///< Onglet des parties des rapports d'analyse : choix du contenu des rapports.  
    static std::uint16_t RefreshFlag;  ///< Drapeau indiquant si si l'interface a été actualisée ou doit l'être
    QListWidget *optionWidget;         ///< composant du dialogue d'options
    QDialogButtonBox *closeButton;     ///< Bouton "Quitter"    
    QList<FDialogConnections*> tabs;   ///< Liste des onglets

    /// Efface les données de  Hash::wrapper et de  Hash::Reference
    
    void clearOptionData();            

private:

    QString lhx_name = "lhx";                  ///< Nom de l'exécutable lhx (lhx, lhx-ext ou lhx-max)

    QStackedWidget *pagesWidget;       ///< composant permettant d'empiler les onglets du dialogue d'options (standardPage,  processPage,  codePage)

    /// Méthode permettant d'ajouter les icônes.
    /// 
    /// Appelle options::createIcons sur une liste prédéterminée d'arguments.
    
    void createIcons();                        
    
    /// Méthode permettant d'ajouter une icône particulière
    ///
    /// \param path  Chemin de l'icône
    /// \param text  Libellé de  l'icône
    
    void createIcon (const char* path);

private slots:
    
    /// Changement d'onglet du dialogue d'options
    
    void changePage(QListWidgetItem *current, QListWidgetItem *previous); 
    
    /// Coche la case "Enchainer les rapports" qui permet de lancer R pour produire des rapports d'analyse à partir de l'interface
    
    void enchainerRapports (int index);
    void ne_pas_ouvrir_documents();
};
#endif // OPTIONS_H
