#include "device.h"
#include <QDate>
#include <iostream>
#include <QDebug>

Device::Device(QObject *parent)
    : QObject{parent}
{
    currentUser = NULL;
    createUser("Test", "User", UNDEFINED, 50, 175, QDate(), "911", "x@y.z", "test");
}

void Device::startBattery(QCheckBox* charging_port_UI, QProgressBar* charging_indicator_UI)
{
    battery = new Battery(charging_port_UI, charging_indicator_UI);
    battery->turn_on_or_off(true); // start using battery power
}

bool Device::createUser(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password)
{

    if (fName == "" || lName == "" || weight <= 0 || height <= 0 || phoneNum == "" || email == "" || password == ""){
        qDebug() << "Invalid input.";
        return false;
    }

    qDebug() << users.size();
    if (users.size() < NUM_USERS){
        users.push_back(new User(nextID++, fName, lName, sex, weight, height, date, phoneNum, email, password));
        qDebug() << "User created.";
        emit userCreated(fName + " " + lName);
        return true;
    }
    else {
        qDebug() << "Maximum users reached.";
        return false;
    }
}

bool Device::verifyUser(string password, int index)
{
    list<User*>::iterator it;
    it = users.begin();
    for (int i = 0; i < index; i++){
        it++;
    }

    if ((*it)->verifyPassword(password)){
        qDebug() << "User verified.";

        currentUser = *it;
        connect(currentUser, SIGNAL(userUpdated(string)), this, SIGNAL(userUpdated(string)));
        connect(currentUser, SIGNAL(userDeleted()), this, SLOT(onUserDeleted()));

        emit userLogin(currentUser->getName());
        return true;
    }
    else {
        qDebug() << "Incorrect password.";
        return false;
    }
}

void Device::userLogout()
{
    if (currentUser != NULL){
        disconnect(currentUser, SIGNAL(userUpdated(string)), this, SIGNAL(userUpdated(string)));
        disconnect(currentUser, SIGNAL(userDeleted()), this, SLOT(onUserDeleted()));
    }
    currentUser = NULL;
}

void Device::getUserNames(list<string> *names)
{
    for (User* user : users){
        names->push_back(user->getName());
    }
}

void Device::printUsers()
{
    list<User*>::iterator it;
    it = users.begin();
    for (it; it != users.end(); it++){
        qDebug() << QString::fromStdString((*it)->getName());
    }
}

void Device::showCurrentUserProfile()
{
    currentUser->showProfile();
}

void Device::onUserDeleted()
{
    printUsers();

    list<User*>::iterator it;
    it = users.begin();
    for (it; it != users.end(); it++){
        if ((*it) == currentUser){
            users.remove(currentUser);
            emit userDeleted();
            break;
        }
    }

    printUsers();
}
