
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
#include <iostream>
#include <string>
#include "fwidgets.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/// Macro de débogage
/// \param X Texte

#define Q(X) QMessageBox::about(nullptr, "test", X);

/// Macro de débogage
/// \param X Nombre

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

#define  Warning(title, text)   QMessageBox::warning(nullptr, title, text)

#define REQUIRE true

class tools : public flags
{

public:

static QString userdatadir; ///< Répertoire d'importation des données de paye (\b xhl).

static const QString   systemPathPrefix; ///< permet de remonter de l'application \em Altair à la racine de la distribution.

#ifdef Q_OS_WIN

    QString   System = "windows";
    QString   systemSuffix = ".exe";

    #ifndef LOCAL_BINPATH
    #define LOCAL_BINPATH
   #endif
#else
  #ifdef Q_OS_LINUX
      QString System = "linux";  ///< Type de système d'exploitation.
      QString   systemSuffix = ""; ///< Extension des exécutables.
      #ifndef PREFIX
       #define PREFIX "/usr"  ///< Répertoire usuel d'installation du répertoire bin/... des exécutables. Non-utilisé.
      #endif
  #endif
#endif

/// Constructeur par défaut.

tools()   { }

/// Reliquat de la forme générale de la bibliothèque, qui permet aussi d'enchâsser des onglets.

enum class TabWidgetTrait {NO_EMBEDDING_TAB_WIDGET=-1};

/// Extrait le nom du sous-dossier d'un dossier de paye dans le cas d'une exportation de paye distributive.
/// \param s Chemin du fichier.
/// \param subDir Répertoire global d'exportation.
/// \return Nom du sous-dossier de \em s dans \em subDir.

static const QString getEmbeddedPath(QString s, QString subDir = "");

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
/// \return Taille en octets.

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
/// \param stop  Rang de la ligne de fin de la lecture du fichier (\em 1-based). -1 pour "pas de fin".
/// \param width Largeur de la lecture en nombre de caractères.
/// \return Rang (\em 1-based) de la dernière ligne lue.
/// \note l'argument list est la sortie réelle de la fonction.

int readFile(const QString &path, QStringList &list, int start=0, int stop=-1, int width=0);

/// List un fichier entre deux lignes données et sur une largeur donnée.
/// \param path Chemin du fichier.
/// \param list Liste des lignes du fichier lues et empilées.
/// \param start Rang de la ligne de départ de la lecture du fichier (\em 1-based).
/// \param stop  Rang de la ligne de fin de la lecture du fichier (\em 1-based). -1 pour "pas de fin".
/// \param width Largeur de la lecture en nombre de caractères
/// \return Rang (\em 1-based) de la dernière ligne lue.
/// \note l'argument \em list est la sortie réelle de la fonction.\n
/// Surcharge de la méthode précédente.

int readFile(const char* path, QStringList &list, int start=0, int stop=-1, int width=0)
{
QString pathstr=QString(path);
return readFile(pathstr, list, start, stop, width);
}

/// Lit un fichier entre deux lignes données et sur une largeur donnée.
/// \param path Chemin du fichier.
/// \param start Rang de la ligne de départ de la lecture du fichier (\em 1-based).
/// \param stop  Rang de la ligne de fin de la lecture du fichi.er (\em 1-based). -1 pour "pas de fin".
/// \param width Largeur de la lecture en nombre de caractères
/// \return Chaîne de caractères correspondant aux lignes lues et extraites.
/// \note Surcharge des méthodes précédentes.

QString readFile(const QString &path,  int start=0, int stop=-1, int width=0);

/// List un fichier entre deux lignes données et sur une largeur donnée.
/// \param path Chemin du fichier.
/// \param start Rang de la ligne de départ de la lecture du fichier (\em 1-based).
/// \param stop Rang de la ligne de fin de la lecture du fichier (\em 1-based). -1 pour "pas de fin".
/// \param width Largeur de la lecture en nombre de caractères.
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


/// Compresser un fichier au format zip.
/// \param filename Chemin du fichier entrant.
/// \param zipfilename Chemin du fichier sortant.
/// \return Booléen : \em true si l'opération a réussi, \em false sinon.

static bool zip (const QString& filename , const QString& zipfilename);

/// Décompresser un fichier au format zip.
/// \param zipfilename Chemin du fichier entrant.
/// \param filename Chemin du fichier sortant.
/// \param del Effacer l'archive si la décompression se fait dans le même dossier (vrai par défaut).
/// \return Booléen : \em true si l'opération a réussi, \em false sinon.

static bool unzip (const QString& zipfilename , const QString& filename, bool del = true);
static bool unzip (const QString& zipfilename);
static bool unzip (const QString& dir, const QStringList& list);

/// Procédure de contrôle des entrées-sorties.
/// Vérifie que le fichier entrant existe, que le fichier sortant a pu être créé.
/// En cas de réécriture d'un fichier existant résultant de l'écrasement par un fichier sortant,\n
/// vérifie que la réécriture est autorisée et effective.
/// \param in Chemin du fichier entrant.
/// \param out Chemin du fichier sortant.
/// \param comment Commentaire préposé devant le message "va être écrasé" en cas de réécriture d'un fichier existant.
/// \param require Si \em require vaut \em true, alors si le fichier entrant n'existe pas, un dialogue d'erreur critique est déclenché.
/// \return Booléen : \em true si la vérification est réussie, \em false sinon.
/// \warning \em require =  \em true est à éviter pour les traitements de masse.

static bool IOControl(const QString& in, const QString& out, const QString& comment = "", bool require = false);

/// Copie un fichier de manière sécurisée.
/// Appelle tools::IOControl.
/// Vérifie que le fichier entrant existe, que le fichier sortant a pu être créé.
/// En cas de réécriture d'un fichier existant résultant de l'écrasement par un fichier sortant,\n
/// vérifie que la réécriture est autorisée et effective.
/// \param in Chemin du fichier entrant.
/// \param out Chemin du fichier sortant.
/// \param comment Commentaire préposé devant le message "va être écrasé" en cas de réécriture d'un fichier existant.
/// \param require Si \em require vaut \em true, alors si le fichier entrant n'existe pas, un dialogue d'erreur critique est déclenché.
/// \return Booléen : \em true si la vérification est réussie, \em false sinon.
/// \warning \em require =  \em true est à éviter pour les traitements de masse.


static bool copyFile(const QString &in, const QString &out, const QString& comment = "", bool require = false);

/// Copie un répertoire de manière sécurisée.
/// Vérifie que les fichiers sortants ont pu être créés.
/// En cas de réécriture d'un fichier existant résultant de l'écrasement par un fichier sortant,\n
/// vérifie que la réécriture est autorisée et effective.
/// \param in Chemin du répertoire entrant.
/// \param out Chemin du répertoire sortant.
/// \param comment Commentaire préposé devant le message "va être écrasé" en cas de réécriture d'un fichier existant.
/// \return Booléen : \em true si la vérification est réussie, \em false sinon.
/// \note Appelle tools::IOControl avec le paramètre \em require fixé à \em false.

static bool copyDir(const QString &in, const QString &out, const QString& comment = "");

/// Compresse l'intégralité d'un répertoire au format \b zip, et copie le dossier compressé dans un chemin donné.
/// \param inPath Chemin du répertoire entrant.
/// \param outPath Chemin du fichier sortant.
/// \return Booléen : \em true si l'opération a réussi, \em false sinon.

static bool zipDir (const QString& inPath , const QString& outPath);

/// Décompresse l'intégralité d'un répertoire au format \b zip, et copie le dossier décompressé dans un chemin donné.
/// \param inPath Chemin du fichier \b .zip entrant.
/// \param outPath Chemin du répertoire décompressé sortant.
/// \param filter Filtre sur les noms de fichier
/// \return Booléen : \em true si l'opération a réussi, \em false sinon.

static bool unzipDir (const QString& inPath , const QString& outPath, const QStringList& filter = {"*.arch"});

/// Décompresse sur place
/// \param inPath Chemin du fichier \b .zip entrant.
/// \param filter Filtre sur les noms de fichier

static bool unzipDir (const QString& inPath , const QStringList& filter = {"*.arch"});

/// Nettoyage d'un répertoire
/// \param inPath Chemin du fichier \b répertoire.
/// \param filter Filtre sur les noms de fichier supprimés.
/// \param excl_dir Répertoire protégé.
/// \return Booléen valant \e true en cas de succès.

static bool cleanDir (const QString& inPath , const QStringList& filter, const QString& excl_dir = "");

/// Remplace les caractères accentués par des caractères proches mains non accentué
/// \param str Chaîne de caractères à modifier.
/// \param toUpper Booléen : \em true si la chaîne doit être aussi mis en lettres majuscules, \em false sinon.
/// \return La chaîne de caractères modifiée.
/// \warning La chaîne à modifier est du type <i>r-value reference</i> et doit donc résulter d'un retour d'une autre fonction (par exemple).

static const QString remAccents(QString &&str, bool toUpper = false);

/// Vérifie l'existence d'un dossier et l'ouvre avec le gestionnaire de fichier du système d'exploitation.
/// \param path Chemin du dossier.

static void openDir(const QString& path);

/// Renvoie la taille d'un fichier en octets.
/// \param path Chemin du fichier.
/// \return Taille du fichier.

static qint64 getFileSize(const QString &path);


/// Sélectionne un répertoire et renvoie son chemin.
/// Vérifie que le répertoire est vide optionnellement, et dans ce cas le nettoie après s'être assuré de l'accord de l'utilisateur.
/// \param checkEmptyness Vérifie si le répertoire est vide (si vaut directory::checkEmptyness) et alors le nettoie avec l'accord de l'utilisateur. Si vaut directory::noCheck, ne fait rien.
/// \return Chemin du répertoire sélectionné.

static const QString openDirDialog(flags::directory checkEmptyness = directory::noCheck);

/// Renvoie le chemin d'un fichier ou d'un répertoire du répertoire racine de la distribution (adjacent au répertoire \em Interface)
/// \param s Nom du fichier ou répertoire.
/// \return Chemin complet du fichier ou répertoire.

static const QString path_access(const QString& s) {return QDir::toNativeSeparators(QDir::cleanPath(QCoreApplication::applicationDirPath()+ systemPathPrefix +  s)); }

};

#endif // tools_H
