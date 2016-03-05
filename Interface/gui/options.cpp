
#include <QFile>

#include "altair.h"
#include "forms.h"
#include "options.h"
#include "browser.h"
#include "altair.h"
#include "templates.h"
#include "flineframe.hpp"

extern template void createHash(QHash<QString, QString>&, const QList<QString>*, const QList<QString>*);


dirPage::dirPage()
{
    QGroupBox *baseBox = new QGroupBox;


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


    applicationNoyau = new FLineFrame(
                {"Application noyau LHX", "Répertoire de l'application noyau" },
                path_access(System),
                "lhxDir",
                {2,0},
                donneesCSV->getLayout());

    baseBox->setLayout(applicationNoyau->getLayout());

    //QGridLayout *v1Layout = new QGridLayout;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel=new FRichLabel("Répertoires des applications", ":/images/directory.png");
    mainLayout->addWidget(mainLabel);
    mainLayout->addWidget(baseBox, 1, 0);
    mainLayout->addSpacing(100);

    setLayout(mainLayout);
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
    maxNLigneLineEdit = new FLineEdit("",
                                      "maxLigne",
                                     {"Données csv", "Découper la base de données par segment d'au maximum ce nombre de lignes."},
                                      "T");

    QGroupBox* optionalFieldBox = new QGroupBox(tr("Variables optionnelles"));

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

    QGridLayout *v1Layout = new QGridLayout, *v2Layout = new QGridLayout;

    v2Layout->addWidget(rangCheckBox,      0, 0, Qt::AlignLeft);
    v2Layout->addWidget(etabCheckBox,      1, 0, Qt::AlignLeft);
    v2Layout->addWidget(echelonCheckBox,   2, 0, Qt::AlignLeft);
    v2Layout->setColumnMinimumWidth(1, 250);

    optionalFieldBox->setLayout(v2Layout);

    tableCheckBox = new FCheckBox("Créer la base de données",
                                flags::status::enabledChecked | flags::commandLineType::altairCommandLine,
                                "genererTable",
                                {"Données csv", "créer la base des lignes et bulletins de paye"},
                                "t",
                                {optionalFieldBox, baseTypeLabel, baseTypeWidget, maxNLigneLabel, maxNLigneLineEdit});


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



    /* A ce stade seules les bases monolithiques et par année peuvent être sous découpées en segments d'au maximum N lignes
     * Les autres types de base doivent donc désactiver la FLineEdit maxNLigneLabel.
     * Le code présuppose que les types de base sont rangés dans l'ordre : Standard > Par année > autres types.
     * Code à désactiver lorsque cette fonctionnalités sera étendue aux autres types. */

    connect(baseTypeWidget, &FComboBox::currentTextChanged, [this] {
           bool value = (baseTypeWidget->currentIndex() > 1);
           maxNLigneLabel->setDisabled(value);
           maxNLigneLineEdit->setDisabled(value);
        });

    v1Layout->addWidget(tableCheckBox,     1, 0, Qt::AlignLeft);
    v1Layout->addWidget(baseTypeLabel,     3, 0, Qt::AlignRight);
    v1Layout->addWidget(baseTypeWidget,    3, 1, Qt::AlignLeft);
    v1Layout->addWidget(maxNLigneLabel,    4, 0, Qt::AlignRight);
    v1Layout->addWidget(maxNLigneLineEdit, 4, 1, Qt::AlignLeft);

    baseTypeBox->setLayout(v1Layout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel=new FRichLabel("Format des bases", ":/images/csv.png");
    mainLayout->addWidget(mainLabel);

    mainLayout->addWidget(baseTypeBox, 1, 0);
    mainLayout->addWidget(optionalFieldBox, 2, 0);

    setLayout(mainLayout);
}


processPage::processPage()
{

    processTypeBox = new QGroupBox(tr("Mode d'exécution"));

    QStringList range3 = QStringList();
    for (int i = 1; i < 12; i++) range3 << QString::number(i);

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

    QLabel* processTypeLabel = new QLabel("Nombre de fils d'exécution  ");
    processTypeWidget = new FComboBox(range3,
                                 "processType",
                                 {"Type de processus", "Nombre de fils d'exécution"},
                                  "j");

    processTypeWidget->setFixedWidth(45);
    processTypeWidget->setFixedHeight(30);
    processTypeWidget->setCurrentIndex(3);
    processTypeWidget->setToolTip(tr("Sélectionner le nombre de fils d'exécution"));

    QGridLayout *v3Layout = new QGridLayout;
    logFrame = new FLineFrame({"Générer un log d'exécution", "Chemin du fichier du log"},
                              QDir::toNativeSeparators(common::generateDatadirPath() + "/altair.log"),
                              "log",
                              {2,1},
                              v3Layout,
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



    connect(memoryUseWidget, &FComboBox::currentTextChanged, [this] {
           bool value = (memoryUseWidget->currentIndex() > 0);
            nLineLabel->setDisabled(value);
            NLineLabel->setDisabled(value);
            nLineEdit->setDisabled(value);
            NLineEdit->setDisabled(value);
        });

    QGridLayout *v2Layout = new QGridLayout;
    v2Layout->addWidget(memoryUseLabel,    3, 0, Qt::AlignRight);
    v2Layout->addWidget(memoryUseWidget,   3, 1, Qt::AlignLeft);
    v2Layout->addWidget(nLineEdit,         4, 1, Qt::AlignLeft);
    v2Layout->addWidget(nLineLabel,        4, 0, Qt::AlignRight);
    v2Layout->addWidget(NLineEdit,         5, 1, Qt::AlignLeft);
    v2Layout->addWidget(NLineLabel,        5, 0, Qt::AlignRight);
    v2Layout->addWidget(processTypeLabel,  6, 0, Qt::AlignRight);
    v2Layout->addWidget(processTypeWidget, 6, 1, Qt::AlignLeft);
    v2Layout->addWidget(consoleCheckBox,   7, 0, Qt::AlignLeft);

    processTypeBox->setLayout(v2Layout);

    v3Layout->addWidget(logCheckBox,       1, 0, Qt::AlignLeft);
//    v3Layout->addWidget(logFrame,          2, 0, Qt::AlignLeft);

    QGroupBox* logBox = new QGroupBox(tr("Log"));
    logBox->setLayout(v3Layout);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel=new FRichLabel("Paramètres de traitement", ":/images/configure-toolbars.png");
    mainLayout->addWidget(mainLabel);
    mainLayout->addWidget(processTypeBox, 1, 0);
    mainLayout->addWidget(logBox, 2, 0);
  //  mainLayout->addSpacing(82);

    setLayout(mainLayout);
}



std::uint16_t options::RefreshFlag;

options::options(Altair* parent)
{
    /* plain old data types must be 0-initialised even though the class instance was new-initialised. */
        
    options::RefreshFlag = interfaceStatus::optionTabs;

    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(48,48));
    contentsWidget->setFont(QFont("Garamond", 10));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setFixedWidth(98);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    standardTab = new standardPage;
    processTab  = new processPage;
    dirTab  = new dirPage;
    pagesWidget->addWidget(standardTab);
    pagesWidget->addWidget(processTab);
    pagesWidget->addWidget(dirTab);

    closeButton = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    closeButton->button(QDialogButtonBox::Ok)->setText("Accepter");
    closeButton->button(QDialogButtonBox::Cancel)->setText("Annuler");
    
    connect(closeButton,
            &QDialogButtonBox::accepted,
            [this,parent]
            {
                options::RefreshFlag =  interfaceStatus::hasUnsavedOptions;
                accept();
                parent->execPath = dirTab->applicationNoyau->getText();
                parent->altairCommandStr =  parent->execPath +  QDir::separator() + ("lhx"+ QString(systemSuffix));

                parent->updateProject(true);

            });

    connect(closeButton, SIGNAL(rejected()), this, SLOT(reject()));
    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));

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


/* implement a global clear() function for the FStringList of data in an FListFrame ; it will be used as Altair::clearData() too. Usage below is faulty. */

void options::clearOptionData()
{
    Hash::wrapper.clear();
    Hash::Reference.clear();
  
    options::RefreshFlag = interfaceStatus::optionTabs;
}


void options::createIcon(const char* path, const char* text)
{
    QListWidgetItem *button = new QListWidgetItem(contentsWidget);
    QString strpath=QString(path);
    QString strtext=QString(text);
    button->setIcon(QIcon(strpath));
    button->setText(strtext);
    button->setTextAlignment(Qt::AlignHCenter);
    button->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}


void options::createIcons()
{
    QList<const char*> iconList=QList<const char*>() << ":/images/csv.png" << "Format" <<
                                                        ":/images/configure-toolbars.png" << "Traitement" <<
                                                        ":/images/directory.png" << "Répertoires";


    for (int i = 0; i < iconList.size()/2 ; i++) createIcon(iconList[2*i], iconList[2*i+1]);


}

void options::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;
    int r;
    r=(current)? contentsWidget->row(current) : 0;
    if (current) pagesWidget->setCurrentIndex(r);
}





