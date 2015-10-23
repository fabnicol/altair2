#include "flineframe.hpp"
#include "common.h"
#include <QWidget>
#include "fwidgets.h"
#include "enums.h"
#if 1
FLineFrame::FLineFrame(const QStringList& titre,
                       const QString& defaut,
                       const QString& xmlTag,
                       const QList<int>& coord,
                       QGridLayout* inputLayout,
                       const QString& commandline,
                       flags::directory check)
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
                            Warning0(QString("Répertoire"), QString("Le répertoire %1 n'a pas été créé").arg(path));
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
                   QString path;
                   QFileInfo info(path);
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
#endif
