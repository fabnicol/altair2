
#include <QFile>
#include <sys/stat.h>
#include <errno.h>
#include <QModelIndex>
#include <QtXml>
#include <QSettings>
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
    
    Hash::description["année"]=QStringList("Fichiers .xhl");
    Abstract::initializeFStringListHash("NBulletins");

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

    QModelIndex index = model->index(path_access("Tests/Exemple/Donnees/xhl"));
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

    project[0]=new FListFrame(this,
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
    outputTextEdit->setMinimumHeight(200);
    outputTextEdit->setReadOnly(true);

    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int)));

    project[0]->model=model;
    project[0]->slotList=nullptr;

    ///// Ce qui suit présupose que les connexions déclenchées par le click
    // sont préalablement traitées par FListFrame (ce qui est le cas)

    connect(project[0]->importFromMainTree, &QToolButton::clicked,
            [this]{
        updateProject();
        displayTotalSize();
        checkAnnumSpan();
    });

    /////


    project[0]->importFromMainTree->setVisible(visibility);
#ifndef USE_RIGHT_CLICK
    connect(project[0]->deleteGroupButton, SIGNAL(clicked()), this, SLOT(deleteGroup()));
    connect(project[0]->retrieveItemButton, SIGNAL(clicked()), this, SLOT(on_deleteItem_clicked()));
#endif
    QGridLayout *projectLayout = new QGridLayout;
    projectLayout->addWidget(project[0]->importFromMainTree, 0, 1);
    projectLayout->addWidget(project[0]->mainTabWidget, 0, 2);

#ifndef USE_RIGHT_CLICK
    QGridLayout *updownLayout = new QGridLayout;
    updownLayout->addWidget(project[0]->getControlButtonBox(), 0, 0);
    updownLayout->setRowMinimumHeight(1, 40);
    updownLayout->setRowMinimumHeight(3, 40);
    projectLayout->addLayout(updownLayout, 0, 3);
#endif

    mainLayout->addLayout(projectLayout);
    progressLayout->addLayout(progress->layout);
    mainLayout->addLayout(progressLayout);

    QStringList labels;
    labels << tr("") << tr("Mois") << tr("Chemin")  << tr("Taille\nFichier") << tr("Total") << tr("Siret Etablissement") << tr("Budget");
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

    projectName=QString("défaut.alt");
    QFile projectFile(projectName);

    if (projectFile.exists()) projectFile.remove();

    parent->saveProjectAs();
    //clearInterfaceAndParseProject();
    outputTextEdit->append(PARAMETER_HTML_TAG "Nouveau projet créé sous " + projectName);
}

inline void     Altair::openProjectFileCommonCode()
{

    RefreshFlag = RefreshFlag  | interfaceStatus::parseXml;

    checkEmptyProjectName();
    setCurrentFile(projectName);
    clearInterfaceAndParseProject();
    // resetting interfaceStatus::parseXml bits to 0
    RefreshFlag = RefreshFlag & (~interfaceStatus::parseXml);

    Hash::createReference(project[0]->getRank());

}

void Altair::on_openProjectButton_clicked()
{
    //if (! Hash::wrapper["XHL"]->isEmpty() && ! Hash::wrapper["XHL"]->at(0).isEmpty()) return;
    closeProject();
    projectName=QFileDialog::getOpenFileName(this,  tr("Ouvrir le projet"), QDir::currentPath(),  tr("projet altair (*.alt)"));
    if (projectName.isEmpty()) return;
    openProjectFileCommonCode();
}


void Altair::openProjectFile()
{
    //if (! Hash::wrapper["XHL"]->isEmpty() && ! Hash::wrapper["XHL"]->at(0).isEmpty()) return;
    closeProject();
    projectName=qobject_cast<QAction *>(sender())->data().toString();
    RefreshFlag = RefreshFlag | interfaceStatus::parseXml;
    openProjectFileCommonCode();
}


bool Altair::clearInterfaceAndParseProject()
{
    options::RefreshFlag = options::RefreshFlag | interfaceStatus::optionTabs;
    RefreshFlag = RefreshFlag | interfaceStatus::tree;

    QTextEdit* editor = parent->getEditor();

    if (editor) editor->clear();

    return refreshProjectManager();

}

void Altair::closeProject()
{
    int projectDimension = project[0]->getRank();

    clearProjectData();
    Altair::totalSize[0] = 0;

    for  (int i = projectDimension; i >= 0;   i--)
    {
        project[0]->mainTabWidget->removeTab(i);
        project[0]->getWidgetContainer().removeAt(i);
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


    project[0]->deleteAllGroups();

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


    project[0]->embeddingTabWidget->setCurrentIndex(0);
    project[0]->initializeWidgetContainer();
}

void Altair::on_helpButton_clicked()
{
    QUrl url=QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/../GUI.html");
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
    projectName=QFileDialog::getSaveFileName(this,  tr("Entrer le nom du projet"), "défaut.alt", tr("projets altair (*.alt)"));
    updateProject(true);
}


bool Altair::updateProject(bool requestSave)
{
    RefreshFlag = RefreshFlag 
                 | interfaceStatus::saveTree
                 | interfaceStatus::tree;


    if (parent->isDefaultSaveProjectChecked() || requestSave)
        writeProjectFile();

    setCurrentFile(projectName);

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


void Altair::assignVariables()
{
    QListIterator<FAbstractWidget*> w(Abstract::abstractWidgetList);

    if (w.hasNext())
    {
        FAbstractWidget* widget=w.next();
        if (Altair::RefreshFlag & interfaceStatus::mainTabs)
        {
            widget->setWidgetFromXml(*Hash::wrapper[widget->getHashKey()]);
        }
    }

    if (options::RefreshFlag & interfaceStatus::optionTabs)
        while (w.hasNext())
        {
            FAbstractWidget* widget=w.next();
            widget->setWidgetFromXml(*Hash::wrapper[widget->getHashKey()]);
        }
}

void Altair::assignGroupFiles(const int group_index)
{

#ifdef DEBUG
    static int last_group;
    if (group_index-last_group) outputTextEdit->append(STATE_HTML_TAG "Ajout de l'onglet " + QString::number(group_index+1));

    last_group=group_index;
#endif
}


bool Altair::refreshProjectManager()
{
    // Step 1: prior to parsing
    checkEmptyProjectName();
    QFile file(projectName);
    bool result = true;

    if ((RefreshFlag&interfaceStatus::treeMask) == interfaceStatus::tree)
    {
      //  managerWidget->clear();
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

void Altair::checkAnnumSpan()
{
    const QStringList monthRef = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
    int r = project[0]->getRank() - 1;

    for (int i = 0; i < r; ++i)
    {
        QStringList monthList;

        for (const QString& fileName : Hash::wrapper["XHL"]->at(i))
            monthList << Hash::Mois[fileName];

        monthList.removeDuplicates();

        QMutableListIterator<QString> w(monthList);

        while (w.hasNext())
        {
            QString month = w.next();

            if (month.at(0) == '0')
                w.setValue(month.remove(0, 1));
        }

        monthList.removeDuplicates();
        monthList.sort();

        QStringListIterator z(monthRef);
        while (z.hasNext())
        {
            QString currentMonth;
            if (! monthList.contains(currentMonth = z.next()))
             QMessageBox::critical(nullptr, "Données incomplètes",
                                            "Il manque des données mensuelles pour l'année " + project[0]->getTabLabels().at(i) +
                                            " mois "+ currentMonth,
                                            QMessageBox::Ok);

        }
    }
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
        closeProject();
        if (false == project[0]->addParsedTreeToListWidget(stringsDragged)) return;
        checkAnnumSpan();

        Hash::createReference(project[0]->getRank());

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
 int maxi = maximum();
 static bool old;
 static float minorIncrement;

 if (dirSize < 1)
 {
  int level = std::min(maxi, std::max(this->parent->fileRank, value()));
  level = std::max(startshift, level);
  minorIncrement += 0.1;
  setValue(std::max(level, qCeil((float) startshift + minorIncrement)));
  parent->outputTextEdit->append((QString)PROCESSING_HTML_TAG + " " +QString::number(level));
  if (value() == maxi) setValue(startshift);
  old = false;

 }
 else
 {
      qreal share=0;
      minorIncrement = 0;
      if (! old)
      {
        setRange(0, parent->size());
        parent->outputTextEdit->append((QString)PROCESSING_HTML_TAG + "Enregistrement des bases de données...");
      }

      if (parent->size() > 1)
      {
          share = static_cast<qreal>(dirSize) * OVERVALUE_DIRSIZE_SHARE_COEFFICIENT / static_cast<qreal>(parent->size());
          if (share > 1) share = 1;

#ifdef REPORT_DATABASE_PROGRESS
          parent->outputTextEdit->append((QString)PROCESSING_HTML_TAG + QString::number(static_cast<int>(share*100)) + " % des bases de données.");
#endif

      }
      else share = 0;

      setValue(std::max(startshift, static_cast<int>(share * maxi)));

      if (value() == maxi) reset();
      old = true;
  }
}

inline void FProgressBar::computeRProgressBar()
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
