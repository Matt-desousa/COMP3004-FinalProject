#include "device.h"
#include <QDate>
#include <iostream>
#include <QDebug>

#include "ui_createprofilewindow.h"
#include "ui_loginwindow.h"
#include "ui_mainwindow.h"

Device::Device(QObject *parent)
    : QObject{parent}
{
    mainWindow = new MainWindow;
    loginWindow = new LoginWindow;
    createWindow = new CreateProfileWindow;

    mainWindow->getUI(&mwUI);
    loginWindow->getUI(&lwUI);
    createWindow->getUI(&cwUI);

    /*
        Main Window Signals and Slots
    */
    // Logout Profile
    connect(mwUI->btnLogOut, SIGNAL(pressed()), this, SLOT(onProfileLogout()));

    // Profile button
    connect(mwUI->btnProfile, SIGNAL(pressed()), this, SLOT(showCurrentProfile()));

    // Save Notes
    connect(mwUI->saveBtn, SIGNAL(pressed()), this, SLOT(saveNotes()));

    connect(mwUI->note_next, &QRadioButton::released, this, &Device::updateNotes);
    connect(mwUI->note_previous, &QRadioButton::released, this, &Device::updateNotes);

    connect(mwUI->ChartSelection, &QComboBox::currentTextChanged, this, &Device::updateChart);


    //debug
    //DELETE LATER
    mwUI->result->setEnabled(false);
    connect(mwUI->result,SIGNAL(pressed()),this,SLOT(processRyodorakuData()));


    /*
        Login Window Signals and Slots
    */
    // Login Button
    connect(lwUI->btnLogin, SIGNAL(pressed()), this, SLOT(onProfileLogin()));

    // Create Profile Button
    connect(lwUI->btnCreateProfile, SIGNAL(pressed()), createWindow, SLOT(show()));
    connect(lwUI->btnCreateProfile, SIGNAL(pressed()), loginWindow, SLOT(hide()));

    // Profile Updated
    connect(this, SIGNAL(profileUpdated(string)), this, SLOT(onProfileUpdate(string)));

    // Profile Deleted
    connect(this, SIGNAL(profileDeleted()), this, SLOT(onProfileDeleted()));


    /*
        Create Profile Window Signals and Slots
    */
    // Cancel Button
    connect(cwUI->btnCancel, SIGNAL(pressed()), loginWindow, SLOT(show()));
    connect(cwUI->btnCancel, SIGNAL(pressed()), createWindow, SLOT(hide()));

    // Create Profile Button
    connect(cwUI->btnCreateProfile, SIGNAL(pressed()), this, SLOT(onProfileCreated()));

    startBattery(mwUI->isCharging, mwUI->ChargeIndicator);

    currentProfile = NULL;
    createProfile("Test", "User", UNDEFINED, 50, 175, QDate(), "911", "x@y.z", "test");

    loginWindow->show();
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
        createWindow->hide();

        lwUI->cmbProfile->addItem(QString::fromStdString(fName + " " + lName));
        loginWindow->show();
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
    for (;it != profiles.end(); it++){
        qDebug() << QString::fromStdString((*it)->getName());
    }
}

void Device::onProfileCreated()
{
    string fName = cwUI->txtFName->text().toStdString();
    string lName = cwUI->txtLName->text().toStdString();
    float weight = cwUI->dsbWeight->value();
    float height = cwUI->dsbHeight->value();
    QDate date = cwUI->datDOB->date();
    string phoneNum = cwUI->txtPhoneNum->text().toStdString();
    string email = cwUI->txtEmail->text().toStdString();
    string password = cwUI->txtPass->text().toStdString();
    string conPassword = cwUI->txtConPass->text().toStdString();

    SEX sex;
    if (cwUI->rbtnMale->isChecked()){
        sex = MALE;
    }
    else if (cwUI->rbtnFemale->isChecked()){
        sex = FEMALE;
    }
    else sex = UNDEFINED;

    if (password.compare(conPassword)){
        qDebug() << "Passwords do not match.";
        cwUI->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        cwUI->txtConPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        return;
    }

    bool validUser = createProfile(fName, lName, sex, weight, height, date, phoneNum, email, password);

    if (!validUser){
        if (fName == "") cwUI->txtFName->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtFName->setStyleSheet("");
        if (lName == "") cwUI->txtLName->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtLName->setStyleSheet("");
        if (weight <= 0) cwUI->dsbWeight->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->dsbWeight->setStyleSheet("");
        if (height <= 0) cwUI->dsbHeight->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->dsbHeight->setStyleSheet("");
        if (phoneNum == "") cwUI->txtPhoneNum->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtPhoneNum->setStyleSheet("");
        if (email == "") cwUI->txtEmail->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtEmail->setStyleSheet("");
        if (password == "") cwUI->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtPass->setStyleSheet("");
        if (conPassword == "") cwUI->txtConPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtConPass->setStyleSheet("");
    }
    else {
        cwUI->txtFName->setStyleSheet("");
        cwUI->txtLName->setStyleSheet("");
        cwUI->dsbWeight->setStyleSheet("");
        cwUI->dsbHeight->setStyleSheet("");
        cwUI->txtPhoneNum->setStyleSheet("");
        cwUI->txtEmail->setStyleSheet("");
        cwUI->txtPass->setStyleSheet("");
        cwUI->txtConPass->setStyleSheet("");
    }
}

void Device::onProfileLogin()
{
    string password = lwUI->txtLoginPass->text().toStdString();
    int index = lwUI->cmbProfile->currentIndex();
    bool loggedIn = verifyProfile(password, index);

    if (loggedIn){
        mainWindow->show();
        mwUI->lblCurrentUser->setText(QString::fromStdString(currentProfile->getName()));

        mainWindow->addData(currentProfile);
        mainWindow->display_note(currentProfile);

        loginWindow->hide();
        lwUI->txtLoginPass->setStyleSheet("");
    }
    else{
        lwUI->txtLoginPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
    }

    lwUI->txtLoginPass->setText("");
}

void Device::onProfileLogout()
{
    mainWindow->hide();
    mwUI->lblCurrentUser->setText(QString::fromStdString(""));
    loginWindow->show();
}

void Device::onProfileUpdate(string name)
{
    qDebug() << "User Updated.";
    int index = lwUI->cmbProfile->currentIndex();
    lwUI->cmbProfile->setItemText(index, QString::fromStdString(name));
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
    for (;it != profiles.end(); it++){
        if ((*it) == currentProfile){
            profiles.remove(currentProfile);

            qDebug() << "User Deleted.";
            int index = lwUI->cmbProfile->currentIndex();
            lwUI->cmbProfile->removeItem(index);
            loginWindow->show();

            mainWindow->hide();

            break;
        }
    }

    printProfiles();
}

void Device::processRyodorakuData()
{
    mainWindow->processRyodorakuData(currentProfile);
}

void Device::saveNotes()
{
    mainWindow->saveNotes(currentProfile);
}

void Device::updateNotes()
{
    mainWindow->display_note(currentProfile);
}

void Device::updateChart(){
    mainWindow->update_chart(currentProfile);
}
