#include "common.h"
#include "altair.h"
#include "enums.h"
#include <QApplication>
#include <QDirIterator>

#include <fstream>
#include <sstream>

using namespace std;
// Should it slow down application launch on some platform, one option could be to launch it just once then on user demand




void MainWindow::standardDisplay()
{
#ifdef HEIGHT 
    height = HEIGHT;
#endif

#ifdef WIDTH 
    width = WIDTH;
#endif

#ifdef MINIMAL
  setGeometry(QRect(200, 200,600,400));
#else

  setGeometry(QRect(200, 300, width / 2, height / 2));
#endif

  
  displayAction->setIcon(QIcon(":/images/show-maximized.png"));
}

MainWindow::MainWindow(char* projectName)
{
   QRect rec = QApplication::desktop()->availableGeometry();
    
   height = rec.height();
   width  = rec.width();
  
  recentFiles = QStringList() ;
  settings = new QSettings("altair", "Juridictions Financières");

# ifndef Q_OS_WIN
  const QString cdROM = common::cdRomMounted();
  if (settings->value("importerAuLancement") == true && ! cdROM.isEmpty())
       {
            if (QDir(cdROM).exists() && ! QDir(cdROM).QDir::entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot).isEmpty())
            {
                process.start("./Avert", {"200"});
            }
       }
# endif
  
  altair = new Altair;
  altair->parent = this;

  createActions();
  createMenus();

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

  bottomDockWidget = new QDockWidget;
  bottomTabWidget = new QTabWidget;
  consoleDialog =  new QTextEdit;
  consoleDialog->setReadOnly(true);

  bottomTabWidget->addTab(altair->outputTextEdit, tr("Messages"));
  bottomTabWidget->addTab(consoleDialog, tr("Console"));
  bottomTabWidget->setCurrentIndex(0);

  fileTreeViewDockWidget = new QDockWidget;
  fileTreeViewDockWidget->setWidget(altair->fileTreeView);
  fileTreeViewDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::LeftDockWidgetArea, fileTreeViewDockWidget);

  managerDockWidget= new QDockWidget;
  managerDockWidget->setWidget(altair->managerWidget);
  managerDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
  addDockWidget(Qt::RightDockWidgetArea, managerDockWidget);
  
  Abstract::initH();
  
  Abstract::refreshOptionFields();

  configureOptions();
  createToolBars();

  bottomTabWidget->setCurrentIndex(0);

  QToolButton *clearBottomTabWidgetButton=new QToolButton;
  const QIcon clearOutputText = QIcon(QString::fromUtf8( ":/images/edit-clear.png"));
  clearBottomTabWidgetButton->setToolTip("Effacer l'onglet courant de la console");
  clearBottomTabWidgetButton->setIcon(clearOutputText);

  QGroupBox *stackedBottomWidget = new QGroupBox;
  QHBoxLayout *stackedBottomWidgetLayout = new QHBoxLayout;
  QVBoxLayout *buttonsForBottomWidgetLayout = new QVBoxLayout;
  buttonsForBottomWidgetLayout->addWidget(clearBottomTabWidgetButton);

  QToolButton *nppBottomTabWidgetButton=new QToolButton;
  const QIcon nppIcon = QIcon(QString::fromUtf8( ":/images/internet-explorer.png"));
  nppBottomTabWidgetButton->setToolTip("Afficher les messages dans l'explorateur internet");
  nppBottomTabWidgetButton->setIcon(nppIcon);
  buttonsForBottomWidgetLayout->addWidget(nppBottomTabWidgetButton);

  stackedBottomWidgetLayout->addLayout(buttonsForBottomWidgetLayout);
  stackedBottomWidgetLayout->addWidget(bottomTabWidget);
  stackedBottomWidget->setLayout(stackedBottomWidgetLayout);

  bottomDockWidget->setWidget(stackedBottomWidget);

  standardDisplay();
 
  addDockWidget(Qt::BottomDockWidgetArea, bottomDockWidget);

  setWindowIcon(QIcon(":/images/altair.png"));
  setWindowTitle("Interface  Altaïr "+ QString(VERSION));

  dialog=new options(altair);
  dialog->setParent(altair, Qt::Window);

  Altair::RefreshFlag =  Altair::RefreshFlag  | interfaceStatus::parseXml;

  if (settings->value("loadProjectBehavior") == true)
      projectFileStatus = altair->clearInterfaceAndParseProject();

  
  // resetting interfaceStatus::parseXml bits to 0
  Altair::RefreshFlag = Altair::RefreshFlag & (~interfaceStatus::parseXml);

  connect(nppBottomTabWidgetButton, &QToolButton::clicked, [this] { on_nppButton_clicked();});
  connect(clearBottomTabWidgetButton, &QToolButton::clicked, [this] { on_clearOutputTextButton_clicked();});
  connect(consoleDialog, SIGNAL(copyAvailable(bool)), consoleDialog, SLOT(copy()));
  connect(&(altair->process), SIGNAL(finished(int)), this, SLOT(resetCounter()));

  if (projectName[0] != '\0')
  {
      // Paraît étrange... mais c'est pour éviter de lire deux fois le projet
      altair->closeProject();
      altair->projectName = projectName;
      altair->openProjectFileCommonCode();
  }
  
  // Mettre un lien symbolique dans le dossier xhl vers cdrom
  // Pour des raisons de dépendances cycliques il faut placer ceci à la fin et dans MainWindow.
  
  if (! settings->value("importerAuLancement").isValid()) 
      settings->value("importerAuLancement") = true;
              
  if (settings->value("importerAuLancement") == true)
      altair->importData();
  
}

void MainWindow::on_nppButton_clicked()
{

    tempLog.setFileName(common::generateDatadirPath("/log.html"));
    if (tempLog.exists()) tempLog.remove();

    tempLog.open(QIODevice::ReadWrite);
    tempLog.write(
        qobject_cast<QTextEdit*>(bottomTabWidget->currentWidget())->toHtml().replace(":", common::generateDatadirPath("")).toLatin1()
                );
    tempLog.close();

    QUrl url=QUrl::fromLocalFile(tempLog.fileName());

    browser::showPage(url);
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

 processMenu->addActions({RAction, lhxAction, cleanAction, anonymAction, openBaseDirAction});

 optionsMenu->addActions({optionsAction, configureAction});

 aboutMenu->addActions({helpAction, aboutAction,licenceAction});
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
  connect(saveAction, &QAction::triggered, [this] { altair->updateProject(true); });
  
  saveAsAction = new QAction(tr("En&registrer le projet comme..."), this);
  saveAsAction->setIcon(QIcon(":/images/document-save-as.png"));
  connect(saveAsAction, SIGNAL(triggered()), altair, SLOT(requestSaveProject()));

  exportAction = new QAction(tr("E&xporter le projet vers...\nAttention : le projet doit avoir été exéxuté"), this);
  exportAction->setIcon(QIcon(":/images/export.png"));
  connect(exportAction, SIGNAL(triggered()), this, SLOT(exportProject()));

  archiveAction = new QAction(tr("Archiver le projet vers..."), this);
  archiveAction->setIcon(QIcon(":/images/archive.png"));
  connect(archiveAction, SIGNAL(triggered()), this, SLOT(archiveProject()));

  restoreAction = new QAction(tr("Désarchiver le projet"), this);
  restoreAction->setIcon(QIcon(":/images/restore.png"));
  connect(restoreAction, SIGNAL(triggered()), this, SLOT(restoreProject()));

  closeAction = new QAction(tr("&Fermer le projet"), this);
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

  cleanAction = new QAction(tr("Nettoyer la base de paye"), this);
  cleanAction->setShortcut(QKeySequence("Ctrl+$"));
  cleanAction->setIcon(QIcon(":/images/clean.png"));
  connect(cleanAction, SIGNAL(triggered()), this, SLOT(cleanBase()));

  anonymAction = new QAction(tr("Anonymiser la base de données XML"), this);
  anonymAction->setIcon(QIcon(":/images/anonymiser.png"));
  connect(anonymAction, SIGNAL(triggered()), this, SLOT(anonymiser()));

  openBaseDirAction = new QAction(tr("Ouvrir le répertoire des bases"), this);
  openBaseDirAction ->setIcon(QIcon(":/images/directory.png"));
  connect(openBaseDirAction, &QAction::triggered, [&] { 
  #           ifdef INSERT_DIRPAGE
                  common::openDir(dialog->dirTab->donneesCSV->getText());  
  #           else
                  
                  QString userdatadir = common::path_access("Tests/Exemple/Donnees/" AltairDir );
                  if (! QFileInfo(userdatadir).isDir())
                  {
                      QDir dir;
                      dir.mkdir(userdatadir);
                  }
                  common::openDir(userdatadir); 
  #           endif
                  });

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

  actionList << newAction << openAction << saveAction << saveAsAction << exportAction
             << archiveAction << restoreAction << closeAction << exitAction << separator[0]
             << RAction << lhxAction << cleanAction << anonymAction << openBaseDirAction
             << displayOutputAction << displayFileTreeViewAction << displayManagerAction <<  separator[4]
             << clearOutputTextAction <<  editProjectAction << separator[3] << configureAction
             << optionsAction << helpAction << aboutAction ;
 
}

vector<string> MainWindow::extraire_donnees_protegees(const string& st)
{
    vector<string> out;
    const size_t taille = st.size();
    out.reserve((size_t) taille / 5);
    string::const_iterator iter = st.begin();

    size_t i = 0;
    const size_t pas = taille / 20;
    size_t k = 0;

    while (*iter != '<' && ++iter != st.end()) continue;

    if (*(iter + 1) == '?')
    {
        string::const_iterator iter1 = iter;
        while (++iter != st.end() && *iter != '>') continue;
        out.emplace_back(string(iter1, iter + 1) + "\n");
    }

    start:
    while (iter != st.end())
    {
        while (++iter != st.end()
               && *iter != '<') { ++i; ++k; continue;};

        if (iter == st.end()) break;

        if (*(iter + 1) == '/') /* </TAG>  */
        {
            string::const_iterator iter1 = iter;
            while (++iter != st.end() && *iter != '>') { ++i; ++k; continue;};
            out.emplace_back(string(iter1, iter + 1) + "\n");
            continue;
        }

        if (iter == st.end()) break;

        if (k >= pas)
        {
            emit(altair->setProgressBar(i));
            qApp->processEvents();
            k =0;
        }

        string::const_iterator iter1 = iter;
        string tag;

        iter1 = iter;

        while (++iter != st.end())
        {
            ++i; ++k;
            if (*iter == ' ')  /* <TAG ...> */
            {
              tag = string(iter1 + 1, iter);
              break;
            }

            if (*iter == '>')  /* <TAG> */
            {
                out.emplace_back("<" + string(iter1 + 1, iter) + ">\n");
                goto start;
            }
        }

        if (iter == st.end()) break;

        while (++iter != st.end())
        {
            ++i; ++k;

            if (*iter == 'V')
            {
               while (++iter != st.end() && *iter == ' ') { ++ i; ++k; continue; };

               if (*iter == '=')
               {
                 break; /*<TAG  V  =... /> */
               }
               else continue; /*<TAG  x ... /> */
            }
            else /* <TAG> */ /*<TAG  x... /> */
            if (*iter == '>')
            {
                out.emplace_back(string(iter1, iter +1) + "\n");
                ++iter;
                ++i; ++k;
                goto start;
            }
            else   /*<TAG  x... /> */
            continue;
        }

        if (iter == st.end()) break;

         /*<TAG  V  =... /> */

        while (++iter != st.end())
        {
            ++i; ++k;
            if (*iter == '\"') break;
        }

        if (iter == st.end()) break;

        iter1 = iter;

        while (++iter != st.end())
        {
            ++i; ++k;
            if (*iter == '\"') break;
        }

        if (iter == st.end()) break;

        string value = string(iter1 + 1, iter);
        auto iter2 = iter;

        while (++iter != st.end())
        {
            ++i; ++k;
            if (*iter == '>') break;
        }

        if (iter == st.end()) break;

        string reste = string(iter2 + 1, iter + 1);

        const array<string, 12> Tags = {
                                             "Civilite",
                                             "Nom",
                                             "Prenom",
                                             "Adr1",
                                             "Adr2",
                                             "Ville",
                                             "CP",
                                             "TitCpte",
                                             "NumUrssaf",
                                             "Siret",
                                             "IdCpte",
                                             "NIR"
                                       };

        for (const string& t : Tags)
        {
            if (tag == t)
            {
                const string& remplacement = (tag == "NIR")? "<NIR  V = \"" + value.substr(0, 5) + "Z\"" + reste + "\n"
                                                    : "<" + tag + " V = \"Z\"" + reste + "\n";

                out.emplace_back(remplacement);

                ++iter; ++i; ++k;

                goto start;
            }
        }

        out.emplace_back("<" + tag + " V = \"" + value + "\"" + reste + "\n");

    }

    return(out);
}

void MainWindow::launch_process(const QString& path)
{

    QFile xml(path);

    std::string xml_out(path.toStdString() + ".new");

    std::ofstream out;

    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Q("Erreur de lancement du processus")
        return;
    }

    out.open(xml_out, std::ofstream::out | std::ofstream::trunc);

    if (! out.is_open())
    {
        Q("Erreur de lancement du processus")
        return;
    }

    altair->outputTextEdit->append(PROCESSING_HTML_TAG "Lecture du fichier...");
    emit(altair->showProgressBar());
    emit(altair->setProgressBar(0));
    altair->outputTextEdit->repaint();
    QString xml_mod = QString(xml.readAll());

    altair->outputTextEdit->append(PROCESSING_HTML_TAG "Remplacement des informations protégées. Patientez...");
    altair->outputTextEdit->repaint();

    int bar_range = xml_mod.size();

    emit(altair->setProgressBar(0, bar_range));

    const vector<string>& v = extraire_donnees_protegees(xml_mod.toStdString());

    for (const string &s: v)
        out << s;

    out.close();

    emit(altair->setProgressBar(0));

    xml.close();
    xml.remove();
    rename(xml_out.c_str(), path.toStdString().c_str());

    altair->outputTextEdit->append(PROCESSING_HTML_TAG  "Anonymisation de " + path + " terminée.");
    altair->outputTextEdit->repaint();
    emit(altair->setProgressBar(0));

    /* Il est souhaitable d'actualiser le projet avant de lancer v() car en cas de non actualisation récente la valeur de la case
     * peut être en décalage avec la réalité. C'est au cours de ces actualisations que la valeur est enregistrée dans une table de hashage. */

    altair->updateProject(true);

}

string MainWindow::nettoyer_donnees(const string& st)
{
    string out;
    const size_t taille = st.size();
    out.reserve((size_t) taille / 5);
    string::const_iterator iter = st.begin();
    string::const_iterator iter2; 
    size_t i = 0;
    size_t k = 0;
    const size_t pas = taille / 20;

    loop :
    while (iter != st.end())
    {
       switch (*iter) 
       {
         case  '&'  :
               iter2 = iter;
                while (++iter2 != st.end())
                   {
                       switch (*iter2)
                       {
                           case  '\"'  :
                           case  ' '  : 
                               iter = iter2 + 1;
                               out += *iter2;
                               goto loop;
          
                           case ';'  :
                               iter = iter2 + 1;
                               goto loop;
                               
                            default :
                               continue;
                       }
                   }
                   
                   continue;

        case '>' :         
                   out += ">\n";
                   while (*++iter != '<' && iter != st.end())  ;
                   break;
                               
        default :
                    out += isprint(*iter) ? *iter : ' ';  // Si par exemple "\nxmls:xsi....
                   ++iter;
                   break;
       }

     ++i;
     ++k;
     if (k >= pas)
     {
         emit(altair->setProgressBar(i));
         qApp->processEvents();
         k = 0;
     }
  }

    return(out);
}

void MainWindow::clean_process(const QString& path)
{
    QFile xml(path);

    std::string xml_out(path.toStdString() + ".new");

    std::ofstream out;

    if (!xml.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Q("Erreur de lancement du processus")
        return;
    }

    out.open(xml_out, std::ofstream::out | std::ofstream::trunc);

    if (! out.is_open())
    {
        Q("Erreur de lancement du processus")
        return;
    }

    altair->outputTextEdit->append(PROCESSING_HTML_TAG "Lecture du fichier...");
    emit(altair->showProgressBar());
    emit(altair->setProgressBar(0));
    altair->outputTextEdit->repaint();
    QString xml_mod = QString(xml.readAll());

    altair->outputTextEdit->repaint();

    int bar_range = xml_mod.size();

    emit(altair->setProgressBar(0, bar_range));

    const string& v = nettoyer_donnees(xml_mod.toStdString());

    out << v;

    out.close();

    emit(altair->setProgressBar(0));

    xml.close();
    xml.remove();
    rename(xml_out.c_str(), path.toStdString().c_str());

    altair->outputTextEdit->append(PROCESSING_HTML_TAG  "Nettoyage de " + path + " terminé.");
    altair->outputTextEdit->repaint();
    
    altair->updateProject(true);
    
}

void MainWindow::anonymiser()
{
    QStringList args;

    altair->updateProject(true);

    args << altair->createCommandLineString();

    altair->outputTextEdit->append(PROCESSING_HTML_TAG + tr("Anonymisation des bases de paye ("));

    for (const QString& s: args)
    {
        if (! s.isEmpty() && QFileInfo(s).isFile())
        {
            altair->outputTextEdit->append(PROCESSING_HTML_TAG "Lancement de l'anonymisation du fichier " + s + ". Patientez...");
            altair->outputTextEdit->repaint();
            launch_process(s);
        }
    }
}

void MainWindow::cleanBase()
{
    QStringList args;

    altair->updateProject(true);

    args << altair->createCommandLineString();

    altair->outputTextEdit->append(PROCESSING_HTML_TAG + tr("Nettoyage des bases de paye..."));

    for (const QString& s: args)
    {
        if (! s.isEmpty() && QFileInfo(s).isFile())
        {
            altair->outputTextEdit->append(PROCESSING_HTML_TAG "Nettoyage du fichier " + s + ". Patientez...");
            altair->outputTextEdit->repaint();
            clean_process(s);
        }
    }
    
    emit(altair->setProgressBar(0, 100));
    emit(altair->setProgressBar(100));
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
 fileToolBar->addActions({newAction, saveAsAction, exportAction, archiveAction, restoreAction, closeAction, exitAction});
 fileToolBar->addSeparator();

 editToolBar->addActions({displayAction, displayOutputAction, displayFileTreeViewAction});

 processToolBar->addActions({RAction, lhxAction, cleanAction, openBaseDirAction});

 optionsToolBar->addActions({optionsAction, configureAction});

 aboutToolBar->addActions({helpAction, aboutAction});
 aboutToolBar->hide();
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

    projectFile.setFileName(altair->projectName);

   if (projectFile.open(QFile::ReadWrite| QFile::Text))
   {
       editor->setPlainText(projectFile.readAll());
       projectFile.close();
   }
   // do not capture file by reference!
   connect(actionHash["Nouveau"],
                 &QAction::triggered,
                 [this] { altair->on_newProjectButton_clicked(); });

   connect(actionHash["Ouvrir"],
                 &QAction::triggered,
                 [this]
                                  {
                                     altair->on_openProjectButton_clicked() ;
                                     editWidget->~QMainWindow();
                                     on_editProjectButton_clicked();
                                   });

   connect(actionHash["Enregistrer"],
                 &QAction::triggered,
                 [this]
                                 {
                                    projectFile.open(QFile::Truncate |QFile::WriteOnly| QFile::Text);
                                    projectFile.write(editor->document()->toPlainText().toUtf8()) ;
                                    projectFile.close();
                                    altair->closeProject();
                                    altair->RefreshFlag = altair->RefreshFlag | interfaceStatus::parseXml;
                                    altair->clearInterfaceAndParseProject();
                                    // resetting interfaceStatus::parseXml bits to 0
                                    altair->RefreshFlag = altair->RefreshFlag & (~interfaceStatus::parseXml);
                                  });

   connect(actionHash["Enregistrer comme..."],
                  &QAction::triggered,
                  [this] {saveProjectAs();});

   connect(actionHash["Actualiser"],
                 &QAction::triggered,
                 [this]
                                 {
                                    altair->updateProject(true);
                                    if (projectFile.open(QFile::ReadWrite |  QFile::Text))
                                       {
                                           editor->clear();
                                           editor->setPlainText(projectFile.readAll());
                                           projectFile.close();
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
                 [this]
                                  {
                                     editWidget->~QMainWindow() ;
                                   });
   editWidget->setCentralWidget(editor);
   editWidget->setGeometry(200,200,600,800);
   editWidget->show();

}


void MainWindow::saveProjectAs(const QString &newstr)
{

    if  (QFileInfo(newstr).isFile())
    {
          QMessageBox::StandardButton result = QMessageBox::warning(nullptr, "Ecraser le fichier ?", "Le projet "+ newstr +" va être écrasé.\nAppuyer sur Oui pour confirmer.",
                                            QMessageBox::Ok | QMessageBox::Cancel);


          if (result != QMessageBox::Ok)
          {
              return;
          }

          std::remove(newstr.toStdString().c_str());
    }

    if (editor)
    {
        QFile file(newstr);
        if (file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text))
       {
           file.write(editor->document()->toPlainText().toUtf8()) ;
           file.close();
       }

       altair->projectName = newstr;
    }
    else
    {
        altair->projectName = newstr;
        altair->writeProjectFile();
    }

    Altair::RefreshFlag =  Altair::RefreshFlag
                            | interfaceStatus::tree;

    altair->setCurrentFile(newstr);
    // attention dans cet ordre !

}

void MainWindow::saveProjectAs()
{
    QString newstr=QFileDialog::getSaveFileName(this, tr("Enregistrer le projet comme..."), altair->userdatadir, tr("projet altair (*.alt)"));
    if (newstr.isEmpty())
                return;
    else
        saveProjectAs(newstr);

 }



bool MainWindow::exportProject(QString dirStr)
{
    if (Hash::wrapper.isEmpty()) return false;
    if (Hash::wrapper["XHL"] == nullptr || Hash::wrapper["XHL"]->isEmpty()) return false;

    if (dirStr.isEmpty())
        dirStr = QFileDialog::getExistingDirectory(this, tr("Exporter le rapport vers le répertoire..."), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    if (! QFileInfo(dirStr).isDir()) return false;

    const QString subDirStr = QDir::toNativeSeparators(dirStr.append("/Altaïr/"));
    QDir().mkpath(subDirStr);

    if (QMessageBox::Ok != QMessageBox::warning(nullptr, "", "Les résultats seront exportés vers le dossier <br>" + subDirStr,  QMessageBox::Cancel|QMessageBox::Ok, QMessageBox::Ok))
        return false;

    altair->outputTextEdit->append(PROCESSING_HTML_TAG "Exportation en cours. Patientez...");
    altair->outputTextEdit->repaint();
    altair->updateProject();
   
    const QString projectRootDir = QDir::toNativeSeparators(QDir::cleanPath(v(base)));
    const QString docxReportFilePath = projectRootDir + QDir::separator() + "altaïr.docx";
	const QString odtReportFilePath = projectRootDir + QDir::separator() + "altaïr.odt";
    const QString pdfReportFilePath = projectRootDir + QDir::separator() + "altaïr.pdf";
    QDir dir(v(base));
    QStringList tableList = dir.entryList(QStringList("Table*.csv"), QDir::Files);
    
    const QString bulletinsFilePath = v(base)  +  QDir::separator() + "Bulletins.csv";
    
    bool result = true;

    result = common::copyFile(docxReportFilePath, subDirStr  + "altaïr.docx", "Le rapport Altaïr Word", REQUIRE);

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été exporté sous : " + subDirStr);

    result &= common::copyFile(odtReportFilePath, subDirStr  + "altaïr.odt", "Le rapport Altaïr Open Office", REQUIRE);

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr Open Office a été exporté sous : " + subDirStr);

    result &= common::copyFile(pdfReportFilePath, subDirStr  + "altaïr.pdf", "Le rapport Altaïr PDF", REQUIRE);

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été exporté sous : " + subDirStr);
    
    if (v(exportTable).isTrue() || v(exportAll).isTrue())
    {
        for (const QString &st: tableList)  
        {
          result = common::copyFile(v(base) + QDir::separator() + st, subDirStr + st, "La base des lignes de paye", REQUIRE);
          if (result)
              altair->outputTextEdit->append(PARAMETER_HTML_TAG  "La base " + v(base) + QDir::separator() + st + " a été exportée sous : " + subDirStr);
          else
              altair->outputTextEdit->append(ERROR_HTML_TAG  "La base " + v(base) + QDir::separator() + st + " n'a pas pu être exportée sous : " + subDirStr);

          altair->outputTextEdit->repaint();
        }
        
        result = common::copyFile(bulletinsFilePath, subDirStr  + "Bulletins.csv", "La base des lignes de paye a été exportée sous : " , REQUIRE);
        if (result)
            altair->outputTextEdit->append(PARAMETER_HTML_TAG  "La base des bulletins de paye a été exportée sous : " + subDirStr);
        else
            altair->outputTextEdit->append(ERROR_HTML_TAG  "La base des bulletins de paye n'a pas pu être exportée sous : " + subDirStr);
    }
    
    if (v(exportXML).isTrue() || v(exportAll).isTrue())
    {
        for (int rank = 0; rank < Hash::wrapper["XHL"]->size() - 3; ++rank)  
        {
          for(const QString &s :  Hash::wrapper["XHL"]->at(rank))
          {
            result = common::copyFile(s, subDirStr  + common::getEmbeddedPath(s), "La base XML " + s, REQUIRE);
            if (result)
                altair->outputTextEdit->append(PARAMETER_HTML_TAG  "La base " +  s + " a été exportée sous : " + subDirStr);
            else
                altair->outputTextEdit->append(ERROR_HTML_TAG  "La base " +  s + " n'a pas pu être exportée sous : " + subDirStr);

            altair->outputTextEdit->repaint();
          }
        }
    }
    
    result &= common::copyDir(projectRootDir + "/Docs", subDirStr + "Docs");
    result &= common::copyDir(projectRootDir + "/Bases", subDirStr + "Bases");
    
    if (result)
        altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Les bases en lien ont été exportées sous : " + subDirStr + "Bases");
    else
        altair->outputTextEdit->append(ERROR_HTML_TAG  "Les bases en lien n'ont pas pu être exportées sous : " + subDirStr + "Bases");


    saveProjectAs(subDirStr + "projet.alt");

    return result;
}

bool MainWindow::archiveProject()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Archiver le rapport vers le répertoire..."),
                                                        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog);

    QString subDirStr = QDir::toNativeSeparators(dirName.append("/Archives Altaïr/" +
                                                               QDate::currentDate().toString("dd MM yyyy")
                                                               + "-" + QTime::currentTime().toString("hh mm ss")));
    altair->updateProject();
            
    if (QMessageBox::Ok != QMessageBox::warning(nullptr, "", "Les résultats seront archivés dans le dossier <br>" + subDirStr,  QMessageBox::Cancel|QMessageBox::Ok, QMessageBox::Ok))
        return false;

    altair->outputTextEdit->append(PROCESSING_HTML_TAG "Archivage en cours. Patientez...");
    altair->outputTextEdit->repaint();
    
    const QString projectRootDir = QDir::toNativeSeparators(QDir::cleanPath(v(base)));
    const QString docxReportFilePath = projectRootDir +  QDir::separator() + "altaïr.docx";
    const QString pdfReportFilePath = projectRootDir +  QDir::separator()  + "altaïr.pdf";
	const QString odtReportFilePath = projectRootDir +  QDir::separator() + "altaïr.odt";
    QDir dir(v(base));
    QStringList tableList = dir.entryList(QStringList("Table*.csv"), QDir::Files);

    const QString bulletinsFilePath = v(base)  +  QDir::separator() + "Bulletins.csv";
	
    bool result = true;
    
    result &= common::zip(docxReportFilePath, subDirStr + QDir::separator() + "altaïr.docx.arch");

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été archivé sous : " + subDirStr);

    result &= common::zip(odtReportFilePath, subDirStr + QDir::separator() + "altaïr.odt.arch");

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr Open Office a été archivé sous : " + subDirStr);

    result &= common::zip(pdfReportFilePath, subDirStr + QDir::separator() + "altaïr.pdf.arch");

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été archivé sous : " + subDirStr);
    
    saveProjectAs(subDirStr + "/projet.alt");

     if (v(archiveTable).isTrue() || v(archiveAll).isTrue())
     {
         for (const QString &st: tableList)  
         {
           result = common::zip(v(base) + QDir::separator() + st, subDirStr + QDir::separator() + common::getEmbeddedPath(st) + ".arch");
           if (result)
               altair->outputTextEdit->append(PARAMETER_HTML_TAG  "La base " + v(base) + QDir::separator() + st + " a été archivée sous : " + subDirStr);
           else
               altair->outputTextEdit->append(ERROR_HTML_TAG  "La base " + v(base) + QDir::separator() + st + " n'a pas pu être archivée sous : " + subDirStr);

           altair->outputTextEdit->repaint();
         }
         result = common::zip(bulletinsFilePath, subDirStr + QDir::separator() + "Bulletins.csv.arch");
         if (result)
             altair->outputTextEdit->append(PARAMETER_HTML_TAG  "La base des bulletins de paye a été archivée sous : " + subDirStr);
         else
             altair->outputTextEdit->append(ERROR_HTML_TAG  "La base des bulletins de paye n'a pas pu être archivée sous : " + subDirStr);
     }

     if (v(archiveXML).isTrue() || v(archiveAll).isTrue())
     {
         for (int rank = 0; rank < Hash::wrapper["XHL"]->size() - 3; ++rank)  
         {
           for(const QString &s :  Hash::wrapper["XHL"]->at(rank))
           {
             result = common::zip(s, subDirStr + QDir::separator() + common::getEmbeddedPath(s) + ".arch");
             if (result)
                 altair->outputTextEdit->append(PARAMETER_HTML_TAG  "La base " +  s + " a été archivée sous : " + subDirStr);
             else
                 altair->outputTextEdit->append(ERROR_HTML_TAG  "La base " +  s + " n'a pas pu être archivée sous : " + subDirStr);

             altair->outputTextEdit->repaint();
           }
         }
     }
     
     result &= common::zipDir(projectRootDir + "/Docs", subDirStr + "/Docs")
              & common::zipDir(projectRootDir + "/Bases", subDirStr + "/Bases");
     
    if (result)
        altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Les bases en lien ont été archivées sous : " + subDirStr + QDir::separator() + "Bases");
    else
        altair->outputTextEdit->append(ERROR_HTML_TAG  "Les bases en lien n'ont pas pu être archivées sous : " + subDirStr + QDir::separator() + "Bases");

  return result;
}

bool MainWindow::restoreProject(QString subDirStr)
{
    if (subDirStr.isEmpty())
        subDirStr = QFileDialog::getExistingDirectory(this, tr("Restaurer le rapport depuis le répertoire..."), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    subDirStr += "/";
    
    if (! QFileInfo(subDirStr).isDir()) return false;

    altair->updateProject();

    if (QMessageBox::Ok != QMessageBox::warning(nullptr, "", "Les résultats seront restaurés depuis le dossier <br>" + subDirStr,  QMessageBox::Cancel|QMessageBox::Ok, QMessageBox::Ok))
        return false;

    const QString projectRootDir = QDir::toNativeSeparators(QDir::cleanPath(v(base)));
    const QString docxReportFilePath = projectRootDir + "/altaïr.docx";
	const QString odtReportFilePath = projectRootDir + "/altaïr.odt";
    const QString pdfReportFilePath = projectRootDir + "/altaïr.pdf";

    QDir dir(subDirStr);
    QStringList tableList = dir.entryList(QStringList("Table*.csv.arch"), QDir::Files);
    
    const QString bulletinsFilePath = v(base)  +  QDir::separator() + "Bulletins.csv";
    
    bool result = true;
    
    altair->outputTextEdit->append(PROCESSING_HTML_TAG "Restauration en cours. Patientez...");
    altair->outputTextEdit->repaint();
    
    result = common::unzip(subDirStr + "altaïr.docx.arch", docxReportFilePath);

    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été décompressé sous : " + projectRootDir);

	result &= common::unzip(subDirStr + "altaïr.odt.arch", odtReportFilePath);

	if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr ODT a été décompressé sous : " + projectRootDir);
    
    result &= common::unzip(subDirStr + "altaïr.pdf.arch", pdfReportFilePath);
	
    if (result) altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été décompressé sous : " + projectRootDir);
	
    if (v(archiveTable).isTrue() || v(archiveAll).isTrue())
    {
        for (const QString &st: tableList)  
        {
          QString st2 = st;
          st2.chop(5);

          result = common::unzip(subDirStr  + st, v(base) + QDir::separator() + st2);
          if (result)
              altair->outputTextEdit->append(PARAMETER_HTML_TAG  "La base " + projectRootDir + QDir::separator() + st2 + " a été décompressée sous : " + subDirStr);
          else
              altair->outputTextEdit->append(ERROR_HTML_TAG  "La base " + projectRootDir + QDir::separator() + st2 + " n'a pas pu être décompressée sous : " + subDirStr);

          altair->outputTextEdit->repaint();
        }
        
        result = common::unzip(subDirStr  + "Bulletins.csv.arch", bulletinsFilePath);
        if (result)
            altair->outputTextEdit->append(PARAMETER_HTML_TAG  "La base des bulletins de paye a été décompressée sous : " + projectRootDir);
        else
            altair->outputTextEdit->append(ERROR_HTML_TAG  "La base des bulletins de paye n'a pas pu être décompressée sous : " + projectRootDir);

    }
    
    if (v(archiveXML).isTrue() || v(archiveAll).isTrue())
    {
           QDirIterator it(subDirStr, QStringList() << "*.xml.arch" << "*.xhl.arch" << "*XML.arch" << "*.XHL.arch", QDir::Files, QDirIterator::Subdirectories);
           while (it.hasNext())        
           {
                const QString s = it.next();
                QString filepath = common::getEmbeddedPath(s, subDirStr);
                filepath.chop(5);
                result = common::unzip(s, projectRootDir + QDir::separator() + filepath);
                if (result)
                    altair->outputTextEdit->append(PARAMETER_HTML_TAG  "La base " +  s + " a été décompressée sous : " + projectRootDir);
                else
                    altair->outputTextEdit->append(ERROR_HTML_TAG  "La base " +  s + " n'a pas pu être décompressée sous : " + projectRootDir);

                altair->outputTextEdit->repaint();
           }
    }
        
    result &= common::unzipDir(subDirStr + "Docs", projectRootDir + "/Docs");
    result &= common::unzipDir(subDirStr + "Bases", projectRootDir + "/Bases");

    common::copyFile(subDirStr + "projet.alt", projectRootDir + "/projet.alt", "Le projet Altaïr", REQUIRE);

    if (result)
        altair->outputTextEdit->append(PARAMETER_HTML_TAG  "Les bases en lien ont été décompressées sous : " + projectRootDir + QDir::separator() + "Bases");
    else
        altair->outputTextEdit->append(ERROR_HTML_TAG  "Les bases en lien n'ont pas pu être décompressées sous : " + projectRootDir + QDir::separator() + "Bases");

  return result;
}

void MainWindow::configureOptions()
{
    /* plain old data types must be 0-initialised even though the class instance was new-initialised. */

    contentsWidget = new QDialog(this);
    contentsWidget->setVisible(false);

    QGroupBox *displayGroupBox = new QGroupBox(tr("Affichage"));
    QGroupBox *displayToolBarsGroupBox = new QGroupBox(tr("Barres d'outils"));

    closeButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    closeButton->button(QDialogButtonBox::Ok)->setText("Accepter");
    closeButton->button(QDialogButtonBox::Cancel)->setText("Annuler");    

    defaultProjectManagerWidgetLayoutBox = new FCheckBox("Afficher le gestionnaire de projet",
                                                   #ifdef MINIMAL
                                                                            flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                   #else
                                                                            flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                                   #endif
                                                                            "projectManagerDisplay",
                                                                            {"Interface", "Afficher le gestionnaire de projet"});

    defaultFileManagerWidgetLayoutBox = new FCheckBox("Afficher le gestionnaire de fichiers",
                                                    #ifdef MINIMAL
                                                                            flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                    #else
                                                                            flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                                    #endif
                                                                            "fileManagerDisplay",
                                                                           {"Interface", "Afficher le gestionnaire de fichiers"});

    defaultFullScreenLayoutBox = new FCheckBox("Plein écran",
                                                        flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                        "fullScreenDisplay",
                                                        {"Interface", "Plein écran au lancement"});

    defaultOutputTextEditBox = new FCheckBox("Afficher les messages",
                                       #ifdef MINIMAL
                                                                   flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                       #else
                                                                   flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                       #endif
                                                                   "outputTextEdit",
                                                                   {"Interface", "Afficher les messages"});
    
    defaultFileToolBarBox = new FCheckBox("Afficher la barre d'outils de fichiers",
                                    #ifdef MINIMAL
                                                                flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                    #else
                                                                flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                    #endif
                                                                "fileToolBar",
                                                                {"Interface", "Afficher la barre d'outils de fichiers"});
    
    defaultEditToolBarBox = new FCheckBox("Afficher la barre d'outils d'édition",
                                                                flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                                                "editToolBar",
                                                                {"Interface", "Afficher la barre d'outils d'édition"});
    
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

    importerAuLancementBox = new FCheckBox("Charger les données utilisateur au lancement",
                                                                            (settings->value("importerAuLancement") == true ? flags::status::enabledChecked : 
                                                                                                                              flags::status::enabledUnchecked) 
                                                                            | flags::commandLineType::noCommandLine,
                                                                            "importerAuLancement",
                                                                            {"Interface", "Charger les données xhl du disque optique\nou du répertoire de données au lancement"});
    
    defaultLoadProjectBehaviorBox = new FCheckBox("Charger le projet par défaut au lancement",
                                                                            flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                                            "loadProjectBehavior",
                                                                            {"Interface", "Charger le projet .alt au lancement"});

    // Ces deux cases sont mutuellement exclusives. On aurait pu mettre un FRadioButton à la place. On laisse des FCheckBox par esthétique
    // et aussi pour éviter de devoir rajouter toute cette classe pour ce seul cas de figure.
    // apparemment ne fonctionne pas sous Windows si l'ordre des deux instruction est inversé... OK sous Linux.

    defaultLoadProjectBehaviorBox->setDisableObjects({importerAuLancementBox});
    importerAuLancementBox->setDisableObjects({defaultLoadProjectBehaviorBox});
                        
    QGroupBox *outputGroupBox= new QGroupBox(tr("Console"));

    defaultMaximumConsoleOutputBox = new FCheckBox("Limiter la sortie de la console",
                                                                            flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                                                                            "limitConsoleOutput",
                                                                            {"Interface", "Limiter le nombre de lignes en sortie de la console"});

    defaultQuietBox = new FCheckBox("Limiter la verbosité",
                                       flags::status::enabledChecked|flags::commandLineType::defaultCommandLine,
                                       "quiet",
                                       {"Interface", "Limiter la verbosité de la console"},
                                        "q");

    displayWidgetListBox   <<  defaultFileManagerWidgetLayoutBox
                           << defaultProjectManagerWidgetLayoutBox
                           << defaultOutputTextEditBox
                           << defaultFullScreenLayoutBox;


    behaviorWidgetListBox  << defaultSaveProjectBehaviorBox
                           << defaultLoadProjectBehaviorBox
                           << importerAuLancementBox;   

    
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
                                    settings->setValue("importerAuLancement", importerAuLancementBox->isChecked());
                                    settings->setValue("loadProjectBehavior", defaultLoadProjectBehaviorBox->isChecked());
                                
                                    if  ((isDefaultSaveProjectChecked())
                                         || (QMessageBox::Yes == QMessageBox::warning(nullptr, tr("Sauvegarder le projet"),
                                                                         tr("Le projet n'a pas été sauvegardé.\nAppuyer sur Oui pour le sauvegarder\nou sur Non pour fermer le dialogue sans sauvegarder le projet."),
                                                                                      QMessageBox::Yes|QMessageBox::No))
                                        )
                                        altair->updateProject(true);
                                         
                                    contentsWidget->accept();
                                });

    /* note on connection syntax
     * Here the new Qt5 connection syntax should be used with care and disregarded when both an action button and an FCheckBox activate a slot as the slots
     * are overloaded (which could possibly be rewritten) and a) the action button uses the argumentless slot whilst
     * b) the boolean version of slots must be used by the FcheckBox. The new Qt5 syntax cannot work this out as it does not manage overloading. */

    connect(closeButton, &QDialogButtonBox::rejected, contentsWidget, &QDialog::reject);
    
    connect(defaultFileManagerWidgetLayoutBox, SIGNAL(toggled(bool)), this, SLOT(on_displayFileTreeViewButton_clicked(bool)));
    connect(defaultProjectManagerWidgetLayoutBox, SIGNAL(toggled(bool)), this, SLOT(on_openManagerWidgetButton_clicked(bool)));
    
    connect(defaultFullScreenLayoutBox, SIGNAL(toggled(bool)), this, SLOT(showMainWidget()));
    connect(defaultMaximumConsoleOutputBox, &FCheckBox::toggled, [this]{v(limitConsoleOutput).toggle();});
    connect(defaultOutputTextEditBox, &FCheckBox::toggled, [this] {bottomDockWidget->setVisible(defaultOutputTextEditBox->isChecked());});

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

void MainWindow::showMainWidget()
{
  if (windowState() != Qt::WindowFullScreen)
  {
      setWindowState(Qt::WindowFullScreen);
      displayAction->setIcon(QIcon(":/images/show-normal.png"));
  }
  else
  {
      setWindowState(Qt::WindowNoState);
      standardDisplay();
  }
}


void MainWindow::feedLHXConsoleWithHtml()
{

    altair->readRankSignal();

    QString buffer = QString::fromUtf8(altair->process.readAllStandardOutput());

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


