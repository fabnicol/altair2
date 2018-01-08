
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

/// Macro de débogage
/// \X Texte

#define Q(X) QMessageBox::about(nullptr, "test", X);

/// Macro de débogage
/// \X Nombre

#define q(X) QMessageBox::about(nullptr, "test", QString::number(X));

/// Accède à la linéarisation de la valeur déréférencée de Hash::wrapper correspondant à la clé X.
/// \param X clé
/// \return FString correspondant à la Hash::wrapper[X]->toFString()

#define v(X) (*FString(#X))

#if !defined(Q_OS_WIN) && !defined (Q_OS_LINUX)
#  error "This application will only compile for Windows or GNU/Linux operating systems."
#endif

/// Dialogue d'avertissement
/// \param title Titre
/// \param text Texte

#define  Warning(title, text)   QMessageBox::warning(0, title, text)

#define REQUIRE true

class tools : public flags
{

public:

  static QString userdatadir; ///< Répertoire d'importation des données de paye (\b xhl).
  static QString tempdir;


  /// Constructeur par défaut.

  tools()   { }

  /// Reliquat de la forme générale de la bibliothèque, qui permet aussi d'enchâsser des onglets.

  enum class TabWidgetTrait {NO_EMBEDDING_TAB_WIDGET=-1};

  /// Extrait le nom du sous-dossier d'un dossier de paye dans le cas d'une exportation de paye distributive.
  /// \param s Chemin du fichier
  /// \param subDir Répertoire global d'exportation
  /// \return Nom du sous-dossier de \em s dans \em subDir

  const QString getEmbeddedPath(QString s, QString subDir = "");

  /// Détecte si un disque optique est monté.
  /// \return Chemin du disque optique s'il est monté, blanc sinon.
  static QString cdRomMounted();

  /// Crée le chemin vers le dossier local de paramétrage
  /// \param path Sous répertoire créé. Si path est vide, le chemin par défaut.
  /// \return Chemin correspondant à ce sous-répertoire.\n
  ///         Sous linux : ~/.local/share/Altair/path

  static QString generateDatadirPath(const char* path = "");

  /// Crée le chemin vers le dossier local de paramétrage
  /// \param path Sous répertoire créé. Si path est vide, le chemin par défaut.
  /// \return Chemin correspondant à ce sous-répertoire.\n
  ///         Sous linux : ~/.local/share/Altair/path

  static QString generateDatadirPath(const QString &path);

  /// Calcule le cumul de la taille des fichiers, éventuellement restreints à une extension donnée, sans un répertoire.
  /// \param path Chemin du répertoire.
  /// \param extension Extension de fichier servant à filtrer le calcul.

  static qint64 getDirectorySize(const QString &path, const QString &extension="");

  /// Renomme un fichier
  /// \param ancien Ancien nom.
  /// \param nouveau Nouveau nom.
  /// \bool \em true si réussit, \em false sinon.

  bool renommer(const QString& ancien, const QString& nouveau);

  /// Remplace des chaînes de caractères, lorsqu'elles correspondent à une expression rationnelle, dans une chaîne de caractères donnée par une chaîne de caractères donnée.
  /// \param reg Expression rationnelle.
  /// \param repl Chaîne de caractère de remplacement.
  /// \param str Chaîne de caractères à modifier par référence.
  /// \return Booléen : \em true si la chaîne str est non vide après modification, \em false sinon.

  bool substituer(const QString &reg, const QString &repl, QString &str);

  /// Liste un fichier entre deux lignes données et sur une largeur donnée.
  /// \param path Chemin du fichier.
  /// \param list Liste des lignes du fichier lues et empilées.
  /// \param start Rang de la ligne de départ de la lecture du fichier (\em 1-based).
  /// \param start Rang de la ligne de fin de la lecture du fichier (\em 1-based).
  /// \param width Largeur de la lecture en nombre de caractères
  /// \return Rang (\em 1-based) de la dernière ligne lue.
  /// \note l'argument list est la sortie réelle de la fonction.

  int readFile(const QString &path, QStringList &list, int start=0, int stop=-1, int width=0);

  /// List un fichier entre deux lignes données et sur une largeur donnée.
  /// \param path Chemin du fichier.
  /// \param list Liste des lignes du fichier lues et empilées.
  /// \param start Rang de la ligne de départ de la lecture du fichier (\em 1-based).
  /// \param start Rang de la ligne de fin de la lecture du fichier (\em 1-based).
  /// \param width Largeur de la lecture en nombre de caractères
  /// \return Rang (\em 1-based) de la dernière ligne lue.
  /// \note l'argument \em list est la sortie réelle de la fonction.\n
  /// Surcharge de la méthode précédente.

  int readFile(const char* path, QStringList &list, int start=0, int stop=-1, int width=0)
  {
    QString pathstr=QString(path);
    return readFile(pathstr, list, start, stop, width);
  }

  /// List un fichier entre deux lignes données et sur une largeur donnée.
  /// \param path Chemin du fichier.
  /// \param start Rang de la ligne de départ de la lecture du fichier (\em 1-based).
  /// \param start Rang de la ligne de fin de la lecture du fichier (\em 1-based).
  /// \param width Largeur de la lecture en nombre de caractères
  /// \return Chaîne de caractères correspondant aux lignes lues et extraites.
  /// \note Surcharge des méthodes précédentes.

  QString readFile(const QString &path,  int start=0, int stop=-1, int width=0);

  /// List un fichier entre deux lignes données et sur une largeur donnée.
  /// \param path Chemin du fichier.
  /// \param start Rang de la ligne de départ de la lecture du fichier (\em 1-based).
  /// \param start Rang de la ligne de fin de la lecture du fichier (\em 1-based).
  /// \param width Largeur de la lecture en nombre de caractères
  /// \return Chaîne de caractères correspondant aux lignes lues et extraites.
  /// \note Surcharge de la méthode précédente.

  QString readFile(const char* path,  int start=0, int stop=-1, int width=0)
  {
    QString pathstr=QString(path);
    return readFile(pathstr, start, stop, width);
  }

/// Créer un fichier temporaire temp dans le répertoire racine et le remplit avec une chaîone de caractères.
/// \param chaine La chaîne de caractères en entrant.
/// \return Chemin du fichier temporaire (par défaut ~/Dev/altair/temp)

QString dump(const QString &chaine);


static bool zip (const QString& filename , const QString& zipfilename);


static bool unzip (const QString& zipfilename , const QString& filename);


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
