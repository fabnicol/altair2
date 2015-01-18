#ifndef FLISTFRAME_H
#define FLISTFRAME_H

#include "fwidgets.h"
#include "fstring.h"
#include "common.h"
#include <QToolButton>
#include <QFileSystemModel>

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
 FListWidget* fileListWidget;
 QString frameHashKey;

 void addGroup();

 int row, currentIndex,  slotListSize;
 bool showAddItemButton=false;
 bool isListConnected=false;
 bool isTotalConnected=false;
 bool activateOnStart=false;
 void deleteAllGroups();
 QStringList parseTreeForFilePaths(const QStringList& stringList);
 bool addStringListToListWidget(const QStringList&, int size);
 void addStringListToHash(const QStringList & stringList, int size);
 
public:
 
 QToolButton *importFromMainTree=new QToolButton,
                        *moveDownItemButton=new QToolButton,
                        *moveUpItemButton=new QToolButton,
                        *retrieveItemButton=new QToolButton,
                        *clearListButton=new QToolButton,
                        *addGroupButton=new QToolButton,
                        *addItemButton=new QToolButton,
                        *deleteGroupButton=new QToolButton;
 
 QTabWidget *mainTabWidget, *embeddingTabWidget;
 QAbstractItemView *fileTreeView;
 QStringList* slotList= new QStringList;
 
 QFileSystemModel *model=new QFileSystemModel;
 QGroupBox *controlButtonBox=new QGroupBox, *tabBox=new QGroupBox;

 /* accessors */
 int getRank() {return widgetContainer.count()-1;}
 const QString &getHashKey() const {return frameHashKey;}
 
 void setStatus(flags::status status) {fileListWidget->status=status;}
 void setCommandLineType(flags::commandLineType cl) {fileListWidget->commandLineType=cl;}
 void setSeparator(QStringList sep) { fileListWidget->separator[0]=sep[0]; fileListWidget->separator[1]=sep[1];}
 void setOptionLabel(QString option) {fileListWidget->optionLabel=option;}
 
 void initializeWidgetContainer()
 {
    widgetContainer = QList<QListWidget*>() << fileListWidget->currentListWidget;
 }
 void clearWidgetContainer()
 {
    widgetContainer.clear(); ;
 }

inline QList<QListWidget*>  getWidgetContainer() {return widgetContainer;}
inline int getWidgetContainerCount() {return widgetContainer.count();}
inline int getWidgetContainerCount(int g) {return widgetContainer[g]->count();}
inline QListWidget*  getWidgetContainer(int rank) {if (rank < widgetContainer.count()) return widgetContainer[rank]; else return nullptr;}
inline QListWidget*  getCurrentWidget() { return widgetContainer.at(this->mainTabWidget->currentIndex());}
QGroupBox* getControlButtonBox() { return controlButtonBox;}
void setControlButtonBoxVisible(bool x) {controlButtonBox->setVisible(x);}

inline int getCurrentIndex() { return this->mainTabWidget->currentIndex(); }
inline int getCurrentRow() { return getCurrentWidget()->currentRow(); }

void addNewTab();

bool addParsedTreeToListWidget(const QStringList& stringList, int size) { return addStringListToListWidget(parseTreeForFilePaths(stringList), size); }
void list_connect(FComboBox* w);
void list_connect(FListFrame* w);
void total_connect(FListFrame* w);

inline bool listConnected() { return isListConnected; }
inline int  getSlotListSize() {  return (isListConnected == true || isTotalConnected == true)? slotListSize : -1; }

FListFrame(QObject* parent,  QAbstractItemView * fileTreeView, short import_type, const QString &hashKey,
            const QStringList &description, const QString &command_line, int commandLineType, const QStringList &separator, const QStringList &xml_tags,
            common::TabWidgetTrait mainTabWidgetRank=common::TabWidgetTrait::NO_EMBEDDING_TAB_WIDGET, QIcon* icon=nullptr, QTabWidget* parentTabWidget=nullptr,
           QStringList* terms=nullptr, QStringList* translation=nullptr, bool showAddItemB =false);


public slots:

    void deleteGroup();
    void deleteGroup(int r, int R);
    void on_deleteItem_clicked();
    void on_clearList_clicked(int currentIndex=-1);
    void  setSlotListSize(int s) ;

protected slots:

    void on_importFromMainTree_clicked();
    void on_moveDownItemButton_clicked();
    void on_moveUpItemButton_clicked();
    void addGroups(int);

protected:
    short importType;
    QStringList tags;

signals:
    void is_ntabs_changed(int);
    void is_ntracks_changed(int);
    void isControlButtonClicked();

};

inline void FListFrame::updateIndexInfo()
{
  fileListWidget->currentListWidget=qobject_cast<QListWidget*>(mainTabWidget->currentWidget());
  if (fileListWidget->currentListWidget == nullptr) return;
  row=fileListWidget->currentListWidget->currentRow();
  currentIndex=mainTabWidget->currentIndex();
}


#endif // FLISTFRAME_H
