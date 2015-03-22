#include "altair.h"
#include "common.h"



inline const QString Altair::makeParserString(int start, int end)
{

    QStringList L=QStringList();

    for (int j = start; j <= end; j++)
    {

        FAbstractWidget* widget = Abstract::abstractWidgetList.at(j);
        QString hK = widget->getHashKey();

        if  (widget->getHashKey().isEmpty())
        {
            Warning0(tr("Erreur"), tr("Erreur d'analyse XML du projet"));
            continue;
        }

        QString xml = widget->setXmlFromWidget().toQString();
        QString widgetDepth = widget->getDepth();

        L <<  "  <" + hK + " profondeur=\"" + widgetDepth +  "\">\n   "
              + xml
              +"\n  </" + hK + ">\n";

    }

    return L.join("");

}


inline const QString  Altair::makeDataString()
{
    return  makeParserString(0,0);
}

inline const QString  Altair::makeSystemString()
{
    return makeParserString(1);
}


void Altair::writeProjectFile()
{
    QFile projectFile;
    checkEmptyProjectName();
    projectFile.setFileName(projectName);
    QErrorMessage *errorMessageDialog = new QErrorMessage(this);
    if (!projectFile.open(QIODevice::WriteOnly))
    {
        errorMessageDialog->showMessage(tr("Impossible d'ouvrir le fichier du projet\n")+ qPrintable(projectFile.errorString()));
        QLabel *errorLabel = new QLabel;
        errorLabel->setText(tr("Si cette case est décochée, ce message "
                               "ne s'affichera plus à nouveau."));
        return;
    }

    QTextStream out(&projectFile);
    out.setCodec("UTF-8");

    out << "<?xml version=\"1.0\"?>\n" <<"<projet>\n";
    out << " <data>\n";

    out << Altair::makeDataString();

    out << " </data>\n";
    out << " <systeme>\n";

    out << Altair::makeSystemString();

    out << " </systeme>\n <recent>\n";

    QStringListIterator w(parent->recentFiles);
    QString str;
    while (w.hasNext() && QFileInfo(str=w.next()).isFile())
        out    <<  "  <fichier>" << str << "</fichier>\n";

    out << " </recent>\n</projet>\n";
    out.flush();
    options::RefreshFlag=interfaceStatus::hasSavedOptions;
}

namespace XmlMethod
{

QTreeWidgetItem *itemParent=nullptr;

inline void stackData(const QDomNode & node, QStringList tags, int level, QVariant &textData, QStringList& tabLabels = QStringList())
{
    QDomNode  childNode=node.firstChild();

    QList<QVariant> stackedInfo;
    QStringList strL;
    QString str;
    QString annee;

    switch(level)
    {
    /* parses < tag> text </tag> */

    case 0:

        tags[0] = node.toElement().tagName();
        str.clear();
        while ((!childNode.isNull()) && (childNode.nodeType() == QDomNode::TextNode))
        {
            str += childNode.toText().data().simplified();
            childNode=childNode.nextSibling();
        }

        textData=QVariant(str);
        break;

        /*
         * parses < tags[0]>
                             <tags[1]>  text </tags[1]>
                             ....
                             <tags[1]> text </tags[1]>
                        </tags[0]>
             note: does not check subordinate tag uniformity
        */

    case 1:

        tags[0]=node.toElement().tagName();

        while (!childNode.isNull())
        {
            QVariant strV;

            stackData(childNode, QStringList(tags[1]), 0, strV);
            strL << strV.toString();
            childNode=childNode.nextSibling();
        }
        textData=QVariant(strL);
        break;

        /*
         *   parses
         *            <tags[0]>
         *               <tags[1]>
                             <tags[2]>  text </tags[2]>
                             ....
                             <tags[2]> text </tags[2]>
                         </tags[1]>
                         ...
                         <tags[1]>
                             <tags[2]>  text </tags[2]>
                             ....
                             <tags[2]> text </tags[2]>
                         </tags[1]>
                       </tags[0]>
        */

    case 2:
        tags[0]=node.toElement().tagName();

        childNode=node.firstChild();

        while (!childNode.isNull())
        {
            if (childNode.toElement().tagName() == "année")
            {
                  annee = childNode.toElement().attribute("V");
                  tabLabels += annee;
            }

            QStringList L={QString(), QString()};
            QVariant M;
            stackData(childNode, L,1, M);
            const QStringList SL = M.toStringList();
            for (const QString& s :  SL)
            {
                Hash::Annee[s] = annee;
            }

            stackedInfo << SL;
            tags[1]=L.at(0);
            tags[2]=L.at(1);
            childNode=childNode.nextSibling();
        }

        textData= QVariant(stackedInfo);
        break;
    }
}

/* computes sizes and sends filenames to main tab Widget */
/* displays on manager tree window */


void displayTextData(const QStringList &firstColumn,
                     const QString &secondColumn="",
                     const QString &thirdColumn="",
                     const QString &fourthColumn="",
                     const QString &fifthColumn="",
                     const QColor &color=QColor("blue"))
{
    static QString last;
    static QTreeWidgetItem* item;
    if ((firstColumn.at(0) != last) && !firstColumn.at(0).isEmpty())
    {
        item = new QTreeWidgetItem(XmlMethod::itemParent);
        item->setText(0, firstColumn.at(0));
        item->setExpanded(true);
    }

    last= firstColumn.at(0);

    if ((secondColumn.isEmpty()) && (firstColumn.count() == 1)) return;

    QTreeWidgetItem* item2 = new QTreeWidgetItem(item);
    if (firstColumn.count() > 1)
    {
        item2->setText(0, firstColumn.at(1));
    }
    else
    {
        if (!thirdColumn.isEmpty())  item2->setText(2, thirdColumn);
        if (!fourthColumn.isEmpty()) item2->setText(3, fourthColumn);
        if (!fifthColumn.isEmpty())  item2->setText(4, fifthColumn);
        
        if (color.isValid())
        {
            item2->setTextColor(2, color);
            item2->setTextColor(3, color);
        }
        
        item2->setTextAlignment(2, Qt::AlignRight);
        item2->setTextAlignment(3, Qt::AlignLeft);
        item2->setTextAlignment(4, Qt::AlignCenter);
    }

    item2->setText(1, secondColumn);
}



/* tags[0] k
 *                       tags[1] 1 : xxx  ...  size Mo
 *                       tags[1] 2 : xxx  ...  size Mo  */

inline qint64 displaySecondLevelData(    const QStringList &tags,
                                         const QList<QStringList> &stackedInfo,
                                         const QList<QStringList> &stackedSizeInfo,
                                         const QList<QStringList> &nBulletins)
{
    int count=0, tagcount=0, l;
    qint64 filesizecount=0;
    QString  firstColumn, root=tags.at(0), secondColumn=tags.at(1),
            thirdColumn, fourthColumn, fifthColumn;

    int tagListSize = tags.size();

    QListIterator<QStringList> i(stackedInfo), j(stackedSizeInfo), k(nBulletins);

    while (i.hasNext() && j.hasNext())
    {
        if (!root.isEmpty())
        {
            if (tagcount < tagListSize) firstColumn = tags.at(tagcount++);
        }

        displayTextData({firstColumn});

        QStringListIterator w(i.next()), y(j.next());
        l=0;

        while (w.hasNext() && y.hasNext())
        {
            ++count;
            if (!tags.at(1).isEmpty())
                secondColumn =  "fichier " + QString::number(++l) + "/"+ QString::number(count) +": ";

            secondColumn += w.next();
            fifthColumn =  "" ; //(z.hasNext())? z.next() : "";

            if ((stackedSizeInfo.size() > 0) && (y.hasNext()))
            {
                QStringList units=y.next().split(" ");
                qint64 msize=units.at(0).toLongLong();
                filesizecount += msize;
                // force coertion into float or double using .0
                thirdColumn    = QString::number(msize/1048576.0, 'f', 1); 
                fourthColumn   = QString::number(filesizecount/1048576.0, 'f', 1)+ " Mo" ;
            }

            displayTextData({""}, secondColumn, thirdColumn, fourthColumn, fifthColumn, (y.hasNext())? QColor("navy"): ((j.hasNext())? QColor("orange") :QColor("red")));
        }
    }
    
    return filesizecount;
}


/* tags[0]
 *                       tags[1] 1 : xxx  ...  (size Mo)
 *                       tags[1] 2 : xxx  ...  (size Mo) ... */

inline void displayFirstLevelData( const QString &tag,  const QString &style, const QStringList &stackedInfo)
{
    QStringListIterator i(stackedInfo);
    int count=0;
    while (i.hasNext())
    {
        ++count;
        displayTextData((count>1)?QStringList(""):QStringList(tag), style+" "+QString::number(count)+": "+i.next());
    }
}


}  // end of XmlMethod namespace


void Altair::parseProjectFile(QIODevice* file)
{
    // Beware: to be able to interactively modify managerWidget in the parseProjectFile child class constructor,
    // pass it as a parameter to the constructor otherwise the protected parent member will be accessible yet unaltered
    file->seek(0);

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(file, true, &errorStr, &errorLine, &errorColumn))
    {
        Warning0(tr("Décodage XML"), tr("Erreur de décodage ligne %1, " "colonne %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
        return;
    }

    QDomElement root=doc.documentElement();

    if (root.tagName() != "projet") return;

    parent->recentFiles.clear();

    QDomNode node= root.firstChild();

    /* this stacks data into relevant list structures, processes information
     * and displays it in the manager tree Widget  */

    Altair::totalSize[0]=0;

    for (const QString& maintag : {"data", "systeme", "recent"})
    {
        if (node.toElement().tagName() != maintag) return;

        QDomNode subnode=node.firstChild();

        while (!subnode.isNull())
        {
            FStringList &&str=parseEntry(subnode);
            if (!str.at(0).at(0).isEmpty())
                *(Hash::wrapper[subnode.toElement().tagName()] = new FStringList) =   str;
            subnode=subnode.nextSibling();
        }

        node = node.nextSibling();
    }

    //refreshProjectManagerValues();

    assignVariables();

    Hash::counter["XHL"] = 0 ;

    for (int group_index=0; group_index<= project[0]->getRank(); group_index++)
    {
        int r=0;
        for (QString text : Hash::wrapper["XHL"]->at(group_index))
        {
            if (!text.isEmpty())
                assignGroupFiles(group_index);
            r++;
        }

        refreshRowPresentation(group_index);

        Hash::counter["XHL"] += r;
    }
    emit(project[0]->is_ntabs_changed(Hash::wrapper["XHL"]->size()));
    emit(project[0]->is_ntracks_changed(Hash::counter["XHL"]));

    /* resets recent files using the ones listed in the dvp project file */

    parent->updateRecentFileActions();

    refreshProjectManagerValues(manager::refreshProjectInteractiveMode
                                | manager::refreshXHLZone
                                | manager::refreshSystemZone);

}



FStringList Altair::parseEntry(const QDomNode &node, QTreeWidgetItem *itemParent)
{

    QVariant textData;
    QStringList tags={QString(),QString(),QString()} ;
    int level=node.toElement().attribute("profondeur").toInt();

    XmlMethod::itemParent = itemParent;

    QStringList tabLabels;

    XmlMethod::stackData(node, tags, level, textData, tabLabels);

   // project[0]->setTabLabels(tabLabels);

    if ((level == 0) &&(tags[0] == "fichier"))
        parent->recentFiles.append(textData.toString());

    if (level == 2)
        project[0]->setTabLabels(tabLabels);

    switch (level)
    {
        case 0:  return FStringList(textData.toString());
        case 1:  return FStringList(textData.toStringList());
        case 2:  return FStringList(textData.toList());
    }

    return FStringList();
}

inline QList<QStringList> Altair::processSecondLevelData(QList<QStringList> &L, bool isFile)
{
    QListIterator<QStringList> i(L);
    int group_index=0;

    QList<QStringList> stackedSizeInfo2 ;
    while (i.hasNext())
    {
        QStringListIterator w(i.next());
        QStringList stackedSizeInfo1;
        while (w.hasNext())
        {
            QString text=w.next();

            if (isFile & QFileInfo(text).isFile())  // double check on file status. First check is for processing speed, so that QFileInfo is only called when necessary
            {
                // computing filesizes
                
                stackedSizeInfo1 <<  QString::number((long) QFileInfo(text).size())+" ";
                                    
            }
        }
        
        stackedSizeInfo2 << stackedSizeInfo1;
        group_index++;
    }

    return stackedSizeInfo2;
}

void Altair::refreshProjectManagerValues(std::uint16_t refreshProjectManagerFlag)
{
    managerWidget->clear();
    if ((refreshProjectManagerFlag & manager::refreshProjectInteractiveMask) == manager::refreshProjectInteractiveMode)
    {
        updateIndexInfo();
        fileSizeDataBase[0] = processSecondLevelData(*Hash::wrapper["XHL"]);
    }

    QTreeWidgetItem *item=new QTreeWidgetItem(managerWidget);
    item->setText(0, "Fichiers xhl");
    item->setExpanded(true);
    XmlMethod::itemParent=item;

    Altair::totalSize[0]=XmlMethod::displaySecondLevelData(
                            project[0]->getTabLabels(),
                           *Hash::wrapper["XHL"],
                            fileSizeDataBase[0],
                           *Hash::wrapper["NBulletins"]);

    if ((refreshProjectManagerFlag & manager::refreshNBulletinsMask) ==  manager::refreshNBulletins)
    {
        for (int i=0; i < Hash::wrapper["NBulletins"]->size(); ++i)
            for (int j=0; i < Hash::wrapper["NBulletins"]->at(i).size(); ++j)
               XmlMethod::displayTextData({""}, "", "", "", Hash::wrapper["NBulletins"]->at(i).at(j));
    }
                                                
        
    item=new QTreeWidgetItem(managerWidget);
    item->setText(0, "Logiciel");
    item->setExpanded(true);
    XmlMethod::itemParent=item;

    for (int k=2; k <Abstract::abstractWidgetList.count(); k++)
        {

            QString key=Abstract::abstractWidgetList[k]->getHashKey();

            if (Abstract::abstractWidgetList[k]->getDepth() == "0")
            {
                XmlMethod::displayTextData(Hash::description[key], Hash::wrapper[key]->toQString());
            }
            else if (Abstract::abstractWidgetList[k]->getDepth() == "1")
                XmlMethod::displayFirstLevelData(Hash::description[key].at(0),   "bouton", Hash::wrapper[key]->at(0));
        }



}
