#include <QDebug>
#include <QMessageBox>
#include <iostream>
using namespace std;

#include "profile.h"

#include "ui_profilewindow.h"
#include "ui_confirmdeletepopup.h"

Profile::Profile(int userID, string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QObject *parent)
    : QObject{parent}
{
    // Init the windows and UIs
    profileWindow = new ProfileWindow;
    popup = new ConfirmDeletePopup;

    profileWindow->getUI(&pwUI);
    popup->getUI(&cdUI);

    // Set the properties.
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

    initProfileWindow();

    // Confirm Changes Button Pressed
    connect(pwUI->btnConfirm, SIGNAL(pressed()), this, SLOT(onProfileUpdate()));

    // "X" Button Pressed
    connect(profileWindow, SIGNAL(closed()), this, SLOT(onProfileClosed()));

    // Delete Profile Button Pressed
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
    if (!this->password.compare(password)){ // If both passwords match
        return true;
    }
    else {
        return false;
    }
}

void Profile::initProfileWindow()
{
    pwUI->txtFName->setText(QString::fromStdString(fName));
    pwUI->txtLName->setText(QString::fromStdString(lName));

    if (sex == FEMALE) pwUI->rbtnFemale->setChecked(true);
    else pwUI->rbtnMale->setChecked(true);

    pwUI->dsbWeight->setValue(weight);
    pwUI->dsbHeight->setValue(height);
    pwUI->datDOB->setDate(dob);
    pwUI->txtPhoneNum->setText(QString::fromStdString(phoneNumber));
    pwUI->txtEmail->setText(QString::fromStdString(email));
    pwUI->txtPass->setText(QString::fromStdString(password));
}

void Profile::onProfileUpdate()
{
    // Get all of the information from the profileWindow.
    string fName = pwUI->txtFName->text().toStdString();
    string lName = pwUI->txtLName->text().toStdString();
    float weight = pwUI->dsbWeight->value();
    float height = pwUI->dsbHeight->value();
    QDate date = pwUI->datDOB->date();
    string phoneNum = pwUI->txtPhoneNum->text().toStdString();
    string email = pwUI->txtEmail->text().toStdString();
    string password = pwUI->txtPass->text().toStdString();
    string conPassword = pwUI->txtConPass->text().toStdString();

    // Get which radio button is checked.
    SEX sex;
    if (pwUI->rbtnMale->isChecked()){
        sex = MALE;
    }
    else if (pwUI->rbtnFemale->isChecked()){
        sex = FEMALE;
    }
    else sex = UNDEFINED;

    // Check that both passwords match.
    if (password.compare(conPassword)){
        qDebug() << "Passwords do not match.";
        pwUI->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        pwUI->txtConPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        return;
    }

    // Boolean to check if the provided information is valid.
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

    // If all the information is valid, update the information if its been changed.
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

        // Notify the device
        emit profileUpdated(this->fName + " " + this->lName);

        profileWindow->hide();
    }
}

void Profile::onProfileDelete()
{
    string conPass = cdUI->txtPass->text().toStdString();
    if (verifyPassword(conPass)){ // Verifies the password.
        popup->hide();
        profileWindow->hide();
        emit profileDeleted();
    }
    else{
        cdUI->txtPass->setText("");
        QMessageBox msgBox;
        msgBox.setText("Incorrect Password. Cancelling...");
        msgBox.exec();
        popup->hide();
    }
}

void Profile::onProfileClosed()
{
    // Reset the profile window
    initProfileWindow();
}
