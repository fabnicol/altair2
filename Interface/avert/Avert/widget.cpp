#include "widget.h"

#include <QIcon>
#include <QLabel>

Widget::Widget(int x) 
{

    setGeometry(QRect(100 + x, 150, 200, 80));

    a = new QLabel;           
    setWindowIcon(QIcon(":/altair.png"));
    a->setText(("<br><b>&nbsp;&nbsp;<span style=\"color: red;\">Lancement</span></b><br><br><b>&nbsp;&nbsp;Analyse du disque optique...</b><br><br>"));
    setCentralWidget(a);
    repaint();
    
 }



   

