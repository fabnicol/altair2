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
    sButton = new QToolDirButton("Sélectionner le répertoire");
    oButton = new QToolDirButton("Ouvrir le répertoire ", actionType::OpenFolder);

    componentList = {sButton, oButton, label, lineEdit};
    frameLayout->addWidget(lineEdit,    row + 1, column);
    frameLayout->addWidget(label,       row, column);
    frameLayout->addWidget(sButton,     row + 1, column + 1);
    frameLayout->addWidget(oButton,     row + 1, column + 2);

    QObject::connect(oButton,
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
                            this->openDir(info.path());
                    }
                    else
                    openDir(path);
               });

    QObject::connect(sButton,
            &QToolButton::clicked,
            [&]{
                   QString path;
                   if ((path = openDirDialog(check)) == nullptr) return;
                   lineEdit->setText(path);
               });

}
#endif
