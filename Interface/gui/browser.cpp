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

#include "browser.h"

#ifndef STATIC
browser::browser(const QUrl &urlPath,  QWidget *parent) :  QWidget(parent)
{
 
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::WindowModal);

    url=urlPath;

    homeButton->setIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));
    homeButton->setToolTip(tr("Accueil"));
    backButton->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    backButton->setToolTip(tr("Retour (Ctrl + <)"));
    backButton->setShortcut(QKeySequence("Ctrl+<"));

    forwardButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
    forwardButton->setToolTip(tr("Suivant (Ctrl + >)"));
    forwardButton->setShortcut(QKeySequence("Ctrl+>"));

    closeButton->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    closeButton->setToolTip(tr("Fermer (Ctrl + Q)"));
    closeButton->setShortcut(QKeySequence("Ctrl+Q"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(homeButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(forwardButton);
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(textBrowser);
    setLayout(mainLayout);

    connect(homeButton, SIGNAL(clicked()), this, SLOT(home()));
    connect(backButton, SIGNAL(clicked()), textBrowser, SLOT(back()));
    connect(forwardButton, SIGNAL(clicked()), textBrowser, SLOT(forward()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(textBrowser, SIGNAL(urlChanged(const QUrl &)), this, SLOT(updateWindowTitle()));

    textBrowser->load(url);
    const QIcon altairIcon=QIcon(QString::fromUtf8( ":/images/altair.png"));
    setWindowIcon(altairIcon);

}

#endif

#ifndef STATIC
void browser::home()
{
    textBrowser->load(url);
}

void browser::updateWindowTitle()
{
    setWindowTitle(textBrowser->url().toString());
}

void browser::showPage(const QUrl &url)
{
    browser *app = new browser(url);
    app->resize(1000, 500);
    app->show();
}
#else

void browser::showPage(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}

#endif


