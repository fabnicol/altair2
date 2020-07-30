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
//

#ifndef FLISTFRAME_H
#define FLISTFRAME_H

#include "fwidgets.h"
#include "fstring.h"
#include "tools.h"
#include <QToolButton>
#include <QFileSystemModel>
#include <QMessageBox>
#include <QMutex>

class FListWidget;
class QToolDirButton;
class Worker;

class FListFrame : public FDialogConnections
{
Q_OBJECT

public:

// Membres données


 std::vector<QThread*> T;
 QVector<QListWidget*> widgetContainer;  ///< Conteneur des widgets listes composant les onglets.
 FListWidget *fileListWidget;            ///< composant fonctionnelassocié à QWidget représentant l'onglet courant.
 QString frameHashKey;                   ///< Balise XML correspondant à la classe.
 std::vector<QThread*> thread;           ///< Vecteur de fils d'exécution permettant de lancer parseXhlFile sur chaque fichier d'onglet.
 int size = 0;                           ///< Nombre total de fichiers dans  FListFrame::widgetContainer.
 QToolButton *importFromMainTree = new QToolButton; ///< Bouton permettant d'importer des fichiers d'une arborescence de fichiers FListFrame::fileTreeView.
 QStringList tabLabels;                  ///< Liste des titres des onglets.
# ifndef USE_RIGHT_CLICK

     QToolButton  *retrieveItemButton=new QToolButton,
                  *deleteGroupButton=new QToolButton,
                  *nppDisplayButton =  new QToolButton;
# endif

 QTabWidget *mainTabWidget;                      ///< Onglet central matrice.
 QAbstractItemView *fileTreeView;                ///< Onglet central matrice.
 QFileSystemModel *model = new QFileSystemModel; ///< Modèle de fichiers sous-jacent à  FListFrame::fileTreeView.
 QGroupBox *controlButtonBox = new QGroupBox;    ///< Boîte permettant de regrouper divers boutons de contrôle (haut/bas etc.).
 bool use_threads = false;                       ///< Par défaut, les fils d'exécution ne seront pas utilisés. Seront activés en cas d'input disque optique.

 // Méthodes
 /// Efface  widgetContainer

 void clearWidgetContainer();

 /// Lancer un fil d'exécution pour lire l'entête d'un fichier XHL (Année, Mois,...) et classer les fichiers par onglet automatiquement
 /// \param rank Rang du fichier dans la liste des fichiers de l'onglet central
 /// \note Cette fonction appelle  parseXhlFile
 /// \todo Cette fonction pourrait être optimisée en ne lançant pas les fils d'exécution de manière successive mais par par groupe avec plusieurs fils parallèles dans chaque groupe

 /// Récupère les titres d'onglet.
 /// \return Liste de format \e QStringList des titres d'onglet.

 QStringList getTabLabels(){
                                QStringList labels;
                                int r = getRank();
                                for (int i=0; i <= r; ++i)
                                     labels << mainTabWidget->tabText(i);
                                return labels;
                            }

 /// Récupère la taille courante (0-based) du conteneur  FListFrame::widgetContainer.
 /// Cette taille doit en principe être alignée avec le nombre d'onglets en présence.
 /// \return Taille du conteneur moins 1.

 int getRank() {return widgetContainer.size()-1;}

 /// Accesseur en lecture de  FListFrame::frameHashKey, balise XML du composant fonctionnel .
 /// \return Valeur de  frameHashKey?

 const QString &getHashKey() const {return frameHashKey;}

 /// Accesseur en lecture de  FListFrame::widgetContainer.
 /// \return Valeur de  FListFrame::widgetContainer.

 QVector<QListWidget*>  getWidgetContainer() {return widgetContainer;}

 /// Accesseur en lecture du nombre d'éléments (nombre d'onglets) du vecteur  widgetContainer.
 /// \return Valeur de la méthode \e size() pour  widgetContainer.

 int getWidgetContainerCount() {return widgetContainer.size();}

 /// Accesseur en lecture du nombre d'items (fichiers) dans l'élément (onglet) de rang donné de  widgetContainer.
 /// \param g Rang (0-based) de l'élément (onglet) du vecteur  widgetContainer.
 /// \return Valeur de la méthode count() pour  widgetContainer[g].

 int getWidgetContainerCount(int g) {return widgetContainer.at(g)->count();}

 /// Accesseur en lecture d'un élément de rang donné du vecteur widgetContainer.
 /// \param rank Rang (0-based) de l'élément du vecteur.
 /// \return Si rank est inférieur à la taille du vecteur, valeur de l'élément de rang rank du vecteur widgetContainer, sinon \e nullptr.

 QListWidget*  getWidgetContainer(int rank) {if (rank < widgetContainer.count()) return widgetContainer[rank]; else return nullptr;}

 /// Accesseur en lecture du rang (index) de l'onglet courant (0-based).
 /// \return Valeur de la méthode \e currentIndex() pour  mainTabWidget

 int getCurrentIndex() { return this->mainTabWidget->currentIndex(); }

 /// Accesseur en lecture du composant (\em widget) courant dans le conteneur widgetContainer. 
 /// \return Composant courant.

 QListWidget*  getCurrentWidget() { return widgetContainer.at(getCurrentIndex());}

 /// Accesseur en lecture du titre de l'onglet courant.
 /// \return Valeur du titre.

 QString  getCurrentLabel() { return this->mainTabWidget->tabText(getCurrentIndex());}

 /// Accesseur en lecture du titre de l'onglet d'index donné.
 /// \return Valeur du titre.

 QString  getLabel(int index) { return this->mainTabWidget->tabText(index);}

 /// Accesseur en lecture de controlButtonBox.
 /// \return Valeur de controlButtonBox.

 QGroupBox* getControlButtonBox() { return controlButtonBox;}

 /// Accesseur en lecture de la ligne courante
 /// \return Valeur de l'index (\em 0-based) de la ligne courante.
 int getCurrentRow() { return getCurrentWidget()->currentRow(); }

 /// Assigne le séparateur qui permet de passer d'une liste d'items (chemins de fichiers) à une ligne de commande.
 /// \param sep Séparateur (blanc en l'occurrence).

 void setSeparator(QStringList sep) { fileListWidget->separator[0]=sep[0]; fileListWidget->separator[1]=sep[1];}
 
 /// Assigne les titres des onglets de l'onglet central
 /// \param tabLabels Liste de chaînes de caractères.

 void setTabLabels(QStringList& tabLabels) { fileListWidget->setTabLabels(tabLabels);}


 /// Constructeur de la classe.
 /// La classe comprend une série d'onglets comprenant des lignes de texte chacun.\n
 /// Elle gouverne aussi l'importation des fichiers entrants et le déroulement de la barre de progression.\n
 /// Elle réalise un premier tri des fichiers de paye en les classant dans l'ordre année-mois, en identifiant les Siret, Etablissements et Budgets.\n
 /// Elle correspond à la balise XHL des fichiers de projet et aux informations sur fichiers de paye dans le gestionnaire de projets.
 /// \param tree  Identifie FListFrame::fileTreeView
 /// \param import_type Identifie FListFrame::importType
 /// \param hashKey Identifie FListFrame::frameHashKey
 /// \param description Description à afficher dans le gestionnaire de projets (paramètre aussi de FListFrame::fileListWidget).
 /// \param command_line Paramètre d eligne de commande de FListFrame::fileListWidget.
 /// \param cli_type Paramètre transmis à FListFrame::fileListWidget permetant de contrôler que la ligne de commande est bien associée à tel ou tel exécutable.
 /// \param separator Liste de deux séparateurs permettant de dérouler en ligne de commande la hiérarchie sur deux niveaux d'un onglet (niveau 1 : onglets; niveau 2 : éléments de chaque onglet).
 /// \param xml_tags Idem pour les balises correspondant aux onglets et aux items.
 /// \param tabLabels Titres des onglets.
 /// \param mainTabWidgetRank Dans cette application, fixé à tools::TabWidgetTrait::NO_EMBEDDING_TAB_WIDGET.
 /// \param terms Transmis à FlistFrame::fileListWidget.
 /// \param translation  Transmis à FlistFrame::fileListWidget.

FListFrame(QAbstractItemView* tree,
           short import_type,
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

/// Initialise le conteneur de composants widgetContainer à blanc.

void initializeWidgetContainer() {   widgetContainer =  QVector<QListWidget*>() << new QListWidget; }

/// Initialise le conteneur de composants widgetContainer à partir d'un composant de liste (pointeur vers QListWidget)
/// \param listWidget Initialise widgetContainer.
void initializeWidgetContainer(QListWidget* listWidget) {   widgetContainer =  QVector<QListWidget*>() << listWidget; }

/// Ajoute les fichiers d'une portion d'arborescence de fichiers au composant courant de l'onglet central.
/// \param strL liste des fichiers sélectionnés dans l'arborescence.

void addParsedTreeToListWidget(const QStringList &strL);


protected:

    short importType;
    QStringList tags;

private:


 QProcess* launch;
 QStringList stringList;
 QListWidget *currentListWidget;

 void clearTabLabels() {fileListWidget->clearTabLabels();}

 int row, currentIndex;
 bool activateOnStart=false;
 bool isTerminated = false;

 /// Actualise  currentWidget,  row et  currentIndex

 void updateIndexInfo();

 /// Efface tous les onglets et, selon la valeur des paramètres, insère un onglet vierge (ou pas) et efface  Hash::wrapper (ou pas)
 /// \param insertFirstGroup  Si true, insère un onglet vierge après l'effacement des onglets
 /// \param eraseAllData Si true, efface toute la table de hachage  Hash::wrapper

 void deleteAllGroups(bool insertFirstGroup = true, bool eraseAllData = true);

 void launch_thread(unsigned long rank);



 /// Appelle  parseXhlFile(const QString&) sur l'ensemble de  widgetContainer

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
    void showContextMenu();
    void handleResults()
    {
        static int count;
        ++count;
        emit(setProgressBar(count));
        if (count == size)
        {
            count = 0;
            emit(imported());

            for (QThread* t : T) {

                connect(t, &QThread::finished, t, &QObject::deleteLater);
            }
        }
    }

protected slots:
    void on_importFromMainTree_clicked();
    void on_file_display(const QString& file);
    void finalise();
};

class Worker : public QObject
{
    Q_OBJECT

private :

    int rank;
    QString filename;

    /// Décode les champs principaux du fichier XHL: Année, Mois, Budget, ...
    /// \param fileName Chemin du fichier décodé.

    void parseXhlFile(const QString& fileName);

public:

    Worker(const int r, const QString &s): rank {r}, filename {s} {}

    void doWork() {

        parseXhlFile(filename);
        emit resultReady(rank);
    }

signals:
    void resultReady(const int);
};

#endif // FLISTFRAME_H
