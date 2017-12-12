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
#include "fstring.h"
#include "altair.h"
#include "forms.h"
#include "options.h"
#include "altair.h"
#include "templates.h"
#include "flineframe.h"

extern int fontsize;
extern template void createHash(QHash<QString, QString>&,
                                const QList<QString>*,
                                const QList<QString>*);

//
// Crée une ligne de codes pour un type donné de prime
//

int codePage::ajouterVariable(const QString& nom)
{
    const QString NOM = nom.toUpper();

    // ligne de codes de primes nom

    FLineEdit *line = new FLineEdit("",
                                    QString(NOM).remove(' '),
    {NOM, "Code de paye"});


    line->setMinimumWidth(MINIMUM_LINE_WIDTH);

    // Ajouter ici les FLineEdit en pile dans listeCodes

    listeCodes << line;

    // Les labels sont du type Codes NOM
    // Ajouter ici les labels correspondants dans liste Labels
    // Les labels doivent correspondre à une variable chaîne nommée codes.label dans prologue_codes.R

    listeLabels << QString(nom).remove(' ');

    listeDialogueLabels << new QLabel("Codes " + NOM + " :  ");

    vLayout->addWidget(listeDialogueLabels.last(), listeDialogueLabels.size() - 1, 0, Qt::AlignRight);

    vLayout->addWidget(listeCodes.last(), listeCodes.size() - 1, 1, Qt::AlignLeft);

    return listeCodes.size();
}


codePage::codePage()
{
    baseBox = new QGroupBox;
    prologue_codes_path = path_access(SCRIPT_DIR "prologue_codes.R");
    appliquerCodes = new QToolButton;

    appliquerCodes->setIcon(QIcon(":/images/view-refresh.png"));
    appliquerCodes->setToolTip("Appuyer pour exporter ces codes de paye<br>    "
                               "pour la génération des rapports d'analyse.   ");
    appliquerCodes->setCheckable(true);

    variables << "traitement" << "nbi" << "pfi" <<
              "pfr" <<  "ipf" << "psr" << "ifts" << "iat" <<
              "ihts" << "vacataires" << "astreintes";

    short index = 0;

    for (const QString& s : variables) index = ajouterVariable(s);

    label = new QLabel;

    vLayout->addWidget(label, index + 1, 1, Qt::AlignLeft);
    vLayout->addWidget(appliquerCodes, index, 1, Qt::AlignLeft);
    vLayout->setColumnMinimumWidth(1, MINIMUM_LINE_WIDTH);
    vLayout->setSpacing(10);

    baseBox->setLayout(vLayout);

    FRichLabel *mainLabel = new FRichLabel("Code de paye des tests");
    mainLayout->addWidget(mainLabel);
    mainLayout->addWidget(baseBox, 1, 0);
    mainLayout->addSpacing(100);

    init_label_text = "Appuyer pour exporter<br> vers les rapports d'analyse ";
    label->setText(init_label_text);

    connect(appliquerCodes, SIGNAL(clicked()), this, SLOT(substituer_valeurs_dans_script_R()));

    for (FLineEdit *a : listeCodes)
        {
            connect(a,
                    &QLineEdit::textEdited,
                    [this]
            {
                label->setText(init_label_text);
                appliquerCodes->setChecked(false);
                appliquerCodes->setIcon(QIcon(":/images/view-refresh.png"));
                reinitialiser_prologue();
            });
        }

    setLayout(mainLayout);

    reinitialiser_prologue();
}

inline const QString regexp(const QString& X)
{
    return "codes." + X + " *<- *NA";
}

inline QString rempl_str(const QString &X, const QString &Y)
{
    QStringList L = Y.split(";", QString::SkipEmptyParts);
    QString Z;

    if (L.size() >= 0)
        {
            for (QString& s : L)
                {
                    s = "\"" + s + "\"";
                }

            Z = "c(" + L.join(",") + ")";
        }
    else
        return   "codes." + X + " <- NA";

    return   "codes." + X + " <- " + Z;
}

void codePage::activer_fph(bool activer)
{
    const QStringList variables_fph = {"prime specifique", "prime de service", "prime de technicite", "ift"};

    if (listeCodes.size() > variables.size())
        {
            for (int i = variables.size(); i < listeCodes.size() ; ++i)
                {
                    listeCodes[i]->setVisible(activer);
                    listeDialogueLabels[i]->setVisible(activer);
                }

            repaint();
            return;
        }

    if (! activer) return;

    vLayout->removeWidget(label);
    vLayout->removeWidget(appliquerCodes);

    for (const QString &s : variables_fph) ajouterVariable(s);

    short index = variables.size() + variables_fph.size();

    vLayout->addWidget(label, index + 1, 1, Qt::AlignLeft);
    vLayout->addWidget(appliquerCodes, index, 1, Qt::AlignLeft);

}

void codePage::substituer_valeurs_dans_script_R()
{
    reinitialiser_prologue();
    QString file_str = common::readFile(prologue_codes_path);
    common::exporter_identification_controle(file_str);
    bool res = false;
    bool res2 = true;

    QIcon icon0 = QIcon(":/images/view-refresh.png");
    QIcon icon1 = QIcon(":/images/msg.png");
    QIcon icon2 = QIcon(":/images/error.png");

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
            Warning("Attention",
                    "Les codes sont tous non renseignés.<br>"
                    "Les tests statutaires se feront <br>"
                    "sous algorithme heuristique seulement.");

            icon = icon2;

            res = renommer(dump(file_str), prologue_codes_path);

            if (Hash::Reference.isEmpty()) return;

            if (res == true)
                label->setText("L'identification du contrôle a été exportée dans le rapport.");
            else
                label->setText("Erreur d'enregistrement du fichier de configuration prologue_codes.R");

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

            Warning("Attention",
                    "Certains code ne sont pas renseignés.<br>"
                    "Les tests statutaires se feront <br>"
                    "sous algorithme heuristique pour :<br>"
                    +
                    liste_codes_nr);
        }

    QString liste_codes;

    for (int rang = 0; rang < listeCodes.size(); ++rang)
        {
            const QString &s     = listeLabels.at(rang);
            const QString &codes = listeCodes.at(rang)->text();
            res = true;

            if (! codes.isEmpty())
                {
                    res = substituer(regexp(s), rempl_str(s, codes), file_str);
                    liste_codes += "<li>" + listeLabels.at(rang).toUpper() + " : "  + codes + "</li>";
                }

            if (res == false)
                {
                    Warning("Attention",
                            "Le remplacement de la variable codes." + s + "<br>"
                            "n'a pas pu être effectué dans le fichier prologue_codes.R<br>"
                            "Les tests statutaires se feront<br>"
                            "sous algorithme heuristique seulement.");

                    icon = icon2;
                }
        }

    res = renommer(dump(file_str), prologue_codes_path);

    appliquerCodes->setIcon(icon);

    if (res == true)
        label->setText("Les codes de paye suivants :"
                       "<ul>" + liste_codes + "</ul>"
                       "seront  pris en compte pour les rapports.");
    else
        label->setText("Erreur d'enregistrement du fichier de configuration prologue_codes.R");
}


bool codePage::reinitialiser_prologue()
{

    QFile(prologue_codes_path).remove();
    bool result = QFile(path_access(SCRIPT_DIR "prologue_init.R")).copy(prologue_codes_path);

    return result;
}

standardPage::standardPage()
{
    QList<QString> range = QList<QString>(), range2 = QList<QString>();

    range << "Standard" << "Par année" << "Toutes catégories" << "Traitement" << "Indemnité"
          << "SFT"      << "Rémunérations diverses"           << "Rappel"     << "Acompte"
          << "Avantage en nature" << "Indemnité de résidence" << "Cotisations"
          << "Déductions"         << "Retenue";

    range2 << ""   << "AN" << "X" << "T" << "I"
           << "S"  << "A"  << "R" << "AC"
           << "AV" << "IR" << "C"
           << "D"  << "RE";

    QLabel* baseTypeLabel = new QLabel("Type de base  ");
    baseTypeBox = new QGroupBox(tr("Type de base en sortie"));
    baseTypeWidget = new FComboBox(range,
                                   "baseType",
    {"Données csv", "Type de base par catégorie"},
    "T");


    maxNLigneLabel = new QLabel("Nombre maximum de lignes\npar segment de base  ");

    maxNLigneLineEdit = new FLineEdit("1000000",
                                      "maxLigne",
    {
        "Données csv",
        "Découper la base de données"
        "par segment d'au maximum ce nombre de lignes."
    },
    "T");

    maxNLigneLineEdit->setFixedWidth(150);

    QGroupBox* optionalFieldBox = new QGroupBox(tr("Variables optionnelles"));

    rangCheckBox = new FCheckBox("Numéroter les lignes",
                                 "genererNumLigne",
    {"Données csv", "numéroter les lignes"},
    "l");

    etabCheckBox = new FCheckBox("Exporter les informations\nsur l'établissement",
                                 flags::status::enabledUnchecked
                                 | flags::commandLineType::altairCommandLine,
                                 "exporterEtab",
    {
        "Données csv",
        "Exporter les champs Budget,"
        " Employeur, Siret, Etablissement"
    },
    "S");


    QList<QString> exportRange = QList<QString>();
    exportRange << "Standard" << "Cumulative" << "Distributive" << "Distributive+";

    QLabel* exportLabel = new QLabel("Exportation  ");

    /* Utiliser % devant l'option active la syntaxe `--option argument' plutôt que `--option=argument' */

    exportWidget = new FComboBox(exportRange,
                                 "exportMode",
    {
        "Données csv",
        "Mode d'exportation"
    },
    "%export");

    exportWidget->status = flags::status::defaultStatus;
    exportWidget->commandLineType = flags::commandLineType::defaultCommandLine;

    exportWidget->setFixedWidth(175);
    exportWidget->setFixedHeight(30);
    exportWidget->setCurrentIndex(0);
    exportWidget->setToolTip(tr("Sélectionner la modalité d'exportation"
                                " des bases de données CSV."
                                "\nStardard : A chaque contrôle, les bases précédemment\n\texportées sur clé sont effacées.\n"
                                "Cumulative : A chaque contrôle, les bases s'empilent\n\tà la fin des bases précédemment exportées.\n"
                                "Distributive : A chaque contrôle, un sous répertoire est créé\n\tsur la clé du nom de chaque dossier\n\timporté du répertoire Données.\n"
                                "Distributive+ : Exportation Distributive et Cumulative activées.\n"));

    QGridLayout *v1Layout = new QGridLayout,
    *v2Layout = new QGridLayout,
    *v3Layout = new QGridLayout,
    *v4Layout = new QGridLayout;

    v2Layout->addWidget(rangCheckBox,     0, 0, Qt::AlignLeft);
    v2Layout->addWidget(etabCheckBox,     1, 0, Qt::AlignLeft);
    v2Layout->setColumnMinimumWidth(1, 250);
    v3Layout->setColumnMinimumWidth(1, 250);

    optionalFieldBox->setLayout(v2Layout);



    FPHCheckBox = new FCheckBox("Fonction publique hospitalière",
                                flags::status::enabledUnchecked
                                | flags::commandLineType::noCommandLine,
                                "FPH",
    {
        "Contrôle hospitalier",
        "Ajuster le rapport pour la FPH"
    }
                               );

    tableCheckBox = new FCheckBox("Créer la base de données",
                                  flags::status::enabledChecked
                                  | flags::commandLineType::altairCommandLine,
                                  "genererTable",
    {
        "Données csv",
        "créer la base des lignes et bulletins de paye"
    },
    "t",
    {
        optionalFieldBox, baseTypeLabel, baseTypeWidget,
        maxNLigneLabel, maxNLigneLineEdit, FPHCheckBox, exportWidget, exportLabel
    });

    QStringList range3 = QStringList();

    for (int i = 1; i < 12; i++) range3 << QString::number(i);

    createHash(baseTypeWidget->comboBoxTranslationHash, &range, &range2);
    baseTypeWidget->status = flags::status::defaultStatus;
    baseTypeWidget->commandLineType = flags::commandLineType::defaultCommandLine;
    baseTypeWidget->setFixedWidth(175);
    baseTypeWidget->setFixedHeight(30);
    baseTypeWidget->setCurrentIndex(0);
    baseTypeWidget->setToolTip(tr("Sélectionner le type de base en sortie"));

    // A ce stade seules les bases monolithiques et par année peuvent être sous découpées en segments d'au maximum N lignes
    // Les autres types de base doivent donc désactiver la FLineEdit maxNLigneLabel.
    // Le code présuppose que les types de base sont rangés dans l'ordre : Standard > Par année > autres types.
    // Code à désactiver lorsque cette fonctionnalités sera étendue aux autres types.

    connect(baseTypeWidget, &FComboBox::currentTextChanged, [this]
    {
        bool value = (baseTypeWidget->currentIndex() > 1);
        maxNLigneLabel->setDisabled(value);
        maxNLigneLineEdit->setDisabled(value);
    });

    connect(FPHCheckBox, SIGNAL(toggled(bool)), this, SLOT(substituer_versant()));


    QGroupBox* archBox = new QGroupBox(tr("Archivage et Restauration"));
    QGroupBox* exportBox = new QGroupBox(tr("Exportation"));

    FCheckBox* archiveTableBox = new FCheckBox("Données tableur",
            "archiveTable",
    {"Données csv", "Archiver/Restaurer les données CSV"});


    FCheckBox* exportTableBox  = new FCheckBox("Données tableur",
            "exportTable",
    {"Données csv", "Exporter les données CSV"});

    FCheckBox* archiveAllBox = new FCheckBox("Tout",
            "archiveAll",
    {"Données XML", "Archiver/Restaurer les données tableur et XML"});

    FCheckBox* exportAllBox  = new FCheckBox("Tout",
            "exportAll",
    {"Données XML", "Exporter les données tableur et XML"});

    FCheckBox* archiveXhlBox = new FCheckBox("Bases XML",
            "archiveXML",
    {"Données XML", "Archiver/Restaurer les bases XML"});

    FCheckBox* exportXhlBox  = new FCheckBox("Bases XML",
            "exportXML",
    {"Données XML", "Exporter les bases XML"});

    v1Layout->addWidget(tableCheckBox,     1, 0, Qt::AlignLeft);
    v1Layout->addWidget(FPHCheckBox,       2, 0, Qt::AlignLeft);
    v1Layout->addWidget(baseTypeLabel,     3, 0, Qt::AlignRight);
    v1Layout->addWidget(baseTypeWidget,    3, 1, Qt::AlignLeft);
    v1Layout->addWidget(exportLabel,       4, 0, Qt::AlignRight);
    v1Layout->addWidget(exportWidget,      4, 1, Qt::AlignLeft);
    v1Layout->addWidget(maxNLigneLabel,    5, 0, Qt::AlignRight);
    v1Layout->addWidget(maxNLigneLineEdit, 5, 1, Qt::AlignLeft);

    baseTypeBox->setLayout(v1Layout);

    v3Layout->addWidget(exportTableBox,      1, 0, Qt::AlignLeft);
    v3Layout->addWidget(exportAllBox,  1, 1, Qt::AlignCenter);
    v3Layout->addWidget(exportXhlBox,  2, 0, Qt::AlignLeft);

    exportBox->setLayout(v3Layout);

    v4Layout->addWidget(archiveTableBox,     1, 0, Qt::AlignLeft);
    v4Layout->addWidget(archiveAllBox, 1, 1, Qt::AlignCenter);
    v4Layout->addWidget(archiveXhlBox, 2, 0, Qt::AlignLeft);

    archBox->setLayout(v4Layout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel = new FRichLabel("Format des bases");
    mainLayout->addWidget(mainLabel);

    mainLayout->addWidget(baseTypeBox,      1, 0);
    mainLayout->addWidget(optionalFieldBox, 2, 0);
    mainLayout->addWidget(exportBox,   3, 0);
    mainLayout->addWidget(archBox,   4, 0);
    // mainLayout->addSpacing(100);

    setLayout(mainLayout);
    substituer_versant();
}



void standardPage::substituer_versant()
{

    const QString &versant_path = path_access(SCRIPT_DIR "versant.R");

    QString file_str = readFile(versant_path);


    if (FPHCheckBox->isChecked())
        substituer("VERSANT_FP <<- .*", "VERSANT_FP <<- \"FPH\"", file_str);
    else
        substituer("VERSANT_FP <<- .*", "VERSANT_FP <<- \"FPT\"", file_str);

    bool res = renommer(dump(file_str), versant_path);

    if (! res) Q("Le versant de la fonction publique n'a pas pu être exporté.")

    }

processPage::processPage()
{
    processTypeBox = new QGroupBox(tr("Mode d'exécution"));

    QStringList range3 = QStringList();

    for (int i = 1; i < 12; i++) range3 << QString::number(i);


    QLabel* processTypeLabel = new QLabel("Nombre de fils d'exécution  ");
    processTypeWidget = new FComboBox(range3,
                                      "processType",
    {"Type de processus", "Nombre de fils d'exécution"},
    "j");

    processTypeWidget->setFixedWidth(45);
    processTypeWidget->setFixedHeight(30);
    processTypeWidget->setCurrentIndex(std::thread::hardware_concurrency() / 2 + 1);
    processTypeWidget->setToolTip(tr("Sélectionner le nombre de fils d'exécution "));

    QGridLayout *v3Layout = new QGridLayout;
    logFrame = new FLineFrame({"Générer un log d'exécution", "Chemin du fichier du log"},
                              QDir::toNativeSeparators(common::generateDatadirPath()
                                      + "/altair.log"),
                              "log",
    {2, 1},
    v3Layout,
    "L");

    logFrame->setPathCategory(flags::flineframe::isFilePath);

    logCheckBox = new FCheckBox("Générer le log  ",
                                flags::status::enabledUnchecked
                                | flags::commandLineType::noCommandLine,
                                "genererLog",
    {"Générer un log d'exécution", "application noyau"},
    logFrame->getComponentList());

    consoleCheckBox = new FCheckBox("Activer la console  ",
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

    QLabel* memoryUseLabel = new QLabel("Utilisation de la mémoire  ");

    /* Utiliser % devant l'option active la syntaxe `--option argument' plutôt que `--option=argument' */

    memoryUseWidget = new FComboBox(ecoRange,
                                    "memoryUse",
    {
        "Gestion de la mémoire",
        "Pourcentage d'utilisation de la mémoire libre"
    },
    "%memshare");


    createHash(memoryUseWidget->comboBoxTranslationHash, &ecoRange, &ecoRange2);
    memoryUseWidget->status = flags::status::defaultStatus;
    memoryUseWidget->commandLineType = flags::commandLineType::defaultCommandLine;

    memoryUseWidget->setFixedWidth(175);
    memoryUseWidget->setFixedHeight(30);
    memoryUseWidget->setCurrentIndex(1);
    memoryUseWidget->setToolTip(tr("Sélectionner l'intensité de l'utilisation"
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
    v2Layout->addWidget(memoryUseLabel,    3, 0, Qt::AlignRight);
    v2Layout->addWidget(memoryUseWidget,   3, 1, Qt::AlignLeft);
    v2Layout->addWidget(processTypeLabel,  6, 0, Qt::AlignRight);
    v2Layout->addWidget(processTypeWidget, 6, 1, Qt::AlignLeft);
    v2Layout->addWidget(consoleCheckBox,   7, 0, Qt::AlignLeft);

    processTypeBox->setLayout(v2Layout);

    v3Layout->addWidget(logCheckBox,       1, 0, Qt::AlignLeft);

    QGroupBox* logBox = new QGroupBox(tr("Log"));
    logBox->setLayout(v3Layout);

    QGridLayout *v4Layout = new QGridLayout;
    QGroupBox* rapportBox = new QGroupBox(tr("Rapports"));


    QLabel* rapportTypeLabel = new QLabel("Type de rapport produit par défaut  ");
    rapportTypeWidget = new FComboBox((QStringList() << "WORD et ODT" << "PDF" << "WORD, ODT et PDF"),
                                      "rapportType",
    {
        "Enchaînements",
        "Type de rapport"
    });

    rapportTypeWidget ->setToolTip(tr("Sélectionner le type de rapport produit \nen cas d'enchaînement automatique extraction-rapport :\n"
                                      "MS Word .docx et LibreOffice .odt\n"
                                      "Adobe .pdf ou les trois formats "
                                      "en même temps"));

    enchainerRapports = new FCheckBox("Enchaîner extraction et analyse",
                                      "enchainerRapports",
    {
        "Enchaînements",
        "Enchaîner l'extraction des données et la production des rapports"
    },
    {rapportTypeWidget, rapportTypeLabel});

    rapportEntier = new FCheckBox("Version expérimentale",
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

    if (QCoreApplication::applicationDirPath() != "/home/fab/Dev/altair/Interface_linux/gui/x64") rapportEntier->setVisible(false);


    connect(rapportEntier, &FCheckBox::toggled, [this]
    {

        const std::string &root = path_access(".").toStdString();
        int current_git_branch = system(std::string("cd " + root + " && test \"$(git rev-parse --symbolic-full-name --abbrev-ref HEAD)\" = \"master-jf\"").c_str());

        if (rapportEntier->isChecked())
            {
                if (current_git_branch == 0)
                    {
                        // La branche courante est la branch Test
                        return;
                    }

                int res = system(std::string("cd " + root + " && git rev-parse --verify master-jf").c_str());

                if (res != 0)
                    {
                        Q("La branche Test n'est pas déployée.")
                        return;
                    }

                Q("Basculement vers la version Test.<br>Cela peut prendre une ou deux minutes.")
                res = system(std::string("cd " + root + " && git checkout -f master-jf").c_str());

                if (res == 0)
                    {
                        Q("Basculement réalisé.")
                    }
                else
                    {
                        Q("Le basculement vers la version Test n'a pas pu être réalisé.")
                    }
            }
        else
            {
                int res = system(std::string("cd " + root + " && git rev-parse --verify release").c_str());

                if (res != 0)
                    {
                        Q("La version standard n'est pas déployée.")
                        return;
                    }

                Q("Basculement vers la version standard.<br>Cela peut prendre une ou deux minutes.")
                res = system(std::string("cd " + root + " && git checkout -f release").c_str());

                if (res == 0)
                    {
                        Q("Basculement réalisé.")
                    }
                else
                    {
                        Q("Le basculement vers la version standard n'a pas pu être réalisé.")
                    }
            }
    });


    v4Layout->addWidget(enchainerRapports, 0, 0, Qt::AlignLeft);
    v4Layout->addWidget(rapportTypeLabel,  1, 0, Qt::AlignRight);
    v4Layout->addWidget(rapportTypeWidget, 1, 1, Qt::AlignLeft);
    v4Layout->addWidget(rapportEntier, 2, 0, Qt::AlignLeft);
    rapportBox->setLayout(v4Layout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel = new FRichLabel("Paramètres de traitement");
    mainLayout->addWidget(mainLabel);
    mainLayout->addWidget(processTypeBox, 1, 0);
    mainLayout->addWidget(logBox, 2, 0);
    mainLayout->addWidget(rapportBox, 3, 0);
    mainLayout->addSpacing(150);

    setLayout(mainLayout);

    // provisoire
    const std::string &root = path_access(".").toStdString();
    int current_git_branch = system(std::string("cd " + root + " && test \"$(git rev-parse --symbolic-full-name --abbrev-ref HEAD)\" = \"master-jf\"").c_str());
    rapportEntier->setChecked(current_git_branch == 0);
}

std::uint16_t options::RefreshFlag;

options::options(Altair* parent)
{
    /* plain old data types must be 0-initialised even though the class instance was new-initialised. */

    options::RefreshFlag = interfaceStatus::optionTabs;

    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(48, 48));
    contentsWidget->setFont(QFont("Garamond", fontsize - 2));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setFixedWidth(98);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    standardTab = new standardPage;
    processTab  = new processPage;
    pagesWidget->addWidget(standardTab);
    pagesWidget->addWidget(processTab);

    codeTab  = new codePage;
    pagesWidget->addWidget(codeTab);

    closeButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    closeButton->button(QDialogButtonBox::Ok)->setText("Accepter");
    closeButton->button(QDialogButtonBox::Cancel)->setText("Annuler");

    connect(closeButton,
            &QDialogButtonBox::accepted,
            [this, parent]
    {
        options::RefreshFlag =  interfaceStatus::hasUnsavedOptions;
        accept();
        parent->execPath = execPath;
        parent->altairCommandStr =  parent->execPath +  QDir::separator()
        + ("lhx" + QString(systemSuffix));

        parent->updateProject(true);

    });

    connect(closeButton, SIGNAL(rejected()), this, SLOT(reject()));
    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*, QListWidgetItem*)));

    connect(standardTab->FPHCheckBox, SIGNAL(toggled(bool)), codeTab, SLOT(activer_fph(bool)));
    connect(standardTab->exportWidget, SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(enchainerRapports(int)));


    createIcons();
    contentsWidget->setCurrentRow(0);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("Options"));
    setWindowIcon(QIcon(":/images/altair.png"));
}

void options::enchainerRapports(int index)
{
    processTab->enchainerRapports->setChecked(index > 1);
}

// implement a global clear() function for the FStringList of data in an FListFrame ;
// it will be used as Altair::clearData() too. Usage below is faulty.

void options::clearOptionData()
{
    Hash::wrapper.clear();
    Hash::Reference.clear();

    options::RefreshFlag = interfaceStatus::optionTabs;
}


void options::createIcon(const char* path, const char* text)
{
    QListWidgetItem *button = new QListWidgetItem(contentsWidget);
    QString strpath = QString(path);
    QString strtext = QString(text);
    button->setIcon(QIcon(strpath));
    button->setText(strtext);
    button->setTextAlignment(Qt::AlignRight);
    button->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}


void options::createIcons()
{
    QList<const char*> iconList = QList<const char*>()
                                  << ":/images/csv.png" << "   Format  "
                                  << ":/images/configure-toolbars.png" << "Traitement "
                                  << ":/images/data-icon.png" << "   Codes   ";

    for (int i = 0; i < iconList.size() / 2 ; i++) createIcon(iconList[2 * i], iconList[2 * i + 1]);

}

void options::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    int r;
    r = (current) ? contentsWidget->row(current) : 0;

    if (current) pagesWidget->setCurrentIndex(r);
}





