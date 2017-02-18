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
   void on_nppButton_clicked();
   void anonymiser();
   void cleanBase();
   void launch_process(const QString&);
   void clean_process(const QString&);
   void on_printBase_clicked();

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
   std::string nettoyer_donnees(const std::string& st);
      
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

signals:
   void exitSignal();
   

};


class MatriculeInput : public QDialog
{
  private :
    QDialogButtonBox *closeButton;
     FLineEdit*  matrLineEdit, *matrLineEdit2, *matrLineEdit3;
     FLineFrame* dossier;

  public :
    MatriculeInput(int, int);
    QString matricules;


};


#endif

