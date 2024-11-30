#include "profilewindow.h"
#include "ui_profilewindow.h"

ProfileWindow::ProfileWindow(Device* device, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileWindow)
{
    ui->setupUi(this);

    this->device = device;

    // Cancel button
    connect(ui->btnCancel, SIGNAL(pressed()), parent, SLOT(show()));

    // Login Profile
    connect(device, SIGNAL(userLogin(string)), this, SLOT(onUserLogin(string)));
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}

void ProfileWindow::onUserLogin(string name)
{

}
