#include <QDebug>
#include <iostream>
using namespace std;

#include "profile.h"

Profile::Profile(int userID, string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QObject *parent)
    : QObject{parent}
{
    profileWindow = new ProfileWindow(fName, lName, sex, weight, height, date, phoneNum, email, password);
    popup = new ConfirmDeletePopup;

    this->userID = userID;
    this->fName = fName;
    this->lName = lName;
    this->dob = date;
    this->height = height;
    this->weight = weight;
    this->sex = sex;
    this->phoneNumber = phoneNum;
    this->email = email;
    this->password = password;

    connect(profileWindow, SIGNAL(updateProfile(string, string, SEX, float, float, QDate, string, string, string)), this, SLOT(updateProfile(string, string, SEX, float, float, QDate, string, string, string)));

    // Delete Profile button
    connect(profileWindow, SIGNAL(onDeleteProfilePressed()), popup, SLOT(exec()));

    // Popup Confirm Delete Button Pressed
    connect(popup, SIGNAL(verifyUser(string)), this, SLOT(verifyProfileForDelete(string)));
}

Profile::~Profile(){

}

void Profile::updateProfile(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password)
{
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

void Profile::verifyProfileForDelete(string password)
{
    if (verifyPassword(password)){
        popup->onUserVerified(true);
        emit profileDeleted();
        profileWindow->hide();
    }
    else
        popup->onUserVerified(false);
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

void Profile::showProfile()
{
    profileWindow->show();
}

string Profile::getName()
{
    return fName + " " + lName;
}
