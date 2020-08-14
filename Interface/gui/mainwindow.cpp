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

/// \file mainwindow.cpp
/// \author Fabrice Nicol
/// \brief Code de l'implémentation de la classe MainWindow de l'interface graphique

#include "common.h"
#include "altair.h"
#include "gui_enums.h"
#include "matricules.h"
#include <QApplication>
#include <QDirIterator>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;
// Should it slow down application launch on some platform, one option could be to launch
// it just once then on user demand

void MainWindow::standardDisplay()
{
#ifdef HEIGHT
    height = HEIGHT;
#endif

#ifdef WIDTH
    width = WIDTH;
#endif

#ifdef MINIMAL
    setGeometry (QRect (200, 200, 600, 400));
#else
    setGeometry (QRect (200, 300, width / 2, height / 2));
#endif

    displayAction->setIcon (QIcon (":/images/show-maximized.png"));
}

MainWindow::MainWindow (char* projectName)
{
    QRect rec = QApplication::desktop()->availableGeometry();

    height = rec.height();
    width  = rec.width();

    settings = new QSettings ("altair", "Juridictions Financières");
    raise();

    altair = new Altair;
    altair->parent = this;
    altair->lower();
    options::RefreshFlag = interfaceStatus::hasSavedOptions;
    createActions();
    createMenus();

    if (!dialog )
    {
        dialog = new options (altair);

        dialog->setParent (altair, Qt::Window);
    }

    if ((settings->value ("defaut").isValid())
            &&
            (!settings->value ("defaut").toString().isEmpty()))
        altair->setCurrentFile (settings->value ("defaut").toString());
    else
        {
            altair->setCurrentFile (projectName);
            settings->setValue ("defaut", projectName);
        }

    setCentralWidget (altair);

    altair->addActions (actionList);

    bottomDockWidget = new QDockWidget;
    bottomTabWidget = new QTabWidget;
    consoleDialog =  new QTextEdit;
    consoleDialog->setReadOnly (true);

    bottomTabWidget->addTab (altair->outputTextEdit, tr ("Messages"));
    bottomTabWidget->addTab (consoleDialog, tr ("Console"));
    bottomTabWidget->setCurrentIndex (0);

    fileTreeViewDockWidget = new QDockWidget;
    fileTreeViewDockWidget->setWidget (altair->fileTreeView);
    fileTreeViewDockWidget->setFeatures (QDockWidget::AllDockWidgetFeatures);
    addDockWidget (Qt::LeftDockWidgetArea, fileTreeViewDockWidget);

    managerDockWidget = new QDockWidget;
    managerDockWidget->setWidget (altair->managerWidget);
    managerDockWidget->setFeatures (QDockWidget::AllDockWidgetFeatures);
    addDockWidget (Qt::RightDockWidgetArea, managerDockWidget);

    Abstract::initH();

    createToolBars();

    bottomTabWidget->setCurrentIndex (0);
    double h = static_cast<double>(height) / 3.6;
    bottomTabWidget->setMinimumHeight (static_cast<int>(round(h)));

    QToolButton *clearBottomTabWidgetButton = new QToolButton;
    const QIcon clearOutputText = QIcon (QString::fromUtf8 (":/images/edit-clear.png"));
    clearBottomTabWidgetButton->setToolTip ("Effacer l'onglet courant de la console");
    clearBottomTabWidgetButton->setIcon (clearOutputText);

    QGroupBox *stackedBottomWidget = new QGroupBox;
    QHBoxLayout *stackedBottomWidgetLayout = new QHBoxLayout;
    QVBoxLayout *buttonsForBottomWidgetLayout = new QVBoxLayout;
    buttonsForBottomWidgetLayout->addWidget (clearBottomTabWidgetButton);

    QToolButton *nppBottomTabWidgetButton = new QToolButton;

    const QString &iconpath = ":/images/internet-web-browser.png";

    const QIcon nppIcon = QIcon (iconpath);
    nppBottomTabWidgetButton->setToolTip ("Afficher les messages dans l'explorateur internet");
    nppBottomTabWidgetButton->setIcon (nppIcon);
    buttonsForBottomWidgetLayout->addWidget (nppBottomTabWidgetButton);

    stackedBottomWidgetLayout->addLayout (buttonsForBottomWidgetLayout);
    stackedBottomWidgetLayout->addWidget (bottomTabWidget);
    stackedBottomWidget->setLayout (stackedBottomWidgetLayout);

    bottomDockWidget->setWidget (stackedBottomWidget);

    standardDisplay();

    addDockWidget (Qt::BottomDockWidgetArea, bottomDockWidget);

    setWindowIcon (QIcon (":/images/altair.png"));

    const QString versionPath = common::path_access("VERSION");
    QString version;
    if (! QFileInfo(versionPath).exists())
    {
        version = VERSION;
    }
    else
    {
        QFile versionFile(versionPath);
        versionFile.open(QIODevice::ReadOnly | QIODevice::Text);
        version = QString(versionFile.readAll());
        versionFile.close();
    }


    setWindowTitle ("Interface  Altaïr " + version);

    m = new MatriculeInput (width / 4, height / 6);

    connect(m, SIGNAL(updateProject(bool)), altair, SLOT(updateProject(bool)));
    connect(altair, SIGNAL(ajouterLigneMatricules()), m, SLOT(ajouterLigneMatricules()));

    Altair::RefreshFlag =  Altair::RefreshFlag  | interfaceStatus::parseXml;

    if (settings->value ("loadProjectBehavior") == true)
        altair->clearInterfaceAndParseProject();

    // resetting interfaceStatus::parseXml bits to 0
    Altair::RefreshFlag = Altair::RefreshFlag & (~interfaceStatus::parseXml);

    connect (nppBottomTabWidgetButton, &QToolButton::clicked, [this] { on_displayLogButton_clicked();});
    connect (clearBottomTabWidgetButton, &QToolButton::clicked, [this] { on_clearOutputTextButton_clicked();});
    connect (consoleDialog, SIGNAL (copyAvailable (bool)), consoleDialog, SLOT (copy()));
    connect (& (altair->process), SIGNAL (finished (int)), this, SLOT (resetCounter()));
    connect (& (altair->process), SIGNAL (finished (int)), this, SLOT (resetTableCheckBox()));

    if (projectName[0] != '\0')
        {
            // Paraît étrange... mais c'est pour éviter de lire deux fois le projet
            altair->closeProject();
            altair->projectName = projectName;
            altair->openProjectFileCommonCode();
        }

    // Mettre un lien symbolique dans le dossier xhl vers cdrom
    // Pour des raisons de dépendances cycliques il faut placer ceci à la fin et dans MainWindow.

    if (! settings->value ("importerAuLancement").isValid())
        settings->value ("importerAuLancement") = true;

    if (settings->value ("importerAuLancement") == true)
        {
            repaint();
            altair->importData();
        }
}

void MainWindow::on_displayLogButton_clicked(bool show, bool absolute_path)
{
    tempLog.setFileName (common::generateDatadirPath ("/log.html"));

    if (tempLog.exists()) tempLog.remove();
    tempLog.open (QIODevice::ReadWrite);
    
    QString s = qobject_cast<QTextEdit*> (bottomTabWidget->currentWidget())->toHtml();
    
    if (absolute_path)
    {
        s.replace (":", common::generateDatadirPath (""));                
    }
    else
    {
        s.replace (":/", "");                
    }
    
#   ifndef Q_OS_WIN
          s.replace ("<html>", "<html>\n<meta charset=\"UTF-8\" content=\"text/html\">");
          tempLog.write (s.toUtf8());
#   else
          s.replace ("<html>", "<html>\n<meta charset=\"ISO-8859-1\" content=\"text/html\">");
          tempLog.write (s.toLatin1());     
#   endif
    
    tempLog.close();
            
    if (show)  
    {
        QUrl url = QUrl::fromLocalFile (tempLog.fileName());
        QDesktopServices::openUrl (url);
    }
}

void MainWindow::on_clearOutputTextButton_clicked()
{
    qobject_cast<QTextEdit*> (bottomTabWidget->currentWidget())->clear();
}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i (recentFiles);

    while (i.hasNext())
        {
            if (!QFile::exists (i.next())) i.remove();
        }

    for (int j  = 0 ; j < MaxRecentFiles ; ++j)
        {
            if (j < recentFiles.count())
                {
                    QString  text = tr ("&%1 %2").arg (j + 1).arg (strippedName (recentFiles[j]));

                    recentFileActions[j]->setText (text);
                    recentFileActions[j]->setData (QVariant (recentFiles[j]));
                    recentFileActions[j]->setVisible (true);
                }
            else

                {
                    recentFileActions[j]->setVisible (false);
                }

        }

    separatorAction->setVisible (!recentFiles.isEmpty());
}

QString MainWindow::strippedName (const QString &fullFileName)
{
    return QFileInfo (fullFileName).fileName();
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu ("&Fichier");
    editMenu = menuBar()->addMenu ("&Editer");
    processMenu = menuBar()->addMenu ("&Lancer");
    optionsMenu = menuBar()->addMenu ("&Configurer");
    aboutMenu = menuBar()->addMenu ("&Aide");

    fileMenu->addActions ({newAction, openAction, saveAction,
                           saveAsAction, exportAction, archiveAction,
                           restoreAction, closeAction
                          });
    separatorAction = fileMenu->addSeparator();
    fileMenu->addActions (recentFileActions);
    fileMenu->addSeparator();
    fileMenu->addAction (exitAction);

    editMenu->addActions ({displayAction, displayOutputAction, displayFileTreeViewAction,
                           displayManagerAction, clearOutputTextAction, editProjectAction, printBaseAction
                          });

    processMenu->addActions ({RAction, lhxAction, cleanAction, anonymAction, openBaseDirAction});

    optionsMenu->addActions ({optionsAction, configureAction});

    aboutMenu->addActions ({helpAction, aboutAction, licenceAction});
}


void MainWindow::createActions()
{
    newAction  = new QAction (tr ("Nouveau projet .alt"), this);
    newAction ->setShortcut (QKeySequence ("Ctrl+N"));
    openAction = new QAction (tr ("&Ouvrir le projet .alt"), this);
    openAction->setShortcut (QKeySequence ("Ctrl+O"));
    newAction->setIcon (QIcon (":/images/project-open.png"));
    openAction->setIcon (QIcon (":/images/document-open-folder.png"));

    connect (newAction, SIGNAL (triggered()), altair, SLOT (on_newProjectButton_clicked()));
    connect (openAction, SIGNAL (triggered()), altair, SLOT (on_openProjectButton_clicked()));

    saveAction = new QAction (tr ("&Enregistrer"), this);
    saveAction->setShortcut (QKeySequence ("Ctrl+S"));
    saveAction->setIcon (QIcon (":/images/document-save.png"));
    connect (saveAction, &QAction::triggered, [this] { altair->updateProject (true); });

    saveAsAction = new QAction (tr ("En&registrer le projet comme..."), this);
    saveAsAction->setIcon (QIcon (":/images/document-save-as.png"));
    connect (saveAsAction, SIGNAL (triggered()), altair, SLOT (requestSaveProject()));

    exportAction = new QAction (tr ("E&xporter le projet vers..."), this);
    exportAction->setIcon (QIcon (":/images/export.png"));
    connect (exportAction, SIGNAL (triggered()), this, SLOT (exportProject()));

    archiveAction = new QAction (tr ("Archiver le projet vers..."), this);
    archiveAction->setIcon (QIcon (":/images/archive.png"));
    connect (archiveAction, SIGNAL (triggered()), this, SLOT (archiveProject()));

    restoreAction = new QAction (tr ("Désarchiver le projet"), this);
    restoreAction->setIcon (QIcon (":/images/restore.png"));
    connect (restoreAction, SIGNAL (triggered()), this, SLOT (restoreProject()));

    closeAction = new QAction (tr ("&Fermer le projet"), this);
    closeAction->setShortcut (QKeySequence ("Ctrl+W"));
    closeAction->setIcon (QIcon (":/images/document-close.png"));
    connect (closeAction, SIGNAL (triggered()), altair, SLOT (closeProject()));

    RAction = new QAction (tr ("&Lancer l'analyse des données"), this);
    RAction->setShortcut (QKeySequence ("Ctrl+R"));
    QIcon RActionIcon = QIcon (":/images/altair.png");

    RAction->setIcon (RActionIcon);

    connect (RAction, SIGNAL (triggered()), this, SLOT (createDialogs()));
    connect (RAction, SIGNAL (triggered()), altair, SLOT (runRAltair()));

    lhxAction = new QAction (tr ("Créer la base de données .csv"), this);
    lhxAction->setShortcut (QKeySequence ("Ctrl+B"));
    lhxAction->setIcon (QIcon (":/images/csv.png"));

    connect (lhxAction, SIGNAL (triggered()), this, SLOT (createDialogs()));
    connect (lhxAction, SIGNAL (triggered()), altair, SLOT (run()));

    cleanAction = new QAction (tr ("Nettoyer la base de paye"), this);
    cleanAction->setShortcut (QKeySequence ("Ctrl+$"));
    cleanAction->setIcon (QIcon (":/images/clean.png"));
    connect (cleanAction, SIGNAL (triggered()), this, SLOT (cleanBase()));

    anonymAction = new QAction (tr ("Anonymiser la base de données XML"), this);
    anonymAction->setIcon (QIcon (":/images/anonymiser.png"));
    connect (anonymAction, SIGNAL (triggered()), this, SLOT (anonymiser()));

    openBaseDirAction = new QAction (tr ("Ouvrir le répertoire des bases"), this);
    openBaseDirAction ->setIcon (QIcon (":/images/directory.png"));
    connect (openBaseDirAction, &QAction::triggered, [&]
    {

        QString useroutdir = common::path_access (DONNEES_SORTIE);

        if (! QFileInfo (useroutdir).isDir())
            {
                QDir().mkdir (useroutdir);
            }
        common::openDir (useroutdir);
    });

    optionsAction = new QAction (tr ("&Options"), this);
    optionsAction->setShortcut (QKeySequence ("Ctrl+T"));
    optionsAction->setIcon (QIcon (":/images/encode.png"));
    connect (optionsAction, SIGNAL (triggered()), this, SLOT (on_optionsButton_clicked()));

    configureAction = new QAction (tr ("&Configurer l'interface"), this);
    configureAction->setIcon (QIcon (":/images/configure-toolbars.png"));
    connect (configureAction, SIGNAL (triggered()), this, SLOT (configure()));

    helpAction = new QAction (tr ("&Aide"), this);
    helpAction->setShortcut (QKeySequence ("Ctrl+H"));
    helpAction->setIcon (QIcon (":/images/help-contents.png"));
    connect (helpAction, SIGNAL (triggered()), altair, SLOT (on_helpButton_clicked()));

    displayAction = new QAction (tr ("&Plein écran/Réduire"), this);
    displayAction->setIcon (QIcon (":/images/show-maximized.png"));
    connect (displayAction, SIGNAL (triggered()), this, SLOT (toggleFullScreen()));

    displayManagerAction = new QAction (tr ("Ouvrir/Fermer le &gestionnaire de projet"), this);
    const QIcon iconViewList = QIcon (QString::fromUtf8 (":/images/manager.png"));
    displayManagerAction->setIcon (iconViewList);
    connect (displayManagerAction, SIGNAL (triggered()), this, SLOT (on_openManagerWidgetButton_clicked()));

    editProjectAction = new QAction (tr ("Editer le projet courant"), this);
    editProjectAction->setShortcut (QKeySequence ("Ctrl+E"));
    editProjectAction->setIcon (QIcon (":/images/application-xml.png"));
    connect (editProjectAction, SIGNAL (triggered()), this, SLOT (on_editProjectButton_clicked()));

    displayOutputAction  = new QAction (tr ("Ouvrir/Fermer les messages"), this);
    const QIcon displayOutput = QIcon (QString::fromUtf8 (":/images/display-output.png"));
    displayOutputAction->setIcon (displayOutput);
    connect (displayOutputAction, &QAction::triggered,  [this] {bottomDockWidget->setVisible (!bottomDockWidget->isVisible());});

    displayFileTreeViewAction  = new QAction (tr ("Ouvrir/Fermer le gestionnaire de fichiers"), this);
    const QIcon displayFileTreeView = QIcon (QString::fromUtf8 (":/images/view-list-tree.png"));
    displayFileTreeViewAction->setIcon (displayFileTreeView);
    connect (displayFileTreeViewAction, SIGNAL (triggered()), this, SLOT (on_displayFileTreeViewButton_clicked()));

    clearOutputTextAction = new QAction (tr ("Effacer le log"), this);
    const QIcon clearOutputText = QIcon (QString::fromUtf8 (":/images/edit-clear.png"));
    clearOutputTextAction->setIcon (clearOutputText);
    connect (clearOutputTextAction, &QAction::triggered,  [this] {on_clearOutputTextButton_clicked();});

    exitAction = new QAction (tr ("&Quitter"), this);
    exitAction->setIcon (QIcon (":/images/application-exit.png"));
    exitAction->setShortcut (QKeySequence ("Ctrl+Q"));
    connect (exitAction, &QAction::triggered,  [this] {

     QMessageBox::StandardButton res = QMessageBox::warning(this,
                                                            "Protection de la confidentialité des données",
                                                            QString("Nettoyage des <b>Données</b> du répertoire <br>") + altair->userdatadir + "<br>Attention, toutes les données de ce répertoire seront effacées.<br>Appuyer sur <b>Non</b> pour annuler ce nettoyage ou sur <b>Ignorer</b> pour ne pas fermer l'application.",
                                                            QMessageBox::No | QMessageBox::Ignore | QMessageBox::Ok);
     switch (res)
     {
       case  QMessageBox::Ok :
         if (QDir(altair->userdatadir).removeRecursively()) {
             QMessageBox::about(this, "Nettoyage des données", "Les données ont été nettoyées.");
         }  else {
             QMessageBox::about(this, "Nettoyage des données", "Les données n'ont pas pu être nettoyées.");
         }

         break;
       case  QMessageBox::Ignore : return ;
       default : break;
     }
     emit (exitSignal());});

    aboutAction = new QAction (tr ("&Au sujet de"), this);
    aboutAction->setIcon (QIcon (":/images/about.png"));

    connect (aboutAction, &QAction::triggered,  []
    {
        QUrl url = QUrl::fromLocalFile (QCoreApplication::applicationDirPath() + "/../about.html") ;
        QDesktopServices::openUrl (url);

    });

    licenceAction = new QAction (tr ("Licence"), this);
    licenceAction->setIcon (QIcon (":/images/web/gplv3.png"));
    connect (licenceAction, &QAction::triggered,  []
    {
        QUrl url = QUrl::fromLocalFile (QCoreApplication::applicationDirPath() + "/../licence.html");
        QDesktopServices::openUrl (url);
    });
    printBaseAction = new QAction ("Extraire des bulletins", this);
    printBaseAction->setIcon (QIcon (":/images/print.png"));
    printBaseAction->setShortcut (QKeySequence ("Ctrl+P"));
    connect (printBaseAction, &QAction::triggered, [this] { on_printBase_clicked(); });

    for (int i = 0; i < MaxRecentFiles ; i++)
        {
            recentFileActions << new QAction (this);
            recentFileActions[i]->setVisible (false);
            connect (recentFileActions[i], SIGNAL (triggered()), altair, SLOT (openProjectFile()));
        }

    QAction* separator[6];

    for (int i = 0; i < 6; i++)
        {
            separator[i] = new QAction (this) ;
            separator[i]->setSeparator (true);
        }

    actionList << newAction << openAction << saveAction << saveAsAction << exportAction
               << archiveAction << restoreAction << closeAction << exitAction << separator[0]
               << RAction << lhxAction << cleanAction << anonymAction << openBaseDirAction
               << displayOutputAction << displayFileTreeViewAction << displayManagerAction <<  separator[4]
               << clearOutputTextAction <<  editProjectAction << printBaseAction << separator[3] << configureAction
               << optionsAction << helpAction << aboutAction ;

}

void MainWindow::createDialogs()
{
    if (! dialog)
    {
        dialog = new options(altair);
        dialog->setParent (this, Qt::Window);
    }

    if (! contentsWidget)
    {
      configureOptions();
    }

}

void MainWindow::on_printBase_clicked()
{
    m->checkDefaultFolder();
    m->exec();

    if (! dialog)
    {
        dialog = new options (altair);
        dialog->setParent (altair, Qt::Window);
    }

    if (! m->matricules.isEmpty())
        {
            dialog->standardTab->tableCheckBox->setChecked (false);
        }
    else
        dialog->standardTab->tableCheckBox->setChecked (true);

}

void MainWindow::resetTableCheckBox()
{
    if (dialog) dialog->standardTab->tableCheckBox->setChecked (true);
}

vector<string> MainWindow::extraire_donnees_protegees (const string& st)
{
    vector<string> out;
    const size_t taille = st.size();
    out.reserve (static_cast<size_t>(taille / 5));
    string::const_iterator iter = st.begin();

    size_t i = 0;
    const size_t pas = taille / 20;
    size_t k = 0;

    while (*iter != '<' && ++iter != st.end()) continue;

    if (* (iter + 1) == '?')
        {
            string::const_iterator iter1 = iter;

            while (++iter != st.end() && *iter != '>') continue;

            out.emplace_back (string (iter1, iter + 1) + "\n");
        }

start:

    while (iter != st.end())
        {
            while (++iter != st.end()
                    && *iter != '<')
                {
                    ++i;
                    ++k;
                    continue;
                };

            if (iter == st.end()) break;

            if (* (iter + 1) == '/') /* </TAG>  */
                {
                    string::const_iterator iter1 = iter;

                    while (++iter != st.end() && *iter != '>')
                        {
                            ++i;
                            ++k;
                            continue;
                        };

                    out.emplace_back (string (iter1, iter + 1) + "\n");

                    continue;
                }

            if (iter == st.end()) break;

            if (k >= pas)
                {
                    emit (altair->setProgressBar (static_cast<long>(i)));
                    qApp->processEvents();
                    k = 0;
                }

            string::const_iterator iter1 = iter;
            string tag;

            while (++iter != st.end())
                {
                    ++i;
                    ++k;

                    if (*iter == ' ')  /* <TAG ...> */
                        {
                            tag = string (iter1 + 1, iter);
                            break;
                        }

                    if (*iter == '>')  /* <TAG> */
                        {
                            out.emplace_back ("<" + string (iter1 + 1, iter) + ">\n");
                            goto start;
                        }
                }

            if (iter == st.end()) break;

            while (++iter != st.end())
                {
                    ++i;
                    ++k;

                    if (*iter == 'V')
                        {
                            while (++iter != st.end() && *iter == ' ')
                                {
                                    ++ i;
                                    ++k;
                                    continue;
                                };

                            if (*iter == '=')
                                {
                                    break; /*<TAG  V  =... /> */
                                }
                            else continue; /*<TAG  x ... /> */
                        }
                    else /* <TAG> */ /*<TAG  x... /> */
                        if (*iter == '>')
                            {
                                out.emplace_back (string (iter1, iter + 1) + "\n");
                                ++iter;
                                ++i;
                                ++k;
                                goto start;
                            }
                        else   /*<TAG  x... /> */
                            continue;
                }

            if (iter == st.end()) break;

            /*<TAG  V  =... /> */

            while (++iter != st.end())
                {
                    ++i;
                    ++k;

                    if (*iter == '\"') break;
                }

            if (iter == st.end()) break;

            iter1 = iter;

            while (++iter != st.end())
                {
                    ++i;
                    ++k;

                    if (*iter == '\"') break;
                }

            if (iter == st.end()) break;

            string value = string (iter1 + 1, iter);
            auto iter2 = iter;

            while (++iter != st.end())
                {
                    ++i;
                    ++k;

                    if (*iter == '>') break;
                }

            if (iter == st.end()) break;

            string reste = string (iter2 + 1, iter + 1);

            const array<string, 19> Tags =
            {
                "Civilite",
                "Nom",
                "Prenom",
                "Adr1",
                "Adr2",
                "Ville",
                "CP",
                "CodeEtab",
                "CodeGuic",
                "IdCpte",
                "CleRib",
                "LibBanc",
                "TitCpte",
                "DteBanc",
                "Description",
                "Service",
                "NumUrssaf",
                "Siret",
                "NIR"
            };

            for (const string& t : Tags)
                {
                    if (tag == t)
                        {
                            const string& remplacement = tag == "NIR" ? "<NIR  V = \"" + value.substr (0, 5) + "Z\"" + reste + "\n"
                                                                      : (tag != "Service" ? "<" + tag + " V = \"Z\"" + reste + "\n" 
                                                                                          : (value.substr(0, 3) != "ELU" ? "<" + tag + " V = \"Z\"" + reste + "\n" 
                                                                                                                         : "<" + tag + " V = \"ELU\"" + reste + "\n"));

                            out.emplace_back (remplacement);

                            ++iter;
                            ++i;
                            ++k;

                            goto start;
                        }
                }

            out.emplace_back ("<" + tag + " V = \"" + value + "\"" + reste + "\n");

        }

    return (out);
}

void MainWindow::launch_process (const QString& path)
{
    QFile xml (path);

    std::string xml_out (path.toStdString() + ".new");
    std::ofstream out;

    if (!xml.open (QIODevice::ReadOnly | QIODevice::Text))
        {
            Q ("Erreur de lancement du processus")
            return;
        }

    out.open (xml_out, std::ofstream::out | std::ofstream::trunc);

    if (! out.is_open())
        {
            Q ("Erreur de lancement du processus")
            return;
        }

    altair->textAppend (PROCESSING_HTML_TAG "Lecture du fichier...");
    emit (altair->showProgressBar());
    emit (altair->setProgressBar (0));
    altair->outputTextEdit->repaint();
    QString xml_mod = QString (xml.readAll());

    altair->textAppend (PROCESSING_HTML_TAG "Remplacement des informations protégées. Patientez...");
    altair->outputTextEdit->repaint();

    int bar_range = xml_mod.size();

    emit (altair->setProgressBar (0, bar_range));

    const vector<string>& v = extraire_donnees_protegees (xml_mod.toStdString());

    for (const string &s : v)
        out << s;

    out.close();

    emit (altair->setProgressBar (0));

    xml.close();
    xml.remove();
    rename (xml_out.c_str(), path.toStdString().c_str());

    altair->textAppend (PROCESSING_HTML_TAG  "Anonymisation de " + path + " terminée.");
    altair->outputTextEdit->repaint();
    emit (altair->setProgressBar (0));

    // Il est souhaitable d'actualiser le projet avant de lancer v() car en cas de non actualisation récente la valeur de la case
    // peut être en décalage avec la réalité. C'est au cours de ces actualisations que la valeur est enregistrée dans une table de hashage.

    altair->updateProject (true);
}

const vector <unsigned char>  MainWindow::nettoyer_donnees (vector <unsigned char>& st)
{
    vector <unsigned char> out;
    const size_t taille = st.size();

    // Découper le fichier en 5
    out.reserve ( taille / 5);
    vector <unsigned char>::const_iterator iter = st.begin();
    vector <unsigned char>::const_iterator iter2;
    size_t i = 0;
    size_t k = 0;

    // Découper le fichier en pas de 1/20 ème de la taille
    const size_t pas = taille / 20;
    bool quote = false; // drapeau d'indentification d'une chaine

loop :

    while (iter != st.end())
        {
            switch (*iter)
                {
                case  0x22 :    // Repérer "
                    quote = ! quote; // On a identifié un "
                    out.push_back (0x22); // L'empiler
                    ++iter;
                    break;

                case  0x26  :   // Repérer &
                    if (! quote)  // En dehors d'une chaine
                        {
                            ++iter;  // sauter ce caractère illégal
                            goto loop;
                        }

                    iter2 = iter; // A l'intérieur d'une chaine

                    // On essaie de repérer les séquences html qui sont illicites sous libxml2 : &accute; par exemple
                    // Elles sont dénuées d'espace, commencent par & et terminent par ;

                    while (++iter2 != st.end())
                        {
                            switch (*iter2)
                                {
                                case  0x22  : // Repérer la fin de la chaine : "
                                    quote = ! quote; // Réinitialiser le drapeau d'indentification d'une chaine
                                    [[fallthrough]];

                                case  0x20  :  // Repérer espace  ' '
                                    out.emplace_back (*iter2); // l'empiler : on est sorti de la séquence
                                    iter = iter2 + 1;  // Fin de la séquence et retour à la boucle principale
                                    goto loop;

                                case  0x3b  :  // SEMICOLON  ';'
                                    iter = iter2 + 1; // Fin de la séquence et retour à la boucle principale
                                    goto loop;

                                default :

                                    continue;
                                }
                        }

                    continue;

                case 0x3e :  //  Repérer fin balise '>'
                    if (quote)
                        {
                            out.push_back (0x3e);
                            ++iter;
                            break;
                        }

                    out.push_back (0x3e);
                    out.push_back ('\n');

                    while (*++iter != 0x3c  //  '<'
                            && iter != st.end())  ;

                    break;

                default :

                    // Cas général
                    // Empiler tous les caractères imprimables
                    // La locale au sens de C++ est principe "C" même si la locate Qt est différente
                    // Donc isprint est false sur l'étendue des caractères accentués
                    if (isprint (*iter)) out.emplace_back (*iter);
                    else
                        switch (*iter)
                            {
                            // Pour les caractères non
                            // Latin-1
                            case 0xe8 : // è
                            case 0xe9 : // é
                            case 0xea : // ê
                            case 0xc8 : // E accent aigu
                            case 0xc9 : // E accent grave
                            case 0xca : // E accent circ
                            case 0xee : // î
                            case 0xce : // I accent circ
                            case 0xd4 :  // ô
                            case 0xf4 :  // O accent circ
                                out.emplace_back (*iter); // Empiler les caractères accentués (1 octet en Latin-1)
                                break;

                            // UTF-8
                            case 0xc3 :
                                switch (* (iter + 1))
                                    {
                                    case 0xa8 : // è
                                    case 0xa9 : // é
                                    case 0xaa : // ê
                                    case 0x88 : // E accent aigu
                                    case 0x89 : // E accent grave
                                    case 0x8a : // E accent circ
                                    case 0xae : // î
                                    case 0x8e : // I accent circ
                                    case 0xb4 :  // ô
                                    case 0x94 :  // O accent circ
                                        out.emplace_back (*iter);
                                        out.emplace_back (*++iter); // Empiler les caractères accentués (2 octets en UTF-8)
                                        break;

                                    default :
                                        break;
                                    }

                                break;

                            default :
                                out.push_back (0x20); // Sinon par défaut remplacer par une espace tout caractère non imprimable non accentué (SP ' ')
                                break;

                            }

                    ++iter;
                    break;
                }

            ++i;
            ++k;

            // A chaque pas de 1/20 de la taille émettre un signal à la barre de progression et forcer l'interface à se raffraichir
            if (k >= pas)
                {
                    emit (altair->setProgressBar (i));
                    qApp->processEvents();
                    k = 0;
                }
        }

    return (out);
}

void MainWindow::clean_process (const QString& path)
{
    ifstream xml;
    xml.open (path.toStdString(), ios_base::binary);

    if (! xml.is_open())
        {
            Q ("Erreur de lancement du processus")
            return;
        }

    const QString &xml_out = path + ".new";

    ofstream out;

    out.open (xml_out.toStdString(), std::ios_base::binary);

    if (! out.is_open())
        {
            Q ("Erreur de lancement du processus")
            return;
        }

    altair->textAppend (PROCESSING_HTML_TAG "Lecture du fichier...");
    emit (altair->showProgressBar());
    emit (altair->setProgressBar (0));
    altair->outputTextEdit->repaint();

    vector <unsigned char> xml_mod;
    long bar_range = (long) common::getFileSize (path);
    xml_mod.reserve (bar_range);
    xml_mod.insert (
        xml_mod.end(),
        std::istreambuf_iterator<char> (xml),
        std::istreambuf_iterator<char>());

    altair->outputTextEdit->repaint();

    emit (altair->setProgressBar (0, bar_range));

    for (unsigned char c : nettoyer_donnees (xml_mod))
        out << std::move (c);

    out.close();

    emit (altair->setProgressBar (0));

    xml.close();
    QFile f (path);
    f.remove();
    f.rename (xml_out, path);

    if (QFileInfo (f).exists())
        {
            altair->textAppend (PROCESSING_HTML_TAG  "Nettoyage de " + path + " terminé.");
            altair->outputTextEdit->repaint();
            altair->updateProject (true);
        }
    else
        {
            altair->textAppend (ERROR_HTML_TAG  "Le nettoyage de " + path + " a échoué.");
            altair->outputTextEdit->repaint();
            altair->updateProject (true);
        }
}

void MainWindow::anonymiser()
{
    QStringList args;

    altair->updateProject (true);

    args << altair->createCommandLineString();

    altair->textAppend (PROCESSING_HTML_TAG + tr ("Anonymisation des bases de paye ("));

    for (const QString& s : args)
        {
            if (! s.isEmpty() && QFileInfo (s).isFile())
                {
                    altair->textAppend (PROCESSING_HTML_TAG "Lancement de l'anonymisation du fichier " + s + ". Patientez...");
                    altair->outputTextEdit->repaint();
                    launch_process (s);
                }
        }
}

void MainWindow::cleanBase()
{
    QStringList args;

    altair->updateProject (true);

    args << altair->createCommandLineString();

    altair->textAppend (PROCESSING_HTML_TAG + tr ("Nettoyage des bases de paye..."));

    for (const QString& s : args)
        {
            if (! s.isEmpty() && QFileInfo (s).isFile())
                {
                    altair->textAppend (PROCESSING_HTML_TAG "Nettoyage du fichier " + s + ". Patientez...");
                    altair->outputTextEdit->repaint();
                    clean_process (s);
                }
        }

    emit (altair->setProgressBar (0, 100));
    emit (altair->setProgressBar (100));
}

void MainWindow::configure()
{
    if (! contentsWidget) configureOptions();

    contentsWidget->setVisible (true);
    contentsWidget->raise();
}

void MainWindow::on_optionsButton_clicked()
{
    dialog->setVisible (!dialog->isVisible());
    dialog->raise();
}

void MainWindow::on_displayFileTreeViewButton_clicked (bool isHidden)
{
    fileTreeViewDockWidget->setVisible (isHidden);
    altair->project->importFromMainTree->setVisible (isHidden);
}

void MainWindow::on_displayFileTreeViewButton_clicked()
{
    on_displayFileTreeViewButton_clicked (fileTreeViewDockWidget->isHidden());
}

void MainWindow::on_openManagerWidgetButton_clicked (bool isHidden)
{
    managerDockWidget->setVisible (isHidden);
}

void MainWindow::on_openManagerWidgetButton_clicked()
{
    on_openManagerWidgetButton_clicked (managerDockWidget->isHidden());
}

void MainWindow::createToolBars()
{
#define buildToolBar(bar, text) \
    bar = addToolBar(tr(text));\
    bar->setIconSize(QSize(48, 48));

    buildToolBar (fileToolBar, "&File")

    buildToolBar (editToolBar, "&Edit")
    buildToolBar (processToolBar, "&Process")
    buildToolBar (optionsToolBar, "&Data")
    buildToolBar (aboutToolBar, "&Help")

    fileToolBar->addActions ({newAction, saveAsAction, exportAction, archiveAction, restoreAction, closeAction, exitAction});
    fileToolBar->addSeparator();

    editToolBar->addActions ({displayAction, displayOutputAction, displayFileTreeViewAction});

    processToolBar->addActions ({RAction, lhxAction, cleanAction, openBaseDirAction});

    optionsToolBar->addActions ({optionsAction, configureAction});

    aboutToolBar->addActions ({helpAction, aboutAction});
    aboutToolBar->hide();
}

void MainWindow::on_editProjectButton_clicked()
{
    if (altair->projectName.isEmpty()) return;

    editWidget = new QMainWindow (this);
    editWidget->setWindowTitle (tr ("Edition du projet ") + altair->projectName.left (8) + "..." + altair->projectName.right (12));
    QMenu *fileMenu = new QMenu (tr ("&Fichier"), this);
    editWidget->menuBar()->addMenu (fileMenu);

    const char* keys[] = {"Nouveau", "Ouvrir", "Enregistrer", "Enregistrer comme...", "Actualiser", "Enregistrer et quitter", "Quitter"};
    const char* seq[] = {"Ctrl+N", "Ctrl+O", "Ctrl+S", "Ctrl+T", "Ctrl+R", "Ctrl+E", "Ctrl+Q"};
    int j = 0;

    for (const char* k :  keys)
        {
            actionHash[k] = new QAction (tr (k), this);
            fileMenu->addAction (actionHash[k]);
            actionHash[k]->setShortcut (QKeySequence (seq[j++]));
        }

    QFont font;
    font.setFamily ("Courier");
    font.setFixedPitch (true);
    font.setPointSize (fontsize);

    editor = new QTextEdit;
    editor->setFont (font);

    highlighter = new Highlighter (editor->document());

    if (altair->projectName.isEmpty()) return;

    projectFile.setFileName (altair->projectName);

    if (projectFile.open (QFile::ReadWrite | QFile::Text))
        {
            editor->setPlainText (projectFile.readAll());
            projectFile.close();
        }

    // do not capture file by reference!
    connect (actionHash["Nouveau"],
             &QAction::triggered,
             [this] { altair->on_newProjectButton_clicked(); });

    connect (actionHash["Ouvrir"],
             &QAction::triggered,
             [this]
    {
        altair->on_openProjectButton_clicked() ;
        editWidget->~QMainWindow();
        on_editProjectButton_clicked();
    });

    connect (actionHash["Enregistrer"],
             &QAction::triggered,
             [this]
    {
        projectFile.open (QFile::Truncate | QFile::WriteOnly | QFile::Text);
        projectFile.write (editor->document()->toPlainText().toUtf8()) ;
        projectFile.close();
        altair->closeProject();
        altair->RefreshFlag = altair->RefreshFlag | interfaceStatus::parseXml;
        altair->clearInterfaceAndParseProject();
        // resetting interfaceStatus::parseXml bits to 0
        altair->RefreshFlag = altair->RefreshFlag & (~interfaceStatus::parseXml);
    });

    connect (actionHash["Enregistrer comme..."],
             &QAction::triggered,
             [this] {saveProjectAs();});

    connect (actionHash["Actualiser"],
             &QAction::triggered,
             [this]
    {
        altair->updateProject (true);

        if (projectFile.open (QFile::ReadWrite |  QFile::Text))
            {
                editor->clear();
                editor->setPlainText (projectFile.readAll());
                projectFile.close();
            }
    });

    connect (actionHash["Enregistrer et quitter"],
             &QAction::triggered,
             [this]
    {
        actionHash["Enregistrer"]->trigger();
        actionHash["Quitter"]->trigger();
    });

    connect (actionHash["Quitter"],
             &QAction::triggered,
             [this]
    {
        editWidget->~QMainWindow() ;
    });

    editWidget->setCentralWidget (editor);
    editWidget->setGeometry (200, 200, 600, 800);
    editWidget->show();
}


void MainWindow::saveProjectAs (const QString &newstr)
{

    if (QFileInfo (newstr).isFile())
        {
            QMessageBox::StandardButton result = QMessageBox::warning (nullptr, "Ecraser le fichier ?", "Le projet " + newstr + " va être écrasé.\nAppuyer sur Oui pour confirmer.",
                                                 QMessageBox::Ok | QMessageBox::Cancel);


            if (result != QMessageBox::Ok)
                {
                    return;
                }

            std::remove (newstr.toStdString().c_str());
        }

    if (editor)
        {
            QFile file (newstr);

            if (file.open (QFile::WriteOnly | QFile::Truncate | QFile::Text))
                {
                    file.write (editor->document()->toPlainText().toUtf8()) ;
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

    altair->setCurrentFile (newstr);
    // attention dans cet ordre !

}

void MainWindow::saveProjectAs()
{
    QString newstr = QFileDialog::getSaveFileName (this, tr ("Enregistrer le projet comme..."), altair->userdatadir, tr ("projet altair (*.alt)"));

    if (newstr.isEmpty())
        return;
    else
        saveProjectAs (newstr);

}



bool MainWindow::exportProject (QString dirPath)
{
    if (Hash::wrapper.isEmpty()) return false;

    if (Hash::wrapper["XHL"] == nullptr || Hash::wrapper["XHL"]->isEmpty()) return false;

    Abstract::initH ("base", common::path_access(DONNEES_SORTIE)); // Nécessaire pour v(base)
    const QString projectRootDir = QDir::toNativeSeparators (QDir::cleanPath (v(base)) + "/");
    
    QString dirStr = dirPath;    
    
    if (dirStr.isEmpty())
        dirStr = QFileDialog::getExistingDirectory (this, tr ("Exporter le rapport vers le répertoire..."), projectRootDir);

    if (! QFileInfo (dirStr).isDir()) return false;

    on_displayLogButton_clicked(false, false);
    
    subDirStr = QDir::toNativeSeparators (dirStr.append ("/Altaïr/" +
                        QDate::currentDate().toString ("dd MM yyyy")
                        + "-" + QTime::currentTime().toString ("hh mm ss")) + "/");
    
    QDir().mkpath (subDirStr);

    if (QMessageBox::Ok != QMessageBox::warning (nullptr, "", "Les résultats seront exportés vers le dossier <br>" + subDirStr,  QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok))
        return false;

    altair->textAppend (PROCESSING_HTML_TAG "Exportation en cours. Patientez...");
    altair->outputTextEdit->repaint();
   
    const QString docxReportFilePath = projectRootDir + "altaïr.docx";
    const QString odtReportFilePath = projectRootDir  + "altaïr.odt";
    const QString pdfReportFilePath = projectRootDir  + "altaïr.pdf";
    QDir dir (v(base));
    QStringList tableList = dir.entryList (QStringList ("Table*.csv"), QDir::Files);

    const QString bulletinsFilePath = v(base)  +  QDir::separator() + "Bulletins.csv";

    emit (altair->showProgressBar());

    int dimension_paye = 0;

    if (v(exportXML).isTrue() || v(exportAll).isTrue())
        {
            for (int rank = 0; rank < Hash::wrapper["XHL"]->size() - 3; ++rank)
                {
                    dimension_paye += Hash::wrapper["XHL"]->at (rank).size();
                }
        }

    altair->setProgressBar (0, 6 + (v(exportTable).isTrue() || v(exportAll).isTrue()) * (1 + tableList.size()) + dimension_paye);

    bool result = true;

    result = common::copyFile(tempLog.fileName(), subDirStr   + "log.html", "Le log", REQUIRE); 
    if (result)
       result = common::copyDir(common::generateDatadirPath("/images"), subDirStr   + "images"); 
    
    result &= common::copyFile (docxReportFilePath, subDirStr  + "altaïr.docx", "Le rapport Altaïr Word", REQUIRE);

    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été exporté sous : " + subDirStr);
            altair->setProgressBar (2);
        }

    result &= common::copyFile (odtReportFilePath, subDirStr  + "altaïr.odt", "Le rapport Altaïr Open Office", REQUIRE);

    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Open Office a été exporté sous : " + subDirStr);
            altair->setProgressBar (3);
        }

    result &= common::copyFile (pdfReportFilePath, subDirStr  + "altaïr.pdf", "Le rapport Altaïr PDF", REQUIRE);

    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été exporté sous : " + subDirStr);
            altair->setProgressBar (4);
        }

    int bar = 4;

    if (v(exportTable).isTrue() || v(exportAll).isTrue())
        {
            for (const QString &st : tableList)
                {
                    result = common::copyFile (v(base) + QDir::separator() + st, subDirStr + st, "La base des lignes de paye", REQUIRE);

                    if (result)
                        {
                            altair->textAppend (PARAMETER_HTML_TAG  "La base " + v(base) + QDir::separator() + st + " a été exportée sous : " + subDirStr);
                            altair->setProgressBar (++bar);
                        }
                    else
                        altair->textAppend (ERROR_HTML_TAG  "La base " + v(base) + QDir::separator() + st + " n'a pas pu être exportée sous : " + subDirStr);

                    altair->outputTextEdit->repaint();
                }

            result = common::copyFile (bulletinsFilePath, subDirStr  + "Bulletins.csv", "La base des lignes de paye a été exportée sous : ", REQUIRE);

            if (result)
                {
                    altair->textAppend (PARAMETER_HTML_TAG  "La base des bulletins de paye a été exportée sous : " + subDirStr);
                    altair->setProgressBar (++bar);
                }
            else
                altair->textAppend (ERROR_HTML_TAG  "La base des bulletins de paye n'a pas pu être exportée sous : " + subDirStr);
        }

    if (v(exportXML).isTrue() || v(exportAll).isTrue())
        {
            for (int rank = 0; rank < Hash::wrapper["XHL"]->size() - 3; ++rank)
                {
                    for (const QString &s :  Hash::wrapper["XHL"]->at (rank))
                        {
                            result = common::copyFile (s, subDirStr + tools::getEmbeddedPath(s), "La base XML " + s, REQUIRE);

                            if (result)
                                {
                                    altair->textAppend (PARAMETER_HTML_TAG  "La base " +  s + " a été exportée sous : " + subDirStr);
                                    altair->setProgressBar (++bar);
                                }
                            else
                                altair->textAppend (ERROR_HTML_TAG  "La base " +  s + " n'a pas pu être exportée sous : " + subDirStr);

                            altair->outputTextEdit->repaint();
                        }
                }
        }

    result &= common::copyDir (projectRootDir + "Docs", subDirStr + "Docs");
    result &= common::copyDir (projectRootDir + "Bases", subDirStr + "Bases");

    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Les bases en lien ont été exportées sous : " + subDirStr + "Bases");
        }
    else
        altair->textAppend (ERROR_HTML_TAG  "Les bases en lien n'ont pas pu être exportées sous : " + subDirStr + "Bases");

    QFile (altair->projectName).close();
    
    result &= common::copyFile (altair->projectName, subDirStr  + "projet.alt", "Le projet .alt", false);
   
    altair->setProgressBar (bar + 2);

    altair->textAppend (STATE_HTML_TAG "Exportation terminée.");

    return result;
}

bool MainWindow::archiveProject()
{
    Abstract::initH ("base", common::path_access (DONNEES_SORTIE)); // Nécessaire pour v(base)
    const QString projectRootDir = QDir::toNativeSeparators (QDir::cleanPath (v(base)) + "/");
    
    QString dirName = QFileDialog::getExistingDirectory (this, tr ("Archiver le rapport vers le paquet..."),
                      projectRootDir,
                      QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog);


    subDirStr = QDir::toNativeSeparators (dirName.append ("/Archives Altaïr/" +
                        QDate::currentDate().toString ("dd MM yyyy")
                        + "-" + QTime::currentTime().toString ("hh mm ss")) + "/");

    if (QMessageBox::Ok != QMessageBox::warning (nullptr, "", "Les résultats seront archivés dans le paquet <br>" + subDirStr.chopped(1) + ".arch",  QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok))
        return false;
    
    QDir().mkpath (subDirStr);
    
    altair->textAppend (PROCESSING_HTML_TAG "Archivage en cours. Patientez...");
    altair->outputTextEdit->repaint();

    on_displayLogButton_clicked(false, false);
   
    const QString docxReportFilePath = projectRootDir  + "altaïr.docx";
    const QString pdfReportFilePath = projectRootDir   + "altaïr.pdf";
    const QString odtReportFilePath = projectRootDir   + "altaïr.odt";
    QDir dir (v(base));
    QStringList tableList = dir.entryList (QStringList ("Table*.csv"), QDir::Files);

    const QString bulletinsFilePath = v(base)  +  QDir::separator() + "Bulletins.csv";

    bool result = true;
    emit (altair->showProgressBar());

    int dimension_paye = 0;

    if (v(archiveXML).isTrue() || v(archiveAll).isTrue())
        {
            for (int rank = 0; rank < Hash::wrapper["XHL"]->size() - 3; ++rank)
                {
                    dimension_paye += Hash::wrapper["XHL"]->at (rank).size();
                }
        }
    int max = 6 + (v(archiveTable).isTrue() || v(archiveAll).isTrue()) * (1 + tableList.size()) + dimension_paye;
    altair->setProgressBar (0, max);

    result = common::zip(tempLog.fileName(), subDirStr + "log.html.arch");
    
    result &= common::zipDir(common::generateDatadirPath("/images"), subDirStr  + "images");
            
    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le log a été archivé sous : " + subDirStr);
            altair->setProgressBar (1);
        }
    
    result &= common::zip (docxReportFilePath, subDirStr + "altaïr.docx.arch");

    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été archivé sous : " + subDirStr);
            altair->setProgressBar (2);
        }

    result &= common::zip (odtReportFilePath, subDirStr  + "altaïr.odt.arch");

    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Open Office a été archivé sous : " + subDirStr);
            altair->setProgressBar (3);
        }

    result &= common::zip (pdfReportFilePath, subDirStr  + "altaïr.pdf.arch");

    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été archivé sous : " + subDirStr);
            altair->setProgressBar (4);
        }

    int bar = 4;

    if (v(archiveTable).isTrue() || v(archiveAll).isTrue())
        {

            for (const QString &st : tableList)
                {
                    result = common::zip (v(base) + QDir::separator() + st, subDirStr + tools::getEmbeddedPath(st) + ".arch");

                    if (result)
                        {
                            altair->textAppend (PARAMETER_HTML_TAG  "La base " + v(base) + QDir::separator() + st + " a été archivée sous : " + subDirStr);
                            altair->setProgressBar (++bar);
                        }
                    else
                        altair->textAppend (ERROR_HTML_TAG  "La base " + v(base) + QDir::separator() + st + " n'a pas pu être archivée sous : " + subDirStr);

                    altair->outputTextEdit->repaint();
                }

            result = common::zip (bulletinsFilePath, subDirStr + "Bulletins.csv.arch");

            if (result)
                {
                    altair->textAppend (PARAMETER_HTML_TAG  "La base des bulletins de paye a été archivée sous : " + subDirStr);
                    altair->setProgressBar (++bar);
                }

            else
                altair->textAppend (ERROR_HTML_TAG  "La base des bulletins de paye n'a pas pu être archivée sous : " + subDirStr);
        }

    if (v(archiveXML).isTrue() || v(archiveAll).isTrue())
        {
            for (int rank = 0; rank < Hash::wrapper["XHL"]->size() - 3; ++rank)
                {
                    for (const QString &s :  Hash::wrapper["XHL"]->at (rank))
                        {
                            result = common::zip (s, subDirStr + tools::getEmbeddedPath(s) + ".arch");

                            if (result)
                                {
                                    altair->textAppend (PARAMETER_HTML_TAG  "La base " +  s + " a été archivée sous : " + subDirStr);
                                    altair->setProgressBar (++bar);
                                }
                            else
                                altair->textAppend (ERROR_HTML_TAG  "La base " +  s + " n'a pas pu être archivée sous : " + subDirStr);

                            altair->outputTextEdit->repaint();
                        }
                }
        }

    result = common::zipDir (projectRootDir    + "Docs", subDirStr + "Docs");
    result &= common::zipDir (projectRootDir  + "Bases", subDirStr + "Bases");

    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Les bases en lien ont été archivées sous : " + subDirStr + "Bases");
        }
    else
        altair->textAppend (ERROR_HTML_TAG  "Les bases en lien n'ont pas pu être archivées sous : " + subDirStr + "Bases");
    QFile (altair->projectName).close();
    saveProjectAs (subDirStr + "projet.alt");
    
#ifdef Q_OS_UNIX
    process.start ("tar", QStringList() << "-cf" << subDirStr.chopped(1) + ".arch" <<  "-C" << subDirStr << ".");
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(tarFinished()));
#endif    
  
    altair->setProgressBar (max);
    altair->textAppend (STATE_HTML_TAG "Archivage terminé.");
    return result;
}

void MainWindow::tarFinished() 
{
    if (QFileInfo(subDirStr).isDir())
        QDir(subDirStr).removeRecursively();
}


bool MainWindow::restoreProject (QString archfile)
{
    Abstract::initH ("base", common::path_access (DONNEES_SORTIE)); // Nécessaire pour v(base)
    const QString projectRootDir = QDir::toNativeSeparators (QDir::cleanPath (v(base)) + "/");
    
    subDirStr = archfile;
    if (subDirStr.isEmpty())
        subDirStr = QFileDialog::getOpenFileName(this, tr ("Restaurer le rapport depuis le paquet..."), projectRootDir);

    if (! QFileInfo (subDirStr).isFile()) return false;

    if (QMessageBox::Ok != QMessageBox::warning (nullptr, "", "Les résultats seront restaurés depuis le paquet <br>" + subDirStr,  QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok))
        return false;
       
#ifdef Q_OS_UNIX
    process.setWorkingDirectory(QFileInfo(subDirStr).absolutePath());
    process.start ("tar", QStringList() << "-xf" << subDirStr << "-C" << projectRootDir);
    process.waitForFinished();
    
#endif    

    QDir dir (projectRootDir);
    QStringList tableList = dir.entryList (QStringList () << "Table*.csv.arch" << "Bulletins*.csv.arch", QDir::Files);
    QStringList rapportList = dir.entryList (QStringList () << "*.odt.arch" << "*.pdf.arch" << "*.docx.arch" << "*.html.arch", QDir::Files);
    QDir dir_images (projectRootDir + "images");
    QStringList imagesList = dir_images.entryList (QStringList () << "*.png.arch" << "*.jpg.arch" << "*.ico.arch" << "*.svg.arch", QDir::Files);
      
    bool result = true;
    emit (altair->showProgressBar());
    int dimension_paye = 0;

    if (v(archiveXML).isTrue() || v(archiveAll).isTrue())
        {
            for (int rank = 0; rank < Hash::wrapper["XHL"]->size() - 3; ++rank)
                {
                    dimension_paye += Hash::wrapper["XHL"]->at (rank).size();
                }
        }
    
    int max = 6 + (v(archiveTable).isTrue() || v(archiveAll).isTrue()) * (1 + tableList.size()) + dimension_paye;
    altair->setProgressBar (0, max);

    altair->textAppend (PROCESSING_HTML_TAG "Restauration en cours. Patientez...");
    altair->outputTextEdit->repaint();
   
    result &= common::unzip(projectRootDir, rapportList);
    result &= common::unzip(dir_images.absolutePath(), imagesList);
   
    if (result) altair->textAppend(PARAMETER_HTML_TAG "Extraction des données... fait.");
                                   
    if (QFileInfo("log.html").exists())
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le log a été désarchivé sous : " + projectRootDir);
            altair->setProgressBar (1);
        }
    
    if (QFileInfo("altaïr.docx").exists())
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été décompressé sous : " + projectRootDir);
            altair->setProgressBar (2);
        }

    if (QFileInfo("altaïr.odt").exists())
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr ODT a été décompressé sous : " + projectRootDir);
            altair->setProgressBar (3);
        }

    if (QFileInfo("altaïr.pdf").exists())
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été décompressé sous : " + projectRootDir);
            altair->setProgressBar (4);
        }

    int bar = 5;
    
    if (v(archiveTable).isTrue() || v(archiveAll).isTrue())
    {
        result = common::unzip(projectRootDir, tableList);
        for (auto& f: tableList) result &= QFileInfo(f.chopped(5)).exists();
        if (result)    
            {
                altair->textAppend (PARAMETER_HTML_TAG  "La base des bulletins de paye a été décompressée sous : " + projectRootDir);
                altair->setProgressBar (++bar);
            }
        else
            altair->textAppend (ERROR_HTML_TAG  "La base des bulletins de paye n'a pas décompressée sous : " + projectRootDir);
    }
    else 
           for (auto& f: tableList) QFile(f).remove();
             
   if (v(archiveXML).isTrue() || v(archiveAll).isTrue())
    {
       result = common::unzipDir(projectRootDir, QStringList() << "*.xhl.arch" << "*.XHL.arch" << "*.xml.arch" << "*.XML.arch");
   
       if (result)
            {
                altair->textAppend (PARAMETER_HTML_TAG  "Les bases XML/XHL ont été décompressées sous : " + projectRootDir);
                altair->setProgressBar (0.8 * max);
            }
       else
            altair->textAppend (ERROR_HTML_TAG  "Les bases XML/XHL n'ont pas été décompressées sous : " + projectRootDir);

      altair->outputTextEdit->repaint();
    }
   //else
     //      QDir("home").removeRecursively();
      
    result = common::unzipDir (projectRootDir + "Docs");
    result &= common::unzipDir (projectRootDir + "Bases");
 
    QDir::setCurrent(projectRootDir);
    if (result && QFileInfo("Bases").isDir() && QFileInfo("Docs").isDir())
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Les bases en lien ont été décompressées sous : " + projectRootDir + "Bases");
        }
    else
        altair->textAppend (ERROR_HTML_TAG  "Les bases en lien n'ont pas été décompressées sous : " + projectRootDir + "Bases");

    // nettoyage
    tools::cleanDir(projectRootDir, {"*.arch", "*.tmp"}, QFileInfo(subDirStr).absolutePath());
        
    altair->setProgressBar (max);
    altair->textAppend (STATE_HTML_TAG "Désarchivage terminé.");
    return result;
}

void MainWindow::configureOptions()
{
    /* plain old data types must be 0-initialised even though the class instance was new-initialised. */

    if (!contentsWidget) contentsWidget = new QDialog (this);
    contentsWidget->setVisible (false);

    QGroupBox *displayGroupBox = new QGroupBox (tr ("Affichage"));
    QGroupBox *displayToolBarsGroupBox = new QGroupBox (tr ("Barres d'outils"));

    closeButton = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    closeButton->button (QDialogButtonBox::Ok)->setText ("Accepter");
    closeButton->button (QDialogButtonBox::Cancel)->setText ("Annuler");

    defaultProjectManagerWidgetLayoutBox = new FCheckBox ("Afficher le gestionnaire de projet",
                                                #ifdef MINIMAL
                                                            flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                                #else
                                                            flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                                #endif
                                                            "projectManagerDisplay",
                                                            {"Interface", "Afficher le gestionnaire de projet"});

    defaultFileManagerWidgetLayoutBox = new FCheckBox ("Afficher le gestionnaire de fichiers",
                                            #ifdef MINIMAL
                                                        flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                            #else
                                                        flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                            #endif
                                                        "fileManagerDisplay",
                                                        {"Interface", "Afficher le gestionnaire de fichiers"});

    defaultFullScreenLayoutBox = new FCheckBox ("Plein écran",
                                                flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                                "fullScreenDisplay",
                                                {"Interface", "Plein écran au lancement"});

    defaultOutputTextEditBox = new FCheckBox ("Afficher les messages",
                                    #ifdef MINIMAL
                                                flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                    #else
                                                flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                    #endif
                                                "outputTextEdit",
                                                {"Interface", "Afficher les messages"});

    defaultFileToolBarBox = new FCheckBox ("Afficher la barre d'outils de fichiers",
#ifdef MINIMAL
                                           flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
#else
                                           flags::status::enabledChecked | flags::commandLineType::noCommandLine,
#endif
                                           "fileToolBar",
                                           {"Interface", "Afficher la barre d'outils de fichiers"});

    defaultEditToolBarBox = new FCheckBox ("Afficher la barre d'outils d'édition",
                                           flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                           "editToolBar",
                                           {"Interface", "Afficher la barre d'outils d'édition"});

    defaultProcessToolBarBox = new FCheckBox ("Afficher la barre d'outils de processus",
                                    #ifdef MINIMAL
                                                flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                    #else
                                                flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                    #endif
                                                "processToolBar",
                                                {"Interface", "Afficher la barre d'outils de processus"});

    defaultOptionsToolBarBox = new FCheckBox ("Afficher la barre d'options",
                                    #ifdef MINIMAL
                                                flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                    #else
                                                flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                    #endif
                                                "optionsToolBar",
                                                {"Interface", "Afficher les options"});

    defaultAboutToolBarBox = new FCheckBox ("Afficher la barre d'A propos",
                                            flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                            "aboutToolBar",
                                            {"Interface", "Afficher la barre A propos"});

    QGroupBox* behaviorGroupBox = new QGroupBox (tr ("Sauvegarder/Lancer"));

    defaultSaveProjectBehaviorBox = new FCheckBox ("Sauvegarder le projet .alt automatiquement",
                                                    flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                                    "saveProjectBehavior",
                                                    {"Interface", "Sauvegarder le projet .alt automatiquement"});

    importerAuLancementBox = new FCheckBox ("Charger les données utilisateur au lancement",
                                            (settings->value ("importerAuLancement") == true ? flags::status::enabledChecked :
                                                    flags::status::enabledUnchecked)
                                            | flags::commandLineType::noCommandLine,
                                            "importerAuLancement",
    {"Interface", "Charger les données xhl du disque optique\nou du répertoire de données au lancement"});

    defaultLoadProjectBehaviorBox = new FCheckBox ("Charger le projet par défaut au lancement",
                                                    flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                                    "loadProjectBehavior",
                                                    {"Interface", "Charger le projet .alt au lancement"});

    // Ces deux cases sont mutuellement exclusives. On aurait pu mettre un FRadioButton à la place. On laisse des FCheckBox par esthétique
    // et aussi pour éviter de devoir rajouter toute cette classe pour ce seul cas de figure.
    // apparemment ne fonctionne pas sous Windows si l'ordre des deux instruction est inversé... OK sous Linux.

    defaultLoadProjectBehaviorBox->setDisableObjects ({importerAuLancementBox});
    importerAuLancementBox->setDisableObjects ({defaultLoadProjectBehaviorBox});

    QGroupBox *outputGroupBox = new QGroupBox (tr ("Console"));

    defaultMaximumConsoleOutputBox = new FCheckBox ("Limiter la sortie de la console",
                                                    flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                                    "limitConsoleOutput",
                                                    {"Interface", "Limiter le nombre de lignes en sortie de la console"});

    defaultQuietBox = new FCheckBox ("Limiter la verbosité",
                                     flags::status::enabledChecked | flags::commandLineType::defaultCommandLine,
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

    displayToolBarList <<  fileToolBar
                       << editToolBar
                       << processToolBar
                       << optionsToolBar
                       << aboutToolBar;

    QGridLayout *layout = new QGridLayout;
    QVBoxLayout *displayDocksLayout = new QVBoxLayout;
    QVBoxLayout *displayToolBarsLayout = new QVBoxLayout;
    QVBoxLayout *behaviourLayout = new QVBoxLayout;
    QVBoxLayout *outputLayout = new QVBoxLayout;

    for (FCheckBox* a : displayWidgetListBox)    displayDocksLayout->addWidget (a);

    for (FCheckBox* a : behaviorWidgetListBox)   behaviourLayout->addWidget (a);

    for (FCheckBox* a : outputListBox)           outputLayout->addWidget (a);

    for (int i = 0; i < displayToolBarList.size(); ++i)
        {
            displayToolBarsLayout->addWidget (displayToolBarCBoxListBox[i]);

            connect (displayToolBarCBoxListBox[i], SIGNAL (toggled (bool)), displayToolBarList[i], SLOT (setVisible (bool)));
        }

    displayGroupBox->setLayout (displayDocksLayout);
    behaviorGroupBox->setLayout (behaviourLayout);
    displayToolBarsGroupBox->setLayout (displayToolBarsLayout);
    outputGroupBox->setLayout (outputLayout);

    layout->addWidget (displayGroupBox, 0, 0);
    layout->addWidget (behaviorGroupBox, 1, 0);
    layout->addWidget (displayToolBarsGroupBox, 0, 1);
    layout->addWidget (outputGroupBox, 1, 1);
    layout->addWidget (closeButton, 2, 0);
    contentsWidget->setLayout (layout);

    connect (closeButton, &QDialogButtonBox::accepted,
             [this]
    {
        settings->setValue ("importerAuLancement", importerAuLancementBox->isChecked());
        settings->setValue ("loadProjectBehavior", defaultLoadProjectBehaviorBox->isChecked());

        if ((isDefaultSaveProjectChecked())
                || (QMessageBox::Yes == QMessageBox::warning (nullptr, tr ("Sauvegarder le projet"),
                        tr ("Le projet n'a pas été sauvegardé.\nAppuyer sur Oui pour le sauvegarder\nou sur Non pour fermer le dialogue sans sauvegarder le projet."),
                        QMessageBox::Yes | QMessageBox::No))
           )
            altair->updateProject (true);

        contentsWidget->accept();
    });

    /* note on connection syntax
     * Here the new Qt5 connection syntax should be used with care and disregarded when both an action button and an FCheckBox activate a slot as the slots
     * are overloaded (which could possibly be rewritten) and a) the action button uses the argumentless slot whilst
     * b) the boolean version of slots must be used by the FcheckBox. The new Qt5 syntax cannot work this out as it does not manage overloading. */

    connect (closeButton, &QDialogButtonBox::rejected, contentsWidget, &QDialog::reject);

    connect (defaultFileManagerWidgetLayoutBox, SIGNAL (toggled (bool)), this, SLOT (on_displayFileTreeViewButton_clicked (bool)));
    connect (defaultProjectManagerWidgetLayoutBox, SIGNAL (toggled (bool)), this, SLOT (on_openManagerWidgetButton_clicked (bool)));

    connect (defaultFullScreenLayoutBox, SIGNAL (toggled (bool)), this, SLOT (displayFullScreen (bool)));
    connect (defaultMaximumConsoleOutputBox, &FCheckBox::toggled, [] {v(limitConsoleOutput).toggle();});
    connect (defaultOutputTextEditBox, &FCheckBox::toggled, [this] {bottomDockWidget->setVisible (defaultOutputTextEditBox->isChecked());});

    connect (defaultMaximumConsoleOutputBox, &FCheckBox::toggled, [this]
    {
        QTimer *timer = new QTimer (this);

        if (v(limitConsoleOutput).isTrue())
            {
                connect (timer, &QTimer::timeout, [&] { altair->readRankSignal();});
                connect (& (altair->process), SIGNAL (finished (int)), timer, SLOT (stop()));
                timer->start (PROGRESSBAR_TIMEOUT);
            }
        else
            {
                timer->stop();
            }

    });

    setWindowTitle (tr ("Configuration"));
    setWindowIcon (QIcon (":/images/altair.png"));
}


void MainWindow::displayFullScreen (bool state)
{
    if (state)
        {
            setWindowState (Qt::WindowFullScreen);
            displayAction->setIcon (QIcon (":/images/show-normal.png"));
        }
    else
        {
            setWindowState (Qt::WindowNoState);
            standardDisplay();
        }
}


void MainWindow::toggleFullScreen()
{
    if (windowState() != Qt::WindowFullScreen)
        {
            setWindowState (Qt::WindowFullScreen);
            displayAction->setIcon (QIcon (":/images/show-normal.png"));
        }
    else
        {
            setWindowState (Qt::WindowNoState);
            standardDisplay();
        }
}

void MainWindow::feedLHXConsoleWithHtml()
{
    altair->readRankSignal();

    QString buffer = QString::fromUtf8 (altair->process.readAllStandardOutput());

    consoleDialog->insertHtml (buffer);
    ++consoleCounter;

    consoleDialog->moveCursor (QTextCursor::End);
}

void MainWindow::feedRConsoleWithHtml()
{
                QString buffer = QString::fromUtf8 (altair->process.readAll());
                if (altair->fileRank > 100) altair->fileRank = 5;
                else if (altair->fileRank > 90) ++altair->fileRank;
                else
                     altair->fileRank += ceil((100 - altair->fileRank) / 10);

                consoleDialog->insertHtml (buffer.replace ("\n", "<br>"));
                consoleDialog->moveCursor (QTextCursor::End);
}


void MainWindow::resetCounter()
{
    consoleCounter = 0;
}


