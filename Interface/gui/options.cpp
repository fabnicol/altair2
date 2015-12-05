
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

    QLabel* maxNLigneLabel;
    QList<QString> range=QList<QString>(), range2=QList<QString>();
    range << "Standard" << "Par année" << "Toutes catégories" << "Traitement" << "Indemnité"
          << "SFT"      << "Rémunérations diverses"           << "Rappel"     << "Acompte"
          << "Avantage en nature" << "Indemnité de résidence" << "Cotisations"
          << "Déductions"         << "Retenue";

    range2 << ""   << "AN" << "X" << "T" << "I"
           << "S"  << "A"  << "R" << "AC"
           << "AV" << "IR" << "C"
           << "D"  << "RE";

    maxNLigneLabel = new QLabel("Nombre maximum de lignes  ");
    maxNLigneLineEdit = new FLineEdit("",
                                      "maxLigne",
                                     {"Données csv", "Nombre maximum de lignes"},
                                      "T");

    QGroupBox *baseBox= new QGroupBox(tr("Répertoires"));
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
    baseTypeWidget=new FComboBox(range,
                                 "baseType",
                                 {"Données csv", "Type de base par catégorie"},
                                  "T");

    rangCheckBox=new FCheckBox("Numéroter les lignes",
                               "genererNumLigne",
                               {"Données csv", "numéroter les lignes"},
                               "l");

    etabCheckBox=new FCheckBox("Ne pas exporter les informations\nsur l'établissement",
                                flags::status::enabledChecked | flags::commandLineType::altairCommandLine,
                                "conserverEtab",
                                {"Données csv", "conserver les champs Budget, Employeur, Siret, Etablissement"},
                                "S");

    tableCheckBox=new FCheckBox("Créer la base de données",
                                flags::status::enabledChecked | flags::commandLineType::altairCommandLine,
                                "genererTable",
                                {"Données csv", "créer la base des lignes et bulletins de paye"},
                                "t",
                                {etabCheckBox, rangCheckBox, baseTypeLabel, baseTypeWidget, maxNLigneLabel, maxNLigneLineEdit});

    applicationNoyau = new FLineFrame(
                {"Application noyau LHX", "Répertoire de l'application noyau" },
                path_access(System),
                "lhxDir",
                {2,0},
                donneesCSV->getLayout());

    baseBox->setLayout(applicationNoyau->getLayout());
    
    baseTypeBox=new QGroupBox(tr("Type de base en sortie"));
    processTypeBox=new QGroupBox(tr("Mode d'exécution"));


    QStringList range3=QStringList();
    for (int i=1; i < 12; i++) range3 << QString::number(i);
    


    createHash(baseTypeWidget->comboBoxTranslationHash, &range, &range2);
    baseTypeWidget->status=flags::status::defaultStatus;
    baseTypeWidget->commandLineType=flags::commandLineType::defaultCommandLine;

    baseTypeWidget->setFixedWidth(175);
    baseTypeWidget->setFixedHeight(30);
    baseTypeWidget->setCurrentIndex(0);
    baseTypeWidget->setToolTip(tr("Sélectionner le type de base en sortie"));


    maxNLigneLineEdit->setFixedWidth(60);
    
    QLabel* processTypeLabel = new QLabel("Nombre de fils d'exécution  ");
    processTypeWidget=new FComboBox(range3,
                                 "processType",
                                 {"Type de processus", "Nombre de fils d'exécution"},
                                  "j");

    processTypeWidget->setFixedWidth(40);
    processTypeWidget->setFixedHeight(30);
    processTypeWidget->setCurrentIndex(3);
    processTypeWidget->setToolTip(tr("Sélectionner le nombre de fils d'exécution"));

    QLabel* nLineLabel = new QLabel("Nombre maximum d'agents par mois  ");
    nLineEdit=new FLineEdit("", 
                            "nAgent",
                           {"Nombre maximum d'agents", ""},
                            "n");
    nLineEdit->setFixedWidth(40);
    
    QLabel* NLineLabel = new QLabel("Nombre maximum de lignes de paye par agent  ");
    NLineEdit=new FLineEdit("",
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

    logCheckBox=new FCheckBox("Générer le log  ",
                              flags::status::enabledUnchecked | flags::commandLineType::noCommandLine,
                                "genererLog",
                                {"Générer un log d'exécution", "application noyau"},
                               logFrame->getComponentList());

    consoleCheckBox=new FCheckBox("Activer la console  ",
                                  flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                  "activerConsole",
                                  {"Générer un log d'exécution", "Utiliser l'onglet console"});

    economeCheckBox=new FCheckBox("Economiser la RAM  ",
                                  flags::status::enabledChecked | flags::commandLineType::noCommandLine,
                                  "ecoRAM",
                                 {"Mode économe en mémoire", ""},
                                 QList<QWidget*>(),   // attention {nullptr} serait ambigu
                                 {nLineLabel, NLineLabel, nLineEdit, NLineEdit});


    v1Layout->addWidget(tableCheckBox,     1, 0, Qt::AlignLeft);
    v1Layout->addWidget(etabCheckBox,      2, 0, Qt::AlignLeft);
    v1Layout->addWidget(baseTypeLabel,     3, 0, Qt::AlignRight);
    v1Layout->addWidget(baseTypeWidget,    3, 1, Qt::AlignLeft);
    v1Layout->addWidget(maxNLigneLabel,    4, 0, Qt::AlignRight);
    v1Layout->addWidget(maxNLigneLineEdit, 4, 1, Qt::AlignLeft);
    v1Layout->addWidget(rangCheckBox,      5, 0, Qt::AlignLeft);

    v2Layout->addWidget(economeCheckBox,   3, 0, Qt::AlignLeft);
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




