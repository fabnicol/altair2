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

extern int fontsize;

QString codePage::prologue_codes_path;

int codePage::ajouterVariable (const QString& nom)
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

    return listeCodes.size();
}

codePage::codePage()
{
    baseBox = new QGroupBox;
    prologue_codes_path = path_access (SCRIPT_DIR "prologue_codes.R");

    appliquerCodes = new QToolButton;

    appliquerCodes->setIcon (QIcon (":/images/view-refresh.png"));
    appliquerCodes->setToolTip ("Appuyer pour exporter ces codes de paye<br>    "
                                "pour la génération des rapports d'analyse.   ");
    appliquerCodes->setCheckable (true);

    variables << "traitement" << "nbi" << "pfi" 
              << "pfr" <<  "ipf" << "psr" << "ifts" << "iat" 
              << "ifse" << "iemp" << "iss" 
              << "ihts" << "vacataires" << "astreintes" 
              << "nas";

    short index = 0;

    // Pour chacun des membres de variables, ajouter une ligne FLineEdit au dialogue
    // qui donnera lieu à exportation dans prologue_codes.R

    for (const QString& s : variables) index = ajouterVariable (s);

    label = new QLabel;

    vLayout->addWidget (label, index + 1, 1, Qt::AlignLeft);
    vLayout->addWidget (appliquerCodes, index, 1, Qt::AlignLeft);
    vLayout->setColumnMinimumWidth (1, MINIMUM_LINE_WIDTH);
    vLayout->setSpacing (10);

    baseBox->setLayout (vLayout);

    FRichLabel *mainLabel = new FRichLabel ("Code de paye des tests");

    mainLayout->addWidget (mainLabel);
    mainLayout->addWidget (baseBox, 1, 0);
    mainLayout->addSpacing (100);

    init_label_text = "Appuyer pour exporter<br> vers les rapports d'analyse ";
    label->setText (init_label_text);

    // Lorsque que bouton appliqueCodes est cliqué, la substitution des valeurs de lignes du dialogue
    // est opérée dans prologue_codes.R

    connect (appliquerCodes, SIGNAL (clicked()), this, SLOT (substituer_valeurs_dans_script_R()));

    // A chaque fois qu'une ligne est éditée à la main, réinitialiser l'état d'exportation (bouton et fichier prologue_codes.R à partor de prologue_init.R)

    for (FLineEdit *a : listeCodes)
        {
            connect (a,
                     &QLineEdit::textEdited,
                     [this]
            {
                label->setText (init_label_text);
                appliquerCodes->setChecked (false);
                appliquerCodes->setIcon (QIcon (":/images/view-refresh.png"));
                reinitialiser_prologue();
            });
        }

    setLayout (mainLayout);

    reinitialiser_prologue();
}

inline const QString regexp (const QString& X)
{
    return "codes." + X + " *<- *NA";
}

inline QString rempl_str (const QString &X, const QString &Y)
{
    QStringList L = Y.split (";", QString::SkipEmptyParts);
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
        return   "codes." + X + " <- NA";

    return   "codes." + X + " <- " + Z;
}

void codePage::activer_fph (bool activer)
{
    const QStringList variables_fph = {"prime specifique", "prime de service", "prime de technicite", "ift"};

    if (listeCodes.size() > variables.size())
        {
            for (int i = variables.size(); i < listeCodes.size() ; ++i)
                {
                    listeCodes[i]->setVisible (activer);
                    listeDialogueLabels[i]->setVisible (activer);
                }

            repaint();
            return;
        }

    if (! activer) return;

    vLayout->removeWidget (label);
    vLayout->removeWidget (appliquerCodes);

    for (const QString &s : variables_fph) ajouterVariable (s);

    short index = variables.size() + variables_fph.size();

    vLayout->addWidget (label, index + 1, 1, Qt::AlignLeft);
    vLayout->addWidget (appliquerCodes, index, 1, Qt::AlignLeft);

}

void codePage::substituer_valeurs_dans_script_R()
{
    reinitialiser_prologue();
    QString file_str = common::readFile (prologue_codes_path);
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

            res = renommer (dump(file_str), prologue_codes_path);

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

    res = renommer (dump (file_str), prologue_codes_path);

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

    listeCB << box;
    int size = listeCB.size() - 1;    
    vLayout->addWidget (listeCB.last(), size  % 13, size / 13, Qt::AlignLeft);

    return size + 1;
}


void rapportPage::ajusterDependances(int i)
{
    QVector<int> L, M;
    switch (i)
    {
     case 0:
        if (! listeCB[0]->isChecked())
            {
                L << 1 << 2 ;  // Effectifs nécessaires pour pyramides et durée de service
            }
        if (listeCB[0]->isChecked())
        {
            M << 1 << 2 ;  // Effectifs nécessaires pour pyramides et durée de service
        }
        break;
       
     case 3:
        if (! listeCB[3]->isChecked())
            {
                L << 4;  // Rémunérations et comparaisons du brut liées
            }
        if (listeCB[3]->isChecked())
        {
            M << 4;  // Rémunérations et comparaisons du brut liées
        }
        
        break;
        
     case 5: 
         if (! listeCB[5]->isChecked())
            {
                L << 6 << 7; // Rémunérations nettes liées à RMPP/noria et évolutions du net
            }
         if (listeCB[5]->isChecked())
         {
             M <<  6 << 7; // Rémunérations nettes liées à RMPP/noria et évolutions du net
         }
         
        break;
         
     case 8:    
        if (! listeCB[8]->isChecked())
        {
            L << 11 << 18;  // NBI nécessaire pour contrôle NAS et astreintes
        }
        if (listeCB[8]->isChecked())
        {
            M << 11 << 18;  // NBI nécessaire pour contrôle NAS et astreintes
        }
        break;
     
     case 12:   
        if (! listeCB[12]->isChecked())
        {
            L << 13 << 14 << 15 << 16;  // cumuls indemnités et IFTS
        }
        if (listeCB[12]->isChecked())
        {
            M << 13 << 14 << 15 << 16;  // cumuls indemnités et IFTS
        }
        
        break;
        
     case 17:   
        if (! listeCB[17]->isChecked())
        {
            L << 18; //HS et astreintes liées
        }
        if (listeCB[17]->isChecked())
        {
            M << 18; //HS et astreintes liées
        }
        break;
     
     case 20:   
        if (! listeCB[20]->isChecked())
        {
            L << 24; // comptabilité et annexe liées par objet code.libelle
        }
        if (listeCB[20]->isChecked())
        {
            M << 24; // comptabilité et annexe liées par objet code.libelle
        }
        break;
    }
    for (int l : L) listeCB[l]->setEnabled(false); // si la case maître est décochée alors désactiver les cases dépendantes
    for (int m : M) listeCB[m]->setEnabled(true);  // si la case maître est réactivée alors réactiver les cases dépendantes 
}

rapportPage::rapportPage()
{
    baseBox = new QGroupBox("Parties du rapport d'analyse");
    appliquerCodes = new QToolButton;

    appliquerCodes->setIcon (QIcon (":/images/view-refresh.png"));
    appliquerCodes->setToolTip ("Appuyer pour modifier la sélection <br>    "
                                "des parties des rapports d'analyse.   ");
    appliquerCodes->setCheckable (true);
    variables << "effectifs" << "pyramides" << "durée de service" << "rémunérations brutes" << "comparaisons du brut" 
              << "rémunérations nettes" << "rmpp et noria" << "évolution du net" << "NBI" << "PFI" 
              << "vacataires" << "NAS" << "IAT IFTS" << "PFR" << "PSR" 
              << "IPF" << "RIFSEEP" << "HS" << "astreintes" << "élus" 
              << "comptabilité" << "SFT" << "retraites" << "FPH" << "annexe";  
    
    short index = 0;

    // Pour chacun des membres de variables, ajouter une ligne FCheckBox au dialogue
    // qui donnera lieu à exportation dans prologue_codes.R

    for (const QString& s : variables) index = ajouterVariable (s);
    
    profils = new FComboBox({"Complet", "Allégé", "Minimal", "Démographie", "Démographie et revenus", "Contrôles juridiques"},
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
    mainLayout->addWidget (baseBox, 1, 0);

    mainLayout->addSpacing (100);

    init_label_text = "Appuyer pour valider le contenu<br> des rapports d'analyse ";
    label->setText (init_label_text);

    // Lorsque que bouton appliqueCodes est cliqué, la substitution des valeurs de lignes du dialogue
    // est opérée dans prologue_codes.R

    connect (appliquerCodes, SIGNAL (clicked()), this, SLOT (substituer_valeurs_dans_script_R()));
    
    // Lorsqu'une case est modifiée, vérifier l'adéquation des dépendances
    
    for (int i = 0; i < listeCB.size(); ++i) connect( listeCB[i], &QCheckBox::toggled, [this, i] { ajusterDependances(i); });

    // A chaque fois qu'une case est éditée à la main, réinitialiser l'état d'exportation (bouton et fichier prologue_codes.R à partir de prologue_init.R)

    for (FCheckBox *a : listeCB)
        {
            connect (a,
                     &QCheckBox::toggled,
                     [this]
                        {
                            label->setText (init_label_text);
                            appliquerCodes->setChecked (false);
                            appliquerCodes->setIcon (QIcon (":/images/view-refresh.png"));
                            reinitialiser_prologue();
                        });
        }
    
    connect(profils,
            &QComboBox::currentTextChanged, 
            [this] 
                {
                    
                    QString text = profils->currentText();
                    QVector<int> L;
                    if (text == "Complet") // 25 points
                    {
                        for (int i = 0;  i < variables.size(); ++i)
                            L << i;
                    }
                    else
                    if (text == "Minimal") // 7 sur 25
                    {
                            
                            L = {0, 1, 3, 8, 12, 17, 21 };
                    }
                    else 
                    if (text == "Démographie") // 3 sur 25
                    {
                            L = {0, 1, 2};   
                    }
                    else 
                    if (text == "Allégé")   // 15 sur 25
                    {
                            L = {0, 1, 2, 5, 7, 8, 12, 13, 14, 16, 17, 18, 20, 21, 24};   
                    }
                    else 
                    if (text == "Démographie et revenus") // 8 sur 25
                    {
                            L = {0, 1, 2, 3, 4, 5, 6, 7};   
                    }
                    else 
                    if (text == "Contrôles juridiques") // 17 sur 25
                    {
                            for (int i = 8 ; i <= 24; ++i)  L << i;
                    }
                    
                    for (int i = 0; i < listeCB.size(); ++i) 
                        listeCB[i]->setChecked(L.contains(i));
                });

    setLayout (mainLayout);

    reinitialiser_prologue();
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
                
    res = substituer ("script_" + t + " *<- *TRUE", "script_" + t + " <- " + (value ? "TRUE" : "FALSE"), file_str);
    if (paire) res2 = substituer ("script_" + t2 + " *<- *TRUE", "script_" + t2 + " <- " + (value2 ? "TRUE" : "FALSE"), file_str);
    
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
    reinitialiser_prologue();
    liste_cb.clear();
    file_str = common::readFile (prologue_scripts_path);
    
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
    
    res = renommer (dump (file_str), prologue_scripts_path);

    if (res == true)
        label->setText ("Les parties suivantes :"
                        "<ul>" + liste_cb + "</ul>"
                        "seront  prises en compte pour les rapports.");
    else
        label->setText ("Erreur d'enregistrement du fichier de configuration prologue_codes.R");
}

standardPage::standardPage()
{
    QList<QString> range = QList<QString>(), range2 = QList<QString>();

    range << "Standard" << "Par année" << "Par agent" << "Toutes catégories" << "Traitement" << "Indemnité"
          << "SFT"      << "Rémunérations diverses"           << "Rappel"     << "Acompte"
          << "Avantage en nature" << "Indemnité de résidence" << "Cotisations"
          << "Déductions"         << "Retenue";

    range2 << ""   << "AN" << "AG" << "X" << "T" << "I"
           << "S"  << "A"  << "R" << "AC"
           << "AV" << "IR" << "C"
           << "D"  << "RE";

    QLabel* baseTypeLabel = new QLabel ("Type de base  ");
    baseTypeBox = new QGroupBox (tr ("Type de base en sortie"));

    baseTypeWidget = new FComboBox (range,                          // Contenu du menu déroulant
                                    "baseType",                     // Balise XML du projet .alt
                                    {"Données csv",
                                     "Type de base par catégorie"}, // Présentation du gestionnaire de projets
                                    "T");                           // Ligne de commande -T valeur

    maxNLigneLabel = new QLabel ("Nombre maximum de lignes\npar segment de base  ");

    maxNLigneLineEdit = new FLineEdit ("1000000",                                                // Valeur par défaut
                                       "maxLigne",                                               // Balise XML du projet .alt
                                        {
                                            "Données csv",
                                            "Découper la base de données"
                                            " par segment d'au maximum ce nombre de lignes."
                                        },                                                       // Présentation du gestionnaire de projets
                                        "T");                                                    // Ligne de commande -T valeur

    maxNLigneLineEdit->setFixedWidth (150);

    QGroupBox* optionalFieldBox = new QGroupBox (tr ("Variables optionnelles"));

    rangCheckBox = new FCheckBox ("Numéroter les lignes",                   // Titre de la case à cocher
                                  "genererNumLigne",                        // Balise XML du projet .alt
                                  {"Données csv", "numéroter les lignes"},  ///
                                  "l");

    etabCheckBox = new FCheckBox ("Exporter les informations\nsur l'établissement",      // Titre de la case à cocher
                                  "exporterEtab",                                        // Balise XML du projet .alt
                                  {
                                    "Données csv",
                                    "Exporter les champs Budget,"
                                    " Employeur, Siret, Etablissement"                   // Présentation du gestionnaire de projets
                                  },
                                  "S");                                                  // Ligne de commande -S si cochée

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
    exportWidget->setFixedHeight (30);
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
    v2Layout->setColumnMinimumWidth (1, 250);
    v3Layout->setColumnMinimumWidth (1, 250);

    optionalFieldBox->setLayout (v2Layout);

    FPHCheckBox = new FCheckBox ("Fonction publique hospitalière",         // Titre de la case à cocher
                                 flags::status::enabledUnchecked           // Décochée par défaut
                                 | flags::commandLineType::noCommandLine,  // Pas de ligne de commande
                                 "FPH",                                    // Balise XML du projet .alt
                                 {
                                    "Contrôle hospitalier",
                                    "Ajuster le rapport pour la FPH"       // Présentation du gestionnaire de projets
                                 }
                                );

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
    baseTypeWidget->setFixedHeight (30);
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


    FCheckBox* exportTableBox  = new FCheckBox ("Données tableur",
                                                flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                                 "exportTable",
                                                {"Données csv", "Exporter les données CSV"});

    FCheckBox* archiveAllBox = new FCheckBox ("Tout",
                                              "archiveAll",
                                             {"Données XML", "Archiver/Restaurer les données tableur et XML"});

    FCheckBox* exportAllBox  = new FCheckBox ("Tout",
                                              "exportAll",
                                              {"Données XML", "Exporter les données tableur et XML"});

    FCheckBox* archiveXhlBox = new FCheckBox ("Bases XML",
                                              "archiveXML",
                                              {"Données XML", "Archiver/Restaurer les bases XML"});

    FCheckBox* exportXhlBox  = new FCheckBox ("Bases XML",
                                              "exportXML",
                                              {"Données XML", "Exporter les bases XML"});

    v1Layout->addWidget (tableCheckBox,     1, 0, Qt::AlignLeft);
    v1Layout->addWidget (FPHCheckBox,       2, 0, Qt::AlignLeft);
    v1Layout->addWidget (baseTypeLabel,     3, 0, Qt::AlignRight);
    v1Layout->addWidget (baseTypeWidget,    3, 1, Qt::AlignLeft);
    v1Layout->addWidget (exportLabel,       4, 0, Qt::AlignRight);
    v1Layout->addWidget (exportWidget,      4, 1, Qt::AlignLeft);
    v1Layout->addWidget (maxNLigneLabel,    5, 0, Qt::AlignRight);
    v1Layout->addWidget (maxNLigneLineEdit, 5, 1, Qt::AlignLeft);

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
    mainLayout->addWidget (baseTypeBox,      1, 0);
    mainLayout->addWidget (optionalFieldBox, 2, 0);
    mainLayout->addWidget (exportBox,        3, 0);
    mainLayout->addWidget (archBox,          4, 0);

    setLayout (mainLayout);
    substituer_versant();
}

void standardPage::substituer_versant()
{

    const QString &versant_path = path_access (SCRIPT_DIR "versant.R");

    QString file_str = readFile (versant_path);


    if (FPHCheckBox->isChecked())
        substituer ("VERSANT_FP <<- .*", "VERSANT_FP <<- \"FPH\"", file_str);
    else
        substituer ("VERSANT_FP <<- .*", "VERSANT_FP <<- \"FPT\"", file_str);

    bool res = renommer (dump (file_str), versant_path);

    if (! res) Q ("Le versant de la fonction publique n'a pas pu être exporté.")

}

processPage::processPage()
{
    processTypeBox = new QGroupBox (tr ("Mode d'exécution"));

    QStringList range3 = QStringList();

    for (int i = 1; i < 12; i++) range3 << QString::number (i);

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

    QList<QString> ecoRange = QList<QString>(), ecoRange2 = QList<QString>();
    ecoRange << "Intensive (100 %)" << "Standard (80 %)" << "Modérée (60 %)"
             << "Econome (40 %)" << "Très économe (20 %)" << "Minimale (10 %)"
             << "Rationnée (5%)";

    ecoRange2 << "100"   << "80" << "60" << "40" << "20" << "10" << "5";

    QLabel* memoryUseLabel = new QLabel ("Utilisation de la mémoire  ");

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
    rapportTypeWidget = new FComboBox ((QStringList() << "WORD et ODT" << "PDF" << "WORD, ODT et PDF"),
                                       "rapportType",
    {
        "Enchaînements",
        "Type de rapport"
    });

    rapportTypeWidget ->setToolTip (tr ("Sélectionner le type de rapport produit \nen cas d'enchaînement automatique extraction-rapport :\n"
                                        "MS Word .docx et LibreOffice .odt\n"
                                        "Adobe .pdf ou les trois formats "
                                        "en même temps"));

    enchainerRapports = new FCheckBox ("Enchaîner extraction et analyse",
                                       "enchainerRapports",
                                        {
                                            "Enchaînements",
                                            "Enchaîner l'extraction des données et la production des rapports"
                                        },
                                        {rapportTypeWidget, rapportTypeLabel});

    rapportEntier = new FCheckBox ("Version expérimentale",
                                   flags::status::enabledUnchecked
                                   | flags::commandLineType::noCommandLine
                                   | flags::status::excluded,  // exclu de la liste des widgets qui déclenche la ligne de commande
                                   //ou la l'importation/l'exportation de la valeur sur le projet XML.
                                   "rapportEntier",
                                    {
                                        "Version expérimentale",
                                        "Produire les rapports expérimentaux (EQTP et rémunérations)"
                                    });

    // La version expérimentale n'est accessible que sous compte administrateur

    if (QCoreApplication::applicationDirPath() != "/home/fab/Dev/altair/Interface_linux/gui/x64") rapportEntier->setVisible (false);

    connect (rapportEntier, &FCheckBox::toggled, [this]
    {

        const std::string &root = path_access (".").toStdString();
        int current_git_branch = system (std::string ("cd " + root + " && test \"$(git rev-parse --symbolic-full-name --abbrev-ref HEAD)\" = \"master-jf\"").c_str());
        int current_git_branch2 = 1;
        if (current_git_branch  != 0) current_git_branch2 = system (std::string ("cd " + root + " && test \"$(git rev-parse --symbolic-full-name --abbrev-ref HEAD)\" = \"dev\"").c_str());

        if (rapportEntier->isChecked())
            {
                if (current_git_branch == 0 || current_git_branch2 == 0)
                    {
                        // La branche courante est la branch Test
                        return;
                    }

                int res  = system (std::string ("cd " + root + " && git rev-parse --verify master-jf").c_str());
                
                int res2 = 1;
                
                if (res != 0) res2 = system (std::string ("cd " + root + " && git rev-parse --verify dev").c_str());

                if (res != 0 && res2 != 0)
                    {
                        Q ("La branche Test n'est pas déployée.")
                        return;
                    }

                Q ("Basculement vers la version Test.<br>Cela peut prendre une ou deux minutes.")
                if (res == 0)        
                    res = system (std::string ("cd " + root + " && git checkout -f master-jf").c_str());
                else // res2 = 0
                    res = system (std::string ("cd " + root + " && git checkout -f dev").c_str());

                if (res == 0)
                    {
                        Q ("Basculement réalisé.")
                    }
                else
                    {
                        Q ("Le basculement vers la version Test n'a pas pu être réalisé.")
                    }
            }
        else
            {
                int res = system (std::string ("cd " + root + " && git rev-parse --verify release").c_str());

                if (res != 0)
                    {
                        Q ("La version standard n'est pas déployée.")
                        return;
                    }

                Q ("Basculement vers la version standard.<br>Cela peut prendre une ou deux minutes.")
                res = system (std::string ("cd " + root + " && git checkout -f release").c_str());

                if (res == 0)
                    {
                        Q ("Basculement réalisé.")
                    }
                else
                    {
                        Q ("Le basculement vers la version standard n'a pas pu être réalisé.")
                    }
            }
    });
        
    openCheckBox = new FCheckBox("Ouvrir le document à la fin de l'exécution",
                                             flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                            "ouvrirDocFinExec",
                                            {"Rapports", "Ouvrir en fin d'exécution"});
       
    parallelCheckBox = new FCheckBox("Exécution en parallèle du rapport",
                                            flags::status::enabledChecked|flags::commandLineType::noCommandLine,
                                            "parallelExec",
                                            {"Rapports", "Exécution parallèle"});
    
                   
    for (const FCheckBox* a : {parallelCheckBox, openCheckBox})
    {
        
        connect(a, &FCheckBox::toggled, [this] {
            reinitialiser_prologue();
            file_str = common::readFile (prologue_options_path);
            substituer("séquentiel *<- *FALSE", QString("séquentiel <- ") + (parallelCheckBox->isChecked() ? "FALSE" : "TRUE"), file_str);
            substituer("ouvrir.document *<- *TRUE", QString("ouvrir.document <- ") + (openCheckBox->isChecked() ? "TRUE" : "FALSE"), file_str);
            renommer (dump (file_str), prologue_options_path);
            });
    }
            
    v4Layout->addWidget (enchainerRapports, 0, 0, Qt::AlignLeft);
    v4Layout->addWidget (rapportTypeLabel,  1, 0, Qt::AlignRight);
    v4Layout->addWidget (rapportTypeWidget, 1, 1, Qt::AlignLeft);
    v4Layout->addWidget (rapportEntier,     2, 0, Qt::AlignLeft);
    v4Layout->addWidget (openCheckBox,      3, 0, Qt::AlignLeft);
    v4Layout->addWidget (parallelCheckBox,  4, 0, Qt::AlignLeft);
    
    rapportBox->setLayout (v4Layout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel = new FRichLabel ("Paramètres de traitement");
    mainLayout->addWidget (mainLabel);
    mainLayout->addWidget (processTypeBox, 1, 0);
    mainLayout->addWidget (logBox, 2, 0);
    mainLayout->addWidget (rapportBox, 3, 0);
    mainLayout->addSpacing (150);

    setLayout (mainLayout);

    // provisoire
    const std::string &root = path_access (".").toStdString();
    int current_git_branch = system(std::string ("cd " + root + " && test \"$(git rev-parse --symbolic-full-name --abbrev-ref HEAD)\" = \"master-jf\"").c_str());
    
    int current_git_branch2 = 1;
    current_git_branch2 = system(std::string ("cd " + root + " && test \"$(git rev-parse --symbolic-full-name --abbrev-ref HEAD)\" = \"dev\"").c_str());
    
    rapportEntier->setChecked(current_git_branch == 0 || current_git_branch2  == 0);
        
    reinitialiser_prologue();
}

std::uint16_t options::RefreshFlag;


extraPage::extraPage()
{
    QGridLayout *v3Layout = new QGridLayout;
        
    budgetFrame = new FLineFrame ({"Utiliser la correspondance budgétaire", "Chemin de la table de correspondance"},
                                   QDir::toNativeSeparators (path_access(DONNEES_SORTIE  "/paye_budget.csv")),
                                   "budget",
                                   {2, 1},
                                   v3Layout,
                                   "",   // pas de ligne de commande
                                   directory::noCheck, // ne pas vérifier que le chemin est vide
                                   flags::flineframe::isFilePath,
                                   "Fichier CSV (*.csv)"); // il s'agit d'un chemin de fichier
    
    budgetFrame->setSaveFileName(false);
   
    constexpr const char* budgetTip = "Le fichier importé donne la correspondance entre <br>"
                                      "le code de paye et le sous-compte du compte 64 <br>"
                                      "utilisé par les comptes administratifs et de gestion.<br>"
                                      "Il doit être au format CSV (séparateur point-virgule)<br>"
                                      "et encodé en caractères Latin-1 ou Windows-1252.<br>"
                                      "Les colonnes doivent comporter les intitulés Code,<br>"
                                      "Libellé, Statut, Type et Compte, dans cet ordre : <br>"
                                      "<ul><li><b>Code</b> : code de  paye de la base dématérialisée.</li>"
                                      "<li><b>Libellé</b> : libellé de  paye associé au code.</li>"
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
                                      "point-virgule et encodé en Latin-1 ou Windows-1252.<br>"
                                      "Il doit comporter une ligne d'intitulés de colonnes.<br>"
                                      "La première colonne est intitulée <b>Grade</b> et la seconde <br>"
                                      "<b>Catégorie</b>.<br>"
                                      "A défaut d'importation manuelle de ce fichier, le logiciel<br>"
                                      "réalise une interpolation relativement exacte de la <br>"
                                      "correspondance entre grade et catégorie statutaire.<br>"
                                      "Ce fichier est généré sous le dossier <b>Bases/Effectifs</b><br>"
                                      "sous le nom de fichier <b>grades.categories.csv</b><br>";
            
            
    gradesBox->setLayout(v4Layout);
    gradesBox->setToolTip(gradesTip);
    
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
                                      "<li><b>Année</b> : année de la période sous revue.</li>"
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
        
    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel = new FRichLabel ("Fichiers externes");
    mainLayout->addWidget (mainLabel);
    mainLayout->addWidget (budgetBox, 1, 0);
    mainLayout->addWidget (gradesBox, 2, 0);
    mainLayout->addWidget (logtBox,   3, 0);
    mainLayout->addWidget (ifseBox,   4, 0);
    mainLayout->addSpacing (250);

    setLayout (mainLayout);
}


void extraPage::do_copies()
{
    short i = 0;
    FLineFrame* frameList[] = {budgetFrame, gradesFrame, logtFrame, ifseFrame};
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

options::options (Altair* parent)
{
    /* plain old data types must be 0-initialised even though the class instance was new-initialised. */

    options::RefreshFlag = interfaceStatus::optionTabs;

    optionWidget = new QListWidget;
    optionWidget->setViewMode (QListView::IconMode);
    optionWidget->setIconSize (QSize (48, 48));
    optionWidget->setFont (QFont ("Garamond", fontsize - 2));
    optionWidget->setMovement (QListView::Static);
    optionWidget->setFixedWidth (98);
    optionWidget->setSpacing (12);

    pagesWidget = new QStackedWidget;
    standardTab = new standardPage;
    processTab  = new processPage;
    
    pagesWidget->addWidget (standardTab);
    pagesWidget->addWidget (processTab);

    codeTab  = new codePage;
    pagesWidget->addWidget (codeTab);

    extraTab  = new extraPage();
    pagesWidget->addWidget (extraTab);
    
    rapportTab  = new rapportPage();
    pagesWidget->addWidget (rapportTab);
    
    closeButton = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    closeButton->button (QDialogButtonBox::Ok)->setText ("Accepter");
    closeButton->button (QDialogButtonBox::Cancel)->setText ("Annuler");

    connect (closeButton,
             &QDialogButtonBox::accepted,
             [this, parent]
                {
                    options::RefreshFlag =  interfaceStatus::hasUnsavedOptions;
                    accept();
                    parent->execPath = execPath;
                    parent->altairCommandStr =  parent->execPath +  QDir::separator()
                    + ("lhx" + QString (systemSuffix));
                    
                    parent->updateProject (true);
                    
                    extraTab->do_copies();

                });

    connect (closeButton, SIGNAL (rejected()), this, SLOT (reject()));
    connect (optionWidget,
             SIGNAL (currentItemChanged (QListWidgetItem*, QListWidgetItem*)),
             this, SLOT (changePage (QListWidgetItem*, QListWidgetItem*)));

    connect (standardTab->FPHCheckBox, SIGNAL (toggled (bool)), codeTab, SLOT (activer_fph (bool)));
    connect (standardTab->exportWidget, SIGNAL (currentIndexChanged (int)),
             this,
             SLOT (enchainerRapports (int)));

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


void options::createIcon (const char* path, const char* text)
{
    QListWidgetItem *button = new QListWidgetItem (optionWidget);
    QString strpath = QString (path);
    QString strtext = QString (text);
    button->setIcon (QIcon (strpath));
    button->setText (strtext);
    button->setTextAlignment (Qt::AlignRight);
    button->setFlags (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}


void options::createIcons()
{
    QList<const char*> iconList = QList<const char*>()
                                  << ":/images/csv.png" << "   Format  "
                                  << ":/images/configure-toolbars.png" << "Traitement "
                                  << ":/images/data-icon.png" << "   Codes   "
                                  << ":/images/extra.png" << "   Extra   "
                                  << ":/images/rapport.png" << "  Rapport  ";  

    for (int i = 0; i < iconList.size() / 2 ; ++i) createIcon (iconList[2 * i], iconList[2 * i + 1]);

}

void options::changePage (QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    int r;
    r = (current) ? optionWidget->row (current) : 0;

    if (current) pagesWidget->setCurrentIndex (r);
}





