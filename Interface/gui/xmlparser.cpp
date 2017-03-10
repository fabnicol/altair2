// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie,
// de modification et de redistribution accordés par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
// seule une responsabilité restreinte pèse sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les concédants successifs.
//
// A cet égard l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels
// avertis possédant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invités à charger et tester l'adéquation du
// logiciel à leurs besoins dans des conditions permettant d'assurer la
// sécurité de leurs systèmes et ou de leurs données et, plus généralement,
// à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accepté les
// termes.
//
//
#include "altair.h"
#include "common.h"



inline const QString Altair::makeParserString(int start, int end)
{

    QStringList L=QStringList();
    int listsize = Abstract::abstractWidgetList.size();
    for (int j = start; j <= end && j < listsize; j++)
    {

        FAbstractWidget* widget = Abstract::abstractWidgetList.at(j);
        if (widget == nullptr) return "";
        QString hK = widget->getHashKey();

        if  (hK.isEmpty())
        {
            Warning(tr("Erreur"), tr("Erreur d'analyse XML du projet"));
            continue;
        }
        QString xml = widget->setXmlFromWidget().toQString();

        if (hK == "XHL" && xml.isEmpty()) continue;

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
                               "ne s'affichera plus à  nouveau."));
        return;
    }

    QTextStream out(&projectFile);
    out.setCodec("UTF-8");

    out << "<?xml version=\"1.0\"?>\n"
        <<"<projet version=\"" VERSION "\">\n";
    out << " <data>\n";

    out << Altair::makeDataString();

    out << " </data>\n";
    out << " <systeme>\n";

    out << Altair::makeSystemString();

    out << " </systeme>\n";

    out << "</projet>\n";
    out.flush();
    options::RefreshFlag=interfaceStatus::hasSavedOptions;

}

namespace XmlMethod
{

QTreeWidgetItem *itemParent=nullptr;

inline void stackData(const QDomNode & node, int level, QVariant &textData)
{
    QDomNode  childNode=node.firstChild();

    QStringList strL;
    QString str;

    switch(level)
    {
    /* parses < tag> text </tag> */

    case 0:

    //    tags[0] = node.toElement().tagName();
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
/* Add properties collection here to read month */
      //  tags[0]=node.toElement().tagName();

        while (!childNode.isNull())
        {
            QVariant strV;

            stackData(childNode, 0, strV);
            QString str = strV.toString();
            strL << str;
            QDomElement element = childNode.toElement();
            Hash::Mois[str] = element.attribute("V");
            Hash::Siret[str] << element.attribute("S");
            Hash::Budget[str] = element.attribute("B");
            Hash::Etablissement[str] << element.attribute("E");
            Hash::Employeur[str] = element.attribute("EM");
            QDomNamedNodeMap attribs = element.attributes();
            int i = 2;
            QString attr;

            while (attribs.contains(attr = "S" + QString::number(i)))
            {
                   Hash::Siret[str] << childNode.toElement().attribute(attr);
                   ++i;
            }
            i = 2;

            while (attribs.contains(attr = "E" + QString::number(i)))
            {
                   Hash::Etablissement[str] << childNode.toElement().attribute(attr);
                   ++i;
            }

            childNode=childNode.nextSibling();
        }
        textData=QVariant(strL);
        break;

        /*
         *   parses
         *            <tags[0]>
         *               <tags[1] V=tags[3]>
                             <tags[2] V=tags[4] S=tags[5] B=tags[6] E=tags[7]>  text </tags[2]>
                             ....
                             <tags[2] V=tags[4] S=tags[5] B=tags[6] E=tags[7]> text </tags[2]>
                         </tags[1]>
                         ...
                         <tags[1] V=tags[3]>
                             <tags[2]  V=tags[4] S=tags[5] B=tags[6] E=tags[7]>  text </tags[2]>
                             ....
                             <tags[2]  V=tags[4] S=tags[5] B=tags[6] E=tags[7]> text </tags[2]>
                         </tags[1]>
                       </tags[0]>
        */

  }
}

inline void stackData(const QDomNode & node, QVariant &textData, QStringList& tabLabels)
{
    QDomNode  childNode=node.firstChild();
    QList<QVariant> stackedInfo;
    QString annee;

//    tags[0]=node.toElement().tagName();
    childNode=node.firstChild();

    while (!childNode.isNull())
    {
        if (childNode.toElement().tagName() == "onglet")
        {
              annee = childNode.toElement().attribute("V");
//              if (annee[0] != '2') break;
              tabLabels += annee;
        }

        QVariant M;
        stackData(childNode, 1, M);
        const QStringList SL = M.toStringList();
        for (const QString& s :  SL)
        {
            Hash::Annee[s] = annee;
        }

        stackedInfo << SL;
        childNode=childNode.nextSibling();
    }

    textData= QVariant(stackedInfo);

}

/* computes sizes and sends filenames to main tab Widget */
/* displays on manager tree window */


void displayTextData(const QStringList &firstColumn,
                     const QString &secondColumn="",
                     const QString &thirdColumn="",
                     const QString &fourthColumn="",
                     const QString &fifthColumn="",
                     const QString &sixthColumn="",
                     const QString &seventhColumn="",
                     const QColor &color=QColor("blue"))
{
    static QString last;
    static QTreeWidgetItem* item;

    if ( (firstColumn.at(0) != last) &&  !firstColumn.at(0).isEmpty())
    {
        item = new QTreeWidgetItem(XmlMethod::itemParent);
        item->setText(0, firstColumn.at(0));
        item->setExpanded(true);
     }

    last= firstColumn.at(0);

    if ((thirdColumn.isEmpty()) && (firstColumn.count() == 1)) return;

    if (item == nullptr) return;

    QTreeWidgetItem* item2 = new QTreeWidgetItem(item);
    if (item2 == nullptr) return;
    if (firstColumn.count() > 1)
    {
        item2->setText(0, firstColumn.at(1));
    }
    else
    {
        if (!thirdColumn.isEmpty())   item2->setText(2, thirdColumn);
        if (!fourthColumn.isEmpty())  item2->setText(3, fourthColumn);
        if (!fifthColumn.isEmpty())   item2->setText(4, fifthColumn);
        if (!sixthColumn.isEmpty())   item2->setText(5, sixthColumn);
        if (!seventhColumn.isEmpty()) item2->setText(6, seventhColumn);

        if (color.isValid())
        {
            item2->setTextColor(3, color);
            item2->setTextColor(4, color);
        }

        item2->setTextAlignment(2, Qt::AlignLeft);
        item2->setTextAlignment(3, Qt::AlignRight);
        item2->setTextAlignment(4, Qt::AlignCenter);
        item2->setTextAlignment(5, Qt::AlignLeft);
        item2->setTextAlignment(6, Qt::AlignCenter);
    }

    item2->setText(1, secondColumn);

}



/* tags[0] k
 *                       tags[1] 1 : xxx  ...  size Mo
 *                       tags[1] 2 : xxx  ...  size Mo  */

inline qint64 displaySecondLevelData(    const QStringList &tags,
                                         const QVector<QStringList> &stackedInfo,
                                         const QVector<QStringList> &stackedSizeInfo)
{
    int count=0, tagcount=0, l;
    qint64 filesizecount=0;

    QString firstColumn,
            root=tags.at(0),
            secondColumn=" ",
            thirdColumn,
            fourthColumn,
            fifthColumn,
            sixthColumn,
            seventhColumn;

    int tagListSize = tags.size();

    QVectorIterator<QStringList> i(stackedInfo), j(stackedSizeInfo);

    while (i.hasNext() && j.hasNext())
    {
        if (!root.isEmpty() && tagcount < tagListSize)
                firstColumn = tags.at(tagcount++);

        if (firstColumn[0] != '2') break;
        displayTextData({firstColumn});

        QStringListIterator w(i.next()), y(j.next());
        l=0;

        while (w.hasNext() && y.hasNext())
        {
            ++count;

            thirdColumn =  "fichier " + QString::number(++l) + "/"+ QString::number(count) +": ";
            const QString filename = w.next();
            thirdColumn += filename;
            secondColumn =  Hash::Mois[filename];
            sixthColumn = "";
            for (int s = 0; s < Hash::Siret[filename].size() && s < Hash::Etablissement[filename].size(); ++s)
            {
                sixthColumn += ((s > 0)? "\n" : "") + Hash::Employeur[filename];
                sixthColumn += " " + Hash::Siret[filename].at(s);
                sixthColumn += " " + Hash::Etablissement[filename].at(s);
            }

            seventhColumn =  Hash::Budget[filename];
            if ((stackedSizeInfo.size() > 0) && (y.hasNext()))
            {
                QStringList units=y.next().split(" ");
                qint64 msize=units.at(0).toLongLong();
                filesizecount += msize;
                // force coertion into float or double using .0
                fourthColumn    = QString::number(msize/1048576.0, 'f', 1);
                fifthColumn   = QString::number(filesizecount/1048576.0, 'f', 1)+ " Mo" ;
            }

            displayTextData({""},
                            secondColumn,
                            thirdColumn,
                            fourthColumn,
                            fifthColumn,
                            sixthColumn,
                            seventhColumn,
                            (y.hasNext())? QColor("navy"): ((j.hasNext())? QColor("orange") :QColor("red")));
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
        Warning(tr("Décodage XML"), tr("Erreur de décodage ligne %1, " "colonne %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr));
        return;
    }

    QDomElement root=doc.documentElement();

    if (root.tagName() != "projet") return;

    Hash::wrapper.clear();

    Hash::wrapper["version"]  = new FStringList;
    if (Hash::wrapper["version"] == nullptr) return;

    *Hash::wrapper["version"] = FStringList(root.toElement().attribute("version"));

    QDomNode node= root.firstChild();

    /* this stacks data into relevant list structures, processes information
     * and displays it in the manager tree Widget  */

    Altair::totalSize[0]=0;

    for (const QString& maintag : {"data", "systeme"})
    {
        if (node.toElement().tagName() != maintag) return;

        QDomNode subnode=node.firstChild();

        while (!subnode.isNull())
        {
            FStringList &&str = parseEntry(subnode);
            //if (!str.at(0).at(0).isEmpty())
            {
                const QString key  = subnode.toElement().tagName();

                Hash::wrapper[key]  = new FStringList;
                *Hash::wrapper[key] =  str;
            }

            subnode=subnode.nextSibling();
        }

        node = node.nextSibling();
    }

    //refreshProjectManagerValues();
    project[0]->mainTabWidget->clear();

    assignWidgetValues();

    //Hash::wrapper["XHL"]->removeAll(QStringList());
    int projectRank = Hash::wrapper["XHL"]->size();
    if (projectRank == 0) return;

    for (int group_index=0; group_index< projectRank ; group_index++)
    {
        refreshRowPresentation(group_index);
        // Ne pas inclure les onglets Siret et Budget
    }

    emit(project[0]->is_ntabs_changed(projectRank));

    Hash::createReference(projectRank);

    refreshProjectManagerValues(manager::refreshProjectInteractiveMode
                                | manager::refreshXHLZone
                                | manager::refreshSystemZone);

}



FStringList Altair::parseEntry(const QDomNode &node, QTreeWidgetItem *itemParent)
{

    QVariant textData;
    int level=node.toElement().attribute("profondeur").toInt();

    XmlMethod::itemParent = itemParent;

    QStringList tabLabels;

    switch (level)
    {
        case 0:
                XmlMethod::stackData(node, 0, textData);
                return FStringList(textData.toString());
        case 1:
                XmlMethod::stackData(node, 1, textData);
                return FStringList(textData.toStringList());
        case 2:
                XmlMethod::stackData(node, textData, tabLabels);
                project[0]->setTabLabels(tabLabels);
                return FStringList(textData.toList().toVector());
    }

    return FStringList();
}

inline QVector<QStringList> Altair::processSecondLevelData(QVector<QStringList> &L, bool isFile)
{
    QVectorIterator<QStringList> i(L);
    int group_index=0;

    QVector<QStringList> stackedSizeInfo2 ;
    while (i.hasNext())
    {
        QStringListIterator w(i.next());
        QStringList stackedSizeInfo1=QStringList();
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
    QStringList tags = project[0]->getTabLabels();

    if (tags.isEmpty() || Hash::wrapper.isEmpty() || Hash::wrapper["XHL"] == nullptr || Hash::wrapper["XHL"]->isEmpty()) return;

    if ((refreshProjectManagerFlag & manager::refreshProjectInteractiveMask) == manager::refreshProjectInteractiveMode)
    {
        fileSizeDataBase[0] = processSecondLevelData(*Hash::wrapper["XHL"]);
    }

    QTreeWidgetItem *item=new QTreeWidgetItem(managerWidget);
    item->setText(0, "Fichiers xhl");
    item->setExpanded(true);
    XmlMethod::itemParent=item;

    Altair::totalSize[0]=XmlMethod::displaySecondLevelData(
                            tags,
                           *Hash::wrapper["XHL"],
                            fileSizeDataBase[0]);
    Altair::totalSize[0]+=1;

#if 0
    if ((refreshProjectManagerFlag & manager::refreshNBulletinsMask) ==  manager::refreshNBulletins)
    {
        for (int i=0; i < Hash::wrapper["NBulletins"]->size(); ++i)
            for (int j=0; i < Hash::wrapper["NBulletins"]->at(i).size(); ++j)
               XmlMethod::displayTextData({""}, "", "", "", Hash::wrapper["NBulletins"]->at(i).at(j));
    }
#endif

    item=new QTreeWidgetItem(managerWidget);
    item->setText(0, "Logiciel");
    item->setExpanded(true);
    XmlMethod::itemParent=item;
    QStringList L = Hash::wrapper.keys();

    for (int k=1; k < Abstract::abstractWidgetList.count(); k++)
    {
        const QString& key=Abstract::abstractWidgetList[k]->getHashKey();
        if (! L.contains(key) || Hash::wrapper[key] == nullptr) continue;

        if (Abstract::abstractWidgetList[k]->getDepth() == "0")
        {
            XmlMethod::displayTextData(Hash::description[key], Hash::wrapper[key]->toQString());
        }
        else
            if (Abstract::abstractWidgetList[k]->getDepth() == "1")
            XmlMethod::displayFirstLevelData(Hash::description[key].at(0), "bouton", Hash::wrapper[key]->at(0));
    }

}
