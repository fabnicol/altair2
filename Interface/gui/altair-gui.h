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
////////////////////////////////////////////////////////////////////////////


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

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public :

  // Membres données

   MainWindow(char*); ///< Constructeur de l'interface graphique
   options* dialog;   ///< dialogue d'options
   QSettings  *settings; ///< paramètres par défaut mémorisés

   enum { MaxRecentFiles = 5 };  ///< Nombre maximum de fichiers récents
   QStringList recentFiles; ///< Liste des fichiers de projets .alt récents

   QTabWidget *bottomTabWidget; ///< Onglet des Messages et Console (normalement en bas de l'interface)
   QTextEdit *consoleDialog; ///< Editeur de l'onglet Console

  // Méthodes

   void updateRecentFileActions(); ///< Mise à jour de la liste des fichiers de projet récents
   QString strippedName(const QString &fullFuleName); ///< Renvoie le nom de fichier en enlevant le chemin de dossier
   void on_clearOutputTextButton_clicked(); ///< Nettoie l'onglet courant Console ou Messages
   QTextEdit* getEditor() {return editor;} ///< Renvoie l'éditeur du fichier de projet .alt

   /// Vérifie si par défaut le projet .alt doit être enregistré à chaque modification de l'état de l'interface
   /// \return Booléen : vrai si la case du dialogue de configuration est cochée, faux sinon
   bool isDefaultSaveProjectChecked() { return defaultSaveProjectBehaviorBox->isChecked(); }

   /// Sauvegarde du projet .alt selon un chemin à spécifier
   void saveProjectAs();

   /// Sauvegarde du projet .alt selon un chemin donné
   /// \param chemin Chemin du projet
   void saveProjectAs(const QString &chemin);

   /// Envoie du texte formaté HTML dans l'onglet Console
   /// Etape de capture de la sortie cout et cerr de l'application en ligne de commande \e lhx
   void feedLHXConsoleWithHtml();

   /// Envoie du texte formaté HTML dans l'onglet Console
   /// Etape de capture de la sortie des flux d'exécution des scripts R
   void feedRConsoleWithHtml();

   /// Limite (si l'option du dialogue de configuration est cochée) le volume de texte en sortie de l'onglet Console
   /// Peut être utile pour les sorties très pathologiques et éviter une saturation mémoire ou un <i>freeze</i>
   /// \return Un index qui devra être inférieur à #MAXIMUM_CONSOLE_OUTPUT
   uint32_t getConsoleCounter() { return consoleCounter; }

   /// Affiche l'interface à la taille standard (réduite)
   void standardDisplay();

  private :
  
  // Membres données

   QHash<QString, QAction*> actionHash; ///< Table de hachage permettant d'enregistrer les actions sur l'éditeur de projets .alt

   bool projectFileStatus;
   uint32_t consoleCounter = 0;
   QFile tempLog;

   Altair *altair;
   QMainWindow *editWidget;

   int height;
   int width;
   QDockWidget* fileTreeViewDockWidget;
   QDockWidget* bottomDockWidget;
   QDockWidget* managerDockWidget;

   QMenu *fileMenu;
   QMenu *processMenu;
   QMenu *editMenu;
   QMenu *optionsMenu;
   QMenu *aboutMenu;

   QToolBar *fileToolBar;
   QToolBar *processToolBar;
   QToolBar *editToolBar;
   QToolBar *optionsToolBar;
   QToolBar *aboutToolBar;

   QAction *fileToolBarAction;
   QAction *processToolBarAction;
   QAction *editToolBarAction;
   QAction *optionsToolBarAction;
   QAction *aboutToolBarAction;

   QList<QAction*> recentFileActions;
   QAction *separatorAction;
   QAction *newAction;
   QAction *exportAction;
   QAction *archiveAction;
   QAction *restoreAction;
   QAction *openAction;
   QAction *saveAsAction;
   QAction *saveAction;
   QAction *closeAction;
   QAction *RAction;
   QAction *configureAction;
   QAction *lhxAction;
   QAction *cleanAction;
   QAction *anonymAction;
   QAction *openBaseDirAction;
   QAction *decodeAction;
   QAction *aboutAction;
   QAction *licenceAction;
   QAction *printBaseAction;
   QAction *optionsAction;
   QAction *exitAction;
   QAction *helpAction;
   QAction *displayAction;
   QAction *displayManagerAction;
   QAction *editProjectAction;
   QAction *displayOutputAction;
   QAction *displayFileTreeViewAction;
   QAction *clearOutputTextAction;

   QList<QAction*>  actionList;

   QDialog *contentsWidget;
   QDialogButtonBox *closeButton;

   FCheckBox *defaultFullScreenLayoutBox,
                        *defaultRAltairActivationBox,
                        *defaultProjectManagerWidgetLayoutBox,
                        *defaultFileManagerWidgetLayoutBox,
                        *defaultMessageLayoutBox,
                        *defaultOutputTextEditBox,
                        *defaultLoadProjectBehaviorBox,
                        *importerAuLancementBox,
                        *defaultFileToolBarBox,
                        *defaultEditToolBarBox,
                        *defaultProcessToolBarBox,
                        *defaultOptionsToolBarBox,
                        *defaultAboutToolBarBox,
                        *defaultSaveProjectBehaviorBox,
                        *defaultMaximumConsoleOutputBox,
                        *defaultQuietBox;

   QList<FCheckBox*> displayWidgetListBox, behaviorWidgetListBox, displayToolBarCBoxListBox, outputListBox;

   QTextEdit *editor=nullptr;
   Highlighter *highlighter;
   QFile projectFile;
   QProcess process;
   MatriculeInput *m = nullptr;

  // Méthodes

   /// Lit un fichier et renvoie la chaîne de caractères correspondante.
   /// \param fileName Chemin du fichier
   bool readFile(const QString &fileName);

   /// Crée les actions correspondant aux entrées des menus et barre d'outils
   void createActions();

   /// Crée les menus
   void createMenus();

   /// Crée les barres d'outils
   void createToolBars();

   std::vector<std::string> extraire_donnees_protegees(const std::string& st);

   /// Nettoie les données de paye \n
   /// \param st Fichier de paye converti en vecteur de caractères non signés
   /// \return vecteur de caractères non signés
   /// \note Essaie de repérer les séquences html qui sont illicites sous libxml2 : &accute; par exemple. \n
   /// Elimine les caractères non imprimables, sauf les voyelles accentuées, et les remplace par une espace.
   const std::vector <unsigned char>  nettoyer_donnees(std::vector <unsigned char>& st);
      

private slots:

 void on_displayFileTreeViewButton_clicked();
 void on_openManagerWidgetButton_clicked();
 void on_displayFileTreeViewButton_clicked(bool);
 void on_openManagerWidgetButton_clicked(bool);
 void on_editProjectButton_clicked();
 void on_optionsButton_clicked();
 void displayFullScreen(bool);
 void toggleFullScreen();
 void configure();
 void configureOptions();
 bool exportProject(QString str="");
 bool archiveProject();
 bool restoreProject(QString str="");
 void resetCounter();
 void on_displayLogButton_clicked();
 void anonymiser();
 void cleanBase();
 void launch_process(const QString&);
 void clean_process(const QString&);
 void on_printBase_clicked();
 void resetTableCheckBox();

signals:
   void exitSignal();

};


class MatriculeInput : public QDialog
{
  private :
    QDialogButtonBox *closeButton;
    FLineEdit *matrLineEdit, *matrLineEdit2, *matrLineEdit3;
    FLineFrame* dossier;
    bool checkInput(FLineEdit* l);
    
  public :
    MatriculeInput(int, int);
    QString matricules = "";
};


#endif

