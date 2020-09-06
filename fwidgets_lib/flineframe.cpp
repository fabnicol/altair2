
// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle (CPI).

// L'auteur se réserve le droit d'exploitation du présent logiciel, 
// et notamment de reproduire et de modifier le logiciel, conformément aux 
// dispositions de l'article L.122-6 du même code. L'auteur se réserve le droit
// de modifier et de corriger les erreurs du logiciel, conformément aux termes 
// du deuxième alinéa du I de l'article L.122-6-1 du même code.

// Le présent logiciel doit être regardé comme un "Module Externe", pour 
// l'application des stipulations de la licence CeCILL, lorsque le code source, 
// ou une bibliothèque compilée à partir du présent code, sont utilisés pour 
// compiler tout logiciel faisant usage du présent code.

// Tout fichier contenant le présent code, ou des versions antérieures du 
// présent code, et faisant apparaître des termes contraires aux stipulations
// précédentes doit être regardé comme faisant partie d'actes préparatoires à
// l'élaboration du présent logiciel et non comme le logiciel lui-même, ou 
// comme des erreurs matérielles dans la copie des licences. Seules les 
// stipulations précédentes doivent être regardées comme s'appliquant au 
// présent logiciel.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de ces stipulations et que vous en avez accepté les
// termes.

// Sans préjudice des dispositions du CPI, une autorisation d'usage et de
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières pour l'exercice de leurs fonctions publiques.
// Le code ainsi mis à disposition ne peut être transmis à d'autres utilisateurs.
//

//
#include "flineframe.h"
#include "tools.h"
#include "enums.h"

#include <QWidget>
#include "fwidgets.h"


FLineFrame::FLineFrame(const QStringList& titre,
                       const QString& defaut,
                       const QString& xmlTag,
                       const QList<int>& coord,
                       QGridLayout* inputLayout,
                       const QString& commandline,
                       flags::directory check,
                       flags::flineframe category,
                       const QString& filter)
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

    QObject::connect(oButton,
            &QToolButton::clicked,
            [&]{
                    QString path= lineEdit->text();
                    const QFileInfo info(path);
                        if (pathCategory == flags::flineframe::isDirectoryPath && ! info.isDir())
                        {
                            QMessageBox::warning(this, QString("Répertoire"), QString("Le répertoire %1 n'a pas été créé").arg(path));
                            return;
                        }
                        if (pathCategory == flags::flineframe::isFilePath)
                        {
                              path = info.path();
                        }

                        tools::openDir(path);
               });

    
    QObject::connect(sButton,
            &QToolButton::clicked,
            [&, titre, filter]{
                   QString path= lineEdit->text();
                   const QFileInfo info(path);
                   if (pathCategory == flags::flineframe::isDirectoryPath)
                   {
                       if ((path = tools::openDirDialog(check)) == nullptr) return;
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
                              dirpath = tools::userdatadir;
                          
                          if (saveFileName)
                              path = QFileDialog::getSaveFileName(nullptr, titre.at(1), dirpath, filter);
                          else
                              path = QFileDialog::getOpenFileName(nullptr, titre.at(1), dirpath, filter);

                          if (path.isNull() || path.isEmpty())
                           return;
                          else
                              emit(textChanged(path));
                       }

                    lineEdit->setText(path);
               });
    
    
    componentList = {sButton, oButton, label, lineEdit};
    frameLayout->addWidget(lineEdit,    row + 1, column);
    frameLayout->addWidget(label,       row, column, Qt::AlignBottom);
    frameLayout->addWidget(sButton,     row + 1, column + 1);
    frameLayout->addWidget(oButton,     row + 1, column + 2);
}

