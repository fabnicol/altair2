
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
//

#ifndef FLISTFRAME_H
#define FLISTFRAME_H

#include "fwidgets.h"
#include "fstring.h"
#include "tools.h"
#include <QToolButton>
#include <QFileSystemModel>
#include <QMessageBox>

class FListWidget;
class QToolDirButton;

class FListFrame : public FDialogConnections
{
Q_OBJECT

public:

// Membres données

 QVector<QListWidget*> widgetContainer;  ///< Conteneur des widgets listes composant les onglets
 FListWidget *fileListWidget;            ///< fwidget associé à QWidget représentant l'onglet courant
 QString frameHashKey;                   ///< Balise XML correspondant à la classe
 std::vector<QThread*> thread;           ///< Vecteur de fils d'exécution permettant de lance \ref parseXhlFile sur chaque fichier d'onglet
 int size = 0;                           ///< Nombre totalde fichiers dans \ref widgetContainer
 QToolButton *importFromMainTree=new QToolButton; ///< Bouton permettant d'importer des fichiers d'une arborescence de fichiers \ref fileTreeView
 QStringList tabLabels;                  ///< Liste des titres des onglets
# ifndef USE_RIGHT_CLICK

     QToolButton  *retrieveItemButton=new QToolButton,
                  *deleteGroupButton=new QToolButton,
                  *nppDisplayButton =  new QToolButton;
# endif

 QTabWidget *mainTabWidget;                      ///< Onglet central matrice
 QAbstractItemView *fileTreeView;                ///< Arborescence de fichiers pour importation par \ref importFromMainTree
 QFileSystemModel *model = new QFileSystemModel; ///< Modèle de fichiers sous-jacent à \ref fileTreeView
 QGroupBox *controlButtonBox = new QGroupBox;    ///< Boîte permettant de regrouper divers boutons de contrôle (haut/bas etc.)
 bool use_threads = false;

 // Méthodes

 /// Récupère les titres d'onglet
 /// \return Liste de format \e QStringList des titres d'onglet

 QStringList getTabLabels(){
                                QStringList labels;
                                int r = getRank();
                                for (int i=0; i <= r; ++i)
                                     labels << mainTabWidget->tabText(i);
                                return labels;
                            }

 /// Récupère la taille courante (0-based) du conteneur \ref widgetContainer
 /// Cette taille doit en principe être alignée avec le nombre d'onglets en présence
 /// \return Taille du conteneur moins 1.

 int getRank() {return widgetContainer.size()-1;}

 /// Accesseur en lecture de \ref frameHashKey, balise XML du fwidget.
 /// \return Valeur de \ref frameHashKey

 const QString &getHashKey() const {return frameHashKey;}

/// Accesseur en lecture de \ref widgetContainer
/// \return Valeur de \ref

 QVector<QListWidget*>  getWidgetContainer() {return widgetContainer;}
int getWidgetContainerCount() {return widgetContainer.size();}

/// Accesseur en lecture de \ref
/// \return Valeur de \ref

int getWidgetContainerCount(int g) {return widgetContainer[g]->count();}

/// Accesseur en lecture de \ref
/// \return Valeur de \ref
QListWidget*  getWidgetContainer(int rank) {if (rank < widgetContainer.count()) return widgetContainer[rank]; else return nullptr;}

/// Accesseur en lecture de \ref
/// \return Valeur de \ref
int getCurrentIndex() { return this->mainTabWidget->currentIndex(); }

/// Accesseur en lecture de \ref
/// \return Valeur de \ref
QListWidget*  getCurrentWidget() { return widgetContainer.at(getCurrentIndex());}

/// Accesseur en lecture de \ref
/// \return Valeur de \ref
QString  getCurrentLabel() { return this->mainTabWidget->tabText(getCurrentIndex());}

/// Accesseur en lecture de \ref
/// \return Valeur de \ref
QString  getLabel(int index) { return this->mainTabWidget->tabText(index);}

/// Accesseur en lecture de \ref
/// \return Valeur de \ref
QGroupBox* getControlButtonBox() { return controlButtonBox;}


void setControlButtonBoxVisible(bool x) {controlButtonBox->setVisible(x);}

/// Accesseur en lecture de \ref
/// \return Valeur de \ref
int getCurrentRow() { return getCurrentWidget()->currentRow(); }

void setStatus(flags::status status) {fileListWidget->status=status;}
void setCommandLineType(flags::commandLineType cl) {fileListWidget->commandLineType=cl;}
void setSeparator(QStringList sep) { fileListWidget->separator[0]=sep[0]; fileListWidget->separator[1]=sep[1];}
void setOptionLabel(QString option) {fileListWidget->optionLabel=option;}
void setTabLabels(QStringList& tabLabels) { fileListWidget->setTabLabels(tabLabels);}

inline bool listConnected() { return isListConnected; }

/// Accesseur en lecture de \ref
/// \return Valeur de \ref
inline int  getSlotListSize() {  return (isListConnected == true || isTotalConnected == true)? slotListSize : -1; }

FListFrame(QAbstractItemView* tree, short import_type,
           const QString &hashKey,
           const QStringList &description,
           const QString &command_line,
           int cli_type,
           const QStringList &separator,
           const QStringList &xml_tags,
           const QStringList &tabLabels,
           tools::TabWidgetTrait mainTabWidgetRank,
           QStringList* terms = nullptr,
           QStringList* translation = nullptr);


void initializeWidgetContainer() {   widgetContainer =  QVector<QListWidget*>() << new QListWidget; }
void initializeWidgetContainer(QListWidget* listWidget) {   widgetContainer =  QVector<QListWidget*>() << listWidget; }

void addParsedTreeToListWidget(const QStringList &strL);


protected:
    short importType;
    QStringList tags;

private:


 QProcess* launch;
 QStringList stringList;
 QListWidget *currentListWidget;

 void clearTabLabels() {fileListWidget->clearTabLabels();}

 int row, currentIndex,  slotListSize;
 bool isListConnected=false;
 bool isTotalConnected=false;
 bool activateOnStart=false;
 bool isTerminated = false;

 void list_connect(FComboBox* w);
 void list_connect(FListFrame* w);
 void total_connect(FListFrame* w);

 /// Actualise \ref currentWidget, \ref row et \ref currentIndex

 void updateIndexInfo();

 /// Efface tous les onglets et, selon la valeur des paramètres, insère un onglet vierge (ou pas) et efface \ref Hash::wrapper (ou pas)
 /// \param insertFirstGroup  Si true, insère un onglet vierge après l'effacement des onglets
 /// \param eraseAllData Si true, efface toute la table de hachage \ref Hash::wrapper

 void deleteAllGroups(bool insertFirstGroup, bool eraseAllData);

 /// Efface \ref widgetContainer

 void clearWidgetContainer();

 /// Lancer un fil d'exécution pour lire l'entête d'un fichier XHL (Année, Mois,...) et classer les fichiers par onglet automatiquement
 /// \param rank Rang du fichier dans la liste des fichiers de l'onglet central
 /// \note Cette fonction appelle \ref parseXhlFile
 /// \todo Cette fonction pourrait être optimisée en ne lançant pas les fils d'exécution de manière successive mais par par groupe avec plusieurs fils parallèles dans chaque groupe

 void launch_thread(int rank);

 struct Header* elemPar;

 /// Décode les champs principaux du fichier XHL: Année, Mois, Budget, ...
 /// \param fileName Chemin du fichier décodé.

 void parseXhlFile(const QString& fileName);

 /// Appelle \ref parseXhlFile(const QString&) sur l'ensemble de \ref widgetContainer

 void parseXhlFile();


 void addStringListToListWidget();
 QStringList parseTreeForFilePaths(const QStringList& stringList);
 void setStrikeOutFileNames(flags::colors color);

signals:

    void terminated();
    void is_ntabs_changed(int);
    void is_ntracks_changed(int);
    void imported();
    void parsed();
    void textAppend(const QString&);
    void setProgressBar(int, int);
    void setProgressBar(int);
    void hideProgressBar();
    void showProgressBar();
    void refreshRowPresentation(int);
    void updateProject(bool);
    void appRepaint();

public slots:

    void deleteGroup();
    void deleteGroup(int r);
    void addNewTab();
    void addNewTab(int r, const QString& label = "");
    void on_deleteItem_clicked();
    void setSlotListSize(int s);
    void showContextMenu();

protected slots:
    void on_importFromMainTree_clicked();
    void on_file_display(const QString& file);
    void finalise();
};



#endif // FLISTFRAME_H
