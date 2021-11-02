
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
//
#ifndef FSTRING_H
#define FSTRING_H

#include <QtCore>
#include "enums.h"

class FString;
class FStringList;
class Hash;

/// Chaîne de caractères fonctionnelle

class FString : public QString
{
private:

 int x; ///< 1 : positif ; 0 : négatif; 2 : vide

 QString p; ///< si "oui" alors positif; si "non" alors négatif;

 /// Transforme 'oui', 'non' et autres chaines en FString (fixation de la valeur de \em x)
 /// \param s Chaîne entrante.
 /// \param flag Autre valeur que 2 pour \em x par défaut pour les chaines quelconques.

 void testBool(QString &s, flags::status flag = flags::status::defaultStatus)
 {
 if (s.isEmpty())
   x = 2;
 else
   {
     if (s == QString("oui"))
       x = 1;
     else
     if (s == QString("non"))
       x = 0;
     else
       // Preserving flagged status
         x = (flag != flags::status::defaultStatus)? static_cast<int>(flag) : 2;
    }
 }

public:

 /// Constructeur à valeur vide {2, "" }

 FString()
 {
   x = 2;
   p = "";
 }

 /// Constructeur de copie.
 /// \param s Chaîne FString.

  FString(const FString &s) : QString(s.p), x {s.x}, p {s.p} {}

 /// Constructeur par déplacement.
 /// \param s Chaîne FString.

  FString(FString &&s) : QString(s.p), x{s.x} { std::swap(p, s.p);}


 /// Constructeur à valeur quelconque QString donnée
 /// \param s Chaîne FString.
 /// \param flag Non utilisé.

  FString(QString s, flags::status flag = flags::status::defaultStatus) : QString(s)
 {
   p = s;
   testBool(s, flag);
 }

 /// Constructeur à valeur initiale quelconque const char* donnée
 /// \param s Chaîne de caractères simple.
 /// \param flag Non utilisé.

  FString(const char* s, flags::status flag = flags::status::defaultStatus) :  QString(s)
  {
      p = s;
      testBool(p, flag);
  }

 /// Constructeur à valeur booléenne.
 /// \param  value Booléen : \em true -> "oui" et \em false -> "non"

  FString(bool value)
 {
   x = static_cast<int>(value);

   p = (value == true) ? "oui" : "non";

   this->append(p);
 }

 /// Opérateur de déréférencement.
 /// \return La valeur de Hash::wrapper[p] convertie ("aplatie") en FString
 /// \note Exemple : *FString("base") -> chemin du répertoire de sortie

 FString  operator * ();

 /// Assignation par copie.
 /// \param s Chaîne FString.
 /// \return Copie du FString.

 FString& operator= (const FString &s) { x = s.x;  p = s.p; return *this;}

 /// Assignation par déplacement.
 /// \param s Chaîne FString.
 /// \return Déplacement du FString.

 FString& operator= (FString &&s) { x = s.x;  std::swap(p, s.p); return *this;}

  /// Opérateur & .
  /// "oui" & "oui" -> "oui" etc.
  /// \param s Chaîne FString.
  /// \return Chaîne de caractères FString résultant de l'opération logique.

 FString   operator & (FString  s)
  {
    if (x * s.x == 1) return FString("oui");
    else return FString("non");
  }

 /// Opérateur & .
 ///  s & b == FString("oui") <=> s == FString("oui") && b == true
 /// \param s Booléen.
 /// \return Chaîne de caractères FString résultant de l'opération logique.

 FString   operator & (bool  s)
  {
    return x * static_cast<int>(s) == 1 ?  FString("oui") : FString("non");
  }

  /// Opérateur &= .
  /// s &= true a même valeur que s, sinon FString("non").
  /// \param b Booléen.

  void   operator &= (bool  b)
  {
    x = x * static_cast<int>(b);
    p = x == 1 ? "oui" : "non";
  }

  /// Opérateur &= .
  /// s &= "oui" a même valeur que s\n
  /// s &= FString("oui") -> s\n
  /// s &= FString("non") -> FString("non")\n
  /// s &= FString() -> FString("non")\n
  /// \param s Chaîne FString.

  void   operator &= (FString  s)
  {
    x = x & s.x;
    p = x == 1 ? "oui" : "non";
  }

  /// Opérateur |.
  /// FString("oui") | FString("oui") -> FString("oui")\n
  /// FString("oui") | FString("non") -> FString("non")\n
  /// FString("non") | FString("oui") -> FString("non")\n

  /// \param s Chaîne FString.
  /// \return Chaîne de caractères FString résultant de l'opération logique.

  FString   operator | (FString  s)
  {
    return (x == 1 || s.x == 1 ) ? FString("oui") : FString("non");
  }

  /// Opérateur !.
  /// ! "non" -> "oui" et autres cas "non"\n
  /// ! FString("oui") -> {0, "non"}\n
  /// ! FString("non") -> {1, "oui"}\n
  /// \return Chaîne de caractère de polarité inversée.

  FString   operator ! ()
  {
    return x != 0 ? FString("non") : FString("oui");
  }

  /// Accesseur de la partie QString p
  /// \return Partie chaîne de caractères.

  QString toQString() const
  {
    return p;
  }

  /// Accesseur de la partie chaîne p.
  /// \return Partie chaîne de caractères.

  QString& toQStringRef()
  {
    return p;
  }

  /// Transformation en booléen.
  /// FString("oui").toBool() -> false\n
  /// FString("non").toBool() -> true\n
  /// FString("").toBool()    -> false\n
  /// \return Booléen.

  bool toBool()
  {
    return x != 1 ? false : true;
  }

  /// Variante de l'opérateur ! .
  /// Si s est vide s.toggle() -> "oui" alors que !s -> "non"\n
  /// FString("oui").toggle() -> {1, "non"}\n
  /// FString("non").toggle() -> {0, "oui"}\n
  /// FString("").toggle()    -> {2, "oui"}

  void toggle()
  {
    if (x == 1) x = 0;
    else
    if (x == 0) x =1;

    p = x ? "oui" : "non";
  }

  /// Inverse de isEmpty()
  /// \return \em false si la chaîne est vide, \em true sinon.

  bool isFilled()
  {
    return (!p.isEmpty());
  }

  /// Conversion depuis un booléen.
  /// \em true -> {1, "oui"} ; \em false -> {0, "non"}
  /// \param value Valeur booléenne à convertir en FString.
  /// \return FString converti.

  const FString  fromBool(bool value)
  {
    x = static_cast<int>(value);
    p = (value == true) ? "oui" : "non";
    return FString(p);
  }

  /// Test booléen sur la partie x du FString.
  /// FString("oui").isTrue() -> \em true. FString("non").isTrue() -> \em false.
  /// \return Booléen correspondant.

  bool isTrue()
  {
      return (x == 1);
  }

  /// Test booléen sur la partie x du FString.
  /// FString("oui").isFalse() -> \em false. FString("non").isFalse() -> \em true.
  /// \return Booléen correspondant.

  bool isFalse()
  {
      return (x == 0);
  }

  /// Test booléen sur la partie x du FString.
  /// \return Booléen correspondant : \em true si x vaut 1 ou 0, \em false sinon.

  bool isBoolean()
  {
    return (x == 0 || x == 1);
  }

  /// Sépare un FString en composants de liste délimités par un séparateur.
  /// \param sep séparateur
  /// \return Liste correspondante de composants.

  const FStringList split(const QString &sep) const;

  /// Sépare un FString en composants de liste délimités par un ou deux séparateurs.
  /// \param sep Liste d'un ou deux séparateurs.
  /// \return Liste correspondante de composants (un seul séparateur) ou liste de listes de séparateurs.

  const FStringList split(const QStringList &sep) const;
};


class FStringList : public QVector<QStringList>
{

public:

  FStringList( ) : QVector<QStringList>() { }
  FStringList(const QString& s) : QVector<QStringList>()  { this->append(QStringList(s)); }
  FStringList(const FString &s):FStringList(s.toQString()) {}
  FStringList(const FString *s):FStringList(s->toQString()) {}
  FStringList(QStringList &L): QVector<QStringList>() {this->append(L);}
  FStringList(const QStringList &L): QVector<QStringList>() {this->append(L);}
  FStringList(const  QVector<QStringList> &s) : QVector<QStringList>(s) {}
  FStringList(const  QVector<QVariant> &s) : QVector<QStringList>()
  {
      QVectorIterator<QVariant> i(s);
      while (i.hasNext())
      {
          QVariant v=i.next();
          if (!v.isValid()) continue;
          if (v.canConvert(QMetaType(QMetaType::QStringList)))
                        this->append(v.toStringList());
      }
  }

  FStringList(const QString& a, const QString& b, const QString& c):QVector<QStringList>()  { this->append(QStringList() << a << b << c);}
  const FString join(const QStringList &) const ;
  const FString join(const char* s) const {return join(QStringList((QString(s)))); }
  const QStringList join() ;
  QString setEmptyTags(const QStringList &)const;
  const QString setTags(const QStringList &tags, const QVector<FStringList> *properties=nullptr) const;
  FString toFString() const { return ((this->isEmpty()) || this->at(0).isEmpty())?  FString() : FString(this->at(0).at(0)); }
  QString toQString() const { return ((this->isEmpty()) || this->at(0).isEmpty())?  "" : this->at(0).at(0); }
  int toInt() const {return ((this->isEmpty() || this->at(0).isEmpty())? 0: this->at(0).at(0).toInt());}
  bool hasNoString() const { return (isEmpty() || (this->at(0).isEmpty()) || (this->at(0).at(0).isEmpty())); }
  bool  isFilled() const { return (!isEmpty() && (!this->at(0).isEmpty()) && (!this->at(0).at(0).isEmpty())); }

  /* copy constructor */
  FStringList(const FStringList  & L):QVector<QStringList>(L)  { }
              
};

class FStringListIterator : public QListIterator<QStringList>
{
public:
  FStringListIterator(const FStringList& list) : QListIterator(list) {}
  FStringListIterator(FStringList *list) : QListIterator(*list) {}
  FStringListIterator(const FStringList *list) : QListIterator(*list) {}
 };



class Hash : public QHash<QString, QString>
{
public:

  /* Hash::description converts a string like "targetDir" into its (sentence-like) description for display in project manager (first column)*/
  static QHash<QString,QStringList> description;

  /* Hash::wrapper  is used for storing information for xml project parsing/writing.
   *It converts a string label into a pointer to an FStringList object that contains a set of file paths
   * (or more generally, text phrases) grouped into a QStringList for each associated file in a list of files */
  static QHash<QString, FStringList*> wrapper;

  static QHash<QString, QString> Annee;
  static QHash<QString, QString> Mois;
  static QHash<QString, QString> Budget;

# ifdef HAVE_APPLICATION

  static QHash<QString, QStringList> Siret;
  static QHash<QString, QStringList> Etablissement;
  static QHash<QString, QString> Employeur;

# endif

  static QHash<QString, bool>    Suppression;
  static QVector<QStringList> Reference;
  static QHash<QString, QStringList> fileList;

  static inline void createReference(int rank)
  {
      Reference.clear();
      for(int j = 0; j <= rank && j < wrapper["XHL"]->size(); ++j)
      {
          (*wrapper["XHL"])[j].sort();
          Reference << wrapper["XHL"]->at(j);
      }
  }

  static QString aplatir(const QHash<QString, QStringList> &h, const QString &sep, const QString &subdir)
  {
      if (subdir.isEmpty()) return aplatir(h, sep);

      QStringList strL;
      for (auto && s : Hash::fileList[subdir])
      {
          QStringList M = h[s];
          M.removeAll("");
          M.removeDuplicates();
          if (M.isEmpty()) continue;
          for (auto &&u : M)
             strL <<  u;
      }

      strL.removeDuplicates();
      return strL.join(sep);
  }

  static QString aplatir(const QHash<QString, QString> &h, const QString &sep, const QString &subdir)
  {
      if (subdir.isEmpty()) return aplatir(h, sep);

      QStringList strL;
      for (auto && s : Hash::fileList[subdir])
      {
          strL << h[s];
      }
      strL.removeAll("");
      strL.removeDuplicates();
      return strL.join(sep);
  }


  static QString aplatir(const QHash<QString, QStringList> &h, const QString &sep)
  {
      QStringList strL;
      for (auto && s : h.values())
      {
          s.removeAll("");
          s.removeDuplicates();
          if (s.isEmpty()) continue;
          for (auto &&u : s)
             strL <<  u;
      }

      strL.removeDuplicates();
      return strL.join(sep);
  }

  static QString aplatir(const QHash<QString, QString> &h, const QString &sep)
  {
      QStringList strL = QStringList(h.values());
      strL.removeAll("");
      strL.removeDuplicates();
      return strL.join(sep);
  }

};

#endif // FSTRING_H

