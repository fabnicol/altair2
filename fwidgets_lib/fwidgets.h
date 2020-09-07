
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
#ifndef FWIDGETS_H
#define FWIDGETS_H

#include <QtWidgets>
#include "fstring.h"
#include "tags.h"

#define Q2VectorWidget QVector<QVector<QWidget*> >
#define Q2VectorIterator QVectorIterator<QVector<QWidget*> >

class FStringList;
class tools;
class FAbstractWidget;
class FListFrame;


class FDialogConnections : public QDialog
{
Q_OBJECT

public:
virtual void substituer_valeurs_dans_script_R(){}

signals:
 void setProgressBar(int, int);
 void setProgressBar(int);
 void hideProgressBar();
 void showProgressBar();

};


class FDialog : public FDialogConnections
{
Q_OBJECT

public:
  QString  outputType;
  QProcess process;
  int fileRank = 1;

public slots:
  virtual void killProcess(){}
  virtual void textAppend(const QString&) {}
};


class QToolDirButton : public QToolButton
{
public:
    enum class openBehavior : int {Directory, File};

    QToolDirButton(actionType type=actionType::Select)
    {
        switch (type)
        {
        case actionType::Select :
            setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
            break;

        case actionType::OpenFolder :
            setIcon(QIcon(":images/directory.png"));
            break;

        case actionType::BrowseFile :
            setIcon(QIcon(":images/document-open.png"));
        }
    }

    QToolDirButton(const QString&  st, const actionType  type=actionType::Select):QToolDirButton(type){setToolTip(st);}
    void setOpenBehavior(openBehavior value) { openType = value;
                                               QString icon = value == openBehavior::File ? ":images/document-open.png" : ":images/directory.png";
                                               setIcon(QIcon(icon));
                                               setToolTip( value == openBehavior::File ? "Ouvrir le fichier " : "Ouvrir le dossier ");}


    openBehavior getOpenBehavior() {return openType; }

 private:
    openBehavior openType = openBehavior::Directory;


};



class FAbstractConnection : QObject
{
  Q_OBJECT

public:

  static void meta_connect(FAbstractWidget* w,  const Q2VectorWidget *enabledObjects,  const Q2VectorWidget *disabledObjects=nullptr);
  static void meta_connect(FAbstractWidget* w,  const QVector<QWidget*> *enabledObjects=nullptr,  const QVector<QWidget*> *disabledObjects=nullptr)
  {
     meta_connect(w, &(*(new Q2VectorWidget) << *enabledObjects),  &(*(new Q2VectorWidget) << *disabledObjects));
  }

};

/* Note :
 *     Windows instantiation request non-recursive abstract class pointer lists, ie,
 *     a list of pointers to FAbstractWidget cannot be a member of FAbstractWidget
 *     This compiles but leads to severe runtime crashes (Qt5.0.2 + mingw-g++4.7 + windows XP or 7)
 *     Both compiles and runs OK under Linux however (Qt5.0.2 + g++4.7 or 4.8 + Ubuntu 13.04).
 *     The following one-member abstract structure works out this intriguing issue that remains poorly understood  */


class FAbstractWidget : public flags
{
public:

 Q2VectorWidget* enabledObjects = nullptr; ///< Pointeur vers un vecteur de vecteurs de widgets activés par le composant courant

 Q2VectorWidget* disabledObjects = nullptr; ///< Pointeur vers un vecteur de vecteurs de widgets désactivés par le composant courant
 // command-line interface type
 flags::commandLineType commandLineType;
 flags::status  status;

  void refresh() {}

  // is used for .alt Xml project writing: refresh Widget information and injects current Widget state into Hash::qstring as left-valued of <...hashKey=...>

 virtual const FString setXmlFromWidget()=0 ;

  // does the reverse of setXmlFromWidget : reads left value of <...hashKey=...> and injects it into commandLineList. Refreshes Widget state accordingly

  virtual void setWidgetFromXml(const FStringList& )=0;

 /// Retourne la clé de hashage de  Hash::wrapper correspondant au composant
 /// \return QString correspondant à la clé  hashKey

 const QString& getHashKey() const {return hashKey; }

 QVector<QWidget*>& getComponentList() { return componentList;}

 /// Accesseur en lecture de la \e profondeur du composant
 /// \return Retourne  widgetDepth

 const QString& getDepth() const {return widgetDepth; }

 /// Accesseur en lecture de la \e description du composant
 /// \return Retourne  description

 const QStringList& getDescription() const { return description; }

  // command-line interface maker

  virtual const QStringList commandLineStringList();

  // isEnabled() cannot be used as it would trigger lexical ambiguity with QWidget-inherited isEnabled() in subclasses
  // yet using virtual derivation makes it possible to invoke the QWidget-inherited isEnabled().

  virtual bool isAbstractEnabled() =0 ;
  bool isAbstractDisabled() {return !isAbstractEnabled();}
  QString optionLabel;
  QList<FString> commandLineList;

  void setDisableObjects(const QVector<QWidget*>& L)  
  {
      if (disabledObjects) delete disabledObjects;
      disabledObjects = new Q2VectorWidget;
      *disabledObjects = Q2VectorWidget() << L;
      FAbstractConnection::meta_connect(this, nullptr, disabledObjects);
  }
  
  void setEnableObjects(const QVector<QWidget*>& L)  
  {
      if (enabledObjects) delete enabledObjects;
      enabledObjects = new Q2VectorWidget;
      *enabledObjects = Q2VectorWidget() << L;
      FAbstractConnection::meta_connect(this, nullptr, enabledObjects);
  }
  
  void setHashKey(const QString& hash) { hashKey = hash; }
      
protected:
  QString hashKey;
  QString widgetDepth;
  QStringList description;

  QVector<QWidget*> componentList;

  void FCore(const QVector<QWidget*>& , FString, int, const QString&, const QStringList& , const QString&,  const QVector<QWidget*> =QVector<QWidget*>(), const QVector<QWidget*> =QVector<QWidget*>());

  void FCore(const QVector<QWidget*>&, FString, int, const QString &, const QStringList &, const QString&, Q2VectorWidget*, Q2VectorWidget*);
  
};


struct Abstract
{
    static QVector<FAbstractWidget*> abstractWidgetList; ///< englobe tous les widgets fonctionnels utilisés


    /// Détruire la valeur allouée de  Hash::wrapper pour la clé \e hashKey si elle existe
    /// et si elle est allouée et réallouer une nouvelle
    /// \param hashKey Clé à réallouer
    /// \param value Nouvelle valeur

    static void initH(const QString &hashKey, const QString& value)
    {
       if (! Hash::wrapper.isEmpty()
                && Hash::wrapper.value(hashKey, nullptr) != nullptr)
                 delete(Hash::wrapper[hashKey]);

        *(Hash::wrapper[hashKey] = new FStringList) = value;
    }

    /// Détruire la valeur allouée de  Hash::wrapper pour la clé \e hashKey si elle existe
    /// et si elle est allouée et réallouer une nouvelle
    /// \param hashKey Clé à réallouer

    static void initH(const QString &hashKey)
    {
        // Détruire la valeur allouée de Hash::wrapper[hashKey] si elle existe
        // et si elle est allouée et réallouer une nouvelle à blanc

        if (! Hash::wrapper.isEmpty()
                && Hash::wrapper.value(hashKey, nullptr) != nullptr)
                 delete(Hash::wrapper[hashKey]);

        Hash::wrapper[hashKey] = new FStringList;
    }

    /// Réinitialiser toutes les valeurs du de la table de hachage  Hash::wrapper

    static void initH()
    {
        for (const QString& hashKey: Hash::wrapper.keys()) initH(hashKey);
    }

    /// Récupère toutes les clés de hachage (de Hash::wrapper) des FWidgets qui
    /// sont dans le vecteur FAbstractWidget::abstractWidgetList, qui englobe tous
    /// les widgets fonctionnels utilisés
    /// \return Une QStringList de toutes les clés

    static QStringList hashKeys()
    {
        QStringList L;
        for (auto &&a : abstractWidgetList)
            L << a->getHashKey();

        return L;
    }

    static void refresh()
    {
         for (auto  &&a : abstractWidgetList)
             a->refresh();
    }

};

class FListWidget : public QWidget, public FAbstractWidget
{
  Q_OBJECT

  friend class FAbstractWidget;

public:

  FListWidget(QWidget *parent, 
              const QString &hashKey = QString(),
              int commandLineType = 0, 
              const QStringList &description = QStringList(),
              const QString &commandLine = QString(),
              const QStringList &sep = QStringList(),
              const QStringList &taglist = QStringList(),  
              const QList<QString> *terms = nullptr, 
              const QList<QString> *translation = nullptr, 
              QWidget *controlledWidget = nullptr);


  void  setWidgetFromXml(const FStringList & );
  const FString setXmlFromWidget();

  bool isAbstractEnabled() {return this->isEnabled();}

  QListWidget* currentListWidget;
  void setSeparator(QStringList sep) { separator=sep;}
  void setTabLabels(const QStringList& labels) { tabLabels = labels;}
  void clearTabLabels() { tabLabels.clear();}
  QStringList getTabLabels() { return tabLabels;}
  QStringList separator;

  void addGroup(const QString& label = "");

private:
  QStringList tags;

  FListFrame* parent;

  friend  QStringList applyHashToStringList(QHash<QString, QString> &H,  QStringList &M);

  QHash<QString, QString> listWidgetTranslationHash;
  const FString& translate(const FStringList &s);
  QStringList tabLabels = QStringList();

signals:
  void forceCloseProject();
};


class  FCheckBox : public QCheckBox,  public FAbstractWidget
{
  Q_OBJECT

  friend class FAbstractWidget;

public:

  FCheckBox(const QString &boxLabel, int status, const QString &hashKey, const QStringList &description, const QString& commandLineString,
                       const QVector<QWidget*> &enabledObjects, const QVector<QWidget*> &disabledObjects);

  FCheckBox(const QString &boxLabel, int status, const QString &hashKey, const QStringList &description, const QString& commandLineString):
                FCheckBox(boxLabel, status, hashKey, description, commandLineString, {nullptr}, {nullptr}){}

  FCheckBox(const QString &boxLabel, int status, const QString &hashKey, const QStringList &description, const QString& commandLineString,
                       const QVector<QWidget*> &enabledObjects):
                FCheckBox(boxLabel, status, hashKey, description, commandLineString, enabledObjects, {nullptr}){}

  FCheckBox(const QString &boxLabel, int status, const QString &hashKey, const QStringList &description,
                       const QVector<QWidget*> &enabledObjects=QVector<QWidget*>(), const QVector<QWidget*> &disabledObjects=QVector<QWidget*>()):
                         FCheckBox(boxLabel,  status, hashKey, description, "",enabledObjects, disabledObjects) {}


  FCheckBox(const QString &boxLabel, const QString &hashKey, const QStringList& description, const QString& commandLineString,
            const QVector<QWidget*> &enabledObjects=QVector<QWidget*>(), const QVector<QWidget*> &disabledObjects=QVector<QWidget*>()):
    FCheckBox(boxLabel, flags::status::enabledUnchecked|flags::commandLineType::defaultCommandLine, hashKey, description, commandLineString,
                         enabledObjects, disabledObjects){}

  FCheckBox(const QString &boxLabel, const QString &hashKey, const QStringList& description,
            const QVector<QWidget*> &enabledObjects=QVector<QWidget*>()):
      FCheckBox(boxLabel, flags::status::enabledUnchecked|flags::commandLineType::defaultCommandLine, hashKey, description, "",
                enabledObjects, {nullptr}){}

  void setWidgetFromXml(const FStringList& );
  bool refresh();
  const FString setXmlFromWidget();
  bool isAbstractEnabled() {return this->QCheckBox::isEnabled();}

private slots:
  void uncheckDisabledBox();

};


class FComboBox : public QComboBox, public FAbstractWidget
{
  Q_OBJECT

  friend class FAbstractWidget;

public:

  FComboBox(const QStringList &labelList, const QStringList &translation, int status, const QString &hashKey, const QStringList &description, const QString &commandLine, QList<QIcon> *iconList);
  FComboBox(const QStringList &labelList, int status, const QString &hashKey, const QStringList &description, const QString &commandLine,  QList<QIcon> *iconList=nullptr):
    FComboBox(labelList, QStringList(), status, hashKey, description, commandLine, iconList){}
  FComboBox(const QStringList &labelList, const QString &hashKey, const QStringList &description, const QString &commandLine,  QList<QIcon> *iconList=nullptr):
      FComboBox(labelList, flags::status::defaultStatus|flags::commandLineType::defaultCommandLine, hashKey, description, commandLine,  iconList){}


  FComboBox(const char* str, int status, const QString &hashKey, const QStringList &description, const QString &commandLine,  QList<QIcon> *iconList=nullptr):
    FComboBox(QStringList(str),  status, hashKey, description, commandLine,  iconList){}

  FComboBox(const QStringList &labelList, const QString &hashKey, const QStringList &description):
      FComboBox(labelList, flags::status::defaultStatus|flags::commandLineType::noCommandLine, hashKey, description, "",  nullptr){}

  void setWidgetFromXml(const FStringList&);
  bool refresh();
  const FString setXmlFromWidget();
  bool isAbstractEnabled() {return this->isEnabled();}

  QHash<QString, QString> comboBoxTranslationHash;

private slots:
  void fromCurrentIndex(const QString&);


};

class FLineEdit : public QLineEdit, public FAbstractWidget
{
  Q_OBJECT
  friend class FAbstractWidget;

public:
  FLineEdit(const QString &defaultstring, int status, const QString &hashKey, const QStringList &description, const QString &commandLine);
  FLineEdit(const QString &defaultstring, flags::status status, const QString &hashKey, const QStringList &description, const QString &commandLine) :
     FLineEdit(defaultstring, static_cast<int>(status), hashKey, description, commandLine) {}    
  
  FLineEdit(const QString &defaultstring, const QString &hashKey, const QStringList &description, const QString &commandLine):
     FLineEdit(defaultstring, flags::status::defaultStatus|flags::commandLineType::defaultCommandLine, hashKey, description, commandLine){}

  FLineEdit(const QString &defaultstring, const QString &hashKey, const QStringList &description):
     FLineEdit(defaultstring, flags::status::defaultStatus|flags::commandLineType::noCommandLine, hashKey, description, ""){}
  FLineEdit() {}
  void setWidgetFromXml(const FStringList&);
  bool refresh();
  const FString setXmlFromWidget();
  bool isAbstractEnabled() {return this->isEnabled();}

  FLineEdit(const FLineEdit* f) :  FLineEdit(f->commandLineList[0].toQString(), f->status, f->hashKey, f->description, f->optionLabel) {}
  
private:

};


class FProgressBar : public QWidget
{
    Q_OBJECT

    typedef void (FDialog::*SlotFunction)();

public:
    FProgressBar(FDialog* parent,
                 SlotFunction  killFunction = nullptr);

    QHBoxLayout* getLayout() {return layout;}
    void setToolTip(const QString & tip) { bar->setToolTip(tip); }

    int  value() { return bar -> value();}
    int  maximum() { return bar -> maximum();}

private:

    QHBoxLayout* layout=new QHBoxLayout;
    QToolButton* killButton=new QToolButton;
    QTimer *timer= new QTimer(this);
    QProgressBar *bar=new QProgressBar ;
    int startshift = 5;
    void computeRProgressBar();
    void computeLHXParsingProgressBar();
    void computeLHXWritingProgressBar(bool = false);

    enum class  State {Parsing, WritingReady, WritingStarted};
    State internalState;

    FDialog* parent;

    void setInterval(int i) { timer->setInterval(i);}
    void setRange(int x, int y) { bar->setRange(x, y); }

    void setCount(int x) { bar->setRange(0, x); }

    void start(int timeout = PROGRESSBAR_TIMEOUT)
    {
        timer->start(timeout);
        killButton->setEnabled(true);
    }

    void hide()
    {
        stop();
        bar->hide();
        killButton->hide();
        bar->reset();
    }

  private slots:
    void setValue(int x, int y) { bar->setValue(x); bar->setMaximum(y);}
    void setValue(int x) { bar->setValue(x); }

  public slots:
    void stop();
    void showProgressBar()
    {
        start();
        bar->reset();
        killButton->show();
        bar->show();
        internalState = State::Parsing;
    }

};



#endif
