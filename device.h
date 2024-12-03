#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <list>

#include "defs.h"
#include "user.h"
#include "battery.h"
#include "readingstorage.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    void startBattery(QCheckBox* charging_port_UI, QProgressBar* charging_indicator_UI);
    bool createUser(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password);
    bool verifyUser(string password, int index);
    void userLogout();

    void getUserNames(list<string>* names);

    void printUsers();

    User* currentUser; //moved this to test access -Evan

private:
    Battery* battery;
    list<User*> users;
    int nextID = 0;

signals:
    void userCreated(string name);
    void userLogin(string name);
    void userUpdated(string name);
    void userDeleted();

private slots:
    void showCurrentUserProfile();
    void onUserDeleted();
};

#endif // DEVICE_H
