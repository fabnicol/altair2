#include "flistframe.h"
#include "common.h"


#include <QMessageBox>


FListFrame::FListFrame(QObject* parent,  QAbstractItemView* tree, short import_type, const QString &hashKey,
                         const QStringList &description, const QString &command_line, int cli_type, const QStringList &separator, const QStringList &xml_tags,
                         common::TabWidgetTrait mainTabWidgetRank, QIcon *icon, QTabWidget *parentTabWidget,
                         QStringList* terms, QStringList* translation, bool showAddItemButtonValue)

{

 setAcceptDrops(true);
 altair = static_cast<Altair*>(parent);

 currentIndex=0;  // necessary for project parsing
 showAddItemButton=showAddItemButtonValue;
 importType=import_type;
 tags=xml_tags;
 fileTreeView=tree;
 
 frameHashKey=hashKey;

 fileListWidget = new FListWidget(this,
                                  hashKey,
                                  cli_type,
                                  description,
                                  command_line,
                                  separator,
                                  tags,
                                  terms,
                                  translation);

 currentListWidget = fileListWidget->currentListWidget;

 initializeWidgetContainer(currentListWidget);

 Abstract::initializeFStringListHash(frameHashKey);

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
     embeddingTabWidget->addTab(currentListWidget, xml_tags[1]+" 1");
     mainTabWidget=embeddingTabWidget;
  }


 mainTabWidget->addTab(currentListWidget, xml_tags[1]+" 1");
 mainTabWidget->setMovable(true);

 const QIcon importIcon = QIcon(QString::fromUtf8( ":/images/document-import.png"));
 importFromMainTree->setIcon(importIcon);
 importFromMainTree->setIconSize(QSize(22, 22));


 deleteGroupButton->setToolTip(tr("Enlever l'onglet courant"));
 const QIcon iconDelete = QIcon(QString::fromUtf8( ":/images/tab-close-other.png"));
 deleteGroupButton->setIcon(iconDelete);
 deleteGroupButton->setIconSize(QSize(22,22));

 retrieveItemButton->setToolTip(tr("Enlever l'item"));
 retrieveItemButton->setObjectName(QString::fromUtf8("Retrieve"));
 const QIcon iconRetrieve = QIcon(QString::fromUtf8( ":/images/retrieve.png"));
 retrieveItemButton->setIcon(iconRetrieve);
 retrieveItemButton->setIconSize(QSize(22, 22));

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

 if (showAddItemButton) 
     controlButtonLayout->addWidget(addItemButton, 1+ showAddItemButton,1,1,1,Qt::AlignCenter);
 controlButtonLayout->addWidget(retrieveItemButton, 2+ showAddItemButton,1,1,1,Qt::AlignCenter);
 controlButtonLayout->setRowMinimumHeight(4+ showAddItemButton, 50);
 controlButtonLayout->addWidget(clearListButton, 4+ showAddItemButton, 1,1,1, Qt::AlignTop);
 controlButtonLayout->addWidget(deleteGroupButton, 6+ showAddItemButton,1,1,1,Qt::AlignCenter);
  
 controlButtonBox->setLayout(controlButtonLayout);
 controlButtonBox->setFlat(true);

 QVBoxLayout *tabLayout=new QVBoxLayout;
 tabLayout->addWidget(embeddingTabWidget);
 tabBox->setLayout(tabLayout);
 tabBox->setFlat(true);

  if (importType == flags::typeIn)
 {
     currentListWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
 }

 
 connect(deleteGroupButton, SIGNAL(clicked()), this, SLOT(deleteGroup()));
 connect(importFromMainTree, SIGNAL(clicked()), this,  SLOT(on_importFromMainTree_clicked()));
 connect(clearListButton, SIGNAL(clicked()), this, SLOT(on_clearList_clicked()));
 connect(retrieveItemButton, SIGNAL(clicked()), this, SLOT(on_deleteItem_clicked()));
 connect(addItemButton, &QToolButton::clicked,  [&]{
                                                     updateIndexInfo();
                                                     QListWidgetItem *item;
                                                     currentListWidget->addItem(item=new QListWidgetItem);
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

  /* Warning : use *[], not ->value, to modifie any list content, even subordinate */

  int count = (*Hash::wrapper[frameHashKey])[currentIndex].count();
  
  Hash::counter[frameHashKey] -= count;
  (*Hash::wrapper[frameHashKey])[currentIndex].clear();

  updateIndexInfo();
  emit(is_ntracks_changed(Hash::counter[frameHashKey]));
}

void FListFrame::on_deleteItem_clicked()
{
  updateIndexInfo();

  if (Hash::wrapper[frameHashKey]->at(currentIndex).isEmpty() && (importType != flags::typeIn) ) return;
  if (row <0) return;
  emit(isControlButtonClicked());
  QModelIndexList L=currentListWidget->selectionModel()->selectedRows();
  int size=L.size();
  int  rank=0, localrow=0;

  while (rank < size)
  {
      localrow=L[rank].row() - rank;
      currentListWidget->takeItem(localrow);
      if  (importType != flags::typeIn) 
      {
          (*Hash::wrapper[frameHashKey])[currentIndex].removeAt(localrow);
          Hash::counter[frameHashKey]--;
      }
      
      rank++;
  }

   if (localrow) currentListWidget->setCurrentRow(localrow-1);
   else if (localrow==0) currentListWidget->setCurrentRow(0);
   row=localrow-1;
   if (localrow <= 0 && (*Hash::wrapper[frameHashKey])[currentIndex].isEmpty()) deleteGroup();
   updateIndexInfo();
   emit(is_ntabs_changed(currentIndex+1)); // emits signal of number of tabs/QListWidgets opened
   emit(is_ntracks_changed(Hash::counter[frameHashKey]));
}


void FListFrame::addNewTab()
{
    addNewTab(getRank());
}

void FListFrame::addNewTab(int r, const QString& label)
{
    if (r < 0) return;


    mainTabWidget->insertTab(r ,widgetContainer.at(r), label);
    mainTabWidget->setCurrentIndex(r);
}

void FListFrame::addGroup()
{
 
        currentListWidget=new QListWidget;
        fileListWidget->currentListWidget = currentListWidget;
        currentListWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
      //  currentListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        if (showAddItemButton) 
        {
            currentListWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
        }
        
        widgetContainer << currentListWidget;
        if (getRank() ==  Hash::wrapper[frameHashKey]->size())
        {
             Hash::wrapper[frameHashKey]->append(QStringList());
        }
        addNewTab();
}

/* Unlike addGroup, this function is just used for reading groups from Xml */

void FListFrame::addGroups(int n)
{

    for (int j=0; j <= n; j++)
      {

         if (j) addGroup();

         widgetContainer[j]->addItems((*Hash::wrapper[frameHashKey])[j]);
      }


}

void FListFrame::deleteAllGroups(bool insertFirstGroup, bool eraseAllData)
{
   // mainTabWidget->clear();
    on_clearList_clicked(0);

    /* cleanly wipe out main Hash */

    if (eraseAllData)
    {
          for (const QString& hashKey: Hash::wrapper.keys()) Hash::wrapper[hashKey]->clear();
    }
    else
    {
//      delete(Hash::wrapper[frameHashKey]);
      Abstract::initializeFStringListHash(frameHashKey);
    }


    Hash::Annee.clear();
    Hash::Mois.clear();
    Hash::Siret.clear();
    Hash::Etablissement.clear();
    Hash::Budget.clear();

    clearTabLabels();

  //  widgetContainer.clear();
    if (insertFirstGroup) addGroup();
}

void FListFrame::deleteGroup(int r, int R)
{
    mainTabWidget->removeTab(r);

     Hash::counter[frameHashKey] -=  Hash::wrapper[frameHashKey]->at(r).count();

     for (const QString& s : Hash::wrapper[frameHashKey]->at(r))
     {
         Hash::Annee.remove(s);
         Hash::Mois.remove(s);
         Hash::Siret.remove(s);
     }

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

void FListFrame::initializeWidgetContainer()
{
   widgetContainer =  QList<QListWidget*>() << new QListWidget;
}

void FListFrame::initializeWidgetContainer(QListWidget* listWidget)
{
   widgetContainer =  QList<QListWidget*>() << listWidget;
}

void FListFrame::clearWidgetContainer()
{
   widgetContainer.clear(); ;
}

void FListFrame::parseXhlFile(const QStringList& stringList)
{
  int rank = 0;
    for (const QString& fileName : stringList)
    {
        parseXhlFile(fileName);
        ++rank;
#ifdef DEBUG_INPUT_FILES
        altair->outputTextEdit->append(PROCESSING_HTML_TAG " Analyse du fichier n°" + QString::number(rank));
#endif
        altair->getProgressBar()->setValue(rank);
    }

}

#include "elemParser.hpp"
struct Header* elemPar;

void FListFrame::parseXhlFile(const QString& fileName)
{
   QFile file(fileName);

    bool result = file.open(QIODevice::ReadOnly | QIODevice::Unbuffered);
    if (! file.isOpen())
             altair->outputTextEdit->append(ERROR_HTML_TAG " Erreur à l'ouverture du fichier.");

    if (result == false || file.size()== 0)
    {
        altair->outputTextEdit->append(WARNING_HTML_TAG " Fichier vide.");
        return;
    }

    file.seek(0);

    char buffer[1500];
    file.read(buffer, 1500);

#ifdef REGEX_PARSING_FOR_HEADERS
    const QString string = QString::fromLatin1(buffer);

    QRegExp reg("DocumentPaye.*(?:Annee) V=\"([0-9]+)\".*(?:Mois) V=\"([0-9]+)\"(.*)(?:Etablissement|Employeur).*(?:Nom) V=\"([^\"]+)\".*(?:Siret) V=\"([0-9A-Z]+)\"");
    reg.setPatternSyntax(QRegExp::RegExp2);
    QRegExp reg2(".*Budget.*Libelle V=\"([^\"]*)\".*");

    if (string.contains(reg))
    {
        Hash::Annee[fileName] = reg.cap(1);
        Hash::Mois[fileName]  = reg.cap(2);
        QString budgetCapture = reg.cap(3);
        if (budgetCapture.contains(reg2))
           Hash::Budget[fileName] = reg2.cap(1) ;
        else
            Hash::Budget[fileName] = "Non renseigné" ;
        Hash::Etablissement[fileName]  = reg.cap(4).replace("&#39;", "\'");
        Hash::Siret[fileName] = reg.cap(5);
    }
    else
    {
        altair->outputTextEdit->append(WARNING_HTML_TAG " Fichier " + fileName + " non conforme à la spécification astre:DocumentPaye");
        Hash::Annee[fileName] = "Inconnu";
        Hash::Mois[fileName]  = "Inconnu";
        Hash::Etablissement[fileName]  = "Inconnu";
        Hash::Siret[fileName] = "Inconnu";

    }


#else

   elemPar  = elem_parser(buffer.constData());
   //if (elemPar->test)
   {
       Hash::Annee[fileName] = QString(elemPar->annee);
       Hash::Mois[fileName]  = QString(elemPar->mois);
       Hash::Siret[fileName] = QString(elemPar->siret);
   }

   free(elemPar);
#endif

   file.close();

   if (file.isOpen())
            altair->outputTextEdit->append(ERROR_HTML_TAG " Erreur à la fermeture du fichier.");

   if (file.error() != QFileDevice::NoError)
         altair->outputTextEdit->append(WARNING_HTML_TAG " Erreur de fichier.");
   return;
}


bool FListFrame::addStringListToListWidget(const QStringList& stringList)
{
    QStringList existingTabLabels;
    mainTabWidget->clear();
    clearTabLabels();

    for (int j = 0; j < getWidgetContainerCount(); j++)
    {
        const QString str = mainTabWidget->tabText(j);
        if (str == "année 1" || str.isEmpty())
        {
            mainTabWidget->removeTab(j);
            delete(mainTabWidget->widget(j));
        }
        else
          existingTabLabels << str;
    }

    altair->outputTextEdit->append(STATE_HTML_TAG " Parcours des entêtes de fichier " );
    int stringListSize = stringList.size();
    altair->getProgressBar()->setRange(0, stringListSize);
    altair->getProgressBar()->reset();
    altair->getProgressBar()->show();
    parseXhlFile(stringList);
    altair->getProgressBar()->hide();

    QStringList tabLabels = getTabLabels();

    altair->outputTextEdit->append(STATE_HTML_TAG " Calcul des labels " );

    for (const QString& fileName : stringList)
            tabLabels  +=   Hash::Annee[fileName];

    int rank = 0;
    QStringList allLabels = tabLabels + existingTabLabels;
    altair->outputTextEdit->append(STATE_HTML_TAG " Elimination des doublons " );
    allLabels.removeDuplicates();
    altair->outputTextEdit->append(STATE_HTML_TAG " Tri des labels " );
    allLabels.sort();

    if (! allLabels.isEmpty())
    {
        altair->outputTextEdit->append(STATE_HTML_TAG + QString(" Nombre d'années détectées : ") + QString::number(allLabels.size()) + " années, " + allLabels.join(", "));

        #define listWidget static_cast<QListWidget*>(mainTabWidget->widget(rank))

        for (const QString& annee : allLabels)
        {

            QStringList keys = Hash::Annee.keys(annee);
            keys.sort();

            if (! existingTabLabels.contains(annee))
            {

                widgetContainer.insert(rank, new QListWidget);
                Hash::wrapper[frameHashKey]->insert(rank, keys);
                Hash::counter[frameHashKey]++;

                addNewTab(rank, annee);
                altair->outputTextEdit->append(STATE_HTML_TAG " Ajout de l'onglet " + annee);
            }
            else
            {
                (*Hash::wrapper[frameHashKey])[rank] =  keys ;
                altair->outputTextEdit->append(STATE_HTML_TAG " Ajout des fichiers de l'onglet au conteneur principal ");
            }

            listWidget->clear();
            altair->outputTextEdit->append(STATE_HTML_TAG " Ajout des fichiers de l'onglet " + annee);
            listWidget->addItems(keys);

            altair->refreshRowPresentation(rank);
            ++rank;
        }

        mainTabWidget->setCurrentIndex(rank - 1);

        QStringList pairs;
        QStringListIterator j(Hash::Etablissement.values());
        QStringListIterator i(Hash::Siret.values());
        while (i.hasNext() && j.hasNext())
        {
            pairs << i.next() + " " + j.next();
        }
        pairs.removeDuplicates();

        int siretCount = pairs.size();

        QStringList tabList ;
        for (int i=0; i < siretCount ; i++)
            tabList <<  pairs[i].left(60);

        widgetContainer.insert(rank, new QListWidget);
        addNewTab(rank, "Siret");
        Hash::wrapper[frameHashKey]->insert(rank, pairs);
        Hash::counter[frameHashKey]++;
        altair->outputTextEdit->append(STATE_HTML_TAG " Ajout de l'onglet Siret");
        listWidget->clear();
        listWidget->addItems(tabList);

/* amongst :
 * aliceblue antiquewhite aqua aquamarine azure beige bisque black blanchedalmond blue blueviolet brown burlywood cadetblue chartreuse
 * chocolate coral cornflowerblue cornsilk crimson cyan darkblue darkcyan darkgoldenrod darkgray darkgreen darkgrey darkkhaki darkmagenta
 * darkolivegreen darkorange darkorchid darkred darksalmon darkseagreen darkslateblue darkslategray darkslategrey darkturquoise darkviolet
 * deeppink deepskyblue dimgray dimgrey dodgerblue firebrick floralwhite forestgreen fuchsia gainsboro ghostwhite gold goldenrod gray green
 *  greenyellow grey honeydew hotpink indianred indigo ivory khaki lavender lavenderblush lawngreen lemonchiffon lightblue lightcoral lightcyan
 *  lightgoldenrodyellow lightgray lightgreen lightgrey lightpink lightsalmon lightseagreen lightskyblue lightslategray lightslategrey lightsteelblue
 *  lightyellow lime limegreen linen magenta maroon mediumaquamarine mediumblue mediumorchid mediumpurple mediumseagreen mediumslateblue
 *  mediumspringgreen mediumturquoise mediumvioletred midnightblue mintcream mistyrose moccasin navajowhite navy oldlace olive olivedrab
 *  orange orangered orchid palegoldenrod palegreen paleturquoise palevioletred papayawhip peachpuff peru pink plum powderblue purple red
 * rosybrown royalblue saddlebrown salmon sandybrown seagreen seashell sienna silver skyblue slateblue slategray slategrey snow springgreen steelblue
 *  tan teal thistle tomato transparent turquoise violet wheat white whitesmoke yellow yellowgreen */

        QList<QString> colorList = { "tomato", "orange" , "yellowgreen", "green",  "darkcyan", "blue", "navy", "darkslateblue", "black"};
        const int colorListSize = colorList.size();
        for (int i=0; i < siretCount; i++)
            listWidget->item(i)->setTextColor(colorList.at(i % colorListSize));
        ++rank;

        pairs.clear();
        tabList.clear();
        pairs = Hash::Budget.values();
        pairs.removeDuplicates();
        int budgetCount = pairs.size();
        for (int i=0; i < budgetCount ; i++)
            tabList <<  pairs[i].left(60);

        widgetContainer.insert(rank, new QListWidget);
        addNewTab(rank, "Budget");
        Hash::wrapper[frameHashKey]->insert(rank, pairs);
        Hash::counter[frameHashKey]++;
        altair->outputTextEdit->append(STATE_HTML_TAG " Ajout de l'onglet Budget");
        listWidget->clear();
        listWidget->addItems(tabList);

        for (int i=0; i < budgetCount; i++)
            listWidget->item(i)->setTextColor(colorList.at(i % colorListSize));
        ++rank;

       #undef listWidget
     }

  updateIndexInfo();
  if (row == 0)
    {
        emit(is_ntabs_changed(currentIndex+1)); // emits signal of number of tabs/QListWidgets opened
    }
   emit(is_ntracks_changed(Hash::counter[frameHashKey]));

  fileListWidget->setTabLabels(allLabels);
  currentListWidget->setCurrentRow(Hash::wrapper[frameHashKey]->at(rank - 1).size());

 return true;
}


// TODO : explorer les possibilités de move semantics pour accélérer la récursion

QStringList FListFrame::parseTreeForFilePaths(const QStringList& stringList)
{
    
    QStringList stringsToBeAdded=QStringList();
    QStringListIterator i(stringList);
    if (importType != flags::importFiles) return QStringList();

    while (i.hasNext())
    {
        QString currentString = i.next();
        if (currentString.isEmpty()) return QStringList();
  
            QFileInfo info = QFileInfo(currentString);
            if (info.isDir())
              {
                QDir dir(currentString);
                QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::Files|QDir::Dirs);
                // Recursion
                  // Utilisation d'une rvalue 
                QStringList &&tempStrings=QStringList();
                for (QFileInfo & embeddedFileInfo: entries)
                {
                     tempStrings <<  embeddedFileInfo.absoluteFilePath();
                }
                
                  // Move semantics : gain de temps et de mémoire (>= Qt5.4)
                  // temStrings est coerced dans le type const QStringList sans copie de données
                                
                stringsToBeAdded << parseTreeForFilePaths(tempStrings);
                           
              }
            else
              if (info.isFile() && info.suffix() == "xhl")
                  stringsToBeAdded << currentString;
    }
    
    return stringsToBeAdded; 
}


void FListFrame::on_importFromMainTree_clicked()
{
 
 altair->outputTextEdit->append(STATE_HTML_TAG " Lancement de l'analyse " );

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
 
 QStringList&& stringsToBeAdded = QStringList();
 int stringListSize=0;
 
 altair->outputTextEdit->append(STATE_HTML_TAG " Parcours de l'arbre " );



 if (importType == flags::importFiles)
    {
     for (const QModelIndex& index : indexList)
       {
         const QString path = model->filePath(index);
         if (! path.isEmpty()) 
         {
             stringListSize++;
             stringsToBeAdded << path;
         }
       }

     altair->outputTextEdit->append(STATE_HTML_TAG " Ajout des chemins à la liste centrale" );

     if (stringListSize) 
         addParsedTreeToListWidget(stringsToBeAdded);
    }

}

void  FListFrame::setSlotListSize(int s) 
{
    slotListSize=s; 
    mainTabWidget->setEnabled(s > 0);
    if (s== 0) deleteAllGroups();
}
