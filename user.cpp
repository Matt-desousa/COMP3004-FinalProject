#include <QDebug>

#include "user.h"

User::User(int userID, string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QObject *parent)
    : QObject{parent}
{

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

}

User::~User(){

}

void User::update(){

}

bool User::verifyPassword(string password)
{
    if (!this->password.compare(password)){
        return true;
    }
    else {
        return false;
    }
}

string User::getName()
{
    return fName + " " + lName;
}
