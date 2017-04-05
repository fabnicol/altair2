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
  static QHash<QString, QStringList> Siret;
  static QHash<QString, QStringList> Etablissement;
  static QHash<QString, QString> Employeur;
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

