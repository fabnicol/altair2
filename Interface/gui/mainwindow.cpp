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
    setGeometry(QRect(width / 2, height / 2, width, height));
#endif

    displayAction->setIcon (QIcon (":/images/show-maximized.png"));
}

MainWindow::MainWindow (char* projectName)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenSize= screen->geometry();
    height = screenSize.height() / 2;
    width = screenSize.width() / 2;
    setGeometry(QRect(width / 2, height / 2, width, height));
    fontsize = 10;

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
    fileTreeViewDockWidget->setFeatures (QDockWidget::DockWidgetVerticalTitleBar |
                                         QDockWidget::DockWidgetFloatable |
                                         QDockWidget::DockWidgetMovable |
                                         QDockWidget::DockWidgetClosable);
    addDockWidget (Qt::LeftDockWidgetArea, fileTreeViewDockWidget);

    managerDockWidget = new QDockWidget;
    managerDockWidget->setWidget (altair->managerWidget);
    managerDockWidget->setFeatures (QDockWidget::DockWidgetVerticalTitleBar |
                                    QDockWidget::DockWidgetFloatable |
                                    QDockWidget::DockWidgetMovable |
                                    QDockWidget::DockWidgetClosable);
    addDockWidget (Qt::RightDockWidgetArea, managerDockWidget);

    Abstract::initH();

    createToolBars();

    bottomTabWidget->setCurrentIndex (0);

    bottomTabWidget->setMinimumHeight (static_cast<int>(round(300)));

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
        version = QString(versionFile.readLine(8));
        versionFile.close();
    }


    setWindowTitle ("Interface  Altaïr " + version);

    m = new MatriculeInput (150, 50);

    connect(m, SIGNAL(updateProject(int)), altair, SLOT(updateProject(int)));
    connect(altair, SIGNAL(ajouterLigneMatricules()), m, SLOT(ajouterLigneMatricules()));

    Altair::RefreshFlag =  Altair::RefreshFlag  | interfaceStatus::parseXml;


    // resetting interfaceStatus::parseXml bits to 0
    Altair::RefreshFlag = Altair::RefreshFlag & (~interfaceStatus::parseXml);

    connect (nppBottomTabWidgetButton, &QToolButton::clicked, [this] { on_displayLogButton_clicked();});
    connect (clearBottomTabWidgetButton, &QToolButton::clicked, [this] { on_clearOutputTextButton_clicked();});
    connect (consoleDialog, SIGNAL (copyAvailable (bool)), consoleDialog, SLOT (copy()));
    connect (& (altair->process), SIGNAL (finished (int)), this, SLOT (resetCounter()));
    connect (& (altair->process), SIGNAL (finished (int)), this, SLOT (resetTableCheckBox()));
    connect (altair, SIGNAL(substituer_valeurs_dans_script_R()), this, SLOT (substituer_valeurs_dans_script_R()));

    if (! QFile(altair->projectName).exists())
    {
          altair->updateProject(update::saveProject | update::noWarnRExport);
    }

    if (projectName[0] != '\0')
        {
            // Paraît étrange... mais c'est pour éviter de lire deux fois le projet
            altair->closeProject();
            altair->projectName = projectName;
            altair->openProjectFileCommonCode();
        }

    createDialogs();

    QRegularExpression reg(defaultLoadProjectBehaviorBox->getHashKey()
                           + QString(".*$\\s*oui"),
                           QRegularExpression::MultilineOption
                           | QRegularExpression::UseUnicodePropertiesOption);

    if (reg.match(readFile(altair->projectName)).hasMatch())
    {
        altair->openProjectFileCommonCode();
    }
}

void MainWindow::substituer_valeurs_dans_script_R()
{
    for (auto &&a: dialog->tabs) a->substituer_valeurs_dans_script_R();
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
                           displayManagerAction, clearOutputTextAction, printBaseAction
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
    connect (saveAction, &QAction::triggered, [this] {
        Altair::RefreshFlag &= ~interfaceStatus::XmlParsed;
        altair->updateProject (update::saveProject | update::noWarnRExport);
    });

    saveAsAction = new QAction (tr ("En&registrer le projet\ncomme..."), this);
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

     QMessageBox::StandardButton
             res = QMessageBox::warning(this,
                     "Protection de la confidentialité des données",
                     QString("Nettoyage des <b>Données</b> du répertoire <br>")
                             + altair->userdatadir +
                               "<br>Attention, toutes les données de ce répertoire seront effacées.<br>Appuyer sur <b>Non</b> pour annuler ce nettoyage ou sur <b>Ignorer</b> pour ne pas fermer l'application.",
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

    dialog->reinitialiser_prologue();
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
               << clearOutputTextAction << printBaseAction << separator[3] << configureAction
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

QByteArray MainWindow::extraire_donnees_protegees (const QByteArray& st)
{
    QByteArray out;
    const size_t taille = st.size();
    out.reserve(static_cast<size_t>(taille / 5));
    QByteArray::const_iterator iter = st.begin();

    size_t i = 0;
    const size_t pas = taille / 20;
    size_t k = 0;

    while (*iter != '<' && ++iter != st.end()) continue;

    out.push_back('<');

    if (* (iter + 1) == '?')
        {
            while (++iter != st.end())
            {
                out.push_back(*iter);
                if (*iter == '>') break;
            }
            out.push_back('\n');
        }

start:

    while (iter != st.end())
        {
            while (++iter != st.end()
                    && *iter != '<')
                {
                    ++i;
                    ++k;
                }

            if (iter == st.end()) break;

            out.push_back ('<');

            if (* (iter + 1) == '/') /* </TAG>  */
                {
                    while (++iter != st.end() )
                        {
                            ++i;
                            ++k;
                            out.push_back (*iter);
                            if (*iter == '>') break;
                        };

                    out.push_back('\n');
                    continue;
                }

            if (iter == st.end()) break;

            if (k >= pas)
                {
                    emit (altair->setProgressBar (static_cast<long>(i)));
                    qApp->processEvents();
                    k = 0;
                }

            QByteArray tag;

            while (++iter != st.end())
                {
                    ++i;
                    ++k;

                    if (*iter == ' '  /* <TAG ...> */
                         || *iter == '>')  /* <TAG> */
                        {
                            break;
                        }
                   tag.push_back(*iter);
                }

            out.push_back(tag);

            if (*iter == '>')
            {
                out.push_back(">\n");
                goto start;
            }
            else
            if (*iter == ' ')
            {
                out.push_back(' ');
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
                                    ++i;
                                    ++k;
                                }

                            if (*iter == '=')
                                {
                                   break; /*<TAG  V  =... /> */
                                }
                            else
                                continue; /*<TAG  x ... /> */
                        }
                    else /* <TAG  x... /> */
                    if (*iter == '>')
                        {
                            out.push_back (">\n");
                            ++iter;
                            ++i;
                            ++k;
                            goto start;
                        }
                    else   /*<TAG  x... /> */
                    {
                      out.push_back(*iter);
                    }
                }

            if (iter == st.end()) break;

            /*<TAG  V  =... /> */

            QByteArray value;

            while (++iter != st.end())
                {
                    ++i;
                    ++k;

                    if (*iter == '\"') break;
                }

            if (iter == st.end()) break;

            while (++iter != st.end())
                {
                    ++i;
                    ++k;
                    if (*iter == '\"') break;
                    value.push_back(*iter);
                }

            if (iter == st.end()) break;

            QByteArray reste;

            while (++iter != st.end())
                {
                    ++i;
                    ++k;

                    reste.push_back(*iter);

                    if (*iter == '>') break;
                }

            if (iter == st.end()) break;

            const array<QByteArray, 20> Tags =
            {
                "Civilite",
                "ComplNom",
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

            for (auto &&t : Tags)
                {
                    if (tag == t)
                        {
                            const QByteArray& remplacement =
                                    tag == "NIR" ? "V = \"" + value.mid (0, 5) + "Z\"" + reste + "\n"
                                                                      :
                                    (tag != "Service" ? "V = \"Z\"" + reste + "\n"
                                                                      :
                                    (value.mid(0, 3) != "ELU" ? "V = \"Z\"" + reste + "\n"
                                                                      :
                                                                "V = \"ELU\"" + reste + "\n"));

                            out.push_back (remplacement);
                            ++iter;
                            ++i;
                            ++k;
                            goto start;
                        }
                }

            out.push_back ("V = \"" + value + "\"" + reste + "\n");
        }

    return (out);
}

void MainWindow::launch_process (const QString& path)
{
    QFile xml (path);

    QString xml_out (path + ".new");
    QFile out(xml_out);

    if (!xml.open (QIODevice::ReadOnly | QIODevice::Text))
        {
            Q ("Erreur de lancement du processus (ouverture du fichier entrée)")
            return;
        }

    if (! out.open (QIODevice::WriteOnly))
        {
            Q ("Erreur de lancement du processus (ouverture du fchier sortie)")
            return;
        }

    altair->textAppend (PROCESSING_HTML_TAG "Lecture du fichier...");
    emit (altair->showProgressBar());
    emit (altair->setProgressBar (0));
    altair->outputTextEdit->repaint();
    QByteArray xml_mod = xml.readAll();

    altair->textAppend (PROCESSING_HTML_TAG "Remplacement des informations protégées. Patientez...");
    altair->outputTextEdit->repaint();

    int bar_range = xml_mod.size();

    emit (altair->setProgressBar (0, bar_range));

    const QByteArray& v = extraire_donnees_protegees (xml_mod);

    if (out.write(v) == -1)
    {
        altair->textAppend (ERROR_HTML_TAG  "Anonymisation de " + path + " : erreur d'écriture en sortie de fichier.");
    }

    out.close();

    emit (altair->setProgressBar (0));
    xml.close();
    xml.remove();
    out.rename(path);

    altair->textAppend (PROCESSING_HTML_TAG  "Anonymisation de " + path + " terminée.");
    altair->outputTextEdit->repaint();
    emit (altair->setProgressBar (0));

    // Il est souhaitable d'actualiser le projet avant de lancer v() car en cas de non actualisation récente la valeur de la case
    // peut être en décalage avec la réalité. C'est au cours de ces actualisations que la valeur est enregistrée dans une table de hashage.

    altair->updateProject (update::saveProject | update::noWarnRExport);
}

const vector <uint8_t>  MainWindow::nettoyer_donnees (vector <uint8_t>& st)
{
    vector <uint8_t> out;
    const size_t taille = st.size();

    // Découper le fichier en 5
    out.reserve (taille / 5);
    vector <uint8_t>::const_iterator iter = st.begin();
    vector <uint8_t>::const_iterator iter2;
    size_t i = 0;
    size_t k = 0;

    // Découper le fichier en pas de 1/20 ème de la taille
    const size_t pas = taille / 20;
    bool quote = false; // drapeau d'indentification d'une chaine
    bool is_open_tag = true;

loop :

    while (iter != st.end())
        {

            switch (*iter)
                {
                case  0x22 :    // Repérer "
                    quote = ! quote; // On a identifié un "
                    out.push_back (0x22); // L'empiler
                    if (! quote) out.push_back(' '); // Eviter les champs collés
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

                    if (quote) // C'est un '>' cité, qui n'est pas une ouverture de tag
                        {
                            out.push_back (0x3e);

                            ++iter;
                            break;
                        }

                    is_open_tag = false;
                    out.push_back (0x3e);
                    out.push_back ('\n');

                    while (*++iter != 0x3c  //  Repérer '<'
                            && iter != st.end())  {}

                    is_open_tag = true;
                    break;

                case 0x0A : // Saut de ligne interne aux tags, sauter car provoque des fails.
                    if (is_open_tag)
                    {
                        ++iter;
                        break;
                    }
                    ++iter;
                    out.push_back (0x0A);
                    break;

                default :

                    // Cas général
                    // Empiler tous les caractères imprimables **et** non potentiellement problématiques

                    if (*iter >= 0x1F && *iter < 0x7F)   // ASCII non-control range
                        out.emplace_back (*iter);
                    else
                        if (*iter >= 0xC0)
                        {
                            // Pour les caractères non ASCII, avec diacritiques, <= 0xFF par construction.
                            // ISO-8859-1
                            //      0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
                            // Cx 	À 	Á 	Â 	Ã 	Ä 	Å 	Æ 	Ç 	È 	É 	Ê 	Ë 	Ì 	Í 	Î 	Ï
                            // Dx 	Ð 	Ñ 	Ò 	Ó 	Ô 	Õ 	Ö 	× 	Ø 	Ù 	Ú 	Û 	Ü 	Ý 	Þ 	ß
                            // Ex 	à 	á 	â 	ã 	ä 	å 	æ 	ç 	è 	é 	ê 	ë 	ì 	í 	î 	ï
                            // Fx 	ð 	ñ 	ò 	ó 	ô 	õ 	ö 	÷ 	ø 	ù 	ú 	û 	ü 	ý 	þ 	ÿ

                            if (*iter != 0xC3)
                            {
                                out.emplace_back (*iter); // Empiler les caractères accentués (1 octet en Latin-1)
                            }
                            else
                            {
                                // UTF-8. Ã va être excessivement rare, sinon inexistante dans les bases.
                                // On part donc du principe que l'occurrence de 0xC3 sert à encoder un fichier UTF-8
                                // et non pas le caractère Ã suivi d'une lettre Latin-1.

                                if (*(iter + 1) >= 0x80 && *(iter + 1) <= 0xBF)
                                {
                                    out.emplace_back (*iter);   // 0xC3
                                    out.emplace_back (*++iter); // Empiler les caractères accentués (2 octets en UTF-8)
                                }
                                else
                                {
                                    out.push_back (0x20); // Sinon par défaut remplacer par une espace tout caractère nettoyé (SP ' ')
                                }
                            }
                        }
                        else
                        {
                            out.push_back (0x20); // Sinon par défaut remplacer par une espace tout caractère nettoyé (SP ' ')
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

    vector <uint8_t> xml_mod;
    long bar_range = (long) common::getFileSize (path);
    xml_mod.reserve (bar_range);
    xml_mod.insert (
        xml_mod.end(),
        std::istreambuf_iterator<char> (xml),
        std::istreambuf_iterator<char>());

    altair->outputTextEdit->repaint();

    emit (altair->setProgressBar (0, bar_range));
    altair->textAppend (PROCESSING_HTML_TAG + tr ("Longueur du fichier: ") + QString::number(xml_mod.size()));

    const vector<uint8_t>& res = nettoyer_donnees(xml_mod);

    out.write((char*) res.data(), res.size());

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
            altair->updateProject (update::saveProject| update::noWarnRExport);
        }
    else
        {
            altair->textAppend (ERROR_HTML_TAG  "Le nettoyage de " + path + " a échoué.");
            altair->outputTextEdit->repaint();
            altair->updateProject (update::saveProject | update::noWarnRExport);
        }
}

void MainWindow::anonymiser()
{
    QStringList args;

    altair->updateProject (update::saveProject | update::noWarnRExport);

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

    altair->updateProject (update::saveProject | update::noWarnRExport);

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

void MainWindow::saveProjectAs (const QString &newstr)
{

    if (QFileInfo (newstr).isFile())
        {
            QMessageBox::StandardButton result = QMessageBox::warning (nullptr,
                                                                       "Ecraser le fichier ?",
                                                                       "Le projet " + newstr + " va être écrasé.\nAppuyer sur Oui pour confirmer.",
                                                                       QMessageBox::Ok | QMessageBox::Cancel);


            if (result != QMessageBox::Ok)
                {
                    return;
                }

            std::remove (newstr.toStdString().c_str());
        }

    altair->projectName = newstr;
    altair->writeProjectFile();

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
   
    const QString docxReportFilePath = projectRootDir + "altair.docx";
    const QString odtReportFilePath = projectRootDir  + "altair.odt";
    const QString pdfReportFilePath = projectRootDir  + "altair.pdf";
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
    
    result &= common::copyFile (docxReportFilePath, subDirStr  + "altair.docx", "Le rapport Altaïr Word", REQUIRE);

    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été exporté sous : " + subDirStr);
            altair->setProgressBar (2);
        }

    result &= common::copyFile (odtReportFilePath, subDirStr  + "altair.odt", "Le rapport Altaïr Open Office", REQUIRE);

    if (result)
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Open Office a été exporté sous : " + subDirStr);
            altair->setProgressBar (3);
        }

    result &= common::copyFile (pdfReportFilePath, subDirStr  + "altair.pdf", "Le rapport Altaïr PDF", REQUIRE);

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
   
    const QString docxReportFilePath = projectRootDir  + "altair.docx";
    const QString pdfReportFilePath = projectRootDir   + "altair.pdf";
    const QString odtReportFilePath = projectRootDir   + "altair.odt";
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
    else
    {
            altair->textAppend (PARAMETER_HTML_TAG  "Le log n'a pas été archivé");
    }
    
    result = common::zip (docxReportFilePath, subDirStr + "altair.docx.arch");

    if (result)
    {
        altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été archivé sous : " + subDirStr);
        altair->setProgressBar (2);
    }
    else
    {
        altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Word n'a pas été archivé");
    }

    result = common::zip (odtReportFilePath, subDirStr  + "altair.odt.arch");

    if (result)
    {
        altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Open Office a été archivé sous : " + subDirStr);
        altair->setProgressBar (3);
    }
    else
    {
        altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Open Office n'a pas été archivé.");
    }

    result = common::zip (pdfReportFilePath, subDirStr  + "altair.pdf.arch");

    if (result)
    {
        altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été archivé sous : " + subDirStr);
        altair->setProgressBar (4);
    }
    else
    {
        altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr PDF n'a pas été archivé");
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
                        altair->textAppend (ERROR_HTML_TAG  "La base " + v(base) + QDir::separator() + st + " n'a pas été archivée");

                    altair->outputTextEdit->repaint();
                }

            result = common::zip (bulletinsFilePath, subDirStr + "Bulletins.csv.arch");

            if (result)
                {
                    altair->textAppend (PARAMETER_HTML_TAG  "La base des bulletins de paye a été archivée sous : " + subDirStr);
                    altair->setProgressBar (++bar);
                }

            else
                altair->textAppend (ERROR_HTML_TAG  "La base des bulletins de paye n'a pas été archivée");
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
                                    altair->textAppend (PARAMETER_HTML_TAG  "La base " + QDir::toNativeSeparators(s) + " a été archivée sous : " + subDirStr);
                                    altair->setProgressBar (++bar);
                                }
                            else
                                altair->textAppend (ERROR_HTML_TAG  "La base " +  QDir::toNativeSeparators(s) + " n'a pas été archivée");

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
        altair->textAppend (ERROR_HTML_TAG  "Les bases en lien n'ont été archivées");

    QFile (altair->projectName).close();
    saveProjectAs (subDirStr + "projet.alt");

#ifdef Q_OS_WINDOWS
    process.start (QString("peazip") + tools::systemSuffix, QStringList() << "-add27z" <<  subDirStr.chopped(1));
#else
    process.start (QString("tar") + tools::systemSuffix, QStringList() << "-cf" << subDirStr.chopped(1) + ".arch" <<  "-C" << subDirStr << ".");
#endif
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(tarFinished()));

    altair->setProgressBar (max);
    altair->textAppend (STATE_HTML_TAG "Archivage terminé sous " + subDirStr.chopped(1) +
                    #ifdef Q_OS_WINDOWS
                        "7z"
                    #else
                        ".arch"
                    #endif
                        );
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
       
    process.setWorkingDirectory(QFileInfo(subDirStr).absolutePath());

#ifdef Q_OS_WINDOWS

    QDir d = QDir(subDirStr.chopped(3));
    if (d.exists()) d.removeRecursively();
    process.start (QString("peazip") + tools::systemSuffix, QStringList() << "-ext2here" <<  subDirStr);
    process.waitForFinished();
    QDir(projectRootDir).removeRecursively();
    QDir(subDirStr.chopped(3)).rename(subDirStr.chopped(3), projectRootDir);
#else
    process.start (QString("tar") + tools::systemSuffix, QStringList() << "-xf" << subDirStr << "-C" << subDirStr);
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
   
    result = common::unzip(projectRootDir, rapportList);
    result &= common::unzip(dir_images.absolutePath(), imagesList);
   
    if (result)
        altair->textAppend(PARAMETER_HTML_TAG "Extraction des rapports... fait.");
    else
        altair->textAppend(PARAMETER_HTML_TAG "Les rapports ne sont pas désarchivés");
                                   
    if (QFileInfo("log.html").exists())
    {
            altair->textAppend (PARAMETER_HTML_TAG  "Le log a été désarchivé sous : " + projectRootDir);
            altair->setProgressBar (1);
    }
    else
    {
        altair->textAppend (PARAMETER_HTML_TAG  "Le log n'est pas désarchivé.");
    }
    
    if (QFileInfo("altair.docx").exists())
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr Word a été décompressé sous : " + projectRootDir);
            altair->setProgressBar (2);
        }

    if (QFileInfo("altair.odt").exists())
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr ODT a été décompressé sous : " + projectRootDir);
            altair->setProgressBar (3);
        }

    if (QFileInfo("altair.pdf").exists())
        {
            altair->textAppend (PARAMETER_HTML_TAG  "Le rapport Altaïr PDF a été décompressé sous : " + projectRootDir);
            altair->setProgressBar (4);
        }

    if (v(archiveTable).isTrue() || v(archiveAll).isTrue())
    {
        common::unzip(projectRootDir, tableList);
        result = true;
        for (auto& f: tableList) result &= QFileInfo(f.chopped(5)).exists();

        // W10 bug ?
#ifndef Q_OS_WINDOWS
        int bar = 5;

        if (result)    
            {
                altair->textAppend (PARAMETER_HTML_TAG  "La base des bulletins de paye a été décompressée sous : " + projectRootDir);
                altair->setProgressBar (++bar);
            }
        else
            altair->textAppend (ERROR_HTML_TAG  "La base des bulletins de paye n'a pas décompressée sous : " + projectRootDir);
#endif
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
            altair->textAppend (ERROR_HTML_TAG  "Les bases XML/XHL n'ont pas été décompressées" );

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
        altair->textAppend (ERROR_HTML_TAG  "Les bases en lien n'ont pas été décompressées");

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

    defaultLoadProjectBehaviorBox = new FCheckBox ("Charger le projet par défaut au lancement",
                                                    flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                                    "loadProjectBehavior",
                                                    {"Interface", "Charger le projet .alt au lancement"});

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


    behaviorWidgetListBox  << defaultLoadProjectBehaviorBox;

    displayToolBarCBoxListBox  <<  defaultFileToolBarBox
                               <<  defaultEditToolBarBox
                               <<  defaultProcessToolBarBox
                               <<  defaultOptionsToolBarBox
                               <<  defaultAboutToolBarBox;

    outputListBox << defaultMaximumConsoleOutputBox
                  << defaultQuietBox;

    QList<QToolBar*> displayToolBarList ;

    displayToolBarList << fileToolBar
                       << editToolBar
                       << processToolBar
                       << optionsToolBar
                       << aboutToolBar;

    QGridLayout *layout = new QGridLayout;
    QVBoxLayout *displayDocksLayout = new QVBoxLayout;
    QVBoxLayout *displayToolBarsLayout = new QVBoxLayout;
    QVBoxLayout *behaviourLayout = new QVBoxLayout;
    QVBoxLayout *outputLayout = new QVBoxLayout;

    for (FCheckBox* a : displayWidgetListBox)    { displayDocksLayout->addWidget (a); Abstract::abstractWidgetList.append(a);}

    for (FCheckBox* a : behaviorWidgetListBox)   { behaviourLayout->addWidget (a); Abstract::abstractWidgetList.append(a);}

    for (FCheckBox* a : outputListBox)           { outputLayout->addWidget (a); Abstract::abstractWidgetList.append(a); }

    for (int i = 0; i < displayToolBarList.size(); ++i)
        {
            displayToolBarsLayout->addWidget (displayToolBarCBoxListBox[i]);
             Abstract::abstractWidgetList.append(displayToolBarCBoxListBox[i]);

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

    connect (closeButton, SIGNAL(accepted()), saveAction, SLOT(trigger()));
    connect (closeButton, SIGNAL(accepted()), contentsWidget, SLOT(accept()));

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
                else if (altair->fileRank > 25) ++altair->fileRank;
                else
                     altair->fileRank += ceil((100 - altair->fileRank) / 25);

                consoleDialog->insertHtml (buffer.replace ("\n", "<br>"));
                consoleDialog->moveCursor (QTextCursor::End);
}


void MainWindow::resetCounter()
{
    consoleCounter = 0;
}


