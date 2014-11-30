
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


int Altair::RefreshFlag=0;
qint64   Altair::totalSize[]={0,0};

class Hash;


void Altair::initialize()
{
    adjustSize();
    
    Hash::description["année"]={"Fichiers .xhl"};
    Hash::description["recent"]={"Récent"};

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
            QMessageBox::warning(this, tr("Parcourir"),
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


    xhlFilterButton->setToolTip("Rafraîchir l'arborescence");
    const QIcon iconAudioFilter = QIcon(QString::fromUtf8( ":/images/application-xml.png"));
    xhlFilterButton->setIcon(iconAudioFilter);
    xhlFilterButton->setIconSize(QSize(22, 22));
    xhlFilterButton->setCheckable(true);
    xhlFilterButton->setAutoFillBackground(true);

    project[0]=new FListFrame(nullptr,      // no parent widget
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

    removeFileTreeElementsButton->setToolTip(tr("Effacer le répertoire ou le fichier..."));
    const QIcon iconremoveFileTreeElements = QIcon(QString::fromUtf8( ":/images/edit-delete.png"));
    removeFileTreeElementsButton->setIcon(iconremoveFileTreeElements);
    removeFileTreeElementsButton->setIconSize(QSize(22, 22));

    progress=new FProgressBar(this,
                              &Altair::getDirectorySize,
                              &Altair::printBaseSize,
                              &Altair::killProcess);

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
        showFilenameOnly();
    });
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
    //progressLayout->addLayout(progress2->layout);

    mainLayout->addLayout(progressLayout);

    QStringList labels;
    labels << tr("") << tr("Chemin") << tr("Taille");
    managerWidget->hide();
    managerWidget->setHeaderLabels(labels);
    managerWidget->setColumnWidth(0,300);
    managerWidget->setColumnWidth(1,300);
    managerWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
    managerLayout->addWidget(managerWidget);

    allLayout->addLayout(mainLayout);
    allLayout->addLayout(managerLayout);

    setLayout(allLayout);
    setWindowTitle(tr("altair-author"));
    const QIcon altairIcon=QIcon(QString::fromUtf8( ":/images/altair.png"));
    setWindowIcon(altairIcon);

}

QProgressBar* Altair::getBar()  { return progress->getBar(); }


void Altair::refreshRowPresentation()
{
    // indexes are supposed to have been recently updated
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
        widget->item(r)->setToolTip(fileSizeDataBase[0].at(j).at(r)+QString("octets"));
        widget->item(r)->setText(Hash::wrapper.value("XHL")->at(j).at(r).section('/',-1));
        widget->item(r)->setTextColor(QColor("navy"));

    }
}

//TODO insert button somewhere or right-click option, and back to sort by name
void Altair::showFilenameOnly()
{
    updateIndexInfo();
    refreshRowPresentation(currentIndex);
}



void Altair::on_newProjectButton_clicked()
{
    closeProject();

    projectName=QString("défaut.alt");
    QFile projectFile(projectName);

    if (projectFile.exists()) projectFile.remove();

    parent->saveProjectAs();

    initializeProject();


    outputTextEdit->append(PARAMETER_HTML_TAG "Nouveau projet créé sous " + projectName);

}



void Altair::on_openProjectButton_clicked()
{
    closeProject();
    projectName=QFileDialog::getOpenFileName(this,  tr("Ouvrir le projet"), QDir::currentPath(),  tr("projet altair (*.alt)"));

    if (projectName.isEmpty()) return;

    RefreshFlag |=ParseXml;
    initializeProject();
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
    RefreshFlag |=ParseXml;
    initializeProject();
}


void Altair::initializeProject(const bool cleardata)
{
    if (cleardata)
    {
        clearProjectData();
    }

    options::RefreshFlag = options::RefreshFlag|UpdateOptionTabs ;
    RefreshFlag |= UpdateTree ;

    QTextEdit* editor = parent->getEditor();

    if (editor) editor->clear();

    refreshProjectManager();

    checkEmptyProjectName();
    setCurrentFile(projectName);
}

void Altair::closeProject()
{
    clearProjectData();

    Altair::totalSize[0]=0;
    displayTotalSize();

    for  (int i = project[0]->getRank()+1; i >=0;   i--)
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
    RefreshFlag = RefreshFlag|UpdateMainTabs|UpdateOptionTabs|UpdateTree;

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

    QMessageBox::StandardButton choice=QMessageBox::Cancel;

    if (options::RefreshFlag ==  hasUnsavedOptions)
    {
        choice=QMessageBox::information(this, "Nouveaux paramètres",
                                        "Ce projet contient de nouveaux paramètres.\nAppuyer sur OK pour les sauvegarder,\nsinon sur Non\nou sur Annuler pour quitter.\n",
                                        QMessageBox::Ok|QMessageBox::No|QMessageBox::Cancel);
        switch (choice)
        {
        case QMessageBox::Ok  :
            // parent->dialog->clearOptionData();
            break;

        case QMessageBox::No :
            options::RefreshFlag = KeepOptionTabs;
            break;

        case QMessageBox::Cancel :
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

    RefreshFlag |= SaveTree|UpdateTree;
    updateProject();
    refreshRowPresentation();
}

void Altair::on_moveDownItemButton_clicked()
{
    updateIndexInfo();
    if (row < 0) return;
    if (row == project[0]->getCurrentWidget()->count() -1) return;

    fileSizeDataBase[0][currentIndex].swap(row, row+1);
    RefreshFlag |= SaveTree | UpdateTree;
    updateProject();
    refreshRowPresentation();
}


void Altair::on_deleteItem_clicked()
{
    RefreshFlag |= SaveTree | UpdateTree;
    updateProject();
    updateIndexInfo();
    displayTotalSize();
}


void Altair::createDirectory()
{
    QModelIndex index = fileTreeView->currentIndex();
    if (!index.isValid())
        return;

    QString dirName = QInputDialog::getText(this, tr("Create Directory"), tr("Directory name"));

    if (!dirName.isEmpty())
    {
        if (!model->mkdir(index, dirName).isValid())
            QMessageBox::information(this, tr("Create Directory"),
                                     tr("Failed to create the directory"));
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
        QMessageBox::information(this, tr("Supprimer"),
                                 tr("Le fichier n'a pas pu être supprimé."));
}



void Altair::requestSaveProject()
{
    projectName=QFileDialog::getSaveFileName(this,  tr("Entrer le nom du projet"), "défaut.alt", tr("projets altair (*.alt)"));
    updateProject(true);
}


void Altair::updateProject(bool requestSave)
{
    RefreshFlag = SaveTree|UpdateTree ;

    xhlFilterButton->setToolTip("Show audio files with extension ");

    if (parent->isDefaultSaveProjectChecked() || requestSave)
        writeProjectFile();

    refreshProjectManager();
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
        if (Altair::RefreshFlag&UpdateMainTabs)
        {
            widget->setWidgetFromXml(*Hash::wrapper[widget->getHashKey()]);
        }
    }

    if (options::RefreshFlag&UpdateOptionTabs)
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

    if ((RefreshFlag&UpdateTreeMask) == UpdateTree)
    {
        managerWidget->clear();
    }

    if ((RefreshFlag&SaveTreeMask) == SaveTree)
    {
        if (!file.isOpen())
            file.open(QIODevice::ReadWrite);
        else
            file.seek(0);
    }

    // Step 2: parsing on opening .dvp project  (=update tree +refresh tabs) or adding/deleting tab files (=update tree)

    if ((RefreshFlag&UpdateTreeMask) == UpdateTree)
    {
        QPalette palette;
        palette.setColor(QPalette::AlternateBase,QColor("silver"));
        managerWidget->setPalette(palette);
        managerWidget->setAlternatingRowColors(true);

        if ((RefreshFlag&ParseXmlMask) == ParseXml)  // refresh display by parsing xml file again
        {

            if (!file.isOpen())
                file.open(QIODevice::ReadWrite);
            else
                file.seek(0);

            if (file.size() == 0)
            {
                outputTextEdit->append(WARNING_HTML_TAG "fichier vide !");
                return false;
            }

            DomParser(&file);

        }
        else  // refresh display using containers without parsing xml file
        {
            refreshProjectManagerValues(refreshProjectInteractiveMode | refreshAudioZone | refreshSystemZone);
        }


    }

    if (file.isOpen()) file.close();
    RefreshFlag &= hasSavedOptionsMask|SaveTreeMask|UpdateTreeMask|UpdateTabMask ;

    //altairCommandStr=parent->dialog->outputTab->applicationLineEdit->text();
    return true;

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

        QList<QUrl> urls=event->mimeData()->urls();
        if (urls.isEmpty()) return;

        QString fileName = urls.first().toLocalFile();
        if (fileName.isEmpty()) return;

        if (QFileInfo(fileName).isDir())
        {
            urls.takeFirst();
            QDir dir(fileName);
            QFileInfoList entries = dir.entryInfoList({"*.xhl"}, QDir::Files);
            for (QFileInfo & localFile: entries)
                urls << QUrl::fromLocalFile(localFile.absoluteFilePath());
        }

        addDraggedFiles(urls);
    }

}


void Altair::addDraggedFiles(const QList<QUrl>& urls)
{
    updateIndexInfo();

    for (const QUrl &u: urls)
    {
        if (false == project[0]->addStringToListWidget(u.toLocalFile())) return;
    }
    updateProject();
    showFilenameOnly();
}


void FProgressBar::stop()
{
    //if (parent->process->state() == QProcess::Running) return;
    if (parent->process->state() == QProcess::Running
        ||
        (parent->process->exitStatus() == QProcess::NormalExit))
    {
        if (bar->value() < bar->maximum()) bar->setValue(bar->maximum());
    }
    else
    {
        bar->reset();
    }

    timer->stop();
    killButton->setDisabled(true);
}

qint64 FProgressBar::updateProgressBar()
{
    if (parent->process->state() != QProcess::Running) return 0;

    qint64   new_value=(parent->*engine)(target, filter);
    int share=qCeil(bar->maximum()*(static_cast<float>(new_value)/static_cast<float>(reference)));
    if (share >= bar->maximum())
    {
        share=bar->maximum();
    }

    bar->setValue((share >= startshift)? share: startshift);
    if ( share == bar->maximum()) stop();
    return new_value;
}


FProgressBar::FProgressBar(Altair* parent,
                           MeasureFunction measureFunction,
                           DisplayFunction displayMessageWhileProcessing,
                           SlotFunction  killFunction,
                           const QString&  fileExtensionFilter,
                           const QString&  measurableTarget,
                           const qint64 referenceSize)
{
    bar->hide();
    bar->setRange(0,100);
    killButton->hide();
    const QIcon iconKill = QIcon(QString::fromUtf8( ":/images/process-stop.png"));
    killButton->setIcon(iconKill);
    killButton->setIconSize(QSize(22,22));
    killButton->setToolTip(tr("Arrêter le processus"));

    layout->addWidget(killButton);
    layout->addWidget(bar);

    target=measurableTarget;
    filter=fileExtensionFilter;
    reference=referenceSize;
    engine=measureFunction;
    this->parent=parent;
    stage_2=false;


    connect(timer,
            &QTimer::timeout,
            [this, parent, displayMessageWhileProcessing] {
             if (stage_2 && parent->process->state() == QProcess::Running)
             (this->parent->*displayMessageWhileProcessing)(updateProgressBar()); });

    connect(timer,
            &QTimer::timeout,
            [this, parent] {
        if (!stage_2 && parent->process->state() == QProcess::Running)
        {
            int value = this->parent->fileRank * Hash::wrapper["processType"]->toInt();
            bar->setValue((value >= startshift)? value: startshift);
        }
    });
    
    connect(killButton, &QToolButton::clicked, parent, killFunction);
    connect(parent->process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(stop()));
}
