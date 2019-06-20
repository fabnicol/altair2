#include "message.h"
#include <QApplication>
#include <QStyle>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow *w = new QMainWindow;
    QTextEdit *edit = new QTextEdit(nullptr);
    w->setCentralWidget(edit);
    w->setWindowTitle("Attention !");
    w->setWindowIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxCritical));

    edit->insertHtml("<html><br><br>"
    "<b><span style='color:red;'>Le PC va bient�t s'arr�ter (actualisation en cours).</span></b>"
    "<br><br>Veuillez patienter jusqu'� l'arr�t..."
    "<br>Ne pas �teindre le PC, merci.</b></html>");
    edit->setFont (QFont ("Verdana", 12));
    w->show();
    w->setGeometry (400, 600, 500, 150);

    return a.exec();
}
