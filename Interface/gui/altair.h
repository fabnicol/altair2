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
#ifndef ALTAIR_H
#define ALTAIR_H

#include "fstring.h"
#include "flistframe.h"
#include "common.h"
#include "altair-gui.h"

class FProgressBar;
class MainWindow;
class FListFrame;

class Altair : public common
{
    Q_OBJECT

public:

    Altair();
    void setCurrentFile(const QString &fileName);
    const QStringList  XML_FILTERS = {"*.xml", "*.xhl", "*.XML", "*.XHL"};    
    MainWindow *parent;
    enum { MaxRecentFiles = 5 };
    static std::uint16_t RefreshFlag;
    static int dialVolume;
    QFileSystemModel *model = nullptr;
    QTreeWidget *managerWidget= new QTreeWidget;
    QTreeView *fileTreeView = nullptr;
    QString projectName;
    FListFrame *project;


    QTextEdit *outputTextEdit = new QTextEdit;

    bool clearInterfaceAndParseProject();
    
    void checkEmptyProjectName()
      {
         if (projectName.isEmpty())
            projectName = userdatadir + QString("défaut.alt");
      }


    void startDrag();
 /* void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event)*/
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    QPoint startPos;
    QProcess process;
    QFile rankFile;
    QFile stateFile;
    QProcess ejectProcess;
    QString fileTreeFile;
    QString outputType;
    QString rootDir = "";

    int fileRank = 1;

    qint64 size() { return Altair::totalSize[0]; }
    void refreshProjectManagerValues(std::uint16_t = manager::refreshAllZones );
    void refreshRowPresentation(uint);

    FProgressBar* getProgressBar() { return progress; }

    inline int getFileCount() {return fileCount;}
    inline void __attribute__((always_inline)) readRankSignal()
    {
            if (! rankFile.exists()) return;
            int baInt = 0;

            if (rankFile.open(QFile::ReadOnly))
            {
                    baInt = rankFile.readLine().toInt();
                    rankFile.close();
            }
            if (baInt == -1) emit(setProgressBar(0, fileCount));
            fileRank = (baInt >= 1)? baInt : 1;

    }

    bool refreshProjectManager();
    void setProcessMsg(const QString& msg) {processMsg =  msg; }
    QStringList createCommandLineString(const QStringList &L = QStringList());
    void writeProjectFile();
    void refreshTreeView(bool create=false);
    QString username;
    
public slots:

   bool updateProject(bool=false);
   void on_openProjectButton_clicked();
   void on_newProjectButton_clicked();
   void on_deleteItem_clicked();
   void closeProject();
   void openProjectFileCommonCode();
   void importData();

private slots:
    
    void run();
    void runRAltair();
    void processFinished(int code) { processFinished(static_cast<exitCode>(code));}
    void processFinished(exitCode);
    void deleteGroup();
    void killProcess();
    void on_helpButton_clicked();
    void requestSaveProject();
    void openProjectFile();

private:

    bool hasIndexChanged;
    int myTimerId = 0;
    int fileCount = 0;
    int row = 0;
    uint currentIndex = 0;
    qint64 value = 0;
    static qint64 totalSize[2];

    QHash <int,  QVector<QStringList>  > fileSizeDataBase;

    QIcon iconShowMaximized, iconShowNormal;
    QRect geom;

    FProgressBar *progress;
    QVBoxLayout *mainLayout= new QVBoxLayout;
    QVBoxLayout *progressLayout= new QVBoxLayout;
    QVBoxLayout *managerLayout= new QVBoxLayout;
    QHBoxLayout *allLayout= new QHBoxLayout;
    void assignWidgetValues();
    void clearProjectData();

    void initialize();
    const QString  makeParserString(int start, int end=Abstract::abstractWidgetList.size()-1);
    const QString  makeDataString( );
    const QString  makeSystemString( );
    QString processMsg;
    
    QVector<QStringList> processSecondLevelData(QVector<QStringList> &L, bool isFile=true);
    QVector<QUrl> parseUrlsDragged(QList<QUrl>& urlsDragged);
    FStringList parseEntry(const QDomNode &, QTreeWidgetItem *parent=0);
    void refreshRowPresentation();
    void setIndexedProperties(QModelIndexList* indexList);
    void setDialogFromProject();
    void updateIndexInfo();
    void updateIndexChangeInfo();
    void displayTotalSize();
    void refreshModel();

    void parseProjectFile(QIODevice*);
    void msg (const QString & text);
    void printMsg(qint64 new_value, const QString &str);
    void printBaseSize(qint64 new_value = 0);

    void checkAnnumSpan();

    void runWorkerDistributed(bool);
    void runWorker(const QString& = "");

#if 0
    void normaliseMultiBudgetFiles(const QStringList& list);
#endif 
 protected:

    QString      sourceDir;
    unsigned int maxRange=0;

signals:

  void hasIndexChangedSignal();
  void setProgressBar(int, int);
  void setProgressBar(int);
  void hideProgressBar();
  void showProgressBar();


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
   
    QHBoxLayout* getLayout() {return layout;}
    void setToolTip(const QString & tip) { bar->setToolTip(tip); }

    int  value() { return bar -> value();}
    int maximum() { return bar->maximum();}

 private:

    QHBoxLayout* layout=new QHBoxLayout;
    QToolButton* killButton=new QToolButton;
    QTimer *timer= new QTimer(this);
    QProgressBar *bar=new QProgressBar ;
    int startshift = 3;
    inline void computeRProgressBar();
    inline void computeLHXParsingProgressBar();
    inline void computeLHXWritingProgressBar(bool = false);

    enum class  State {Parsing, WritingReady, WritingStarted};
    State internalState;

    Altair* parent;

    void setInterval(int i) { timer->setInterval(i);}
    void setRange(int x, int y) { bar->setRange(x, y); }

    void setCount(int x) { bar->setRange(0, x); }

    void start(int timeout = PROGRESSBAR_TIMEOUT)
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

  private slots:
    inline void setValue(int x, int y) { bar->setValue(x); bar->setMaximum(y);}
    void setValue(int x) { bar->setValue(x); }

  public slots:
    void stop();
    void showProgressBar()
    {
        start();
        bar->reset();
        killButton->show();
        bar->show();
        internalState = State::Parsing;
    }

};


#endif
