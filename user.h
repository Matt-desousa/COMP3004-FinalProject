#ifndef USER_H
#define USER_H

#include <QObject>
#include <string>
using namespace std;

#include "defs.h"

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(int userID, string fName, string lName, int age, float height, float weight, SEX sex, QObject *parent = nullptr);
    virtual ~User();
    void update();


private:
    int userID;
    string fName;
    string lName;
    int age;
    float height;
    float weight;
    SEX sex;
    string phoneNumber;
    string email;
    string password;

signals:

};

#endif // USER_H
