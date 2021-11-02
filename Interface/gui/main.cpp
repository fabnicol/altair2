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


/// \file gui/main.cpp
/// \author Fabrice Nicol
/// \brief Lance l'interface graphique en chargeant le bon contexte linguistique et la feuille de style

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include "altair-gui.h"


int main (int argc, char *argv[])
{
    QApplication app (argc, argv);

    app.setFont (QFont ("Verdana", 10));

    QString translationsPath (QCoreApplication::applicationDirPath() + "/../translations");
    QLocale locale = QLocale::French;

    QTranslator qtTranslator;
    bool res = false;

    if ((res = qtTranslator.load (locale, "qt", "_", translationsPath)))
        {
            res = app.installTranslator (&qtTranslator);
        }

    if (res == false)
        QMessageBox::critical (nullptr, "Erreur", "Traductions qt non chargées", QMessageBox::Cancel);

    QTranslator qtBaseTranslator;

    if ((res = qtBaseTranslator.load (locale, "qtbase", "_", translationsPath)))
        {
            res = app.installTranslator (&qtBaseTranslator);
        }

    if (res == false)
        QMessageBox::critical (nullptr, "Erreur", "Traductions qtbase non chargées", QMessageBox::Cancel);

    QTranslator qtXmlTranslator;

    if ((res = qtXmlTranslator.load (locale, "qtxmlpatterns", "_", translationsPath)))
        {
            res = app.installTranslator (&qtXmlTranslator);
        }

    if (res == false)
        QMessageBox::critical (nullptr,
                               "Erreur",
                               "Traductions qtxmlpatterns non chargées",
                               QMessageBox::Cancel);

    char* s;

    if (argc > 1) s = argv[1];
    else s = const_cast<char*>(static_cast<const char*>(""));

    app.setStyleSheet ("\
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


    MainWindow *mainWin = new MainWindow (s);

    QObject::connect (mainWin, &MainWindow::exitSignal, [&] { app.quit();});
    mainWin->show();
    
    mainWin->fontsize = 9;

    return app.exec();
}
