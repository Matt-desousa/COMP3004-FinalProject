#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <list>

#include "defs.h"
#include "profile.h"
#include "battery.h"
#include "readingstorage.h"

#include "mainwindow.h"
#include "loginwindow.h"
#include "createprofilewindow.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    void startBattery(QCheckBox* charging_port_UI, QProgressBar* charging_indicator_UI);
    bool createProfile(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password);
    bool verifyProfile(string password, int index);
    void logoutProfile();

    void getProfileNames(list<string>* names);

    void printProfiles();

    Profile* currentProfile; //moved this to test access -Evan

private:
    MainWindow* mainWindow;
    Ui::MainWindow* mwUI;
    LoginWindow* loginWindow;
    Ui::LoginWindow* lwUI;
    CreateProfileWindow* createWindow;
    Ui::CreateProfileWindow* cwUI;

    Battery* battery;
    list<Profile*> profiles;
    int nextID = 0;

signals:
    void profileCreated(string name);
    void profileLogin(string name);
    void profileUpdated(string name);
    void profileDeleted();

private slots:
    void onProfileCreated();
    void onProfileLogin();
    void onProfileLogout();
    void onProfileUpdate(string name);
    void showCurrentProfile();
    void onProfileDeleted();

    void processRyodorakuData();
    void updateNotes();
    void saveNotes();
    void updateChart();
};

#endif // DEVICE_H
