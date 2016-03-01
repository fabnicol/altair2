
#include <QFile>

#include "altair.h"
#include "forms.h"
#include "options.h"
#include "browser.h"
#include "altair.h"
#include "templates.h"
#include "flineframe.hpp"

extern template void createHash(QHash<QString, QString>&, const QList<QString>*, const QList<QString>*);

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

    maxNLigneLabel = new QLabel("Nombre maximum de lignes\npar segment de base  ");
    maxNLigneLineEdit = new FLineEdit("",
                                      "maxLigne",
                                     {"Données csv", "Découper la base de données par segment d'au maximum ce nombre de lignes."},
                                      "T");

    QGroupBox *baseBox = new QGroupBox(tr("Répertoires"));
    donneesCSV = new FLineFrame({"Données csv", "Répertoire des données"},
                                  #ifdef __linux__
                                   path_access("Tests/Exemple/Donnees/R-Altair" ),
                                  #else
                                   path_access("Tests/Exemple/Donnees/R-Altaïr" ),
                                  #endif
                                   "base",
                                   {0,0},
                                   nullptr,
                                   "D");
    QLabel* baseTypeLabel = new QLabel("Type de base  ");
    baseTypeWidget = new FComboBox(range,
                                 "baseType",
                                 {"Données csv", "Type de base par catégorie"},
                                  "T");

    rangCheckBox = new FCheckBox("Numéroter les lignes",
                               "genererNumLigne",
                               {"Données csv", "numéroter les lignes"},
                               "l");

    etabCheckBox = new FCheckBox("Exporter les informations\nsur l'établissement",
                                flags::status::enabledUnchecked | flags::commandLineType::altairCommandLine,
                                "exporterEtab",
                                {"Données csv", "Exporter les champs Budget, Employeur, Siret, Etablissement"},
                                "S");

    echelonCheckBox = new FCheckBox("Exporter l'échelon",
                                flags::status::enabledUnchecked | flags::commandLineType::altairCommandLine,
                                "exporterEchelon",
                                {"Données csv", "Exporter les champ Echelon"},
                                "E");

    tableCheckBox = new FCheckBox("Créer la base de données",
                                flags::status::enabledChecked | flags::commandLineType::altairCommandLine,
                                "genererTable",
                                {"Données csv", "créer la base des lignes et bulletins de paye"},
                                "t",
                                {etabCheckBox, rangCheckBox, baseTypeLabel, baseTypeWidget, maxNLigneLabel, maxNLigneLineEdit, echelonCheckBox});

    applicationNoyau = new FLineFrame(
                {"Application noyau LHX", "Répertoire de l'application noyau" },
                path_access(System),
                "lhxDir",
                {2,0},
                donneesCSV->getLayout());

    baseBox->setLayout(applicationNoyau->getLayout());
    
    baseTypeBox = new QGroupBox(tr("Type de base en sortie"));
    processTypeBox = new QGroupBox(tr("Mode d'exécution"));

    QStringList range3 = QStringList();
    for (int i = 1; i < 12; i++) range3 << QString::number(i);
    
    createHash(baseTypeWidget->comboBoxTranslationHash, &range, &range2);
    baseTypeWidget->status = flags::status::defaultStatus;
    baseTypeWidget->commandLineType = flags::commandLineType::defaultCommandLine;

    baseTypeWidget->setFixedWidth(175);
    baseTypeWidget->setFixedHeight(30);
    baseTypeWidget->setCurrentIndex(0);
    baseTypeWidget->setToolTip(tr("Sélectionner le type de base en sortie"));

    maxNLigneLineEdit->setFixedWidth(60);
    
    QLabel* processTypeLabel = new QLabel("Nombre de fils d'exécution  ");
    processTypeWidget = new FComboBox(range3,
                                 "processType",
                                 {"Type de processus", "Nombre de fils d'exécution"},
                                  "j");

    processTypeWidget->setFixedWidth(45);
    processTypeWidget->setFixedHeight(30);
    processTypeWidget->setCurrentIndex(3);
    processTypeWidget->setToolTip(tr("Sélectionner le nombre de fils d'exécution"));

    nLineLabel = new QLabel("Nombre maximum d'agents par mois  ");
    nLineLabel->setDisabled(true);
    nLineEdit = new FLineEdit("",
                             flags::status::disabled | flags::commandLineType::defaultCommandLine,
                             "nAgent",
                            {"Nombre maximum d'agents", ""},
                            "n");

    nLineEdit->setFixedWidth(40);
    
    NLineLabel = new QLabel("Nombre maximum de lignes de paye par agent  ");
    NLineLabel->setDisabled(true);
    NLineEdit = new FLineEdit("",
                            flags::status::disabled | flags::commandLineType::defaultCommandLine,
                            "nLine",
                            {"Nombre maximum de ligne de paye par agent", ""},
                            "N");

    NLineEdit->setFixedWidth(40);

    QGridLayout *v1Layout = new QGridLayout;
    QGridLayout *v2Layout = new QGridLayout;

    logFrame = new FLineFrame({"Générer un log d'exécution", "Chemin du fichier du log"},
                              QDir::toNativeSeparators(common::generateDatadirPath() + "/altair.log"),
                              "log",
                              {9,1},
                              v2Layout,
                              "L");

    logFrame->setPathCategory(flags::flineframe::isFilePath);

    logCheckBox = new FCheckBox("Générer le log  ",
                                flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                "genererLog",
                                {"Générer un log d'exécution", "application noyau"},
                                logFrame->getComponentList());

    consoleCheckBox = new FCheckBox("Activer la console  ",
                                  flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                  "activerConsole",
                                  {"Générer un log d'exécution", "Utiliser l'onglet console"});


    QList<QString> ecoRange = QList<QString>(), ecoRange2 = QList<QString>();
    ecoRange << "Intensive" << "Standard" << "Modérée" << "Econome";
    ecoRange2 << "100"   << "90" << "80" << "50";

    QLabel* memoryUseLabel = new QLabel("Utilisation de la mémoire  ");

    /* Utiliser % devant l'option active la syntaxe `--option argument' plutôt que `--option=argument' */

    memoryUseWidget = new FComboBox(ecoRange,
                                 "memoryUse",
                                 {"Gestion de la mémoire", "Pourcentage d'utilisation de la mémoire libre"},
                                  "%memshare");

    createHash(memoryUseWidget->comboBoxTranslationHash, &ecoRange, &ecoRange2);
    memoryUseWidget->status = flags::status::defaultStatus;
    memoryUseWidget->commandLineType = flags::commandLineType::defaultCommandLine;

    memoryUseWidget->setFixedWidth(175);
    memoryUseWidget->setFixedHeight(30);
    memoryUseWidget->setCurrentIndex(1);
    memoryUseWidget->setToolTip(tr("Sélectionner l'intensité de l'utilisation de la mémoire vive (RAM) libre au lancement de l'application. "
                    "\nIntensive\t: 100 % de la mémoire\n"
                      "Standard\t: 90 %\n"
                      "Modérée\t: 80 %\n"
                      "Econome\t: 50 %"));


    /* A ce stade seules les bases monolithiques et par année peuvent être sous découpées en segments d'au maximum N lignes
     * Les autres types de base doivent donc désactiver la FLineEdit maxNLigneLabel.
     * Le code présuppose que les types de base sont rangés dans l'ordre : Standard > Par année > autres types.
     * Code à désactiver lorsque cette fonctionnalités sera étendue aux autres types. */

    connect(baseTypeWidget, &FComboBox::currentTextChanged, [this] {
           bool value = (baseTypeWidget->currentIndex() > 1);
           maxNLigneLabel->setDisabled(value);
           maxNLigneLineEdit->setDisabled(value);
        });

        connect(memoryUseWidget, &FComboBox::currentTextChanged, [this] {
               bool value = (memoryUseWidget->currentIndex() > 0);
                nLineLabel->setDisabled(value);
                NLineLabel->setDisabled(value);
                nLineEdit->setDisabled(value);
                NLineEdit->setDisabled(value);
            });

    v1Layout->addWidget(tableCheckBox,     1, 0, Qt::AlignLeft);
    v1Layout->addWidget(baseTypeLabel,     3, 0, Qt::AlignRight);
    v1Layout->addWidget(baseTypeWidget,    3, 1, Qt::AlignLeft);
    v1Layout->addWidget(maxNLigneLabel,    4, 0, Qt::AlignRight);
    v1Layout->addWidget(maxNLigneLineEdit, 4, 1, Qt::AlignLeft);
    v1Layout->addWidget(rangCheckBox,      5, 0, Qt::AlignLeft);
    v1Layout->addWidget(etabCheckBox,      6, 0, Qt::AlignLeft);
    v1Layout->addWidget(echelonCheckBox,   7, 0, Qt::AlignLeft);

    v2Layout->addWidget(memoryUseLabel,    3, 0, Qt::AlignRight);
    v2Layout->addWidget(memoryUseWidget,   3, 1, Qt::AlignLeft);
    v2Layout->addWidget(nLineEdit,         4, 1, Qt::AlignLeft);
    v2Layout->addWidget(nLineLabel,        4, 0, Qt::AlignRight);
    v2Layout->addWidget(NLineEdit,         5, 1, Qt::AlignLeft);
    v2Layout->addWidget(NLineLabel,        5, 0, Qt::AlignRight);
    v2Layout->addWidget(processTypeLabel,  6, 0, Qt::AlignRight);
    v2Layout->addWidget(processTypeWidget, 6, 1, Qt::AlignLeft);
    v2Layout->addWidget(consoleCheckBox,   7, 0, Qt::AlignLeft);
    v2Layout->addWidget(logCheckBox,       8, 0, Qt::AlignLeft);

    baseTypeBox->setLayout(v1Layout);
    processTypeBox->setLayout(v2Layout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel=new FRichLabel("Paramètres de sortie", ":/images/encode.png");
    mainLayout->addWidget(mainLabel);
    mainLayout->addWidget(baseBox, 1, 0);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(baseTypeBox, 3, 0);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(processTypeBox, 5, 0);

    setLayout(mainLayout);
}


std::uint16_t options::RefreshFlag;

options::options(Altair* parent)
{
    /* plain old data types must be 0-initialised even though the class instance was new-initialised. */
        
    options::RefreshFlag = interfaceStatus::optionTabs;

    pagesWidget = new QStackedWidget;
    standardTab = new standardPage;
    pagesWidget->addWidget(standardTab);

    closeButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    closeButton->button(QDialogButtonBox::Ok)->setText("Accepter");
    closeButton->button(QDialogButtonBox::Cancel)->setText("Annuler");
    
    connect(closeButton,
            &QDialogButtonBox::accepted,
            [this,parent]
            {
                options::RefreshFlag =  interfaceStatus::hasUnsavedOptions;
                accept();
                parent->execPath = standardTab->applicationNoyau->getText();
                parent->altairCommandStr =  parent->execPath +  QDir::separator() + ("lhx"+ QString(systemSuffix));

                parent->updateProject(true);

            });

    connect(closeButton, SIGNAL(rejected()), this, SLOT(reject()));
        
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
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


/* implement a global clear() function for the FStringList of data in an FListFrame ; it will be used as Altair::clearData() too. Usage below is faulty. */

void options::clearOptionData()
{
    Hash::wrapper.clear();
    Hash::Reference.clear();
  
    options::RefreshFlag = interfaceStatus::optionTabs;
}




