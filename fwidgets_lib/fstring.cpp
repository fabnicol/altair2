
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
#include "fstring.h"
#include "tools.h"



FString  FString::operator * ()
{
  return Hash::wrapper[p]->toFString();
}


const FStringList FString::split(const QString &sep) const
{
   QStringList Q=QStringList();
  for (int i=0; i < sep.size(); i++)
       Q << QString(sep[i]) ;
  return split(Q);
}


const FStringList FString::split(const QStringList &separator) const
{
  if (this->isEmpty()) return FStringList();

  short length=separator.size();

  // switch cannot be used here because of QListIterator declaration

  switch (length)
    {

    case 1:
      return this->toQString().split(separator[0]); break; //Beware of calling toQString() to avoid infinite loop.

    case 2:
      if (this->length() >= 3)
        {
            QListIterator<QString> i=QListIterator<QString>(this->toQString().split(separator[1]));
            QVector<QStringList> L=QVector<QStringList>();
            while (i.hasNext())
            L << i.next().split(separator[0]);
            return L;
        }

      [[fallthrough]];

    default:
      return FStringList(this);
    }
}



const QStringList FStringList::join()
{
  // Flattens FStringList into QStringList

  FStringListIterator i(this);
  QStringList S=QStringList();
  while (i.hasNext())
    {
      S << i.next();
    }
  return S;
}

const FString  FStringList::join(const QStringList &separator) const
{

  if (this->size() == 0) return FString();

  QStringList S;
  QString sep, str;
  FStringListIterator i(this);

   switch(separator.length())
    {
     case 0:
      for (int j=0; j < size() ; j++)
      {
        str += this->at(j).join("");
      }
      return FString(str);

    case 1:
       if (size() == 1)
          return FString(this->at(0).join(separator[0]));
      return FString();

   case 2:
      sep = QString(separator[0]);
      if (size() == 1)
        {
          return FString(this->at(0).join(sep));
        }
      S=QStringList();
      while (i.hasNext())
        S  << i.next().join(sep);
      sep=QString(separator[1]);
      return FString(S.join(sep));
   }

   return FString();
}

// version simplifiée

inline QStringList setDistributedTags(const QString & tag, const QStringList &tagged)
{
  QStringList taggedList;
  taggedList =  (tagged.isEmpty()) ? QStringList("") : tagged;
  QStringListIterator i(taggedList);
  QStringList S=QStringList();
  while (i.hasNext())
    {
      const QString item = i.next();
      QString str =  QString("<") + tag ;
      str += QString(">")+ item +QString("</")+tag+QString(">");
      S << str;
    }
  return S;
}


// version à liste de propriétés

inline QStringList setDistributedTags(const QString & tag,const FStringList &properties, const QStringList &tagged)
{
  QStringList taggedList;
  taggedList =  (tagged.isEmpty()) ? QStringList("") : tagged;
  QStringListIterator i(taggedList);

  FStringListIterator j(properties);
  QStringList S=QStringList();

  while (i.hasNext() && j.hasNext())
    {
      const QString item = i.next();
      const QStringList prop = j.next();
      QStringListIterator w(prop);
      QString str =  QString("<") + tag ;
      QStringList propStringLabels = {"V", "S", "B", "E", "EM" };  // Annee ou Mois, Siret, Budget, Etablissement, Employeur
      if (prop.size() > 5)
         for (int i = 2; i <= prop.size() - 3; ++i)
              propStringLabels << "S" + QString::number(i) << "E" + QString::number(i) << "EM" + QString::number(i);

      QStringListIterator z(propStringLabels);
      while (w.hasNext() && z.hasNext())
          str += " " + z.next() + "=\""+  w.next() +  "\"";
      str += QString(">")+ item +QString("</")+tag+QString(">");
      S << str;
    }
  return S;
}


QString FStringList::setEmptyTags(const QStringList & tags) const
{
  QStringListIterator i(tags);
  QString S="";
  while (i.hasNext())
    {
      QString tag=i.next();
      S =  "<" + tag + ">" + S + "</" + tag + ">" ;
    }
  return S;
}


const QString FStringList::setTags(const QStringList  &tags, const QVector<FStringList> *properties ) const
{
  if ((this == nullptr) ||  this->hasNoString())
  {
      // should trigger a crash
      return setEmptyTags(tags);
  }


    // deactivated

  if ((properties) && (properties->size() != size() + 1) )
      return setEmptyTags(tags);
   // deactivated

  QStringList S;

  if (tags.length() >= 3 ) return setEmptyTags(tags);

  for (int i=0; i < size(); ++i)
    {
      QStringList tagged=this->at(i);
      if  (tagged.isEmpty()) continue;
      QString str;
      if (properties)
      {
        str="     "+ setDistributedTags(tags[0], properties->at(i), tagged).join("\n     ");
      }
      else
        str="     "+ setDistributedTags(tags[0], tagged).join("\n     ");
      S << "\n"   + str   + "\n   ";
    }

  QString str;

  if (tags.length() == 1)
    {
      return S.join("");
    }
  else
    if (tags.length() == 2)
      {

      if (properties)
          str=setDistributedTags(tags[1],  properties->at(size()),  S).join("\n   ");
     else
          str=setDistributedTags(tags[1],  S).join("\n   ");
      }

   return (str);
}
