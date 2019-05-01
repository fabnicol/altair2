#include "message.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRect rec = QApplication::desktop()->screenGeometry();
    int width0 = rec.width();
    int fontsize = (width0 > 2000) ? width0 / 1920 * 10 : 10;
    a.setFont (QFont ("Verdana", fontsize));

    QMessageBox::critical(nullptr, "Attention !", "Le PC va bientôt s'arrêter (actualisation en cours).<br>Veuillez vous déconnecter immédiatement.");

    return a.exec();
}
