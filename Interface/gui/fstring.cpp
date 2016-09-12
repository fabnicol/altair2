#include "fstring.h"
#include "common.h"
QHash<QString,QStringList>    Hash::description;
QHash<QString, FStringList* >   Hash::wrapper;
QHash<QString, QString>  Hash::Annee;
QHash<QString, QString>  Hash::Mois;
QHash<QString, QString>  Hash::Budget;
QHash<QString, QStringList>  Hash::Siret;
QHash<QString, QStringList>  Hash::Etablissement;
QHash<QString, QString>  Hash::Employeur;
QHash<QString, bool>     Hash::Suppression;
QList<QStringList>     Hash::Reference;
QHash<QString, QList<int> > Hash::SiretPos;


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
            QList<QStringList> L=QList<QStringList>();
            while (i.hasNext())
            L << i.next().split(separator[0]);
            return L;
        }

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

  if (this->size() == 0) return "";

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
          return this->at(0).join(separator[0]);
      return "";

   case 2:
      sep=QString(separator[0]);
      if (size() == 1)
        {
          return this->at(0).join(sep);
        }
      S=QStringList();
      while (i.hasNext())
        S  << i.next().join(sep);
      sep=QString(separator[1]);
      return FString(S.join(sep));
   }

   return ("");
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
      QStringList propStringLabels = {"V", "S", "B", "E", "EM" };  // Année ou Mois, Siret, Budget, Etablissement, Employeur
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


const QString FStringList::setTags(const QStringList  &tags, const QList<FStringList> *properties ) const
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

  for (int i=0; i < size(); i++)
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
