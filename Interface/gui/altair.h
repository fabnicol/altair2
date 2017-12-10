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

    /// Constructeur de la classe Altair, agent de la classe MainWindow qui implémente les fonctions globales de l'interface graphique
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
    qint64 size() { return Altair::totalSize; }

    /// Rafraîchit le gestionnaire de projet à la droite de l'interface.
    /// \param \ref manager::refreshAllZones ou bien \ref manager::refreshXHLZone,\n
    /// \ref manager::refreshNBulletins, \ref manager::refreshSystemZone, selon la zone à rafraichir.
    void refreshProjectManagerValues(std::uint16_t = manager::refreshAllZones );

    /// Rafraîchit le gestionnaire de projet à la droite de l'interface. Fonction globale \n par défaut.
    bool refreshProjectManager();

    /// Renvoie le widget fonctionnel associé à la barre de progression
    FProgressBar* getProgressBar() { return progress; }

    /// Accesseur en lecture de \ref fileCount
    inline int getFileCount() {return fileCount;}

    /// Lit le fichier \ref rankFile pour l'index de la barre progression, le lit dans \n
    /// \ref fileRank \n et ajuste la barre de progression
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

    /// Ecrit le projet XML d'extension .alt contenant les références des donnéees de paye
    /// \note Ecrit un en-tête classique puis lance \ref makeDataString() et \ref makeSystemString()
    void writeProjectFile();

    /// Rafraichit la vue d'arbre \ref fileTreeView de l'exporateur de fichiers à gauche de \n l'interface
    /// \param create (= false) Si \e true, alloue la vue.
    void refreshTreeView(bool create=false);

    
public slots:

   /// Rafraichir le projet .alt après avoir actualisé l'interface
   /// \param enreg Si \e true, enregistre le projet actualisé.
   bool updateProject(bool enreg = false);

   /// Ouvre le projet, le décode en appelant \ref parseProjectFile et actualise l'interface\n
   /// en conséquence
   void on_openProjectButton_clicked();

   /// Crée un nouveau projet défaut.alt
   void on_newProjectButton_clicked();

   /// Ecrase un item d'un onglet donné
   void on_deleteItem_clicked();

   /// Ferme le projet et réinitialise l'interface (gestionnaire de projet inclus)
   void closeProject();

   /// Code commun à différentes fonctions lancées pour l'analyse des projets
   void openProjectFileCommonCode();

   /// Importer les données du répertoire \ref userdatadir ou du disque optique s'il est monté,\n
   /// au lancement de l'interface, si l'option est cochée.
   void importData();

   /// Ajouter du texte à l'onglet des messages
   void textAppend(const QString& s) { outputTextEdit->append(s);}

   /// Tuer l'application en ligne de commande \e lhx
   void killProcess();

private slots:

   /// Lance l'exécution de l'application en ligne de commande lhx
   void run();

   /// Lance l'application Rstudio et les scripts R
   void runRAltair();

   /// Traitement de la fin de l'exécution de lhx
   void processFinished(int code) { processFinished(static_cast<exitCode>(code));}

   /// Traitement de la fin de l'exécution de lhx
   void processFinished(exitCode);

   /// Supprimer un onglet central
   void deleteGroup();

   /// Lancer l'aide (menu ou barre)
   void on_helpButton_clicked();

   /// Enregistre le projet \e .alt en actualisant \ref projectName et l'état de l'interface \n
   void requestSaveProject();

   /// Ouvre le projet \e .alt et décode le projet pour actualiser l'interface
   void openProjectFile();

   /// Raccourcit les chemins dans les onglets en se limitant au non de fichier et les présente \n
   /// en surlignage bleu alterné de gris argent
   void refreshRowPresentation(int);

protected:

   QString      sourceDir;
   unsigned int maxRange=0;

signals:

 void hasIndexChangedSignal();

private:

 // Données privées

    bool hasIndexChanged;
    int myTimerId = 0;
    int fileCount = 0; ///< Nombre de fichiers de paye en input de la ligne de commande
    int row = 0;
    uint currentIndex = 0;
    qint64 value = 0;
    static qint64 totalSize; ///< Taille des données extraites

    QHash <int,  QVector<QStringList>  > fileSizeDataBase; ///< Table de hashage contenant les tailles de fichier

    QIcon iconShowMaximized, iconShowNormal;

    FProgressBar *progress; ///< Barre de progression

    QVBoxLayout *mainLayout= new QVBoxLayout; ///< Layout de l'interface
    QVBoxLayout *progressLayout= new QVBoxLayout; ///< Layout de la barre de progression
    QVBoxLayout *managerLayout= new QVBoxLayout; ///< Layout du gestionnaire de projets
    QHBoxLayout *allLayout= new QHBoxLayout;  ///< Layout global


    /// Parcout l'ensemble des widgets fonctionnels \ref Abstract::abstractWidgetList
    /// et actualise leur statut interne en fonction de l'état du projet .alt
    /// en appelant \e setWidgetFromXml
    void assignWidgetValues();

    /// Efface toutes les données du projet .alt et réinitialise l'interface et les conteneurs
    /// de données internes
    void clearProjectData();

    /// Inititalise les variables utilisateur et de localisation (\ref userdatadir)
    void initialize();

    /// Crée la chaîne QString permettant d'écrire un projet XML d'extension .alt
    /// \param start rang de départ pour l'écriture du projet XML
    /// \param end rang de fin pour l'écriture du projet XML
    /// \note Utilise une des fonctionnalités des FWidget (setXmlFromWidget) qui permet
    /// de transposer l'état du widget en valeurs
    const QString  makeParserString(int start, int end=Abstract::abstractWidgetList.size()-1);

    /// Lance \ref Altair::makeParserString sur le premier item courant de abstractWidgetList (celui des données de paye)
    const QString  makeDataString( );

    /// Lance \ref Altair::makeParserString sur l'ensemble de la liste des FWidgets sauf le premier (tous les widgets sauif les données de paye)
    const QString  makeSystemString( );

    QString processMsg;
    
    QVector<QStringList> processSecondLevelData(QVector<QStringList> &L, bool isFile=true);
    QVector<QUrl> parseUrlsDragged(QList<QUrl>& urlsDragged);

    FStringList parseEntry(const QDomNode &, QTreeWidgetItem *parent=0);
    void refreshRowPresentation();

    void setIndexedProperties(QModelIndexList* indexList);
    void setDialogFromProject();
    void updateIndexInfo();
    void displayTotalSize();

    /// Détruit et reconstruit de modèle de l'arborescence des fichiers
    void refreshModel();

    /// Décode le projet représenté par un pointeur vers QFile ou similaire
    /// \param file Pointeur ver un fichier contenant le projet.

    void parseProjectFile(QIODevice* file);

    void printMsg(qint64 new_value, const QString &str);

    void printBaseSize(qint64 new_value = 0);

    /// Vérifie que les onglets de contiennent pas des années incomplètes
    /// et signale les mois manquants
    void checkAnnumSpan();

    bool runWorkerDistributed(bool);
    void runWorker(const QString& = "");
};



#endif
