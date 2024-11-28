#include "device.h"
#include <QDate>
#include <iostream>
#include <QDebug>

Device::Device(QObject *parent)
    : QObject{parent}
{
    for (int i = 0; i < NUM_USERS; i++){
        users[i] = NULL;
    }

}

void Device::createUser(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password)
{
    for (int i = 0; i < NUM_USERS; i++){
        if (users[i] == NULL){
            users[i] = new User(nextID++, fName, lName, sex, weight, height, date, phoneNum, email, password);
            qDebug() << "User created.";
            emit userCreated(1, fName + " " + lName);
            break;
        }
    }
}
