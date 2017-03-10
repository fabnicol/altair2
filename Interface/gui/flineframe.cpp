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
#include "flineframe.hpp"
#include "common.h"
#include <QWidget>
#include "fwidgets.h"
#include "enums.h"

FLineFrame::FLineFrame(const QStringList& titre,
                       const QString& defaut,
                       const QString& xmlTag,
                       const QList<int>& coord,
                       QGridLayout* inputLayout,
                       const QString& commandline,
                       flags::directory check,
                       flags::flineframe category)
{
    if (inputLayout == nullptr)
       frameLayout= new QGridLayout;
    else
        frameLayout = inputLayout;

    if (frameLayout == nullptr) return;
    int row = coord.at(0);
    int column = coord.at(1);

    if (titre.size() < 2) return;

    if (commandline.isEmpty())
        lineEdit =  new FLineEdit(defaut, xmlTag, titre);
    else
        lineEdit = new FLineEdit(defaut, xmlTag, titre, commandline);

    label = new QLabel(titre.at(1));
    pathCategory = category;
    sButton = new QToolDirButton(QString("Sélectionner le ")+ ((pathCategory == flags::flineframe::isDirectoryPath)?
                                                     "répertoire" : "fichier") );

    oButton = new QToolDirButton("Ouvrir le répertoire ", actionType::OpenFolder);

    componentList = {sButton, oButton, label, lineEdit};
    frameLayout->addWidget(lineEdit,    row + 1, column);
    frameLayout->addWidget(label,       row, column);
    frameLayout->addWidget(sButton,     row + 1, column + 1);
    frameLayout->addWidget(oButton,     row + 1, column + 2);

    QObject::connect(oButton,
            &QToolButton::clicked,
            [&]{
                    QString path= lineEdit->text();
                    const QFileInfo info(path);
                        if (pathCategory == flags::flineframe::isDirectoryPath && ! info.isDir())
                        {
                            Warning(QString("Répertoire"), QString("Le répertoire %1 n'a pas été créé").arg(path));
                            return;
                        }
                        if (pathCategory == flags::flineframe::isFilePath)
                        {
                              path = info.path();
                        }

                        common::openDir(path);
               });

    QObject::connect(sButton,
            &QToolButton::clicked,
            [&, titre]{
                   QString path= lineEdit->text();
                   const QFileInfo info(path);
                   if (pathCategory == flags::flineframe::isDirectoryPath)
                   {
                       if ((path = common::openDirDialog(check)) == nullptr) return;
                   }
                   else
                       if (pathCategory == flags::flineframe::isFilePath)
                       {
                           QString dirpath;

                          if (info.isFile())
                          {
                              dirpath = info.path();
                          }
                          else
                          if (info.isDir())
                          {
                              dirpath = path;
                          }
                          else
                              dirpath = QDir::currentPath();

                          path = QFileDialog::getSaveFileName(nullptr, titre.at(1), dirpath, "Fichier Log (*.log)");

                          if (path.isNull() || path.isEmpty())
                           return;
                       }

                    lineEdit->setText(path);
               });
}

