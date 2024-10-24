
// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle (CPI).

// L'auteur se réserve le droit d'exploitation du présent logiciel, 
// et notamment de reproduire et de modifier le logiciel, conformément aux 
// dispositions de l'article L.122-6 du même code. L'auteur se réserve le droit
// de modifier et de corriger les erreurs du logiciel, conformément aux termes 
// du deuxième alinéa du I de l'article L.122-6-1 du même code.

// Le présent logiciel doit être regardé comme un "Module Externe", pour 
// l'application des stipulations de la licence CeCILL, lorsque le code source, 
// ou une bibliothèque compilée à partir du présent code, sont utilisés pour 
// compiler tout logiciel faisant usage du présent code.

// Tout fichier contenant le présent code, ou des versions antérieures du 
// présent code, et faisant apparaître des termes contraires aux stipulations
// précédentes doit être regardé comme faisant partie d'actes préparatoires à
// l'élaboration du présent logiciel et non comme le logiciel lui-même, ou 
// comme des erreurs matérielles dans la copie des licences. Seules les 
// stipulations précédentes doivent être regardées comme s'appliquant au 
// présent logiciel.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de ces stipulations et que vous en avez accepté les
// termes.

// Sans préjudice des dispositions du CPI, une autorisation d'usage et de
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières pour l'exercice de leurs fonctions publiques.
// Le code ainsi mis à disposition ne peut être transmis à d'autres utilisateurs.
//

#include "flistframe.h"
#include "custom.h"

FListFrame::FListFrame(QAbstractItemView* tree,
                       short import_type,
                       const QString &hashKey,
                       const QStringList &description,
                       const QString &command_line,
                       int cli_type,
                       const QStringList &separator,
                       const QStringList &xml_tags,
                       const QStringList &tabLabels,
                       tools::TabWidgetTrait mainTabWidgetRank,
                       QStringList* terms,
                       QStringList* translation)
{
 setAcceptDrops(true);

 currentIndex = 0;  // necessary for project parsing

 importType = import_type;
 tags = xml_tags;
 fileTreeView = tree;
 this->tabLabels = tabLabels;
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


 currentListWidget = fileListWidget->currentListWidget;

 initializeWidgetContainer(currentListWidget);

 Abstract::initH(frameHashKey);

 if (mainTabWidgetRank == tools::TabWidgetTrait::NO_EMBEDDING_TAB_WIDGET)
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

 connect(this, SIGNAL(imported()), this, SLOT(finalise()));
}

void FListFrame::addParsedTreeToListWidget(const QStringList &strL)
{
    if (strL.isEmpty()) return;
    stringList = parseTreeForFilePaths(strL);
    addStringListToListWidget();
}


void FListFrame::updateIndexInfo()
{
  currentListWidget = qobject_cast<QListWidget*>(mainTabWidget->currentWidget());
  if (currentListWidget == nullptr) return;
  row = currentListWidget->currentRow();
  currentIndex = mainTabWidget->currentIndex();
}

void FListFrame::deleteAllGroups(bool insertFirstGroup, bool eraseAllData)
{

    if (! widgetContainer.empty()) widgetContainer[0]->clear();

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

    if (insertFirstGroup) fileListWidget->addGroup();
}

void FListFrame::launch_thread(int rank, const QString& s)
{
    Worker* w = new Worker(rank, s);

    W.push_back(w);

    QThreadPool::globalInstance()->start(w);

    connect(w, &Worker::resultReady, this, &FListFrame::handleResults);
    connect(w, &Worker::textAppend, this, &FListFrame::textAppend);
    // Cette fonction pourrait être optimisée en ne lançant pas les fils d'exécution de manière successive mais par par groupe avec plusieurs fils parallèles dans chaque groupe
}

QMutex mutex, mutex2;

void Worker::parseXhlFile(const QString& fileName)
{

    QFile file(fileName);
    long long ligne = 0;
    bool result = file.open(QIODevice::ReadOnly);
    if (! file.isOpen())
    {
        mutex.lock();
        emit(textAppend(ERROR_HTML_TAG "Erreur à  l'ouverture du fichier."));
        mutex.unlock();
    }

    if (result == false || file.size()== 0)
    {
        mutex.lock();
        emit(textAppend(WARNING_HTML_TAG "Fichier vide."));
        mutex.unlock();
        return;
    }

    file.seek(0);
    QByteArray buffer0 = file.readAll();
    QString str = QString::fromLatin1(buffer0, BUFFER_SIZE);

#   define QUOTE "(?:\"|')"

    str.remove(QRegularExpression("[\\(\\)]"));

    QRegularExpression reg("DocumentPaye.*(?:Annee)\\s*V.?=.?" QUOTE "([0-9]+)"
                           QUOTE ".*(?:Mois)\\s*V.?=.?" QUOTE "([0-9]+)"
                           QUOTE "(.*)(?:Employeur).*(?:Nom)\\s*V.?=.?"
                           QUOTE "([^" QUOTE "]+)"
                           QUOTE ".*(?:Siret)\\s*V.?=.?"
                           QUOTE "([0-9A-Z]+)"
                           QUOTE ".*DonneesIndiv(.*)PayeIndivMensuel");

    QFlags<QRegularExpression::PatternOption> pattern = QRegularExpression::CaseInsensitiveOption| QRegularExpression::DotMatchesEverythingOption| QRegularExpression::UseUnicodePropertiesOption;
    reg.setPatternOptions(pattern);// | QRegularExpression::UseUnicodePropertiesOption| QRegularExpression::ExtendedPatternSyntaxOption);
    QRegularExpression reg2(".*Budget.*Libelle\\s*V.?=.?" QUOTE "([^" QUOTE "]*)" QUOTE ".*");
    reg2.setPatternOptions(pattern);
    QRegularExpression reg3(".*(?:Etablissement).*(?:Nom)\\s*V.?=.?" QUOTE "([^" QUOTE "]+)" QUOTE ".*(?:Siret)\\s*V.?=.?" QUOTE "([0-9A-Z]+)" QUOTE);
    reg3.setPatternOptions(pattern);

    QRegularExpressionMatch match = reg.match(str);
    QByteArray::const_iterator it;

    if (match.hasMatch())
    {
        QString budgetCapture = match.captured(3);
        QString etabCapture = match.captured(6);

        mutex.lock();

        Hash::Annee[fileName] = match.captured(1);
        Hash::Mois[fileName]  = match.captured(2);

        QRegularExpressionMatch match2 = reg2.match(budgetCapture);
        if (match2.hasMatch())
           Hash::Budget[fileName] = tools::remAccents(match2.captured(1).replace("&#39;", "\'").replace("&apos;", "\'").trimmed());
        else
           Hash::Budget[fileName] = "" ;

        QRegularExpressionMatch match3 = reg3.match(etabCapture);
        if (match3.hasMatch())
           Hash::Etablissement[fileName] << tools::remAccents(match3.captured(1).replace("&#39;", "\'").replace("&apos;", "\'").trimmed());
        else
           Hash::Etablissement[fileName] << "" ;

        Hash::Employeur[fileName]  = tools::remAccents(match.captured(4).replace("&#39;", "\'").replace("&apos;", "\'").trimmed());

        if (match3.hasMatch())
          {
               Hash::Siret[fileName] << match3.captured(2);
          }
        else
             Hash::Siret[fileName] << match.captured(5);
        mutex.unlock();
    }
    else
    {
        mutex.lock();

        if (! str.toUpper().contains("DONNEESINDIV")) emit(textAppend(WARNING_HTML_TAG "Pas de données individuelles"));
        if (! str.toUpper().contains("PAYEINDIVMENSUEL")) emit(textAppend(WARNING_HTML_TAG "Pas de payes individuelles"));

        emit(textAppend(WARNING_HTML_TAG "L'entête DocumentPaye... du fichier "
                        + fileName +
                       " est non conforme à l'annexe de la convention cadre de dématérialisation."));

        //      DocumentPaye.*(?:Annee) V.?=.?" QUOTE "([0-9]+)" QUOTE ".*(?:Mois) V.?=.?" QUOTE "([0-9]+)" QUOTE "(.*)(?:Employeur).*(?:Nom) V.?=.?" QUOTE "([^" QUOTE "]+)" QUOTE ".*(?:Siret) V.?=.?" QUOTE "([0-9A-Z]+)" QUOTE ".*DonneesIndiv(.*)PayeIndivMensuel")


        Hash::Budget[fileName] = "";
        Hash::Annee[fileName] = "Inconnu";
        Hash::Mois[fileName]  = "Inconnu";
        Hash::Employeur[fileName]  = "";
        Hash::Etablissement[fileName]  << "";
        Hash::Siret[fileName] << "";

        /* effacer les fichiers mal formés de la liste des fichiers qui vont être envoyés en commandline */

        Hash::Suppression[fileName] = true;
        mutex.unlock();

        goto out;
    }

    it = buffer0.cbegin() + str.indexOf("DonneesIndiv") + 15;

    while(it != buffer0.cend())
      {

        if (*it == '\n')
        {
            ++ligne;
            ++it;
            continue;
        }
        if (*++it != '<') continue;
        if (*++it != 'D') continue;
        if (*++it != 'o') continue;
        if (*++it != 'n') continue;
        if (*++it != 'n') continue;
        if (*++it != 'e') continue;
        if (*++it != 'e') continue;
        if (*++it != 's') continue;
        if (*++it != 'I') continue;

        it += 6;

        for (int u = 0; u < BUFFER_SIZE; ++u) str[u] = *++it;

        QRegularExpression reg4("(?:Etablissement|Employeur).*(?:Nom) V=" QUOTE "([^" QUOTE "]+)"
                                QUOTE ".*(?:Siret) V=" QUOTE "([0-9A-Z]+)" QUOTE "");
        reg4.setPatternOptions(pattern);

        QRegularExpressionMatch match4 = reg4.match(str);
        if (match4.hasMatch())
         {
            QString s1 = match4.captured(1).replace("&#39;", "\'");
            QString s2 = match4.captured(2);
            mutex2.lock();

            if (! Hash::Etablissement[fileName].contains(s1))
                Hash::Etablissement[fileName]  << s1;
            if (! Hash::Etablissement[fileName].contains(s2))
                Hash::Siret[fileName] << s2;
            mutex2.unlock();
         }
      }


out :

   file.close();

   if (file.isOpen())
   {
       mutex.lock();
       emit(textAppend(ERROR_HTML_TAG " Erreur à  la fermeture du fichier."));
       mutex.unlock();
   }

   if (file.error() != QFileDevice::NoError)
   {
       mutex.lock();
       emit(textAppend(WARNING_HTML_TAG " Erreur de fichier."));
       mutex.unlock();
   }

   return;
}

void FListFrame::parseXhlFile()
{
    int rank = 0;

    size = stringList.size();

    // L'usage de threads est indiqué pour éviter un freeze de l'interface graphique

    const QString cdROM = tools::cdRomMounted();

    if (isTerminated
            || cdROM.isEmpty()
            || ! QDir(cdROM).exists()
            ||  QDir(cdROM).QDir::entryInfoList(QDir::Dirs
                                                 | QDir::Files
                                                 | QDir::NoDotAndDotDot).isEmpty())
     {

        while (rank < size)
        {
             launch_thread(rank , stringList.at(rank));
             ++rank;
        }

        sequentiel = false;

        importFromMainTree->show();

        return;
     }

    int res = QMessageBox::warning(nullptr, "Vous avez un disque optique",
                                   "Cliquer Non et enlevez-le du lecteur<br>"
                                   "si vous ne souhaitez pas importer les données du disque."
                                   "<br>Sinon confirmer par Oui pour poursuivre l'extraction.",
                                   QMessageBox::No, QMessageBox::Yes);
    if (res == QMessageBox::No) return;

    isTerminated = false;
    importFromMainTree->hide();
    sequentiel = true;
    if (size == 0) return;

#ifdef HAVE_APPLICATION
    emit(showProgressBar());
#endif

    launch_thread(0, stringList.at(0));

    while (++rank < size)
        connect(W.back(), &Worker::resultReady, [this, rank] { if (! isTerminated) launch_thread(rank, stringList.at(rank)); });

        #ifdef DEBUG_INPUT_FILES
           app->outputTextEdit->append(PROCESSING_HTML_TAG "Analyse du fichier n°" + QString::number(rank));
        #endif

    // Le thread contrôleur retarde l'importation des fichiers dans l'onglet central
    // à la finalisation de la lecture du disue optique et gère aussi la barre
    // de progression.


     QTimer *timer = new QTimer(this);

     connect(timer, &QTimer::timeout, [&, timer] {
                     int test = 0;
                     for (Worker *t : W)
                     {
                         if (t)
                             test += static_cast<int>(t->isFinished());
                     }

                     #ifdef HAVE_APPLICATION
                        emit(setProgressBar(test));
                     #endif

                     if (test == size)
                     {
                         emit(imported());
                         timer->stop();
                     }
         });

     connect(this, SIGNAL(terminated()), timer, SLOT(stop()));
     connect(this, &FListFrame::terminated, [this] {
         isTerminated = true;
         sequentiel = false;
         importFromMainTree->show();
     });

       timer->start(PROGRESSBAR_TIMEOUT);

}


void FListFrame::addStringListToListWidget()
{

    clearTabLabels();
    widgetContainer.clear();

    mainTabWidget->clear();
    Hash::wrapper[frameHashKey]->clear();

#   ifdef HAVE_APPLICATION
      int stringListSize = stringList.size();
      emit(showProgressBar());
      emit(setProgressBar(0, stringListSize));
#   endif

    parseXhlFile();
}
constexpr const char* _7z = "7Z";
constexpr const char* bzip2 = "BZ2";
constexpr const char* tar = "TAR";
constexpr const char* gzip = "GZ";
constexpr const char* zip = "ZIP";
constexpr const char* formats[5] = {_7z, bzip2, tar, gzip, zip};

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
                if (info.baseName() == "PaxHeader") continue;  // "Déchets" de l'extraction 7zip
                QDir dir(currentString);
                QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot|QDir::Files|QDir::Dirs);
                // Recursion
                  // Utilisation d'une rvalue
                QStringList &&tempStrings = QStringList();
                for (QFileInfo & embeddedFileInfo: entries)
                {
                     tempStrings <<  embeddedFileInfo.absoluteFilePath();
                }

                  // Move semantics : gain de temps et de mémoire (>= Qt5.4)
                  // tempStrings est coerced dans le type const QStringList sans copie de données

                stringsToBeAdded << parseTreeForFilePaths(tempStrings);

              }
            else
              if (info.isFile())
              {
                 const QString &tempDir = info.absolutePath() + QDir::separator() + info.baseName();
                  if (QFileInfo(tempDir).isDir())
                  {
                     QDir(tempDir).removeRecursively();
                  }

                  int res = 0;


                  if (info.suffix().toUpper() == "XHL" || info.suffix().toUpper() == "XML")
                  {
                         stringsToBeAdded << currentString;
                         res = 1;
                  }
                  else
#ifdef Q_OS_WIN

                  {
                      for (short i = 0; i < 5; ++i)
                      {
                        if (info.suffix().toUpper() == formats[i])
                          {
                              res = 1;
                              emit(textAppend(PROCESSING_HTML_TAG + QString("Décompression du fichier " + currentString + ". Patientez...")));

                              QStringList cl = QStringList() <<  "-ext2folder" << QDir::toNativeSeparators(currentString) ;

                              QString app = "peazip" + QString(tools::systemSuffix);
                              process.setWorkingDirectory(QApplication::applicationDirPath());
                              process.start(app, cl);

                              emit(textAppend(STATE_HTML_TAG + app + " " + cl.join(" ")));
                              if (process.waitForStarted())
                              {
                                 emit(textAppend(PROCESSING_HTML_TAG + QString("Le fichier ")
                                               + currentString + " est en cours de décompression..."));

                              }
                              else
                                 emit(textAppend(WARNING_HTML_TAG + QString("Le fichier ")
                                                + currentString + " n'a pas été décompressé ou des erreurs sont rencontrées.<br>" WARNING_HTML_TAG + "Ligne de commande " +cl.join(" ")));

                              process.waitForFinished();
                              stringsToBeAdded << parseTreeForFilePaths({tempDir});

                          }
                     }
                 }
#else

               {

                  if (info.suffix().toUpper() == "ZIP")
                    {
                        emit(textAppend(PROCESSING_HTML_TAG + QString("Décompression du fichier " + currentString + ". Patientez...")));
                        int res = system(QString("unzip -C '" + currentString + "' '*.x[hm]l' -d '" + tempDir + "'").toStdString().c_str());

                        if (res == 0)
                           emit(textAppend(STATE_HTML_TAG + QString("Le fichier ")
                                         + currentString + " a été décompressé."));
                        else
                           emit(textAppend(WARNING_HTML_TAG + QString("Le fichier ")
                                          + currentString + " n'a pas été décompressé."));

                        stringsToBeAdded << parseTreeForFilePaths({tempDir});
                        emit(textAppend(tempDir));
                        res = 1;

                    }
                    else
                    {

                        for (short i = 0; i < 4; ++i)
                        {
                          if (info.suffix().toUpper() == formats[i])
                            {
                                emit(textAppend(PROCESSING_HTML_TAG + QString("Décompression du fichier " + currentString + ". Patientez...")));
                                constexpr const char* types[4] = {"7z", "bzip2", "tar", "gzip"};
                                const QString &cl = QString("7z x '" + currentString + "' -o'" + tempDir + "' -t" + QString(types[i]));
                                res = system(cl.toStdString().c_str());

                                if (res < 2)
                                {
                                   emit(textAppend(STATE_HTML_TAG + QString("Le fichier ")
                                                 + currentString + " a été décompressé."));
                                   emit(textAppend(tempDir));
                                   stringsToBeAdded << parseTreeForFilePaths({tempDir});

                                }
                                else

                                   emit(textAppend(WARNING_HTML_TAG + QString("Le fichier ")
                                                  + currentString + " n'a pas été décompressé ou des erreurs sont rencontrées.<br>" WARNING_HTML_TAG + "Ligne de commande " +cl));
                               res = 1;
                            }

                      }
                   }
              }
#endif

#  ifdef HAVE_APPLICATION
                 if (! res)
                 {
                          emit(textAppend(WARNING_HTML_TAG + QString("Le fichier ")
                                       + currentString + " sera ignoré. Les fichiers doivent avoir une extension du type .xml, .xhl, .7z, .zip, .tar.gz ou .tar.bz2"));
                 }
#   endif


           }
    }
 
    return stringsToBeAdded;
}


void FListFrame::showContextMenu()
{
        if (Hash::wrapper.isEmpty()) return;
        if (Hash::wrapper["XHL"] == nullptr || Hash::wrapper["XHL"]->isEmpty()) return;

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
            if (currentIndex < Hash::Reference.size())
                    (*Hash::wrapper["XHL"])[currentIndex] = Hash::Reference.at(currentIndex);
        }

        if (selectedItem == displayAction)
        {
           if (currentIndex < Hash::Reference.size()
                && row < Hash::Reference.at(currentIndex).size())
           {
                QString str = Hash::Reference.at(currentIndex).at(row);
                on_file_display(str);
           }

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

            if (currentIndex < Hash::Reference.size()
                    && localrow < Hash::Reference.at(currentIndex).size())
                    str = Hash::Reference.at(currentIndex).at(localrow);

            Hash::Suppression[str] = isDeleteAction ;

            font.setStrikeOut(isDeleteAction);

            item->setFont(font);
            item->setForeground(QBrush(isDeleteAction ? "red" : "navy"));
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
        emit(updateProject(update::saveProject | update::noWarnRExport));
}


void FListFrame::setStrikeOutFileNames(flags::colors color)
{
    if (Hash::wrapper.isEmpty()) return;
    if (Hash::wrapper["XHL"] == nullptr || Hash::wrapper["XHL"]->isEmpty()) return;

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
                                                                       + QString::number(size_j) + " et du composant : " + QString::number(listWidget->count()) + " pour l'onglet "
                                                                       + QString::number(j +1) + ".", QMessageBox::Cancel);
            return;
        }

        for (int k = 0; k < size_j; ++k)
        {
            QListWidgetItem *item = listWidget->item(k);
            QFont font =  item->font();
            const QString str = Hash::Reference.at(j).at(k);

            bool suppression_partielle = false;

            if (Hash::Suppression[Hash::Budget[str]]  || Hash::Suppression[Hash::Employeur[str]])
                Hash::Suppression[str] = true;

            // on regarde si une partie du fichier est supprimée (siret/établissement)

            if (j < size - 3)
            {
              for (int l = 0; l < Hash::Siret[str].size() && l < Hash::Etablissement[str].size(); ++l)
              {
                      suppression_partielle  =  Hash::Suppression[Hash::Siret[str].at(l) + " " + Hash::Etablissement[str].at(l)]
                                             ||  suppression_partielle;

              }

                 suppression_partielle =  ! Hash::Suppression[Hash::Budget[str]]  && ! Hash::Suppression[Hash::Employeur[str]] && suppression_partielle;

                 if (suppression_partielle)
                     if (! Hash::Suppression[Hash::Budget[str]] && ! Hash::Suppression[Hash::Employeur[str]])
                         Hash::Suppression[str] = false;
            }

            if (Hash::Suppression[str])
                    {
                       font.setStrikeOut(true);
                       font.setItalic(false);
                       item->setFont(font);

                       if (color == flags::colors::yes)
                           item->setForeground(QBrush("red"));
                    }
                    else
                    {
                        if (suppression_partielle)
                        {
                            font.setItalic(true);
                        }
                        else
                            font.setItalic(false);

                        if (! strL.contains(str))
                            strL << str;
                        font.setStrikeOut(false);
                        item->setFont(font);
                        if (color == flags::colors::yes)
                            item->setForeground(QBrush("green"));
                    }
        }

        (*Hash::wrapper["XHL"])[j] = strL;
    }
}

// public slots

void FListFrame::deleteGroup()
{
 updateIndexInfo();
 deleteGroup(currentIndex);
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

//protected slots

void FListFrame::on_importFromMainTree_clicked()
{

 #ifdef HAVE_APPLICATION
    emit(fileListWidget->forceCloseProject());
    emit(appRepaint());
 #endif

 QItemSelectionModel *selectionModel = fileTreeView->selectionModel();
 QModelIndexList  indexList=selectionModel->selectedIndexes();

 if (indexList.isEmpty()) return;

 QStringList&& stringsToBeAdded = QStringList();

#ifdef DEBUG
 app->outputTextEdit->append(STATE_HTML_TAG "Parcours de l'arbre " );
#endif

 if (importType == flags::importFiles)
    {
        int stringListSize=0;
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
         app->outputTextEdit->append(STATE_HTML_TAG "Ajout des chemins à la liste centrale" );
        #endif

         if (stringListSize)
         {
             addParsedTreeToListWidget(stringsToBeAdded);
         }
     }
 else
     if (importType == flags::importNames)
     {
       for (const QModelIndex& index : indexList)
       {    
           stringsToBeAdded << index.data().toString();
       }
     }
}


void FListFrame::on_file_display(const QString& file)
{
#ifdef Q_OS_WIN
    QString strnotepad = tools::path_access("npp/notepad++.exe");
#else
    QString strnotepad = "/usr/bin/kate";
#endif
     QProcess* launch = new QProcess;
     launch->start(strnotepad, QStringList() << file);

}


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

static constexpr const int colorListSize = 9;
static constexpr const  std::array<const char*, colorListSize > &colorList = { "tomato", "navy", "yellowgreen", "marroon", "orange", "green",  "darkcyan", "blue", "black"};

inline void finalise_macro(FListFrame* listFrame, QStringList& pairs, const QString& label, const int rank)
{

    pairs.removeDuplicates();
    pairs.removeAll("");

    QStringList tabList;
    for (int i=0; i < pairs.size(); i++)
           tabList <<  pairs[i].left(60);

    QListWidget* composant = new QListWidget;
    composant->setSelectionMode(QAbstractItemView::ExtendedSelection);
    composant->setSelectionBehavior(QAbstractItemView::SelectRows);
    listFrame->widgetContainer.insert(rank, composant);

     listFrame->addNewTab(rank, label);
     Hash::wrapper[listFrame->frameHashKey]->insert(rank, pairs);

    listFrame->widgetContainer[rank]->clear();
    listFrame->widgetContainer[rank]->addItems(tabList);

    for (int i=0; i < listFrame->widgetContainer[rank]->count(); i++)
     {
        listFrame->widgetContainer[rank]->item(i)->setForeground(QBrush(colorList.at(i % colorListSize)));
    }

}

template<typename T>
inline void finalise_macro(FListFrame* listFrame, const T &hash, const QString& label, const int rank)
{
    QStringList pairs;
    pairs = hash.values();
    pairs.sort();

    finalise_macro(listFrame, pairs, label, rank);
}


void FListFrame::finalise()
{
#ifdef HAVE_APPLICATION
    emit(hideProgressBar());
#endif

    QStringList allLabels;

    for (const QString& fileName : stringList)
            allLabels  +=   Hash::Annee[fileName];

    int rank = 0;

    allLabels.removeDuplicates();
    allLabels.sort();

    if (! allLabels.isEmpty())
    {

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
# ifdef HAVE_APPLICATION
            emit(refreshRowPresentation(rank));
# endif
            ++rank;
        }

        mainTabWidget->setCurrentIndex(rank - 1);

        QStringList pairs;
        FStringListIterator j(Hash::Etablissement.values().toVector());
        FStringListIterator i(Hash::Siret.values().toVector());

        while (i.hasNext() && j.hasNext())
        {
            QStringList etab = i.next();
            QStringList siret = j.next();
            for (int j = 0; j < etab.size() && j < siret.size(); ++j)
            {
                pairs << etab.at(j) + " " + siret.at(j);
            }

        }

# ifdef HAVE_APPLICATION

        FINALISE_MACRO

# endif

       #undef listWidget
     }

  updateIndexInfo();

  if (row == 0)
    {
        emit(is_ntabs_changed(currentIndex+1)); // emits signal of number of tabs/QListWidgets opened
    }

  fileListWidget->setTabLabels(allLabels << tabLabels);
  if (rank) currentListWidget->setCurrentRow(Hash::wrapper[frameHashKey]->at(rank - 1).size());

#ifdef HAVE_APPLICATION
  emit(textAppend(STATE_HTML_TAG "Fichiers ajoutés au projet." ));
  emit(updateProject(update::saveProject | update::noWarnRExport));
#endif

  Hash::createReference(widgetContainer.size() - 1);
  setStrikeOutFileNames(flags::colors::no);

}


