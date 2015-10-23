//#include <QWidget>
#ifndef FLINEFRAME_HPP
#define FLINEFRAME_HPP
#include "fwidgets.h"
#include "fstring.h"
#include "common.h"
#include "enums.h"

#if 1
class FLineFrame : public FLineEdit
{
Q_OBJECT

public:

//FLineFrame() : FLineFrame("","","NoDesc") {}
FLineFrame(const QStringList& ={"", ""},
           const QString& ="",
           const QString& ="",
           const QList<int>& pair = {0,0},
           QGridLayout* inputLayout = nullptr,
           const QString& commandLine ="",
           flags::directory check = directory::checkEmptyness);


QString getText(){ return lineEdit->text();}
QGridLayout* getLayout(){ return frameLayout;}
// on a dû la réimplémenter pour des raisons obscures. Pourtant FLineEdit dérive de FAbstractWidget...
QList<QWidget*> getComponentList() {return componentList;}
void setPathCategory(flags::flineframe t)
     {
       pathCategory = t;
       sButton->setText("Sélectionner le fichier");
     }

private:

QGridLayout* frameLayout;
FLineEdit* lineEdit;
QLabel *label;
QToolDirButton *sButton;
QToolDirButton *oButton;
QList<QWidget*> componentList;
flags::flineframe pathCategory;
};
#endif
#endif
