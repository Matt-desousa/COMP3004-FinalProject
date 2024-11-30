#ifndef USER_H
#define USER_H

#include <QObject>
#include <QDate>
#include <string>
using namespace std;

#include "defs.h"

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(int userID, string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QObject *parent = nullptr);
    virtual ~User();
    void update();
    bool verifyPassword(string password);

    string getName();


private:
    int userID;
    string fName;
    string lName;
    QDate dob;
    float height;
    float weight;
    SEX sex;
    string phoneNumber;
    string email;
    string password;

signals:

};

#endif // USER_H
