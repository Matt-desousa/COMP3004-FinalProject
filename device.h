#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include "defs.h"
#include "user.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);
    void createUser(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password);

private:
    User* users[NUM_USERS];
    int nextID = 0;

signals:
    void userCreated(int add, string name);
};

#endif // DEVICE_H
