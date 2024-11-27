#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //Setup UI
    ui->setupUi(this);

    //Setup Battery
    battery = new Battery(ui->isCharging,  ui->ChargeIndicator);
    battery->turn_on_or_off(true); //start using battery power


}

MainWindow::~MainWindow()
{
    delete ui;
    delete battery;
}

