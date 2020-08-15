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


/// \file altair.cpp
/// \author Fabrice Nicol
/// \brief Code implémentant la classe principale Altair, qui joue le rôle d'agent de l'interface graphique
/// \sa MainWindow

#include <QFile>
#include <sys/stat.h>

#include <QModelIndex>
#include <QtXml>
#include <QSettings>
#include <mutex>
#include "fstring.h"
#include "altair.h"
#include "options.h"
#include "fstring.h"
#include "tags.h"


std::uint16_t Altair::RefreshFlag = interfaceStatus::hasUnsavedOptions;
qint64   Altair::totalSize;

class Hash;
QHash<QString, QString> Hash::Annee;
QHash<QString, QString> Hash::Mois;
QHash<QString, QString> Hash::Budget;
QHash<QString, QStringList> Hash::Siret;
QHash<QString, QStringList> Hash::Etablissement;
QHash<QString, QString> Hash::Employeur;
QHash<QString, bool>    Hash::Suppression;
QVector<QStringList> Hash::Reference;
QHash<QString, QStringList> Hash::fileList;
QHash<QString, FStringList*> Hash::wrapper;
QHash<QString, QStringList> Hash::description;

void Altair::initialize()
{
    // Taille par défaut

    adjustSize();

    // Récupérer la variable d'environnement USER/USERNAME

#ifdef Q_OS_WIN
    username = "Public";
#else
    username = qgetenv ("USER");

    if (username.isEmpty())
        username = qgetenv ("USERNAME");

#endif

    // Accès au répertoire des données XML par défaut (userdatadir)

    const QString xhl = path_access (QString (DONNEES_XHL)) + QDir::separator();

#ifdef Q_OS_WIN
    userdatadir = xhl;
#else
    userdatadir = username == "fab" ? xhl : xhl + username + QDir::separator() ;
#endif

    if (! QFileInfo (userdatadir).isDir())
        {
            bool res = QDir().mkpath(userdatadir);
            if (! res) Q("Le répertoire " + userdatadir + " dans lequel le projet défaut.alt doit être généré n'a pas pu être créé.\nMerci de le créer manuellement.")
        }

    Hash::description["année"] = QStringList ("Fichiers .xhl");
}

void Altair::refreshModel()
{
    // Reconstruire complètement l'arborescence de fichiers (explorateur à gauche de l'interface)
    // Partie modèle

    delete (model);
    model = new QFileSystemModel;
    model->setReadOnly (false);
    model->setRootPath ("");

    model->sort (Qt::AscendingOrder);
    model->setFilter (QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    model->setNameFilterDisables (false);
    XML_FILTERS = QStringList({"*.xml", "*.xhl", "*.XML", "*.XHL", "*.zip", "*.ZIP", "*.7z", "*.7Z", "*.tar.bz2", "*.TAR.BZ2", "*.tar.gz", "*.TAR.GZ"});
    model->setNameFilters (XML_FILTERS);
}

void Altair::refreshTreeView (bool create)
{
    // Reconstruire complètement l'arborescence de fichiers (explorateur à gauche de l'interface)
    // Partie vue

    if (create)
        {
            fileTreeView = new QTreeView;
        }

    fileTreeView->setModel (model);
    fileTreeView->setRootIndex (model->index (userdatadir));
    fileTreeView->hideColumn (1);
    fileTreeView->hideColumn (2);
    fileTreeView->hideColumn (3);
    fileTreeView->setMinimumWidth (300);
    fileTreeView->setColumnWidth (0, 300);
    fileTreeView->setSelectionMode (QAbstractItemView::ExtendedSelection);
    fileTreeView->setSelectionBehavior (QAbstractItemView::SelectItems);
    fileTreeView->header()->setStretchLastSection (true);
    fileTreeView->expandAll();  // ne semble pas fonctionner
    fileTreeView->setSortingEnabled (true);
    fileTreeView->sortByColumn (0, Qt::AscendingOrder); //  note: doc Qt5 erronée. Il faut préciser cette option qui n'est pas un défaut.
}



Altair::Altair()
{
    //////////////  Initialisation   ///////////////////

    setAttribute (Qt::WA_DeleteOnClose);

    // Initialiser des variables utilisateur/répertoire données

    initialize();
    setAcceptDrops (true);

    // Créer l'arborescence des fichiers

    refreshModel();
    refreshTreeView (true);

    bool visibility = true;

    // Créer l'onglet central à vide

    project = new FListFrame (fileTreeView,                    // les fichiers peuvent être importés dans l'onglet depuis cette arborescence
                              importFiles,                      // l'onglet autorise l'importation de fichiers
                              "XHL",                            // Balise des fichiers de paye pour l'exportation du projet .alt (<XHL>...</XHL>)
    {"Décodeur de fichiers XHL", ""},      // Section du gestionnaire de projet (à droite de l'interface)
    "g",                                   // Option de ligne de commande introduisant les fichiers de paye
    flags::commandLineType::coreApplicationCommandLine | flags::status::hasListCommandLine | flags::status::enabled,  // Génère une ligne de commande + à partir d'une liste enchâssée + le composant est activé par défaut
    {" ", " -g "},                       // A chaque ligne d'un onglet correspond un séparateur blanc dans la ligne de commande. Pour chaque onglet différent, -g est préfixé devant la liste des fichiers
    {"item", "onglet"},                  // Balises de niveau 1 et 2 echassées sous <XML> : <XML> <onglet><item>...</item><item>...</item></onglet> </XML>
    {"Siret", "Budget", "Employeur"},    // Onglets supplémentaires générés en sus de ceux qui résultent de la décomposition des fichiers en années (1 année = 1 onglet)
    tools::TabWidgetTrait::NO_EMBEDDING_TAB_WIDGET);                      // pas d'enchâssement de l'onglet central dans un onglet matrice

    // Assigne le modèle de fichiers de la classe comme membre de project
    project->model = model;
    project->importFromMainTree->setVisible (visibility);

    // Création de la barre de progression

    progress = new FProgressBar (this, &FDialog::killProcess);
    progress->setToolTip (tr ("Décodage"));

    // Caractéristiques générales de l'onglet des messages

    outputTextEdit->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    outputTextEdit->setAcceptDrops (false);
    outputTextEdit->setReadOnly (true);

    //////////////  Signaux et Slots   ///////////////////

    // Ce qui suit présupose que les connexions déclenchées par le clic
    // sont préalablement traitées par FListFrame (ce qui est le cas)

    // Une fois exécutés les automatismes de FListFrame, finaliser en actualisant le projet
    // et en affichant la taille totale ainsi qu'en vérifiant les années.

    connect (project->importFromMainTree,
             &QToolButton::clicked,
             [this]
    {
        updateProject();
        displayTotalSize();
        checkAnnumSpan();

    });

    // Idem pour fermer le projet, animer la barre d'outil, ajouter du texte à l'onglet des messages
    // ou actualiser l'interface
    connect (project->fileListWidget, SIGNAL (forceCloseProject()), this, SLOT (closeProject()));
    connect (project, SIGNAL (showProgressBar()), this, SIGNAL (showProgressBar()));
    connect (project, SIGNAL (setProgressBar (int, int)), this, SIGNAL (setProgressBar (int, int)));
    connect (project, SIGNAL (setProgressBar (int)), this, SIGNAL (setProgressBar (int)));
    connect (project, SIGNAL (textAppend (const QString&)), this, SLOT (textAppend (const QString&)));
    connect (project, SIGNAL (refreshRowPresentation (int)), this, SLOT (refreshRowPresentation (int)));
    connect (project, SIGNAL (updateProject (bool)), this, SLOT (updateProject (bool)));
    connect (project, SIGNAL (appRepaint()), this, SLOT (repaint()));
#ifndef USE_RIGHT_CLICK
    connect (project->deleteGroupButton, SIGNAL (clicked()), this, SLOT (deleteGroup()));
    connect (project->retrieveItemButton, SIGNAL (clicked()), this, SLOT (on_deleteItem_clicked()));
#endif

    // A rebours le signal "click droit" est transmis à project
    connect (this, &QDialog::customContextMenuRequested, [this] {  project->showContextMenu(); });

    // Traitement de la fin d'exécution par processFinished
    connect (&process, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (processFinished (int)));


    //////////////  Graphisme   ///////////////////

    QGridLayout *projectLayout = new QGridLayout;

    // Ajout de la flèche d'importation des fichiers

    projectLayout->addWidget (project->importFromMainTree, 0, 1);

    // Ajout de l'onglet central

    projectLayout->addWidget (project->mainTabWidget, 0, 2);

#ifndef USE_RIGHT_CLICK
    QGridLayout *updownLayout = new QGridLayout;
    updownLayout->addWidget (project->getControlButtonBox(), 0, 0);
    updownLayout->setRowMinimumHeight (1, 40);
    updownLayout->setRowMinimumHeight (3, 40);
    projectLayout->addLayout (updownLayout, 0, 3);
#endif

    mainLayout->addLayout (projectLayout);
    progressLayout->addLayout (progress->getLayout());
    mainLayout->addLayout (progressLayout);

    // Graphisme du gestionnaire de projets

    QStringList labels;
    labels << tr ("") << tr ("Mois") << tr ("Chemin")  << tr ("Taille\nFichier") << tr ("Total")
           << tr ("Employeur Siret Etablissement") << tr ("Budget");

    managerWidget->hide();
    managerWidget->setHeaderLabels (labels);
    managerWidget->setColumnWidth (0, 300);
    managerWidget->setColumnWidth (1, 50);
    managerWidget->setColumnWidth (2, 300);
    managerWidget->setColumnWidth (3, 50);
    managerWidget->setColumnWidth (4, 60);
    managerWidget->setColumnWidth (5, 220);
    managerWidget->setColumnWidth (6, 90);
    managerWidget->setContextMenuPolicy (Qt::ContextMenuPolicy::NoContextMenu);
    managerLayout->addWidget (managerWidget);

    allLayout->addLayout (mainLayout);
    allLayout->addLayout (managerLayout);

    // clics droits

    setContextMenuPolicy (Qt::CustomContextMenu);

    setLayout (allLayout);

    // Titre et icone de l'interface

    const QIcon altairIcon = QIcon (QString::fromUtf8 (":/images/altair.png"));
    setWindowIcon (altairIcon);
}

void Altair::importData()
{
    const QString cdROM = cdRomMounted();

    if (! cdROM.isEmpty())
        {
            QDir c = QDir (cdROM, "", QDir::IgnoreCase, QDir::Drives | QDir::Dirs);

            if (c.exists()
                    && ! c.QDir::entryInfoList (QDir::Dirs
                                                | QDir::Drives
                                                | QDir::Files
                                                | QDir::NoDotAndDotDot).isEmpty())
                {
                    textAppend (PROCESSING_HTML_TAG "Analyse du disque optique...Veuillez patienter...");
                    fileTreeView->setCurrentIndex (model->index (cdROM));

                    project->importFromMainTree->click();

                    return;
                }
        }

    QDir d = QDir (userdatadir);

    if (d.exists() && ! d.QDir::entryInfoList (QDir::Dirs
            | QDir::Files
            | QDir::NoDotAndDotDot).isEmpty())
        {
            fileTreeView->setCurrentIndex (model->index (userdatadir));
            project->importFromMainTree->click();

            // l'opération précédente semble annuler la possibilité de sélectionner les indices proprement
            // à nouveau. Peut-être un bug de Qt. On fait un reset suivi d'un reset.

            fileTreeView->reset();
            refreshTreeView();
            return;
        }
}

void Altair::refreshRowPresentation()
{
    refreshRowPresentation (currentIndex);
}


void Altair::refreshRowPresentation (int j)
{

    if (Hash::wrapper.isEmpty()) return;

    QPalette palette;
    palette.setColor (QPalette::AlternateBase, QColor ("silver"));
    QFont font = QFont ("Courier", parent->fontsize);

    QListWidget *composant = project->getWidgetContainer (static_cast<int>(j));

    if (composant == nullptr) return;

    composant->setPalette (palette);
    composant->setAlternatingRowColors (true);
    composant->setFont (font);
    composant->setSelectionMode (QAbstractItemView::ExtendedSelection);
    composant->setSelectionBehavior (QAbstractItemView::SelectRows);

    QStringList strL = Hash::wrapper["XHL"]->at (static_cast<int>(j));

    int size = strL.size();

    for (int r = 0; (r < composant->count()) && (r < size); r++)
        {
            composant->item (r)->setText (strL.at (r).section ('/', -1));
            composant->item (r)->setTextColor (QColor ("navy"));
        }
    
    composant->sortItems();
}


void Altair::on_newProjectButton_clicked()
{
    closeProject();

    projectName = QString (userdatadir + "défaut.alt");
    QFile projectFile (projectName);

    if (projectFile.exists()) projectFile.remove();

    parent->saveProjectAs();

    textAppend (PARAMETER_HTML_TAG "Nouveau projet créé sous " + projectName);
}

void  Altair::openProjectFileCommonCode()
{
    RefreshFlag = RefreshFlag  | interfaceStatus::parseXml;

    checkEmptyProjectName();
    setCurrentFile (projectName);

    clearInterfaceAndParseProject();

    // resetting interfaceStatus::parseXml bits to 0
    RefreshFlag = RefreshFlag & (~interfaceStatus::parseXml);

    Hash::createReference (project->getRank());
}

void Altair::on_openProjectButton_clicked()
{
    closeProject();
    projectName = QFileDialog::getOpenFileName (this,  tr ("Ouvrir le projet"), userdatadir,  tr ("projet altair (*.alt)"));

    if (projectName.isEmpty()) return;

    openProjectFileCommonCode();
}


void Altair::openProjectFile()
{
    closeProject();
    projectName = qobject_cast<QAction *> (sender())->data().toString();

    openProjectFileCommonCode();
}


bool Altair::clearInterfaceAndParseProject()
{
    options::RefreshFlag = options::RefreshFlag | interfaceStatus::optionTabs;
    RefreshFlag = RefreshFlag | interfaceStatus::tree;

    QTextEdit* editor = parent->getEditor();

    try
        {
            if (editor) editor->clear();
        }
    catch (...) {}

    return refreshProjectManager();
}

void Altair::closeProject()
{
    int projectDimension = project->getRank();

    clearProjectData();

    Altair::totalSize = 0;

    for (int i = projectDimension; i >= 0;   i--)
        {
            project->mainTabWidget->removeTab (i);
            auto widgetV = project->getWidgetContainer();
            int size = widgetV.size();

            if (i < size) widgetV.removeAt (i);
        }

    project->addNewTab();

    QFile projectFile (projectName);
    projectFile.close();
}

void Altair::clearProjectData()
{
    RefreshFlag =  RefreshFlag
                   | interfaceStatus::mainTabs
                   | interfaceStatus::optionTabs
                   | interfaceStatus::tree;


    if (options::RefreshFlag ==  interfaceStatus::hasUnsavedOptions)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Nouveaux paramètres");
            msgBox.setText("Ce projet contient de nouveaux paramètres.\n");
            msgBox.setInformativeText("Appuyer sur OK pour les sauvegarder,\nsinon sur Non\nou sur Fermer pour quitter.\n");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Yes);

            int choice = msgBox.exec();

            switch (choice)
                {
                    case QMessageBox::No  :
                        // parent->dialog->clearOptionData();
                        break;

                    case QMessageBox::Yes :
                        options::RefreshFlag = interfaceStatus::keepOptionTabs;
                        break;

                    case QMessageBox::Cancel :
                    default:
                        return;
                }
        }

    Hash::Reference.clear();
    // Réinitialise Hash::wrapper profondément.
    Abstract::initH();
    Hash::Annee.clear();
    Hash::Mois.clear();
    Hash::Budget.clear();
    Hash::Siret.clear();
    Hash::Etablissement.clear();
    Hash::Employeur.clear();
    Hash::Suppression.clear();
    Hash::fileList.clear();

    project->mainTabWidget->setCurrentIndex (0);
    project->initializeWidgetContainer();
    if (parent->dialog) parent->dialog->codeTab->resetLabel();

    fileSizeDataBase[0].clear();

    refreshProjectManagerValues(manager::refreshProjectInteractiveMode
                                 | manager::refreshAllZones);

}

void Altair::on_helpButton_clicked()
{
    QUrl url = QUrl::fromLocalFile (QCoreApplication::applicationDirPath() + "/../GUI.html");
    textAppend (STATE_HTML_TAG + QString ("Ouverture de l'aide : ") + url.toDisplayString());
    QDesktopServices::openUrl (url);
}


void Altair::displayTotalSize()
{
    static qint64 comp;
    qint64 tot = Altair::totalSize;

    if (tot != comp && v(quiet).isFalse())
        textAppend (STATE_HTML_TAG "Taille des bases de paye :  "
                    + QString::number (tot) + " B (" + QString::number (tot / (1024 * 1024)) + " Mo)");

    comp = tot;
}

void Altair::deleteGroup()
{
    updateIndexInfo();

    // Taille du conteneur de données
    //   project->widgetContainer
    // moins 1, autrement dit du nombre d'onglets contenant des fichiers de paye, moins 1

    int rank = project->getRank();

    // le conteneur fileSizeDataBase contient toutes les tailles de fichier pour tous les
    // onglets. On vide le conteneur à l'index correspondant à l'onglet supprimé

    if (fileSizeDataBase[0].size() > currentIndex)
        fileSizeDataBase[0][currentIndex].clear();

    // Il faut se fonder sur rank plutot que sur currentIndex, qui peut être sur des
    // onglets supplémentaires (type Siret...) qui ne correspondent pas à des données
    // de fichier

    if (rank > 0)
        {
            // Pour tous les onglets de rang supérieur à l'onglet courant et inférieur à l'onglet
            // maximal contenant des données, translater les données de taille de fichier d'un
            // index vers la gauche après suppression de l'onglet courant

            if (currentIndex < rank)
                {

                    for (int j = currentIndex; j < rank ; ++j)
                        {
                            fileSizeDataBase[0][j] = fileSizeDataBase[0][j + 1];
                        }
                }
        }

    // Réactualiser le projet .alt et le gestionnaire de projet à droite de l'interface

    updateProject();

    // Afficher la taille des bases de paye dans l'onglet Messages
    // Elle sera en principe inférieure à la taille avant suppression de l'onglet

    displayTotalSize();
}


void Altair::updateIndexInfo()
{
    if (project == nullptr) return;

    currentIndex = project->getCurrentIndex();
    row = project->getCurrentRow();
}

void Altair::on_deleteItem_clicked()
{
    // Ces drapeaux jouent sur updateProject()
    RefreshFlag = RefreshFlag & ~interfaceStatus::parseXml; // reparser le projet pour actualiser le gestionnaire de projet

    // Supprimer le fichier dans le projet
    updateProject();

    // Actualiser l'index courant
    updateIndexInfo();

    // Recalculer la tailler totale des données
    displayTotalSize();
}

void Altair::requestSaveProject()
{
    projectName = QFileDialog::getSaveFileName (this,
                  tr ("Entrer le nom du projet"),
                  userdatadir + "défaut.alt",
                  tr ("projets altair (*.alt)"));
    updateProject (true);
}

bool Altair::updateProject (bool requestSave)
{
    RefreshFlag = RefreshFlag | interfaceStatus::saveTree // ouvrir le fichier projet pour le modifier
                  | interfaceStatus::tree;  // actualisation le gestionnaire de projet

    setCurrentFile (projectName);

    // Si la case du dialogue de confirguration est cochée, ou si la sauvegarde est forcée
    // par requetSave = true alors réécrire le projet .alt

    if (parent->isDefaultSaveProjectChecked() || requestSave)
        writeProjectFile();

    Abstract::initH ("base", path_access (DONNEES_SORTIE));

    Abstract::initH ("lhxDir", path_access (System));

    return refreshProjectManager();
}


void Altair::setCurrentFile (const QString &fileName)
{
    setWindowModified (false);

    if (! fileName.isEmpty())
        {
            if (parent->recentFiles.isEmpty() || parent->recentFiles.at (0) != fileName)
                parent->recentFiles.prepend (fileName);

            parent->updateRecentFileActions();

        }

    parent->settings->setValue ("defaut", QVariant (fileName));
}

void Altair::assignWidgetValues()
{
    if (Hash::wrapper.isEmpty()) return;

    QVectorIterator<FAbstractWidget*> w (Abstract::abstractWidgetList);
    QList<QString> keyList = Hash::wrapper.keys();

    while (w.hasNext())
        {
            FAbstractWidget* composant = w.next();
            const QString key = composant->getHashKey();

            if (! keyList.contains (key))
                {
                    textAppend (WARNING_HTML_TAG "Le composant de clé "
                                + key +
                                " n'est pas référencé pas dans cette version des fichiers de projet Altaïr"
                                + (Hash::wrapper["version"]->isEmpty() ? "." :
                                   " (version" + v(version) + " )."));

                    continue;
                }


            if (key == "XHL")
                {
                    if (Altair::RefreshFlag & interfaceStatus::mainTabs)
                        {
                            composant->setWidgetFromXml (*Hash::wrapper[key]);
                        }
                }
            else
                {
                    if (options::RefreshFlag & interfaceStatus::optionTabs)
                        {
                            composant->setWidgetFromXml (*Hash::wrapper[key]);
                        }
                }
        }

    if (v(quiet).isFalse())
        {
            if (keyList.size() - 1 != Abstract::abstractWidgetList.size())
                {
                    // On assigne base et lhxDir en hard code donc il n'est nu dans l'abstractWidgetList ni
                    // dans le projet

                    // version est lu dans le projet mais n'a pas de composant

                    textAppend (WARNING_HTML_TAG "Le nombre de Widget à identifier ("
                                + QString::number (Abstract::abstractWidgetList.size())
                                + ") est différent du nombre de clés lues dans le projet ("
                                + QString::number (keyList.size() - 1) + ").");

                }

            QHashIterator<QString, FStringList*> w (Hash::wrapper);
            QStringList hashKeys = Abstract::hashKeys();

            QStringList exclusion = {"version", "base", "lhxDir"};

            while (w.hasNext())
                {
                    auto h = w.next();

                    if (! exclusion.contains (h.key()) && ! hashKeys.contains (h.key()))
                        textAppend (WARNING_HTML_TAG "Pas de composant de clé " + h.key()
                                    + " pour cette version (" VERSION ") de l'interface Altaïr.");
                }

            textAppend (STATE_HTML_TAG "Version du projet : "
                        + (Hash::wrapper["version"]->isEmpty() ? "non référencée." :
                           v(version).toQString()));
            textAppend (STATE_HTML_TAG "Version de l'interface : " VERSION);

        }

}

bool Altair::refreshProjectManager()
{
    // Step 1: prior to parsing
    checkEmptyProjectName();
    QFile file (projectName);

    if ((RefreshFlag & interfaceStatus::saveTreeMask) == interfaceStatus::saveTree)
        {
            bool result = true;
            if (!file.isOpen())
                result = file.open (QIODevice::ReadWrite);
            else
                result = file.seek (0);

            if (!result) return false;
        }

    qint64 filesize = file.size() ;

    // Step 2: parse le projet .alt

    if ((RefreshFlag & interfaceStatus::treeMask) == interfaceStatus::tree)
        {
            // Colorier en couleurs alternées silver et blanche le gestionnaire de projet

            QPalette palette;
            palette.setColor (QPalette::AlternateBase, QColor ("silver"));
            managerWidget->setPalette (palette);
            managerWidget->setAlternatingRowColors (true);

            if ((RefreshFlag & interfaceStatus::parseXmlMask)
                    == interfaceStatus::parseXml)  // Re-parser le fichier XML projet
                {
                    if (!file.isOpen())
                        file.open (QIODevice::ReadWrite);
                    else
                        file.seek (0);

                    if (filesize == 0)
                        {
                            textAppend (WARNING_HTML_TAG " Pas de projet en cours (défaut.alt est vide).");
                            return false;
                        }

                    parseProjectFile (&file);
                }
            else  // Ne pas reparser le XML simplement utiliser le conteneur de données
                {
                    refreshProjectManagerValues (manager::refreshProjectInteractiveMode
                                                 | manager::refreshXHLZone
                                                 | manager::refreshSystemZone);
                }
        }

    if (file.isOpen()) file.close();

    RefreshFlag =  RefreshFlag
                   & (interfaceStatus::savedOptionsMask
                      | interfaceStatus::saveTreeMask
                      | interfaceStatus::treeMask
                      | interfaceStatus::tabMask
                      | interfaceStatus::parseXmlMask) ;

    return (filesize !=  0);
}

void Altair::checkAnnumSpan()
{
    if (Hash::wrapper.isEmpty()) return;

    int r = project->getRank() - 2;
    const QStringList& years = project->getTabLabels();

    for (int i = 0; i < r; ++i)
        {
            QList<int> monthList;

            for (const QString& fileName : Hash::wrapper["XHL"]->at (i))
                monthList << Hash::Mois[fileName].toInt();

            monthList = monthList.toSet().toList();
            std::sort (monthList.begin(), monthList.end());

            QMutableListIterator<int> w (monthList);

            QString annee = years.at (i);

            if (annee != "" && annee.at (0) == '2')
                {
                    for (int z = 1; z <= 12; ++z)
                        {
                            if (! monthList.contains (z))
                                {
                                    QMessageBox::critical (nullptr, "Données incomplètes",
                                                           "Il manque des données mensuelles pour l'année " + annee +
                                                           " mois " + QString::number (z),
                                                           QMessageBox::Ok);
                                }
                        }
                }
        }
}


void Altair::dragEnterEvent (QDragEnterEvent *event)
{
    if (event->source() != this)
        {
            event->setDropAction (Qt::CopyAction);
            event->accept();
        }
}

void Altair::dragMoveEvent (QDragMoveEvent *event)
{
    if (event->source() != this)
        {
            event->setDropAction (Qt::CopyAction);
            event->accept();
        }
}


void Altair::dropEvent (QDropEvent *event)
{
    if (event->source() != this)
        {
            QList<QUrl> urlsDragged = event->mimeData()->urls();
            QStringList stringsDragged;
            int size = 0;

            for (const QUrl& url : urlsDragged)
                {
                    const QString path = url.toLocalFile();

                    if (! path.isEmpty())
                        {
                            stringsDragged << path;
                            size ++;
                        }
                }

            if (size == 0) return;
            openProjectFileCommonCode();
            updateIndexInfo();
            closeProject();
            project->addParsedTreeToListWidget (stringsDragged);
            checkAnnumSpan();
            updateProject();
        }
}

