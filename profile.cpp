#include <QDebug>
#include <iostream>
using namespace std;

#include "profile.h"

#include "ui_profilewindow.h"
#include "ui_confirmdeletepopup.h"

Profile::Profile(int userID, string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QObject *parent)
    : QObject{parent}
{
    profileWindow = new ProfileWindow(fName, lName, sex, weight, height, date, phoneNum, email, password);
    popup = new ConfirmDeletePopup;

    profileWindow->getUI(&pwUI);
    popup->getUI(&cdUI);

    this->profileID = userID;
    this->fName = fName;
    this->lName = lName;
    this->dob = date;
    this->height = height;
    this->weight = weight;
    this->sex = sex;
    this->phoneNumber = phoneNum;
    this->email = email;
    this->password = password;

    // Confirm Changes Button
    connect(pwUI->btnConfirm, SIGNAL(pressed()), this, SLOT(onProfileUpdate()));

    // Delete Profile button
    connect(pwUI->btnDeleteProfile, SIGNAL(pressed()), popup, SLOT(exec()));

    // Popup Confirm Delete Button Pressed
    connect(cdUI->btnDelete, SIGNAL(pressed()), this, SLOT(onProfileDelete()));

    // Popup Cancel Button Pressed
    connect(cdUI->btnCancel, SIGNAL(pressed()), popup, SLOT(hide()));
}

Profile::~Profile(){

}

void Profile::showProfile()
{
    profileWindow->show();
}

bool Profile::verifyPassword(string password)
{
    if (!this->password.compare(password)){
        return true;
    }
    else {
        return false;
    }
}

string Profile::getName()
{
    return fName + " " + lName;
}

void Profile::onProfileUpdate()
{
    string fName = pwUI->txtFName->text().toStdString();
    string lName = pwUI->txtLName->text().toStdString();
    float weight = pwUI->dsbWeight->value();
    float height = pwUI->dsbHeight->value();
    QDate date = pwUI->datDOB->date();
    string phoneNum = pwUI->txtPhoneNum->text().toStdString();
    string email = pwUI->txtEmail->text().toStdString();
    string password = pwUI->txtPass->text().toStdString();
    string conPassword = pwUI->txtConPass->text().toStdString();

    SEX sex;
    if (pwUI->rbtnMale->isChecked()){
        sex = MALE;
    }
    else if (pwUI->rbtnFemale->isChecked()){
        sex = FEMALE;
    }
    else sex = UNDEFINED;

    if (password.compare(conPassword)){
        qDebug() << "Passwords do not match.";
        pwUI->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        pwUI->txtConPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        return;
    }

    bool validUser = true;

    if (fName == "") {pwUI->txtFName->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else pwUI->txtFName->setStyleSheet("");
    if (lName == "") {pwUI->txtLName->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else pwUI->txtLName->setStyleSheet("");
    if (weight <= 0) {pwUI->dsbWeight->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else pwUI->dsbWeight->setStyleSheet("");
    if (height <= 0) {pwUI->dsbHeight->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else pwUI->dsbHeight->setStyleSheet("");
    if (phoneNum == "") {pwUI->txtPhoneNum->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else pwUI->txtPhoneNum->setStyleSheet("");
    if (email == "") {pwUI->txtEmail->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else pwUI->txtEmail->setStyleSheet("");
    if (password == "") {pwUI->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else pwUI->txtPass->setStyleSheet("");
    if (conPassword == "") {pwUI->txtConPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red"); validUser = false;}
    else pwUI->txtConPass->setStyleSheet("");

    if (validUser){
        if (this->fName.compare(fName)){
            this->fName = fName;
        }
        if (this->lName.compare(lName)){
            this->lName = lName;
        }
        if (this->dob != date){
            this->dob = date;
        }
        if (this->height != height){
            this->height = height;
        }
        if (this->weight != weight){
            this->weight = weight;
        }
        if (this->phoneNumber.compare(phoneNum)){
            this->phoneNumber = phoneNum;
        }
        if (this->email.compare(email)){
            this->email = email;
        }
        if (this->password.compare(password)){
            this->password = password;
        }
        if (this->sex != sex) {
            this->sex = sex;
        }

        emit profileUpdated(this->fName + " " + this->lName);
    }
}

void Profile::onProfileDelete()
{
    string conPass = cdUI->txtPass->text().toStdString();
    if (verifyPassword(conPass)){
        popup->hide();
        profileWindow->hide();
        emit profileDeleted();
    }
    else{
        cdUI->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        cdUI->txtPass->setText("");
    }
}
