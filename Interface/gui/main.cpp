/*

main.cpp  - Author a DVD-Audio DVD with altair-author

This application uses Qt4.8 . Check Qt's licensing details on http://qt.nokia.com


Copyright Fabrice Nicol <fabnicol@users.sourceforge.net> Feb 2009,2011-2013

The latest version can be found at http://dvd-audio.sourceforge.net

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/



#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QTextCodec>
#include "altair-gui.h"
#include <thread>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setFont(QFont("Verdana", 10));
    QString translationsPath(QCoreApplication::applicationDirPath() + "/../translations");
    QLocale locale = QLocale::system();

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTranslator qtTranslator;
    bool res=false;
    if ((res = qtTranslator.load(locale, "qt", "_", translationsPath)))
      {
        res = app.installTranslator(&qtTranslator);
      }

    if (res == false)
        QMessageBox::critical(nullptr, "Erreur", "Traductions qt non chargées", QMessageBox::Cancel);

    QTranslator qtBaseTranslator;
    if ((res = qtBaseTranslator.load(locale, "qtbase", "_", translationsPath)))
    {
        res=app.installTranslator(&qtBaseTranslator);
    }

    if (res == false)
        QMessageBox::critical(nullptr, "Erreur", "Traductions qtbase non chargées", QMessageBox::Cancel);

    QTranslator qtXmlTranslator;
    if ((res = qtXmlTranslator.load(locale, "qtxmlpatterns", "_", translationsPath)))
    {
        res=app.installTranslator(&qtXmlTranslator);
    }

    if (res == false)
        QMessageBox::critical(nullptr, 
                                          "Erreur", 
                                          "Traductions qtxmlpatterns non chargées",
                                          QMessageBox::Cancel);

    char* s;
    if (argc > 1) s=argv[1];
    else s=(char*)"";

    app.setStyleSheet("\
    QGroupBox {\
        border: 1px solid silver;\
        border-radius: 5px;\
        margin-top: 1ex;\
        padding: 10px;\
    }\
\
    QGroupBox::title {\
        subcontrol-origin: margin;\
        subcontrol-position: top center;\
        padding: 0 10px;\
    }");

  QLabel *a = nullptr;
  const QString cdROM = common::cdRomMounted();

  if (! cdROM.isEmpty())
  {
   if (QDir(cdROM).exists() && ! QDir(cdROM).QDir::entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot).isEmpty())
   {
        a = new QLabel(" Lancement d'Altaïr...");
        a->setGeometry(QRect(500, 300, 200, 80));
        a->setWindowIcon(QIcon(":/images/altair.png"));
        a->show();
        QTimer::singleShot(1300, a, SLOT(close()));
        app.exec();
        QObject::connect(a, &QLabel::destroyed, [&] { app.quit();});
    }
  }

    MainWindow *mainWin = new MainWindow(s);

    QObject::connect(mainWin, &MainWindow::exitSignal, [&] { app.quit();});
    mainWin->show();
    if (a) a->raise();
    
    return app.exec();
}
