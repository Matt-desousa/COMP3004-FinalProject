#include "mainwindow.h"
#include "device.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Device* device = new Device();

    QApplication a(argc, argv);
    MainWindow w(device);
//    w.show();
    return a.exec();
}


/////Evan was here again
////Rach
