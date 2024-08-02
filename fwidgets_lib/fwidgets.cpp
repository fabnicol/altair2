
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
#include "fwidgets.h"
#include "tools.h"
#include "templates.h"
#include "flistframe.h"

/* using above function with controlled object encapsulation */

QStringList  applyHashToStringList(QHash<QString, QString> &H,  QStringList &M)
{
    QStringList L;
    QStringListIterator j(M);
    while (j.hasNext())
        L << H[j.next()];
   
    return L;
}


void FAbstractConnection::meta_connect(FAbstractWidget* w,  const Q2VectorWidget *enabledObjects,  const Q2VectorWidget *disabledObjects)
{
    if ((enabledObjects != nullptr) &&  (! enabledObjects->isEmpty()) )
    {

        QVectorIterator<QWidget*> componentlistIterator(w->getComponentList());
        Q2VectorIterator objectlistIterator(*enabledObjects);
        while ((componentlistIterator.hasNext()) && (objectlistIterator.hasNext()))
        {
            QWidget* component=componentlistIterator.next();
            QVectorIterator<QWidget*> i(objectlistIterator.next());
            while (i.hasNext())
            {
                QWidget* item=i.next();
                if ((item == nullptr) || (component==nullptr)) continue;
                // This does not always work automatically through Qt parenting as it normally should, so it is necessary to reimplement enabling dependencies
                // e.g. for QLabels

                if (! component->isEnabled())
                    item->setEnabled(false);
                else
                    if (component->metaObject()->className() == QString("FCheckBox"))
                        item->setEnabled(static_cast<FCheckBox*>(component)->isChecked());

                 connect(component, SIGNAL(toggled(bool)), item, SLOT(setEnabled(bool)));

                // this connection must follow the general enabling adjustments above
                // A way to avoid this default behaviour is to group the enabled boxes into a  flat (invisible) GroupBox

                if (item->metaObject()->className() == QString("FCheckBox"))
                    connect(component, SIGNAL(toggled(bool)), item, SLOT(uncheckDisabledBox()));
                else
                    if (item->metaObject()->className() == QString("FRadioBox"))
                        connect(component, SIGNAL(toggled(bool)), item, SLOT(resetRadioBox(bool)));


            }
        }
    }

    if ((disabledObjects != nullptr) &&  (!disabledObjects->isEmpty()))
    {

        QVectorIterator<QWidget*> newcomponentlistIterator(w->getComponentList());
        Q2VectorIterator newobjectlistIterator(*disabledObjects);
        while ((newcomponentlistIterator.hasNext()) && (newobjectlistIterator.hasNext()))
        {

            QWidget* component=newcomponentlistIterator.next();
            QVectorIterator<QWidget*> j(newobjectlistIterator.next());
            while (j.hasNext())
            {
                QWidget* item=j.next();

                if ((item == nullptr) || (component==nullptr)) continue;

                if (! component->isEnabled())
                    item->setEnabled(false);
                else
                    if (component->metaObject()->className() == QString("FCheckBox"))
                        item->setEnabled(! static_cast<FCheckBox*>(component)->isChecked());

                connect(component, SIGNAL(toggled(bool)), item , SLOT(setDisabled(bool)));

                // this connexion must follow the general enabling adjustments above
                if (item->metaObject()->className() == QString("FCheckBox"))
                    connect(component, SIGNAL(toggled(bool)), item, SLOT(uncheckDisabledBox()));
            }
        }
    }
}

inline void FAbstractWidget::FCore(const QVector<QWidget*>& w,
                                   FString defaultCommandLine, 
                                   int commandLineType, 
                                   const QString &hashKey, 
                                   const QStringList & description,
                                   const QString &option,
                                   const QVector<QWidget*>enabledObjects,
                                   const QVector<QWidget*>disabledObjects)
{
    Q2VectorWidget *dObjects = new Q2VectorWidget,
                   *eObjects = new Q2VectorWidget;

    if (enabledObjects.isEmpty())
    {
        delete(eObjects);
        eObjects = nullptr;
    }
    else
        *eObjects << enabledObjects;

    if (disabledObjects.isEmpty())
    {
        delete(dObjects);
        dObjects = nullptr;
    }
    else
        *dObjects << disabledObjects;

    FCore(w, FString(defaultCommandLine), commandLineType, hashKey, description, option, eObjects, dObjects);
}

inline void FAbstractWidget::FCore(const QVector<QWidget*>& w, FString defaultCommandLine, int commandLineType, const QString &hashKey, const QStringList & description,
                  const QString &option,  Q2VectorWidget *enabledObjects, Q2VectorWidget *disabledObjects)
{
    this->enabledObjects = enabledObjects;
    this->disabledObjects = disabledObjects;

    w.at(0)->setToolTip(description.at(1));
    w.at(0)->setEnabled((commandLineType & flags::status::enabledMask) ==  flags::status::enabled);

    this->status = static_cast<flags::status>(commandLineType & static_cast<int>(flags::status::statusMask));

    this->commandLineList = QList<FString>() << defaultCommandLine;
    this->componentList= w;
    this->hashKey = hashKey;
    this->commandLineType = static_cast<flags::commandLineType>(commandLineType & static_cast<int>(flags::commandLineType::commandLineMask));
    this->optionLabel = option;

    if (!hashKey.isEmpty())
            {
                if (!description.isEmpty())\
                {
                     this->description=description;
                    Hash::description[hashKey]=description;
                }
            }

    Hash::wrapper[hashKey] = new FStringList;
    *Hash::wrapper[hashKey]  << (QStringList() << QString());

    if (static_cast<flags::status>(commandLineType & static_cast<int>(flags::status::excludeMask)) 
            != flags::status::excluded)  Abstract::abstractWidgetList.append(this);

    FAbstractConnection::meta_connect(this, this->enabledObjects, this->disabledObjects);

    if ((commandLineType & flags::status::widgetMask) ==  flags::status::checked)
       static_cast<FCheckBox*>(w.at(0))->toggle();
}


const QStringList FAbstractWidget::commandLineStringList()
{
    /* If command line option is ill-formed, or if a corresponding checkbox is unchecked (or negatively checked)
  * or if an argument-taking option has no-argument, return empty */
//Warning(nullptr, "", optionLabel+":"+QString::number(commandLineType & flags::::widgetMask));

    if (
               optionLabel.isEmpty()
//            || commandLineList.isEmpty()
            || (! commandLineList.isEmpty() && commandLineList[0].isFalse())
//           // ||  (commandLineList[0].toQString().isEmpty())
            ||  (this->isAbstractDisabled())) return {};

    QStringList strL=QStringList();

      if ((this->status & flags::status::widgetMask) == flags::status::hasListCommandLine)  // hashkey == XHL
        {
            QListIterator<FString> i(commandLineList);
            while (i.hasNext())
                if (!i.peekNext().isEmpty())
                    strL <<  i.next().trimmed();
                else
                    i.next();

        }
        else
        {
          if (commandLineList[0].isTrue())
            {
                if  (optionLabel.size() == 1)
                   strL = QStringList("-"+optionLabel);
                else
                {
                    if (optionLabel.at(0) == '^')
                      strL = QStringList(optionLabel.mid(1).trimmed());
                    else
                      strL = QStringList ("--" +optionLabel);
                }
            }
         else
            {
                if (! commandLineList[0].toQString().isEmpty())
                {
                    if (optionLabel.at(0) == '^')
                    {
                        if (commandLineList[0] != "  ")
                           strL = QStringList(optionLabel.mid(1).trimmed() + "=" + commandLineList[0].toQString());
                    }
                    else
                    if (optionLabel.at(0) == '%')
                    {
                        if (commandLineList[0] != "  ")
                            strL = (optionLabel.size() == 1)? (QStringList() << "-" + optionLabel.mid(1) << commandLineList[0].toQString())
                                                         : (QStringList() << "--" + optionLabel.mid(1) << commandLineList[0].toQString());
                    }
                    else
                    {
                       if (commandLineList[0] != "  ")
                       {
                           strL = (optionLabel.size() == 1)? (QStringList() << "-"+optionLabel << commandLineList[0].toQString())
                                                        :(QStringList() << "--" + optionLabel+"=" << commandLineList[0].toQString());
                       }
                    }
                }
            }
         }
      
       return QStringList(strL);
}

QVector<FAbstractWidget*> Abstract::abstractWidgetList = QVector<FAbstractWidget*>();

FListWidget::FListWidget(QWidget* par,
                         const QString& hashKey,
                         int commandLineType,
                         const QStringList& description,
                         const QString& commandLine,
                         const QStringList& sep,
                         const QStringList &taglist,
                         const QList<QString> *terms,
                         const QList<QString>*translation,
                         QWidget* controlledWidget)
{
    setAcceptDrops(true);

    widgetDepth="2";
    parent = static_cast<FListFrame*>(par);
    Abstract::initH(hashKey);

    setObjectName(hashKey+" "+description.join(" "));

    currentListWidget=new QListWidget;
    
    currentListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    currentListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    currentListWidget->setToolTip(description.at(1));
    
    componentList=QVector<QWidget*>() << currentListWidget;

    FCore({this}, FString(), commandLineType, hashKey, description, commandLine, QVector<QWidget*>() << controlledWidget);

    separator=sep;

    tags=taglist;

    /* if a Hash has been activated, build the terms-translation Hash table so that translated terms
   * can be translated back to original terms later on, so as to get the correct command line string chunks */

    if (terms != nullptr || translation != nullptr)
    {
        createHash(listWidgetTranslationHash, translation, terms);
    }

}


const FString& FListWidget::translate(const FStringList &s)
{
    FStringListIterator i(s)  ;
    FStringList L=FStringList();
    int j=0;

    while (i.hasNext())
    {
        L << QStringList();
        QStringList translation=QStringList();
        translation=i.next();
        L[j++]= applyHashToStringList(listWidgetTranslationHash, translation);
    }

    commandLineList[0] = L.join(separator);
    return commandLineList[0] ;
}



void FListWidget::setWidgetFromXml(const FStringList &s)
{
    /* for display */

    if (s.isFilled())
    {
        int size = s.size()-1;

        if (tabLabels.size() != size + 1) 
        {
                   QMessageBox::warning(nullptr, "Onglets", "Erreur de décodage des titres d'onglet  : tabLabels est de taille " +
                                         QString::number(tabLabels.size()) + " et la FStringList est de taille " + QString::number(size + 1));
                   return;
        }

        /* add as many groups as there are QStringLists in excess of 1 and fill in the tabs with files */
        for (int j=0; j <= size; j++)
          {
             if (j) addGroup();
             parent->widgetContainer[j]->addItems(Hash::wrapper[parent->frameHashKey]->at(j));

             parent->addNewTab(j, tabLabels.at(j));
          }
    }
    else
    {
        commandLineList = {FString()};
        return;
    }

    /* for command-line */
    /* if a Hash has been activated, strings are saved in Xml projects
    * as "translated" items to be displayed straightaway in list widgets
    * command lines, in this case, need to be translated back to original terms */

    if (!listWidgetTranslationHash.isEmpty())
        commandLineList= QList<FString>() << translate(s);
    else
    {
        if ((this->status & flags::status::widgetMask) == flags::status::hasListCommandLine)
        {
            commandLineList.clear();
           // Hash::wrapper[hashKey]->removeAll(QStringList());
            int size = Hash::wrapper[hashKey]->size();

            for (int i = 0; i < size - 3; ++i)
            {
                QStringList strL = Hash::wrapper[hashKey]->at(i);

                if (strL.isEmpty()) continue;
                for (const QString &s : strL)
                {
                    if (QFileInfo(s).exists())
                        commandLineList << FString(s);
                    else
                    {
                        QMessageBox::critical(nullptr,
                                              "Erreur d'importation du projet .alt",
                                              "Le fichier " + s + " n'existe pas.\n Importation annulée.\n"
                                              "N onglets = " +QString::number(size) + " onglet = " + QString::number(i) + "\n" +
                                              + " Hash : " + hashKey,
                                              QMessageBox::Ok);
                        emit(forceCloseProject());
                       return;
                    }
                }
            }
        }
        else
            commandLineList[0]= Hash::wrapper[hashKey]->join(separator);
    }


}



void FListWidget::addGroup(const QString &label)
{
        currentListWidget = new QListWidget;

        currentListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        currentListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        parent->widgetContainer << currentListWidget;
        if (parent->getRank() ==  Hash::wrapper[parent->frameHashKey]->size())
        {
             Hash::wrapper[parent->frameHashKey]->append(QStringList());
        }
        parent->addNewTab(parent->getRank(), label);
}


const FString FListWidget::setXmlFromWidget()
{
    if (Hash::wrapper[hashKey] == nullptr) return FString();

    if (! Hash::wrapper.contains(hashKey)) return FString(FStringList().setEmptyTags(tags));

    if (! listWidgetTranslationHash.isEmpty())
        commandLineList=QList<FString>() << translate(*Hash::wrapper[hashKey]);
    else
    {
        if ((this->status & flags::status::widgetMask)  == flags::status::hasListCommandLine)
        {
            commandLineList.clear();
            int size = Hash::wrapper[hashKey]->size();
            // Pour éviter d'inclure les onglets Siret et Budget dans la ligne de commande
            for (int k = 0; k < size - 3; ++k)
            {
                const QStringList strL = Hash::wrapper[hashKey]->at(k);
                if (strL.isEmpty()) continue;
                for (const QString &s : strL)
                    commandLineList << FString(s);
            }

        }
        else
            commandLineList[0] = Hash::wrapper[hashKey]->join(separator);
    }

    QVector<FStringList>* properties = new QVector<FStringList>;

    for (const QStringList &strL :  *Hash::wrapper[hashKey])
    {
      FStringList fstrl;
      for (const QString &str : strL)
      {
          QStringList qstrl = QStringList() << Hash::Mois[str]
                                            << (Hash::Siret[str].isEmpty()? "": Hash::Siret[str].at(0))
                                            << Hash::Budget[str]
                                            << (Hash::Etablissement[str].isEmpty()? "" : Hash::Etablissement[str].at(0));

          if  (Hash::Siret[str].size() > 1 && Hash::Etablissement[str].size() > 1)
          {
              for (int j = 1; j < Hash::Siret[str].size() && j < Hash::Etablissement[str].size(); ++j)
              {
                   qstrl  << Hash::Siret[str].at(j)
                          << Hash::Etablissement[str].at(j);
              }
          }

          qstrl << Hash::Employeur[str];

          fstrl  << qstrl;
      }

      //on réordonne

      *properties <<  fstrl;
    }

    QStringListIterator k(tabLabels);
    FStringList fstrl;
    while (k.hasNext())
         fstrl <<  QStringList(k.next());

    *properties << fstrl;

    return FString(Hash::wrapper[hashKey]->setTags(tags, properties));
}



FCheckBox::FCheckBox(const QString &boxLabel, int status, const QString &hashKey, const QStringList &description, const QString& commandLineString,
                     const QVector<QWidget*> &enabledObjects, const QVector<QWidget*> &disabledObjects) : QCheckBox(boxLabel)
{
    setLayoutDirection(Qt::RightToLeft);
    bool mode= ((status & flags::status::widgetMask) == flags::status::checked) ;
    widgetDepth="0";

    FCore({this}, FString(mode), status, hashKey, description, commandLineString, enabledObjects, disabledObjects);
}

void FCheckBox::uncheckDisabledBox()
{
    if (!this->isEnabled()) this->setChecked(false);
}

bool FCheckBox::refresh()
{
  if (! Hash::wrapper.contains(hashKey) || Hash::wrapper[hashKey] == nullptr) return false;
  *Hash::wrapper[hashKey] = commandLineList[0].fromBool(this->isChecked());
  return true;
}

const FString FCheckBox::setXmlFromWidget()
{
    if ( ! refresh()) return FString("non");
    return FString(commandLineList[0].toQStringRef());
}

void FCheckBox::setWidgetFromXml(const FStringList &s)
{
    QString st=s.toFString();
    commandLineList[0]=FString(st);

    // subtilité liée au fait qu'en se chargeant, la checkbox ne va pas nécessairement désactiver un élément qu'elle déclenche.
    // Il faut donc donner un signal
    if (this->enabledObjects || this->disabledObjects) this->setChecked(true);

    this->setChecked(commandLineList[0].isTrue());
    emit(toggled(this->isChecked()));
}

FComboBox::FComboBox(const QStringList &labelList,
                     const QStringList &translation,
                     int status,
                     const QString &hashKey,
                     const QStringList &description,
                     const QString &commandLine,
                     QList<QIcon> *iconList) : QComboBox()
{

    widgetDepth="0";
    addItems(labelList);
    if (labelList.isEmpty())
        return;

    FCore({this}, FString(labelList.at(0)), status, hashKey, description, commandLine);

    if (iconList)
    {
        int j=0;
        QListIterator<QIcon> i(*iconList);
        while (i.hasNext())
            setItemIcon(j++, i.next());
    }
    setIconSize(QSize(48,24));

   /* if a Hash has been activated, build the terms-translation Hash table so that translated terms
   * can be translated back to original terms later on, so as to get the correct command line string chunks */
    
    if ((!labelList.isEmpty()) && (!translation.isEmpty()))
    {
        createHash(comboBoxTranslationHash, &labelList, &translation);
    }

    connect(this, SIGNAL(currentTextChanged(const QString &)), this, SLOT(fromCurrentIndex(const QString &)));
 }

void FComboBox::fromCurrentIndex(const QString &text)
{
    commandLineList[0] = FString((!comboBoxTranslationHash.isEmpty())? comboBoxTranslationHash.value(text) : text);
    if (commandLineList[0].isEmpty()) commandLineList[0] = FString("  ");
}

bool FComboBox::refresh()
{
    if (! Hash::wrapper.contains(hashKey) || Hash::wrapper[hashKey] == nullptr) return false;
    *Hash::wrapper[getHashKey()] = FStringList(currentText());
    return true;
}

const FString FComboBox::setXmlFromWidget()
{
    if (! refresh()) return FString();
    commandLineList[0] =  FString(! comboBoxTranslationHash.isEmpty() ? comboBoxTranslationHash.value(currentText()) : "'" + currentText() + "'");
    if (commandLineList[0].isEmpty()) commandLineList[0] = FString("  ");
    return FString(commandLineList[0].toQStringRef());
}


void FComboBox::setWidgetFromXml(const FStringList &s)
{
    commandLineList[0] = s.toFString();
    FString str {commandLineList[0]};
    if (str.isEmpty()) return;

    if (!comboBoxTranslationHash.isEmpty())
        str = FString(comboBoxTranslationHash.key(str));

    if (findText(str.remove('\'')) != -1)
        setCurrentIndex(findText(str));
    else
        if (isEditable())
        {
            addItem(commandLineList[0]);
        }
}


FLineEdit::FLineEdit(const QString &defaultString, int status, const QString &hashKey, const QStringList &description, const QString &commandLine):QLineEdit()
{
    widgetDepth="0";

    FCore({this}, FString(defaultString), status, hashKey, description, commandLine);
    this->setText(defaultString);
}

bool FLineEdit::refresh()
{
  if (! Hash::wrapper.contains(hashKey) || Hash::wrapper[hashKey] == nullptr) return false;
  *Hash::wrapper[getHashKey()] = FString(this->text());
  return true;
}

const FString FLineEdit::setXmlFromWidget()
{
    if (!refresh()) return FString();
    commandLineList[0] = FString(this->text());
    return commandLineList[0];
}

void FLineEdit::setWidgetFromXml(const FStringList &s)
{
    commandLineList[0] = s.toFString();
    this->setText(commandLineList[0].toQString());
}


void FProgressBar::stop()
{
    if (parent->process.state() == QProcess::Running
        ||
        (parent->process.exitStatus() == QProcess::NormalExit))
        {
            bar->setValue(bar->maximum());
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
        parent->textAppend(PROCESSING_HTML_TAG "Analyse des bases de données...");
    }

    setValue(level);
}

void FProgressBar::computeLHXWritingProgressBar(bool print_message)
{
    if (print_message)
    {
      parent->textAppend(PROCESSING_HTML_TAG "Enregistrement des bases de données...");
      bar->setValue(0);
      bar->setRange(0, 100);
    }

    internalState = State::WritingStarted;

    if (parent->fileRank >= 100) parent->fileRank = startshift;

    setValue(std::max(parent->fileRank, value()));

#ifdef REPORT_DATABASE_PROGRESS
        parent->textAppend((QString)PROCESSING_HTML_TAG + QString::number() + " % des bases de données.");
#endif

}


void FProgressBar::computeRProgressBar()
{

      setRange(0, 100);
#ifdef DEBUG
      parent->textAppend((QString)PROCESSING_HTML_TAG + QString::number(static_cast<int>(parent->fileRank)) + " % de l'analyse des données.");
#endif
      setValue(parent->fileRank);

      if (parent->fileRank == 100) setValue(startshift);

}



FProgressBar::FProgressBar(FDialog* parent,
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
    connect(this->parent, SIGNAL(setProgressBar(int, int)), this, SLOT(setValue(int, int)));
    connect(this->parent, SIGNAL(setProgressBar(int)), this, SLOT(setValue(int)));
    connect(this->parent, &FDialog::hideProgressBar, [this] { hide(); });
    connect(this->parent, &FDialog::showProgressBar, [this] { bar->reset(); bar->show(); killButton->show(); killButton->setEnabled(true);});

}






