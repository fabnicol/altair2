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
#  error "This application will only compile for Windows or GNU/Linux operating systems."
#endif

#define  Warning(title, text)   QMessageBox::warning(0, title, text)

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

  static QString cdRomMounted();
  static QString userdatadir;
  static QString tempdir;
  static QString generateDatadirPath(const char* path="");
  static QString generateDatadirPath(const QString &path);
  static qint64 getDirectorySize(const QString &path, const QString &extension="");

  static const QString remAccents(QString str, bool toUpper = true);

 bool substituer(const QString &s, const QString &repl, QString &file_str);

  int readFile(const QString &path, QStringList &list, int start=0, int stop=-1, int width=0);
  int readFile(const char* path, QStringList &list, int start=0, int stop=-1, int width=0)
  {
    QString pathstr=QString(path);
    return readFile(pathstr, list, start, stop, width);
  }
  QString readFile(const QString &path,  int start=0, int stop=-1, int width=0);
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
bool renommer(const QString& ancien, const QString& nouveau);
QString dump(const QString &chaine);

void setWhatsThisText(QWidget* widget, int start, int stop);
static void openDir(QString path);
qint64 getFileSize(const QString &);
static const QString openDirDialog(flags::directory checkEmptyness = directory::noCheck);

static const QString path_access(const QString& s) {return QDir::toNativeSeparators(QDir::cleanPath(QCoreApplication::applicationDirPath()+ systemPathPrefix +  s)); }

static const QString getEmbeddedPath(QString s, QString subDir = "");

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
      // Passer les '/' soit avec QDir::toNativeSeparators() soit utiliser QDir::separator() sous Windows.
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
