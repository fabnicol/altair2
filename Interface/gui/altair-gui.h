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
   QTextEdit *consoleDialog;
   void feedLHXConsoleWithHtml();
   void feedRConsoleWithHtml();
   uint32_t getConsoleCounter() { return consoleCounter; }

  private slots:

   void on_displayFileTreeViewButton_clicked();
   void on_openManagerWidgetButton_clicked();
   void on_displayFileTreeViewButton_clicked(bool);
   void on_openManagerWidgetButton_clicked(bool);
   void on_editProjectButton_clicked();
   void on_optionsButton_clicked();
   void showMainWidget();
   void showMainWidget(bool);
   void configure();
   void configureOptions();
   bool exportProject(QString str="");
   bool archiveProject(QString str="");
   bool restoreProject(QString str="");
   void resetCounter();

  private :
  
   
   void f();
   
   QHash<QString, QAction*> actionHash;

   bool readFile(const QString &fileName);
   bool projectFileStatus;
   uint32_t consoleCounter = 0;


   Altair *altair;
   QMainWindow *editWidget;
   void createActions();
   void createMenus();
   void createToolBars();
   void loadFile(const QString &fileName);
   void adjustDisplay(bool);

   inline void connectShowActions();

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
   QAction *openBaseDirAction;
   QAction *decodeAction;
   QAction *aboutAction;
   QAction *licenceAction;
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

signals:
   void exitSignal();

};


#endif

