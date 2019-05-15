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

#include <QFile>
#include <sys/stat.h>

#include <QModelIndex>
#include <QtXml>
#include <QSettings>
#include <mutex>
#include "fstring.h"
#include "altair.h"
#include "options.h"
#include "browser.h"
#include "fstring.h"
#include "tags.h"


std::uint16_t Altair::RefreshFlag = interfaceStatus::hasUnsavedOptions;
qint64   Altair::totalSize[]={0,0};

class Hash;


void Altair::initialize()
{
    adjustSize();
#ifdef Q_OS_WIN
    username = "Public";
#else
    username = qgetenv("USER");
    if (username.isEmpty())
       username = qgetenv("USERNAME");
#endif
    const QString xhl = path_access(QString(DONNEES_XHL)) + QDir::separator();
    #ifdef Q_OS_WIN
        userdatadir = xhl;
    #else
       userdatadir = username == "fab" ? xhl : xhl + username + QDir::separator() ;
    #endif

    if (! QFileInfo(userdatadir).isDir())
    {
        userdatadir = xhl;
    }
            
    Hash::description["année"]=QStringList("Fichiers .xhl");
#if 0
    Abstract::initH("NBulletins");
#endif
}


void Altair::refreshModel()
{
    delete(model);
    model = new QFileSystemModel;
    model->setReadOnly(false);
    model->setRootPath("");
    
    model->sort(Qt::AscendingOrder);
    model->setFilter(QDir::AllDirs|QDir::Files|QDir::NoDotAndDotDot);
    model->setNameFilterDisables(false);
    model->setNameFilters(XML_FILTERS);
}


void Altair::refreshTreeView(bool create)
{
    if (create)
    {
       fileTreeView = new QTreeView;
    }
    
    fileTreeView->setModel(model);
    
    fileTreeView->setRootIndex(model->index(userdatadir));
    fileTreeView->hideColumn(1);
    fileTreeView->hideColumn(2);
    fileTreeView->hideColumn(3);
    fileTreeView->setMinimumWidth(300);
    fileTreeView->setColumnWidth(0,300);
    fileTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);
    fileTreeView->header()->setStretchLastSection(true);
    fileTreeView->expandAll();  // ne semble pas fonctionner
    fileTreeView->setSortingEnabled(true);
    fileTreeView->sortByColumn(0, Qt::AscendingOrder); //  note: doc Qt5 erronée. Il faut préciser cette option qui n'est pas un défaut.
}



Altair::Altair()
{
    setAttribute(Qt::WA_DeleteOnClose);
    initialize();
    setAcceptDrops(true);
    
    refreshModel();
    refreshTreeView(true);

    bool visibility =
                        #ifdef MINIMAL
                           false;
                        #else
                           true;
                        #endif

    project=new FListFrame(this,
                              fileTreeView,                   // files may be imported from this tree view
                              importFiles,                     // FListFrame type
                              "XHL",                          // superordinate xml tag
                              {"Décodeur de fichiers XHL", ""},                   // project manager widget on-screen tag
                              "g",                                  // command line label
                              flags::commandLineType::altairCommandLine|flags::status::hasListCommandLine|flags::status::enabled,  // command line characteristic features
                              {" ", " -g "},                       // command line separators
                              {"item", "onglet"},                // subordinate xml tags
                              common::TabWidgetTrait::NO_EMBEDDING_TAB_WIDGET);                      //tab icon

    progress=new FProgressBar(this, &Altair::killProcess);

    progress->setToolTip(tr("Décodage"));

    outputTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outputTextEdit->setAcceptDrops(false);
    //outputTextEdit->setMinimumHeight(200);
    outputTextEdit->setReadOnly(true);

    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int)));

    project->model=model;
    project->slotList=nullptr;

    ///// Ce qui suit présupose que les connexions déclenchées par le click
    // sont préalablement traitées par FListFrame (ce qui est le cas)
                
    connect(project->importFromMainTree,
            &QToolButton::clicked,
            [this]{
        updateProject();
        displayTotalSize();
        checkAnnumSpan();

    });


    /////


    project->importFromMainTree->setVisible(visibility);
#ifndef USE_RIGHT_CLICK
    connect(project->deleteGroupButton, SIGNAL(clicked()), this, SLOT(deleteGroup()));
    connect(project->retrieveItemButton, SIGNAL(clicked()), this, SLOT(on_deleteItem_clicked()));
#endif

        
    QGridLayout *projectLayout = new QGridLayout;
    projectLayout->addWidget(project->importFromMainTree, 0, 1);
    projectLayout->addWidget(project->mainTabWidget, 0, 2);

#ifndef USE_RIGHT_CLICK
    QGridLayout *updownLayout = new QGridLayout;
    updownLayout->addWidget(project->getControlButtonBox(), 0, 0);
    updownLayout->setRowMinimumHeight(1, 40);
    updownLayout->setRowMinimumHeight(3, 40);
    projectLayout->addLayout(updownLayout, 0, 3);
#endif

    mainLayout->addLayout(projectLayout);
    progressLayout->addLayout(progress->getLayout());
    mainLayout->addLayout(progressLayout);

    QStringList labels;
    labels << tr("") << tr("Mois") << tr("Chemin")  << tr("Taille\nFichier") << tr("Total") << tr("Employeur Siret Etablissement") << tr("Budget");
    managerWidget->hide();
    managerWidget->setHeaderLabels(labels);
    managerWidget->setColumnWidth(0,300);
    managerWidget->setColumnWidth(1,50);
    managerWidget->setColumnWidth(2,300);
    managerWidget->setColumnWidth(3,50);
    managerWidget->setColumnWidth(4,60);
    managerWidget->setColumnWidth(5,220);
    managerWidget->setColumnWidth(6,90);
    managerWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
    managerLayout->addWidget(managerWidget);

    allLayout->addLayout(mainLayout);
    allLayout->addLayout(managerLayout);

    setContextMenuPolicy(Qt::CustomContextMenu);

    setLayout(allLayout);
    setWindowTitle(tr("altair-author"));
    const QIcon altairIcon=QIcon(QString::fromUtf8( ":/images/altair.png"));
    setWindowIcon(altairIcon);
    
    
}

void Altair::importData()
{ 
   const QString cdROM = cdRomMounted();

   if (! cdROM.isEmpty())
   {
        QDir c = QDir(cdROM, "", QDir::IgnoreCase, QDir::Drives|QDir::Dirs);

       if (c.exists()
           && ! c.QDir::entryInfoList(QDir::Dirs
                                     | QDir::Drives
                                     | QDir::Files
                                     | QDir::NoDotAndDotDot).isEmpty())
       {
           outputTextEdit->append(PROCESSING_HTML_TAG "Analyse du disque optique...Veuillez patienter...");
           fileTreeView->setCurrentIndex(model->index(cdROM));

           project->importFromMainTree->click();
           
           return;
       }
   }

   QDir d = QDir(userdatadir);
   if (d.exists() && ! d.QDir::entryInfoList(QDir::Dirs
                                            | QDir::Files
                                            | QDir::NoDotAndDotDot).isEmpty())
   {
       fileTreeView->setCurrentIndex(model->index(userdatadir));    
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
    refreshRowPresentation(currentIndex);
}


void Altair::refreshRowPresentation(uint j)
{
    if (Hash::wrapper.isEmpty()) return;

    QPalette palette;
    palette.setColor(QPalette::AlternateBase,QColor("silver"));
    QFont font=QFont("Courier",10);

    QListWidget *widget=project->getWidgetContainer(j);
    if (widget == nullptr) return;
    widget->setPalette(palette);
    widget->setAlternatingRowColors(true);
    widget->setFont(font);
    widget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //widget->setSelectionBehavior(QAbstractItemView::ExtendedSelection);
    QStringList strL = Hash::wrapper["XHL"]->at(j);
    strL.sort();
    int size = strL.size();
    for (int r=0; (r < widget->count()) && (r < size); r++ )
    {
        widget->item(r)->setText(strL.at(r).section('/',-1));
        widget->item(r)->setTextColor(QColor("navy"));
    }
}


void Altair::on_newProjectButton_clicked()
{
    closeProject();

    projectName=QString(userdatadir + "défaut.alt");
    QFile projectFile(projectName);

    if (projectFile.exists()) projectFile.remove();

    parent->saveProjectAs();
    //clearInterfaceAndParseProject();
    outputTextEdit->append(PARAMETER_HTML_TAG "Nouveau projet créé sous " + projectName);
}

void  Altair::openProjectFileCommonCode()
{
    RefreshFlag = RefreshFlag  | interfaceStatus::parseXml;

    checkEmptyProjectName();
    setCurrentFile(projectName);
    //geom = parent->geometry();
    clearInterfaceAndParseProject();
    //parent->setGeometry(geom);

    // resetting interfaceStatus::parseXml bits to 0
    RefreshFlag = RefreshFlag & (~interfaceStatus::parseXml);

    Hash::createReference(project->getRank());
}

void Altair::on_openProjectButton_clicked()
{
    //if (! Hash::wrapper["XHL"]->isEmpty() && ! Hash::wrapper["XHL"]->at(0).isEmpty()) return;
    closeProject();
    projectName=QFileDialog::getOpenFileName(this,  tr("Ouvrir le projet"), userdatadir,  tr("projet altair (*.alt)"));
    if (projectName.isEmpty()) return;
    openProjectFileCommonCode();
}


void Altair::openProjectFile()
{
    //if (! Hash::wrapper["XHL"]->isEmpty() && ! Hash::wrapper["XHL"]->at(0).isEmpty()) return;
    closeProject();
    projectName=qobject_cast<QAction *>(sender())->data().toString();

    openProjectFileCommonCode();
}


bool Altair::clearInterfaceAndParseProject()
{
    options::RefreshFlag = options::RefreshFlag | interfaceStatus::optionTabs;
    RefreshFlag = RefreshFlag | interfaceStatus::tree;

    QTextEdit* editor = parent->getEditor();

    try { if (editor) editor->clear(); }
    catch (...) {}
    
    return refreshProjectManager();

}

void Altair::closeProject()
{
    int projectDimension = project->getRank();

    clearProjectData();
    Altair::totalSize[0] = 0;

    for  (int i = projectDimension; i >= 0;   i--)
    {
        project->mainTabWidget->removeTab(i);
        auto widgetV = project->getWidgetContainer();
        int size = widgetV.size();
        if (i < size) widgetV.removeAt(i);
    }

    project->addNewTab();

    QFile projectFile(projectName);
    projectFile.close();
    //projectName=;
}


void Altair::clearProjectData()
{
    RefreshFlag =  RefreshFlag
                     | interfaceStatus::mainTabs
                     | interfaceStatus::optionTabs
                     | interfaceStatus::tree;


    project->deleteAllGroups();

    fileSizeDataBase[0].clear();

    refreshProjectManagerValues(manager::refreshProjectInteractiveMode
                                | manager::refreshXHLZone
                                | manager::refreshSystemZone);
    int choice = 2;

    if (options::RefreshFlag ==  interfaceStatus::hasUnsavedOptions)
    {
        choice=QMessageBox::information(this,
                                        "Nouveaux paramètres",
                                        "Ce projet contient de nouveaux paramètres.\nAppuyer sur OK pour les sauvegarder,\nsinon sur Non\nou sur Fermer pour quitter.\n",
                                        "Oui", "Non", "Fermer");
        switch (choice)
        {
        case 0  :
            // parent->dialog->clearOptionData();
            break;

        case 1 :
            options::RefreshFlag = interfaceStatus::keepOptionTabs;
            break;

        case 2 :
        default:
            return;
            break;
        }
    }

    Hash::Reference.clear();
    Hash::Annee.clear();
    Hash::Mois.clear();
    Hash::Budget.clear();
    Hash::Siret.clear();
    Hash::Etablissement.clear();
    Hash::Employeur.clear();
    Hash::Suppression.clear();
    Hash::fileList.clear();


    project->mainTabWidget->setCurrentIndex(0);
    project->initializeWidgetContainer();
    parent->dialog->codeTab->resetLabel();
}

void Altair::on_helpButton_clicked()
{
    QUrl url=QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/../GUI.html");
    outputTextEdit->append(STATE_HTML_TAG + QString("Ouverture de l'aide : ") + url.toDisplayString());
    browser::showPage(url);
}

void Altair::displayTotalSize()
{
    static qint64 comp;
    qint64 tot=Altair::totalSize[0];
    if (tot != comp && v(quiet).isFalse())
        outputTextEdit->append(STATE_HTML_TAG "Taille des bases de paye :  " + QString::number(tot) + " B ("+QString::number(tot/(1024*1024))+" Mo)");
    comp=tot;
}

void Altair::deleteGroup()
{
    updateIndexInfo();
    uint rank=(uint) project->getRank();

    if ((uint) fileSizeDataBase[0].size() > currentIndex)
        fileSizeDataBase[0][currentIndex].clear();

    if (rank > 0)
    {
        if (currentIndex < rank)
        {

            for (unsigned j=currentIndex; j < rank ; j++)
            {
                fileSizeDataBase[0][j]=fileSizeDataBase[0][j+1];
            }
        }
    }

    updateProject();
    displayTotalSize();
}

static bool firstSelection=true;

void Altair::updateIndexChangeInfo()
{
    static uint oldVideo;
    static uint oldCurrentIndex;
    static int oldRow;
    hasIndexChanged=(0 != oldVideo) | (currentIndex != oldCurrentIndex) |  (row != oldRow);
    if (firstSelection) hasIndexChanged=false;

    emit(hasIndexChangedSignal());

    oldVideo=0;
    oldCurrentIndex=currentIndex;
    oldRow=row;
    firstSelection=false;
}

void Altair::updateIndexInfo()
{
    if (project == nullptr) return;

    currentIndex = project->getCurrentIndex();
    row = project->getCurrentRow();

    // row = -1 if nothing selected
}


void Altair::on_deleteItem_clicked()
{
    RefreshFlag = RefreshFlag & ~interfaceStatus::parseXml;
    RefreshFlag = RefreshFlag | interfaceStatus::saveTree
                              | interfaceStatus::tree;

    updateProject();
    updateIndexInfo();
    displayTotalSize();
}



void Altair::requestSaveProject()
{
    projectName=QFileDialog::getSaveFileName(this,  tr("Entrer le nom du projet"), userdatadir + "défaut.alt", tr("projets altair (*.alt)"));
    updateProject(true);
}


bool Altair::updateProject(bool requestSave)
{
    RefreshFlag = RefreshFlag
                 | interfaceStatus::saveTree
                 | interfaceStatus::tree;

    setCurrentFile(projectName);

    if (parent->isDefaultSaveProjectChecked() || requestSave)
        writeProjectFile();

    
# ifndef INSERT_DIRPAGE
           Abstract::initH("base", path_access("Tests/Exemple/Donnees/" AltairDir));
           Abstract::initH("lhxDir", path_access(System));
# endif  

    return refreshProjectManager();
}

/* Remember that the first two elements of the FAvstractWidgetList are DVD-A and DVD-V respectively, which cuts down parsing time */


void Altair::setCurrentFile(const QString &fileName)
{
    setWindowModified(false);

    if (! fileName.isEmpty())
    {
        if (parent->recentFiles.isEmpty() || parent->recentFiles.at(0) != fileName)
          parent->recentFiles.prepend(fileName);
        parent->updateRecentFileActions();

    }

    parent->settings->setValue("defaut", QVariant(fileName));
}

void Altair::assignWidgetValues()
{
    if (Hash::wrapper.isEmpty()) return;
    QVectorIterator<FAbstractWidget*> w(Abstract::abstractWidgetList);
    QList<QString> keyList = Hash::wrapper.keys();

    while (w.hasNext())
    {
        FAbstractWidget* widget = w.next();
        const QString key = widget->getHashKey();

        if (! keyList.contains(key))
        {
            outputTextEdit->append(WARNING_HTML_TAG "Le Widget de clé "
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
               widget->setWidgetFromXml(*Hash::wrapper[key]);
            }
        }
        else
        {
            if (options::RefreshFlag & interfaceStatus::optionTabs)
            {
                  widget->setWidgetFromXml(*Hash::wrapper[key]);
            }
        }
    }

    if (v(quiet).isFalse())
    {
        if (keyList.size() - 1 != Abstract::abstractWidgetList.size())
        {
            // On assigne base et lhxDir en hard code donc il n'est nu dans l'abstractWidgetList ni
            // dans le projet

            // version est lu dans le projet mais n'a pas de Widget

            outputTextEdit->append(WARNING_HTML_TAG "Le nombre de Widget à identifier ("
                                   + QString::number(Abstract::abstractWidgetList.size())
                                   + ") est différent du nombre de clés lues dans le projet ("
                                   + QString::number(keyList.size() - 1) +").");

        }
            QHashIterator<QString, FStringList*> w(Hash::wrapper);
            QStringList hashKeys = Abstract::hashKeys();

            QStringList exclusion = {"version", "base", "lhxDir"};

            while (w.hasNext())
            {
               auto h = w.next();
               if (! exclusion.contains(h.key()) && ! hashKeys.contains(h.key()))
                       outputTextEdit->append(WARNING_HTML_TAG "Pas de Widget de clé " + h.key()
                                              + " pour cette version (" VERSION ") de l'interface Altaïr.");
            }

            outputTextEdit->append(STATE_HTML_TAG "Version du projet : "
                                   + (Hash::wrapper["version"]->isEmpty() ? "non référencée." :
                                      v(version)));
            outputTextEdit->append(STATE_HTML_TAG "Version de l'interface : " VERSION);

    }

}

bool Altair::refreshProjectManager()
{
    // Step 1: prior to parsing
    checkEmptyProjectName();
    QFile file(projectName);
    bool result = true;

//    if ((RefreshFlag&interfaceStatus::treeMask) == interfaceStatus::tree)
//    {
//      //  managerWidget->clear();
//    }

    if ((RefreshFlag&interfaceStatus::saveTreeMask) == interfaceStatus::saveTree)
    {
        if (!file.isOpen())
            result = file.open(QIODevice::ReadWrite);
        else
            result = file.seek(0);
        if (!result) return false;
    }

    qint64 filesize = file.size() ;

    // Step 2: parsing on opening .dvp project  (=update tree +refresh tabs) or adding/deleting tab files (=update tree)

    if ((RefreshFlag & interfaceStatus::treeMask) == interfaceStatus::tree)
    {

        QPalette palette;
        palette.setColor(QPalette::AlternateBase,QColor("silver"));
        managerWidget->setPalette(palette);
        managerWidget->setAlternatingRowColors(true);

        if ((RefreshFlag & interfaceStatus::parseXmlMask) == interfaceStatus::parseXml)  // refresh display by parsing xml file again
        {

            if (!file.isOpen())
                file.open(QIODevice::ReadWrite);
            else
                file.seek(0);

            if (filesize == 0)
            {
                outputTextEdit->append(WARNING_HTML_TAG " Pas de projet en cours (défaut.alt est vide).");
                return false;
            }

            parseProjectFile(&file);

        }
        else  // refresh display using containers without parsing xml file
        {
            refreshProjectManagerValues(manager::refreshProjectInteractiveMode
                                        | manager::refreshNBulletins
                                        | manager::refreshXHLZone
                                        | manager::refreshSystemZone);
        }
    }

    if (file.isOpen()) file.close();

    RefreshFlag =  RefreshFlag
                       & (interfaceStatus::hasSavedOptionsMask
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

        for (const QString& fileName : Hash::wrapper["XHL"]->at(i))
            monthList << Hash::Mois[fileName].toInt();

        monthList = monthList.toSet().toList();
        std::sort(monthList.begin(), monthList.end());

        QMutableListIterator<int> w(monthList);

        QString annee = years.at(i);
        if (annee != "" && annee.at(0) == '2')
        {
            for (int z = 1; z <= 12; ++z)
            {
                if (! monthList.contains(z))
                {
                    QMessageBox::critical(nullptr, "Données incomplètes",
                                                    "Il manque des données mensuelles pour l'année " + annee +
                                                    " mois "+ QString::number(z),
                                                    QMessageBox::Ok);
                }
            }
        }
    }
}

#if 0
void Altair::normaliseMultiBudgetFiles(const QStringList& list)
{

    for (int i = 0; i < Hash::wrapper["XHL"]->size(); ++i)
    {
        for (const QString& str : Hash::wrapper["XHL"]->at(i))
        {

            if (Hash::Siret[str].size() == 1 || Hash::Etablissement[str].size() == 1 || ! Hash::SiretPos.contains(str)) continue;

            for (int l = 0; l < Hash::Siret[str].size() && l < Hash::Etablissement[str].size(); ++l )
            {
                if (Hash::Suppression[Hash::Siret[str].at(l) + " " + Hash::Etablissement[str].at(l)])
                    continue;
                QFile file(str);
                file.open(QIODevice::ReadOnly);

                file.seek(Hash::SiretPos[str].at(l));

                QByteArray array = file.read(Hash::SiretPos[str].at(l+1) - Hash::SiretPos[str].at(l));
                QTemporaryFile tempfile(QDir::tempPath());
                tempfile.open();
                tempfile.write(array);
                 (*Hash::wrapper["XHL"])[i] << tempfile.fileName();
                tempfile.close();
            }
        }
    }
}
#endif

void Altair::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->source() != this)
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

void Altair::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->source() != this)
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}


void Altair::dropEvent(QDropEvent *event)
{
    if (event->source() != this)
    {
        QList<QUrl> urlsDragged=event->mimeData()->urls();
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

        updateIndexInfo();
        closeProject();
        project->addParsedTreeToListWidget(stringsDragged);
        checkAnnumSpan();
        updateProject();
     }
}


void FProgressBar::stop()
{
    if (parent->process.state() == QProcess::Running
        ||
        (parent->process.exitStatus() == QProcess::NormalExit))
        {
            if (bar->value() < bar->maximum()) bar->setValue(bar->maximum());
        }
        else
        {
                showProgressBar();
        }

    timer->stop();
    killButton->setDisabled(true);
    internalState = State::Parsing;
}


void FProgressBar::computeLHXParsingProgressBar()
{
    if (parent->process.state() != QProcess::Running) return;

    int level = std::min(maximum(), this->parent->fileRank);

    if(QDir(v(base)).entryList({"*.csv"}, QDir::Files).count() > 0)
    {
           internalState = State::WritingReady;
           return;
    }

    if (value() - level > 4/5* maximum())
    {
        parent->outputTextEdit->append((QString)PROCESSING_HTML_TAG + "Analyse des bases de données...");
    }

    setValue(level);

}

void FProgressBar::computeLHXWritingProgressBar(bool print_message)
{
    if (print_message)
    {
      parent->outputTextEdit->append((QString)PROCESSING_HTML_TAG + "Enregistrement des bases de données...");
      bar->setValue(0);
      bar->setRange(0, 100);
    }

    internalState = State::WritingStarted;

    if (parent->fileRank >= 100) parent->fileRank = 0;

    setValue(std::max(parent->fileRank, value()));

#ifdef REPORT_DATABASE_PROGRESS
        parent->outputTextEdit->append((QString)PROCESSING_HTML_TAG + QString::number() + " % des bases de données.");
#endif

}


void FProgressBar::computeRProgressBar()
{

      setRange(0, 100);
#ifdef DEBUG
      parent->outputTextEdit->append((QString)PROCESSING_HTML_TAG + QString::number(static_cast<int>(parent->fileRank)) + " % de l'analyse des données.");
#endif
      setValue(parent->fileRank);

      if (parent->fileRank == 100) setValue(startshift);

}


FProgressBar::FProgressBar(Altair* parent,
                           SlotFunction  killFunction)
{
    bar->hide();
    killButton->hide();

    const QIcon iconKill = QIcon(QString::fromUtf8( ":/images/process-stop.png"));
    killButton->setIcon(iconKill);
    killButton->setIconSize(QSize(22,22));
    killButton->setToolTip(tr("Arrêter le processus"));

    layout->addWidget(killButton);
    layout->addWidget(bar);

    this->parent = parent;  // ne pas utiliser parent sans this-> dans les closures.

    internalState = State::Parsing;

    connect(timer,
            &QTimer::timeout,
            [&] {

                 if (this->parent->outputType[0] == 'L')
                 {
                  switch (internalState)
                  {
                       case State::Parsing:
                          computeLHXParsingProgressBar();
                          break;

                       case State::WritingReady:
                          computeLHXWritingProgressBar(true);
                          break;

                       case State::WritingStarted:
                          computeLHXWritingProgressBar(false);
                          break;
                  }
                 }
                 else
                  computeRProgressBar();});

    connect(&(this->parent->process), SIGNAL(started()), this, SLOT(showProgressBar()));
    connect(killButton, &QToolButton::clicked, parent, killFunction);
    connect(&this->parent->process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(stop()));
    connect(this->parent, SIGNAL(setProgressBar(int,int)), this, SLOT(setValue(int, int)));
    connect(this->parent, SIGNAL(setProgressBar(int)), this, SLOT(setValue(int)));
    connect(this->parent, &Altair::hideProgressBar, [this] { hide(); });
    connect(this->parent, &Altair::showProgressBar, [this] { bar->reset(); bar->show(); killButton->show(); killButton->setEnabled(true);});
}



