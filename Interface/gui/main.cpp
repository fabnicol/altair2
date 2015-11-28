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


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
app.setFont(QFont("Verdana"));
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
        QMessageBox::critical(nullptr, "Erreur", "Traductions qtxmlpatterns non chargées", QMessageBox::Cancel);

    char* s;
    if (argc > 1) s=argv[1];
    else s=(char*)"";

    MainWindow *mainWin=new MainWindow(s);

    QObject::connect(mainWin, &MainWindow::exitSignal, [&] { app.quit();});
    mainWin->show();
    return app.exec();
}
