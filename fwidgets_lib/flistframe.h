// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012
// fabrnicol@gmail.com
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

#include "fwidgets.h"
#include "fstring.h"
#include "tools.h"
#include <QToolButton>
#include <QFileSystemModel>
#include <QMessageBox>

class FListWidget;
class QToolDirButton;

class FListFrame : public FDialogConnections
{
Q_OBJECT

public:

 QVector<QListWidget*> widgetContainer;
 FListWidget *fileListWidget;
 QString frameHashKey;
 std::vector<QThread*> thread;
 int size = 0;
 QToolButton *importFromMainTree=new QToolButton;
 QStringList tabLabels;
# ifndef USE_RIGHT_CLICK

     QToolButton  *retrieveItemButton=new QToolButton,
                  *deleteGroupButton=new QToolButton,
                  *nppDisplayButton =  new QToolButton;
# endif

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

inline bool listConnected() { return isListConnected; }
inline int  getSlotListSize() {  return (isListConnected == true || isTotalConnected == true)? slotListSize : -1; }

FListFrame(QAbstractItemView* tree, short import_type,
           const QString &hashKey,
           const QStringList &description,
           const QString &command_line,
           int cli_type,
           const QStringList &separator,
           const QStringList &xml_tags,
           const QStringList &tabLabels,
           tools::TabWidgetTrait mainTabWidgetRank,
           QStringList* terms = nullptr,
           QStringList* translation = nullptr);


void initializeWidgetContainer() {   widgetContainer =  QVector<QListWidget*>() << new QListWidget; }
void initializeWidgetContainer(QListWidget* listWidget) {   widgetContainer =  QVector<QListWidget*>() << listWidget; }

void addParsedTreeToListWidget(const QStringList &strL);


protected:
    short importType;
    QStringList tags;

private:


 QProcess* launch;
 QStringList stringList;
 QListWidget *currentListWidget;

 void clearTabLabels() {fileListWidget->clearTabLabels();}

 int row, currentIndex,  slotListSize;
 bool isListConnected=false;
 bool isTotalConnected=false;
 bool activateOnStart=false;
 bool isTerminated = false;

 void list_connect(FComboBox* w);
 void list_connect(FListFrame* w);
 void total_connect(FListFrame* w);
 void updateIndexInfo();
 void deleteAllGroups(bool insertFirstGroup, bool eraseAllData);
 void clearWidgetContainer();
 void launch_thread(int rank);
 struct Header* elemPar;
 void parseXhlFile(const QString& fileName);
 void parseXhlFile();
 void addStringListToListWidget();
 QStringList parseTreeForFilePaths(const QStringList& stringList);
 void setStrikeOutFileNames(flags::colors color);

signals:

    void terminated();
    void is_ntabs_changed(int);
    void is_ntracks_changed(int);
    void imported();
    void parsed();
    void textAppend(const QString&);
    void setProgressBar(int, int);
    void setProgressBar(int);
    void hideProgressBar();
    void showProgressBar();
    void refreshRowPresentation(int);
    void updateProject(bool);
    void appRepaint();

public slots:

    void deleteGroup();
    void deleteGroup(int r);
    void addNewTab();
    void addNewTab(int r, const QString& label = "");
    void on_deleteItem_clicked();
    void setSlotListSize(int s);
    void showContextMenu();

protected slots:
    void on_importFromMainTree_clicked();
    void on_file_display(const QString& file);
    void finalise();


};



#endif // FLISTFRAME_H
