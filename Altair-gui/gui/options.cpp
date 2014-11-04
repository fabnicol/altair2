
#include <QFile>

#include "altair.h"
#include "forms.h"
#include "options.h"
#include "browser.h"
#include "altair.h"
#include "templates.h"

extern template void createHash(QHash<QString, QString>*, const QList<QString>*, const QList<QString>*);

standardPage::standardPage()
{

    QGroupBox *baseBox= new QGroupBox(tr("Base .csv"));
    QGridLayout *baseLayout= new QGridLayout;

    baseLineEdit= new FLineEdit(generateDatadirPath("Base"),
                                        "base",
                                       {"Base", "Répertoire de la base .csv"},
                                        "D");

    QLabel *baseLabel= new QLabel(tr("Répertoire de la base"));
    QToolDirButton *baseButton;

    baseButton= new QToolDirButton(tr("Sélectionner le répertoire de la base de données .csv\nen sortie de l'application"));
    
    QToolDirButton *openBaseButton=new QToolDirButton(tr("Ouvrir le répertoire "), actionType::OpenFolder);
    
    baseLayout->addWidget(baseLineEdit,   1, 0);
    baseLayout->addWidget(baseLabel,      0, 0);
    baseLayout->addWidget(baseButton,     1, 1);
    baseLayout->addWidget(openBaseButton, 1, 2);
            
    baseBox->setLayout(baseLayout);
    
    baseTypeBox=new QGroupBox(tr("Type de base en sortie"));
    processTypeBox=new QGroupBox(tr("Mode d'exécution"));

    QList<QString> range=QList<QString>(), range2=QList<QString>();
    range << "Standard" << "Par année" << "Toutes catégories" << "Traitement" << "Indemnité"
          << "SFT"      << "Rémunérations diverses"           << "Rappel"     << "Acompte"
          << "Avantage en nature" << "Indemnité de résidence" << "Cotisations"
          << "Déductions"         << "Retenue";

    range2 << ""   << "AN" << "X" << "T" << "I"
           << "S"  << "A"  << "R" << "AC"
           << "AV" << "IR" << "C"
           << "D"  << "RE";

    QStringList range3=QStringList();
    for (int i=1; i < 9; i++) range3 << QString::number(i);
    
    QLabel* baseTypeLabel = new QLabel("Type de base  ");
    baseTypeWidget=new FComboBox(range,
                                 "baseType",
                                 {"Type de base", "Par catégorie"},
                                  "T");

    createHash(baseTypeWidget->comboBoxTranslationHash, &range, &range2);
    baseTypeWidget->status=flags::status::defaultStatus;
    baseTypeWidget->commandLineType=flags::commandLineType::defaultCommandLine;

    baseTypeWidget->setFixedWidth(175);
    baseTypeWidget->setFixedHeight(30);
    baseTypeWidget->setCurrentIndex(0);
    baseTypeWidget->setToolTip(tr("Sélectionner le type de base en sortie"));

    QLabel* maxNLigneLabel = new QLabel("Nombre maximum de lignes  ");
    maxNLigneLineEdit = new FLineEdit("", 
                                      "maxLigne",
                                     {"Type de base", "Nombre maximum de lignes"},
                                      "T");
    maxNLigneLineEdit->setFixedWidth(60);
    
    QLabel* sepLabel = new QLabel("Séparateur de champs ");
    sepLineEdit = new FLineEdit(",",
                                "separateur",
                               {"Séparateurs", "Séparateur de champ"},
                                "s");
    sepLineEdit->setFixedWidth(15);
    
    QLabel* decLabel = new QLabel("Séparateur décimal  ");
    decLineEdit = new FLineEdit(".",
                                "decimal",
                               {"Séparateurs", "Séparateur décimal"},
                                "d");
    decLineEdit->setFixedWidth(15);
    
    QLabel* processTypeLabel = new QLabel("Nombre de fils d'exécution  ");
    processTypeWidget=new FComboBox(range3,
                                 "processType",
                                 {"Type de processus", "Nombre de fils d'exécution"},
                                  "j");

    processTypeWidget->setFixedWidth(40);
    processTypeWidget->setFixedHeight(30);
    processTypeWidget->setCurrentIndex(0);
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
    
    tableCheckBox=new FCheckBox("Générer la table  ",
                                "genererTable",
                                {"Générer la table .csv", "type standard"},
                                "t");
    economeCheckBox=new FCheckBox("Economiser la RAM  ",
                                  "ecoRAM",
                                 {"Mode économe en mémoire", ""},
                                  "t",
                                 {NULL},
                                 { nLineLabel, NLineLabel, nLineEdit, NLineEdit});

    QGridLayout *v1Layout = new QGridLayout;
    QGridLayout *v2Layout = new QGridLayout;
    
    v1Layout->addWidget(baseTypeLabel,     1,0,Qt::AlignRight);
    v1Layout->addWidget(baseTypeWidget,    1,1,Qt::AlignLeft);
    v1Layout->addWidget(maxNLigneLabel,    2,0,Qt::AlignRight);
    v1Layout->addWidget(maxNLigneLineEdit, 2,1,Qt::AlignLeft);
    v1Layout->addWidget(sepLabel,          3,0,Qt::AlignRight);
    v1Layout->addWidget(sepLineEdit,       3,1,Qt::AlignLeft);
    v1Layout->addWidget(decLabel,          4,0,Qt::AlignRight);
    v1Layout->addWidget(decLineEdit,       4,1,Qt::AlignLeft);
    
    v2Layout->addWidget(processTypeLabel,  5,0,Qt::AlignRight);
    v2Layout->addWidget(processTypeWidget, 5,1,Qt::AlignLeft);
    v2Layout->addWidget(tableCheckBox,     1,0,Qt::AlignLeft);
    v2Layout->addWidget(economeCheckBox,   2,0,Qt::AlignLeft);
    v2Layout->addWidget(nLineEdit,         3,1,Qt::AlignLeft);
    v2Layout->addWidget(NLineEdit,         4,1,Qt::AlignLeft);
    v2Layout->addWidget(nLineLabel,        3,0,Qt::AlignRight);
    v2Layout->addWidget(NLineLabel,        4,0,Qt::AlignRight);
    
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

    connect(baseTypeWidget,
            SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_baseTypeWidgetChanged(int)));

    connect(processTypeWidget,
            SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_processTypeWidgetChanged(int)));
    
    connect(openBaseButton,
            SIGNAL(clicked()),
            this, SLOT(on_openBaseDirButton_clicked()));
    
    connect(baseButton,
            SIGNAL(clicked()),
            this, SLOT(selectOutput()));

}


void standardPage::on_openBaseDirButton_clicked()
{
    QString path=baseLineEdit->text();
    QDir targetDirObject(path);
    if (targetDirObject.mkpath(path) == false)
    {
        QMessageBox::warning(0, QString("Répertoire"), QString("Le répertoire %1 n'a pas été créé").arg(path), QMessageBox::Ok);
        return;
    }
    
    common::openDir(baseLineEdit->text());
}


void standardPage::selectOutput()
{
    QString path=QFileDialog::getExistingDirectory(this, QString("Sélection du répertoire"),
                                                   QDir::currentPath(),
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty()) return;

    qint64 size=common::getDirectorySize(path, "*");

    if (size)
    {
        if (QMessageBox::warning(0, QString("Répertoire"), QString("Le répertoire %1 n'est pas vide (Taille %2B). Ecraser et recréer ? ").arg(path,QString::number(size)), QMessageBox::Ok | QMessageBox::Cancel)
                == QMessageBox::Ok)
        {
            if (!common::removeDirectory(path))    QMessageBox::information(0, QString("Supprimer le répertoire"),
                                                           QString("Le répertoire n'a pas été supprimé' %1").arg(QDir::toNativeSeparators(path)));

            QDir targetDirObject(path);
            if (targetDirObject.mkpath(path) == false)
            {
                QMessageBox::warning(0, QString("Répertoire"), QString("Le répertoire %1 n'a pas été créé").arg(path), QMessageBox::Ok);
                return;
            }
        }
    } 
    else
    {
        QString path=Hash::wrapper["base"]->toQString();
        QDir targetDirObject(path);
        if (targetDirObject.mkpath(path) == false)
        {
            QMessageBox::warning(0, QString("Répertoire"), QString("Le répertoire %1 n'a pas été créé").arg(path), QMessageBox::Ok);
            return;
        }
    }

    baseLineEdit->setText(path);
}


int options::RefreshFlag;

options::options(Altair* parent)
{
    /* plain old data types must be 0-initialised even though the class instance was new-initialised. */

        
    options::RefreshFlag=UpdateOptionTabs;

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
                options::RefreshFlag =  hasUnsavedOptions;
                accept();
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
  
    options::RefreshFlag = UpdateOptionTabs;
}




