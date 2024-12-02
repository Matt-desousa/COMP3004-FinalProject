#include "mainwindow.h"
#include "device.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Device* device = new Device();

    MainWindow w(device);
//    w.show();
    return a.exec();
}


/////Evan was here again
////Rach
