//#include <QWidget>
#ifndef FLINEFRAME_HPP
#define FLINEFRAME_HPP
#include "fwidgets.h"
#include "fstring.h"

class FLineFrame : public FLineEdit
{
Q_OBJECT

public:

FLineFrame(const QString& titre,
           const QString& defaut,
           const QString& xmlTag,
           std::pair<int, int> coord=std::make_pair(0,0),
           const QString& commandLine ="");

private:

FLineEdit* lineEdit;
QLabel *label;
QToolDirButton *sButton;
QToolDirButton *oButton;
};

#endif
