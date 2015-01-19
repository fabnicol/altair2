#ifndef ALTAIR_H
#define ALTAIR_H

#include "fstring.h"
#include "flistframe.h"
#include "common.h"
#include <QtXml>
#include "altair-gui.h"

class FProgressBar;
class MainWindow;

class Altair : public common
{
    Q_OBJECT

public:

    Altair();
    void setCurrentFile(const QString &fileName);
    MainWindow *parent;
    enum { MaxRecentFiles = 5 };
    static std::uint16_t RefreshFlag;
    static int dialVolume;
    QFileSystemModel *model = nullptr;
    QTreeWidget *managerWidget= new QTreeWidget;
    QTreeView *fileTreeView= new QTreeView;
    QString projectName;
    QString curFile;
    FListFrame *project[2];
    QPushButton *xhlFilterButton=new QPushButton;
    QToolButton *mkdirButton= new QToolButton;
    QToolButton *removeFileTreeElementsButton= new QToolButton;

    QTextEdit *outputTextEdit = new QTextEdit;

    void initializeProject(const bool cleardata=true);
    
    void checkEmptyProjectName()
      {
         if (projectName.isEmpty())
            projectName=QDir::currentPath() + "/défaut.alt";
      }

    void startDrag();
 /* void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event)*/
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    QPoint startPos;
    QProcess* process = new QProcess;
    QProcess ejectProcess;
    QString fileTreeFile;
    QString outputType;

    int fileRank=0;

    qint64 size() { return Altair::totalSize[0]; }
    void refreshProjectManagerValues(std::uint16_t = manager::refreshAllZones );

public slots:

   void updateProject(bool=false);
   void on_openProjectButton_clicked();
   void on_newProjectButton_clicked();

private slots:
    
    void on_moveUpItemButton_clicked();
    void on_moveDownItemButton_clicked();
    void on_deleteItem_clicked();
    void removeFileTreeElements();
    void createDirectory();
    void run();
    void runRAltair();
    void processFinished(int code) { processFinished(static_cast<exitCode>(code));}
    void processFinished(exitCode);
    void addGroup();
    void deleteGroup();
    void killProcess();
    void on_helpButton_clicked();
    void on_xhlFilterButton_clicked(bool active=false);
    void requestSaveProject();
    void writeProjectFile();
    void assignGroupFiles( const int group_index);
    void openProjectFile();
    void closeProject();

private:

    bool hasIndexChanged;
    int myTimerId=0;
    int row=0;
    uint currentIndex=0;
    qint64 value=0;
    static qint64 totalSize[2];

     QHash <int,  QList<QStringList>  > fileSizeDataBase;

    QIcon iconShowMaximized, iconShowNormal;
    
    FProgressBar *progress;
    QVBoxLayout *mainLayout= new QVBoxLayout;
    QVBoxLayout *progressLayout= new QVBoxLayout;
    QVBoxLayout *managerLayout= new QVBoxLayout;
    QHBoxLayout *allLayout= new QHBoxLayout;

    void assignVariables();
    void clearProjectData();
    QStringList createCommandLineString(flags::commandLineType commandLineType);
    void initialize();
    const QString  makeParserString(int start, int end=Abstract::abstractWidgetList.size()-1);
    const QString  makeDataString( );
    const QString  makeSystemString( );
    QList<QStringList> processSecondLevelData(QList<QStringList> &L, bool isFile=true);
    QList<QUrl> parseUrlsDragged(QList<QUrl>& urlsDragged);
    FStringList parseEntry(const QDomNode &, QTreeWidgetItem *parent=0);

    void refreshRowPresentation();
    void refreshRowPresentation(uint);
    void setIndexedProperties(QModelIndexList* indexList);
    void setDialogFromProject();
    void showFilenameOnly();
    void updateIndexInfo();
    void updateIndexChangeInfo();
    void displayTotalSize();
    void refreshModel();
    void refreshTreeView();

    void DomParser(QIODevice*);
    
    bool refreshProjectManager();
    void msg (const QString & text);
    void printMsg(qint64 new_value, const QString &str);
    void printBaseSize(qint64 new_value = 0);

    inline int removeFileTreeElement(int);
    int applyFunctionToSelectedFiles(int (Altair::*f)(int));

 protected:

    QString      sourceDir;
    unsigned int maxRange=0;

signals:

  void hasIndexChangedSignal();

};


class FProgressBar : public QWidget
{
   Q_OBJECT

    typedef  qint64 (Altair::*MeasureFunction)(const QString &, const QString &) ;
    typedef void (Altair::*DisplayFunction)(qint64 );
    typedef void (Altair::*SlotFunction)();

public:
    FProgressBar(Altair* parent,
                 SlotFunction  killFunction=nullptr);
   
    QHBoxLayout* layout=new QHBoxLayout;

    void show()
    {
        start();
        bar->reset();
        killButton->show();
        bar->show();
    }

    void start(int timeout=0)
    {
        timer->start(timeout);
        killButton->setEnabled(true);
    }

    void hide()
    {
        stop();
        bar->hide();
        killButton->hide();
        bar->reset();
    }

    void setToolTip(const QString & tip) { bar->setToolTip(tip); }
    void setInterval(int i) { timer->setInterval(i);}
    void setRange(int x, int y) { bar->setRange(x, y); }
    void setValue(int x) { bar->setValue(x); }
    void reset() {bar->reset();}
    int  value() { return bar -> value();}
    int maximum() { return bar->maximum();}
    void rewind()
    {
        show();
        setInterval(1200);
     }

 private:
    QToolButton* killButton=new QToolButton;
    QTimer *timer= new QTimer(this);
    QProgressBar *bar=new QProgressBar ;
    int startshift = 8;

    Altair* parent;

  public slots:
    void stop();

};


#endif
