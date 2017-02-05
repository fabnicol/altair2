#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QLabel>

class Widget : public QMainWindow
{
    Q_OBJECT
    
public:
    Widget(int x);

private:
    QLabel* a;
    
};

#endif // WIDGET_H
