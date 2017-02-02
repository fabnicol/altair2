/* Fichier à encoder en UTF-8 */

#include "flistframe.h"
#include "common.h"
#include "enums.h"

#include <QMessageBox>


FListFrame::FListFrame(QObject* parent,  QAbstractItemView* tree, short import_type, const QString &hashKey,
                         const QStringList &description, const QString &command_line, int cli_type, const QStringList &separator, const QStringList &xml_tags,
                         common::TabWidgetTrait mainTabWidgetRank,   QStringList* terms, QStringList* translation)

{

 setAcceptDrops(true);
 altair = static_cast<Altair*>(parent);

 currentIndex = 0;  // necessary for project parsing

 importType = import_type;
 tags = xml_tags;
 fileTreeView = tree;
 
 frameHashKey = hashKey;

 fileListWidget = new FListWidget(this,
                                  hashKey,
                                  cli_type,
                                  description,
                                  command_line,
                                  separator,
                                  tags,
                                  terms,
                                  translation);

  
 connect(fileListWidget, SIGNAL(forceCloseProject()), altair, SLOT(closeProject()));

 currentListWidget = fileListWidget->currentListWidget;

 initializeWidgetContainer(currentListWidget);

 Abstract::initH(frameHashKey);

 if (mainTabWidgetRank == common::TabWidgetTrait::NO_EMBEDDING_TAB_WIDGET)
  {
     mainTabWidget = new QTabWidget(this);
     mainTabWidget->addTab(currentListWidget, xml_tags[1]+" 1");
  }

 mainTabWidget->addTab(currentListWidget, xml_tags[1]+" 1");
 mainTabWidget->setMovable(false);
 
 const QIcon importIcon = QIcon(QString::fromUtf8( ":/images/document-import.png"));
 importFromMainTree->setIcon(importIcon);
 importFromMainTree->setToolTip("Importer les fichiers pour extraire les données");
 importFromMainTree->setIconSize(QSize(22, 22));

#ifndef USE_RIGHT_CLICK
 deleteGroupButton->setToolTip(tr("Enlever l'onglet courant"));
 const QIcon iconDelete = QIcon(QString::fromUtf8( ":/images/tab-close-other.png"));
 deleteGroupButton->setIcon(iconDelete);
 deleteGroupButton->setIconSize(QSize(22,22));

 retrieveItemButton->setToolTip(tr("Enlever l'item"));
 retrieveItemButton->setObjectName(QString::fromUtf8("Retrieve"));
 const QIcon iconRetrieve = QIcon(QString::fromUtf8( ":/images/retrieve.png"));
 retrieveItemButton->setIcon(iconRetrieve);
 retrieveItemButton->setIconSize(QSize(22, 22));

 nppDisplayButton->setToolTip(tr("Afficher dans l'éditeur externe"));
 const QIcon iconNpp = QIcon(QString::fromUtf8( ":/images/npp.ico"));
 nppDisplayButton->setIcon(iconNpp);
 nppDisplayButton->setIconSize(QSize(22,22));

 QGridLayout *controlButtonLayout=new QGridLayout;

 controlButtonLayout->addWidget(retrieveItemButton, 2,1,1,1,Qt::AlignCenter);
 controlButtonLayout->setRowMinimumHeight(4, 50);
 controlButtonLayout->addWidget(clearListButton, 4, 1,1,1, Qt::AlignTop);
 controlButtonLayout->addWidget(deleteGroupButton, 6,1,1,1,Qt::AlignCenter);
 controlButtonLayout->addWidget(deleteGroupButton, 8,1,1,1,Qt::AlignCenter);
  
 controlButtonBox->setLayout(controlButtonLayout);
 controlButtonBox->setFlat(true);
 connect(deleteGroupButton, SIGNAL(clicked()),  this, SLOT(deleteGroup()));
 connect(retrieveItemButton, SIGNAL(clicked()), this, SLOT(on_deleteItem_clicked()));
 connect(nppDisplayButton, SIGNAL(clicked()), this, SLOT(on_xhl_display()));

#endif

 QVBoxLayout *tabLayout=new QVBoxLayout;
 tabLayout->addWidget(mainTabWidget);


 if (importType == flags::typeIn)
 {
     currentListWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
 }

 connect(importFromMainTree, SIGNAL(clicked()), this,  SLOT(on_importFromMainTree_clicked()));
 connect(reinterpret_cast<QWidget*>(parent), &QWidget::customContextMenuRequested,
                         [&] {
                                 this->showContextMenu();
                                 altair->updateProject();
                             });


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

void FListFrame::on_xhl_display(const QString& xhl)
{
#if Q_OS_WINDOWS
    QString strnotepad = common::path_access("Outils/npp/notepad++.exe");
#else
    QString strnotepad = "/usr/bin/kate";
#endif    
    launch.start(strnotepad, QStringList(xhl));
}

void FListFrame::on_deleteItem_clicked()
{
  updateIndexInfo();

  if (Hash::wrapper[frameHashKey]->at(currentIndex).isEmpty() && (importType != flags::typeIn) ) return;
  if (row <0) return;

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
      }
      
      rank++;
  }

   if (localrow) currentListWidget->setCurrentRow(localrow-1);
   else if (localrow==0) currentListWidget->setCurrentRow(0);
   row=localrow-1;
   if (localrow <= 0 && (*Hash::wrapper[frameHashKey])[currentIndex].isEmpty()) deleteGroup();
   updateIndexInfo();
   emit(is_ntabs_changed(currentIndex+1)); // emits signal of number of tabs/QListWidgets opened
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

void FListFrame::deleteAllGroups(bool insertFirstGroup, bool eraseAllData)
{
   // mainTabWidget->clear();

    widgetContainer[0]->clear();

    updateIndexInfo();

    /* cleanly wipe out main Hash */

    if (eraseAllData)
    {
          for (const QString& hashKey: Hash::wrapper.keys())
              if (Hash::wrapper[hashKey])
                  Hash::wrapper[hashKey]->clear();
    }
    else
    {
      Hash::wrapper[frameHashKey]->clear();
      Abstract::initH(frameHashKey);
    }


    Hash::Annee.clear();
    Hash::Mois.clear();
    Hash::Siret.clear();
    Hash::Etablissement.clear();
    Hash::Employeur.clear();
    Hash::Budget.clear();
    Hash::Reference.clear();

    clearTabLabels();

  //  widgetContainer.clear();
    if (insertFirstGroup) fileListWidget->addGroup();
}

void FListFrame::deleteGroup(int r)
{
    mainTabWidget->removeTab(r);

    for (const QString& s : Hash::wrapper[frameHashKey]->at(r))
     {
         Hash::Annee.remove(s);
         Hash::Mois.remove(s);
         Hash::Siret.remove(s);
     }

    Hash::wrapper[frameHashKey]->removeAt(r);
    QStringList tabLabels = fileListWidget->getTabLabels();

    if (r < Hash::Reference.size())
            Hash::Reference.removeAt(r);

    tabLabels.removeAt(r);
    fileListWidget->setTabLabels(tabLabels);
    if (r < widgetContainer.size()) widgetContainer.removeAt(r);
    updateIndexInfo();
    emit(is_ntabs_changed(currentIndex+1)); // emits signal of number of tabs/QListWidgets opened
}

void FListFrame::deleteGroup()
{
 updateIndexInfo();
 deleteGroup(currentIndex);
}


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
           altair->outputTextEdit->append(PROCESSING_HTML_TAG "Analyse du fichier n°" + QString::number(rank));
        #endif

        emit(altair->setProgressBar(rank));
    }
}


struct Header* elemPar;

void FListFrame::parseXhlFile(const QString& fileName)
{
   QFile file(fileName);

    bool result = file.open(QIODevice::ReadOnly);
    if (! file.isOpen())
             altair->outputTextEdit->append(ERROR_HTML_TAG "Erreur à  l'ouverture du fichier.");

    if (result == false || file.size()== 0)
    {
        altair->outputTextEdit->append(WARNING_HTML_TAG "Fichier vide.");
        return;
    }

    file.seek(0);
    QByteArray buffer0 = file.read(BUFFER_SIZE);
    const QString string = QString::fromLatin1(buffer0, BUFFER_SIZE);

    QRegExp reg("DocumentPaye.*(?:Annee) V.?=.?\"([0-9]+)\".*(?:Mois) V.?=.?\"([0-9]+)\"(.*)(?:Employeur).*(?:Nom) V.?=.?\"([^\"]+)\".*(?:Siret) V.?=.?\"([0-9A-Z]+)\".*DonneesIndiv(.*)PayeIndivMensuel");
    reg.setPatternSyntax(QRegExp::RegExp2);
    reg.setCaseSensitivity(Qt::CaseInsensitive);
    QRegExp reg2(".*Budget.*Libelle V.?=.?\"([^\"]*)\".*");
    reg2.setCaseSensitivity(Qt::CaseInsensitive);
    QRegExp reg3(".*(?:Etablissement).*(?:Nom) V.?=.?\"([^\"]+)\".*(?:Siret) V.?=.?\"([0-9A-Z]+)\"");
    reg3.setCaseSensitivity(Qt::CaseInsensitive);

    if (string.contains(reg))
    {
        Hash::Annee[fileName] = reg.cap(1);
        Hash::Mois[fileName]  = reg.cap(2);
        QString budgetCapture = reg.cap(3);

        if (budgetCapture.contains(reg2))
           Hash::Budget[fileName] = common::remAccents(std::move(reg2.cap(1).replace("&#39;", "\'").replace("&apos;", "\'").trimmed()));
        else
           Hash::Budget[fileName] = "Non renseigné" ;

        QString etabCapture = reg.cap(6);
        if (etabCapture.contains(reg3))
           Hash::Etablissement[fileName] << common::remAccents(std::move(reg3.cap(1).replace("&#39;", "\'").replace("&apos;", "\'").trimmed()));
        else
           Hash::Etablissement[fileName] << "Non renseigné" ;

        Hash::Employeur[fileName]  = common::remAccents(std::move(reg.cap(4).replace("&#39;", "\'").replace("&apos;", "\'").trimmed()));

        if (etabCapture.contains(reg3))
          {
               Hash::Siret[fileName] << reg3.cap(2);
          }
        else
             Hash::Siret[fileName] << reg.cap(5);
    }
    else
    {
        if (! string.toUpper().contains("DONNEESINDIV")) altair->outputTextEdit->append(WARNING_HTML_TAG "Pas de données individuelles");
        if (! string.toUpper().contains("PAYEINDIVMENSUEL")) altair->outputTextEdit->append(WARNING_HTML_TAG "Pas de payes individuelles");

        altair->outputTextEdit->append(WARNING_HTML_TAG "L'entête DocumentPaye... du fichier " + fileName + " est non conforme à la spécification Xemelios");
        
        //      DocumentPaye.*(?:Annee) V.?=.?\"([0-9]+)\".*(?:Mois) V.?=.?\"([0-9]+)\"(.*)(?:Employeur).*(?:Nom) V.?=.?\"([^\"]+)\".*(?:Siret) V.?=.?\"([0-9A-Z]+)\".*DonneesIndiv(.*)PayeIndivMensuel")
        
        Hash::Budget[fileName] = "";
        Hash::Annee[fileName] = "Inconnu";
        Hash::Mois[fileName]  = "Inconnu";
        Hash::Employeur[fileName]  = "";
        Hash::Etablissement[fileName]  << "";
        Hash::Siret[fileName] << "";

        /* effacer les fichiers mal formés de la liste des fichiers qui vont être envoyés en commandline */

        Hash::Suppression[fileName] = true;

    }


    if (Hash::Budget[fileName].left(5).toUpper() == "MULTI" && Hash::Budget[fileName].right(7).toUpper() == "BUDGETS")
    {
       int pos = -1;
       buffer0.clear();
       file.seek(0);
       buffer0 = file.readAll();
       pos = buffer0.indexOf("<DonneesIndiv>");

       /* On enregistre la position du Siret dans le fichier */

       Hash::SiretPos[fileName] << pos;

       /* On recherche la position suivante */

       pos += 15;
       buffer0 = buffer0.mid(pos);
       qint64 filesize = file.size();

       if (buffer0.size() + pos == filesize)
       {
           while ((pos = buffer0.indexOf("<DonneesIndiv>")) != -1)
           {
              Hash::SiretPos[fileName] << pos;
              const QString string = QString::fromLatin1(buffer0.mid(pos, BUFFER_SIZE));

              QRegExp reg3("(?:Etablissement|Employeur).*(?:Nom) V=\"([^\"]+)\".*(?:Siret) V=\"([0-9A-Z]+)\"");
              reg3.setPatternSyntax(QRegExp::RegExp2);

              if (string.contains(reg3))
               {
                  QString s1 = reg3.cap(1).replace("&#39;", "\'");
                  QString s2 = reg3.cap(2);

                  if (! Hash::Etablissement[fileName].contains(s1))
                      Hash::Etablissement[fileName]  << s1;
                  if (! Hash::Etablissement[fileName].contains(s2))
                      Hash::Siret[fileName] << s2;
               }
               else
               {
                  Hash::Etablissement[fileName]  << "Etablissement/Employer inconnu";
                  Hash::Siret[fileName] << "Siret inconnu";
               }

               buffer0 = buffer0.mid(pos + 15);

               /* on recommence en boucle */
           }

           Hash::SiretPos[fileName] << filesize;
       }
       else
           QMessageBox::warning(nullptr, "Erreur", "Erreur de lecture du fichier " + fileName, QMessageBox::Ok);
    }



   file.close();

   if (file.isOpen())
            altair->outputTextEdit->append(ERROR_HTML_TAG " Erreur à  la fermeture du fichier.");

   if (file.error() != QFileDevice::NoError)
         altair->outputTextEdit->append(WARNING_HTML_TAG " Erreur de fichier.");
   return;
}


void FListFrame::addStringListToListWidget(const QStringList& stringList)
{

    clearTabLabels();
    widgetContainer.clear();

    for (int j = 0; j < getWidgetContainerCount(); j++)
    {
            mainTabWidget->removeTab(j);
            delete(mainTabWidget->widget(j));
    }

    mainTabWidget->clear();
    Hash::wrapper[frameHashKey]->clear();

    int stringListSize = stringList.size();
    emit(altair->showProgressBar());
    emit(altair->setProgressBar(0, stringListSize));

    parseXhlFile(stringList);

    emit(altair->hideProgressBar());

    QStringList allLabels;

    for (const QString& fileName : stringList)
            allLabels  +=   Hash::Annee[fileName];

    int rank = 0;

    allLabels.removeDuplicates();
    allLabels.sort();

    if (! allLabels.isEmpty())
    {
        altair->outputTextEdit->append(STATE_HTML_TAG + QString("Nombre d'années détectées : ") + QString::number(allLabels.size()) + " années, " +     allLabels.join(", "));

       #define listWidget   widgetContainer[rank]

        for (const QString& annee : allLabels)
        {
            QStringList keys = Hash::Annee.keys(annee);
            keys.sort();

            widgetContainer.insert(rank, new QListWidget);
            Hash::wrapper[frameHashKey]->insert(rank, keys);

            addNewTab(rank, annee);

            listWidget->clear();
            listWidget->addItems(keys);

            altair->refreshRowPresentation(rank);
            ++rank;
        }

        mainTabWidget->setCurrentIndex(rank - 1);

        QStringList pairs;
        FStringListIterator j(Hash::Etablissement.values());
        FStringListIterator i(Hash::Siret.values());
        while (i.hasNext() && j.hasNext())
        {
            QStringList etab = i.next();
            QStringList siret = j.next();
            for (int j = 0; j < etab.size() && j < siret.size(); ++j)
            {
                pairs << etab.at(j) + " " + siret.at(j);
            }

        }

        pairs.sort();
        pairs.removeDuplicates();
        pairs.removeAll(" ");

        QStringList tabList ;
        for (int i=0; i < pairs.size(); i++)
               tabList <<  pairs[i].left(60);

        widgetContainer.insert(rank, new QListWidget);
        addNewTab(rank, "Siret");
        Hash::wrapper[frameHashKey]->insert(rank, pairs);

        #ifdef DEBUG
          altair->outputTextEdit->append(STATE_HTML_TAG "Ajout de l'onglet Siret");
        #endif
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
        for (int i=0; i < listWidget->count(); i++)
        {
            listWidget->item(i)->setTextColor(colorList.at(i % colorListSize));
        }
        ++rank;

        pairs.clear();
        tabList.clear();
        pairs = Hash::Budget.values();
        pairs.sort();
        pairs.removeDuplicates();
        pairs.removeAll("");

        for (int i=0; i < pairs.size(); i++)
               tabList <<  pairs[i].left(60);

        widgetContainer.insert(rank, new QListWidget);
        addNewTab(rank, "Budget");
        Hash::wrapper[frameHashKey]->insert(rank, pairs);

        #ifdef DEBUG
          altair->outputTextEdit->append(STATE_HTML_TAG "Ajout de l'onglet Budget");
        #endif
        listWidget->clear();
        listWidget->addItems(tabList);

        for (int i=0; i < listWidget->count(); i++)
         {
            listWidget->item(i)->setTextColor(colorList.at(i % colorListSize));
        }
        ++rank;

        pairs.clear();
        tabList.clear();
        pairs = Hash::Employeur.values();
        pairs.sort();
        pairs.removeDuplicates();
        pairs.removeAll("");

        for (int i=0; i < pairs.size(); i++)
               tabList <<  pairs[i].left(60);

        widgetContainer.insert(rank, new QListWidget);
        addNewTab(rank, "Employeur");
        Hash::wrapper[frameHashKey]->insert(rank, pairs);

        #ifdef DEBUG
          altair->outputTextEdit->append(STATE_HTML_TAG "Ajout de l'onglet Employeur");
        #endif
        listWidget->clear();
        listWidget->addItems(tabList);

        for (int i=0; i < listWidget->count(); i++)
         {
            listWidget->item(i)->setTextColor(colorList.at(i % colorListSize));
        }
        ++rank;

       #undef listWidget
     }

  updateIndexInfo();
  if (row == 0)
    {
        emit(is_ntabs_changed(currentIndex+1)); // emits signal of number of tabs/QListWidgets opened
    }

  fileListWidget->setTabLabels(allLabels << "Siret" << "Budget" << "Employeur");
  if (rank) currentListWidget->setCurrentRow(Hash::wrapper[frameHashKey]->at(rank - 1).size());

}

void FListFrame::addParsedTreeToListWidget(const QStringList& stringList)
{
    if (stringList.isEmpty()) return;
    addStringListToListWidget(parseTreeForFilePaths(stringList));
    Hash::createReference(widgetContainer.size() - 1);
    setStrikeOutFileNames(flags::colors::no);
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
              if (info.isFile())
              {
                  if (info.suffix().toUpper() == "XHL" || info.suffix().toUpper() == "XML")
                    stringsToBeAdded << currentString;
                  else
                    altair->outputTextEdit->append(WARNING_HTML_TAG + QString("Le fichier ")
                                                   + currentString + " sera ignoré. Les fichiers doivent avoir une extension du type .xml ou .xhl.");
              }
              else return {};
    }

    return stringsToBeAdded; 
}

void FListFrame::on_importFromMainTree_clicked()
{
 
 altair->closeProject();
 
 altair->outputTextEdit->append(STATE_HTML_TAG "Bases de paye ajoutées au projet." );

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
#ifdef DEBUG
 altair->outputTextEdit->append(STATE_HTML_TAG "Parcours de l'arbre " );
#endif

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
        #ifdef DEBUG
         altair->outputTextEdit->append(STATE_HTML_TAG "Ajout des chemins à la liste centrale" );
        #endif

         if (stringListSize)
         {
             addParsedTreeToListWidget(stringsToBeAdded);
         }
     }
}

void  FListFrame::setSlotListSize(int s) 
{
    slotListSize=s; 
    mainTabWidget->setEnabled(s > 0);
    if (s== 0) deleteAllGroups();
}

void FListFrame::showContextMenu()
{

        updateIndexInfo();
        if (currentListWidget->count() == 0) return;

        QAction *deleteAction = new QAction(tr("Exclure"), this);
        deleteAction->setIcon(QIcon(":/images/retrieve.png"));

        QAction *addAction = new QAction(tr("Inclure"), this);
        addAction->setIcon(QIcon(":/images/include.png"));

        QAction *displayAction = new QAction(tr("Afficher le code XML"), this);
        displayAction->setIcon(QIcon(":/images/npp.ico"));

        QMenu myMenu;
        const int size = Hash::wrapper["XHL"]->size();

        myMenu.addActions({deleteAction, addAction, displayAction});

#ifdef USE_RIGHT_CLICK
        QAction *deleteGroupAction = new QAction(tr("Enlever l'onglet courant"), this);
        const QIcon iconDelete = QIcon(QString::fromUtf8( ":/images/tab-close-other.png"));
        deleteGroupAction->setIcon(iconDelete);

        if (currentIndex < size - 3)
            myMenu.addActions({deleteGroupAction});
#endif

        QAction* selectedItem = myMenu.exec(QCursor::pos());
        if (selectedItem == nullptr) return;
        if (currentIndex < size - 3)
        {
            if (selectedItem == deleteGroupAction)
            {
                deleteGroup();
                return;
            }
            (*Hash::wrapper["XHL"])[currentIndex] = Hash::Reference[currentIndex];
        }

        if (selectedItem == displayAction)
        {
            QString str = Hash::Reference.at(currentIndex).at(row);
            on_xhl_display(str);

            return;
        }

        int localrow = 0;
        bool isDeleteAction = (selectedItem == deleteAction);

        QFont font;
        for (const QModelIndex &index :  currentListWidget->selectionModel()->selectedRows())
        {
            QString str;
            QListWidgetItem *item = currentListWidget->item(localrow = index.row());

            font = item->font();

            str = Hash::Reference.at(currentIndex).at(localrow);

            Hash::Suppression[str] = isDeleteAction ;

            font.setStrikeOut(isDeleteAction);
            item->setFont(font);
            item->setTextColor(isDeleteAction ? "red" : "navy");
        }

        if (Hash::Reference.size() != size || getRank()+1 != size)
        {
            QMessageBox::critical(nullptr, "Erreur", "Incohérence des tailles des tables de référence : \n\
Référence : " + QString::number(Hash::Reference.size()) +
"\nWrapper size : " + QString::number(size) +
"\nWidget size : " + QString::number(getRank()+1),
                                  QMessageBox::Cancel);

            return;
        }

        setStrikeOutFileNames(flags::colors::yes);

        currentListWidget->setCurrentRow(localrow);
}

void FListFrame::setStrikeOutFileNames(flags::colors color)
{
    const int size = Hash::Reference.size();
    if (size != widgetContainer.size())
    {
        QMessageBox::critical(nullptr, "Erreur", "Incohérence des tailles des conteneurs de référence : " + QString::number(size)
                                                                   + " et de widgets : " + QString::number(widgetContainer.size()) , QMessageBox::Cancel);
        return;
    }

    for (int j = 0; j < size; ++j)
    {
        QStringList strL;
        const QListWidget *listWidget = widgetContainer.at(j);
        int size_j = Hash::Reference.at(j).size();
        if (size_j > listWidget->count())
        {
            QMessageBox::critical(nullptr, "Erreur", "Incohérence des tailles de la table de référence : "
                                                                       + QString::number(size_j) + " et du widget : " + QString::number(listWidget->count()) + " pour l'onglet "
                                                                       + QString::number(j +1) + ".", QMessageBox::Cancel);
            return;
        }

        // On barre dès qu'au moins un Siret du fichier est barré

        for (int k = 0; k < size_j; ++k)
        {
            QListWidgetItem *item = listWidget->item(k);
            QFont font =  item->font();
            const QString str = Hash::Reference.at(j).at(k);

            bool test_for_multi_case = true;
            if (j < size - 3 && Hash::Siret[str].size() > 1 && Hash::Etablissement[str].size() > 1)
              for (int l = 1; l < Hash::Siret[str].size() && l < Hash::Etablissement[str].size(); ++l)
                      test_for_multi_case  = ! Hash::Suppression[Hash::Siret[str].at(l) + " " + Hash::Etablissement[str].at(l)]
                                             &&  test_for_multi_case;

            bool restrictions_on_xhl_files =  true;
            if (j < size - 3)
            {
                 restrictions_on_xhl_files =  ! Hash::Suppression[Hash::Budget[str]]
                                        &&
                                       ! Hash::Suppression[Hash::Employeur[str]];
                 if (! Hash::Siret[str].empty() && ! Hash::Etablissement[str].empty())
                 {
                      restrictions_on_xhl_files =   restrictions_on_xhl_files &&
                         ! Hash::Suppression[Hash::Siret[str].at(0) + " " + Hash::Etablissement[str].at(0)]
                           &&
                         ((Hash::Siret[str].size() == 1 && Hash::Etablissement[str].size() == 1) || test_for_multi_case);
                 }
            }

            if (restrictions_on_xhl_files && ! Hash::Suppression[str])
                    {
                        strL << str;
                        font.setStrikeOut(false);
                        font.setItalic(false);
                        item->setFont(font);
                        if (color == flags::colors::yes)
                            item->setTextColor("green");
                    }
                    else
                    {
                        if (test_for_multi_case == false)
                        {
                                font.setItalic(true);
                        }

                        font.setStrikeOut(true);
                        item->setFont(font);
                        if (color == flags::colors::yes)
                           item->setTextColor("red");
                    }
        }

        (*Hash::wrapper["XHL"])[j] = strL;
    }
}
