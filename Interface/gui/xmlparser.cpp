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


/// \file xmlparser.cpp
/// \author Fabrice Nicol
/// \brief Implémentation de la classe  Altair agent de la classe  MainWindow qui construit l'interface graphique.\n
/// Cette partie de l'implémentation concerne uniquement l'enregistrement ou le décodage des projets XML d'extension \b .alt \n
/// ainsi que l'ajustement de l'état de l'interface graphique et des widgets en fonction du décodage des projets.

#include "altair.h"
#include "common.h"


/// \mainpage Sommaire
/// La documentation du projet comprend :
/// - \subpage   page1 "la documentation de l'algorithme d'analyse des noeuds Remuneration"
/// - \subpage   page_alt "la documentation des paramètres de l'interface graphique et des projets Altaïr d'extension .alt"
/// - \subpage   page_lhx "la documentation de la ligne de commande de l'application-noyau lhx"
/// - \ref todo "la liste des choses à faire"
/// - l'onglet <a href="namespaces.html"><b>Espaces de nommage</b></a>
/// - l'onglet <a href="annotated.html"><b>Classes</b></a>
/// - et l'onglet <a href="files.html"><b>Fichiers</b></a>
///\n\n
/// \note Dans toute la documentation, un petit triangle bleu en marge gauche indique un lien dynamique. \n
/// Les éléments suivants sont accessibles en cliquant sur ces liens : \n
/// <ul>
/// <li> graphe d'héritage de classes</li>
/// <li> graphe de collaboration de classes</li>
/// <li> graphe des appelants d'une fonction</li>
/// <li> graphe d'appel d'une fonction</li>
/// <li> liste des types, connecteurs, signaux, fonctions et attributs hérités par une classe.</li></ul> \n
/// Il est possible de naviguer dans les graphes en utilisant la fonctionnalité de navigation (+/- pour zoomer, flèches de direction) en haut à droite des graphes.

/// \page page_alt Documentation des paramètres de l'interface graphique et des projets Altaïr d'extension .alt
/// \tableofcontents
/// \section edition_alt Edition d'un projet .alt
/// Pour éditer un projet :
/// - soit utiliser un éditeur de texte et de suivre les indications \e supra ;
/// - soit utiliser l'éditeur intégré de projet .alt : <b>Ctrl + E</b> ou menu Editer &rarr; Editer le projet courant &rarr; faire les modifications &rarr; menu Fichier interne à l'éditeur &rarr; Enregistrer et Quitter ou autres actions de ce menu.
/// - soit modifier les paramètres dans l'interface elle-même puis enregistrer le projet (<b>Ctrl + S</b> ou menu Fichier &rarr; Enregistrer) ou utiliser la fonctionnalité d'enregistrement automatique de l'interface (menu Configurer &rarr; Configurer l'interface &rarr; Sauvegarder le projet .alt automatiquement) ;
///
/// \section lancement_alt Lancement automatique de l'interface à partir d'un projet .alt
/// Sous Windows comme sous Linux il est possible d'utiliser les fichiers de projet .alt pour lancer directement l'interface graphique, chargée des paramètres et données contenues dans le projet.\n
/// L'utilisateur retrouve ainsi exactement l'état de l'interface et du traitement des données qui était en cours lors d'une utilisation précédente.
/// A cette fin :
/// - vérifier que l'ensemble des dépendances de bibliothèques est dans le chemin d'exécution par défaut (sous Linux, LD_LIBRARY_PATH des fichiers .desktop est exporté en variable globale)
/// - sous Linux (KDE) : Clic droit sur un fichier .alt &rarr; Ouvrir avec &rarr; Autres &rarr; renseigner le chemin de l'interface Altaïr et cocher Mémoriser l'association...\n
/// Un clic sur un fichier .alt permet de charger automatiquement le projet en lançant l'interface graphique.
///
/// \section lancement_alt_manu Chargement manuel d'un projet .alt
/// Alternativement, après lancement de l'interface :
/// - menu Fichier &rarr; Ouvrir le projet .alt ou <b>Ctrl + O</b>
/// - si le projet est en fichiers récents (menu Fichier &rarr; fichiers numérotés par ordre d'apparition), sélectionner le fichier bascule l'interface sur ce projet récent
/// - il est aussi possible de désarchiver un projet archivé (menu Fichier &rarr; Désarchiver le projet)
/// - pour un nouveau projet : <b>Ctrl + N</b> ou menu Fichier &rarr; Nouveau projet.
///
/// \section cli_lhx Génération de la ligne de commande de l'application-noyau lhx
/// Par défaut la ligne de commande contient obligatoirement :
/// <ul><li>-m : calcul des maxima du nombre de bulletins de paye par mois et du nombre de lignes de paye par agent par bulletin</li>
/// <li>-d , : séparateur décimal virgule</li>
/// <li>-s ; : séparateur de champs point-virgule</li>
/// <li>-E : Générer l'échelon</li>
/// <li>-rank sharedir + "/rank" : fichier exporté dans ~/.local/share/applications/Altair ( sharedir par défaut) <br> indiquant l'index de la barre de progression</li>
/// <li>&ndash;&ndash;cdrom : si depuis un disque optique</li>
/// <li>-D  username /Dev/altair/#DONNEES_SORTIE /subdir : répertoire d'exportation des bases si  subdir est non vide </li>
/// <li>&ndash;&ndash;esiret Sirets séparés par blancs : si des Sirets sont excus (barrés) dans l'onglet Siret</li>
/// <li>&ndash;&ndash;ebudget budgets séparés par blancs : si des budgets sont excus (barrés) dans l'onglet Budget</li>
/// <li>&ndash;&ndash;eemployeur employeurs séparés par blancs : si des employeurs sont excus (barrés) dans l'onglet Employeur</li>
/// </ul>
/// Elle contient en outre des valeurs qui dépendent de l'état des composants (\em widgets) de l'interface graphique.\n
/// En fonction des valeurs d'un des composants identifiés dans le tableau suivant, un ou plusieurs éléments sont ajoutés à la ligne de commande de \b lhx.\n
/// En miroir, une valeur (ou une série de valeurs) est générée entre la paire de balises XML correspondantes dans le projet .alt.\n
/// Par exemple, si la case du dialogue de configuration <b> Limiter la verbosité </b> est cochée, le composant MainWindow::defaultQuietBox génère l'option de ligne de commande \b -q.\n
/// En parallèle, une paire de balises \em quiet est munie de la valeur positive \em oui dans le projet XML d'extension .alt :
/// \code
/// <quiet>
///  oui
/// </quiet>
/// \endcode
/// Si la même case est décochée, l'option \b -q n'est pas insérée en ligne de commande. La paire de balises du projet .alt est alors renseignée par \em non :
/// \code
/// <quiet>
///  non
/// </quiet>
/// \endcode
/// \n
/// \section sec_alt1 Balises XML des projets Altaïr
/// \note Lorsque qu'une notation {valeur1, valeur2,...} est rencontrée, comprendre qu'est choisie une valeur parmi valeur1, valeur2, etc.\n
/// La valeur est donnée soit littéralement (en extension) soit en intension (année -> 2011, 2012, etc.).
///
/// <table style="width:100%;">
/// <tr><th>Balise XML</th><th>Attributs</th><th>Valeur</th><th>Dialogue/Onglet</th><th>Génération de ligne de commande pour <i>lhx</i></th><th>Commentaires</th><th>Composant</th></tr>
/// <tr><td style="font-weight:bold;">projet</td><td>version="..."</td><td></td><td></td><td></td><td>Début du projet et version de l'application.</td></tr>
/// <tr><td style="font-weight:bold;">data</td><td></td><td></td><td></td><td></td><td>Début des données de paye</td></tr>
/// <tr><td style="font-weight:bold;">XHL</td><td>profondeur="2"</td><td>Enchâsse les balise <i>onglet</i></td><td>Onglet central</td><td></td><td>Données de paye : 2 niveaux d'enchâssement ("onglet" et "item").</td><td>Altair::project</td></tr>
/// <tr><td style="font-weight:bold;">onglet</td><td>V="{année}"</td><td>Enchâsse les balises <i>item</i></td><td>Onglets annuels centraux</td><td></td><td>Onglet pour chaque année de paye (sauf trois derniers).</td></tr>
/// <tr><td style="font-weight:bold;">item</td><td>V="..." S="..." B="..." E="..." EM="..."</td><td>Chaîne de caractères</td><td>Elément des onglets annuels centraux</td><td>Chemin du fichier.</td><td>Chemin du fichier mensuel de paye pour le mois V, le Siret S, l'établissement E et l'employeur EM</td></tr>
/// <tr><td style="font-weight:bold;">onglet</td><td>V="Siret"</td><td></td><td>3ème onglet en partant de droite.</td><td>Si barré(s) : &ndash;&ndash;esiret <i>Siret(s) exclus(s)</i></td><td>Onglet des Siret.</td></tr>
/// <tr><td style="font-weight:bold;">item</td><td>V="" S="" B="" E="" EM=""</td><td>Chaîne de caractères du Siret.</td><td></td><td></td><td>Siret détecté (éventuellement : exclu).</td></tr>
/// <tr><td style="font-weight:bold;">onglet</td><td>V="Budget"</td><td></td><td>2ème onglet en partant de droite.</td><td>Si barré(s) : &ndash;&ndash;ebudget <i>Budget(s) exclus(s)</i></td><td>Onglet des Budgets.</td></tr>
/// <tr><td style="font-weight:bold;">item</td><td>V="" S="" B="" E="" EM=""</td><td>Chaîne de caractères du Budget.</td><td></td><td></td><td>Budget détecté (éventuellement : exclu).</td></tr>
/// <tr><td style="font-weight:bold;">onglet</td><td>V="Employeur"</td><td></td><td>1er onglet en partant de droite</td><td>Si barré(s) : &ndash;&ndash;employeur <i>Employeur(s) exclus(s)</i></td><td>Onglet des Employeurs.</td></tr>
/// <tr><td style="font-weight:bold;">item</td><td>V="" S="" B="" E="" EM=""</td><td>Chaîne de caractères de l'employeur.</td><td></td><td></td><td>Employeur détecté (éventuellement : exclu).</td></tr>
/// <tr><td style="font-weight:bold;">systeme</td><td></td><td></td><td></td><td></td><td>Paramètres système (paramétrage de l'interface, dialogues d'options et de configuration de l'interface)</td></tr>
/// <tr><td style="font-weight:bold;">projectManagerDisplay</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher le gestionnaire de projets à droite de l'interface.<br>La profondeur 0 indique une balise sans enchâssement : la valeur est lue dans le texte de la balise</td><td>MainWindow::defaultProjectManagerWidgetLayoutBox</td></tr>
/// <tr><td style="font-weight:bold;">fileManagerDisplay</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher le gestionnaire (ou explorateur ou arbre) de fichiers à gauche de l'interface.</td><td>MainWindow::defaultFileManagerWidgetLayoutBox</td></tr>
/// <tr><td style="font-weight:bold;">fullScreenDisplay</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Plein écran ("oui" ou "non")</td><td>MainWindow::defaultFullScreenLayoutBox</td></tr>
/// <tr><td style="font-weight:bold;">outputTextEdit</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher l'onglet des messages en bas de l'interface.</td><td>MainWindow::defaultOutputTextEditBox</td></tr>
/// <tr><td style="font-weight:bold;">fileToolBar</td><td> profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher la barre d'outils du menu Fichier.</td><td>MainWindow::defaultFileToolBarBox</td></tr>
/// <tr><td style="font-weight:bold;">editToolBar</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher la barre d'outils du menu Editer.</td><td>MainWindow::defaultEditToolBarBox</td></tr>
/// <tr><td style="font-weight:bold;">processToolBar</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher la barre d'outils du menu Lancer.</td><td>MainWindow::defaultProcessToolBarBox</td></tr>
/// <tr><td style="font-weight:bold;">optionsToolBar</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher la barre d'outils du menu Configurer.</td><td>MainWindow::defaultOptionsToolBarBox</td></tr>
/// <tr><td style="font-weight:bold;">aboutToolBar</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher la barre d'outils du menu Aide.</td><td>MainWindow::defaultAboutToolBarBox</td></tr>
/// <tr><td style="font-weight:bold;">saveProjectBehavior</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Enregistrer les projet .alt automatiquement à chaque modification d'un paramètre.</td><td>MainWindow::defaultSaveProjectBehaviorBox</td></tr>
/// <tr><td style="font-weight:bold;">importerAuLancement</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Importer les données de paye du répertoire Données (.xhl/.xml) au lancement de l'interface.</td><td>MainWindow::importerAuLancementBox</td></tr>
/// <tr><td style="font-weight:bold;">loadProjectBehavior</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Charger le projet .alt par défaut au lancement de l'interface.</td><td>MainWindow::defaultLoadProjectBehaviorBox</td></tr>
/// <tr><td style="font-weight:bold;">limitConsoleOutput</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Limiter le nombre de lignes du log de l'onglet Console.</td><td>MainWindow::defaultMaximumConsoleOutputBox</td></tr>
/// <tr><td style="font-weight:bold;">TRAITEMENT</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye Traitement</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">NBI</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye NBI</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">PFI</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye PFI</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">PFR</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye PFR</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">IPF</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye IPF</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">PSR</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye PSR</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">IFTS</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye IFTS</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">IAT</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye IAT</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">IHTS</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye IHTS</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">VACATAIRES</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye VACATAIRES</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">ASTREINTES</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye ASTREINTES</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">Matricules</td><td>profondeur="0"</td><td>Matricule(s), mois, année(s) saisi(s) pour extraction d'un bulletin de paye donné.<br>Argument de format : matricule-mois_début...mois_fin-année_début...année_fin;matricule2-mois_début2...mois_fin2-année_début2...année_fin2;[etc.]</td><td>Dialogue Extraire les bulletins</td><td>&ndash;&ndash;bulletins <i>argument</i></td><td>1ère ligne.</td><td>MatriculeInput::matrLineEdit</td></tr>
/// <tr><td style="font-weight:bold;">MatriculesB</td><td>profondeur="0"</td><td>Matricule(s), mois, année(s) saisi(s) pour extraction d'un bulletin de paye donné.<br> Argument de même format.</td><td>Dialogue Extraire les bulletins</td><td>&ndash;&ndash;bulletins <i>argument</i></td><td>2e ligne.</td><td>MatriculeInput::matrLineEdit2</td></tr>
/// <tr><td style="font-weight:bold;">MatriculesC</td><td>profondeur="0"</td><td>Matricule(s), mois, année(s) saisi(s) pour extraction d'un bulletin de paye donné.<br> Argument de même format.</td><td>Dialogue Extraire les bulletins</td><td>&ndash;&ndash;bulletins <i>argument</i></td><td>3e ligne.</td><td>MatriculeInput::matrLineEdit3</td></tr>
/// <tr><td style="font-weight:bold;">dossierBulletins</td><td>profondeur="0"</td><td>Chemin du dossier d'extraction des bulletins de paye individuels.</td><td>Dialogue Extraire les bulletins</td><td>&ndash;&ndash;dossier-bulletins <i>argument</i></td><td></td><td>MatriculeInput::dossier</td></tr>
/// <tr><td style="font-weight:bold;">quiet</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td>-q</td><td>Limiter la verbosité de l'onglet Console.</td><td>MainWindow::defaultQuietBox</td></tr>
/// <tr><td style="font-weight:bold;">baseType</td><td>profondeur="0"</td><td>{"", "AN", "X", "T", "I", "S", "A", "R", "AC", "AV", "IR", "C", "D", "RE"}</td><td>Dialogue d'options &rarr; onglet Format</td><td>-T <i>argument</i><td>Type de base monolithique, par année, tous types, ou par type de ligne de paye (Traitement, Indemnité, SFT, etc.)</td><td>standardPage::baseTypeWidget</td></tr>
/// <tr><td style="font-weight:bold;">maxLigne</td><td>profondeur="0"</td><td>Nombre</td><td>Dialogue d'options &rarr; onglet Format</td><td>-T <i>argument</i><td>Maximum du nombre de lignes de la base Table.csv (lignes de paye)</td><td>standardPage::maxNLigneLineEdit</td></tr>
/// <tr><td style="font-weight:bold;">genererNumLigne</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td>-l</td><td>Numéroter les lignes.</td></tr>
/// <tr><td style="font-weight:bold;">exporterEtab</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td>-S</td><td>Exporter les colonnes Siret, Budget, Etablissement, Employeur.</td></tr>
/// <tr><td style="font-weight:bold;">exportMode</td><td>profondeur="0"</td><td>{'Standard', 'Cumulative', 'Distributive', 'Distributive+'}</td><td>Dialogue d'options &rarr; onglet Format</td><td>&ndash;&ndash;export <i>argument</i></td><td>Mode d'exportation des bases CSV (voir guide utilisateur).</td></tr>
/// <tr><td style="font-weight:bold;">FPH</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Activer les contrôles réglementaires FPH.</td></tr>
/// <tr><td style="font-weight:bold;">genererTable</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td>-t</td><td>Générer les bulletins et lignes de paye au format CSV.</td></tr>
/// <tr><td style="font-weight:bold;">archiveTable</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Archiver les sorties CSV du logiciel dans un format de compression (.arch).</td></tr>
/// <tr><td style="font-weight:bold;">exportTable</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Exporter les sorties CSV du logiciel vers un répertoire donné.</td></tr>
/// <tr><td style="font-weight:bold;">archiveAll</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Archiver l'ensemble des sorties du logiciel dans un format de compression (.arch).</td></tr>
/// <tr><td style="font-weight:bold;">exportAll</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Exporter l'ensemble des sorties du logiciel vers un répertoire donné.</td></tr>
/// <tr><td style="font-weight:bold;">archiveXML</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Archiver les données de paye.</td></tr>
/// <tr><td style="font-weight:bold;">exportXML</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Exporter les données de paye vers un répertoire donné.</td></tr>
/// <tr><td style="font-weight:bold;">processType</td><td>profondeur="0"</td><td>{'1',...,'11'}</td><td>Dialogue d'options &rarr; onglet Traitement</td><td>-j <i>argument</i></td><td>Utiliser autant de fils d'exécution qu'indiqué dans le menu déroulant.</td></tr>
/// <tr><td style="font-weight:bold;">log</td><td>profondeur="0"</td><td>Chemin du fichier altair.log</td><td>Dialogue d'options &rarr; onglet Traitement</td><td>-L <i>argument</i></td><td>Chemin du log d’exécution du test de cohérence entre analyseurs C et XML.</td></tr>
/// <tr><td style="font-weight:bold;">genererLog</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td>Autoriser la génération du log.</td></tr>
/// <tr><td style="font-weight:bold;">activerConsole</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td>Activer les sorties Console.</td></tr>
/// <tr><td style="font-weight:bold;">memoryUse</td><td>profondeur="0"</td><td>Nombre entre 1 et 100</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td>Part maximum de la RAM utilisée.</td></tr>
/// <tr><td style="font-weight:bold;">rapportType</td><td>profondeur="0"</td><td>{'PDF', 'WORD et ODT', 'WORD, ODT et PDF'</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td>Activé uniquement si enchainerRapports vaut 'oui'. Rapports d'analyse de type PDF, MS Word (.docx) et Libreoffice Writer (.odt)</td></tr>
/// <tr><td style="font-weight:bold;">enchainerRapports</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td>Lancer automatiquement les scripts R depuis l'interface graphique sans utiliser RStudio.</td></tr>
/// <tr><td style="font-weight:bold;">rapportEntier</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td><i>Interne à l'interface graphique. Non exporté vers le fichier de projet Altaïr .alt.</i><br>Version expérimentale (code de développement).</td></tr></table>
/// \n\n

/// \section  sec_alt2 Exemple commenté de projet .alt
/// <table style="width:35%;"><tr><th>Code XML</th><th>Commentaires</th></tr></table>
/// \code
/// <?xml version="1.0"?>                Fichier de type XML.
/// <projet version="17.12-1">           Début du projet et version de l'application.
/// <data>                               Partie "données de paye".
///  <XHL profondeur="2">                Balise XHL : 2 niveaux d'enchâssement ("onglet" et "item"). Données de paye.
///   <onglet V="2011">                  Onglet de valeur ("V") 2011 (année).
///     <item V="1"
///         S="28XXXXXXXXXXXX"
///         B="BUDGET PRINCIPAL"
///         E="ETABLISSEMENT
///         X" EM="EMPLOYEUR Y">
///     /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl/2011/01.xhl
///     </item>
///                                      Fichier de paye 01.xhl dans le répertoire xhl/2011 de rang 1, correspondant au Siret 28..., à un budget principal de l'établissement X de l'employeur Y.
///   </onglet>
///   <onglet V="2012">                  Onglet de valeur ("V") 2012 (année).
///     <item V="1"
///          S="28XXXXXXXXXXXX"
///          B="BUDGET PRINCIPAL"
///          E="ETABLISSEMENT X"
///          EM="EMPLOYEUR Y">
///     /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl/2012/02.xhl
///     </item>
///                                      Fichier de paye 02.xhl dans le répertoire xhl/2012 du mois 1 (janvier), correspondant au Siret 28..., à un budget principal de l'établissement X de l'employeur Y.
///     <item V="1"
///          S="28XXXXXXXXXXXX"
///          B="BUDGET ANNEXE FORMATION"
///          E="ETABLISSEMENT X"
///          EM="EMPLOYEUR Y">
///      /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl/2012/03.xhl
///      </item>
///                                      Fichier de paye 03.xhl dans le répertoire xhl/2012 du mois 1 (janvier), correspondant au Siret 28..., à un budget principal de l'établissement X de l'employeur Y.
///   <onglet V="Siret">                 Onglet des Siret
///     <item V="" S="" B=""
///           E="" EM="">
///       28XXXXXXXXXXXX
///     </item>
///                                      Seul le Siret 28... est référencé.
///   </onglet>
///   <onglet V="Budget">                Onglet des Budgets
///     <item V="" S="" B=""
///           E="" EM="">
///       BUDGET ANNEXE FORMATION
///     </item>
///                                      Détection d'un budget annexe formation.
///     <item V="" S="" B=""
///           E="" EM="">
///       BUDGET PRINCIPAL
///     </item>
///                                      Détection d'un budget principal.
///   </onglet>
///   <onglet V="Employeur">             Onglet des employeurs
///     <item V="" S="" B=""
///           E="" EM="">
///       EMPLOYEUR Y
///     </item>
///                                      Détection du seul employeur Y.
///   </onglet>
///  </XHL>                              Fin des données de paye de l'onglet central
/// </data>
/// <systeme>                            Paramètres système (paramétrage de l'interface, dialogues d'options et de configuration de l'interface)
///  <projectManagerDisplay profondeur="0">  La profondeur 0 indique une balise sans enchâssement : la valeur est lue dans le texte entre <balise> et </balise>
///   oui                                Afficher le gestionnaire de projets à droite de l'interface.
///  </projectManagerDisplay>
///  <fileManagerDisplay profondeur="0">
///   oui                                Afficher le gestionnaire (ou explorateur ou arbre) de fichiers à gauche de l'interface.
///  </fileManagerDisplay>
///  <fullScreenDisplay profondeur="0">
///   non                                Pas de plein écran
///  </fullScreenDisplay>
///  <outputTextEdit profondeur="0">
///   oui                                Afficher l'onglet des messages en bas de l'interface.
///  </outputTextEdit>
///  <fileToolBar profondeur="0">
///   oui                                Afficher la barre d'outils du menu Fichier.
///  </fileToolBar>
///  <editToolBar profondeur="0">
///   oui                                Afficher la barre d'outils du menu Editer.
///  </editToolBar>
///  <processToolBar profondeur="0">
///   oui                                Afficher la barre d'outils du menu Lancer.
///  </processToolBar>
///  <optionsToolBar profondeur="0">
///   oui                                Afficher la barre d'outils du menu Configurer.
///  </optionsToolBar>
///  <aboutToolBar profondeur="0">
///   non                                Afficher la barre d'outils du menu Aide.
///  </aboutToolBar>
///  <saveProjectBehavior profondeur="0">
///   non                                Ne pas enregistrer les projet .alt automatiquement à chaque modification d'un paramètre.
///  </saveProjectBehavior>
///  <importerAuLancement profondeur="0">
///   non                                Ne pas importer les données de paye du répertoire Données (.xhl/.xml) au lancement de l'interface.
///  </importerAuLancement>
///  <loadProjectBehavior profondeur="0">
///   non                                Ne pas charger le projet .alt par défaut au lancement de l'interface.
///  </loadProjectBehavior>
///  <limitConsoleOutput profondeur="0">
///   non                                Ne pas limiter le nombre de lignes du log de l'onglet Console.
///  </limitConsoleOutput>
///  <quiet profondeur="0">
///   oui                                Limiter la verbosité de l'onglet Console.
///  </quiet>
///  <baseType profondeur="0">
///  &nbsp;                              Type de base monolithique.
///  </baseType>
///  <maxLigne profondeur="0">
///   1000000                            Nombre maximum de lignes de la base de paye : 1 million.
///  </maxLigne>
///  <genererNumLigne profondeur="0">
///   non                                Ne pas numéroter les lignes.
///  </genererNumLigne>
///  <exporterEtab profondeur="0">
///   non                                Ne pas exporter les colonnes Siret, Budget, Etablissement, Employeur.
///  </exporterEtab>
///  <exportMode profondeur="0">
///   'Standard'                         Exportation standard (avec nettoyage de la clé et sans distribution).
///  </exportMode>
///  <FPH profondeur="0">
///   non                                Ne pas activer les contrôles réglementaires FPH.
///  </FPH>
///  <genererTable profondeur="0">
///   oui                                Générer les bulletins et lignes de paye au format CSV.
///  </genererTable>
///  <archiveTable profondeur="0">
///   non                                Ne pas archiver les sorties CSV du logiciel dans un format de compression (.arch)
///  </archiveTable>
///  <exportTable profondeur="0">
///   non                                Ne pas exporter les sorties CSV du logiciel vers un répertoire donné.
///  </exportTable>
///  <archiveAll profondeur="0">
///   non                                Ne pas archiver l'ensemble des sorties du logiciel dans un format de compression (.arch)
///  </archiveAll>
///  <exportAll profondeur="0">
///   non                                Ne pas exporter l'ensemble des sorties du logiciel vers un répertoire donné.
///  </exportAll>
///  <archiveXML profondeur="0">
///   non                                Archiver les données de paye.
///  </archiveXML>
///  <exportXML profondeur="0">
///   non                                Exporter les données de paye vers un répertoire donné.
///  </exportXML>
///  <processType profondeur="0">
///   '4'                                Utiliser 4 fils d'exécution.
///  </processType>
///  <log profondeur="0">
///   /home/fab/.local/share/Altair/altair.log   Chemin du log d’exécution du test de cohérence entre analyseurs C et XML.
///  </log>
///  <genererLog profondeur="0">
///   non                                Ne pas générer le log.
///  </genererLog>
///  <activerConsole profondeur="0">
///   oui                                Activer les sorties Console.
///  </activerConsole>
///  <memoryUse profondeur="0">
///   80                                 Utiliser 80 % de la RAM au maximum.
///  </memoryUse>
///  <rapportType profondeur="0">
///   'WORD et ODT'                      Rapports d'analyse de type MS Word (.docx) et Libreoffice Writer (.odt)
///  </rapportType>
///  <enchainerRapports profondeur="0">
///   non                                Ne pas enchaîner les rapports d'analyse à la suite de l'extraction des données.
///  </enchainerRapports>
///  <TRAITEMENT profondeur="0">
///  &nbsp;                              Pas de code de paye Traitement saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </TRAITEMENT>
///  <NBI profondeur="0">
///  &nbsp;                              Pas de code de paye NBI saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </NBI>
///  <PFI profondeur="0">
///  &nbsp;                              Pas de code de paye PFI saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </PFI>
///  <PFR profondeur="0">
///  &nbsp;                              Pas de code de paye PFR saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </PFR>
///  <IPF profondeur="0"
///  &nbsp;                              Pas de code de paye IPF saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </IPF>
///  <PSR profondeur="0">
///  &nbsp;                              Pas de code de paye PSR saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </PSR>
///  <IFTS profondeur="0">
///  &nbsp;                              Pas de code de paye IFTS saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </IFTS>
///  <IAT profondeur="0">
///  &nbsp;                              Pas de code de paye IAT saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </IAT>
///  <IHTS profondeur="0">
///  &nbsp;                              Pas de code de paye IHTS saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </IHTS>
///  <VACATAIRES profondeur="0">
///  &nbsp;                              Pas de code de paye VACATAIRES saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </VACATAIRES>
///  <ASTREINTES profondeur="0">
///  &nbsp;                              Pas de code de paye ASTREINTES saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </ASTREINTES>
///  <Matricules profondeur="0">
///  &nbsp;                              Pas de matricule saisi pour extraction d'un bulletin de paye donné.
///  </Matricules>
///  <MatriculesB profondeur="0">
///  &nbsp;                              Pas de matricule saisi pour extraction d'un bulletin de paye donné. (2e ligne)
///  </MatriculesB>
///  <MatriculesC profondeur="0">
///  &nbsp;                              Pas de matricule saisi pour extraction d'un bulletin de paye donné. (3e ligne)
///  </MatriculesC>
///  <dossierBulletins profondeur="0">
///   /home/fab/Dev/altair/Tests/Exemple/Donnees/Bulletins   Dossier d'extraction des bulletins de paye individuels.
///  </dossierBulletins>
/// </systeme>                           Fin des paramètres système.
/// </projet>                            Fin du projet.
/// \endcode



inline const QString Altair::makeParserString (int start, int end)
{
    QStringList L = QStringList();
    auto origin = Abstract::abstractWidgetList.begin();

    // Parcourir l'ensemble de la liste abstractWidgetList des fwidgets

    for (auto  u = origin + start; u != Abstract::abstractWidgetList.end() && u <= origin + end; ++u)
        {
            FAbstractWidget* v = *u;

            if (v == nullptr) return "";

            QString hK = v->getHashKey();

            if (hK.isEmpty())
                {
                    Warning (tr ("Erreur"), tr ("Erreur d'analyse XML du projet"));
                    continue;
                }

            // Enregistrer le projet XML à partir du contenu des widgets

            QString xml = v->setXmlFromWidget().toQString();

            if (hK == "XHL" && xml.isEmpty()) continue;

            // La profondeur permet d'ecrire des objets complexes

            QString widgetDepth = v->getDepth();

            L <<  "  <" + hK + " profondeur=\"" + widgetDepth +  "\">\n   "
              + xml
              + "\n  </" + hK + ">\n";
        }

    // Retourner le QString du projet .alt

    return L.join ("");
}


inline const QString  Altair::makeDataString()
{
    return  makeParserString (0, 0);
}

inline const QString  Altair::makeSystemString()
{
    return makeParserString (1);
}


void Altair::writeProjectFile()
{
    checkEmptyProjectName();
    QFile projectFile (projectName);
    QErrorMessage *errorMessageDialog = new QErrorMessage (this);

    if (projectFile.isOpen()) projectFile.close();

    if (! projectFile.open (QFile::WriteOnly | QFile::Truncate | QFile::Text))
        {
            errorMessageDialog->showMessage ("Impossible d'ouvrir le fichier du projet " + projectName + "\n" + qPrintable (projectFile.errorString()));
            QLabel *errorLabel = new QLabel;
            errorLabel->setText (tr ("Si cette case est décochée, ce message "
                                     "ne s'affichera plus à  nouveau."));
            return;
        }

    QTextStream out (&projectFile);
    out.setCodec ("UTF-8");

    out << "<?xml version=\"1.0\"?>\n"
        << "<projet version=\"" VERSION "\">\n";
    out << " <data>\n";

    out << Altair::makeDataString();

    out << " </data>\n";
    out << " <systeme>\n";

    out << Altair::makeSystemString();

    out << " </systeme>\n";

    out << "</projet>\n";
    out.flush();
    options::RefreshFlag = interfaceStatus::hasSavedOptions;

}

namespace XmlMethod
{

QTreeWidgetItem *itemParent = nullptr;

/// Empile les données pour un noeud donné, pour une profondeur d'enchâssement donnée
/// \param node Le noeud de l'arborescence abstraite QDomNode
/// \param level Profondeur de l'enchâssement
/// \param textData Texte à empiler.

inline void stackData (const QDomNode &node, int level, QVariant &textData)
{
    QDomNode  childNode = node.firstChild();

    QStringList strL;
    QString str;

    switch (level)
        {

        // parses < tag> text </tag>

        case 0:

            //    tags[0] = node.toElement().tagName();

            str.clear();

            while ((!childNode.isNull()) && (childNode.nodeType() == QDomNode::TextNode))
                {
                    str += childNode.toText().data().simplified();
                    childNode = childNode.nextSibling();
                }

            textData = QVariant (str);
            break;


//  parses < tags[0]>
//                    <tags[1]>  text </tags[1]>
//                       ....
//                    <tags[1]> text </tags[1]>
//         </tags[0]>
//  note: does not check subordinate tag uniformity


        case 1:

// Add properties collection here to read month */
//       tags[0]=node.toElement().tagName();

            while (!childNode.isNull())
                {
                    QVariant strV;

                    stackData (childNode, 0, strV);
                    QString str = strV.toString();
                    strL << str;
                    QDomElement element = childNode.toElement();
                    Hash::Mois[str] = element.attribute ("V");
                    Hash::Siret[str] << element.attribute ("S");
                    Hash::Budget[str] = element.attribute ("B");
                    Hash::Etablissement[str] << element.attribute ("E");
                    Hash::Employeur[str] = element.attribute ("EM");
                    QDomNamedNodeMap attribs = element.attributes();
                    int i = 2;
                    QString attr;

                    while (attribs.contains (attr = "S" + QString::number (i)))
                        {
                            Hash::Siret[str] << childNode.toElement().attribute (attr);
                            ++i;
                        }

                    i = 2;

                    while (attribs.contains (attr = "E" + QString::number (i)))
                        {
                            Hash::Etablissement[str] << childNode.toElement().attribute (attr);
                            ++i;
                        }

                    childNode = childNode.nextSibling();
                }

            textData = QVariant (strL);
            break;


//                parses
//                         <tags[0]>
//                            <tags[1] V=tags[3]>
//                                 <tags[2] V=tags[4] S=tags[5] B=tags[6] E=tags[7]>  text </tags[2]>
//                                 ....
//                                 <tags[2] V=tags[4] S=tags[5] B=tags[6] E=tags[7]> text </tags[2]>
//                             </tags[1]>
//                             ...
//                             <tags[1] V=tags[3]>
//                                 <tags[2]  V=tags[4] S=tags[5] B=tags[6] E=tags[7]>  text </tags[2]>
//                                 ....
//                                 <tags[2]  V=tags[4] S=tags[5] B=tags[6] E=tags[7]> text </tags[2]>
//                             </tags[1]>
//                           </tags[0]>

        }
}

inline void stackData (const QDomNode & node, QVariant &textData, QStringList& tabLabels)
{
    QDomNode  childNode = node.firstChild();
    QList<QVariant> stackedInfo;
    QString annee;

//    tags[0]=node.toElement().tagName();
    childNode = node.firstChild();

    while (!childNode.isNull())
        {
            if (childNode.toElement().tagName() == "onglet")
                {
                    annee = childNode.toElement().attribute ("V");
//              if (annee[0] != '2') break;
                    tabLabels += annee;
                }

            QVariant M;
            stackData (childNode, 1, M);
            const QStringList SL = M.toStringList();

            for (const QString& s :  SL)
                {
                    Hash::Annee[s] = annee;
                }

            stackedInfo << SL;
            childNode = childNode.nextSibling();
        }

    textData = QVariant (stackedInfo);

}

// Calcule les tailles et envoie les noms de fichier dans l'onglet central.computes sizes and sends filenames to main tab composant
// Affichage sur l'onglet central

void displayTextData (const QStringList &firstColumn,
                      const QString &secondColumn = "",
                      const QString &thirdColumn = "",
                      const QString &fourthColumn = "",
                      const QString &fifthColumn = "",
                      const QString &sixthColumn = "",
                      const QString &seventhColumn = "",
                      const QColor &color = QColor ("blue"))
{
    static QString last;
    static QTreeWidgetItem* item;

    if ((firstColumn.at (0) != last) &&  !firstColumn.at (0).isEmpty())
        {
            item = new QTreeWidgetItem (XmlMethod::itemParent);
            item->setText (0, firstColumn.at (0));
            item->setExpanded (true);
        }

    last = firstColumn.at (0);

    if ((thirdColumn.isEmpty()) && (firstColumn.count() == 1)) return;

    if (item == nullptr) return;

    QTreeWidgetItem* item2 = new QTreeWidgetItem (item);

    if (item2 == nullptr) return;

    if (firstColumn.count() > 1)
        {
            item2->setText (0, firstColumn.at (1));
        }
    else
        {
            if (!thirdColumn.isEmpty())   item2->setText (2, thirdColumn);

            if (!fourthColumn.isEmpty())  item2->setText (3, fourthColumn);

            if (!fifthColumn.isEmpty())   item2->setText (4, fifthColumn);

            if (!sixthColumn.isEmpty())   item2->setText (5, sixthColumn);

            if (!seventhColumn.isEmpty()) item2->setText (6, seventhColumn);

            if (color.isValid())
                {
                    item2->setTextColor (3, color);
                    item2->setTextColor (4, color);
                }

            item2->setTextAlignment (2, Qt::AlignLeft);
            item2->setTextAlignment (3, Qt::AlignRight);
            item2->setTextAlignment (4, Qt::AlignCenter);
            item2->setTextAlignment (5, Qt::AlignLeft);
            item2->setTextAlignment (6, Qt::AlignCenter);
        }

    item2->setText (1, secondColumn);

}


// tags[0] k
//                       tags[1] 1 : xxx  ...  size Mo
//                       tags[1] 2 : xxx  ...  size Mo

inline qint64 displaySecondLevelData (const QStringList &tags,
                                      const QVector<QStringList> &stackedInfo,
                                      const QVector<QStringList> &stackedSizeInfo)
{
    int count = 0, tagcount = 0, l;
    qint64 filesizecount = 0;

    QString firstColumn,
            root = tags.at (0),
            secondColumn = " ",
            thirdColumn,
            fourthColumn,
            fifthColumn,
            sixthColumn,
            seventhColumn;

    int tagListSize = tags.size();

    QVectorIterator<QStringList> i (stackedInfo), j (stackedSizeInfo);

    while (i.hasNext() && j.hasNext())
        {
            if (!root.isEmpty() && tagcount < tagListSize)
                firstColumn = tags.at (tagcount++);

            if (firstColumn[0] != '2') break;

            displayTextData ({firstColumn});

            QStringListIterator w (i.next()), y (j.next());
            l = 0;

            while (w.hasNext() && y.hasNext())
                {
                    ++count;

                    thirdColumn =  "fichier " + QString::number (++l) + "/" + QString::number (count) + ": ";
                    const QString filename = w.next();
                    thirdColumn += filename;
                    secondColumn =  Hash::Mois[filename];
                    sixthColumn = "";

                    for (int s = 0; s < Hash::Siret[filename].size() && s < Hash::Etablissement[filename].size(); ++s)
                        {
                            sixthColumn += ((s > 0) ? "\n" : "") + Hash::Employeur[filename];
                            sixthColumn += " " + Hash::Siret[filename].at (s);
                            sixthColumn += " " + Hash::Etablissement[filename].at (s);
                        }

                    seventhColumn =  Hash::Budget[filename];

                    if ((stackedSizeInfo.size() > 0) && (y.hasNext()))
                        {
                            QStringList units = y.next().split (" ");
                            qint64 msize = units.at (0).toLongLong();
                            filesizecount += msize;
                            // force coertion into float or double using .0
                            fourthColumn    = QString::number (msize / 1048576.0, 'f', 1);
                            fifthColumn   = QString::number (filesizecount / 1048576.0, 'f', 1) + " Mo" ;
                        }

                    displayTextData ({""},
                                     secondColumn,
                                     thirdColumn,
                                     fourthColumn,
                                     fifthColumn,
                                     sixthColumn,
                                     seventhColumn,
                                     (y.hasNext()) ? QColor ("navy") : ((j.hasNext()) ? QColor ("orange") : QColor ("red")));
                }
        }

    return filesizecount;
}


//tags[0]
//                       tags[1] 1 : xxx  ...  (size Mo)
//                       tags[1] 2 : xxx  ...  (size Mo) ...

inline void displayFirstLevelData (const QString &tag,  const QString &style, const QStringList &stackedInfo)
{
    QStringListIterator i (stackedInfo);
    int count = 0;

    while (i.hasNext())
        {
            ++count;
            displayTextData ((count > 1) ? QStringList ("") : QStringList (tag), style + " " + QString::number (count) + ": " + i.next());
        }
}


}  // end of XmlMethod namespace


void Altair::parseProjectFile (QIODevice* file)
{
    // Beware: to be able to interactively modify managerWidget in the parseProjectFile child class constructor,
    // pass it as a parameter to the constructor otherwise the protected parent member will be accessible yet unaltered

    file->seek (0);

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;

    if (!doc.setContent (file, true, &errorStr, &errorLine, &errorColumn))
        {
            Warning (tr ("Décodage XML"), tr ("Erreur de décodage ligne %1, " "colonne %2:\n%3").arg (errorLine).arg (errorColumn).arg (errorStr));
            return;
        }

    QDomElement root = doc.documentElement();

    if (root.tagName() != "projet") return;

    Hash::wrapper.clear();

    Hash::wrapper["version"]  = new FStringList;

    if (Hash::wrapper["version"] == nullptr) return;

    *Hash::wrapper["version"] = FStringList (root.toElement().attribute ("version"));

    QDomNode node = root.firstChild();

//   Empiler les données dans les listes pertinentes, traiter l'information et l'afficher dans l'arbre

    Altair::totalSize = 0;

    for (const QString& maintag :
    {"data", "systeme"
    })
    {
        if (node.toElement().tagName() != maintag) return;

        QDomNode subnode = node.firstChild();

        while (!subnode.isNull())
            {
                const QString key   = subnode.toElement().tagName();

                // Empiler les informations des différentes balises
                // dans une FStringList et ajouter à la table de hashage Hash::wrapper
                // dont la clé est la première balise
                // Dans tout le code on utilisera l'opérateur v(key) pour lire la valeur
                // de la table de manière globale et efficace

                Hash::wrapper[key]  = new FStringList;
                *Hash::wrapper[key] = parseEntry (subnode);

                subnode = subnode.nextSibling();
            }

        node = node.nextSibling();
    }

    project->mainTabWidget->clear();

    // Parcourt l'ensemble des widgets fonctionnels  Abstract::abstractWidgetList
    // et actualise leur statut interne en fonction de l'état du projet .alt
    // en appelant \e setWidgetFromXml

    assignWidgetValues();

    int projectRank = Hash::wrapper["XHL"]->size();

    if (projectRank == 0) return;

    for (int group_index = 0; group_index < projectRank ; group_index++)
        {
            refreshRowPresentation (group_index);
            // Ne pas inclure les onglets supplémentaires (Employeur, Siret et Budget) à ce stade
        }

    emit (project->is_ntabs_changed (projectRank));

    // Il est nécessaire d'avoir une référence (un tableau) des chemins de fichiers initialement importés
    // Pour pouvoir supprimer et surtout récupérer les suppressions de fichiers par l'utilisateur
    // en mémorisant l'état de l'onglet avant suppression

    Hash::createReference (projectRank);

    // Actualiser le gestionnaire de projets

    refreshProjectManagerValues (manager::refreshProjectInteractiveMode
                                 | manager::refreshXHLZone
                                 | manager::refreshSystemZone);
}



FStringList Altair::parseEntry (const QDomNode &node, QTreeWidgetItem *itemParent)
{

    QVariant textData;
    int level = node.toElement().attribute ("profondeur").toInt();

    XmlMethod::itemParent = itemParent;

    QStringList tabLabels;

    // En fonction de la profondeur d'enchâssement des données dans la balise XML,
    // appelle la bonne version de XmlMethod::stackData, et renvoie la FStringList "empilée"

    switch (level)
        {
        case 0:
            XmlMethod::stackData (node, 0, textData);
            return FStringList (textData.toString());

        case 1:
            XmlMethod::stackData (node, 1, textData);
            return FStringList (textData.toStringList());

        case 2:
            XmlMethod::stackData (node, textData, tabLabels);
            project->setTabLabels (tabLabels);
            return FStringList (textData.toList().toVector());
        }

    return FStringList();
}

inline QVector<QStringList> Altair::processSecondLevelData (QVector<QStringList> &L, bool isFile)
{
    QVectorIterator<QStringList> i (L);
    int group_index = 0;

    QVector<QStringList> stackedSizeInfo2 ;

    while (i.hasNext())
        {
            QStringListIterator w (i.next());
            QStringList stackedSizeInfo1 = QStringList();

            while (w.hasNext())
                {
                    QString text = w.next();

                    if (isFile & QFileInfo (text).isFile()) // double check on file status. First check is for processing speed, so that QFileInfo is only called when necessary
                        {
                            // computing filesizes

                            stackedSizeInfo1 <<  QString::number ((long) QFileInfo (text).size()) + " ";

                        }
                }

            stackedSizeInfo2 << stackedSizeInfo1;
            group_index++;
        }

    return stackedSizeInfo2;
}

void Altair::refreshProjectManagerValues (std::uint16_t refreshProjectManagerFlag)
{
    managerWidget->clear();
    QStringList tags = project->getTabLabels();

    if (tags.isEmpty() || Hash::wrapper.isEmpty() || Hash::wrapper["XHL"] == nullptr || Hash::wrapper["XHL"]->isEmpty()) return;

    if ((refreshProjectManagerFlag & manager::refreshProjectInteractiveMask) == manager::refreshProjectInteractiveMode)
        {
            fileSizeDataBase[0] = processSecondLevelData (*Hash::wrapper["XHL"]);
        }

    QTreeWidgetItem *item = new QTreeWidgetItem (managerWidget);
    item->setText (0, "Fichiers xhl");
    item->setExpanded (true);
    XmlMethod::itemParent = item;

    Altair::totalSize = XmlMethod::displaySecondLevelData (
                            tags,
                            *Hash::wrapper["XHL"],
                            fileSizeDataBase[0]);
    Altair::totalSize += 1;

    item = new QTreeWidgetItem (managerWidget);
    item->setText (0, "Logiciel");
    item->setExpanded (true);
    XmlMethod::itemParent = item;
    QStringList L = Hash::wrapper.keys();

    for (int k = 1; k < Abstract::abstractWidgetList.count(); k++)
        {
            const QString& key = Abstract::abstractWidgetList[k]->getHashKey();

            if (! L.contains (key) || Hash::wrapper[key] == nullptr) continue;

            if (Abstract::abstractWidgetList[k]->getDepth() == "0")
                {
                    XmlMethod::displayTextData (Hash::description[key], Hash::wrapper[key]->toQString());
                }
            else if (Abstract::abstractWidgetList[k]->getDepth() == "1")
                XmlMethod::displayFirstLevelData (Hash::description[key].at (0), "bouton", Hash::wrapper[key]->at (0));
        }

}
