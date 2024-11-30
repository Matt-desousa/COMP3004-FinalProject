#include "device.h"
#include <QDate>
#include <iostream>
#include <QDebug>

Device::Device(QObject *parent)
    : QObject{parent}
{
//    mainWindow = new MainWindow;
//    loginWindow = new LoginWindow;
//    createProfileWindow = new CreateProfileWindow;


    currentUser = NULL;
    createUser("Test", "User", UNDEFINED, 50, 175, QDate(), "911", "x@y.z", "test");
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

void Device::verifyUser(string password, int index)
{
    list<User*>::iterator it;
    it = users.begin();
    for (int i = 0; i < index; i++){
        it++;
    }

    if ((*it)->verifyPassword(password)){
        qDebug() << "User verified.";
        currentUser = *it;
        emit userLogin(currentUser->getName());
    }
    else {
        qDebug() << "Incorrect password.";
    }
}

void Device::userLogout()
{
    currentUser = NULL;
}

void Device::getUserNames(list<string> *names)
{
    for (User* user : users){
        names->push_back(user->getName());
    }
}
