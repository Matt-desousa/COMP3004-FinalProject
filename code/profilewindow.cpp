#include "profilewindow.h"
#include "ui_profilewindow.h"

#include <QDebug>
#include <iostream>
using namespace std;

ProfileWindow::ProfileWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileWindow)
{
    ui->setupUi(this);
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}










