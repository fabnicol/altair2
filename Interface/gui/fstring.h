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


class FStringList : public QList<QStringList>
{

public:

  FStringList( ) : QList<QStringList>() { }
  FStringList(const QString& s) : QList<QStringList>()  { this->append(QStringList(s)); }
  FStringList(const FString &s):FStringList(s.toQString()) {}
  FStringList(const FString *s):FStringList(s->toQString()) {}
  FStringList(QStringList &L): QList<QStringList>() {this->append(L);}
  FStringList(const QStringList &L): QList<QStringList>() {this->append(L);}
  FStringList(const  QList<QStringList> &s) : QList<QStringList>(s) {}
  FStringList(const  QList<QVariant> &s) : QList<QStringList>()
  {
      QListIterator<QVariant> i(s);
      while (i.hasNext())
      {
          QVariant v=i.next();
          if (!v.isValid()) continue;
          if (v.canConvert(QMetaType::QStringList))
                        this->append(v.toStringList());
      }
  }

  FStringList(const QString& a, const QString& b, const QString& c):QList<QStringList>()  { this->append(QStringList() << a << b << c);}
  const FString join(const QStringList &) const ;
  const FString join(const char* s) const {return join(QStringList((QString(s)))); }
  const QStringList join() ;
  QString setEmptyTags(const QStringList &)const;
  const QString setTags(const QStringList &tags, const QList<FStringList> *properties=nullptr) const;
  FString toFString() const {if (this == nullptr) return ""; else return ((this->isEmpty()) || this->at(0).isEmpty())?  "" : FString(this->at(0).at(0)); }
  QString toQString() const { return ((this->isEmpty()) || this->at(0).isEmpty())?  "" : QString(this->at(0).at(0)); }
  int toInt() const {return ((this->isEmpty() || this->at(0).isEmpty())? 0: this->at(0).at(0).toInt());}
  bool hasNoString() const { return (isEmpty() || (this->at(0).isEmpty()) || (this->at(0).at(0).isEmpty())); }
  bool  isFilled() const { return (!isEmpty() && (!this->at(0).isEmpty()) && (!this->at(0).at(0).isEmpty())); }

  /* copy constructor */
  FStringList(const FStringList  & L):QList<QStringList>(L)  { }
              
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
   *It converts a string label like "audioMenu" into a pointer to an FStringList object that contains a set of file paths
   * (or more generally, text phrases) grouped into a QStringList for each associated file in a list of files */
  static QHash<QString, FStringList*> wrapper;

  static QHash<QString, QString> Annee;
  static QHash<QString, QString> Mois;
  static QHash<QString, QString> Budget;
  static QHash<QString, QStringList> Siret;
  static QHash<QString, QList<int> > SiretPos;
  static QHash<QString, QStringList> Etablissement;
  static QHash<QString, QString> Employeur;
  static QHash<QString, bool>    Suppression;
  static QList<QStringList> Reference;
  static inline void createReference(int rank)
  {
      Reference.clear();
      for(int j = 0; j <= rank && j < wrapper["XHL"]->size(); ++j)
      {
          (*wrapper["XHL"])[j].sort();
          Reference << wrapper["XHL"]->at(j);
      }
  }

};

#endif // FSTRING_H

