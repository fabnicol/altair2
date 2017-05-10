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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QProcess>
#include <QDomNode>
#include <QFile>
#include <QtXml>

#include "options.h"
#include "browser.h"
#include "common.h"
#include "highlighter.h"
#include "enums.h"
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
   MainWindow(char*);
   options* dialog;
   QSettings  *settings;

   enum { MaxRecentFiles = 5 };
   QStringList recentFiles;
   void updateRecentFileActions();
   QString strippedName(const QString &fullFuleName);
   void on_clearOutputTextButton_clicked();

   QTabWidget *bottomTabWidget;

   QTextEdit* getEditor() {return editor;}
   bool isDefaultSaveProjectChecked() { return defaultSaveProjectBehaviorBox->isChecked(); }
   void saveProjectAs();
   void saveProjectAs(const QString &newst);
   QTextEdit *consoleDialog;
   void feedLHXConsoleWithHtml();
   void feedRConsoleWithHtml();
   uint32_t getConsoleCounter() { return consoleCounter; }
   void standardDisplay();
   
  private slots:

   void on_displayFileTreeViewButton_clicked();
   void on_openManagerWidgetButton_clicked();
   void on_displayFileTreeViewButton_clicked(bool);
   void on_openManagerWidgetButton_clicked(bool);
   void on_editProjectButton_clicked();
   void on_optionsButton_clicked();
   void showMainWidget();
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

  private :
  

   QHash<QString, QAction*> actionHash;

   bool readFile(const QString &fileName);
   bool projectFileStatus;
   uint32_t consoleCounter = 0;
   QFile tempLog;

   Altair *altair;
   QMainWindow *editWidget;
   void createActions();
   void createMenus();
   void createToolBars();
   void loadFile(const QString &fileName);
   void adjustDisplay(bool);
   
   int height;
   int width;
   
   inline void connectShowActions();
   std::vector<std::string> extraire_donnees_protegees(const std::string& st);
   const std::vector <unsigned char>  nettoyer_donnees(std::vector <unsigned char>& st);
      
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

