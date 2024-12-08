#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <list>
#include <QMessageBox>

#include "defs.h"
#include "profile.h"
#include "battery.h"
#include "readingstorage.h"
#include "historyviewer.h"
#include "recommendation.h"

#include "mainwindow.h"
#include "loginwindow.h"
#include "createprofilewindow.h"

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = nullptr);

    /*
        Creates a Profile using the given information.
        Returns true if a profile is successfully created.
    */
    bool createProfile(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password);

    /*
        Verifies a Profile using the password given.
        Returns true if the password and index match.
    */
    bool verifyProfile(string password, int index);

    // Logs a profile out.
    void logoutProfile();

    // Adds random data to each profile once logged in.
    void addData();

    // Displays the notes of the currentProfile.
    void display_note();

    // Displays the chart of the currentProfile.
    void update_chart();

    // Resets the mainWindow. For when a profile is logged out.
    void reset();

private:
    // Windows and UIs
    MainWindow* mainWindow;
    Ui::MainWindow* mwUI;
    LoginWindow* loginWindow;
    Ui::LoginWindow* lwUI;
    CreateProfileWindow* createWindow;
    Ui::CreateProfileWindow* cwUI;

    Profile* currentProfile = NULL; // Currently logged in profile.
    Battery* battery; // Battery for the device.
    HistoryViewer* history_viewer; // Help class for the chart tab in mainWindow.
    list<Profile*> profiles; // List of all profiles stored in the device.
    QMap<QString,QPair<int,int>> ranges; //ranges for each spot(min,max)
    QList<QCheckBox*> scanCheckboxes; //List of all checkboxes
    QList<QPushButton*> tagButtonGroup; //data collections:tags
    QMap<QString,int> spotValues; // store scanned data
    int nextID = 0; // Given to each user on login (for testing)
    bool lastState = false; // The last state of the Skin Contact check box. True or False for checked and not checked.

    // Private Functions
    void shutdown(); // Shutdowns the device when battery reaches 0.
    void handleCheckboxToggled(bool checked);// skin contact
    void trackScanning(); //Keep track of the scanning
    void checkAllScansCompleted(); //Check if scanning is completed
    int calculateAverage();
    void PrintDia(); // Displays the diagnostic and recommendations in the recommendations tab in mainWindow.

    //testing stuff //DELETE LATER
    ReadingStorage* test_storage;
    Profile* test_user;
    QVector<ReadingStorage*> test_readings;

private slots:
    void onProfileCreated();
    void onProfileLogin();
    void onProfileLogout();
    void onProfileUpdate(string name);
    void onProfileDeleted();
    void onProfileShow();
    void onAutoScanPressed();
    void onFahrenheitSelected();//temp conversion
    void onCelsiusSelected();//temp convresion
    void onAddTagButtonClicked(); //add tag
    void onSaveNotesPressed(); //Data collection

    void processRyodorakuData(); // Process the data.
};

#endif // DEVICE_H
