
#include <QFile>
#include <thread>
#include "altair.h"
#include "forms.h"
#include "options.h"
#include "browser.h"
#include "altair.h"
#include "templates.h"
#include "flineframe.hpp"
#include <fstream>

extern template void createHash(QHash<QString, QString>&, 
                                const QList<QString>*,
                                const QList<QString>*);


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
    FRichLabel *mainLabel=new FRichLabel("Répertoires des applications", 
                                         ":/images/directory.png");
    mainLayout->addWidget(mainLabel);
    mainLayout->addWidget(baseBox, 1, 0);
    mainLayout->addSpacing(100);

    setLayout(mainLayout);
}


int codePage::ajouterVariable(const QString& nom) 
{
   static int index; 
   const QString &NOM = nom.toUpper();
   
   // Ajouter ici les FLineEdit en pile dans listeCodes
   
   FLineEdit *line = new FLineEdit("",
                                   NOM,
                                  {NOM, "Code de paye"});
   
   line->setMinimumWidth(MINIMUM_LINE_WIDTH);
   
   listeCodes << line;
      
   // Ajouter ici les labels correspondants dans liste Labels
   // Les labels doivent correspondre à une variable chaîne nommée codes.label dans prologue_codes.R
   
   listeLabels << nom;
   
   vLayout->addWidget(new QLabel("Codes " + NOM + " :  "), index, 0, Qt::AlignRight);
   
   vLayout->addWidget(listeCodes[index], index, 1, Qt::AlignLeft);
   
   ++index;
   
   return index;
 }   



codePage::codePage()
{
    QGroupBox *baseBox = new QGroupBox;
    
    appliquerCodes = new QToolButton;
    
    appliquerCodes->setIcon(QIcon(":/images/view-refresh.png"));
    appliquerCodes->setToolTip("Appuyer pour exporter ces codes de paye<br>    "
                               "pour la génération des rapports d'analyse.   ");
    appliquerCodes->setCheckable(true);

    QStringList variables = {"nbi", "pfi", "pfr", "ifts", "iat", "ihts", "vacataires", "elus"};
    
    short index;
    
    for (const QString& s : variables) index = ajouterVariable(s);    
    
    label = new QLabel;
    
    vLayout->addWidget(label, index, 0, Qt::AlignRight);
    vLayout->addWidget(appliquerCodes, index,1, Qt::AlignLeft);
    vLayout->setColumnMinimumWidth(1, MINIMUM_LINE_WIDTH);
    vLayout->setSpacing(10);
    
    baseBox->setLayout(vLayout);
    
    QVBoxLayout* mainLayout = new QVBoxLayout;
    FRichLabel *mainLabel=new FRichLabel("Code de paye des tests", 
                                         ":/images/data-input.png");
    mainLayout->addWidget(mainLabel);
    mainLayout->addWidget(baseBox, 1, 0);
    mainLayout->addSpacing(100);
    
    init_label_text = "Appuyer pour exporter<br> vers les rapports d'analyse ";
    label->setText(init_label_text);
    
    connect(appliquerCodes, SIGNAL(clicked()), this, SLOT(substituer_valeurs_dans_script_R()));
    
    for (FLineEdit *a: listeCodes)
    {
        connect(a,
                &QLineEdit::textEdited, 
                [this] {
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
    return   "codes." + X + " <- " + Y ;
}

void codePage::substituer_valeurs_dans_script_R()
{
    reinitialiser_prologue();
          
    QIcon icon0 = QIcon(":/images/view-refresh.png");
    QIcon icon1 = QIcon(":/images/msg.png");
    QIcon icon2 = QIcon(":/images/error.png");
      
    QIcon icon = (appliquerCodes->isChecked()) ?
                  icon1 :
                  icon0;
    
    bool res = true;

    for (const FLineEdit* a: listeCodes)
    {
          res |= a->text().isEmpty();
    }

    if (res == false)
    {
        Warning("Attention", 
                "Les codes sont tous non renseignés.<br>"
                "Les tests statutaires se feront <br>"
                "sous algorithme heuristique seulement.");
        
        icon = icon2;
 
        return;
    }

    res = substituer("ajuster_prologue *<- *FALSE",
                     "ajuster_prologue <- TRUE",
                     "Tests/Exemple/prologue.R");

    if (res == false)
    {
        Warning("Attention",
                "Le remplacement de la variable ajuster_prologue<br>"
                "n'a pas pu être effectué dans le fichier prologue.R<br>"
                "Les tests statutaires se feront <br>sous algorithme heuristique seulement.");
        
        icon = icon2;
    }
    else
    {
        for (int rang = 0; rang < listeCodes.size(); ++rang)
        {
            const QString &s     = listeLabels.at(rang);
            const QString &codes = listeCodes.at(rang)->text();

            res &= substituer(regexp(s),
                              rempl_str(s, codes),
                              "Tests/Exemple/prologue_codes.R");

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
    }

    appliquerCodes->setIcon(icon);
    
    if (res == true)
        label->setText("Les codes de paye seront <br>pris en compte pour les rapports  ");
    else
        label->setText("Tous les codes de paye ne pourront pas<br>"
                       "être pris en compte pour les rapports  ");
}

bool codePage::reinitialiser_prologue()
{
    const QString &path_codes = path_access("Tests/Exemple/prologue_codes.R");
    QFile(path_codes).remove();
    bool result = QFile(path_access("Tests/Exemple/prologue_init.R")).copy(path_codes);
    result &= substituer("ajuster_prologue *<- *TRUE",
                         "ajuster_prologue <- FALSE",
                         "Tests/Exemple/prologue.R");
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
    maxNLigneLineEdit = new FLineEdit("",
                                      "maxLigne",
                                     {"Données csv",
                                      "Découper la base de données"
                                      "par segment d'au maximum ce nombre de lignes."},
                                      "T");

    QGroupBox* optionalFieldBox = new QGroupBox(tr("Variables optionnelles"));

    rangCheckBox = new FCheckBox("Numéroter les lignes",
                               "genererNumLigne",
                               {"Données csv", "numéroter les lignes"},
                               "l");

    etabCheckBox = new FCheckBox("Exporter les informations\nsur l'établissement",
                                  flags::status::enabledUnchecked 
                                | flags::commandLineType::altairCommandLine,
                                 "exporterEtab",
                                 {"Données csv",
                                  "Exporter les champs Budget,"
                                  " Employeur, Siret, Etablissement"},
                                 "S");


    QGridLayout *v1Layout = new QGridLayout, *v2Layout = new QGridLayout;

    v2Layout->addWidget(rangCheckBox,      0, 0, Qt::AlignLeft);
    v2Layout->addWidget(etabCheckBox,      1, 0, Qt::AlignLeft);
    v2Layout->setColumnMinimumWidth(1, 250);

    optionalFieldBox->setLayout(v2Layout);

    tableCheckBox = new FCheckBox("Créer la base de données",
                                   flags::status::enabledChecked 
                                 | flags::commandLineType::altairCommandLine,
                                  "genererTable",
                                 {"Données csv",
                                  "créer la base des lignes et bulletins de paye"},
                                  "t",
                                 {optionalFieldBox, baseTypeLabel, baseTypeWidget,
                                  maxNLigneLabel, maxNLigneLineEdit});


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

    // A ce stade seules les bases monolithiques et par année peuvent être sous découpées en segments d'au maximum N lignes
    // Les autres types de base doivent donc désactiver la FLineEdit maxNLigneLabel.
    // Le code présuppose que les types de base sont rangés dans l'ordre : Standard > Par année > autres types.
    // Code à désactiver lorsque cette fonctionnalités sera étendue aux autres types. 

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
                              flags::status::disabled 
                            | flags::commandLineType::defaultCommandLine,
                              "nAgent",
                             {"Nombre maximum d'agents", ""},
                              "n");

    nLineEdit->setFixedWidth(40);

    NLineLabel = new QLabel("Nombre maximum de lignes de paye par agent  ");
    NLineLabel->setDisabled(true);
    NLineEdit = new FLineEdit("",
                              flags::status::disabled 
                            | flags::commandLineType::defaultCommandLine,
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
    processTypeWidget->setCurrentIndex(std::thread::hardware_concurrency() / 2 + 1);
    processTypeWidget->setToolTip(tr("Sélectionner le nombre de fils d'exécution "));

    QGridLayout *v3Layout = new QGridLayout;
    logFrame = new FLineFrame({"Générer un log d'exécution", "Chemin du fichier du log"},
                                QDir::toNativeSeparators(common::generateDatadirPath() 
                                                       + "/altair.log"),
                               "log",
                               {2,1},
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
                                   {"Générer un log d'exécution",
                                    "Utiliser l'onglet console"});


    QList<QString> ecoRange = QList<QString>(), ecoRange2 = QList<QString>();
    ecoRange << "Intensive (100 %)" << "Standard (80 %)" << "Modérée (60 %)" 
             << "Econome (40 %)" << "Très économe (20 %)" << "Minimale (10 %)"
             << "Rationnée (5%)";
    
    ecoRange2 << "100"   << "80" << "60" << "40" << "20" << "10" << "5";

    QLabel* memoryUseLabel = new QLabel("Utilisation de la mémoire  ");

    /* Utiliser % devant l'option active la syntaxe `--option argument' plutôt que `--option=argument' */

    memoryUseWidget = new FComboBox(ecoRange,
                                 "memoryUse",
                                 {"Gestion de la mémoire", 
                                  "Pourcentage d'utilisation de la mémoire libre"},
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
    FRichLabel *mainLabel=new FRichLabel("Paramètres de traitement", 
                                         ":/images/configure-toolbars.png");
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
    contentsWidget->setFont(QFont("Garamond", 8));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setFixedWidth(98);
    contentsWidget->setSpacing(12);
    

    pagesWidget = new QStackedWidget;
    standardTab = new standardPage;
    processTab  = new processPage;
    dirTab  = new dirPage;
    codeTab  = new codePage;
    
    pagesWidget->addWidget(standardTab);
    pagesWidget->addWidget(processTab);
    pagesWidget->addWidget(codeTab);
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
                parent->altairCommandStr =  parent->execPath +  QDir::separator() 
                                          + ("lhx"+ QString(systemSuffix));

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
    QString strpath=QString(path);
    QString strtext=QString(text);
    button->setIcon(QIcon(strpath));
    button->setText(strtext);
    button->setTextAlignment(Qt::AlignRight);
    button->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}


void options::createIcons()
{
    QList<const char*> iconList=QList<const char*>()
                                    << ":/images/csv.png" << "   Format  " 
                                    << ":/images/configure-toolbars.png" << "Traitement "
                                    << ":/images/data-icon.png" << "   Codes   " 
                                    << ":/images/directory.png" << "Répertoires";


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





