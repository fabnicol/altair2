/// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser
// les fichiers de paye produits au format spécifié par l'annexe de la
// convention-cadre nationale de dématérialisation en vigueur à compter de
// l'année 2008.
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
////////////////////////////////////////////////////////////////////////////

#ifndef ALTAIR_H
#define ALTAIR_H

/// \file altair.h
/// \author Fabrice Nicol
/// \brief Code de la classe principale Altair, qui joue le rôle d'agent de l'interface graphique

#include "fstring.h"
#include "flistframe.h"
#include "fwidgets.h"
#include "common.h"
#include "enums.h"
#include "altair-gui.h"

class FProgressBar;
class MainWindow;
class FListFrame;

class Altair : public FDialog, public common
{
    Q_OBJECT

public:

  // Membres données

    Altair();

    MainWindow *parent; ///< Interface graphique dont cette classe est l'agent.

    enum { MaxRecentFiles = 5 ///< Maximum de la liste des fichiers récents
    };

    FListFrame *project; ///< Occurrence de la classe englobant les contenus de l'onglet central

    QTextEdit *outputTextEdit = new QTextEdit; ///< onglet des messages en bas de l'interface
    QFileSystemModel *model = nullptr; ///< Modèle Qt de l'arborescence de fichiers de l'exporateur de fichiers à gauche de l'interface
    QTreeView *fileTreeView = nullptr; ///< Vue associée
    QTreeWidget *managerWidget= new QTreeWidget; ///< Arbre du gestionnaire de projet à droite de l'interface



    QFile rankFile; ///< Fichier permettant de lire l'indice de progression de l'application noyau lhx et d'ajuster la barre de progression de manière dynamique
    QString projectName; ///< Nom du projet .alt
    QString rootDir = ""; ///< Fichier racine des données
    QString username; ///< NOm de l'utilisateur Unix

    const QStringList  XML_FILTERS = {"*.xml", "*.xhl", "*.XML", "*.XHL"}; ///< Liste des extensions valides pour les fichiers entrants

    static std::uint16_t RefreshFlag; ///< Drapeau permettant d'indiquer l'état de rafraichissement de l'interface.


  // Méthodes

    /// Efface les onglets, le gestionnaire de projet et lit le projet .alt en cours
    bool clearInterfaceAndParseProject();
    
    /// Remplace un nom de projet vide par un nom par défaut
    void checkEmptyProjectName()
      {
         if (projectName.isEmpty())
            projectName = userdatadir + QString("défaut.alt");
      }

    /// Début du glisser-déplacer
    void startDrag();

    /// Fonction réimplémentée : événement du déplacement de la souris
    void dragMoveEvent(QDragMoveEvent *event);

    /// Fonction réimplémentée : début du déplacement de la souris
    void dragEnterEvent(QDragEnterEvent *event);

    /// Fonction réimplémentée : fin du déplacement de la souris
    void dropEvent(QDropEvent *event);

    /// Assigner le fichier courant
    void setCurrentFile(const QString &fileName);

    /// Renvoie la taille des données exportées
    qint64 size() { return Altair::totalSize[0]; }

    /// Rafraîchit le gestionnaire de projet à la droite de l'interface.
    /// \param \ref manager::refreshAllZones ou bien \ref manager::refreshXHLZone, \ref manager::refreshNBulletins, \ref manager::refreshSystemZone, selon la zone à rafraichir.
    void refreshProjectManagerValues(std::uint16_t = manager::refreshAllZones );

    /// Rafraîchit le gestionnaire de projet à la droite de l'interface. Fonction globale par défaut.
    bool refreshProjectManager();

    /// Renvoie le widget fonctionnel associé à la barre de progression
    FProgressBar* getProgressBar() { return progress; }

    /// Accesseur en lecture de \ref fileCount
    inline int getFileCount() {return fileCount;}

    /// Lit le fichier \ref rankFile pour l'index de la barre progression, le lit dans \ref fileRank et ajuste la barre de progression
    inline void __attribute__((always_inline)) readRankSignal()
    {
            if (! rankFile.exists()) return;
            int baInt = 0;

            if (rankFile.open(QFile::ReadOnly))
            {
                    baInt = rankFile.readLine().toInt();
                    rankFile.close();
            }
            if (baInt == -1) emit(setProgressBar(0, fileCount));
            fileRank = (baInt >= 1)? baInt : 1;

    }

    /// Crée une ligne de commande
    QStringList createCommandLineString(const QStringList &L = QStringList());

    /// Ecrit le fichier de projet .alt
    void writeProjectFile();

    /// Rafraichit la vue d'arbre \ref fileTreeView de l'exporateur de fichiers à gauche de l'interface
    /// \param create (= false) Si \e true, alloue la vue.
    void refreshTreeView(bool create=false);

    
public slots:

   bool updateProject(bool=false);
   void on_openProjectButton_clicked();
   void on_newProjectButton_clicked();
   void on_deleteItem_clicked();
   void closeProject();
   void openProjectFileCommonCode();
   void importData();
   void textAppend(const QString& s) { outputTextEdit->append(s);}
   void killProcess();

private slots:
    
    void run();
    void runRAltair();
    void processFinished(int code) { processFinished(static_cast<exitCode>(code));}
    void processFinished(exitCode);
    void deleteGroup();
    void on_helpButton_clicked();
    void requestSaveProject();
    void openProjectFile();
    void refreshRowPresentation(int);

protected:

   QString      sourceDir;
   unsigned int maxRange=0;

signals:

 void hasIndexChangedSignal();

private:

    bool hasIndexChanged;
    int myTimerId = 0;
    int fileCount = 0; ///< Nombre de fichiers de paye en input de la ligne de commande
    int row = 0;
    uint currentIndex = 0;
    qint64 value = 0;
    static qint64 totalSize[2];

    QHash <int,  QVector<QStringList>  > fileSizeDataBase;

    QIcon iconShowMaximized, iconShowNormal;

    FProgressBar *progress;
    QVBoxLayout *mainLayout= new QVBoxLayout;
    QVBoxLayout *progressLayout= new QVBoxLayout;
    QVBoxLayout *managerLayout= new QVBoxLayout;
    QHBoxLayout *allLayout= new QHBoxLayout;
    void assignWidgetValues();
    void clearProjectData();

    void initialize();
    const QString  makeParserString(int start, int end=Abstract::abstractWidgetList.size()-1);
    const QString  makeDataString( );
    const QString  makeSystemString( );
    QString processMsg;
    
    QVector<QStringList> processSecondLevelData(QVector<QStringList> &L, bool isFile=true);
    QVector<QUrl> parseUrlsDragged(QList<QUrl>& urlsDragged);
    FStringList parseEntry(const QDomNode &, QTreeWidgetItem *parent=0);
    void refreshRowPresentation();

    void setIndexedProperties(QModelIndexList* indexList);
    void setDialogFromProject();
    void updateIndexInfo();
    void updateIndexChangeInfo();
    void displayTotalSize();
    void refreshModel();

    void parseProjectFile(QIODevice*);
    void msg (const QString & text);
    void printMsg(qint64 new_value, const QString &str);
    void printBaseSize(qint64 new_value = 0);

    void checkAnnumSpan();

    bool runWorkerDistributed(bool);
    void runWorker(const QString& = "");


};



#endif
