#ifndef COMMON_H
#define COMMON_H
#include <QtWidgets>
#include "fwidgets.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifndef VERSION
#define VERSION " 15.04"
#endif


#define TREE_FILE 2
#define TREE_DIR  3

#define Max(X,Y) ((X>Y)? X : Y)
#define Q(X) QMessageBox::about(nullptr, "test", X);
#define q(X) QMessageBox::about(nullptr, "test", QString::number(X));
#define v(X) (*FString(#X))
#if !defined(Q_OS_WIN) && !defined (Q_OS_LINUX)
#error "This application will only compile for Windows or GNU/Linux operating systems."
#endif


#define  Warning(title, text)   QMessageBox::warning(0, title, text,  "Oui", "Non")
#define  Warning0(title, text)   QMessageBox::warning(0, title, text, "Fermer")


class common : public QDialog, public flags
{
  Q_OBJECT

 private:
 //   QString whatsThisPath;

public:
   QString whatsThisPath;
  common()   {    whatsThisPath=generateDatadirPath("/whatsthis.info");  }
  
  enum class TabWidgetTrait {NO_EMBEDDING_TAB_WIDGET=-1};

  static QString tempdir;
  static QString generateDatadirPath(const char* path="");
  static QString generateDatadirPath(const QString &path);

  qint64 getDirectorySize(const QString &path, const QString &extension="");

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
static void writeFile(QString & path, const QStringList &list, QFlags<QIODevice::OpenModeFlag> flag= QFile::WriteOnly | QFile::Truncate) ;
void setWhatsThisText(QWidget* widget, int start, int stop);
void openDir(QString path);
qint64 getFileSize(const QString &, const QString& ="");
const QString openDirDialog();


protected :
  QString  videoFilePath;
  static FString  htmlLogPath;
  const QString   systemPathPrefix = "/../";

#ifdef Q_OS_WIN

      const QString   System = "win";
      const QString   systemSuffix = ".exe";

      #ifndef LOCAL_BINPATH
      #define LOCAL_BINPATH
     #endif
#else
    #ifdef Q_OS_LINUX
        const QString System = "linux";
        const QString   systemSuffix = "";
        #ifndef PREFIX
         #define PREFIX "/usr"
        #endif
    #endif
#endif
#ifdef LOCAL_BINPATH
 /* insert executable at root of windows package */
  const QString execPath = QDir::toNativeSeparators(QDir::cleanPath(QCoreApplication::applicationDirPath()+systemPathPrefix+System));
  const QString sharedir = generateDatadirPath();
 
  QString altairCommandStr=execPath+ QDir::separator()+("lhx"+ QString(systemSuffix));

    #ifdef MINIMAL
      QString RAltairDirStr = QDir::toNativeSeparators(QDir::cleanPath(QCoreApplication::applicationDirPath()+ systemPathPrefix+ "R/bin/x64" ));
      // Passer les '/' soit à QDir::toNativeSeparators() soit utiliser QDir::separator() sous Windows.
      QString RAltairCommandStr = RAltairDirStr + QDir::separator() + "Rscript" + QString(systemSuffix);
    #else
      QString RAltairDirStr = QDir::toNativeSeparators(QDir::cleanPath(QCoreApplication::applicationDirPath()+ systemPathPrefix+ "RStudio"));
      QString RAltairCommandStr = RAltairDirStr + QDir::separator() + "bin" + QDir::separator() + "rstudio" + QString(systemSuffix);
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
