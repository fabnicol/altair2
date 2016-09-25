#ifndef COMMON_H
#define COMMON_H
#include <QtWidgets>
#include "fwidgets.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#define Q(X) QMessageBox::about(nullptr, "test", X);
#define q(X) QMessageBox::about(nullptr, "test", QString::number(X));
#define v(X) (*FString(#X))
#if !defined(Q_OS_WIN) && !defined (Q_OS_LINUX)
#error "This application will only compile for Windows or GNU/Linux operating systems."
#endif

#define  Warning(title, text)   QMessageBox::warning(0, title, text)
#define  Warning0(title, text)   QMessageBox::warning(0, title, text)
#define REQUIRE true

class common : public QDialog, public flags
{
  //Q_OBJECT

 private:
 //   QString whatsThisPath;

public:
   QString whatsThisPath;
  common()   {    whatsThisPath=generateDatadirPath("/whatsthis.info");  }
  
  enum class TabWidgetTrait {NO_EMBEDDING_TAB_WIDGET=-1};

  static QString tempdir;
  static QString generateDatadirPath(const char* path="");
  static QString generateDatadirPath(const QString &path);
  static qint64 getDirectorySize(const QString &path, const QString &extension="");

  static const QString remAccents(QString str, bool toUpper = true);

  int readFile(QString &path, QStringList &list, int start=0, int stop=-1, int width=0);
  int readFile(const char* path, QStringList &list, int start=0, int stop=-1, int width=0)
  {
    QString pathstr=QString(path);
    return readFile(pathstr, list, start, stop, width);
  }
  QString readFile(QString &path,  int start=0, int stop=-1, int width=0);
  QString readFile(const char* path,  int start=0, int stop=-1, int width=0)
  {
    QString pathstr=QString(path);
    return readFile(pathstr, start, stop, width);
  }

static bool zip (const QString& filename , const QString& zipfilename);
static bool unzip (const QString& filename , const QString& zipfilename);
static bool IOControl(const QString& in, const QString& out, const QString& comment="", bool require = false);
static bool copyFile(const QString &in, const QString &out, const QString& comment = "", bool require = false);
static bool copyDir(const QString &in, const QString &out, const QString& comment = "");
static bool zipDir (const QString& inPath , const QString& outPath);
static bool unzipDir (const QString& inPath , const QString& outPath);

void setWhatsThisText(QWidget* widget, int start, int stop);
static void openDir(QString path);
qint64 getFileSize(const QString &);
static const QString openDirDialog(flags::directory checkEmptyness = directory::noCheck);

static const QString path_access(const QString& s) {return QDir::toNativeSeparators(QDir::cleanPath(QCoreApplication::applicationDirPath()+ systemPathPrefix +  s)); }

public :
  QString  videoFilePath;
  static FString  htmlLogPath;
  static const QString   systemPathPrefix;

#ifdef Q_OS_WIN

      QString   System = "win" + QString(CORE2);
      QString   systemSuffix = ".exe";

      #ifndef LOCAL_BINPATH
      #define LOCAL_BINPATH
     #endif
#else
    #ifdef Q_OS_LINUX
        QString System = "linux";
        QString   systemSuffix = "";
        #ifndef PREFIX
         #define PREFIX "/usr"
        #endif
    #endif
#endif
#ifdef LOCAL_BINPATH
 /* insert executable at root of windows package */
  QString execPath = path_access(System);
  const QString sharedir = generateDatadirPath();
  QString altairCommandStr = execPath + QDir::separator()+("lhx"+ QString(systemSuffix));

    #ifdef MINIMAL
      QString RAltairDirStr = path_access("R/bin/x64");
      // Passer les '/' soit Ã  QDir::toNativeSeparators() soit utiliser QDir::separator() sous Windows.
      QString RAltairCommandStr = RAltairDirStr + QDir::separator() + "Rscript" + QString(systemSuffix);
    #else
      #ifdef __WIN32__
         QString RAltairDirStr = path_access("RStudio");
         QString RAltairCommandStr = RAltairDirStr + QDir::separator() + "bin" + QDir::separator() + "rstudio" + QString(systemSuffix) ;
      #else
         QString RAltairCommandStr = QString("/usr/bin/rstudio");
      #endif
    #endif

#else

   QString execPath = PREFIX+QString("/bin");
   QString sharedir = PREFIX+QString("/share/applications/altair");
   QString RAltairCommandStr = PREFIX+QString("/lib/rstudio/bin/rstudio");
   QString RAltairDirStr = QString("/lib/rstudio/bin");
   QString altairCommandStr = PREFIX+QString("/bin/lhx");

#endif
 
};

#endif // COMMON_H
