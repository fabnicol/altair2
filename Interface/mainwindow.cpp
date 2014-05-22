#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    p=new QProcess(this);
    p->setProcessEnvironment(env);

    ui->radioButton_5->setVisible(false);

    connect(ui->radioButton, SIGNAL(clicked()), this, SLOT(startRStudio()));

    connect(ui->radioButton_2, SIGNAL(clicked()), this, SLOT(startAcroRd32()));
    connect(ui->radioButton_3, SIGNAL(clicked()), this, SLOT(startBrowser()));
    connect(ui->radioButton_4, SIGNAL(clicked()), this, SLOT(startMSWord()));

  }

void MainWindow::startRStudio()
{
   p->execute("cmd.exe /C start /HIGH /B rstudio.exe ");
   ui->radioButton_5->toggle();
}


void MainWindow::startAcroRd32()
{
   p->execute("cmd.exe /C start /HIGH /B acrord32.exe " + QCoreApplication::applicationDirPath() + "/../Rapport/exemple.pdf");
   ui->radioButton_5->toggle();
}

void MainWindow::startBrowser()
{
  QDesktopServices::openUrl(QUrl("file:///"+ QCoreApplication::applicationDirPath() + "/../Rapport/exemple.html"));
  ui->radioButton_5->toggle();
}

void MainWindow::startMSWord()
{
   p->execute("cmd.exe /C start /HIGH /B winword.exe " + QCoreApplication::applicationDirPath() + "/../Rapport/exemple.docx");
   ui->radioButton_5->toggle();
}


MainWindow::~MainWindow()
{
    delete ui;
}
