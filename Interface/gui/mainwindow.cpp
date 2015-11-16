
#include "altair.h"
#include "enums.h"

// Should it slow down application launch on some platform, one option could be to launch it just once then on user demand


MainWindow::MainWindow(char* projectName)
{
  #ifdef MINIMAL
    setGeometry(QRect(200, 200,600,400));
  #else
    setGeometry(QRect(200, 200,1150,700));
  #endif

  raise();
  recentFiles = QStringList() ;

  altair=new Altair;
  altair->parent=this;
  altair->projectName=projectName;

  createActions();
  createMenus();

  settings = new QSettings("altair", "Free Software Inc");

  if ((settings->value("defaut").isValid())
                &&
     (!settings->value("defaut").toString().isEmpty()))
        altair->setCurrentFile(settings->value("defaut").toString());
  else
    {
        altair->setCurrentFile(projectName);
        settings->setValue("defaut",projectName);
    }

  setCentralWidget(altair);

  altair->addActions(actionList);

  bottomDockWidget=new QDockWidget;
  bottomTabWidget=new QTabWidget;
  consoleDialog=  new QTextEdit;
  consoleDialog->setReadOnly(true);

  bottomTabWidget->addTab(altair->outputTextEdit, tr("Messages"));
  bottomTabWidget->addTab(consoleDialog, tr("Console"));
  bottomTabWidget->setCurrentIndex(0);

  fileTreeViewDockWidget= new QDockWidget;
  fileTreeViewDockWidget->setWidget(altair->fileTreeView);
  fileTreeViewDockWidget->setMinimumHeight((unsigned) (height()*0.3));
  fileTreeViewDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
  fileTreeViewDockWidget->hide();
  addDockWidget(Qt::LeftDockWidgetArea, fileTreeViewDockWidget);

  managerDockWidget= new QDockWidget;
  managerDockWidget->setWidget(altair->managerWidget);
  managerDockWidget->setMinimumHeight((unsigned) (height()*0.3));
  managerDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::RightDockWidgetArea, managerDockWidget);
  
  Abstract::initializeFStringListHashes();
  Abstract::refreshOptionFields();

  configureOptions();
  createToolBars();

  bottomTabWidget->setCurrentIndex(0);

  QToolButton *clearBottomTabWidgetButton=new QToolButton;
  const QIcon clearOutputText = QIcon(QString::fromUtf8( ":/images/edit-clear.png"));
  clearBottomTabWidgetButton->setIcon(clearOutputText);

  QGroupBox *stackedBottomWidget=new QGroupBox;
  QHBoxLayout *stackedBottomWidgetLayout=new QHBoxLayout;
  stackedBottomWidgetLayout->addWidget(clearBottomTabWidgetButton);
  stackedBottomWidgetLayout->addWidget(bottomTabWidget);
  stackedBottomWidget->setLayout(stackedBottomWidgetLayout);
#ifndef MINIMAL
  bottomDockWidget->setMinimumHeight(400);
 #endif
  bottomDockWidget->setWidget(stackedBottomWidget);

  addDockWidget(Qt::BottomDockWidgetArea, bottomDockWidget);

  setWindowIcon(QIcon(":/images/altair.png"));
  setWindowTitle("Interface  Altaïr"+ QString(VERSION));

  dialog=new options(altair);
  dialog->setParent(altair, Qt::Window);

  Altair::RefreshFlag =  Altair::RefreshFlag  | interfaceStatus::parseXml;

  if (settings->value(defaultLoadProjectBehaviorBox->getHashKey()) == true)
      projectFileStatus = altair->clearInterfaceAndParseProject();

  adjustDisplay(projectFileStatus);

  // resetting interfaceStatus::parseXml bits to 0
  Altair::RefreshFlag = Altair::RefreshFlag & (~interfaceStatus::parseXml);

  connect(clearBottomTabWidgetButton, &QToolButton::clicked, [this] { on_clearOutputTextButton_clicked();});
  connect(consoleDialog, SIGNAL(copyAvailable(bool)), consoleDialog, SLOT(copy()));
  connect(&(altair->process), SIGNAL(finished(int)), this, SLOT(resetCounter()));

}

void MainWindow::on_clearOutputTextButton_clicked()
{
    qobject_cast<QTextEdit*>(bottomTabWidget->currentWidget())->clear();
}


void MainWindow::updateRecentFileActions()
{
QMutableStringListIterator i(recentFiles);

 while (i.hasNext())
 {
   if (!QFile::exists(i.next())) i.remove();
 }


 for (int j  =0 ; j < MaxRecentFiles ; ++j)
 {
   if (j < recentFiles.count())
   {
     QString  text = tr("&%1 %2").arg(j+1).arg(strippedName(recentFiles[j]));

     recentFileActions[j]->setText(text);
     recentFileActions[j]->setData(QVariant(recentFiles[j]));
     recentFileActions[j]->setVisible(true);
   } else

   {
     recentFileActions[j]->setVisible(false);
   }

 }

 separatorAction->setVisible(!recentFiles.isEmpty());
}



QString MainWindow::strippedName(const QString &fullFileName)
{
  return QFileInfo(fullFileName).fileName();
}

void MainWindow::createMenus()
{
 fileMenu = menuBar()->addMenu("&Fichier");
 editMenu = menuBar()->addMenu("&Editer");
 processMenu = menuBar()->addMenu("&Lancer");
 optionsMenu = menuBar()->addMenu("&Configurer");
 aboutMenu = menuBar()->addMenu("&Aide");

 fileMenu->addActions({newAction, openAction, saveAction,
                       saveAsAction, exportAction, archiveAction,
                       restoreAction, closeAction});
 separatorAction=fileMenu->addSeparator();
 fileMenu->addActions(recentFileActions);
 fileMenu->addSeparator();
 fileMenu->addAction(exitAction);

 editMenu->addActions({displayAction, displayOutputAction, displayFileTreeViewAction,
                       displayManagerAction, clearOutputTextAction, editProjectAction});

 processMenu->addActions({RAction, lhxAction, openBaseDirAction});

 optionsMenu->addActions({optionsAction, configureAction});

 aboutMenu->addActions({helpAction, aboutAction,licenceAction});
}

void MainWindow::f()
{
    altair->updateProject(true);
}

void MainWindow::createActions()
{

  newAction  = new QAction(tr("Nouveau projet .alt"), this);
  openAction = new QAction(tr("&Ouvrir le projet .alt"), this);
  newAction ->setShortcut(QKeySequence("Ctrl+N"));
  openAction->setShortcut(QKeySequence("Ctrl+O"));
  newAction->setIcon(QIcon(":/images/project-open.png"));
  openAction->setIcon(QIcon(":/images/document-open-folder.png"));

  connect(newAction, SIGNAL(triggered()), altair, SLOT(on_newProjectButton_clicked()));
  connect(openAction, SIGNAL(triggered()), altair, SLOT(on_openProjectButton_clicked()));

  saveAction = new QAction(tr("&Enregistrer"), this);
  saveAction->setShortcut(QKeySequence("Ctrl+S"));
  saveAction->setIcon(QIcon(":/images/document-save.png"));

  connect(saveAction, &QAction::triggered, this, &MainWindow::f);
  
  saveAsAction = new QAction(tr("En&registrer le projet comme..."), this);
  saveAsAction->setIcon(QIcon(":/images/document-save-as.png"));
  connect(saveAsAction, SIGNAL(triggered()), altair, SLOT(requestSaveProject()));

  exportAction = new QAction(tr("E&xporter le rapport vers..."), this);
  exportAction->setIcon(QIcon(":/images/export.png"));
  connect(exportAction, SIGNAL(triggered()), this, SLOT(exportProject()));

  archiveAction = new QAction(tr("Archiver le rapport vers..."), this);
  archiveAction->setIcon(QIcon(":/images/archive.png"));
  connect(archiveAction, SIGNAL(triggered()), this, SLOT(archiveProject()));

  restoreAction = new QAction(tr("Désarchiver le rapport"), this);
  restoreAction->setIcon(QIcon(":/images/restore.png"));
  connect(restoreAction, SIGNAL(triggered()), this, SLOT(restoreProject()));

  closeAction = new QAction(tr("&Fermer le projet .alt"), this);
  closeAction->setShortcut(QKeySequence("Ctrl+W"));
  closeAction->setIcon(QIcon(":/images/document-close.png"));
  connect(closeAction, SIGNAL(triggered()), altair, SLOT(closeProject()));

  RAction = new QAction(tr("&Lancer l'analyse des données"), this);
  RAction->setShortcut(QKeySequence("Ctrl+R"));
  RAction->setIcon(QIcon(":/images/altair.png"));
  connect(RAction, SIGNAL(triggered()), altair, SLOT(runRAltair()));

  lhxAction = new QAction(tr("Créer la base de données .csv"), this);
  lhxAction->setShortcut(QKeySequence("Ctrl+B"));
  lhxAction->setIcon(QIcon(":/images/csv.png"));
  connect(lhxAction, SIGNAL(triggered()), altair, SLOT(run()));

  openBaseDirAction = new QAction(tr("Ouvrir le répertoire des bases"), this);
  openBaseDirAction ->setIcon(QIcon(":/images/directory.png"));
  connect(openBaseDirAction, &QAction::triggered, [&] { common::openDir(dialog->standardTab->donneesCSV->getText());  });

  optionsAction = new QAction(tr("&Options"), this);
  optionsAction->setShortcut(QKeySequence("Ctrl+P"));
  optionsAction->setIcon(QIcon(":/images/encode.png"));
  connect(optionsAction, SIGNAL(triggered()), this, SLOT(on_optionsButton_clicked()));

  configureAction= new QAction(tr("&Configurer l'interface"), this);
  configureAction->setIcon(QIcon(":/images/configure-toolbars.png"));
  connect(configureAction, SIGNAL(triggered()), this, SLOT(configure()));

  helpAction = new QAction(tr("&Aide"), this);
  helpAction->setShortcut(QKeySequence("Ctrl+H"));
  helpAction->setIcon(QIcon(":/images/help-contents.png"));
  connect(helpAction, SIGNAL(triggered()), altair, SLOT(on_helpButton_clicked()));

  displayAction = new QAction(tr("&Plein écran/Réduire"), this);
  displayAction->setIcon(QIcon(":/images/show-maximized.png"));
  connect(displayAction, SIGNAL(triggered()), this, SLOT(showMainWidget()));

  displayManagerAction = new QAction(tr("Ouvrir/Fermer le &gestionnaire de projet"), this);
  const QIcon iconViewList = QIcon(QString::fromUtf8( ":/images/manager.png"));
  displayManagerAction->setIcon(iconViewList);
  connect(displayManagerAction, SIGNAL(triggered()), this, SLOT(on_openManagerWidgetButton_clicked()));

  editProjectAction=new QAction(tr("Editer le projet courant"), this);
  editProjectAction->setShortcut(QKeySequence("Ctrl+E"));
  editProjectAction->setIcon(QIcon(":/images/application-xml.png"));
  connect(editProjectAction, SIGNAL(triggered()), this, SLOT(on_editProjectButton_clicked()));

  displayOutputAction  = new QAction(tr("Ouvrir/Fermer les messages"), this);
  const QIcon displayOutput = QIcon(QString::fromUtf8( ":/images/display-output.png"));
  displayOutputAction->setIcon(displayOutput);
  connect(displayOutputAction, &QAction::triggered,  [this] {bottomDockWidget->setVisible(!bottomDockWidget->isVisible());});

  displayFileTreeViewAction  = new QAction(tr("Ouvrir/Fermer le gestionnaire de fichiers"), this);
  const QIcon displayFileTreeView = QIcon(QString::fromUtf8( ":/images/view-list-tree.png"));
  displayFileTreeViewAction->setIcon(displayFileTreeView);
  connect(displayFileTreeViewAction, SIGNAL(triggered()), this, SLOT(on_displayFileTreeViewButton_clicked()));
  
  clearOutputTextAction = new QAction(tr("Effacer le log"), this);
  const QIcon clearOutputText = QIcon(QString::fromUtf8( ":/images/edit-clear.png"));
  clearOutputTextAction->setIcon(clearOutputText);
  connect(clearOutputTextAction, &QAction::triggered,  [this] {on_clearOutputTextButton_clicked();});

  exitAction = new QAction(tr("&Quitter"), this);
  exitAction->setIcon(QIcon(":/images/application-exit.png"));
  exitAction->setShortcut(QKeySequence("Ctrl+Q"));
  connect(exitAction, &QAction::triggered,  [this] { emit(exitSignal());});

  aboutAction=new QAction(tr("&Au sujet de"), this);
  aboutAction->setIcon(QIcon(":/images/about.png"));

  connect(aboutAction, &QAction::triggered,  [this]  {
                                                          QUrl url=QUrl::fromLocalFile( QCoreApplication::applicationDirPath() + "/../about.html") ;
                                                          browser::showPage(url);
                                                      });

  licenceAction=new QAction(tr("Licence"), this);
  licenceAction->setIcon(QIcon(":/images/web/gplv3.png"));
  connect(licenceAction, &QAction::triggered,  [this]  {
                                                            QUrl url=QUrl::fromLocalFile( QCoreApplication::applicationDirPath() + "/../licence.html");
                                                            browser::showPage(url);
                                                        });

  for (int i=0; i < MaxRecentFiles ; i++)
  {
    recentFileActions << new QAction(this);
    recentFileActions[i]->setVisible(false);
    connect(recentFileActions[i], SIGNAL(triggered()), altair, SLOT(openProjectFile()));
  }

  QAction* separator[6];
  for (int i=0; i < 6; i++)
    {
      separator[i] = new QAction(this) ;
      separator[i]->setSeparator(true);
    }

  actionList << newAction << openAction << saveAction << saveAsAction << exportAction << archiveAction << restoreAction << closeAction << exitAction << separator[0] <<
                RAction << lhxAction << openBaseDirAction << displayOutputAction << displayFileTreeViewAction <<
                displayManagerAction <<  separator[4] <<
                clearOutputTextAction <<  editProjectAction << separator[3] << configureAction <<
                optionsAction << helpAction << aboutAction ;
  
}

void MainWindow::configure()
{
     contentsWidget->setVisible(true);
     contentsWidget->raise();
}

void MainWindow::on_optionsButton_clicked()
{
  dialog->setVisible(!dialog->isVisible());
  dialog->raise();
}

void MainWindow::on_displayFileTreeViewButton_clicked(bool isHidden)
{
   fileTreeViewDockWidget->setVisible(isHidden);
   altair->project[0]->importFromMainTree->setVisible(isHidden);

 }

void MainWindow::on_displayFileTreeViewButton_clicked()
{
    on_displayFileTreeViewButton_clicked(fileTreeViewDockWidget->isHidden());
}


void MainWindow::on_openManagerWidgetButton_clicked(bool isHidden)
{
   managerDockWidget->setVisible(isHidden);
 }

void MainWindow::on_openManagerWidgetButton_clicked()
{
    on_openManagerWidgetButton_clicked(managerDockWidget->isHidden());
}

void MainWindow::createToolBars()
{
 fileToolBar->addActions({newAction, saveAction, saveAsAction, exportAction, archiveAction, restoreAction, closeAction, exitAction});
 fileToolBar->addSeparator();

 editToolBar->addActions({displayAction, displayOutputAction, displayFileTreeViewAction,
                          displayManagerAction, editProjectAction});

 processToolBar->addActions({RAction, lhxAction, openBaseDirAction});

 optionsToolBar->addActions({optionsAction, configureAction});

 aboutToolBar->addActions({helpAction, aboutAction});
}

void MainWindow::on_editProjectButton_clicked()
{

    if (altair->projectName.isEmpty()) return;
    editWidget = new QMainWindow(this);
    editWidget->setWindowTitle(tr("Edition du projet ")+altair->projectName.left(8)+"..."+altair->projectName.right(12));
    QMenu *fileMenu = new QMenu(tr("&Fichier"), this);
    editWidget->menuBar()->addMenu(fileMenu);

     const char* keys[]={"Nouveau", "Ouvrir", "Enregistrer", "Enregistrer comme...", "Actualiser", "Enregistrer et quitter", "Quitter"};
     const char* seq[]={"Ctrl+N","Ctrl+O","Ctrl+S","Ctrl+T","Ctrl+R","Ctrl+E","Ctrl+Q"};
     int j=0;

    for (const char* k:  keys)
    {
        actionHash[k]=new QAction(tr(k), this);
        fileMenu->addAction(actionHash[k]);
        actionHash[k]->setShortcut(QKeySequence(seq[j++]));
    }

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new QTextEdit;
    editor->setFont(font);

    highlighter = new Highlighter(editor->document());

    if (altair->projectName.isEmpty()) return;

   QFile  *file=new QFile(altair->projectName);

   if (file->open(QFile::ReadWrite| QFile::Text))
   {
       editor->setPlainText(file->readAll());
       file->close();
   }
   // do not capture file by reference!
   connect(actionHash["Nouveau"],
                 &QAction::triggered,
                 [this] { altair->on_newProjectButton_clicked(); });

   connect(actionHash["Ouvrir"],
                 &QAction::triggered,
                 [file, this]
                                  {
                                     file->~QFile();
                                     altair->on_openProjectButton_clicked() ;
                                     editWidget->~QMainWindow();
                                     on_editProjectButton_clicked();
                                   });

   connect(actionHash["Enregistrer"],
                 &QAction::triggered,
                 [file, this]
                                 {
                                    file->open(QFile::Truncate |QFile::WriteOnly| QFile::Text);
                                    file->write(editor->document()->toPlainText().toUtf8()) ;
                                    file->close();
                                    Altair::RefreshFlag =  Altair::RefreshFlag
                                                        | interfaceStatus::tree
                                                        | interfaceStatus::parseXml;
                                    // resetting interfaceStatus::parseXml bits to 0

                                    altair->clearInterfaceAndParseProject();
                                    Altair::RefreshFlag = Altair::RefreshFlag & (~interfaceStatus::parseXml);
                                  });

   connect(actionHash["Enregistrer comme..."],
                  &QAction::triggered,
                  [this] {saveProjectAs();});

   connect(actionHash["Actualiser"],
                 &QAction::triggered,
                 [file, this]
                                 {
                                    altair->updateProject(true);
                                    if (file->open(QFile::ReadWrite |  QFile::Text))
                                       {
                                           editor->clear();
                                           editor->setPlainText(file->readAll());
                                           file->close();
                                       }
                                  });

   connect(actionHash["Enregistrer et quitter"],
                  &QAction::triggered,
                 [this]
                         {
                            actionHash["Enregistrer"]->trigger();
                            actionHash["Quitter"]->trigger();
                         });

   connect(actionHash["Quitter"],
                 &QAction::triggered,
                 [file, this]
                                  {
                                     file->~QFile();
                                     editWidget->~QMainWindow() ;
                                   });
   editWidget->setCentralWidget(editor);
   editWidget->setGeometry(200,200,600,800);
   editWidget->show();

}


void MainWindow::saveProjectAs()
{
    QString newstr=QFileDialog::getSaveFileName(this, tr("Enregistrer le projet comme..."), QDir::currentPath(), tr("projet altair (*.alt)"));
    if (newstr.isEmpty())
                return;

    if  (QFileInfo(newstr).isFile())
    {
          QMessageBox::StandardButton result = QMessageBox::warning(nullptr, "Ecraser le fichier ?", "Ce fichier va être écrasé.\nAppuyer sur Oui pour confirmer.",
                                            QMessageBox::Ok | QMessageBox::Cancel);


          if (result != QMessageBox::Ok)
          {
              return;
          }
          else
          {
                 QFile newfile(newstr);
                 newfile.remove();
          }
    }

    QFile* file = new QFile(newstr);

    if (file->open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
    {
       if (editor) file->write(editor->document()->toPlainText().toUtf8()) ;
    }
    file->close();
    Altair::RefreshFlag =  Altair::RefreshFlag
                            | interfaceStatus::tree;

    altair->setCurrentFile(newstr);
    // attention dans cet ordre !
    altair->projectName=newstr;

}


bool MainWindow::exportProject(QString dirStr)
{
    if (dirStr.isEmpty())
        dirStr = QFileDialog::getExistingDirectory(this, tr("Exporter le rapport vers le répertoire..."), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    if (! QFileInfo(dirStr).isDir()) return false;

    QString subDirStr = QDir::toNativeSeparators(dirStr.append("/Altaïr"));

    altair->updateProject();
    QString projectRootDir = QDir::toNativeSeparators(QDir::cleanPath(v(base) + "/../.."));
    QString docxReportFilePath = projectRootDir + "/altair.docx";
    QString pdfReportFilePath = projectRootDir + "/altair.pdf";
    bool result = true;

    result = common::copyFile(docxReportFilePath, subDirStr + "/altaïr.docx", "Le rapport Altaïr Word");

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été exporté sous : " + subDirStr);

    result = common::copyFile(pdfReportFilePath, subDirStr + "/altaïr.pdf", "Le rapport Altaïr PDF");

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été exporté sous : " + subDirStr);

    common::copyDir(projectRootDir + "/Docs", subDirStr + "/Docs");
    result = common::copyDir(projectRootDir + "/Bases", subDirStr + "/Bases");

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Les bases ont été exportées sous : " + subDirStr + QDir::separator() + "Bases");
    return result;
}

bool MainWindow::archiveProject()
{
    QString dirName;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::List);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setWindowTitle("Exporter le rapport vers le répertoire...");
    dialog.setDirectory("c:/Users/Public/Dev/altair/Archives");
    if (dialog.exec())
        dirName = dialog.selectedFiles().at(0);

    if (! QFileInfo(dirName).isDir())
    {
        QDir dir;
        dir.mkpath(dirName);
    }

    QString subDirStr = QDir::toNativeSeparators(dirName.append("/Archives Altaïr/" +
                                                               QDate::currentDate().toString("dd MM yyyy")
                                                               + "-" + QTime::currentTime().toString("hh mm ss")));

    altair->updateProject();
    QString projectRootDir = QDir::toNativeSeparators(QDir::cleanPath(v(base) + "/../.."));
    QString docxReportFilePath = projectRootDir + "/altair.docx";
    QString pdfReportFilePath = projectRootDir + "/altair.pdf";
    bool result = true;

    result = common::zip(docxReportFilePath, subDirStr + "/altaïr.docx.arch");

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été archivé sous : " + subDirStr);

    result = common::zip(pdfReportFilePath, subDirStr + "/altaïr.pdf.arch");

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été archivé sous : " + subDirStr);

    common::zipDir(projectRootDir + "/Docs", subDirStr + "/Docs");
    result = common::zipDir(projectRootDir + "/Bases", subDirStr + "/Bases");

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Les bases ont été archivées sous : " + subDirStr + QDir::separator() + "Bases");

  return result;
}

bool MainWindow::restoreProject(QString subDirStr)
{
    if (subDirStr.isEmpty())
        subDirStr = QFileDialog::getExistingDirectory(this, tr("Restorer le rapport depuis le répertoire..."), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    if (! QFileInfo(subDirStr).isDir()) return false;

    altair->updateProject();
    QString projectRootDir = QDir::toNativeSeparators(QDir::cleanPath(v(base) + "/../.."));
    QString docxReportFilePath = projectRootDir + "/altair.docx";
    QString pdfReportFilePath = projectRootDir + "/altair.pdf";
    bool result = true;

    result = common::unzip(subDirStr + "/altaïr.docx.arch", docxReportFilePath);

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été décompressé sous : " + projectRootDir);

    result = common::unzip(subDirStr + "/altaïr.pdf.arch", pdfReportFilePath);

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été décompressé sous : " + projectRootDir);

    common::unzipDir(subDirStr + "/Docs", projectRootDir + "/Docs");
    result = common::unzipDir(subDirStr + "/Bases", projectRootDir + "/Bases");

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Les bases ont été décompressées sous : " + projectRootDir + QDir::separator() + "Bases");

  return result;
}


void MainWindow::configureOptions()
{
    /* plain old data types must be 0-initialised even though the class instance was new-initialised. */

    contentsWidget = new QDialog(this);
    contentsWidget->setVisible(false);

    QGroupBox *displayGroupBox =new QGroupBox(tr("Affichage"));
    QGroupBox *displayToolBarsGroupBox =new QGroupBox(tr("Barres d'outils"));

    closeButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    closeButton->button(QDialogButtonBox::Ok)->setText("Accepter");
    closeButton->button(QDialogButtonBox::Cancel)->setText("Annuler");    

    defaultFileManagerWidgetLayoutBox=new FCheckBox("Afficher le gestionnaire de fichiers",
                                                    #ifdef MINIMAL
                                                                            flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                    #else
                                                                            flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                                    #endif
                                                                            "fileManagerDisplay",
                                                                           {"Interface", "Afficher le gestionnaire de fichiers"});

    defaultProjectManagerWidgetLayoutBox=new FCheckBox("Afficher le gestionnaire de projet",
                                                   #ifdef MINIMAL
                                                                            flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                   #else
                                                                            flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                                   #endif
                                                                            "projectManagerDisplay",
                                                                            {"Interface", "Afficher le gestionnaire de projet"});

    defaultFullScreenLayoutBox=new FCheckBox("Plein écran",
                                                        flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                        "fullScreenDisplay",
                                                        {"Interface", "Plein écran au lancement"});

    defaultOutputTextEditBox=new FCheckBox("Afficher les messages",
                                       #ifdef MINIMAL
                                                                   flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                       #else
                                                                   flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                       #endif
                                                                   "outputTextEdit",
                                                                   {"Interface", "Afficher les messages"});
    
    defaultFileToolBarBox=new FCheckBox("Afficher la barre d'outils de fichiers",
                                    #ifdef MINIMAL
                                                                flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                    #else
                                                                flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                    #endif
                                                                "fileToolBar",
                                                                {"Interface", "Afficher la barre d'outils de fichiers"});
    
    defaultEditToolBarBox=new FCheckBox("Afficher la barre d'outils d'édition",
                                                                flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                                "editToolBar",
                                                                {"Interface", "Display Edit toolBar"});
    
    defaultProcessToolBarBox = new FCheckBox("Afficher la barre d'outils de processus",
                                       #ifdef MINIMAL
                                                                flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                       #else
                                                                flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                       #endif
                                                                "processToolBar",
                                                                {"Interface", "Afficher la barre d'outils de processus"});
    
    defaultOptionsToolBarBox = new FCheckBox("Afficher la barre d'options",
                                       #ifdef MINIMAL
                                                                flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                       #else
                                                                flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                       #endif
                                                                "optionsToolBar",
                                                                {"Interface", "Afficher les options"});
    
    defaultAboutToolBarBox = new FCheckBox("Afficher la barre d'A propos",
                                                                flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                                "aboutToolBar",
                                                                {"Interface", "Afficher la barre A propos"});
    
    QGroupBox* behaviorGroupBox = new QGroupBox(tr("Sauvegarder/Lancer"));

    defaultSaveProjectBehaviorBox = new FCheckBox("Sauvegarder le projet .alt automatiquement",
                                                                flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                                                "saveProjectBehavior",
                                                                {"Interface", "Sauvegarder le projet .alt automatiquement"});

    defaultLoadProjectBehaviorBox = new FCheckBox("Charger le projet par défaut au lancement",
                                                                              flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                                            "loadProjectBehavior",
                                                                            {"Interface", "Charger le projet .alt au lancement"});


    QGroupBox *outputGroupBox= new QGroupBox(tr("Console"));

    defaultMaximumConsoleOutputBox = new FCheckBox("Limiter la sortie de la console",
                                                                            flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                                            "limitConsoleOutput",
                                                                            {"Interface", "Limiter le nombre de lignes en sortie de la console"});

    defaultQuietBox = new FCheckBox("Limiter la verbosité",
                                       "quiet",
                                       {"Interface", "Limiter la verbosité de la console"},
                                        "q");

    displayWidgetListBox   <<  defaultFileManagerWidgetLayoutBox
                           << defaultProjectManagerWidgetLayoutBox
                           << defaultOutputTextEditBox
                           << defaultFullScreenLayoutBox;


    behaviorWidgetListBox  << defaultSaveProjectBehaviorBox
                           << defaultLoadProjectBehaviorBox;

    
    displayToolBarCBoxListBox  <<  defaultFileToolBarBox
                               <<  defaultEditToolBarBox
                               <<  defaultProcessToolBarBox
                               <<  defaultOptionsToolBarBox
                               <<  defaultAboutToolBarBox;
    
    outputListBox << defaultMaximumConsoleOutputBox
                  << defaultQuietBox;

    QList<QToolBar*> displayToolBarList ;
    
#define buildToolBar(bar, text) \
  bar = addToolBar(tr(text));\
  bar->setIconSize(QSize(48,48)); 
      
    buildToolBar(fileToolBar, "&File") 
    buildToolBar(editToolBar, "&Edit") 
    buildToolBar(processToolBar, "&Process")
    buildToolBar(optionsToolBar, "&Data") 
    buildToolBar(aboutToolBar, "&Help")          
    
    displayToolBarList <<  fileToolBar
                       << editToolBar 
                       << processToolBar
                       << optionsToolBar 
                       << aboutToolBar;                                  

    QGridLayout *layout=new QGridLayout;
    QVBoxLayout *displayDocksLayout=new QVBoxLayout;
    QVBoxLayout *displayToolBarsLayout=new QVBoxLayout;
    QVBoxLayout *behaviourLayout=new QVBoxLayout;
    QVBoxLayout *outputLayout=new QVBoxLayout;


    for (FCheckBox* a : displayWidgetListBox)    displayDocksLayout->addWidget(a);
    for (FCheckBox* a : behaviorWidgetListBox)   behaviourLayout->addWidget(a);
    for (FCheckBox* a : outputListBox)           outputLayout->addWidget(a);

    for (int i=0; i< displayToolBarList.size(); i++)
    {
        displayToolBarsLayout->addWidget(displayToolBarCBoxListBox[i]);

        connect(displayToolBarCBoxListBox[i], SIGNAL(toggled(bool)), displayToolBarList[i], SLOT(setVisible(bool)));

    }

    displayGroupBox->setLayout(displayDocksLayout);
    behaviorGroupBox->setLayout(behaviourLayout);
    displayToolBarsGroupBox->setLayout(displayToolBarsLayout);
    outputGroupBox->setLayout(outputLayout);

    layout->addWidget(displayGroupBox, 0,0);
    layout->addWidget(behaviorGroupBox, 1, 0);
    layout->addWidget(displayToolBarsGroupBox, 0, 1);
    layout->addWidget(outputGroupBox, 1, 1);
    layout->addWidget(closeButton, 2, 0);
    contentsWidget->setLayout(layout);
    
    connect(closeButton, &QDialogButtonBox::accepted,
                        [this]  {

                                    settings->setValue(defaultLoadProjectBehaviorBox->getHashKey(), defaultLoadProjectBehaviorBox->isChecked());
                                
                                    if (    (isDefaultSaveProjectChecked())
                                         || (QMessageBox::Yes == Warning(tr("Sauvegarder le projet"),
                                                                         tr("Le projet n'a pas été sauvegardé.\nAppuyer sur Oui pour le sauvegarder\nou sur Non pour fermer le dialogue sans sauvegarder le projet.")))
                                        )
                                        altair->updateProject();
                                         
                                    contentsWidget->accept();
                                });

    /* note on connection syntax
     * Here the new Qt5 connection syntax should be used with care and disregarded when both an action button and an FCheckBox activate a slot as the slots
     * are overloaded (which could possibly be rewritten) and a) the action button uses the argumentless slot whilst
     * b) the boolean version of slots must be used by the FcheckBox. The new Qt5 syntax cannot work this out as it does not manage overloading. */

    connect(closeButton, &QDialogButtonBox::rejected, contentsWidget, &QDialog::reject);
    
    connect(defaultFileManagerWidgetLayoutBox, SIGNAL(toggled(bool)), this, SLOT(on_displayFileTreeViewButton_clicked(bool)));
    connect(defaultProjectManagerWidgetLayoutBox, SIGNAL(toggled(bool)), this, SLOT(on_openManagerWidgetButton_clicked(bool)));
    
    connect(defaultFullScreenLayoutBox, SIGNAL(toggled(bool)), this, SLOT(showMainWidget(bool)));
    connect(defaultMaximumConsoleOutputBox, &FCheckBox::toggled, [this]{v(limitConsoleOutput).toggle();});
    connect(defaultOutputTextEditBox, &FCheckBox::toggled, [this] {bottomDockWidget->setVisible(defaultOutputTextEditBox->isChecked());});
    connect(defaultLoadProjectBehaviorBox, &FCheckBox::toggled, [this] {  if (defaultLoadProjectBehaviorBox->isChecked())
                                                                            altair->RefreshFlag = altair->RefreshFlag 
                                                                                                  | interfaceStatus::parseXml;});

    connect(defaultMaximumConsoleOutputBox, &FCheckBox::toggled, [this] {
        QTimer *timer = new QTimer(this);
        if (v(limitConsoleOutput).isTrue())
        {
            connect(timer, &QTimer::timeout, [&] { altair->readRankSignal();});
            connect(&(altair->process), SIGNAL(finished(int)), timer, SLOT(stop()));
            timer->start(PROGRESSBAR_TIMEOUT);
        } else
        {
            timer->stop();
        }

    });

    setWindowTitle(tr("Configuration"));
    setWindowIcon(QIcon(":/images/altair.png"));

}

void MainWindow::adjustDisplay(bool projectFileStatus)
{
    if (projectFileStatus == false || (altair->RefreshFlag & interfaceStatus::parseXmlMask) !=  interfaceStatus::parseXml)
    {

        for (FCheckBox* a :  displayWidgetListBox +  behaviorWidgetListBox + displayToolBarCBoxListBox)
        {
            a->toggle();
            a->toggle();
            // Ceci pour lancer très simplement les signaux et slots afin d'activer les effets sur l'interface
            // correspondant à la configuration
        }

        // Peut être modifié pour ajuster le comportement par défaut minimal ici :

#ifdef MINIMAL
        for (FCheckBox* a :  displayWidgetListBox +  behaviorWidgetListBox + displayToolBarCBoxListBox)
            if (a != defaultProcessToolBarBox &&
                a != defaultOptionsToolBarBox &&
                a != defaultAboutToolBarBox)
                   a->setChecked(false);
#endif
    }
}

void MainWindow::showMainWidget(bool full)
{
  static QSize bottomsize;
  static QSize managersize;
  static QSize filetreesize;
  if (full)
  {
      bottomsize =  bottomDockWidget->size();
      filetreesize = fileTreeViewDockWidget->size();
      managersize = managerDockWidget->size();
  }

   if (full)
  {
      setWindowState(Qt::WindowFullScreen);
      displayAction->setIcon(QIcon(":/images/show-normal.png"));
  }
  else
  {
      setWindowState(Qt::WindowNoState);
      displayAction->setIcon(QIcon(":/images/show-maximized.png"));
  }

}

void MainWindow::showMainWidget()
{
      showMainWidget(this->windowState() != Qt::WindowFullScreen);
}


void MainWindow::feedLHXConsoleWithHtml()
{

    altair->readRankSignal();

    QString buffer = QString::fromLatin1(altair->process.readAllStandardOutput());

    consoleDialog->insertHtml(buffer);
    ++consoleCounter;

    consoleDialog->moveCursor(QTextCursor::End);
}

void MainWindow::feedRConsoleWithHtml()
{
    QRegExp reg("([0-9]+).*%");
    while (altair->process.canReadLine())

    while (altair->process.canReadLine())
    {
        QString buffer=QString::fromLocal8Bit(altair->process.readLine());

        if (buffer.contains(reg))
        {
            altair->fileRank = reg.cap(1).toInt();
            if (altair->fileRank <= 0) altair->fileRank = 1;
        }
        consoleDialog->insertHtml(buffer.replace("\n", "<br>"));
        consoleDialog->moveCursor(QTextCursor::End);
    }
}


void MainWindow::resetCounter()
{
    consoleCounter = 0;
}


