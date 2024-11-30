#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "battery.h"
#include "readingstorage.h"
#include "historyviewer.h"
#include "device.h"
#include "note.h"
#include <QMap>
#include <QPair>
#include <QDebug>
#include <QList>
#include <QPushButton>
#include <QInputDialog>

#include "loginwindow.h"

#include "recommendation.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Device* device, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LoginWindow* loginWindow;
    Device* device;
    Battery* battery;
    HistoryViewer* history_viewer;
    QMap<QString,QPair<int,int>> ranges; //ranges for each spot(min,max)
    bool lastState;
    void handleCheckboxToggled(bool checked);// skin contact
    void trackScanning(); //Keep track of the scanning
    void checkAllScansCompleted(); //Check if scanning is completed
    void saveNotes(); //Data collection
    int calculateAverage();
    void processRyodorakuData();
    void PrintDia();
    QList<QCheckBox*> scanCheckboxes; //List of all checkboxes
    QList<QPushButton*> tagButtonGroup; //data collections:tags
    QMap<QString,int> spotValues; // store scanned data
    Recommendation recommend;

    //testing stuff //DELETE LATER
    ReadingStorage* test_storage;
    User* test_user;
    QVector<ReadingStorage*> test_readings;

private slots:
    void onFahrenheitSelected();//temp conversion
    void onCelsiusSelected();//temp convresion
    void onAddTagButtonClicked(); //add tag
    void onUserLogin(string name);
    void onUserLogout();
};
#endif // MAINWINDOW_H
