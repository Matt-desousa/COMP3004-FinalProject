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

class Profile : public QObject
{
    Q_OBJECT
public:
    explicit Profile(int userID, string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QObject *parent = nullptr);
    virtual ~Profile();

    /*
        Verifies the profile with the given password.
        Returns true if the stored password and given password match.
    */
    bool verifyPassword(string password);

    void initProfileWindow();

    /*
        Shows the profileWindow of the profile.
    */
    void showProfile();

    inline string getName(){return fName + " " + lName;}; // Returns the first and last name of the profile.
    inline QVector<ReadingStorage*>* getSessions(){return &sessions;} // Returns the profiles sessions.
    inline void log_session(ReadingStorage* reading){sessions.push_front(reading);} // Pushes a new reading to the profiles sessions for storage.

private:
    // Windows and UI
    ProfileWindow* profileWindow;
    Ui::ProfileWindow* pwUI;
    ConfirmDeletePopup* popup;
    Ui::ConfirmDeletePopup* cdUI;

    // Properties of a profile
    int profileID; // For easy identification (for testing)
    string fName;
    string lName;
    QDate dob;
    float height;
    float weight;
    SEX sex;
    string phoneNumber;
    string email;
    string password;

    // List of all readings
    QVector<ReadingStorage*> sessions;

private slots:
    void onProfileUpdate();
    void onProfileDelete();
    void onProfileClosed();

signals:
    void profileUpdated(string);
    void profileDeleted();

};

#endif // PROFILE_H
