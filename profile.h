#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>
#include <QDate>
#include <string>
using namespace std;

#include "defs.h"

#include "profilewindow.h"
#include "confirmdeletepopup.h"

#include "readingstorage.h"

class ProfileWindow;

class Profile : public QObject
{
    Q_OBJECT
public:
    explicit Profile(int userID, string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QObject *parent = nullptr);
    virtual ~Profile();
    bool verifyPassword(string password);
    void showProfile();

    string getName();

    inline QVector<ReadingStorage*>* getSessions(){return &sessions;}



private:
    ProfileWindow* profileWindow;
    ConfirmDeletePopup* popup;
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

    QVector<ReadingStorage*> sessions;

private slots:
    void updateProfile(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password);
    void verifyProfileForDelete(string);

signals:
    void profileUpdated(string);
    void profileDeleted();

};

#endif // PROFILE_H
