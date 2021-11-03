// Copyright Cour des comptes, 2017
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
// //////////////////////////////////////////////////////////////////////////


/// \file altair.h
/// \author Fabrice Nicol
/// \brief Code de la classe principale Altair, qui joue le rôle d'agent de l'interface graphique

#ifndef ALTAIR_H
#define ALTAIR_H

#include "fstring.h"
#include "flistframe.h"
#include "fwidgets.h"
#include "common.h"
#include "enums.h"
#include "altair-gui.h"

class FProgressBar;
class MainWindow;
class FListFrame;

/// Macro tendant à forcer l'inlining sous GCC

#define GCC_INLINE __attribute__((always_inline))

/// Classe Altair, agent de la classe MainWindow qui implémente les fonctions globales de l'interface graphique

class Altair : public FDialog, public common
{
    Q_OBJECT

public:

    // Membres données

    /// Constructeur

    Altair();

    MainWindow *parent; ///< Interface graphique dont cette classe est l'agent.

    FListFrame *project; ///< Occurrence de la classe englobant les contenus de l'onglet central

    QTextEdit *outputTextEdit = new QTextEdit; ///< onglet des messages en bas de l'interface
    QFileSystemModel *model = nullptr; ///< Modèle Qt de l'arborescence de fichiers de l'exporateur de fichiers à gauche de l'interface
    QTreeView *fileTreeView = nullptr; ///< Vue associée
    QTreeWidget *managerWidget = new QTreeWidget; ///< Arbre du gestionnaire de projet à droite de l'interface

    QFile rankFile; ///< Fichier permettant de lire l'indice de progression de l'application noyau lhx et d'ajuster la barre de progression de manière dynamique
    QString projectName; ///< Nom du projet \b .alt
    QString rootDir = ""; ///< Fichier racine des données
    QString username; ///< Nom de l'utilisateur Unix
    QStringList  XML_FILTERS; ///< Liste des extensions valides pour les fichiers entrants

    static std::uint16_t RefreshFlag; ///< Drapeau permettant d'indiquer l'état de rafraichissement de l'interface.

    // Méthodes

    /// Efface les onglets, le gestionnaire de projet et lit le projet \b .alt en cours

    bool clearInterfaceAndParseProject();

    /// Remplace un nom de projet vide par un nom par défaut

    void checkEmptyProjectName()
    {
        if (projectName.isEmpty())
            projectName = userdatadir + QString ("défaut.alt");
    }

    /// Début du glisser-déplacer

    void startDrag();

    /// Fonction réimplémentée : événement du déplacement de la souris

    void dragMoveEvent (QDragMoveEvent *event);

    /// Fonction réimplémentée : début du déplacement de la souris

    void dragEnterEvent (QDragEnterEvent *event);

    /// Fonction réimplémentée : fin du déplacement de la souris

    void dropEvent (QDropEvent *event);

    /// Assigner le fichier courant

    void setCurrentFile (const QString &fileName);

    /// Renvoie la taille des données exportées

    qint64 size()
    {
        return Altair::totalSize;
    }

    /// Rafraîchit le gestionnaire de projet à la droite de l'interface.
    /// \param  statut manager::refreshAllZones ou bien  manager::refreshXHLZone,
    ///  manager::refreshNBulletins,  manager::refreshSystemZone, selon la zone à rafraichir.

    void refreshProjectManagerValues (std::uint16_t statut = manager::refreshAllZones);

    /// Rafraîchit le gestionnaire de projet à la droite de l'interface. Fonction globale par défaut.

    bool refreshProjectManager();

    /// Renvoie le composant fonctionnel associé à la barre de progression

    FProgressBar* getProgressBar()
    {
        return progress;
    }

    /// Accesseur en lecture de  fileCount

    int getFileCount()
    {
        return fileCount;
    }

    /// Lit le fichier  rankFile pour l'index de la barre progression, le lit dans \n
    ///  fileRank \n et ajuste la barre de progression

    void GCC_INLINE readRankSignal()
    {
        if (! rankFile.exists()) return;

        int baInt = 0;

        if (rankFile.open (QFile::ReadOnly))
            {
                baInt = rankFile.readLine().toInt();
                rankFile.close();
            }

        if (baInt == -1) emit (setProgressBar (0, fileCount));

        fileRank = (baInt >= 1) ? baInt : 1;

    }

    /// Génère la ligne de commande à partir des chemins des fichiers de paye
    /// \param files Liste de chaînes de caractères des chemins des fichiers de paye
    /// \return Ligne de commande en forme de liste (un élément par option ou argument)
    /// \sa   Abstract::abstractWidgetList, FAbstractWidget::getHashKey, FAbstractWidget::commandLineStringList, fileCount

    QStringList createCommandLineString (const QStringList &files = QStringList());

    /// Ecrit le projet XML d'extension \b .alt contenant les références des donnéees de paye
    /// \note Ecrit un en-tête classique puis lance  makeDataString() et  makeSystemString()

    void writeProjectFile();

    /// Rafraichit la vue d'arbre  fileTreeView de l'exporateur de fichiers à gauche de \n l'interface
    /// \param create (= false) Si \e true, alloue la vue.

    void refreshTreeView (bool create = false);

public slots:

    /// Rafraichir le projet \b .alt après avoir actualisé l'interface
    /// \param enreg Si \e true, enregistre le projet actualisé.

    bool updateProject (int enreg = 0);

    /// Ouvre le projet, le décode en appelant  parseProjectFile et actualise l'interface \n
    /// en conséquence

    void on_openProjectButton_clicked();

    /// Crée un nouveau projet défaut\b .alt

    void on_newProjectButton_clicked();

    /// Ecrase un item d'un onglet donné

    void on_deleteItem_clicked();

    /// Ferme le projet et réinitialise l'interface (gestionnaire de projet inclus)

    void closeProject();

    /// Code commun à différentes fonctions lancées pour l'analyse des projets

    void openProjectFileCommonCode();

    /// Importer les données du répertoire  userdatadir ou du disque optique s'il est monté, \n
    /// au lancement de l'interface, si l'option est cochée.

    void importData();

    /// Ajouter du texte à l'onglet des messages

    void textAppend (const QString& s)
    {
        outputTextEdit->append (s);
        outputTextEdit->repaint();
    }

    /// Tuer l'application en ligne de commande \e lhx

    void killProcess();

private slots:

    /// Lance l'exécution de l'application en ligne de commande lhx

    void run();

    /// Lance l'application Rstudio et les scripts R

    void runRAltair();

    /// Traitement de la fin de l'exécution de lhx

    void processFinished (int code)
    {
        processFinished (static_cast<exitCode> (code));
    }

    /// Traitement de la fin de l'exécution de lhx

    void processFinished (exitCode);

    /// Supprimer un onglet central

    void deleteGroup();

    /// Lancer l'aide (menu ou barre)

    void on_helpButton_clicked();

    /// Enregistre le projet \e \b .alt en actualisant  projectName et l'état de l'interface \n

    void requestSaveProject();

    /// Ouvre le projet \e \b .alt et décode le projet pour actualiser l'interface

    void openProjectFile();

    /// Raccourcit les chemins dans les onglets en se limitant au non de fichier et les présente \n
    /// en surlignage bleu alterné de gris argent
    /// \param index Index de l'onglet actualisé

    void refreshRowPresentation (int index);

private:

// Données privées

    bool hasIndexChanged;    ///< \e true si l'index de l'onglet courant a changé
    int fileCount = 0;       ///< Nombre de fichiers de paye en input de la ligne de commande
    int row = 0;             ///< Ligne courante de l'onglet sélectionné
    int currentIndex = 0;   ///< Index courant de l'onglet sélectionné
    static qint64 totalSize; ///< Taille des données extraites

    QHash <int,  QVector<QStringList>> fileSizeDataBase; ///< Table de hashage contenant les tailles de fichier

    QIcon iconShowMaximized, ///< Etat de l'icone de la barre d'outils d'édition qui indique que l'interface est en plein écran
          iconShowNormal;    ///< Etat de l'icone de la barre d'outils d'édition qui indique que l'interface est en format réduit

    FProgressBar *progress;  ///< Barre de progression

    QVBoxLayout *mainLayout = new QVBoxLayout;     ///< Layout de l'interface
    QVBoxLayout *progressLayout = new QVBoxLayout; ///< Layout de la barre de progression
    QVBoxLayout *managerLayout = new QVBoxLayout;  ///< Layout du gestionnaire de projets
    QHBoxLayout *allLayout = new QHBoxLayout;      ///< Layout global

    /// Parcourt l'ensemble des widgets fonctionnels  Abstract::abstractWidgetList
    /// et actualise leur statut interne en fonction de l'état du projet \b .alt
    /// en appelant \e setWidgetFromXml

    void assignWidgetValues();

    /// Efface toutes les données du projet \b .alt et réinitialise l'interface et les conteneurs
    /// de données internes.
    /// Si l'interface a des éléments de statut non encore sauvegardés, affiche une boîte de dialogue
    /// permettant soit de sauvegarder l'état de l'interface dans un fichier projet \b .alt, soit de réinitialiser
    /// l'interface, soit de quitter le choix.

    void clearProjectData();

    /// Inititalise les variables utilisateur et de localisation ( userdatadir)

    void initialize();

    /// Initialise la ligne de commande RAltair

    void initRAltairCommandStr();

    /// Crée la chaîne QString permettant d'écrire un projet XML d'extension \b .alt
    /// Parcourt l'ensemble de la liste Abstract::abstractWidgetList des fwidgets (widgets fonctionnels)
    /// Enregistre le projet XML à partir du contenu des fwidgets
    /// \param  start  Index de départ dans la liste (0-based, inclus)
    /// \param  end    Index de fin dans la liste (0-based, inclus)
    /// \return Retourner le QString du projet \b .alt
    /// \note \e profondeur permet d'ecrire des objets complexes en encodant le degré d'enchâssement (0 à 2)
    /// \note Utilise une des fonctionnalités des FWidget (setXmlFromWidget) qui permet
    /// de transposer l'état du composant en valeurs
    /// \sa FAbstractWidget, FAbstractWidget::setXmlFromWidget

    const QString  makeParserString (int start, int end = Abstract::abstractWidgetList.size() - 1);

    /// Lance  Altair::makeParserString sur le premier item courant de abstractWidgetList (celui des données de paye)

    const QString  makeDataString();

    /// Lance  Altair::makeParserString sur l'ensemble de la liste des FWidgets sauf le premier (tous les widgets sauif les données de paye)

    const QString  makeSystemString();

    /// Parcourt la table de chachage  Hash::wrapper dans laquelle sont stockés à la clé "XHL" les chemins
    /// des fichiers de paye des onglets, calcule leur taille, affiche leurs données dans le gestionnaire de projets
    /// \param L Structure de stockage QVector<QStringList> des fichiers de l'onglet central.
    /// \param isFile  Si true (défaut) force le calcul de la taille des fichiers.
    /// \return La table de hashage  fileSizeDateBase

    QVector<QStringList> processSecondLevelData (QVector<QStringList> &L, bool isFile = true);

    /// Lors du décodage du projet XML \e \b .alt, décoder le noed courant et renvoyer l'empilement des valeurs en FStringList
    /// \param noeud Noeud QDom courant
    /// \param parent item parent de l'arbre composant (aucun par défaut)
    /// \return FStringList des valeurs empilées seloon le degré d'enchâssement (profondeur) du noeud XML

    FStringList parseEntry (const QDomNode &noeud, QTreeWidgetItem *parent = 0);

    /// Raccourcit les chemins dans les onglets en se limitant au non de fichier et les présente \n
    /// en surlignage bleu alterné de gris argent
    /// \note Appelle  refreshRowPresentation(int index) pour le noeud courant

    void refreshRowPresentation();

    /// Raffraîchit la valeur de  row et  currentIndex en fonction de l'état de l'onglet

    void updateIndexInfo();

    /// Afficher la taille des bases de paye dans l'onglet Messages

    void displayTotalSize();

    /// Détruit et reconstruit de modèle de l'arborescence des fichiers

    void refreshModel();

    /// Décode le projet représenté par un pointeur vers QFile ou similaire
    /// \param file Pointeur ver un fichier contenant le projet.

    void parseProjectFile (QIODevice* file);

    /// Affiche la taille des bases du projet
    /// \param new_value Taille du projet
    /// \param str Texte du message

    void printMsg (qint64 new_value, const QString &str);

    /// Appelle  printMsg une première fois.
    /// \param new_value Taille de la base

    void printBaseSize (qint64 new_value = 0);

    /// Vérifie que les onglets de contiennent pas des années incomplètes
    /// et signale les mois manquants

    void checkAnnumSpan();

    /// Parcourt le répertoire en cours du mode distributif, un à chaque appel
    /// Exporte l'identification dans chaque répertoire.
    /// Lance runWorker à chaque changement de répertoire
    /// \param reset Si \e true, "rembobine" la liste Hash::fileList des répertoires
    /// \return \e true si il y a encore un répertoire à traiter ou \e false sinon.

    bool runWorkerDistributed (bool);

    /// Construction de la ligne de commande pour des bases de paye dans un répertoire donné
    /// \param subdir Répertoire dans lequel sont recherchés les fichiers de paye
    /// \note \e Algorithme :\n
    /// Récupérer la liset des fichiers de paye du répertoire distribué  subdir
    /// Le traitement du répertoire suivant sera assuré par une nouvelle itération.
    /// Par défaut la ligne de commande contient de obligatoirement :
    /// <ul><li>-m : calcul des maxima du nombre de bulletins de paye par mois et du nombre de lignes de paye par agent par bulletin</li>
    /// <li>-d , : séparateur décimal virgule</li>
    /// <li>-s ; : séparateur de champs point-virgule</li>
    /// <li>-rank sharedir + "/rank" : fichier exporté dans ~/.local/share/applications/Altair ( sharedir par défaut) <br> indiquant l'index de la barre de progression</li>
    /// <li><pre>--cdrom : si depuis un disque optique</pre></li>
    /// <li>-D  username /Dev/altair/#DONNEES_SORTIE /subdir : répertoire d'exportation des bases si  subdir est non vide </li>
    /// <li>&ndash;&ndash;esiret Sirets séparés par blancs : si des Sirets sont excus (barrés) dans l'onglet Siret</li>
    /// <li>&ndash;&ndash;ebudget budgets séparés par blancs : si des budgets sont excus (barrés) dans l'onglet Budget</li>
    /// <li>&ndash;&ndash;eemployeur employeurs séparés par blancs : si des employeurs sont excus (barrés) dans l'onglet Employeur</li>
    /// </ul>
    /// \sa  Altair::runWorkerDistributed, classe Hash

    void runWorker (const QString& = "");

signals:

    /// Ajout d'une ligne de saisie des matricules au dialogue \em Extraction des bulletins (classe MatriculeInput).

    void ajouterLigneMatricules();

    /// Exporter les valeurs qui ne passent pas en ligne de commande mais en paramétrage du script R prologue_init.R

    void substituer_valeurs_dans_script_R();

};



#endif // ALTAIR_H
