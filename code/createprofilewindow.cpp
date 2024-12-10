#include "createprofilewindow.h"
#include "ui_createprofilewindow.h"

#include <QDebug>

CreateProfileWindow::CreateProfileWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateProfileWindow)
{
    ui->setupUi(this);

}

CreateProfileWindow::~CreateProfileWindow()
{
    delete ui;
}
