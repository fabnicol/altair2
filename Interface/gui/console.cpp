// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
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
//
#include "console.h"

Console::Console(MainWindow* p): QDialog(0)
      {
            QGridLayout* consoleLayout=new QGridLayout;
            hide();
            setSizeGripEnabled(true);
            setWindowTitle("Console");
            setMinimumSize(800,600);
            QToolButton *closeConsoleButton=new QToolButton;
            closeConsoleButton->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
            closeConsoleButton->setToolTip(tr("Fermer (Ctrl + Q)"));
            closeConsoleButton->setShortcut(QKeySequence("Ctrl+Q"));
            const QIcon clearOutputText = QIcon(QString::fromUtf8( ":/images/edit-clear.png"));
            QToolButton *clearConsoleButton=new QToolButton;
            clearConsoleButton->setIcon(clearOutputText);
            clearConsoleButton->setShortcut(QKeySequence("Ctrl+N"));
            clearConsoleButton->setToolTip(tr("Fermer la console (Ctrl + N)"));
            consoleLayout->addWidget(textWidget,0,0);
            consoleLayout->addWidget(closeConsoleButton, 1,0,Qt::AlignRight);
            consoleLayout->addWidget(clearConsoleButton, 2,0,Qt::AlignRight);
            setLayout(consoleLayout);
            // [=] not [&]
            connect(closeConsoleButton, &QToolButton::clicked, [=]{on_displayConsoleButton_clicked(p);});
            connect(clearConsoleButton, &QToolButton::clicked, [this]{textWidget->clear();});
     }


void Console::detachConsole(bool isDetached, MainWindow* parent)
{
    if (isDetached)
    {
        hide();
        parent->bottomTabWidget->addTab(parent->consoleDialog, tr("Console"));
        parent->bottomTabWidget->setCurrentIndex(1);
    }
    else
    {
        show();
        parent->bottomTabWidget->removeTab(1);
    }
}

void Console::on_displayConsoleButton_clicked(MainWindow* parent)
    {
        static bool isDetached;
        detachConsole(isDetached, parent);
        isDetached=!isDetached;
    }

