
// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle (CPI). 

// L'auteur se réserve le droit d'exploitation du présent logiciel, 
// et notamment de reproduire et de modifier le logiciel, conformément aux 
// dispositions de l'article L.122-6 du même code. L'auteur se réserve le droit
// de modifier et de corriger les erreurs du logiciel, conformément aux termes 
// du deuxième alinéa du I de l'article L.122-6-1 du même code.

// Le présent logiciel doit être regardé comme un "Module Externe", pour 
// l'application des stipulations de la licence CeCILL, lorsque le code source, 
// ou une bibliothèque compilée à partir du présent code, sont utilisés pour 
// compiler tout logiciel faisant usage du présent code.

// Tout fichier contenant le présent code, ou des versions antérieures du 
// présent code, et faisant apparaître des termes contraires aux stipulations
// précédentes doit être regardé comme faisant partie d'actes préparatoires à
// l'élaboration du présent logiciel et non comme le logiciel lui-même, ou 
// comme des erreurs matérielles dans la copie des licences. Seules les 
// stipulations précédentes doivent être regardées comme s'appliquant au 
// présent logiciel.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de ces stipulations et que vous en avez accepté les
// termes.

// Sans préjudice des dispositions du CPI, une autorisation d'usage et de 
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières pour l'exercice de leurs fonctions publiques. 
// Le code ainsi mis à disposition ne peut être transmis à d'autres utilisateurs.
//



#ifndef tools_H
#define tools_H
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

class tools : public flags
{

public:

  tools()   { }
  enum class TabWidgetTrait {NO_EMBEDDING_TAB_WIDGET=-1};
  const QString getEmbeddedPath(QString s, QString subDir = "");
  static QString cdRomMounted();
  static QString userdatadir;
  static QString tempdir;
  static QString generateDatadirPath(const char* path = "");
  static QString generateDatadirPath(const QString &path);
  static qint64 getDirectorySize(const QString &path, const QString &extension="");
  bool renommer(const QString& ancien, const QString& nouveau);
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

static const QString ents(QString str, bool toUpper = true);
QString dump(const QString &chaine);
static bool zip (const QString& filename , const QString& zipfilename);
static bool unzip (const QString& filename , const QString& zipfilename);
static bool IOControl(const QString& in, const QString& out, const QString& comment="", bool require = false);
static bool copyFile(const QString &in, const QString &out, const QString& comment = "", bool require = false);
static bool copyDir(const QString &in, const QString &out, const QString& comment = "");
static bool zipDir (const QString& inPath , const QString& outPath);
static bool unzipDir (const QString& inPath , const QString& outPath);
static const QString remAccents(QString str, bool toUpper = false);
static void openDir(QString path);
static qint64 getFileSize(const QString &);
static const QString openDirDialog(flags::directory checkEmptyness = directory::noCheck);

static const QString path_access(const QString& s) {return QDir::toNativeSeparators(QDir::cleanPath(QCoreApplication::applicationDirPath()+ systemPathPrefix +  s)); }
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
 
};

#endif // tools_H
