#include "flistframe.h"
#include "common.h"

#include <QMessageBox>

FListFrame::FListFrame(QObject* parent,  QAbstractItemView* tree, short import_type, const QString &hashKey,
                         const QStringList &description, const QString &command_line, int cli_type, const QStringList &separator, const QStringList &xml_tags,
                         common::TabWidgetTrait mainTabWidgetRank, QIcon *icon, QTabWidget *parentTabWidget,
                         QStringList* terms, QStringList* translation, bool showAddItemButtonValue)

{
 setAcceptDrops(true);
 Hash::counter[hashKey]=0;
 currentIndex=0;  // necessary for project parsing
 showAddItemButton=showAddItemButtonValue;
 importType=import_type;
 tags=xml_tags;
  fileTreeView=tree;
 
 frameHashKey=hashKey;

 fileListWidget = new FListWidget(hashKey,
                                  cli_type,
                                  description,
                                  command_line,
                                  separator,
                                  tags,
                                  terms,
                                  translation);

      
 initializeWidgetContainer();

 if (mainTabWidgetRank != common::TabWidgetTrait::NO_EMBEDDING_TAB_WIDGET)
   {
     if (parentTabWidget == nullptr)
       {
         embeddingTabWidget = new QTabWidget(this);
       }
     else
       {
         embeddingTabWidget = parentTabWidget;
       }

     mainTabWidget=new QTabWidget(embeddingTabWidget);
     if (icon)
       {
           embeddingTabWidget->insertTab(static_cast<int>(mainTabWidgetRank), mainTabWidget, *icon, "");
           embeddingTabWidget->setIconSize(QSize(48,48));
           embeddingTabWidget->setMovable(true);
           embeddingTabWidget->setTabToolTip(static_cast<int>(mainTabWidgetRank), description.at(0));
       }
     else
       embeddingTabWidget->insertTab(static_cast<int>(mainTabWidgetRank), mainTabWidget, xml_tags[1]);
  }
 else
  {
     embeddingTabWidget = new QTabWidget(this);
     embeddingTabWidget->addTab(fileListWidget->currentListWidget, xml_tags[1]+" 1");
     mainTabWidget=embeddingTabWidget;
  }


 mainTabWidget->addTab(fileListWidget->currentListWidget, xml_tags[1]+" 1");
 mainTabWidget->setMovable(true);

 const QIcon importIcon = QIcon(QString::fromUtf8( ":/images/document-import.png"));
 importFromMainTree->setIcon(importIcon);
 importFromMainTree->setIconSize(QSize(22, 22));

 addGroupButton->setToolTip(tr("Ajouter un nouvel onglet"));
 const QIcon iconNew = QIcon(QString::fromUtf8( ":/images/tab-new.png"));
 addGroupButton->setIcon(iconNew);
 addGroupButton->setIconSize(QSize(22,22));

 deleteGroupButton->setToolTip(tr("Enlever l'onglet courant"));
 const QIcon iconDelete = QIcon(QString::fromUtf8( ":/images/tab-close-other.png"));
 deleteGroupButton->setIcon(iconDelete);
 deleteGroupButton->setIconSize(QSize(22,22));

 moveUpItemButton->setToolTip(tr("Déplacer l'item vers le haut"));
 const QIcon iconUp = QIcon(QString::fromUtf8( ":/images/arrow-up.png"));
 moveUpItemButton->setIcon(iconUp);
 moveUpItemButton->setIconSize(QSize(22, 22));

 retrieveItemButton->setToolTip(tr("Enlever l'item"));
 retrieveItemButton->setObjectName(QString::fromUtf8("Retrieve"));
 const QIcon iconRetrieve = QIcon(QString::fromUtf8( ":/images/retrieve.png"));
 retrieveItemButton->setIcon(iconRetrieve);
 retrieveItemButton->setIconSize(QSize(22, 22));

 moveDownItemButton->setToolTip(tr("Déplacer l'item vers le bas"));
 moveDownItemButton->setObjectName(QString::fromUtf8("Down"));
 const QIcon iconDown = QIcon(QString::fromUtf8( ":/images/arrow-down.png"));
 moveDownItemButton->setIcon(iconDown);
 moveDownItemButton->setIconSize(QSize(22, 22));

 clearListButton->setToolTip(tr("Effacer tout"));
 const QIcon clearIcon = QIcon(QString::fromUtf8( ":/images/edit-clear.png"));
 clearListButton->setIcon(clearIcon);
 clearListButton->setIconSize(QSize(22,22));

 addItemButton=new QToolButton;
 addItemButton->setToolTip(tr("Ajouter un fichier"));
 const QIcon addItemIcon = QIcon(QString::fromUtf8( ":/images/list-add.png"));
 addItemButton->setIcon(addItemIcon);
 addItemButton->setIconSize(QSize(22,22));
 
 QGridLayout *controlButtonLayout=new QGridLayout;

 controlButtonLayout->addWidget(moveUpItemButton, 1,1,1,1,Qt::AlignCenter);
 if (showAddItemButton) 
     controlButtonLayout->addWidget(addItemButton, 1+ showAddItemButton,1,1,1,Qt::AlignCenter);
 controlButtonLayout->addWidget(retrieveItemButton, 2+ showAddItemButton,1,1,1,Qt::AlignCenter);
 controlButtonLayout->addWidget(moveDownItemButton, 3+ showAddItemButton,1,1,1,Qt::AlignCenter);
 controlButtonLayout->setRowMinimumHeight(4+ showAddItemButton, 50);
 controlButtonLayout->addWidget(clearListButton, 4+ showAddItemButton, 1,1,1, Qt::AlignTop);
 controlButtonLayout->addWidget(addGroupButton, 5+ showAddItemButton,1,1,1,Qt::AlignCenter);
 controlButtonLayout->addWidget(deleteGroupButton, 6+ showAddItemButton,1,1,1,Qt::AlignCenter);
  
 controlButtonBox->setLayout(controlButtonLayout);
 controlButtonBox->setFlat(true);

 QVBoxLayout *tabLayout=new QVBoxLayout;
 tabLayout->addWidget(embeddingTabWidget);
 tabBox->setLayout(tabLayout);
 tabBox->setFlat(true);

  if (importType == flags::typeIn)
 {
     fileListWidget->currentListWidget->setEditTriggers(QAbstractItemView::AllEditTriggers); 
 }
  
  
 connect(addGroupButton,
               &QToolButton::clicked,
               [this]{
                       if   ((    (isListConnected == true || isTotalConnected == true) 
                               && (getRank() >= getSlotListSize()-1))
                             ||
                               (Hash::wrapper[frameHashKey]->last().isEmpty())) 
                            return;
                        addGroup();
                     });
 
 connect(deleteGroupButton, SIGNAL(clicked()), this, SLOT(deleteGroup()));
 connect(importFromMainTree, SIGNAL(clicked()), this,  SLOT(on_importFromMainTree_clicked()));
 connect(moveUpItemButton, SIGNAL(clicked()), this, SLOT(on_moveUpItemButton_clicked()));
 connect(retrieveItemButton, SIGNAL(clicked()), this, SLOT(on_deleteItem_clicked()));
 connect(moveDownItemButton, SIGNAL(clicked()), this, SLOT(on_moveDownItemButton_clicked()));
 connect(fileListWidget, SIGNAL(open_tabs_signal(int)), this, SLOT(addGroups(int)));
 connect(clearListButton, SIGNAL(clicked()), this, SLOT(on_clearList_clicked()));
           
 connect(addItemButton, &QToolButton::clicked,  [&]{
                                                     updateIndexInfo();
                                                     QListWidgetItem *item;
                                                     fileListWidget->currentListWidget->addItem(item=new QListWidgetItem);
                                                     item->setFlags(item->flags () | Qt::ItemIsEditable); });
  
}



void FListFrame::list_connect(FComboBox* w)
{
    if (w == nullptr) return;
    isListConnected=true;
    connect(w, SIGNAL(currentIndexChanged(int)), this, SLOT(setSlotListSize(int))); 
    setSlotListSize(0);
}


void FListFrame::list_connect(FListFrame* w)
{
    if (w == nullptr) return;
    isListConnected=true;
    connect(w, SIGNAL(is_ntabs_changed(int)), this, SLOT(setSlotListSize(int))); 
    setSlotListSize(0);
}

void FListFrame::total_connect(FListFrame* w)
{
    if (w == nullptr) return;
    if (isListConnected) return;
    isTotalConnected=true;
    connect(w, SIGNAL(is_ntracks_changed(int)), this, SLOT(setSlotListSize(int))); 
    setSlotListSize(0);
}


void FListFrame::on_clearList_clicked(int currentIndex)
{
    if (currentIndex == -1)
    {
       updateIndexInfo();
       currentIndex=this->currentIndex;
    }

  if (Hash::wrapper[frameHashKey]->count() < currentIndex+1) return;

  widgetContainer[currentIndex]->clear();

  /* warning : use *[], not ->value, to modifie any list content, even subordinate */

  int count = (*Hash::wrapper[frameHashKey])[currentIndex].count();
  
  Hash::counter[frameHashKey] -= count;
  (*Hash::wrapper[frameHashKey])[currentIndex].clear();

  updateIndexInfo();
  emit(is_ntracks_changed(Hash::counter[frameHashKey]));
}

void FListFrame::on_moveUpItemButton_clicked()
{
  updateIndexInfo();

  if (!row) return;
  emit(isControlButtonClicked());
  int currentIndex=mainTabWidget->currentIndex();

  QListWidgetItem *temp=fileListWidget->currentListWidget->takeItem(row);
  QListWidgetItem *temp2=fileListWidget->currentListWidget->takeItem(row-1);
  fileListWidget->currentListWidget->insertItem(row-1, temp);
  fileListWidget->currentListWidget->insertItem(row, temp2);
  fileListWidget->currentListWidget ->setCurrentRow(row-1);

   (*Hash::wrapper[frameHashKey])[currentIndex].swap(row, row-1);
}

void FListFrame::on_deleteItem_clicked()
{
  updateIndexInfo();

  if (Hash::wrapper[frameHashKey]->at(currentIndex).isEmpty() && (importType != flags::typeIn) ) return;
  if (row <0) return;
  emit(isControlButtonClicked());
  QModelIndexList L=fileListWidget->currentListWidget->selectionModel()->selectedRows();
  int size=L.size();
  int  rank=0, localrow=0;
  while (rank < size)
  {
      localrow=L[rank].row() - rank;
      fileListWidget->currentListWidget->takeItem(localrow);
      if  (importType != flags::typeIn) 
      {
          (*Hash::wrapper[frameHashKey])[currentIndex].removeAt(localrow);
          Hash::counter[frameHashKey]--;
      }
      
      rank++;
  }

   if (localrow) fileListWidget->currentListWidget->setCurrentRow(localrow-1);
   else if (localrow==0) fileListWidget->currentListWidget->setCurrentRow(0);
   row=localrow-1;
   if (localrow <= 0) deleteGroup();
   updateIndexInfo();
   emit(is_ntabs_changed(currentIndex+1)); // emits signal of number of tabs/QListWidgets opened
   emit(is_ntracks_changed(Hash::counter[frameHashKey]));
}


void FListFrame::on_moveDownItemButton_clicked()
{
  updateIndexInfo();

  if (row < 0) return;
  if (row == fileListWidget->currentListWidget->count() -1) return;
  
  emit(isControlButtonClicked());

  QListWidgetItem *temp=fileListWidget->currentListWidget->takeItem(row+1);
  QListWidgetItem *temp2=fileListWidget->currentListWidget->takeItem(row);
  fileListWidget->currentListWidget->insertItem(row, temp);
  fileListWidget->currentListWidget->insertItem(row+1, temp2);
  fileListWidget->currentListWidget->setCurrentRow(row+1);

   (*Hash::wrapper[frameHashKey])[currentIndex].swap(row, row+1);
}


void FListFrame::addNewTab()
{
    int r=getRank();
    mainTabWidget->insertTab(r ,widgetContainer.at(r) , tags[1] + " "+ QString::number(r+1));
    mainTabWidget->setCurrentIndex(r);
}

void FListFrame::addGroup()
{
 
        fileListWidget->currentListWidget=new QListWidget;
        fileListWidget->currentListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        fileListWidget->currentListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        if (showAddItemButton) 
        {
            fileListWidget->currentListWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        }
        
        widgetContainer << fileListWidget->currentListWidget;
        if (getRank() ==  Hash::wrapper[frameHashKey]->size())
        {
             Hash::wrapper[frameHashKey]->append(QStringList());
        }
        addNewTab();
}

/* Unlike addGroup, this function is just used for reading groups from Xml */

void FListFrame::addGroups(int n)
{
    widgetContainer.clear();
    widgetContainer << fileListWidget->currentListWidget;
    for (int j=0; j <= n; j++)
   {
     if (j) addGroup();

     widgetContainer[j]->addItems((*Hash::wrapper[frameHashKey])[j]);
   }
}

void FListFrame::deleteAllGroups()
{
    mainTabWidget->clear();
    Hash::counter[frameHashKey]=0;
    Hash::wrapper[frameHashKey]->clear();
    widgetContainer.clear();
    addGroup();
}

void FListFrame::deleteGroup(int r, int R)
{
    mainTabWidget->removeTab(r);

     Hash::counter[frameHashKey] -=  Hash::wrapper[frameHashKey]->at(r).count();
     Hash::wrapper[frameHashKey]->removeAt(r);

    if (r < R)
      {
        for (int j=currentIndex; j < R+1 ; j++)
          {
            mainTabWidget->setTabText(j,  tags[1] + " " + QString::number(j+1));
          }
      }

    if (r < widgetContainer.size()) widgetContainer.removeAt(r);
    updateIndexInfo();
    emit(is_ntabs_changed(currentIndex+1)); // emits signal of number of tabs/QListWidgets opened
    emit(is_ntracks_changed(Hash::counter[frameHashKey]));
}

void FListFrame::deleteGroup()
{
 updateIndexInfo();
 int R=getRank();
 if (R < 1) return;
 deleteGroup(currentIndex, R);
}

#if 0
void FListFrame::deleteGroups(QList<int> &L)
{

 foreach (int j,  L)
   {
     mainTabWidget->removeTab(j);
     Hash::wrapper[frameHashKey]->removeAt(j);
     getRank()--;
   }

 if (L[0] <getRank())
   {

     for (int j=L[0]; j < getRank() +1 ; j++)
       {
         mainTabWidget->setTabText(j,  tags[1] + " " + QString::number(j+1));
       }
   }
}
#endif

void FListFrame::addDirectoryToListWidget(const QFileInfo& info)
{
 QStringList filters;
 filters+="*";
 QDir dir=QDir(info.canonicalFilePath());
 foreach (QFileInfo file, dir.entryInfoList(filters,QDir::AllDirs | QDir::NoDotAndDotDot|QDir::Files))
   {
     if (file.isDir())
       {
         addDirectoryToListWidget(file);
       }
     else
       {
         QString path=file.canonicalFilePath();
         addStringToListWidget(path);
       }
   }
}

void FListFrame::addStringToHash(const QString & filepath)
{
    (*Hash::wrapper[frameHashKey])[currentIndex] << filepath;
    Hash::counter[frameHashKey]++;
    updateIndexInfo();
    if (row == 0) 
    {
        emit(is_ntabs_changed(currentIndex+1)); // emits signal of number of tabs/QListWidgets opened
    }
    emit(is_ntracks_changed(Hash::counter[frameHashKey]));
}


bool FListFrame::addStringToListWidget(const QString& filepath)
{

 updateIndexInfo();
 if ((filepath.isEmpty()) || (currentIndex >= (*Hash::wrapper[frameHashKey]).count() )) return false;


 if  (!filepath.isEmpty())
 {
     fileListWidget->currentListWidget->addItem(filepath);
     fileListWidget->currentListWidget->setCurrentRow(row+1);
     addStringToHash(filepath);
 }

 
 return true;
}


bool FListFrame::addStringToListWidget(const QString& str, int rank)
{
 
 if ((str.isEmpty()) || (rank >= (*Hash::wrapper[frameHashKey]).count() )) return false;

 
 (*Hash::wrapper[frameHashKey])[rank] << str;
 Hash::counter[frameHashKey]++;
 
 if  (!str.isEmpty())
 {
      fileListWidget->currentListWidget->addItem(str);
      fileListWidget->currentListWidget->setCurrentRow(Hash::wrapper[frameHashKey]->at(rank).count()-1);
 }

  fileListWidget->currentListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
  
 return true;
}

void FListFrame::on_importFromMainTree_clicked()
{
 
 if (isListConnected || isTotalConnected)
   {
     if(getSlotListSize() == 0)
       {
         return;
       }
   }

 QItemSelectionModel *selectionModel = fileTreeView->selectionModel();
 QModelIndexList  indexList=selectionModel->selectedIndexes();

 if (indexList.isEmpty()) return;
 uint size=indexList.size();

 for (uint i = 0; i < size; i++)
   {
     QModelIndex index;
     index=indexList.at(i);

     if (importType == flags::importFiles)
       {
         const QFileInfo info=model->fileInfo(index);

         if (info.isFile())
           {
             QString filepath=info.canonicalFilePath();
      
            addStringToListWidget(filepath);
           }
         else if (info.isDir())
           {
             addDirectoryToListWidget(info);
           }
         else
           {
             QMessageBox::warning(this, tr("Parcourir"),
                                  tr("%1 n'est pas un fichier ou un répertoire.").arg(index.data().toString()));
           }
       }
     else if (importType == flags::importNames)
       {
         QString name=index.data().toString();
         //
         addStringToListWidget(name);
       }
  }
}

void  FListFrame::setSlotListSize(int s) 
{
    slotListSize=s; 
    mainTabWidget->setEnabled(s > 0);
    if (s== 0) deleteAllGroups();
}
