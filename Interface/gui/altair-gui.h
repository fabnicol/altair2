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


/// \file altair-gui.h
/// \author Fabrice Nicol
/// \brief Code de la classe MainWindow qui construit l'interface graphique

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QProcess>
#include <QDomNode>
#include <QFile>
#include <QtXml>

#include "options.h"
#include "common.h"
#include "highlighter.h"
#include "gui_enums.h"
#include "altair.h"
#include "matricules.h"

class QAction;
class QDirModel;
class QPushButton;
class QTreeView;

class QProcess;
class QCheckBox;
class QTextEdit;
class QListWidget;
class QTabWidget;
class QDomElement;
class QIODevice;
class QTreeWidget;
class QTreeWidgetItem;
class options;
class Altair;
class common;
class Console;
class MatriculeInput;


/// Classe principale de l'interface graphique utilisateur.
/// Régit une classe agent Altair à laquelle sont déléguées des rôles non stictement liés à l'interfaçage graphique.

class MainWindow : public QMainWindow
{
    Q_OBJECT

public :

    // Membres données

    MainWindow (char*);           ///< Constructeur de l'interface graphique
    options* dialog;              ///< dialogue d'options
    QSettings  *settings;         ///< paramètres par défaut mémorisés.
    QStringList recentFiles;      ///< Liste des fichiers de projet \b .alt récents
    QTextEdit* consoleDialog;     ///< Editeur de l'onglet console.
    int   fontsize; ///<Taille de la fonte

    // Méthodes publiques

    /// Renvoie le nom de fichier en enlevant le chemin de dossier.
    /// \param fullName Chemin complet du fichier.
    /// \return Nom du fichier sans le chemin du dossier.

    QString strippedName (const QString &fullName);

    void on_clearOutputTextButton_clicked();           ///< Nettoie l'onglet courant Console ou Messages

    /// Renvoie l'éditeur du fichier de projet \b .alt

    QTextEdit* getEditor()
    {
        return editor;
    }

    /// Vérifie si par défaut le projet \b .alt doit être enregistré à chaque modification de l'état de l'interface.
    /// \return Booléen \e true si la case du dialogue de configuration est cochée, \e false sinon

    bool isDefaultSaveProjectChecked()
    {
        return defaultSaveProjectBehaviorBox->isChecked();
    }

    /// Sauvegarde du projet \b .alt selon un chemin à spécifier.

    void saveProjectAs();

    /// Sauvegarde du projet \b .alt selon un chemin donné.
    /// \param chemin Chemin du projet

    void saveProjectAs (const QString &chemin);

    /// Envoie du texte formaté HTML dans l'onglet Console.
    /// Etape de capture de la sortie cout et cerr de l'application en ligne de commande \e lhx

    void feedLHXConsoleWithHtml();

    /// Envoie du texte formaté HTML dans l'onglet Console.
    /// Etape de capture de la sortie des flux d'exécution des scripts R

    void feedRConsoleWithHtml();

    /// Limite (si l'option du dialogue de configuration est cochée) le volume de texte en sortie de l'onglet Console.
    /// Peut être utile pour les sorties très pathologiques et éviter une saturation mémoire ou un <i>freeze</i>
    /// \return Un index qui devra être inférieur à #MAXIMUM_CONSOLE_OUTPUT

    uint32_t getConsoleCounter()
    {
        return consoleCounter;
    }

    /// Affiche l'interface à la taille standard (réduite).

    void standardDisplay();

    /// Actualise la liste des fichiers de projet \b .alt récents dans le menu Fichier

    void updateRecentFileActions();

    /// Réinitialise le dialogue d'extraction des matricules
    /// @return Booléen \em true si le dialogue d'extraction était rempli à l'origine, \em false sinon

    bool resetExtractionBulletins() { return m->clear();}

private :

    // Membres données

    /// Maximum de la liste des fichiers récents
    enum { MaxRecentFiles = 5 };

    uint32_t consoleCounter = 0; ///< Compteur de lignes de log sur l'onglet Console
    int height;                  ///< Hauteur de l'interface en pixels
    int width;                   ///< Largeur de l'interface en pixels

    Altair *altair;                      ///< Classe agent de l'interface graphique permettant d'encapsuler sous statut privé les opérations les plus techniques tendant à traiter les fichiers de paye

    QHash<QString, QAction*> actionHash; ///< Table de hachage permettant d'enregistrer les actions sur l'éditeur de projets \b .alt
    QFile tempLog;                       ///< Fichier log.html ouvert par un navigateur pour exporter le log de l'onglet Console
    QMainWindow *editWidget;             ///< Fenêtre contenant l'éditeur de projet.
    QString subDirStr;                   ///< Emplacement de l'archivage etc.
    
    // Les QDockWidget sont les widgets mobiles de l'interface :
    // arborescence de fichiers (à gauche),
    // onglets Messages et Console (en bas),
    // gestionnaire de projet (à droite)
    // Ils peuvent être dissimulés ou déplacés par l'utilisateur

    QDockWidget *fileTreeViewDockWidget;  ///< composant mobile de l'arborescence de fichiers
    QDockWidget *bottomDockWidget;        ///< composant mobile des onglets Messages et Console
    QDockWidget *managerDockWidget;       ///< composant mobile du gestionnaire de projets
    QTabWidget  *bottomTabWidget;         ///< Conteneur des onglets du composant mobile bottomDockWidget;
    // Menus

    QMenu *fileMenu;      ///< Menu fichiers de projet
    QMenu *processMenu;   ///< Menu Exécution des autres modules
    QMenu *editMenu;      ///< Menu Edition (projet, bases)
    QMenu *optionsMenu;   ///< Menu dialogues de configuration et d'options
    QMenu *aboutMenu;     ///< Menu d'aide

    // Barres d'outils

    QToolBar *fileToolBar;        ///< Barre fichiers de projet
    QToolBar *processToolBar;     ///< Barre Exécution des autres modules
    QToolBar *editToolBar;        ///< Barre Edition (projet, bases)
    QToolBar *optionsToolBar;     ///< Barre dialogues de configuration et d'options
    QToolBar *aboutToolBar;       ///< Barre d'aide

    // Actions correspondant aux barres d'outils

    QAction *fileToolBarAction;      ///< Afficher/cacher la barre Fichier (fichiers de rojet)
    QAction *processToolBarAction;   ///< Afficher/cacher la barre Lancer (lancement de \e lhx, de \e RStudio, de l'anonymisation, du nettoyage et de l'ouverture du dossier des bases CSV en sortie
    QAction *editToolBarAction;      ///< Afficher/cacher la barre Editer (Edition du projet et widgets mobiles de l'interface graphique)
    QAction *optionsToolBarAction;   ///< Afficher/cacher la barre Configurer (dialogue des options et de configuration)
    QAction *aboutToolBarAction;     ///< Afficher/cacher la barre Aide (dialogue de l'aide / Au sujet de /Licence)

    // Actions des menus

    QList<QAction*> recentFileActions; ///< Fichiers de projet récents
    QAction *separatorAction;          ///< Insertion d'un séparateur
    QAction *newAction;                ///< Nouveau projet
    QAction *exportAction;             ///< Exportation de l'ensemble des sorties de tous les modules vers un dossier donné
    QAction *archiveAction;            ///< Compression et archivage de l'ensemble des sorties de tous les modules vers un dossier donné
    QAction *restoreAction;            ///< Décompression et désarchivage de l'ensemble des sorties de tous les modules
    QAction *openAction;               ///< Ouvrir un fichier projet \b .alt
    QAction *saveAsAction;             ///< Sauvegarder un fichier projet à un emplacement à déterminer
    QAction *saveAction;               ///< Sauvegarder un fichier projet en place
    QAction *closeAction;              ///< Fermer un fichier projet et réinitialiser l'interface
    QAction *RAction;                  ///< Lancer RStudio
    QAction *configureAction;          ///< Lancer le dialogue de configuration
    QAction *lhxAction;                ///< Lancer l'application noyau en ligne de commande \e lhx
    QAction *cleanAction;              ///< Nettoyer la base de paye contenant des caractères non conformes
    QAction *anonymAction;             ///< Anonymiser la base de paye
    QAction *openBaseDirAction;        ///< Ouvrir le répertoire des bases CSV exportées par l'application noyau \e lhx
    QAction *aboutAction;              ///< Ouvrir le dialogue Au sujet de...
    QAction *licenceAction;            ///< Ouvrir la licence
    QAction *printBaseAction;          ///< Extraire des bulletins de paye
    QAction *optionsAction;            ///< Ouvrir le dialogue des paramètres (options)
    QAction *exitAction;               ///< Fermer l'interface
    QAction *helpAction;               ///< Ouvrir l'aide en ligne
    QAction *displayAction;            ///< Afficher en plein écran/en affichage normal
    QAction *displayManagerAction;     ///< Afficher le gestionnaire de projets
    QAction *editProjectAction;        ///< Editer le projet
    QAction *displayOutputAction;      ///< Afficher l'onglet des messages et de console
    QAction *displayFileTreeViewAction;///< Afficher les répertoires et fichiers xhl/xml
    QAction *clearOutputTextAction;    ///< Nettoyer l'onglet courant des messages ou de console

    QList<QAction*>  actionList;       ///< Liste des actions

    QDialog *contentsWidget;           ///< composant du dialogue des options de configuration
    QDialogButtonBox *closeButton;     ///< Bouton Quitter du dialogue des options

    // Cases à cocher du dialogue de configuration

    FCheckBox    *defaultFullScreenLayoutBox;           ///< Case à cocher du dialogue de configuration : plein écran par défaut
    FCheckBox    *defaultProjectManagerWidgetLayoutBox; ///< Case à cocher du dialogue de configuration : gestionnaire de projets par défaut
    FCheckBox    *defaultFileManagerWidgetLayoutBox;    ///< Case à cocher du dialogue de configuration : gestionnaire de fichiers par défaut
    FCheckBox    *defaultOutputTextEditBox;             ///< Case à cocher du dialogue de configuration : onglets des messages et de console par défaut
    FCheckBox    *defaultLoadProjectBehaviorBox;        ///< Case à cocher du dialogue de configuration : charger le projet au lancement par défaut
    FCheckBox    *importerAuLancementBox;               ///< Case à cocher du dialogue de configuration : importer les données au lancement par défaut
    FCheckBox    *defaultFileToolBarBox;                ///< Case à cocher du dialogue de configuration : barre d'outils des Fichier par défaut
    FCheckBox    *defaultEditToolBarBox;                ///< Case à cocher du dialogue de configuration : barre d'outils Edition par défaut
    FCheckBox    *defaultProcessToolBarBox;             ///< Case à cocher du dialogue de configuration : barre d'outils Lancer par défaut
    FCheckBox    *defaultOptionsToolBarBox;             ///< Case à cocher du dialogue de configuration : barre d'outils Options défaut
    FCheckBox    *defaultAboutToolBarBox;               ///< Case à cocher du dialogue de configuration : barre d'outils Aide défaut
    FCheckBox    *defaultSaveProjectBehaviorBox;        ///< Case à cocher du dialogue de configuration : sauvegarder automatiquement le projet par défaut
    FCheckBox    *defaultMaximumConsoleOutputBox;       ///< Case à cocher du dialogue de configuration : limiter la longueur du log en console par défaut
    FCheckBox    *defaultQuietBox;                      ///< Case à cocher du dialogue de configuration : réduire la verbosité par défaut

    QList<FCheckBox*> displayWidgetListBox, behaviorWidgetListBox, displayToolBarCBoxListBox, outputListBox;

    QTextEdit *editor = nullptr;  ///< Editeur de projet
    Highlighter *highlighter;     ///< Agent colorisateur de la syntaxe XML du projet
    QFile projectFile;            ///< Fichier du projet
    QProcess process;             ///< Processus associé au lancement de l'application-noyau \e lhx ou de \e RStudio
    MatriculeInput *m = nullptr;  ///< Agent sélecteur de latricule pour l'extraction de bulletin particulier

    // Méthodes privées

    /// Crée les actions correspondant aux entrées des menus et barre d'outils.

    void createActions();

    /// Crée les menus.

    void createMenus();

    /// Crée les barres d'outils.

    void createToolBars();

    std::vector<std::string> extraire_donnees_protegees (const std::string& st);

    /// Nettoie les données de paye.
    /// \param st Fichier de paye converti en vecteur de caractères non signés.
    /// \return vecteur de caractères non signés.
    /// \note Essaie de repérer les séquences html qui sont illicites sous libxml2 : \code &accute; \endcode par exemple. \n
    /// Elimine les caractères non imprimables, sauf les voyelles accentuées, et les remplace par une espace.

    const std::vector <unsigned char>  nettoyer_donnees (std::vector <unsigned char>& st);

private slots:

    /// Afficher / Masquer l'arborescence de fichiers

    void on_displayFileTreeViewButton_clicked();

    /// Afficher / Masquer l'arborescence de fichiers.
    /// \param val Si true, afficher, sinon masquer

    void on_displayFileTreeViewButton_clicked (bool val);

    /// Afficher / Masque le gestionnaire de projets

    void on_openManagerWidgetButton_clicked();

    /// Afficher / Masquer le gestionnaire de projets.
    /// \param val Si true, afficher, sinon masquer

    void on_openManagerWidgetButton_clicked (bool val);

    /// Afficher / Masque l'éditeur de projets.

    void on_editProjectButton_clicked();

    /// Afficher le dialogue d'options / des paramètres

    void on_optionsButton_clicked();

    /// Afficher en plein écran ou en format standard
    /// \param plein Si vaut \e true alors plein écran sinon standard

    void displayFullScreen (bool plein);

    /// Basculer de plein écran à écran standard et réciproquement

    void toggleFullScreen();

    /// Afficher le dialogue de configuration de l'interface graphique

    void configure();

    /// Créer le dialogue de configuration de l'interface graphique

    void configureOptions();

    /// Enregistrer l'ensemble des données d'output (bases CSV, rapports...) dans un répertoire
    /// \param str Si str est vide, appeler un dialogue de sélection de répertoire, sinon exporter dans le répertoire en argument
    /// \result Booléen : \e true si réussite de l'ensemble des opérations, \e false sinon.

    bool exportProject (QString str = "");

    /// Archiver le projet courant
    /// \result Booléen : \e true si réussite de l'ensemble des opérations, \e false sinon.

    bool archiveProject();

    /// Désarchiver le projet
    /// \param str Si str n'est pas spécifié, appelle un dialogue d'explorateur de fichiers\n
    /// Sinon, désarchive le projet dont le chemin est indiqué en argument
    /// \result Booléen : \e true si réussite de l'ensemble des opérations, \e false sinon.

    bool restoreProject (QString str = "");

    /// Réinitialise  consoleCounter à zéro.

    void resetCounter();

    /// Afficher le log dans le navigateur internet par défaut (si show vaut \e true)
    /// \param show Si true, lance le navigateur pour afficher
    /// \param absolutte_path Si true, remplace des chemins relatifs par le chemin absolu défini sous \link generateDataDirPath

    void on_displayLogButton_clicked(bool show = true, bool absolute_path = true);

    /// Lancer l'anonymisation des bases de paye

    void anonymiser();

    /// Lancer le nettoyage des bases de paye

    void cleanBase();

    /// Lancer l'anonymisation d'un fichier de paye
    /// \param st Chemin du fichier de paye

    void launch_process (const QString& st);

    /// Lancer le nettoyage d'un fichier de paye XML
    /// \param st Chemin du fichier de paye

    void clean_process (const QString& st);

    /// Désactive l'export des tables lors de l'extraction de bulletins de paye

    void on_printBase_clicked();

    /// Réinitialise à \e true l'exportation des bases CSV en fin d'exécution d'un processus d'extraction

    void resetTableCheckBox();

    /// Nettoyage des répertoires temporaires avant empaquetage de l'archive
    
    void tarFinished();
    
   
    
signals:
    /// Fermer l'interface

    void exitSignal();
};

#endif // MAINWINDOW_H

