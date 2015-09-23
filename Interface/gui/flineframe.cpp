#include "flineframe.hpp"
#include "common.h"
#include <QWidget>
#include "fwidgets.h"
#include "enums.h"

FLineFrame::FLineFrame(const QString& titre,
                       const QString& defaut,
                       const QString& xmlTag,
                       std::pair<int, int> coord,
                       const QString& commandline,
                       flags::directory check)
{
    QGridLayout *baseLayout= new QGridLayout;
    if (baseLayout == nullptr) return;
    int row = coord.first;
    int column = coord.second;
    const QStringList qstrl = QStringList() << titre << "Répertoire de" + titre;

    lineEdit = commandline.isEmpty()?  new FLineEdit(defaut, xmlTag, qstrl) : new FLineEdit(defaut, xmlTag, qstrl, commandline);
    label = new QLabel("Répertoire de" + titre);
    sButton = new QToolDirButton("Sélectionner le répertoire de" + titre);
    oButton = new QToolDirButton("Ouvrir le répertoire ", actionType::OpenFolder);

    baseLayout->addWidget(lineEdit,    row + 1, column);
    baseLayout->addWidget(label,       row, column);
    baseLayout->addWidget(sButton,     row + 1, column + 1);
    baseLayout->addWidget(oButton,     row + 1, column + 2);

    connect(oButton,
            &QToolButton::clicked,
            [&]{
                    const QString &path= lineEdit->text();
                    QFileInfo info(path);
                    if (info.isDir() == false)
                    {
                        if (info.isFile() == false)
                        {
                            Warning0(QString("Répertoire"), QString("Le répertoire ou le fichier %1 n'a pas été créé").arg(path));
                            return;
                        }

                        else
                            common::openDir(info.path());
                    }
                    else
                    openDir(path);
               });

    connect(oButton,
            &QToolButton::clicked,
            [&]{
                   QString path;
                   if ((path = openDirDialog(check)) == nullptr) return;
                   lineEdit->setText(path);
               });

}
