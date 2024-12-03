#include "device.h"
#include <QDate>
#include <iostream>
#include <QDebug>

Device::Device(QObject *parent)
    : QObject{parent}
{
    currentProfile = NULL;
    createProfile("Test", "User", UNDEFINED, 50, 175, QDate(), "911", "x@y.z", "test");
}

void Device::startBattery(QCheckBox* charging_port_UI, QProgressBar* charging_indicator_UI)
{
    battery = new Battery(charging_port_UI, charging_indicator_UI);
    battery->turn_on_or_off(true); // start using battery power
}

bool Device::createProfile(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password)
{

    if (fName == "" || lName == "" || weight <= 0 || height <= 0 || phoneNum == "" || email == "" || password == ""){
        qDebug() << "Invalid input.";
        return false;
    }

    qDebug() << profiles.size();
    if (profiles.size() < NUM_USERS){
        profiles.push_back(new Profile(nextID++, fName, lName, sex, weight, height, date, phoneNum, email, password));
        qDebug() << "User created.";
        emit profileCreated(fName + " " + lName);
        return true;
    }
    else {
        qDebug() << "Maximum users reached.";
        return false;
    }
}

bool Device::verifyProfile(string password, int index)
{
    list<Profile*>::iterator it;
    it = profiles.begin();
    for (int i = 0; i < index; i++){
        it++;
    }

    if ((*it)->verifyPassword(password)){
        qDebug() << "User verified.";

        currentProfile = *it;
        connect(currentProfile, SIGNAL(profileUpdated(string)), this, SIGNAL(profileUpdated(string)));
        connect(currentProfile, SIGNAL(profileDeleted()), this, SLOT(onProfileDeleted()));

        emit profileLogin(currentProfile->getName());
        return true;
    }
    else {
        qDebug() << "Incorrect password.";
        return false;
    }
}

void Device::logoutProfile()
{
    if (currentProfile != NULL){
        disconnect(currentProfile, SIGNAL(profileUpdated(string)), this, SIGNAL(profileUpdated(string)));
        disconnect(currentProfile, SIGNAL(profileDeleted()), this, SLOT(onProfileDeleted()));
    }
    currentProfile = NULL;
}

void Device::getProfileNames(list<string> *names)
{
    for (Profile* user : profiles){
        names->push_back(user->getName());
    }
}

void Device::printProfiles()
{
    list<Profile*>::iterator it;
    it = profiles.begin();
    for (it; it != profiles.end(); it++){
        qDebug() << QString::fromStdString((*it)->getName());
    }
}

void Device::showCurrentProfile()
{
    currentProfile->showProfile();
}

void Device::onProfileDeleted()
{
    printProfiles();

    list<Profile*>::iterator it;
    it = profiles.begin();
    for (it; it != profiles.end(); it++){
        if ((*it) == currentProfile){
            profiles.remove(currentProfile);
            emit profileDeleted();
            break;
        }
    }

    printProfiles();
}
