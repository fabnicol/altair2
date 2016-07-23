
#include <QFile>
#include "altair-author-gui.h"
#include "options.h"
#include "forms.h"


lplexPage::lplexPage(altair* parent)
{


  int lplexComboBoxWidth=100;
  int lplexLineEditWidth=250;
  
  QLabel *lplexVideoLabel = new QLabel(tr("TV standard"));

  lplexVideoType = new FComboBox({ "ntsc" , "pal" , "secam"},
                                 flags::commandLineType::lplexFiles|flags::status::enabled,
                                 "lplexVideoType",
                                 {"Lplex","TV standard"},
                                  "video" );

  lplexVideoType->setMinimumWidth(lplexComboBoxWidth);

  QLabel *lplexBackgroundLabel = new QLabel(tr("Path to background"));
  QToolButton *lplexBackgroundButton=new QToolDirButton;
  /* connect to ... */
  lplexBackgroundLineEdit = new FLineEdit(generateDatadirPath("/black.jpg"),
                                          flags::commandLineType::lplexFiles|flags::status::enabled,
                                          "lplexJpegPath",
                                         {"Lplex", "Path to lplex background jpeg"},
                                          "jpeg");

  lplexBackgroundLineEdit->setMinimumWidth(lplexLineEditWidth);

  lplexScreenParameterBox = new FCheckBox("Use DVD-Audio display parameters",
                      flags::status::enabledUnchecked|flags::commandLineType::defaultCommandLine,
                      "lplexDisplayIsDVD-Audio",
                      {"Lplex", "Use DVD-Audio display parameters"},
                       "",
                      {nullptr},
                     {
                          lplexBackgroundLineEdit,
                          lplexBackgroundLabel,
                          lplexBackgroundButton,
                          lplexVideoType,
                          lplexVideoLabel
                      });

  QLabel *lplexCreateLabel = new QLabel(tr("Authoring"));

  lplexCreateType = new FComboBox({ "lpcm" , "m2v" , "dvdstyler" , "mpeg" , "dvd" , "iso"} ,
                                  flags::commandLineType::lplexFiles|flags::status::enabled,
                                  "lplexCreateType",
                                 {"Lplex","Authoring type"},
                                  "create" );

  lplexCreateType->setMinimumWidth(lplexComboBoxWidth);

  QLabel *lplexMediaLabel = new QLabel(tr("Disc size"));

  lplexMediaType = new FComboBox( {"dvd+r" ,  "dvd-r" ,  "dl" ,  "none"},
                                  flags::commandLineType::lplexFiles|flags::status::enabled,
                                  "lplexMediaType",
                                 {"Lplex","Maximum disc size"},
                                  "media");

  lplexMediaType->setMinimumWidth(lplexComboBoxWidth);

  lplexDirLineEdit = new FLineEdit(tempdir,
                                   flags::commandLineType::lplexFiles|flags::status::enabled,
                                   "lplexDirPath",
                                   {"Lplex","Output everything to this directory"},
                                   "dir" );

  lplexDirLineEdit->setMinimumWidth(lplexLineEditWidth);
  QLabel *lplexDirLabel = new QLabel(tr("Path to output directory"));
  QToolButton *lplexDirButton=new QToolDirButton;
  lplexDirButton->setMaximumWidth(40);

  QGroupBox *lplexBox = new QGroupBox(tr("Video options"));
  QGridLayout *lplexLayout=new QGridLayout;

  lplexLayout->addWidget(lplexScreenParameterBox , 1,0);
  lplexLayout->addWidget(lplexVideoLabel, 2,0, Qt::AlignLeft);
  lplexLayout->addWidget(lplexVideoType, 2,0, Qt::AlignHCenter);
  lplexLayout->addWidget(lplexBackgroundLabel, 3, 0, Qt::AlignLeft);
  lplexLayout->addWidget(lplexBackgroundLineEdit , 3, 0, Qt::AlignRight);
  lplexLayout->addWidget(lplexBackgroundButton, 3,1,Qt::AlignLeft);
  lplexLayout->addWidget(lplexCreateLabel, 4, 0, Qt::AlignLeft);
  lplexLayout->addWidget(lplexCreateType,  4, 0, Qt::AlignHCenter);
  lplexLayout->addWidget(lplexMediaLabel,  5, 0, Qt::AlignLeft);
  lplexLayout->addWidget(lplexMediaType,   5, 0, Qt::AlignHCenter);
  lplexLayout->addWidget(lplexDirLabel, 6, 0, Qt::AlignLeft);
  lplexLayout->addWidget(lplexDirLineEdit, 6, 0, Qt::AlignRight);
  lplexLayout->addWidget(lplexDirButton, 6, 1, Qt::AlignLeft);

  lplexLayout->setColumnMinimumWidth(0, 450);

  lplexBox->setLayout(lplexLayout);


  /******    advanced ********/

  QLabel *lplexSpliceLabel = new QLabel(tr("Splice"));

  lplexSpliceType = new FComboBox({"seamless" , "discrete" , "padded" , "none"},
                                  flags::status::enabledUnchecked|flags::commandLineType::lplexFiles,
                                  "lplexSpliceType",
                                 {"Lplex","Physically structure track transition point"},
                                  "splice" );

  lplexSpliceType->setMinimumWidth(lplexComboBoxWidth);

  QLabel *lplexShiftLabel = new QLabel(tr("Move startpoints"));

  lplexShiftType = new FComboBox({"backward" , "forward" , "nearest" },
                                 flags::status::enabledUnchecked|flags::commandLineType::lplexFiles,
                                 "lplexShiftType",
                                 {"Lplex","Move seamless startpoints"},
                                 "shift");

  lplexShiftType->setMinimumWidth(lplexComboBoxWidth);


  lplexMd5AwareBox = new FCheckBox("Generate MD5 tags",
                                    flags::status::enabledUnchecked|flags::commandLineType::lplexFiles,
                                   "lplexMd5Aware",
                                   {"Lplex","Generate MD5 tags"},
                                   "md5aware");

  lplexRescaleBox = new FCheckBox("Rescale images to TV standard",
                                  flags::status::enabledUnchecked|flags::commandLineType::lplexFiles,
                                  "lplexRescale",
                                { "Lplex","If jpegs sized for ntsc [pal] are being used\nto create a pal [ntsc] dvd, rescale them"},
                                  "rescale") ;

  lplexInfoDirLineEdit = new FLineEdit(tempdir,
                                  flags::status::enabledUnchecked|flags::commandLineType::lplexFiles,
                                       "lplexInfoDirPath",
                                       {"Lplex","Path to directory to be added\n to Lplex-made XTRA directory"},
                                       "infodir");

  lplexInfoDirLineEdit->setMinimumWidth(lplexLineEditWidth);
  QLabel *lplexInfoDirLabel = new QLabel(tr("Path to Info directory"));

  QToolButton *lplexInfoDirButton=new QToolDirButton;
  lplexInfoDirButton->setMaximumWidth(40);

  /* connect to  QFileDialog::getExistingDirectory(this, "Browse Info directory"); */

  lplexInfoDirBox = new FCheckBox("Add Info directory to disc",
                                  flags::status::disabled|flags::commandLineType::noCommandLine,
                                  "lplexInfoDir",
                                  {"Lplex","Add Info directory\nto disc XTRA directory"},
                                    {
                                      lplexInfoDirLabel,
                                      lplexInfoDirLineEdit,
                                      lplexInfoDirButton
                                    });


  lplexInfofilesBox = new FCheckBox("Generate info files",
                                    flags::status::enabled|flags::commandLineType::noCommandLine,
                                    "lplexInfoFiles",
                                    {"Lplex", "Make an 'XTRA' info folder"},
                                    {lplexInfoDirBox}) ;

  QGroupBox *lplexAdvancedBox= new QGroupBox(tr("Advanced parameters"));
  QGridLayout *lplexAdvancedLayout=new QGridLayout;

  
  launchLplexAloneBox = new FCheckBox("Launch Lplex alone",
                      flags::status::enabledUnchecked|flags::commandLineType::noCommandLine,
                      "launchLplexAlone",
                      {"Lplex", "Start Lplex with own command line"},
                      {lplexBox, lplexAdvancedBox});
  
  
  lplexAdvancedLayout->addWidget(lplexSpliceLabel, 1, 0, Qt::AlignLeft);
  lplexAdvancedLayout->addWidget(lplexSpliceType,  1, 0, Qt::AlignHCenter);
  lplexAdvancedLayout->addWidget(lplexShiftLabel,   2, 0, Qt::AlignLeft);
  lplexAdvancedLayout->addWidget(lplexShiftType,    2, 0, Qt::AlignHCenter);
  lplexAdvancedLayout->addWidget(lplexMd5AwareBox, 3, 0);
  lplexAdvancedLayout->addWidget(lplexRescaleBox, 4, 0);
  lplexAdvancedLayout->addWidget(lplexInfofilesBox, 5, 0);
  lplexAdvancedLayout->addWidget(lplexInfoDirBox, 6, 0);
  lplexAdvancedLayout->addWidget(lplexInfoDirLabel, 7, 0, Qt::AlignLeft);
  lplexAdvancedLayout->addWidget(lplexInfoDirLineEdit, 7, 0, Qt::AlignRight);
  lplexAdvancedLayout->addWidget(lplexInfoDirButton, 7, 1, Qt::AlignLeft);

  lplexAdvancedLayout->setColumnMinimumWidth(0, 450);
  lplexAdvancedBox->setLayout(lplexAdvancedLayout);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  FRichLabel *lplexLabel=new FRichLabel("Lplex options", ":/images/64x64/lplex.png");
  
  mainLayout->addWidget(lplexLabel);
  mainLayout->addWidget(launchLplexAloneBox);
  setWhatsThisText(launchLplexAloneBox, 108, 121);
  
  mainLayout->addSpacing(20);
  mainLayout->addWidget(lplexBox);
  mainLayout->addSpacing(20);
  mainLayout->addWidget(lplexAdvancedBox);
  mainLayout->addStretch(1);
  mainLayout->setMargin(20);
  setLayout(mainLayout);
  altairParent=parent;
   
  connect(lplexDirButton, SIGNAL(clicked(bool)), this, SLOT(on_lplexDirButton_clicked()));
  connect(lplexInfoDirButton, SIGNAL(clicked(bool)), this, SLOT(on_lplexInfoDirButton_clicked()));
  connect(lplexBackgroundButton, SIGNAL(clicked()), this, SLOT(on_lplexBackgroundButton_clicked()));
  connect(launchLplexAloneBox, SIGNAL(toggled(bool)), this, SLOT(resetVideoProjectFeatures(bool)));
    
  resetVideoProjectFeatures(v(launchLplexAlone).isTrue());
}

void lplexPage::resetVideoProjectFeatures(bool checked)
{
    if (checked)
    {
       altairParent->project[VIDEO]->setOptionLabel("^");
       altairParent->project[VIDEO]->setCommandLineType(flags::commandLineType::lplexFiles);
       altairParent->project[VIDEO]->setStatus(flags::status::hasListCommandLine);
       altairParent->project[VIDEO]->setSeparator({" ", " ts "});
    }
    else
    {
       altairParent->project[VIDEO]->setOptionLabel("dvdv-tracks");
       altairParent->project[VIDEO]->setCommandLineType(flags::commandLineType::altairCommandLine);
       altairParent->project[VIDEO]->setStatus(flags::status::enabled);
       altairParent->project[VIDEO]->setSeparator({",", ";" });
    }
    
}

void lplexPage::on_lplexInfoDirButton_clicked()
{
    lplexInfoDirLineEdit->setText(QFileDialog::getExistingDirectory(this, lplexInfoDirBox->getDescription().join(": ")));
}

void lplexPage::on_lplexDirButton_clicked()
{
    lplexDirLineEdit->setText(QFileDialog::getExistingDirectory(this, lplexDirLineEdit->getDescription().join(": ")));
}

void lplexPage::on_lplexBackgroundButton_clicked()
{
    lplexBackgroundLineEdit->setText(QFileDialog::getOpenFileName(this, lplexBackgroundLineEdit->getDescription().join(": ")));
}









