#include "widget.h"
#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (argc < 2) return -1;
    Widget w(atoi(argv[1]));
    w.show();
    
    QTimer::singleShot(6400, &w, SLOT(close()));    
    return a.exec();
}
