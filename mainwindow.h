#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "battery.h"
#include "readingstorage.h"
#include "device.h"
#include <QMap>
#include <QPair>
#include <QDebug>
#include <QList>
#include <QPushButton>
#include <QInputDialog>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Device* device;
    Battery* battery;
    QMap<QString,QPair<int,int>> ranges; //ranges for each spot(min,max)
    bool lastState;
    void handleCheckboxToggled(bool checked);// skin contact
    void trackScanning(); //Keep track of the scanning
    void checkAllScansCompleted(); //Check if scanning is completed
    void saveNotes(); //Data collection
    int calculateAverage();
    void processRyodorakuData();
    QList<QCheckBox*> scanCheckboxes; //List of all checkboxes
    QList<QPushButton*> tagButtonGroup; //data collections:tags
    QMap<QString,int> spotValues; // store scanned data
    ReadingStorage* test_storage; //DELETE LATER

private slots:
    void onFahrenheitSelected();//temp conversion
    void onCelsiusSelected();//temp convresion
    void onAddTagButtonClicked(); //add tag
    void onCreateProfile(); // Creating a profile
    void updateProfiles(int add, string name); // Update profile to login

};
#endif // MAINWINDOW_H
