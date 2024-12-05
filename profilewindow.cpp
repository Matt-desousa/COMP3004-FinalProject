#include "profilewindow.h"
#include "ui_profilewindow.h"

#include <QDebug>
#include <iostream>
using namespace std;

ProfileWindow::ProfileWindow(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileWindow)
{
    ui->setupUi(this);

    ui->txtFName->setText(QString::fromStdString(fName));
    ui->txtLName->setText(QString::fromStdString(lName));

    if (sex == FEMALE) ui->rbtnFemale->setChecked(true);

    ui->dsbWeight->setValue(weight);
    ui->dsbHeight->setValue(height);
    ui->datDOB->setDate(date);
    ui->txtPhoneNum->setText(QString::fromStdString(phoneNum));
    ui->txtEmail->setText(QString::fromStdString(email));
    ui->txtPass->setText(QString::fromStdString(password));
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}










