
#include <QFile>
#include <sys/stat.h>
#include <errno.h>
#include <QModelIndex>
#include <QtXml>
#include <QSettings>

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
    
    Hash::description["année"]=QStringList("Fichiers .xhl");
    Hash::description["recent"]=QStringList("Récent");
    Abstract::initializeFStringListHash("NBulletins");

}


int Altair::applyFunctionToSelectedFiles(int (Altair::*f)(int))
{

    QItemSelectionModel *selectionModel = fileTreeView->selectionModel();
    QModelIndexList  indexList=selectionModel->selectedIndexes();
    int result=0, cum_result=0;

    if (indexList.isEmpty()) return -1;
    updateIndexInfo();
    QListIterator<QModelIndex> i(indexList);

    while (i.hasNext())
    {
        QModelIndex index=i.next();

        fileTreeFile = model->filePath(index);
        result = (this->*f)(model->fileInfo(index).isDir() << 8 | model->fileInfo(index).isFile());

        if (!result)
        {
            Warning0(tr("Parcourir"),
                    tr("%1 n'est pas un fichier ou un répertoire.").arg(model->fileInfo(index).fileName()));
            return 0;
        }

        cum_result += result;
    }

    return cum_result;
}

void Altair::refreshModel()
{
    if (model) delete(model);
    model = new QFileSystemModel;
    model->setReadOnly(false);
    model->setRootPath(QDir::homePath());
    model->sort(Qt::AscendingOrder);
    model->setFilter(QDir::AllDirs|QDir::Drives|QDir::Files|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    model->setNameFilterDisables(false);
    model->setNameFilters({"*.xhl"});
}


void Altair::refreshTreeView()
{
    fileTreeView->setModel(model);
    fileTreeView->hideColumn(1);
    fileTreeView->setMinimumWidth(300);
    fileTreeView->setColumnWidth(0,300);

    fileTreeView->header()->setStretchLastSection(true);
    fileTreeView->header()->setSortIndicator(0, Qt::AscendingOrder);
    fileTreeView->header()->setSortIndicatorShown(true);

    fileTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileTreeView->setSelectionBehavior(QAbstractItemView::SelectItems);

    QModelIndex index = model->index(QDir::currentPath());
    fileTreeView->expand(index);
    fileTreeView->scrollTo(index);
}

Altair::Altair()
{
    setAttribute(Qt::WA_DeleteOnClose);
    initialize();
    setAcceptDrops(true);

    refreshModel();
    refreshTreeView();

    bool visibility =
                        #ifdef MINIMAL
                           false;
                        #else
                           true;
                        #endif



    xhlFilterButton->setToolTip("Rafraîchir l'arborescence");
    const QIcon iconAudioFilter = QIcon(QString::fromUtf8( ":/images/application-xml.png"));
    xhlFilterButton->setIcon(iconAudioFilter);
    xhlFilterButton->setIconSize(QSize(22, 22));
    xhlFilterButton->setCheckable(true);
    xhlFilterButton->setAutoFillBackground(true);
    xhlFilterButton->setVisible(visibility);

    project[0]=new FListFrame(this,
                              fileTreeView,                   // files may be imported from this tree view
                              importFiles,                     // FListFrame type
                              "XHL",                          // superordinate xml tag
                              {"Décodeur de fichiers XHL", ""},                   // project manager widget on-screen tag
                              "g",                                  // command line label
                              flags::commandLineType::altairCommandLine|flags::status::hasListCommandLine|flags::status::enabled,  // command line characteristic features
                              {" ", " -g "},                       // command line separators
                              {"fichier", "année"},                // subordinate xml tags
                              common::TabWidgetTrait::NO_EMBEDDING_TAB_WIDGET);                      //tab icon


    mkdirButton->setToolTip(tr("Créer le répertoire..."));
    const QIcon iconCreate = QIcon(QString::fromUtf8( ":/images/folder-new.png"));
    mkdirButton->setIcon(iconCreate);
    mkdirButton->setIconSize(QSize(22, 22));
    mkdirButton->setVisible(visibility);

    removeFileTreeElementsButton->setToolTip(tr("Effacer le répertoire ou le fichier..."));
    const QIcon iconremoveFileTreeElements = QIcon(QString::fromUtf8( ":/images/edit-delete.png"));
    removeFileTreeElementsButton->setIcon(iconremoveFileTreeElements);
    removeFileTreeElementsButton->setIconSize(QSize(22, 22));
    removeFileTreeElementsButton->setVisible(visibility);

    progress=new FProgressBar(this, &Altair::killProcess);

    progress->setToolTip(tr("Décodage"));

    outputTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    outputTextEdit->setAcceptDrops(false);
    outputTextEdit->setMinimumHeight(200);

    QGridLayout *projectLayout = new QGridLayout;
    QGridLayout *updownLayout = new QGridLayout;
    QVBoxLayout *mkdirLayout = new QVBoxLayout;

    mkdirLayout->addWidget(mkdirButton);
    mkdirLayout->addWidget(removeFileTreeElementsButton);
    mkdirLayout->addWidget(xhlFilterButton);
    projectLayout->addLayout(mkdirLayout,0,0);

    connect(mkdirButton, SIGNAL(clicked()), this, SLOT(createDirectory()));
    connect(removeFileTreeElementsButton, SIGNAL(clicked()), this, SLOT(removeFileTreeElements()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int)));
    connect(xhlFilterButton, SIGNAL(clicked(bool)), this, SLOT(on_xhlFilterButton_clicked(bool)));

    project[0]->model=model;
    project[0]->slotList=nullptr;
    connect(project[0]->addGroupButton, SIGNAL(clicked()), this, SLOT(addGroup()));
    connect(project[0]->deleteGroupButton, SIGNAL(clicked()), this, SLOT(deleteGroup()));
    connect(project[0]->importFromMainTree, &QToolButton::clicked,
            [this]{
        updateProject();
        displayTotalSize();

    });
    project[0]->importFromMainTree->setVisible(visibility);
    connect(project[0]->moveUpItemButton, SIGNAL(clicked()), this, SLOT(on_moveUpItemButton_clicked()));
    connect(project[0]->moveDownItemButton, SIGNAL(clicked()), this, SLOT(on_moveDownItemButton_clicked()));
    connect(project[0]->retrieveItemButton, SIGNAL(clicked()), this, SLOT(on_deleteItem_clicked()));
    connect(project[0]->clearListButton, &QToolButton::clicked, [this] { updateProject(); displayTotalSize(); });

    projectLayout->addWidget(project[0]->tabBox, 0,2);
    updownLayout->addWidget(project[0]->getControlButtonBox(), 0,0);

    projectLayout->addWidget(project[0]->importFromMainTree, 0,1);

    updownLayout->setRowMinimumHeight(1, 40);
    updownLayout->setRowMinimumHeight(3, 40);

    projectLayout->addLayout(updownLayout, 0,3);

    mainLayout->addLayout(projectLayout);
    progressLayout->addLayout(progress->layout);
    mainLayout->addLayout(progressLayout);

    QStringList labels;
    labels << tr("") << tr("Chemin") << tr("Taille\nFichier") << tr("Total") << tr("Nombre\nBulletins");
    managerWidget->hide();
    managerWidget->setHeaderLabels(labels);
    managerWidget->setColumnWidth(0,300);
    managerWidget->setColumnWidth(1,300);
    managerWidget->setColumnWidth(2,50);
    managerWidget->setColumnWidth(3,80);
    managerWidget->setColumnWidth(4,120);
    managerWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
    managerLayout->addWidget(managerWidget);

    allLayout->addLayout(mainLayout);
    allLayout->addLayout(managerLayout);

    setLayout(allLayout);
    setWindowTitle(tr("altair-author"));
    const QIcon altairIcon=QIcon(QString::fromUtf8( ":/images/altair.png"));
    setWindowIcon(altairIcon);

}


void Altair::refreshRowPresentation()
{
    refreshRowPresentation(currentIndex);
}


void Altair::refreshRowPresentation(uint j)
{
    QPalette palette;
    palette.setColor(QPalette::AlternateBase,QColor("silver"));
    QFont font=QFont("Courier",10);

    QListWidget *widget=project[0]->getWidgetContainer(j);
    if (widget == nullptr) return;
    widget->setPalette(palette);
    widget->setAlternatingRowColors(true);
    widget->setFont(font);

    for (int r=0; (r < widget->count()) && (r < Hash::wrapper["XHL"]->at(j).size()); r++ )
    {
        widget->item(r)->setText(Hash::wrapper.value("XHL")->at(j).at(r).section('/',-1));
        widget->item(r)->setTextColor(QColor("navy"));
    }
}


void Altair::on_newProjectButton_clicked()
{
    closeProject();

    projectName=QString("défaut.alt");
    QFile projectFile(projectName);

    if (projectFile.exists()) projectFile.remove();

    parent->saveProjectAs();
    //clearInterfaceAndParseProject();
    outputTextEdit->append(PARAMETER_HTML_TAG "Nouveau projet créé sous " + projectName);
}



void Altair::on_openProjectButton_clicked()
{
    closeProject();
    projectName=QFileDialog::getOpenFileName(this,  tr("Ouvrir le projet"), QDir::currentPath(),  tr("projet altair (*.alt)"));

    if (projectName.isEmpty()) return;

    RefreshFlag = RefreshFlag  | interfaceStatus::parseXml;
    clearInterfaceAndParseProject();
    // resetting interfaceStatus::parseXml bits to 0
    RefreshFlag = RefreshFlag & (~interfaceStatus::parseXml);
}


void Altair::on_xhlFilterButton_clicked(bool active)
{
    refreshModel();
    refreshTreeView();
    model->setNameFilterDisables(active);
}

void Altair::openProjectFile()
{
    closeProject();
    projectName=qobject_cast<QAction *>(sender())->data().toString();
    RefreshFlag = RefreshFlag | interfaceStatus::parseXml;

    // only case in which XML is parsed

    clearInterfaceAndParseProject();

    // resetting interfaceStatus::parseXml bits to 0
    RefreshFlag = RefreshFlag & (~interfaceStatus::parseXml);
}


bool Altair::clearInterfaceAndParseProject(const bool cleardata)
{
    if (cleardata)
    {
        clearProjectData();
    }

    options::RefreshFlag = options::RefreshFlag | interfaceStatus::optionTabs;
    RefreshFlag = RefreshFlag | interfaceStatus::tree;

    QTextEdit* editor = parent->getEditor();

    if (editor) editor->clear();
    checkEmptyProjectName();
    setCurrentFile(projectName);

    return refreshProjectManager();

}

void Altair::closeProject()
{
    int projectDimension = project[0]->getRank();

    clearProjectData();

    Altair::totalSize[0]=0;
    displayTotalSize();

    for  (int i = projectDimension; i >=0;   i--)
    {
        project[0]->mainTabWidget->removeTab(i);
    }

    project[0]->addNewTab();

    QFile projectFile(projectName);

    projectFile.close();

    projectName="";
}


void Altair::clearProjectData()
{
    RefreshFlag =  RefreshFlag
                     | interfaceStatus::mainTabs
                     | interfaceStatus::optionTabs
                     | interfaceStatus::tree;

    int R=project[0]->getRank();

    for (int i=1; 2*i <= R+1; i++)
    {
        /* i <= R-i+1, majorant = nombre de groupes restants */
        project[0]->deleteGroup(i, R-i+1);
    }

    project[0]->on_clearList_clicked();

    project[0]->clearWidgetContainer();
    fileSizeDataBase[0].clear();


    managerWidget->clear();

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


    project[0]->embeddingTabWidget->setCurrentIndex(0);
    project[0]->initializeWidgetContainer();


    /* cleanly wipe out main Hash */
    Abstract::initializeFStringListHashes();
}

void Altair::on_helpButton_clicked()
{
    QUrl url=QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/GUI.html");
    outputTextEdit->append(STATE_HTML_TAG + QString(" Ouverture de l'aide : ") + url.toDisplayString());
    browser::showPage(url);
}


void Altair::addGroup()
{
    updateIndexInfo();

}


void Altair::displayTotalSize()
{
    static qint64 comp;
    qint64 tot=Altair::totalSize[0];
    if (tot != comp)
        outputTextEdit->append(STATE_HTML_TAG "Taille du projet :  " + QString::number(tot) + " B ("+QString::number(tot/(1024*1024))+" Mo)");
    comp=tot;
}

void Altair::deleteGroup()
{
    updateIndexInfo();
    uint rank=(uint) project[0]->getRank();

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
    currentIndex=project[0]->getCurrentIndex();
    row=project[0]->getCurrentRow();
    // row = -1 if nothing selected
}


void Altair::on_moveUpItemButton_clicked()
{
    updateIndexInfo();
    if (row == 0) return;
    fileSizeDataBase[0][currentIndex].swap(row, row-1);

    RefreshFlag = RefreshFlag 
                 | interfaceStatus::saveTree
                 | interfaceStatus::tree;
    updateProject();
    refreshRowPresentation();
}

void Altair::on_moveDownItemButton_clicked()
{
    updateIndexInfo();
    if (row < 0) return;
    if (row == project[0]->getCurrentWidget()->count() -1) return;

    fileSizeDataBase[0][currentIndex].swap(row, row+1);
    RefreshFlag = RefreshFlag 
                 | interfaceStatus::saveTree
                 | interfaceStatus::tree;
    updateProject();
    refreshRowPresentation();
}


void Altair::on_deleteItem_clicked()
{
    RefreshFlag = RefreshFlag 
                 | interfaceStatus::saveTree
                 | interfaceStatus::tree;
    updateProject();
    updateIndexInfo();
    displayTotalSize();
}


void Altair::createDirectory()
{
    QModelIndex index = fileTreeView->currentIndex();
    if (!index.isValid())
        return;

    QString dirName = QInputDialog::getText(this, tr("Répertoire"), tr("Nom du répertoire"));

    if (!dirName.isEmpty())
    {
        if (!model->mkdir(index, dirName).isValid())
            Warning0(tr("Répertoire"), tr("Le répertoire n'a pas été créé"));
    }
}


inline int Altair::removeFileTreeElement(int flag)
{
    int result=0;

    outputTextEdit->append(fileTreeFile+QString::number(flag));
    if (flag & 0x01) result=QFile(fileTreeFile).remove();
    else if(flag & 0x0100)
    {
        QDir d = QDir(fileTreeFile);
        if (d.exists())
            result = d.removeRecursively();

    }

    return result;

}

void Altair::removeFileTreeElements()
{
    int result=0;

    result=applyFunctionToSelectedFiles(&Altair::removeFileTreeElement);

    if (!result)
        Warning0(tr("Supprimer"), tr("Le fichier n'a pas pu être supprimé."));
}



void Altair::requestSaveProject()
{
    projectName=QFileDialog::getSaveFileName(this,  tr("Entrer le nom du projet"), "défaut.alt", tr("projets altair (*.alt)"));
    updateProject(true);
}


bool Altair::updateProject(bool requestSave)
{
    RefreshFlag = RefreshFlag 
                 | interfaceStatus::saveTree
                 | interfaceStatus::tree;
    xhlFilterButton->setToolTip("Show audio files with extension ");

    if (parent->isDefaultSaveProjectChecked() || requestSave)
        writeProjectFile();

    return refreshProjectManager();
}

/* Remember that the first two elements of the FAvstractWidgetList are DVD-A and DVD-V respectively, which cuts down parsing time */


void Altair::setCurrentFile(const QString &fileName)
{
    curFile =fileName;
    setWindowModified(false);

    QString shownName = "Sans titre";

    if (!curFile.isEmpty())
    {
        shownName =parent->strippedName(curFile);
        parent->recentFiles.prepend(curFile);
        parent->updateRecentFileActions();
    }

    parent->settings->setValue("defaut", QVariant(curFile));
}


void Altair::assignVariables()
{
    QListIterator<FAbstractWidget*> w(Abstract::abstractWidgetList);

    if (w.hasNext())
    {
        FAbstractWidget* widget=w.next();
        if (Altair::RefreshFlag&interfaceStatus::mainTabs)
        {
            widget->setWidgetFromXml(*Hash::wrapper[widget->getHashKey()]);
        }
    }

    if (options::RefreshFlag&interfaceStatus::optionTabs)
        while (w.hasNext())
        {
            FAbstractWidget* widget=w.next();
            widget->setWidgetFromXml(*Hash::wrapper[widget->getHashKey()]);
        }

}

void Altair::assignGroupFiles(const int group_index)
{
    static int last_group;
    if (group_index-last_group) outputTextEdit->append(STATE_HTML_TAG "Ajout de l'année " + QString::number(group_index+1));
    last_group=group_index;
}


bool Altair::refreshProjectManager()
{
    // Step 1: prior to parsing
    checkEmptyProjectName();
    QFile file(projectName);
    bool result = true;

    if ((RefreshFlag&interfaceStatus::treeMask) == interfaceStatus::tree)
    {
        managerWidget->clear();
    }

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

    if ((RefreshFlag&interfaceStatus::treeMask) == interfaceStatus::tree)
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
                outputTextEdit->append(WARNING_HTML_TAG " Pas de projet en cours (défault.alt est vide).");
                return false;
            }

            parseProjectFile(&file);

        }
        else  // refresh display using containers without parsing xml file
        {
            refreshProjectManagerValues(manager::refreshProjectInteractiveMode 
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
        if (false == project[0]->addParsedTreeToListWidget(stringsDragged, size)) return;
        updateProject();

    }

}



void FProgressBar::stop()
{
    if (parent->process->state() == QProcess::Running
        ||
        (parent->process->exitStatus() == QProcess::NormalExit))
        {
            if (bar->value() < bar->maximum()) bar->setValue(bar->maximum());
        }
        else
        {
            rewind();
        }

    timer->stop();
    killButton->setDisabled(true);
}



inline void FProgressBar::computeLHXProgressBar()
{
 QString dir = Hash::wrapper["base"]->toQString();
 qint64 dirSize = parent->getDirectorySize(dir, "*.csv");
 if (parent->process->state() != QProcess::Running) return;

 if (dirSize < 1)
 {
  int level = this->parent->fileRank * Hash::wrapper["processType"]->toInt();
  setValue((level >= startshift)? level : std::min(bar->maximum(), std::max(qCeil(value() + 0.1), startshift)));

  if (value() == maximum()) setValue(startshift);

 }
 else
 {
      qreal share;

      setRange(0, parent->size());

      if (parent->size() > 1) {
          share = static_cast<qreal>(dirSize) * 1.5 / static_cast<qreal>(parent->size());
          if (share > 1) share = 1;

          parent->outputTextEdit->append((QString)PROCESSING_HTML_TAG + QString::number(static_cast<int>(share*100)) + " % des bases de données.");
      }

      setValue(std::max(startshift, static_cast<int>(share * maximum())));

      if (value() == maximum()) setValue(startshift);
  }

}

inline void FProgressBar::computeRProgressBar()
{

      setRange(0, 100);

      parent->outputTextEdit->append((QString)PROCESSING_HTML_TAG + QString::number(static_cast<int>(parent->fileRank)) + " % de l'analyse des données.");

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

    this->parent=parent;

    connect(timer,
            &QTimer::timeout,
            [parent,this] {
                 if (parent->outputType[0] == 'L')
                  computeLHXProgressBar();
                 else
                  computeRProgressBar();
    });
    
    connect(killButton, &QToolButton::clicked, parent, killFunction);
    connect(parent->process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(stop()));
}
