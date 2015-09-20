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


private:

  inline void updateIndexInfo();
#if 0
  void deleteGroups(QList<int> &L);
#endif

 QList<QListWidget*> widgetContainer;
 FListWidget *fileListWidget;
 QListWidget *currentListWidget;
 QString frameHashKey;

 void addGroup();
 void clearTabLabels() {fileListWidget->clearTabLabels();}

 int row, currentIndex,  slotListSize;
 bool isListConnected=false;
 bool isTotalConnected=false;
 bool activateOnStart=false;

 QStringList parseTreeForFilePaths(const QStringList& stringList);
 bool addStringListToListWidget(const QStringList&);

 void parseXhlFile(const QString& fileName);
 void parseXhlFile(const QStringList& stringList);
 Altair* altair;
 
public:

 QToolButton *importFromMainTree=new QToolButton;
 #ifndef USE_RIGHT_CLICK
 QToolButton                       *retrieveItemButton=new QToolButton,
                        *deleteGroupButton=new QToolButton;
 #endif

 QTabWidget *mainTabWidget, *embeddingTabWidget;
 QAbstractItemView *fileTreeView;
 QStringList* slotList= new QStringList;

 QFileSystemModel *model=new QFileSystemModel;
 QGroupBox *controlButtonBox=new QGroupBox;

 /* accessors */

 QStringList  getTabLabels() { return fileListWidget->getTabLabels();}

 int getRank() {return widgetContainer.size()-1;}
 const QString &getHashKey() const {return frameHashKey;}

 void initializeWidgetContainer();
 void initializeWidgetContainer(QListWidget* listWidget);
 void clearWidgetContainer();

inline QList<QListWidget*>  getWidgetContainer() {return widgetContainer;}
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
bool addParsedTreeToListWidget(const QStringList& stringList) { return addStringListToListWidget(parseTreeForFilePaths(stringList)); }
void list_connect(FComboBox* w);
void list_connect(FListFrame* w);
void total_connect(FListFrame* w);

inline bool listConnected() { return isListConnected; }
inline int  getSlotListSize() {  return (isListConnected == true || isTotalConnected == true)? slotListSize : -1; }

FListFrame(QObject* parent,  QAbstractItemView * fileTreeView, short import_type, const QString &hashKey,
            const QStringList &description, const QString &command_line, int commandLineType, const QStringList &separator, const QStringList &xml_tags,
            common::TabWidgetTrait mainTabWidgetRank=common::TabWidgetTrait::NO_EMBEDDING_TAB_WIDGET, QIcon* icon=nullptr, QTabWidget* parentTabWidget=nullptr,
           QStringList* terms=nullptr, QStringList* translation=nullptr);


public slots:

    void deleteGroup();
    void deleteGroup(int r);
    void on_deleteItem_clicked();
    void  setSlotListSize(int s) ;
    void addGroups(int);

protected slots:

    void on_importFromMainTree_clicked();

protected:
    short importType;
    QStringList tags;

signals:
    void is_ntabs_changed(int);
    void is_ntracks_changed(int);


};

inline void FListFrame::updateIndexInfo()
{
  currentListWidget=qobject_cast<QListWidget*>(mainTabWidget->currentWidget());
  if (currentListWidget == nullptr) return;
  row=currentListWidget->currentRow();
  currentIndex=mainTabWidget->currentIndex();
}


#endif // FLISTFRAME_H
