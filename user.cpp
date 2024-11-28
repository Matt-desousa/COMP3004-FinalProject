#include "user.h"

User::User(int userID, string fName, string lName, int age, float height, float weight, SEX sex, QObject *parent)
    : QObject{parent}
{

    this->userID = userID;
    this->fName = fName;
    this->lName = lName;
    this->age = age;
    this->height = height;
    this->weight = weight;
    this->sex = sex;

}

User::~User(){

}

void User::update(){

}