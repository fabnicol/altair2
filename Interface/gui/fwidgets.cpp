#include "fwidgets.h"
#include "common.h"
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


void FAbstractConnection::meta_connect(FAbstractWidget* w,  const Q2ListWidget *enabledObjects,  const Q2ListWidget *disabledObjects)
{
    if ((enabledObjects != nullptr) &&  (!enabledObjects->isEmpty()) )
    {

        QListIterator<QWidget*> componentlistIterator(w->getComponentList());
        Q2ListIterator objectlistIterator(*enabledObjects);
        while ((componentlistIterator.hasNext()) && (objectlistIterator.hasNext()))
        {
            QWidget* component=componentlistIterator.next();
            QListIterator<QWidget*> i(objectlistIterator.next());
            while (i.hasNext())
            {
                QWidget* item=i.next();
                if ((item == nullptr) || (component==nullptr)) continue;
                // This does not always work automatically through Qt parenting as it normally should, so it is necessary to reimplement enabling dependencies
                // e.g. for QLabels

                if (!component->isEnabled() || (component->metaObject()->className() == QString("FCheckBox") &&  ! static_cast<FCheckBox*>(component)->isChecked()))
                    item->setEnabled(false);

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

        QListIterator<QWidget*> newcomponentlistIterator(w->getComponentList());
        Q2ListIterator newobjectlistIterator(*disabledObjects);
        while ((newcomponentlistIterator.hasNext()) && (newobjectlistIterator.hasNext()))
        {

            QWidget* component=newcomponentlistIterator.next();
            QListIterator<QWidget*> j(newobjectlistIterator.next());
            while (j.hasNext())
            {
                QWidget* item=j.next();

                if ((item == nullptr) || (component==nullptr)) continue;

                connect(component, SIGNAL(toggled(bool)), item , SLOT(setDisabled(bool)));

                // this connection must follow the general enabling adjustments above
                if (item->metaObject()->className() == QString("FCheckBox"))
                    connect(component, SIGNAL(toggled(bool)), item, SLOT(uncheckDisabledBox()));
            }
        }
    }
}



inline void FAbstractWidget::FCore(const QList<QWidget*>& w, FString defaultCommandLine, int commandLineType, const QString &hashKey, const QStringList & description,
                  const QString &option, const QList<QWidget*>&enabledObjects, const QList<QWidget*>&disabledObjects)
{
    Q2ListWidget *dObjects=new Q2ListWidget, *eObjects=new Q2ListWidget;
    if (enabledObjects.isEmpty()) eObjects=nullptr;
    else
        *eObjects << enabledObjects;
    if (disabledObjects.isEmpty()) dObjects=nullptr;
    else
        *dObjects << disabledObjects;

    FCore(w, defaultCommandLine, commandLineType, hashKey, description, option, eObjects, dObjects);
}

inline void FAbstractWidget::FCore(const QList<QWidget*>& w, FString defaultCommandLine, int commandLineType, const QString &hashKey, const QStringList & description,
                  const QString &option, const Q2ListWidget *enabledObjects, const Q2ListWidget *disabledObjects)
{
    this->enabledObjects=enabledObjects;
    this->disabledObjects=disabledObjects;

    w.at(0)->setToolTip(description.at(1));

    w.at(0)->setEnabled((commandLineType & flags::status::enabledMask) ==  flags::status::enabled);
    if ((commandLineType & flags::status::widgetMask) ==  flags::status::checked)
       static_cast<FCheckBox*>(w.at(0))->setChecked((commandLineType & flags::status::widgetMask) ==  flags::status::checked);

    if ((status & flags::status::widgetMask) == flags::status::multimodal) { this->commandLineList[0].setMultimodal(); }
    this->status=static_cast<flags::status>(commandLineType & static_cast<int>(flags::status::statusMask));

    this->commandLineList= QList<FString>() << defaultCommandLine;
    this->componentList= w;
    this->hashKey=hashKey;
    this->commandLineType=static_cast<flags::commandLineType>(commandLineType & static_cast<int>(flags::commandLineType::commandLineMask));
    this->optionLabel=option;

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

    Abstract::abstractWidgetList.append(this);

    FAbstractConnection::meta_connect(this, this->enabledObjects, this->disabledObjects);

}


const QStringList FAbstractWidget::commandLineStringList()
{
    /* If command line option is ill-formed, or if a corresponding checkbox is unchecked (or negatively checked)
  * or if an argument-taking option has no-argument, return empty */
//Warning(nullptr, "", optionLabel+":"+QString::number(commandLineType & flags::::widgetMask));

    if (
               optionLabel.isEmpty() 
            || commandLineList.isEmpty()
            || commandLineList[0].isFalse()
           // ||  (commandLineList[0].toQString().isEmpty())
            ||  (this->isAbstractDisabled())) return {};

    QStringList strL=QStringList();

      if ((this->status & flags::status::widgetMask) == flags::status::hasListCommandLine)
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
          if (commandLineList[0].isTrue() | commandLineList[0].isMultimodal())
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
                if (!commandLineList[0].toQString().isEmpty())
                {
                    if (optionLabel.at(0) == '^')
                    {
                        if (commandLineList[0] != "  ")
                           strL = QStringList(optionLabel.mid(1).trimmed()+"="+commandLineList[0].toQString());
                    }
                    else
                    {
                       if (commandLineList[0] != "  ")
                       {
                           strL = (optionLabel.size() == 1)? QStringList() << "-"+optionLabel << commandLineList[0].toQString()
                                                        :QStringList("--"+optionLabel+"="+commandLineList[0].toQString());
                       }
                    }
                }
            }
         }
      
       return QStringList(strL);
}

/* caution : abstractWidgetList must have its first two elements as respectively being with "DVD-A" and "DVD-V" hashKeys. */

QList<FAbstractWidget*> Abstract::abstractWidgetList= QList<FAbstractWidget*>();


void Abstract::refreshOptionFields()
{
    QListIterator<FAbstractWidget*>  j(Abstract::abstractWidgetList);
    while (j.hasNext())
    {
        j.next()->refreshWidgetDisplay();
    }
}

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
    parent = par;
    Abstract::initializeFStringListHash(hashKey);

    setObjectName(hashKey+" "+description.join(" "));

    currentListWidget=new QListWidget;
    currentListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    currentListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    componentList=QList<QWidget*>() << currentListWidget;

    componentList[0]->setToolTip(description.at(1));

    FCore({this}, "", commandLineType, hashKey, description, commandLine, QList<QWidget*>() << controlledWidget);

    separator=sep;

    tags=taglist;

    /* if a Hash has been activated, build the terms-translation Hash table so that translated terms
   * can be translated back to original terms later on, so as to get the correct command line string chunks */

    if ((terms != nullptr) || (translation != nullptr))
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
        L[j++]= std::move(applyHashToStringList(listWidgetTranslationHash, translation));
    }

    return commandLineList[0]=L.join(separator);
}


void FListWidget::setWidgetFromXml(const FStringList &s)
{
    /* for display */

    if (s.isFilled())
    {
        int size=s.size()-1;

        if (tabLabels.size() != size + 1) 
        {
                   QMessageBox::warning(nullptr, "Onglets", "Erreur de décodage des titres d'onglet  : tabLabels est de taille " +
                                         QString::number(tabLabels.size()) + " et la FStringList est de taille " + QString::number(size + 1));
                   return;
        }

        /* add as many groups as there are QStringLists in excess of 1 and fill in the tabs with files */
         static_cast<FListFrame*>(parent)->addGroups(size) ;

         for (int j =0; j <= size; j++)
             static_cast<FListFrame*>(parent)->mainTabWidget->setTabText(j, tabLabels[j]);
    }
    else
    {
        commandLineList={""};
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
            int size = Hash::wrapper[hashKey]->size();
            for (int i = 0; i < size; ++i)
            {
                QStringList strL = Hash::wrapper[hashKey]->at(i);

                if (strL.isEmpty()) continue;
                for (const QString &s : strL)
                {
                    if (i >= size - 2 || QFileInfo(s).exists())
                        commandLineList << s;
                    else
                    {
                        QMessageBox::critical(nullptr,
                                              "Erreur d'importation du projet .alt",
                                              "Le fichier " + s + " n'existe pas. Importation annulée.",
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

const FString FListWidget::setXmlFromWidget()
{
    if (!Hash::wrapper.contains(hashKey)) return FStringList().setEmptyTags(tags);

    if (!listWidgetTranslationHash.isEmpty())
        commandLineList=QList<FString>() << translate(*Hash::wrapper[hashKey]);
    else
    {
        if ((this->status & flags::status::widgetMask)  == flags::status::hasListCommandLine)
        {
            commandLineList.clear();
            int size = Hash::wrapper[hashKey]->size();
            // Pour éviter d'inclure les onglets Siret et Budget dans la ligne de commande
            for (int k = 0; k < size - 2; ++k)
            {
                const QStringList strL = Hash::wrapper[hashKey]->at(k);
                if (strL.isEmpty()) continue;
                for (const QString &s : strL)
                    commandLineList << s;
            }

        }
        else
            commandLineList[0]=Hash::wrapper[hashKey]->join(separator);
    }

    QList<FStringList>* properties = new QList<FStringList>;

    for (const QStringList &strL :  *Hash::wrapper[hashKey])
    {
      FStringList fstrl;
      for (const QString &str : strL)
      {
          QStringList qstrl = QStringList() << Hash::Mois[str] << (Hash::Siret[str].isEmpty()? "": Hash::Siret[str].at(0))
                                                               << Hash::Budget[str] << (Hash::Etablissement[str].isEmpty()? "" :Hash::Etablissement[str].at(0));
          if (Hash::Siret[str].size() > 1 && Hash::Etablissement[str].size() > 1)
              for (int j = 1; j < Hash::Siret[str].size() && j < Hash::Etablissement[str].size(); ++j)
                   qstrl  << Hash::Siret[str].at(j) << Hash::Etablissement[str].at(j);
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

    return Hash::wrapper[hashKey]->setTags(tags, properties);
}


void FListWidget::refreshWidgetDisplay()
{
//    currentListWidget->clear();
//    if ((Hash::wrapper.contains(hashKey)) && (Hash::wrapper[hashKey]->count() > rank ))
//         currentListWidget->addItems(Hash::wrapper[hashKey]->at(getank));
}



FCheckBox::FCheckBox(const QString &boxLabel, int status, const QString &hashKey, const QStringList &description, const char* commandLineString,
                     const QList<QWidget*> &enabledObjects, const QList<QWidget*> &disabledObjects) : QCheckBox(boxLabel)
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


void FCheckBox::refreshWidgetDisplay()
{
    bool checked=commandLineList[0].isTrue();

    setChecked(checked);

    if ((enabledObjects) && (enabledObjects->size()))
    {
        QListIterator<QWidget*> i(enabledObjects->at(0));

        while (i.hasNext())
        {
            QWidget *item=i.next();
            if (item == nullptr) continue;
            item->setEnabled(checked);
        }
    }

    if ((disabledObjects) && (disabledObjects->size()))
    {
        QListIterator<QWidget*> i(disabledObjects->at(0));
        while (i.hasNext())
        {
            QWidget* item=i.next();
            if (item == nullptr) continue;
            item->setDisabled(checked);
        }
    }
}

const FString FCheckBox::setXmlFromWidget()
{
    *Hash::wrapper[getHashKey()]=commandLineList[0].fromBool(this->isChecked());
    return commandLineList[0].toQStringRef();
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

    FCore({this}, labelList.at(0), status, hashKey, description, commandLine);

    if (iconList)
    {
        int j=0;
        QListIterator<QIcon> i(*iconList);
        while (i.hasNext())
            setItemIcon(j++, i.next());
    }
    setIconSize(QSize(48,24));

    signalList=new QStringList;
    *signalList=QStringList() << labelList.at(0);

    /* if a Hash has been activated, build the terms-translation Hash table so that translated terms
   * can be translated back to original terms later on, so as to get the correct command line string chunks */
    
    if ((!labelList.isEmpty()) && (!translation.isEmpty()))
    {
        createHash(comboBoxTranslationHash, &labelList, &translation);
    }

    connect(this, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(fromCurrentIndex(const QString &)));
 }

void FComboBox::fromCurrentIndex(const QString &text)
{
    commandLineList[0]= (!comboBoxTranslationHash.isEmpty())? comboBoxTranslationHash.value(text) : text;
    if (commandLineList[0].isEmpty()) commandLineList[0]="  ";
    signalList->clear();
    for (int i=0; i < text.toInt() ; i++)
        *signalList << QString::number(i+1);
}


void FComboBox::refreshWidgetDisplay()
{
    FString str = commandLineList[0];
    if (str.isFilled())
    {
        if (!comboBoxTranslationHash.isEmpty()) 
            str = comboBoxTranslationHash.key(str);
        
        if (findText(str.remove('\'')) != -1)
            setCurrentIndex(findText(str));
        else
            if (isEditable())
            {
                addItem(commandLineList[0]);
            }
    }
}

const FString FComboBox::setXmlFromWidget()
{
    QString str=currentText();
    *Hash::wrapper[getHashKey()]=FStringList(str);
    commandLineList[0]=  (!comboBoxTranslationHash.isEmpty())? comboBoxTranslationHash.value(str) : "'"+str+"'";
    if (commandLineList[0].isEmpty()) commandLineList[0]="  ";
    return commandLineList[0].toQStringRef();
}


void FComboBox::setWidgetFromXml(const FStringList &s)
{
    commandLineList[0] = s.toFString();
    refreshWidgetDisplay();
}


FLineEdit::FLineEdit(const QString &defaultString, int status, const QString &hashKey, const QStringList &description, const QString &commandLine):QLineEdit()
{
    widgetDepth="0";

    FCore({this}, defaultString, status, hashKey, description, commandLine);
    this->setText(defaultString);
}


void FLineEdit::refreshWidgetDisplay()
{
    this->setText(commandLineList[0].toQString());
}

const FString FLineEdit::setXmlFromWidget()
{
    commandLineList[0]=FString(this->text());
    *Hash::wrapper[getHashKey()]=FStringList(this->text());
    if (commandLineList[0].isEmpty()) commandLineList[0]="  ";
    return commandLineList[0].toQStringRef();
}

void FLineEdit::setWidgetFromXml(const FStringList &s)
{
    commandLineList[0] = s.toFString();
    this->setText(commandLineList[0].toQString());
}

