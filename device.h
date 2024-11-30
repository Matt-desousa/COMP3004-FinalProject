#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <list>

#include "defs.h"
#include "user.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    bool createUser(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password);
    void verifyUser(string password, int index);
    void userLogout();

    void getUserNames(list<string>* names);

private:
    list<User*> users;
    int nextID = 0;
    User* currentUser;

signals:
    void userCreated(string name);
    void userLogin(string name);
};

#endif // DEVICE_H
