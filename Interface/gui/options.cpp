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


/// \file options.cpp
/// \author Fabrice Nicol
/// \brief Implémentation des classes du dialogue d'options

#include <QFile>
#include <thread>
#include <fstream>
#include <vector>
#include "fstring.h"
#include "altair.h"
#include "forms.h"
#include "options.h"
#include "altair.h"
#include "templates.h"
#include "flineframe.h"


QString common::prologue_options_path;

void codePage::ajouterVariable (const QString& nom)
{
    const QString NOM = nom.toUpper();

    // ligne de codes de primes nom

    FLineEdit *line = new FLineEdit ("",                          // Pas de valeur par défaut
                                     QString (NOM).remove (' '),  // Titre (enlever les blancs)
                                    {NOM, "Code de paye"});       // Présentation du gestionnaire de projets


    line->setMinimumWidth (MINIMUM_LINE_WIDTH);

    // Ajouter ici les FLineEdit en pile dans listeCodes

    listeCodes << line;

    // Les labels sont du type Codes NOM
    // Ajouter ici les labels correspondants dans liste Labels
    // Les labels doivent correspondre à une variable chaîne nommée codes.label dans prologue_codes.R

    listeLabels << QString (nom).remove (' ');

    listeDialogueLabels << new QLabel ("Codes " + NOM + " :  ");

    vLayout->addWidget (listeDialogueLabels.last(), listeDialogueLabels.size() - 1, 0, Qt::AlignRight);

    vLayout->addWidget (listeCodes.last(), listeCodes.size() - 1, 1, Qt::AlignLeft);


}

codePage::codePage()
{
   // setVisible(false);

    prologue_options_path = path_access (SCRIPT_DIR "prologue_codes.R");

    appliquerCodes = new QToolButton;

    appliquerCodes->setIcon (QIcon (":/images/view-refresh.png"));
    appliquerCodes->setToolTip ("Appuyer pour exporter ces codes de paye<br>    "
                                "pour la génération des rapports d'analyse.   ");
    appliquerCodes->setCheckable (true);

    variables << "traitement" << "nbi" << "pfi" 
              << "pfr" <<  "ipf" << "psr" << "ifts" << "iat" 
              << "ifse" << "iemp" << "iss" 
              << "ihts" << "vacataires" << "astreintes" ;

    nbVar = variables.size();

    // Pour chacun des membres de variables, ajouter une ligne FLineEdit au dialogue
    // qui donnera lieu à exportation dans prologue_codes.R

    for (const QString& s : variables) ajouterVariable (s);

    label->setFont(QFont("Verdana", 12));

    /***********************************************************************************************************************/
    /* Les lignes de codes ci-après Copyright privé Fabrice Nicol septembre 2020, sous licence identique au reste du code. */
    /* Jusqu'à la ligne indiquée plus loin.                       */
    /************************************************************ */

    codesFrame = new FLineFrame ({"Utiliser un fichier de codes importés", "Chemin du fichier de codes de paye :"},
                                   QDir::toNativeSeparators (path_access(DONNEES_SORTIE "/codes.csv")),
                                   "codesImport",
                                   {0, 2},
                                   v2Layout,
                                   "",   // pas de ligne de commande
                                   directory::noCheck, // ne pas vérifier que le chemin est vide
                                   flags::flineframe::isFilePath,
                                   "Fichier CSV (*.csv)"); // il s'agit d'un chemin de fichier

    codesFrame->setSaveFileName(false);
    codesFrame->setFont("Verdana", 12);
    codesFrame->setToolTip("Cliquer sur le bouton Dossier pour importer les codes de paye<br>"
                           "saisis dans un fichier tableur CSV<br>"
                           "Si un fichier nommé <b>codes.csv</b><br>"
                           "est présent dans le répertoire d'exportation des données (clé, etc.),<br>"
                           "il est automatiquement importé dans la grille de cet onglet.");

    static_cast<QToolDirButton*>(codesFrame->getComponentList()[1])->setOpenBehavior(QToolDirButton::openBehavior::File);

    if (QFileInfo(codesFrame->getText()).isFile())
    {
        importCodesCSV(codesFrame->getText());
    }

    connect(codesFrame, SIGNAL(textChanged(const QString &)), this, SLOT(importCodesCSV(const QString& )));

    const QString& chemin_table_code_libelle
            = path_access(QString(DONNEES_SORTIE)
              + QDir::separator() + "Bases" + QDir::separator() + "Fiabilite"
              + QDir::separator() + "code.libelle.short.csv");

    codesLibellesFrame = new FLineFrame (
                                  {"Ouvrir la table des correspondances Codes-Libellés après génération du rapport",
                                   "Chemin de la table Codes-Libellés générée par le logiciel :"},
                                   QDir::toNativeSeparators (chemin_table_code_libelle),
                                   "codesLibellesAltair",
                                   {2, 2},
                                   v2Layout,
                                   "",   // pas de ligne de commande
                                   directory::noCheck, // ne pas vérifier que le chemin est vide
                                   flags::flineframe::isFilePath,
                                   "Fichier CSV (*.csv)"); // il s'agit d'un chemin de fichier

    codesLibellesFrame->setSaveFileName(false);
    codesLibellesFrame->setFont("Verdana", 12);
    codesLibellesFrame->setToolTip(
                                   "Si elle n'est pas à l'emplacement standard,<br>"
                                   "cliquer sur l'icône foncée pour rechercher<br>"
                                   "la table Codes-Libellés<br>"
                                   "générée par le logiciel dans l'annexe<br>"
                                   "<b>après</b> génération du rapport<br>");

    static_cast<QToolDirButton*>(codesLibellesFrame->getComponentList()[1])->setOpenBehavior(QToolDirButton::openBehavior::File);

    v2Layout->addWidget (label, 5, 2, Qt::AlignLeft);
    v2Layout->addWidget (appliquerCodes, 4, 2, Qt::AlignLeft);
    v2Layout->setColumnMinimumWidth (1, 20);
    v2Layout->setSpacing (10);
    importBox->setLayout (v2Layout);

    /*********************************************/
    /* Fin du copyright privé sous licence libre */
    /********************************************/

    baseBox->setLayout (vLayout);

    vLayout->setSpacing (10);
    FRichLabel *mainLabel = new FRichLabel ("Code de paye des tests");

    mainLayout->addWidget (mainLabel);
    QGridLayout* hLayout= new QGridLayout;
    hLayout->addWidget (baseBox,   0, 0, Qt::AlignTop);
    hLayout->addWidget (importBox, 0, 1, Qt::AlignTop);

    QToolButton* reinitButton = new QToolButton;
    reinitButton->setText("Réinitialiser");
    connect(reinitButton, &QToolButton::clicked, [this] { reinit(update::warnRExport); });
    reinitButton->setToolTip("Réinitialiser tous les champs de cet onglet");

    mainLayout->addLayout (hLayout);
    mainLayout->addWidget (reinitButton, 0, Qt::AlignRight);
    mainLayout->addSpacing (100);

    init_label_text = "Appuyer pour exporter<br> vers les rapports d'analyse ";
    label->setText (init_label_text);

    // Lorsque que bouton appliqueCodes est cliqué, la substitution des valeurs de lignes du dialogue
    // est opérée dans prologue_codes.R

    connect (appliquerCodes, SIGNAL (clicked()), this, SLOT (substituer_valeurs_dans_script_R()));

    // A chaque fois qu'une ligne est éditée à la main,
    // réinitialiser l'état d'exportation (bouton et fichier prologue_codes.R à partir de prologue_init.R)

    for (FLineEdit *a : listeCodes)
        {
            connect (a,
                     &QLineEdit::textEdited,
                     [this]
            {
                label->setText (init_label_text);
                appliquerCodes->setChecked (false);
                appliquerCodes->setIcon (QIcon (":/images/view-refresh.png"));
            });
        }

    setLayout (mainLayout);

    reinitialiser_prologue();
}

/***********************************************************************************************************************/
/* Les lignes de codes ci-après Copyright privé Fabrice Nicol septembre 2020, sous licence identique au reste du code. */
/* Jusqu'à la ligne indiquée plus loin. */

bool codePage::importCodesCSV(const QString& path)
{
    QFile csvFile(path);

    if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
             return false;

    QTextStream in(&csvFile);

    QString headers = in.readLine();
    QString sep = ";";

    QStringList headerList = headers.split(sep);
    if (headerList.size() <= 1)
    {
        headerList = headers.split(",");
        if (headerList.size() <= 1)
        {
            Warning("Attention", "Le fichier CSV doit être à séparateur point-virgule ou virgule");
            return false;
        }
        else sep = ",";
    }

    in.seek(0);

    // on élimine les lignes vides au début

    int nbElem = 0;

    do {
          headers = in.readLine();
          headerList = headers.split(sep, Qt::SkipEmptyParts);
          nbElem = headerList.size();
    } while (nbElem == 0);

    int skipCol = headers.split(sep).size() - nbElem;  // Si width > size, il y a des colonnes vides à gauche

    int i = 0;
    for (auto && s : variables)
    {
        if (i > nbElem - 1)
        {

            QString remainder;

            for (int j = i; j < variables.size(); ++j)
                remainder += variables.at(j);

            Warning("Attention", "il manque des variables dans le fichier CSV : "
                    + remainder
                    );

            return false;
        }
        if (headerList.at(i).toUpper() == s.toUpper())
        {
            ++i;
        }
        else
        {
            Warning("Attention",
                    "Le fichier CSV doit contenir, dans l'ordre, les variables suivantes en colonnes : "
                    + variables.join("<br>"));

            return false;
        }
    }

    for (auto && a : listeCodes) a->setText("");

    while (! in.atEnd())
    {
            QString line = in.readLine();
            QStringList varList = line.split(sep);
            if (skipCol) varList.erase(varList.begin(), varList.begin() + skipCol);
            for (int i = 0; i < variables.size() && i < varList.size(); ++i)
            {
                QString text = listeCodes[i]->text();
                if (! text.isEmpty())
                   text += ";";
                if (! varList.at(i).isEmpty())
                    listeCodes[i]->setText( text + varList.at(i));
            }
    }

    csvFile.close();

    return true;

}

/*********************************************/
/* Fin du copyright privé sous licence libre */
/********************************************/

inline const QString regexp (const QString& X)
{
    return "\"codes." + X + "\" *%a% *NA";
}

inline QString rempl_str (const QString &X, const QString &Y)
{
    QStringList L = Y.split (";", Qt::SkipEmptyParts);
    QString Z;

    if (L.size() >= 0)
        {
            for (QString& s : L)
                {
                    s = "\"" + s + "\"";
                }

            Z = "c(" + L.join (",") + ")";
        }
    else
        return   "\"codes." + X + "\" %a% NA";

    return   "\"codes." + X + "\" %a% " + Z;
}

void codePage::activer_fph (bool activer)
{
    const QStringList variables_fph = {"prime specifique", "prime de service", "prime de technicite", "ift"};

//    if (listeCodes.size() > variables.size())
//        {
//            for (int i = variables.size(); i < listeCodes.size() ; ++i)
//                {
//                    listeCodes[i]->setVisible (activer);
//                    listeDialogueLabels[i]->setVisible (activer);
//                }

//            repaint();
//            return;
//        }

    if (variables.size() == nbVar)
    {
        if (activer)
        {
          variables << variables_fph;
          for (const QString &s : variables_fph) ajouterVariable (s);
        }
    } else {
       if (!activer)
       {
         for (int i = 0; i < variables_fph.size(); ++i)
         {
           variables.removeLast();
           vLayout->removeWidget(listeCodes.last());
           listeCodes.last()->hide();
           vLayout->removeWidget(listeDialogueLabels.last());
           listeDialogueLabels.last()->hide();
           listeCodes.removeLast();
           listeLabels.removeLast();
           listeDialogueLabels.removeLast();

         }
       }
    }

    repaint();
}

void codePage::reinit(int value)
{
  for (auto && a : listeCodes) a->setText("");
  codesLibellesFrame->setText("");
  codesFrame->setText("");
  label->setText(init_label_text);
  appliquerCodes->setChecked (false);
  appliquerCodes->setIcon (QIcon (":/images/view-refresh.png"));
  if ((value & update::warnRExport) == update::warnRExport) substituer_valeurs_dans_script_R();
}

void codePage::substituer_valeurs_dans_script_R()
{

    QString file_str = common::readFile (prologue_options_path);
    common::exporter_identification_controle (file_str);
    bool res = false;
    bool res2 = true;

    QIcon icon0 = QIcon (":/images/view-refresh.png");
    QIcon icon1 = QIcon (":/images/msg.png");
    QIcon icon2 = QIcon (":/images/error.png");

    QIcon icon = (appliquerCodes->isChecked()) ?
                 icon1 :
                 icon0;

    for (const FLineEdit* a : listeCodes)
        {
            bool test = ! a->text().isEmpty();
            res |= test;
            res2 &= test;
        }

    if (res == false)
        {
            Warning ("Attention",
                     "Les codes sont tous non renseignés.<br>"
                     "Les tests statutaires se feront <br>"
                     "sous algorithme heuristique seulement.");

            icon = icon2;

            res = renommer (dump(file_str), prologue_options_path);

            if (Hash::Reference.isEmpty()) return;

            if (res == true)
                label->setText ("L'identification du contrôle a été exportée dans le rapport.");
            else
                label->setText ("Erreur d'enregistrement du fichier de configuration prologue_codes.R");

            return;
        }
    else if (res2 == false)
        {
            QString  liste_codes_nr;
            int i = 0;

            for (const FLineEdit* a : listeCodes)
                {
                    if (a->text().isEmpty())
                        liste_codes_nr += listeLabels[i].toUpper() + "<br>";

                    ++i;
                }

            Warning ("Attention",
                     "Certains code ne sont pas renseignés.<br>"
                     "Les tests statutaires se feront <br>"
                     "sous algorithme heuristique pour :<br>"
                     +
                     liste_codes_nr);
        }

    QString liste_codes;

    for (int rang = 0; rang < listeCodes.size(); ++rang)
        {
            const QString &s     = listeLabels.at (rang);
            const QString &codes = listeCodes.at (rang)->text();
            res = true;

            if (! codes.isEmpty())
                {
                    res = substituer (regexp (s), rempl_str (s, codes), file_str);
                    liste_codes += "<li>" + listeLabels.at (rang).toUpper() + " : "  + codes + "</li>";
                }

            if (res == false)
                {
                    Warning ("Attention",
                             "Le remplacement de la variable codes." + s + "<br>"
                             "n'a pas pu être effectué dans le fichier prologue_codes.R<br>"
                             "Les tests statutaires se feront<br>"
                             "sous algorithme heuristique seulement.");

                    icon = icon2;
                }
        }

    res = renommer (dump (file_str), prologue_options_path);

    appliquerCodes->setIcon (icon);

    if (res == true)
        label->setText ("Les codes de paye suivants :"
                        "<ul>" + liste_codes + "</ul>"
                        "seront  pris en compte pour les rapports.");
    else
        label->setText ("Erreur d'enregistrement du fichier de configuration prologue_codes.R");
}



int rapportPage::ajouterVariable (const QString& nom)
{
    // ligne de codes de primes nom
    QString NOM = nom.toUpper();
    
    FCheckBox *box = new FCheckBox("Partie " + NOM + " : ",
                                   flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                   "Partie_rapport_" + NOM.remove(" "),
                                   {"Rapports" , nom});
    
    // Ajouter ici les FCheckBox en pile dans listeCB

    box->set_tag(nom);

    listeCB << box;
    int size = listeCB.size() - 1;    
    vLayout->addWidget (listeCB.last(), size  % 13, size / 13, Qt::AlignLeft);

    return size + 1;
}


void rapportPage::ajusterDependances(const QString& s, bool checked)
{
    QStringList L, M;

    if (s == "effectifs")
    {
             L << "pyramides" << "durée de service" ;  // Effectifs nécessaires pour pyramides et durée de service
             M << "pyramides" << "durée de service" ;  // Effectifs nécessaires pour pyramides et durée de service
    }
    else
    if (s == "rémunérations brutes")
    {
             L << "comparaisons du brut";  // Rémunérations et comparaisons du brut liées
             M << "comparaisons du brut";  // Rémunérations et comparaisons du brut liées
    }
    else
    if (s == "évolution du net")
    {
             L << "rmpp"; // Rémunérations nettes liées à RMPP/noria et évolutions du net
             M << "rmpp"; // Rémunérations nettes liées à RMPP/noria et évolutions du net
    }
    else
    if (s == "noria")
    {
        L = {}; // Rémunérations nettes liées à RMPP/noria et évolutions du net
        M << "rémunérations nettes" <<  "évolution du net" << "rmpp"; // Rémunérations nettes liées à RMPP/noria et évolutions du net
    }
    else
    if (s == "astreintes")
    {
             L = {}; //HS et astreintes liées
             M << "HS"; //HS et astreintes liées
    }
    else
    if (s == "comptabilité")
    {
            L << "annexe"; // comptabilité et annexe liées par objet code.libelle
            M << "annexe"; // comptabilité et annexe liées par objet code.libelle
    }

    if (checked)
    {
      for (auto&& l : listeCB)
          if (M.contains(l->get_tag()))
          {
              l->setChecked(true); // si la case maître est réactivée alors réactiver les cases dépendantes
          }
    }
    else
    {
      for (auto&& l : listeCB)
          if (L.contains(l->get_tag()))
          {
              l->setChecked(false);
          } // si la case maître est décochée alors désactiver les cases dépendantes
    }
}

rapportPage::rapportPage()
{
    setVisible(false);
    baseBox = new QGroupBox("Parties du rapport d'analyse");
    appliquerCodes = new QToolButton;

    appliquerCodes->setIcon (QIcon (":/images/view-refresh.png"));
    appliquerCodes->setToolTip ("Appuyer pour modifier la sélection <br>    "
                                "des parties des rapports d'analyse.   ");
    appliquerCodes->setCheckable (true);

    variables << "effectifs" << "pyramides" << "durée de service"
              << "rémunérations brutes" << "comparaisons du brut"
              << "rémunérations nettes" << "rmpp" << "évolution du net"
              << "noria"
              << "NBI" << "PFI"
              << "IAT IFTS" << "PFR" << "PSR"
              << "IPF" << "RIFSEEP" << "HS"
              << "astreintes"   << "élus"
              << "comptabilité" << "SFT" << "retraites"
              << "FPH" << "annexe";
    
    int index = 0;

    // Pour chacun des membres de variables, ajouter une ligne FCheckBox au dialogue
    // qui donnera lieu à exportation dans prologue_codes.R

    for (const QString& s : variables)
        index = ajouterVariable(s);
    
    profils = new FComboBox({"Complet", "Allégé",
                             "Démographie", "Démographie et revenus",
                             "Contrôles juridiques", "Réinitialiser"},
                             "profilRapport",
                            {"Rapport", "Niveau de détail du rapport"});
    
    label = new QLabel;
    
    vLayout->addWidget (label, index + 2, 1, Qt::AlignLeft);
    vLayout->addWidget (appliquerCodes, index + 2, 0, Qt::AlignRight);
    vLayout->addWidget (profils, index + 1, 0, Qt::AlignRight);
    vLayout->setColumnMinimumWidth (1, MINIMUM_LINE_WIDTH);
    vLayout->setSpacing (10);

    setToolTip("Sélectionner ou déselectionner certaines parties du rapport d'analyse en fonction des besoins.<br>"
               "Attention certaines parties sont dépendantes d'autres. <br>"
               "Dans ce cas, les cases dépendantes sont désactivées.<br>"
               "Elles peuvent être réactivées en recochant la case dont elles dépendent.");
    
    baseBox->setLayout (vLayout);
        
    FRichLabel *mainLabel = new FRichLabel ("Options des rapports");

    mainLayout->addWidget (mainLabel);
    mainLayout->addWidget (baseBox);

    mainLayout->addSpacing (100);

    init_label_text = "Appuyer pour valider le contenu<br> des rapports d'analyse ";
    label->setText (init_label_text);

    // Lorsque que bouton appliqueCodes est cliqué, la substitution des valeurs de lignes du dialogue
    // est opérée dans prologue_codes.R

    connect (appliquerCodes, SIGNAL (clicked()), this, SLOT (substituer_valeurs_dans_script_R()));
    
    // Lorsqu'une case est modifiée, vérifier l'adéquation des dépendances
    
    for (auto &&cb : listeCB)
        connect(cb, &QCheckBox::toggled, [cb, this] { ajusterDependances(cb->get_tag(), cb->isChecked()); });

    // A chaque fois qu'une case est éditée à la main,
    // réinitialiser l'état d'exportation (bouton et fichier prologue_codes.R à partir de prologue_init.R)

    for (FCheckBox *a : listeCB)
        {
            connect (a,
                     &QCheckBox::toggled,
                     [this]
                        {
                            label->setText (init_label_text);
                            appliquerCodes->setChecked (false);
                            appliquerCodes->setIcon (QIcon (":/images/view-refresh.png"));
                        });
        }
    
    connect(profils,
            &QComboBox::currentTextChanged, 
            [this] 
                {
                    QString text = profils->currentText();
                    QStringList L;

                    if (text == "Allégé")
                    {
                        L = { "effectifs" , "pyramides" , "durée de service",
                               "rémunérations brutes",  "comparaisons du brut",
                               "rémunérations nettes", "rmpp", "évolution du net",
                               "NBI", "RIFSEEP", "HS",
                               "astreintes", "élus", "SFT",
                               "retraites", "FPH", "annexe" };
                    }
                    else
                    if (text == "Complet")
                    {
                        L = variables;
                    }
                    else
                    if (text == "Démographie")
                    {
                        L = {"effectifs", "pyramides", "durée de service", "annexe"};
                    }
                    else 
                    if (text == "Démographie et revenus")
                    {
                        L = {"effectifs" , "pyramides" , "durée de service",
                             "rémunérations brutes",  "comparaisons du brut",
                             "rémunérations nettes", "rmpp", "évolution du net",
                             "noria", "annexe"};
                    }
                    else 
                    if (text == "Contrôles juridiques")
                    {
                        L = { "NBI", "PFI",
                               "IAT IFTS", "PFR", "PSR",
                               "IPF", "RIFSEEP", "HS",
                               "astreintes", "élus",
                               "comptabilité", "SFT", "retraites",
                               "FPH", "annexe"};
                    }
                    else
                    if (text == "Réinitialiser")

                    {
                        L = {};
                    }

                    for (auto &&cb : listeCB)
                    {
                        cb->setChecked(L.contains(cb->get_tag()));
                    }

                });

    setLayout (mainLayout);
}

void rapportPage::message(int r, QIcon& icon, bool paire)
{
    const QIcon& icon2 = QIcon (":/images/error.png");
    
    QString s     = variables.at(r);
    QString s2;
    if (paire) s2 = variables.at(r + 1);
    const bool value    = listeCB.at(r)->isChecked();
    bool value2 = false;
    if (paire) value2 = listeCB.at(r + 1)->isChecked();
    bool res  = true;
    bool res2 = true; 
    QString t  = s;
    QString t2 = s2;
    t.remove(" ");
    if (paire) t2.remove(" ");
                
    res = substituer ("\"script_" + t + "\" *%a% *\\w{4,5}", "\"script_" + t
                      + "\" %a% " + (value ? "TRUE" : "FALSE"), file_str);
    if (paire) res2 = substituer ("script_" + t2 + " *%a% *\\w{4,5}", "script_" + t2
                                  + " %a% " + (value2 ? "TRUE" : "FALSE"), file_str);
    
    if (value) 
    {
        liste_cb += "<li>" + s.toUpper();
        liste_cb += value2 ? (" &nbsp; &nbsp; -  &nbsp; &nbsp;" + s2.toUpper() + "</li>") : "</li>";
    }
    else
    {
        if (value2) liste_cb += "<li>" + s2.toUpper() + "</li>";
    }

    if (res == false)
        {
            Warning ("Attention",
                     "Le remplacement de la variable script_" + s + "<br>"
                     "n'a pas pu être effectué dans le fichier prologue_codes.R<br>");

            icon = icon2;
        }
    
     if (res2 == false)
         {
             Warning ("Attention",
                      "Le remplacement de la variable script_" + t + "<br>"
                      "n'a pas pu être effectué dans le fichier prologue_codes.R<br>");
    
             icon = icon2;
         }
     
      appliquerCodes->setIcon (icon);
}

void rapportPage::substituer_valeurs_dans_script_R()
{
    liste_cb.clear();
    file_str = common::readFile (prologue_options_path);
    
    bool res = false;

    int size = listeCB.size();
     
    const QIcon& icon0 = QIcon (":/images/view-refresh.png");
    const QIcon& icon1 = QIcon (":/images/msg.png");
    QIcon icon = (appliquerCodes->isChecked()) ?
                 icon1 :
                 icon0;
    
    for (int rang = 0; rang < size / 2; ++rang)
        {
            int r = 2 * rang;
            message(r, icon);
        }

    if (size % 2 && size > 0) message(size - 1, icon, false);
    
    res = renommer (dump (file_str), prologue_options_path);

    if (res == true)
        label->setText ("Les parties suivantes :"
                        "<ul>" + liste_cb + "</ul>"
                        "seront  prises en compte pour les rapports.");
    else
        label->setText ("Erreur d'enregistrement du fichier de configuration prologue_codes.R");
}

standardPage::standardPage()
{
    setVisible(false);
    QList<QString> range = QList<QString>(), range2 = QList<QString>(), range2b = QList<QString>();

    range << "Standard" << "Par année" << "Par agent"
          << "Toutes catégories" << "Traitement" << "Indemnité"
          << "SFT"      << "Rémunérations diverses"  
          << "Rappel"     << "Acompte"
          << "Avantage en nature" << "Indemnité de résidence" << "Cotisations"
          << "Déductions"         << "Retenue";

    range2 << ""   << "AN" << "AG" << "X" << "T" << "I"
           << "S"  << "A"  << "R" << "AC"
           << "AV" << "IR" << "C"
           << "D"  << "RE";

    range2b << "Standard" << "Etendu" << "Maximal";

    baseTypeBox = new QGroupBox (tr ("Type de base en sortie"));

    QLabel* baseWidthLabel = new QLabel ("Nombre de colonnes  ");
    baseWidthWidget = new FComboBox (range2b,                         // Contenu du menu déroulant
                                    "baseWidth",                     // Balise XML du projet .alt
                                    {"Données csv",
                                     "Nombre de colonnes"},          // Présentation du gestionnaire de projets
                                     "W");                           // Ligne de commande -W valeur

    baseWidthWidget->setFixedWidth (175);
    baseWidthWidget->setFixedHeight (35);
    baseWidthWidget->setCurrentIndex (0);

    baseWidthWidget->setToolTip (tr ("Sélectionner le nombre de colonnes"
                                  " des bases de données CSV."
                                  "\nStandard : Exporte les colonnes\n\tles plus couramment utilisées.\n"
                                  "Etendu : Exporte les colonnes du mode Standard\n\tplus Civilité, Adresse, Compte bancaire\n\tet nomenclature de l'emploi.\n"
                                  "Maximal : Toutes les données des fichiers XML\n\thormis les nomenclatures globales.\n"));

    QLabel* baseTypeLabel = new QLabel ("Format des bases ");

    baseTypeWidget = new FComboBox (range,                          // Contenu du menu déroulant
                                    "baseType",                     // Balise XML du projet .alt
                                    {"Données csv",
                                     "Type de base par catégorie"}, // Présentation du gestionnaire de projets
                                     "T");                           // Ligne de commande -T valeur


    maxNLigneLabel = new QLabel ("Nombre maximum de lignes\npar segment de base  ");

    maxNLigneLineEdit = new FLineEdit ("1000000",           // Valeur par défaut
                                       "maxLigne",          // Balise XML du projet .alt
                                        {
                                            "Données csv",
                                            "Découper la base de données"
                                            " par segment d'au maximum ce nombre de lignes."
                                        },                  // Présentation du gestionnaire de projets
                                        "T");               // Ligne de commande -T valeur

    maxNLigneLineEdit->setFixedWidth (150);

    QGroupBox* optionalFieldBox = new QGroupBox (tr ("Variables optionnelles"));

    rangCheckBox = new FCheckBox ("Numéroter les lignes",   // Titre de la case à cocher
                                  "genererNumLigne",                        // Balise XML du projet .alt
                                  {"Données csv", "numéroter les lignes"},
                                  "l");

    etabCheckBox = new FCheckBox ("Exporter les informations\nsur l'établissement",      // Titre de la case à cocher
                                  "exporterEtab",                                        // Balise XML du projet .alt
                                  {
                                    "Données csv",
                                    "Exporter les champs Budget,"
                                    " Employeur, Siret, Etablissement"                   // Présentation du gestionnaire de projets
                                  },
                                  "S");                                                  // Ligne de commande -S si cochée

    bomCheckBox = new FCheckBox ("Ne pas forcer la lecture UTF-8 des exports",   // Titre de la case à cocher
                                 flags::status::enabledUnchecked           // Décochée par défaut
                                | flags::commandLineType::coreApplicationCommandLine,  // ligne de commande
                                  "sansBOM",                                 // Balise XML du projet .alt
                                  {"Données csv", "Pas de BOM UTF-8 pour les exports CSV"},       // Présentation du gestionnaire de projets
                                  "sans-bom");                                      // Ligne de commande --sans-bom si cochée

    bomCheckBox ->setToolTip(tr("Cocher cette case pour supprimer le marquage BOM de l'encodage UTF-8\ndes fichiers CSV exportés. \
Utile seulement en cas de visualisation sous certains éditeurs.\nExcel et Calc ne nécessitent pas cette option."));
    QList<QString> exportRange = QList<QString>();
    exportRange << "Standard" << "Cumulative" << "Distributive" << "Distributive+";

    QLabel* exportLabel = new QLabel ("Exportation  ");

    // Utiliser % devant l'option active la syntaxe `--option argument' plutôt que `--option=argument'

    exportWidget = new FComboBox(exportRange,             // Contenu du menu déroulant
                                  "exportMode",           // Balise XML du projet .alt
                                  {
                                    "Données csv",
                                    "Mode d'exportation"
                                  },                      // Présentation du gestionnaire de projets
                                  "%export");             // Ligne de commande --export valeur

    exportWidget->status = flags::status::defaultStatus;
    exportWidget->commandLineType = flags::commandLineType::defaultCommandLine;

    exportWidget->setFixedWidth (175);
    exportWidget->setFixedHeight (35);
    exportWidget->setCurrentIndex (0);
    exportWidget->setToolTip (tr ("Sélectionner la modalité d'exportation"
                                  " des bases de données CSV."
                                  "\nStardard : A chaque contrôle, les bases précédemment\n\texportées sur clé sont effacées.\n"
                                  "Cumulative : A chaque contrôle, les bases s'empilent\n\tà la fin des bases précédemment exportées.\n"
                                  "Distributive : A chaque contrôle, un sous répertoire est créé\n\tsur la clé du nom de chaque dossier\n\timporté du répertoire Données.\n"
                                  "Distributive+ : Exportation Distributive et Cumulative activées.\n"));

    QGridLayout *v1Layout = new QGridLayout,
    *v2Layout = new QGridLayout,
    *v3Layout = new QGridLayout,
    *v4Layout = new QGridLayout;

    v2Layout->addWidget (rangCheckBox,     0, 0, Qt::AlignLeft);
    v2Layout->addWidget (etabCheckBox,     1, 0, Qt::AlignLeft);
    v2Layout->addWidget (bomCheckBox,      2, 0, Qt::AlignLeft);
    v2Layout->setColumnMinimumWidth (1, 250);
    v3Layout->setColumnMinimumWidth (1, 250);

    optionalFieldBox->setLayout (v2Layout);

    FPHCheckBox = new FCheckBox ("Fonction publique hospitalière",         // Titre de la case à cocher
                                 flags::status::enabledUnchecked           // Décochée par défaut
                                | flags::commandLineType::noCommandLine,  // ligne de commande
                                 "FPH",                                    // Balise XML du projet .alt
                                 {
                                    "Contrôle hospitalier",
                                    "Ajuster le rapport pour la FPH"       // Présentation du gestionnaire de projets
                                 });

    tableCheckBox = new FCheckBox ("Créer la base de données",                           // Titre de la case à cocher
                                   flags::status::enabledChecked                         // Par défaut, case cochée génératrice de ligne de commande
                                   | flags::commandLineType::coreApplicationCommandLine,
                                   "genererTable",                                       // Balise XML du projet .alt
                                    {
                                        "Données csv",
                                        "créer la base des lignes et bulletins de paye"  // Présentation du gestionnaire de projets
                                    },
                                    "t",                                                 // Ligne de commande -t si cochée
                                    {
                                        optionalFieldBox, baseTypeLabel,                 // Objets activés lorsque la case l'est, désactivés lorsqu'elle ne l'est pas.
                                        baseTypeWidget,   maxNLigneLabel,
                                        maxNLigneLineEdit, FPHCheckBox,
                                        exportWidget, exportLabel
                                    });

    QStringList range3 = QStringList();

    for (int i = 1; i < 12; i++) range3 << QString::number (i);

    createHash (baseTypeWidget->comboBoxTranslationHash, &range, &range2);
    baseTypeWidget->setFixedWidth (175);
    baseTypeWidget->setFixedHeight (35);
    baseTypeWidget->setCurrentIndex (0);
    baseTypeWidget->setToolTip (tr ("Sélectionner le type de base en sortie"));

    // A ce stade seules les bases monolithiques et par année peuvent être sous découpées en segments d'au maximum N lignes
    // Les autres types de base doivent donc désactiver la FLineEdit maxNLigneLabel.
    // Le code présuppose que les types de base sont rangés dans l'ordre : Standard > Par année > autres types.
    // Code à désactiver lorsque cette fonctionnalités sera étendue aux autres types.

    connect (baseTypeWidget, &FComboBox::currentTextChanged, [this]
    {
        bool value = (baseTypeWidget->currentIndex() > 1);
        maxNLigneLabel->setDisabled (value);
        maxNLigneLineEdit->setDisabled (value);
    });

    connect (FPHCheckBox, SIGNAL (toggled (bool)), this, SLOT (substituer_versant()));

    QGroupBox* archBox = new QGroupBox (tr ("Archivage et Restauration"));
    QGroupBox* exportBox = new QGroupBox (tr ("Exportation"));

    // Les cases à cocher suivantes de classe FCheckBox ont :
    //    - un titre (premier argument),
    //    - une balise XML du projet .alt (deuxième arg.)
    //    - une présentation dans le gestionnaire de projets (troisième arg.)
    // Elles ne sont pas génératrices de ligne de commande.
    // Leur portée est limitée à l'interface graphique
    // L'archivage et l'exportation sont deux commandes déclenchées par l'utilisateurs (menus et boutons)

    FCheckBox* archiveTableBox = new FCheckBox ("Données tableur",
                                                flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                                "archiveTable",
                                                {"Données csv", "Archiver/Restaurer les données CSV"});

    archiveTableBox->setToolTip("Rapport, bases annexées<br>Bases CSV<br>Log, projet");

    FCheckBox* exportTableBox  = new FCheckBox ("Données tableur",
                                                flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                                 "exportTable",
                                                {"Données csv", "Exporter les données CSV"});

    exportTableBox->setToolTip("Rapport, bases annexées<br>Bases CSV<br>Log, projet");

    FCheckBox* archiveAllBox = new FCheckBox ("Tout",
                                              "archiveAll",
                                             {"Données XML", "Archiver/Restaurer les données tableur et XML"});

    archiveAllBox->setToolTip("Rapport, bases annexées<br>Bases CSV<br>Bases XHL/XML<br>Log, projet");

    FCheckBox* exportAllBox  = new FCheckBox ("Tout",
                                              "exportAll",
                                              {"Données XML", "Exporter les données tableur et XML"});

    exportAllBox->setToolTip("Rapport, bases annexées<br>Bases CSV<br>Bases XHL/XML<br>Log, projet");

    FCheckBox* archiveXhlBox = new FCheckBox ("Bases XML",
                                              "archiveXML",
                                              {"Données XML", "Archiver/Restaurer les bases XML"});

    archiveXhlBox->setToolTip("Rapport, bases annexées<br>Bases XHL/XML<br>Log, projet");

    FCheckBox* exportXhlBox  = new FCheckBox ("Bases XML",
                                              "exportXML",
                                              {"Données XML", "Exporter les bases XML"});

    archiveXhlBox->setToolTip("Rapport, bases annexées<br>Bases XHL/XML<br>Log, projet");

    v1Layout->addWidget (tableCheckBox,     1, 0, Qt::AlignLeft);
    v1Layout->addWidget (FPHCheckBox,       2, 0, Qt::AlignLeft);
    v1Layout->addWidget (baseWidthLabel,    4, 0, Qt::AlignRight);
    v1Layout->addWidget (baseWidthWidget,   4, 1, Qt::AlignLeft);
    v1Layout->addWidget (baseTypeLabel,     5, 0, Qt::AlignRight);
    v1Layout->addWidget (baseTypeWidget,    5, 1, Qt::AlignLeft);
    v1Layout->addWidget (exportLabel,       6, 0, Qt::AlignRight);
    v1Layout->addWidget (exportWidget,      6, 1, Qt::AlignLeft);
    v1Layout->addWidget (maxNLigneLabel,    7, 0, Qt::AlignRight);
    v1Layout->addWidget (maxNLigneLineEdit, 7, 1, Qt::AlignLeft);

    baseTypeBox->setLayout (v1Layout);

    v3Layout->addWidget (exportTableBox,    1, 0, Qt::AlignLeft);
    v3Layout->addWidget (exportAllBox,      1, 1, Qt::AlignCenter);
    v3Layout->addWidget (exportXhlBox,      2, 0, Qt::AlignLeft);

    exportBox->setLayout (v3Layout);

    v4Layout->addWidget (archiveTableBox,   1, 0, Qt::AlignLeft);
    v4Layout->addWidget (archiveAllBox,     1, 1, Qt::AlignCenter);
    v4Layout->addWidget (archiveXhlBox,     2, 0, Qt::AlignLeft);

    archBox->setLayout (v4Layout);

    QVBoxLayout* mainLayout = new QVBoxLayout;

    FRichLabel *mainLabel = new FRichLabel ("Format des bases");

    mainLayout->addWidget (mainLabel);
    mainLayout->addWidget (baseTypeBox);
    mainLayout->addWidget (optionalFieldBox);
    mainLayout->addWidget (exportBox);
    mainLayout->addWidget (archBox);

    connect(archiveAllBox, &FCheckBox::toggled, [archiveTableBox, archiveXhlBox, archiveAllBox] {
        if (archiveAllBox->isChecked()) archiveTableBox-> setChecked(true) ;
        if (archiveAllBox->isChecked()) archiveXhlBox-> setChecked(true) ;
    });

    connect(exportAllBox, &FCheckBox::toggled, [exportTableBox, exportXhlBox, exportAllBox] {
        if (exportAllBox->isChecked()) exportTableBox-> setChecked(true) ;
        if (exportAllBox->isChecked()) exportXhlBox-> setChecked(true) ;
    });

    connect(archiveTableBox, &FCheckBox::toggled, [archiveTableBox, archiveAllBox] {
        if (! archiveTableBox->isChecked()) archiveAllBox-> setChecked(false) ;
    });

    connect(archiveXhlBox, &FCheckBox::toggled, [archiveXhlBox, archiveAllBox] {
        if (! archiveXhlBox->isChecked()) archiveAllBox-> setChecked(false) ;
    });

    connect(exportTableBox, &FCheckBox::toggled, [exportTableBox, exportAllBox] {
        if (! exportTableBox->isChecked()) exportAllBox-> setChecked(false) ;
    });

    connect(exportXhlBox, &FCheckBox::toggled, [exportXhlBox, exportAllBox] {
        if (! exportXhlBox->isChecked()) exportAllBox-> setChecked(false) ;
    });

    setLayout (mainLayout);
    substituer_versant();
}

void standardPage::substituer_versant()
{

    const QString &versant_path = path_access (SCRIPT_DIR "prologue_codes.R");

    QString file_str = readFile (versant_path);


    if (FPHCheckBox->isChecked())
        substituer ("\"VERSANT_FP\" *%a% *\"\\w{3}\"", "\"VERSANT_FP\" %a% \"FPH\"", file_str);
    else
        substituer ("\"VERSANT_FP\" *%a% *\"\\w{3}\"", "\"VERSANT_FP\" %a% \"FPT\"", file_str);

    bool res = renommer (dump (file_str), versant_path);

    if (! res) Q ("Le versant de la fonction publique n'a pas pu être exporté.")

}

processPage::processPage()
{
    setVisible(false);
    processTypeBox = new QGroupBox (tr ("Mode d'exécution"));

    const  QStringList &range3 = {"1", "2", "3", "4",
                                  "5", "6", "7", "8",
                                  "9", "10", "11", "12"};

    QLabel* processTypeLabel = new QLabel ("Nombre de fils d'exécution  ");
    processTypeWidget = new FComboBox (range3,
                                       "processType",
                                        {"Type de processus", "Nombre de fils d'exécution"},
                                        "j");

    processTypeWidget->setFixedWidth (45);
    processTypeWidget->setFixedHeight (30);
    processTypeWidget->setCurrentIndex (std::thread::hardware_concurrency() / 2 + 1);
    processTypeWidget->setToolTip (tr ("Sélectionner le nombre de fils d'exécution "));

    QGridLayout *v3Layout = new QGridLayout;
    logFrame = new FLineFrame ({"Générer un log d'exécution", "Chemin du fichier du log"},
                               QDir::toNativeSeparators (common::generateDatadirPath()
                                       + "/altair.log"),
                               "log",
                               {2, 1},
                               v3Layout,
                               "L");

    logFrame->setPathCategory (flags::flineframe::isFilePath);

    logCheckBox = new FCheckBox ("Générer le log  ",
                                 flags::status::enabledUnchecked
                                 | flags::commandLineType::noCommandLine,
                                 "genererLog",
                                { 
                                  "Générer un log d'exécution", 
                                  "application noyau"
                                 },
                                logFrame->getComponentList());

    consoleCheckBox = new FCheckBox ("Activer la console  ",
                                     flags::status::enabledChecked
                                     | flags::commandLineType::noCommandLine,
                                     "activerConsole",
                                    {
                                        "Générer un log d'exécution",
                                        "Utiliser l'onglet console"
                                    });

    const QList<QString> &ecoRange =
                        {"Intensive (100 %)", "Standard (80 %)", "Modérée (60 %)",
                         "Econome (40 %)", "Très économe (20 %)", "Minimale (10 %)",
                         "Rationnée (5%)"};

    const QList<QString> &ecoRange2 = {"100", "80", "60", "40", "20", "10", "5"};

    QLabel *memoryUseLabel = new QLabel ("Utilisation de la mémoire  ");

    /* Utiliser % devant l'option active la syntaxe `--option argument' plutôt que `--option=argument' */

    memoryUseWidget = new FComboBox (ecoRange,
                                     "memoryUse",
                                        {
                                            "Gestion de la mémoire",
                                            "Pourcentage d'utilisation de la mémoire libre"
                                        },
                                      "%memshare");

    createHash (memoryUseWidget->comboBoxTranslationHash, &ecoRange, &ecoRange2);

    memoryUseWidget->status = flags::status::defaultStatus;
    memoryUseWidget->commandLineType = flags::commandLineType::defaultCommandLine;

    memoryUseWidget->setFixedWidth (175);
    memoryUseWidget->setFixedHeight (30);
    memoryUseWidget->setCurrentIndex (1);
    memoryUseWidget->setToolTip (tr ("Sélectionner l'intensité de l'utilisation"
                                     " de la mémoire vive (RAM) libre au lancement"
                                     " de l'application. "
                                     "\nIntensive\t: 100 % de la mémoire\n"
                                     "Standard\t: 80 %\n"
                                     "Modérée\t: 60 %\n"
                                     "Econome\t: 40 %\n"
                                     "Très économe\t: 20 %\n"
                                     "Minimale\t: 10 %\n"
                                     "Rationnée\t:  5 %\n"));

    QGridLayout *v2Layout = new QGridLayout;
    v2Layout->addWidget (memoryUseLabel,    3, 0, Qt::AlignRight);
    v2Layout->addWidget (memoryUseWidget,   3, 1, Qt::AlignLeft);
    v2Layout->addWidget (processTypeLabel,  6, 0, Qt::AlignRight);
    v2Layout->addWidget (processTypeWidget, 6, 1, Qt::AlignLeft);
    v2Layout->addWidget (consoleCheckBox,   7, 0, Qt::AlignLeft);

    processTypeBox->setLayout (v2Layout);

    v3Layout->addWidget (logCheckBox,       1, 0, Qt::AlignLeft);

    QGroupBox* logBox = new QGroupBox (tr ("Log"));
    logBox->setLayout (v3Layout);

    QGridLayout *v4Layout = new QGridLayout;
    QGroupBox* rapportBox = new QGroupBox (tr ("Rapports"));


    QLabel* rapportTypeLabel = new QLabel ("Type de rapport produit par défaut  ");
    rapportTypeWidget = new FComboBox ({"WORD, ODT et PDF", "WORD et ODT", "PDF", "HTML", "Pas de rapport (Bases seules)"},
                                       "rapportType",
                                        {
                                            "Enchaînements",
                                            "Type de rapport"
                                        });

    rapportTypeWidget ->setToolTip (tr ("Sélectionner le type de rapport produit \n"
                                        "en cas d'enchaînement automatique extraction-rapport :\n"
                                        "MS Word .docx et LibreOffice .odt\n"
                                        "Adobe .pdf ou les trois formats "
                                        "en même temps"
                                        "HTML est proposé sans combinaison possible \n"
                                        "avec d'autres formats"));

    enchainerRapports = new FCheckBox ("Enchaîner extraction et analyse",
                                       flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                       "enchainerRapports",
                                        {
                                            "Enchaînements",
                                            "Enchaîner l'extraction des données et la production des rapports"
                                        },
                                        {rapportTypeWidget, rapportTypeLabel});

        
    openCheckBox = new FCheckBox("Ouvrir le document à la fin de l'exécution",
                                            flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                            "ouvrirDocFinExec",
                                            {"Rapports", "Ouvrir en fin d'exécution"});

    v4Layout->addWidget (enchainerRapports, 0, 0, Qt::AlignLeft);
    v4Layout->addWidget (rapportTypeLabel,  1, 0, Qt::AlignRight);
    v4Layout->addWidget (rapportTypeWidget, 1, 1, Qt::AlignLeft);
    v4Layout->addWidget (openCheckBox,      3, 0, Qt::AlignLeft);

#ifndef Q_OS_WIN
    parallelCheckBox = new FCheckBox("Exécution en parallèle du rapport",
                                            flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                            "parallelExec",
                                            {"Rapports", "Exécution parallèle"});
     v4Layout->addWidget (parallelCheckBox,  4, 0, Qt::AlignLeft);
     auto connectedCB = {parallelCheckBox, openCheckBox};
#else
     auto connectedCB = {openCheckBox};
#endif

    rapportBox->setLayout (v4Layout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel = new FRichLabel ("Paramètres de traitement");
    mainLayout->addWidget (mainLabel);
    mainLayout->addWidget (processTypeBox);
    mainLayout->addWidget (logBox);
    mainLayout->addWidget (rapportBox);
    mainLayout->addSpacing (150);

    setLayout (mainLayout);
    for (const FCheckBox* a : connectedCB)
    {

        connect(a, &FCheckBox::toggled, [this] {
                 substituer_valeurs_dans_script_R();
           });
    }
}

void processPage::substituer_valeurs_dans_script_R()
{
    file_str = common::readFile (prologue_options_path);
    #ifndef Q_OS_WIN
    substituer("\"sequentiel\" *%a% *\\w{4,5}", QString("\"sequentiel\" %a% ")
               + (parallelCheckBox->isChecked() ? "FALSE" : "TRUE"), file_str);
    #else
    substituer("\"sequentiel\" *%a% *FALSE", QString("\"sequentiel\" %a% TRUE"), file_str);
    #endif
    substituer("\"ouvrir.document\" *%a% *\\w{4,5}", QString("\"ouvrir.document\" %a% ")
               + (openCheckBox->isChecked() ? "TRUE" : "FALSE"), file_str);
    renommer (dump (file_str), prologue_options_path);
}

std::uint16_t options::RefreshFlag;

extraPage::extraPage()
{
    setVisible(false);
    QGridLayout *v3Layout = new QGridLayout;
        
    budgetFrame = new FLineFrame ({"Utiliser la correspondance budgétaire", "Chemin de la table de correspondance"},
                                   QDir::toNativeSeparators (path_access(DONNEES_SORTIE  "/paye_budget.csv")),
                                   "budget",
                                   {2, 1},
                                   v3Layout,
                                   "",   // pas de ligne de commande
                                   directory::noCheck, // ne pas vérifier si le chemin est vide
                                   flags::flineframe::isFilePath,
                                   "Fichier CSV (*.csv)"); // il s'agit d'un chemin de fichier
    
    budgetFrame->setSaveFileName(false);
   
    constexpr const char* budgetTip = "Le fichier importé donne la correspondance entre <br>"
                                      "le code de paye et le sous-compte du compte 64 <br>"
                                      "utilisé par les comptes administratifs et de gestion.<br>"
                                      "Il doit être au format CSV (séparateur point-virgule)<br>"
                                      "et encodé en caractères UTF-8.<br>"
                                      "Les colonnes doivent comporter les intitulés Code,<br>"
                                      "Libellé, Statut, Type et Compte, dans cet ordre : <br>"
                                      "<ul><li><b>Code</b> : code de  paye de la base dématérialisée.</li>"
                                      "<li><b>Libelle</b> : libellé de  paye associé au code.</li>"
                                      "<li><b>Statut</b> : statut éligible à ce code. Si plusieurs<br>"
                                      "statuts sont éligibles, utiliser une ligne par statut.<br>"
                                      "Valeurs possibles : <br>"
                                      "<ul><li>TITULAIRE</li>"
                                      "<li>STAGIAIRE</li>"
                                      "<li>NON_TITULAIRE</li>"
                                      "<li>AUTRE_STATUT</li>"
                                      "<li>EMPLOI_AIDE</li></ul></li><br>"
                                      "<li><b>Type</b> : Type de ligne de paye. Valeurs possibles :<br>"
                                      "<ul><li>Traitement</li>"
                                      "<li> Indemnité </li>"
                                      "<li> Autres rémunérations</li>"
                                      "<li> Rappels</li>"
                                      "<li> Supplément familial</li>"
                                      "<li> Indemnité de résidence</li></ul><br>"
                                      "<li><b>Compte</b> : sous-compte du 64 (à 5 chiffres).</li></ul><br>"
                                      "A défaut d'importation manuelle de ce fichier, le <br>"
                                      "logiciel réalise une interpolation imparfaite de la <br>"
                                      "table de correspondance et peut produire des résultats<br>"
                                      "relativement éloignés des agrégats comptables. Cette <br>"
                                      "table par défaut est générée sous <b>Bases/Fiabilite</b> <br>"
                                      "sous le nom de fichier <b>code.libelle.csv</b>.";
    
    budgetCheckBox = new FCheckBox ("Correspondance budgétaire  ",
                                    flags::status::enabledUnchecked
                                     | flags::commandLineType::noCommandLine,
                                    "genererBudget",
                                    {
                                       "Générer une correspondance budgétaire", 
                                        ""
                                    },
                                    budgetFrame->getComponentList());
    
    
    v3Layout->addWidget (budgetCheckBox,       1, 0, Qt::AlignLeft);

    QGroupBox* budgetBox = new QGroupBox (tr ("Budget"));
    
    budgetBox->setToolTip(budgetTip);
    budgetBox->setLayout (v3Layout);

    QGridLayout *v4Layout = new QGridLayout;
    
    gradesFrame = new FLineFrame ({"Utiliser un fichier grade/catégorie", "Chemin du fichier de correspondance"},
                                   QDir::toNativeSeparators (path_access(DONNEES_SORTIE "/grades.categories.csv")),
                                   "gradeCategorie",
                                   {2, 1},
                                   v4Layout,
                                   "",   // pas de ligne de commande
                                   directory::noCheck, // ne pas vérifier que le chemin est vide
                                   flags::flineframe::isFilePath,
                                   "Fichier CSV (*.csv)"); // il s'agit d'un chemin de fichier
   
    gradesFrame->setSaveFileName(false);
    
    gradesCheckBox = new FCheckBox ("Correspondance grade-catégorie  ",
                                    flags::status::enabledUnchecked
                                     | flags::commandLineType::noCommandLine,
                                    "genererGradeCategorie",
                                    {
                                       "Générer une correspondance grade-catégorie", 
                                        ""
                                    },
                                    gradesFrame->getComponentList());
    
    v4Layout->addWidget(gradesCheckBox,       1, 0, Qt::AlignLeft);

    QGroupBox* gradesBox = new QGroupBox(tr("Grade et catégorie statutaire"));
    
    constexpr const char* gradesTip = "Le fichier importé donne la correspondance entre les<br>"
                                      "libellés de grade des bases de paye et les catégories <br>"
                                      "statutaires (A, B, C à l'exclusion de toute autre <br>"
                                      "possibilité).<br>"
                                      "Pour les non-titulaires, le renseignement est <br>"
                                      "optionnel. <br>"
                                      "En cas de grade à cheval sur deux catégories, il<br>"
                                      "convient de distinguer les libellés de chaque grade<br>"
                                      "correspondant à chacune des deux catégories, dans ce <br>"
                                      "fichier et dans toute les bases de paye produites par<br>"
                                      "l'extracteur de données.<br>"
                                      "Le fichier importé doit être de type CSV à séparateur<br>"
                                      "point-virgule et encodé en UTF-8.<br>"
                                      "Il doit comporter une ligne d'intitulés de colonnes.<br>"
                                      "La première colonne est intitulée <b>Grade</b> et la seconde <br>"
                                      "<b>Categorie</b>.<br>"
                                      "A défaut d'importation manuelle de ce fichier, le logiciel<br>"
                                      "réalise une interpolation relativement exacte de la <br>"
                                      "correspondance entre grade et catégorie statutaire.<br>"
                                      "Ce fichier est généré sous le dossier <b>Bases/Effectifs</b><br>"
                                      "sous le nom de fichier <b>grades.categories.csv</b><br>";
            
            
    gradesBox->setLayout(v4Layout);
    gradesBox->setToolTip(gradesTip);
    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel = new FRichLabel ("Fichiers externes");
    mainLayout->addWidget (mainLabel);
    mainLayout->addWidget (budgetBox);
    mainLayout->addWidget (gradesBox);

#if DEBUG
    QGridLayout *v5Layout = new QGridLayout;    
    
    logtFrame = new FLineFrame ({"Concessions de logement", "Chemin de la table Matricules-Dates"},
                                   QDir::toNativeSeparators (path_access(DONNEES_SORTIE  "/logements.csv")),
                                   "logement",
                                   {2, 1},
                                   v5Layout,
                                   "",   // pas de ligne de commande
                                   directory::noCheck, // ne pas vérifier que le chemin est vide
                                   flags::flineframe::isFilePath,
                                   "Fichier CSV (*.csv)"); // il s'agit d'un chemin de fichier
   
    logtFrame->setSaveFileName(false);
    
    constexpr const char* logtTip = "Le fichier importé énumère la liste des matricules  <br>"
                                      "bénéficiant d'une concession de logement, pour chaque <br>"
                                      "année et mois.<br>"
                                      "Il doit être au format CSV (séparateur point-virgule)<br>"
                                      "et encodé en caractères Latin-1 ou Windows-1252.<br>"
                                      "Les colonnes doivent comporter les intitulés Matricule,<br>"
                                      "Année, Mois, Logement, dans cet ordre : <br>"
                                      "<ul><li><b>Matricule</b> : Matricule de l'agent au mois concerné.</li>"
                                      "<li><b>Annee</b> : année de la période sous revue.</li>"
                                      "<li><b>Mois</b> : mois de la période sous revue.</li>"
                                      "<li><b>Logement</b> : type du logement, par nécessité absolue de<br>"
                                      "service ou par utilité de service.<br>"
                                      "Valeurs possibles : <br>"
                                      "<ul><li><b>NAS</b> : nécessité absolue de service</li>"
                                      "<li><b>US</b> : utilité de service</li>"
                                      "<li><b>AUTRE</b> : autre cas</li>"
                                      "</ul></li></ul><br>"
                                      "Ces données sont utilisées pour contrôler le cumul des <br>"
                                      "concessions de logement et de certaines indemnités.<br>";
    
    logtCheckBox = new FCheckBox ("Concessions de logement  ",
                                    flags::status::enabledUnchecked
                                     | flags::commandLineType::noCommandLine,
                                    "genererLogt",
                                    {
                                       "Contrôler les concessions de logement", 
                                       ""
                                    },
                                    logtFrame->getComponentList());
        
    v5Layout->addWidget (logtCheckBox,       1, 0, Qt::AlignLeft);

    QGroupBox* logtBox = new QGroupBox (tr ("Logement"));
    
    logtBox->setToolTip(logtTip);
    logtBox->setLayout (v5Layout);
    
    QGridLayout *v6Layout = new QGridLayout;    
    
    ifseFrame = new FLineFrame ({"IFSE", "Chemin de la table Plafonds IFSE"},
                                   QDir::toNativeSeparators (path_access(DONNEES_SORTIE  "/plafonds_ifse.csv")),
                                   "ifse",
                                   {2, 1},
                                   v6Layout,
                                   "",   // pas de ligne de commande
                                   directory::noCheck, // ne pas vérifier que le chemin est vide
                                   flags::flineframe::isFilePath,
                                   "Fichier CSV (*.csv)"); // il s'agit d'un chemin de fichier
   
    ifseFrame->setSaveFileName(false);
    
    constexpr const char* ifseTip = "Le fichier importé énumère la liste des plafonds  <br>"
                                    "de l'IFSE (RIFSEEP), pour chaque groupe de grades <br>"
                                    "logé ou pas.<br>"
                                    "Il doit être au format CSV (séparateur point-virgule)<br>"
                                    "et encodé en caractères Latin-1 ou Windows-1252.<br>"
                                    "Les colonnes doivent comporter les intitulés Grade,<br>"
                                    "Groupe, Plafond, dans cet ordre : <br>"
                                    "<ul><li><b>Grade</b> : Tous les grades de la base de paye sans omission.<br>"
                                    "Pour remplir cette colonne on utilisera la première colonne de la<br> " 
                                    "table <b>grade.categories.csv</b> du dossier Bases/Effectifs.</li>"
                                    "<li><b>Groupe</b> : groupe défini par délibération de l'organisme.<br>"
                                    "Pour tout groupe, il y a en général un sous-groupe logé <br>"
                                    "et un sous-groupe non logé distinct.</li>"
                                    "<li><b>Logement</b> : indique <b>NAS</b> pour le sous-groupe du même grade<br>"
                                    "des agents logés par nécessité absolue de service.</li>"
                                    "<li><b>Plafond</b> : Maximum de l'IFSE pour le grade et le groupe, <br>"
                                    "<b>en euros</b>.</li>"
                                    "Il est recommandé d'utiliser simultanément les fonctionnalités <b>Logement</b><br>"
                                    "(groupe précédent) et <b>IFSE</b>. <br>"
                                    "Lorsque tel est le cas, la colonne Logement du fichier <b>logements.csv</b><br>"
                                    "doit préciser le libellé <b>NAS</b> des agents logés par nécessité absolue de service."
                                    "</ul><br>";
    
    ifseCheckBox = new FCheckBox ("IFSE  ",
                                    flags::status::enabledUnchecked
                                     | flags::commandLineType::noCommandLine,
                                    "genererIFSE",
                                    {
                                       "Contrôler l'IFSE (RIFSEEP)", 
                                       ""
                                    },
                                    ifseFrame->getComponentList());
    
    v6Layout->addWidget (ifseCheckBox,       1, 0, Qt::AlignLeft);

    QGroupBox* ifseBox = new QGroupBox (tr ("IFSE"));
    
    ifseBox->setToolTip(ifseTip);
    ifseBox->setLayout (v6Layout);
    mainLayout->addWidget (logtBox);
    mainLayout->addWidget (ifseBox);

#endif

    mainLayout->addSpacing (250);

    setLayout (mainLayout);
}


void extraPage::do_copies()
{
    short i = 0;

    FLineFrame* frameList[] = {budgetFrame, gradesFrame};
    const char* pathList[] = {"paye_budget.csv",
                     "grades.categories.csv",
                     "logements.csv",
                     "grades.plafonds.csv"};
    
    const char* msgList[]  = {"Le fichier de correspondance paye-budget",
                     "Le fichier de correspondance grade-catégorie",
                     "Le fichier des concessions de logement",
                     "Le fichier des plafonds IFSE"};
    
    for (FLineFrame *a : frameList) 
        {
                if (! a->isEnabled()) continue;
                const QString &file = a->getText();
                if (! file.isEmpty() && QFileInfo(file).exists())
                {
                                   
                      tools::copyFile(file, 
                          userdatadir + (QDir::separator() + QString(pathList[i])),
                          msgList[i],  // message d'erreur fenêtre
                          REQUIRE);    // force ce message en cas d'échec
                }
            
            ++i;
        }
}

void extraPage::substituer_valeurs_dans_script_R()
{
    QString file_str = common::readFile (prologue_options_path);
    substituer("\"paye.budget.existe\" *%a% *\\w{4,5}", QString("\"paye.budget.existe\" %a% ")
               + (budgetCheckBox ->isChecked() ? "TRUE" : "FALSE"), file_str);
    renommer (dump (file_str), prologue_options_path);
}

options::options (Altair* parent)
{
    /* plain old data types must be 0-initialised even though the class instance was new-initialised. */
    lower();
    //setVisible(false);
    options::RefreshFlag = interfaceStatus::optionTabs;

    optionWidget = new QListWidget;
    optionWidget->setViewMode (QListView::IconMode);
    optionWidget->setIconSize (QSize (48, 48));
    optionWidget->setFont (QFont ("Garamond", parent->parent->fontsize - 2));
    optionWidget->setMovement (QListView::Static);
    optionWidget->setFixedWidth (98);
    optionWidget->setSpacing (12);
    optionWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    standardTab = new standardPage;
    codeTab     = new codePage;
    extraTab    = new extraPage();
    rapportTab  = new rapportPage();
    processTab  = new processPage;

    connect(processTab->consoleCheckBox, &FCheckBox::toggled,
            [this, parent]
    {
        if (processTab->consoleCheckBox->isChecked())
           emit(parent->textAppend(PARAMETER_HTML_TAG "Console activée."));
        else
           emit(parent->textAppend(PARAMETER_HTML_TAG "Console désactivée."));

    });

    tabs = {standardTab, codeTab, rapportTab, processTab};

    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget (standardTab);
    pagesWidget->addWidget (processTab);
    pagesWidget->addWidget (codeTab);
    pagesWidget->addWidget (extraTab);
    pagesWidget->addWidget (rapportTab);

    closeButton = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    closeButton->button (QDialogButtonBox::Ok)->setText ("Accepter");
    closeButton->button (QDialogButtonBox::Cancel)->setText ("Annuler");

    createIcons();
    optionWidget->setCurrentRow (0);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget (optionWidget);
    horizontalLayout->addWidget (pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch (1);
    buttonsLayout->addWidget (closeButton);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout (horizontalLayout);
    mainLayout->addStretch (1);
    mainLayout->addLayout (buttonsLayout);
    setLayout (mainLayout);
    setWindowTitle (tr ("Options"));
    setWindowIcon (QIcon (":/images/altair.png"));

    connect (closeButton,
             &QDialogButtonBox::accepted,
             [this, parent]
                {
                    options::RefreshFlag =  interfaceStatus::hasUnsavedOptions;
                    accept();

                    parent->updateProject (update::saveProject | update::noWarnRExport);

                    parent->execPath = execPath;

                    extraTab->do_copies();
                    extraTab->substituer_valeurs_dans_script_R();

                });

    connect (closeButton, SIGNAL (rejected()), this, SLOT (reject()));
    connect (optionWidget,
             SIGNAL (currentItemChanged (QListWidgetItem*, QListWidgetItem*)),
             this, SLOT (changePage (QListWidgetItem*, QListWidgetItem*)));

    connect (standardTab->FPHCheckBox, SIGNAL (toggled (bool)), codeTab, SLOT (activer_fph (bool)));
    connect (standardTab->exportWidget, SIGNAL (currentIndexChanged (int)),
             this,
             SLOT (enchainerRapports (int)));
    connect (standardTab->exportWidget, SIGNAL(currentIndexChanged(int)),
             this,
             SLOT(ne_pas_ouvrir_documents()));
//#endif

}

void options::ne_pas_ouvrir_documents()
{
    if (standardTab->exportWidget->currentText().left(4) == "Dist")
                          processTab->openCheckBox->setChecked(false);
}

void options::enchainerRapports (int index)
{
    processTab->enchainerRapports->setChecked (index > 1);
}

// implement a global clear() function for the FStringList of data in an FListFrame ;
// it will be used as Altair::clearData() too. Usage below is faulty.

void options::clearOptionData()
{
    Hash::wrapper.clear();
    Hash::Reference.clear();

    options::RefreshFlag = interfaceStatus::optionTabs;
}


void options::createIcon (const char* path)
{
    QListWidgetItem *button = new QListWidgetItem (optionWidget);
    QString strpath = QString (path);

    QIcon icon = QIcon (strpath);
    button->setIcon(icon);
    button->setFlags (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    // toutes les icones devraient être de taille 48x48 ici
}


void options::createIcons()
{
    QList<const char*> iconList = QList<const char*>()
                                  << ":/images/csv2.png"
                                  << ":/images/process2.png"
                                  << ":/images/data-icon2.png"
                                  << ":/images/extra2.png"
                                  << ":/images/rapport2.png";

    for (auto && i : iconList) createIcon (i);
}

void options::changePage (QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    int r;
    r = (current) ? optionWidget->row (current) : 0;

    if (current) pagesWidget->setCurrentIndex (r);
}





