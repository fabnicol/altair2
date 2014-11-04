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
   QAction *playInSpectrumAnalyzerAction ;
   QAction *playAction ;
   long long cdRecordProcessedOutput;
   //void contextMenuEvent(QContextMenuEvent *event);

   enum { MaxRecentFiles = 5 };
   QStringList recentFiles;
   void updateRecentFileActions();
   QString strippedName(const QString &fullFuleName);
   void on_clearOutputTextButton_clicked();
   FCheckBox *defaultSaveProjectBehavior;
   QTabWidget *bottomTabWidget;
   QTextEdit *consoleDialog;
   
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


  private :
  
   
   void f();
   
   QHash<QString, QAction*> actionHash;
   void feedConsole();
   bool readFile(const QString &fileName);
   Altair *altair;
   QMainWindow *editWidget;
   //QTimer *timer;
   void createActions();
   void createMenus();
   void createToolBars();
   void createFontDataBase();
   void loadFile(const QString &fileName);
   void saveProjectAs(QFile* file);
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
   
   QAction *recentFileActions[MaxRecentFiles];
   QAction *separatorAction;
   QAction *openAction;
   QAction *saveAsAction;
   QAction *saveAction;
   QAction *closeAction;
   QAction *RAction;
   QAction *configureAction;
   QAction *lhxAction;
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

   FCheckBox *defaultFullScreenLayout,
                        *defaultRAltairActivation,
                        *defaultProjectManagerWidgetLayoutBox,
                        *defaultFileManagerWidgetLayoutBox,
                        *defaultMessageLayoutBox,
                        *defaultOutputTextEditBox,
                        *defaultLoadProjectBehavior,
                        *defaultFileToolBarBox,
                        *defaultEditToolBarBox,
                        *defaultProcessToolBarBox,
                        *defaultOptionsToolBarBox,
                        *defaultAboutToolBarBox;


   QList<FCheckBox*> displayWidgetList, behaviorWidgetList, displayToolBarCBoxList;
   
   QTextEdit *editor;
   Highlighter *highlighter;
   void feedConsoleWithHtml();

signals:
   void switch_to_progress_2();

};


#endif

