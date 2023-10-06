#include "QWalkingMaps.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWalkingMaps w;
    w.show();
    return a.exec();
}
