#include "message.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setFont (QFont ("Verdana", 12));

    QMessageBox::critical(nullptr, "Attention !", "Le PC va bientôt s'arrêter (actualisation en cours).<br>Veuillez patienter jusqu'à l'arrêt.");

    return a.exec();
}
