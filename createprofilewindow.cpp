#include "createprofilewindow.h"
#include "ui_createprofilewindow.h"

#include <QDebug>

CreateProfileWindow::CreateProfileWindow(Device* device, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreateProfileWindow)
{
    ui->setupUi(this);

    this->device = device;

    // Cancel button
    connect(ui->btnCancel, SIGNAL(pressed()), parent, SLOT(show()));
    connect(ui->btnCancel, SIGNAL(pressed()), this, SLOT(hide()));

    // Create Profile
    connect(ui->btnCreateProfile, SIGNAL(pressed()), this, SLOT(onCreateProfileButtonPressed()));
    connect(device, SIGNAL(userCreated(string)), this, SLOT(hide()));

//    connect(ui->btnCreateProfile, SIGNAL(pressed()), parent, SLOT(show()));
//    connect(device, SIGNAL(userCreated(string)), parent, SLOT(show()));
}

CreateProfileWindow::~CreateProfileWindow()
{
    delete ui;
}

void CreateProfileWindow::onCreateProfileButtonPressed()
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

    bool validUser = device->createUser(fName, lName, sex, weight, height, date, phoneNum, email, password);

    if (!validUser){
        if (fName == "") ui->txtFName->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else ui->txtFName->setStyleSheet("");
        if (lName == "") ui->txtLName->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else ui->txtLName->setStyleSheet("");
        if (weight <= 0) ui->dsbWeight->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else ui->dsbWeight->setStyleSheet("");
        if (height <= 0) ui->dsbHeight->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else ui->dsbHeight->setStyleSheet("");
        if (phoneNum == "") ui->txtPhoneNum->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else ui->txtPhoneNum->setStyleSheet("");
        if (email == "") ui->txtEmail->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else ui->txtEmail->setStyleSheet("");
        if (password == "") ui->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else ui->txtPass->setStyleSheet("");
        if (conPassword == "") ui->txtConPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else ui->txtConPass->setStyleSheet("");
    }
    else {
        ui->txtFName->setStyleSheet("");
        ui->txtLName->setStyleSheet("");
        ui->dsbWeight->setStyleSheet("");
        ui->dsbHeight->setStyleSheet("");
        ui->txtPhoneNum->setStyleSheet("");
        ui->txtEmail->setStyleSheet("");
        ui->txtPass->setStyleSheet("");
        ui->txtConPass->setStyleSheet("");
    }
}
