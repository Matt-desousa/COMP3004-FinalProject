#include "profilewindow.h"
#include "ui_profilewindow.h"

#include <QDebug>
#include <iostream>
using namespace std;

ProfileWindow::ProfileWindow(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProfileWindow)
{
    this->parent = (User*) parent;

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

    // Confirm changes button
    connect(ui->btnConfirm, SIGNAL(pressed()), this, SLOT(onConfirmButtonPressed()));

    // Delete Profile button
    connect(ui->btnDeleteProfile, SIGNAL(pressed()), this, SIGNAL(onDeleteUserPressed()));
}

ProfileWindow::~ProfileWindow()
{
    delete ui;
}

void ProfileWindow::onConfirmButtonPressed()
{

    string fName = ui->txtFName->text().toStdString();
    string lName = ui->txtLName->text().toStdString();
    float weight = ui->dsbWeight->value();
    float height = ui->dsbHeight->value();
    QDate date = ui->datDOB->date();
    string phoneNum = ui->txtPhoneNum->text().toStdString();
    string email = ui->txtEmail->text().toStdString();
    string password = ui->txtPass->text().toStdString();
    string conPassword = ui->txtConPass->text().toStdString();

    SEX sex;
    if (ui->rbtnMale->isChecked()){
        sex = MALE;
    }
    else if (ui->rbtnFemale->isChecked()){
        sex = FEMALE;
    }
    else sex = UNDEFINED;

    if (password.compare(conPassword)){
        qDebug() << "Passwords do not match.";
        ui->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        ui->txtConPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        return;
    }

    bool validUser = true;

    if (fName == "") {ui->txtFName->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else ui->txtFName->setStyleSheet("");
    if (lName == "") {ui->txtLName->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else ui->txtLName->setStyleSheet("");
    if (weight <= 0) {ui->dsbWeight->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else ui->dsbWeight->setStyleSheet("");
    if (height <= 0) {ui->dsbHeight->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else ui->dsbHeight->setStyleSheet("");
    if (phoneNum == "") {ui->txtPhoneNum->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else ui->txtPhoneNum->setStyleSheet("");
    if (email == "") {ui->txtEmail->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else ui->txtEmail->setStyleSheet("");
    if (password == "") {ui->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else ui->txtPass->setStyleSheet("");
    if (conPassword == "") {ui->txtConPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else ui->txtConPass->setStyleSheet("");

    if (validUser) emit updateProfile(fName, lName, sex, weight, height, date, phoneNum, email, password);
}










