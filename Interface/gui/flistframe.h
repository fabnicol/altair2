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
#ifndef FLISTFRAME_H
#define FLISTFRAME_H

#include "altair.h"
#include "fwidgets.h"
#include "fstring.h"
#include "common.h"
#include "altair.h"
#include <QtXml>
#include <QToolButton>
#include <QFileSystemModel>

class Altair;
class FListWidget;
class QToolDirButton;

class FListFrame : public QWidget//: public common
{
Q_OBJECT


public:

 QVector<QListWidget*> widgetContainer;
 QString frameHashKey;
 std::vector<QThread*> thread;
 int size = 0;
 QToolButton *importFromMainTree=new QToolButton;
 #ifndef USE_RIGHT_CLICK
 QToolButton            *retrieveItemButton=new QToolButton,
                        *deleteGroupButton=new QToolButton,
                        *nppDisplayButton =  new QToolButton;
 #endif

 QTabWidget *mainTabWidget;
 QAbstractItemView *fileTreeView;
 QStringList* slotList= new QStringList;

 QFileSystemModel *model=new QFileSystemModel;
 QGroupBox *controlButtonBox=new QGroupBox;
 bool use_threads = false;
 /* accessors */

 inline QStringList getTabLabels(){
                                        QStringList labels;
                                        int r = getRank();
                                        for (int i=0; i <= r; ++i)
                                             labels << mainTabWidget->tabText(i);
                                        return labels;
                                   }

 int getRank() {return widgetContainer.size()-1;}
 const QString &getHashKey() const {return frameHashKey;}

 void initializeWidgetContainer();
 void initializeWidgetContainer(QListWidget* listWidget);
 void clearWidgetContainer();

inline QVector<QListWidget*>  getWidgetContainer() {return widgetContainer;}
inline int getWidgetContainerCount() {return widgetContainer.size();}
inline int getWidgetContainerCount(int g) {return widgetContainer[g]->count();}
inline QListWidget*  getWidgetContainer(int rank) {if (rank < widgetContainer.count()) return widgetContainer[rank]; else return nullptr;}
inline int getCurrentIndex() { return this->mainTabWidget->currentIndex(); }
inline QListWidget*  getCurrentWidget() { return widgetContainer.at(getCurrentIndex());}
inline QString  getCurrentLabel() { return this->mainTabWidget->tabText(getCurrentIndex());}
inline QString  getLabel(int index) { return this->mainTabWidget->tabText(index);}
QGroupBox* getControlButtonBox() { return controlButtonBox;}
void setControlButtonBoxVisible(bool x) {controlButtonBox->setVisible(x);}
inline int getCurrentRow() { return getCurrentWidget()->currentRow(); }

void setStatus(flags::status status) {fileListWidget->status=status;}
void setCommandLineType(flags::commandLineType cl) {fileListWidget->commandLineType=cl;}
void setSeparator(QStringList sep) { fileListWidget->separator[0]=sep[0]; fileListWidget->separator[1]=sep[1];}
void setOptionLabel(QString option) {fileListWidget->optionLabel=option;}
void setTabLabels(QStringList& tabLabels) { fileListWidget->setTabLabels(tabLabels);}

void addNewTab(int r, const QString& label="");
void addNewTab();
void deleteAllGroups(bool insertNewGroup = false, bool eraseAllData = true);
void addParsedTreeToListWidget(const QStringList& stringList);

void list_connect(FComboBox* w);
void list_connect(FListFrame* w);
void total_connect(FListFrame* w);

inline bool listConnected() { return isListConnected; }
inline int  getSlotListSize() {  return (isListConnected == true || isTotalConnected == true)? slotListSize : -1; }

FListFrame(QObject* parent,  QAbstractItemView * fileTreeView, short import_type, const QString &hashKey,
            const QStringList &description, const QString &command_line, int commandLineType, const QStringList &separator, const QStringList &xml_tags,
            common::TabWidgetTrait mainTabWidgetRank=common::TabWidgetTrait::NO_EMBEDDING_TAB_WIDGET,
           QStringList* terms=nullptr, QStringList* translation=nullptr);

public slots:
    void deleteGroup();
    void deleteGroup(int r);
    void on_deleteItem_clicked();
    void  setSlotListSize(int s) ;

protected slots:
    void on_importFromMainTree_clicked();

private slots:
    void on_xhl_display(const QString& xhl="");
    void finalise();

protected:
    short importType;
    QStringList tags;

private:

 inline void updateIndexInfo();
 QProcess* launch;
 FListWidget *fileListWidget;
 QStringList stringList;
 QListWidget *currentListWidget;

 void clearTabLabels() {fileListWidget->clearTabLabels();}

 int row, currentIndex,  slotListSize;
 bool isListConnected=false;
 bool isTotalConnected=false;
 bool activateOnStart=false;

 QStringList parseTreeForFilePaths(const QStringList& stringList);
 void addStringListToListWidget();
 void parseXhlFile(const QString& fileName);
 void parseXhlFile();
 void showContextMenu();
 void setStrikeOutFileNames(flags::colors);
 void launch_thread(int rank);

 Altair* altair;

signals:
    void is_ntabs_changed(int);
    void is_ntracks_changed(int);
    void imported();
    void parsed();

};

inline void FListFrame::updateIndexInfo()
{
  currentListWidget=qobject_cast<QListWidget*>(mainTabWidget->currentWidget());
  if (currentListWidget == nullptr) return;
  row=currentListWidget->currentRow();
  currentIndex=mainTabWidget->currentIndex();
}


#endif // FLISTFRAME_H
