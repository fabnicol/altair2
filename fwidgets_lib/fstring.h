
// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle. 

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

// Pour l'année 2017, une autorisation d'usage, de modification et de 
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières. Cette autorisation est temporaire et peut être 
// révoquée.
//
//
#ifndef FSTRING_H
#define FSTRING_H

#include <QtCore>
#include "enums.h"

class FString;
class FStringList;
class Hash;

class FString : public QString
{
private:
 int x;
 QString p;
 inline void testBool(QString &s, flags::status flag=flags::status::defaultStatus)
 {
 if (s.isEmpty())
   x=2;
 else
   {
     if (s == QString("oui"))
       x=1;
     else
     if (s == QString("non"))
       x=0;
     else
       // Preserving flagged status
         x= (flag != flags::status::defaultStatus)? static_cast<int>(flag) : 2;
    }
 }

public:


  inline FString   operator & (FString  s)
  {
    if (x * s.x == 1) return "oui";
    else return "non";
  }


  inline FString   operator & (bool  s)
  {
    if (x * s ==1) return "oui";
    else return "non";
  }

  void   operator &= (bool  s)
  {
    x = x & (int) s;
    if (x == 1) p="oui";
    else p="non";
  }

  void   operator &= (FString  s)
  {
    x = x & s.x;
    if (x ==1) p="oui";
    else p="non";
  }


  FString   operator | (FString  s)
  {
    if ((x == 1) || (s.x == 1) )return "oui";
    else return "non";
  }

  FString   operator ! ()
  {
    switch (x)
      {
        case  1:  return "non"; break;
        case  0:  return "oui"; break;
        default:  return "non";
      }
  }

  QString toQString() const
  {
    return p;
  }


  QString& toQStringRef()
  {
    return p;
  }



  short toBool()
  {
    if ( x > 1) return 0;
    else return x;
  }

  void toggle()
  {
    if (x == 1) x = 0;
    else
        if (x == 0) x =1;

    if (x) p = "oui"; else p = "non";
  }


  bool isFilled()
  {
    return (!p.isEmpty());
  }

  const FString  fromBool(bool value)
  {
    x=value;
    if (value) p="oui"; else p="non";
    return FString(p);
  }

  bool isTrue()
  {
      return (p == "oui");
  }

  bool isMultimodal()
  {
    return (x == static_cast<int>(flags::status::multimodal));
  }

  void setMultimodal()
  {
    x = static_cast<int>(flags::status::multimodal);
  }

  bool isFalse()
  {
      return (p == "non");
  }

  bool isBoolean()
  {
    return ((x == 0) | (x == 1));
  }

  inline FString()
  {
    x=2;
    p="";
  }

  inline FString(QString s, flags::status flag=flags::status::defaultStatus):QString(s)
  {
    p=s;
    testBool(s, flag);
  }

  inline FString(const char* s):FString(QString(s))  {  }

  inline FString(bool value)
  {
    x=value;
    if (value) p="oui"; else p="non";
    this->append(p); // caution! does not work otherwise
  }

  FString  operator * ();

  /* copy constructor */
  FString(const FString  & v):QString(v.p)
  {
    x=v.x;
    p=v.p;
  }


  const FStringList split(const QString &) const;
  const FStringList split(const QStringList &separator) const;



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
          if (v.canConvert(QMetaType::QStringList))
                        this->append(v.toStringList());
      }
  }

  FStringList(const QString& a, const QString& b, const QString& c):QVector<QStringList>()  { this->append(QStringList() << a << b << c);}
  const FString join(const QStringList &) const ;
  const FString join(const char* s) const {return join(QStringList((QString(s)))); }
  const QStringList join() ;
  QString setEmptyTags(const QStringList &)const;
  const QString setTags(const QStringList &tags, const QVector<FStringList> *properties=nullptr) const;
  FString toFString() const { return ((this->isEmpty()) || this->at(0).isEmpty())?  "" : FString(this->at(0).at(0)); }
  QString toQString() const { return ((this->isEmpty()) || this->at(0).isEmpty())?  "" : QString(this->at(0).at(0)); }
  int toInt() const {return ((this->isEmpty() || this->at(0).isEmpty())? 0: this->at(0).at(0).toInt());}
  bool hasNoString() const { return (isEmpty() || (this->at(0).isEmpty()) || (this->at(0).at(0).isEmpty())); }
  bool  isFilled() const { return (!isEmpty() && (!this->at(0).isEmpty()) && (!this->at(0).at(0).isEmpty())); }

  /* copy constructor */
  FStringList(const FStringList  & L):QVector<QStringList>(L)  { }
              
};

class FStringListIterator : public QVectorIterator<QStringList>
{
public:
  FStringListIterator(const FStringList& list) : QVectorIterator(list) {}
  FStringListIterator(FStringList *list) : QVectorIterator(*list) {}
  FStringListIterator(const FStringList *list) : QVectorIterator(*list) {}
 };



class Hash : public QHash<QString, QString>
{
public:

  /* Hash::description converts a string like "targetDir" into its (sentence-like) description for display in project manager (first column)*/
  static QHash<QString,QStringList> description;

  /* Hash::wrapper  is used for storing information for xml project parsing/writing.
   *It converts a string label like "audioMenu" into a pointer to an FStringList object that contains a set of file paths
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

