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
    bool createProfile(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password);
    bool verifyProfile(string password, int index);
    void logoutProfile();
    void addData();
    void display_note();
    void update_chart();
    void reset();

private:
    // Windows and UIs
    MainWindow* mainWindow;
    Ui::MainWindow* mwUI;
    LoginWindow* loginWindow;
    Ui::LoginWindow* lwUI;
    CreateProfileWindow* createWindow;
    Ui::CreateProfileWindow* cwUI;

    // Others
    Profile* currentProfile;
    Battery* battery;
    HistoryViewer* history_viewer;
    list<Profile*> profiles;
    QMap<QString,QPair<int,int>> ranges; //ranges for each spot(min,max)
    QList<QCheckBox*> scanCheckboxes; //List of all checkboxes
    QList<QPushButton*> tagButtonGroup; //data collections:tags
    QMap<QString,int> spotValues; // store scanned data
    int nextID = 0;
    bool lastState = false;

    // Private Functions
    void shutdown();
    void handleCheckboxToggled(bool checked);// skin contact
    void trackScanning(); //Keep track of the scanning
    void checkAllScansCompleted(); //Check if scanning is completed
    int calculateAverage();
    void PrintDia();

    //testing stuff //DELETE LATER
    ReadingStorage* test_storage;
    Profile* test_user;
    QVector<ReadingStorage*> test_readings;

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
    void onProfileDeleted();
    void onProfileShow();
    void onAutoScanPressed();
    void onFahrenheitSelected();//temp conversion
    void onCelsiusSelected();//temp convresion
    void onAddTagButtonClicked(); //add tag
    void onSaveNotesPressed(); //Data collection

    void processRyodorakuData();
};

#endif // DEVICE_H
