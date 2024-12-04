#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "readingstorage.h"
#include "historyviewer.h"
#include "profile.h"
#include "note.h"
#include <QMap>
#include <QPair>
#include <QDebug>
#include <QList>
#include <QPushButton>
#include <QInputDialog>
#include <QCheckBox>

#include "recommendation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    inline void getUI(Ui::MainWindow** ui){*ui=this->ui;}

    void addData(Profile* currentProfile);

    void processRyodorakuData(Profile* currentProfile);
    void saveNotes(Profile* currentProfile); //Data collection
    void display_note(Profile* currentProfile);

private:
    Ui::MainWindow *ui;
    HistoryViewer* history_viewer;
    QMap<QString,QPair<int,int>> ranges; //ranges for each spot(min,max)
    bool lastState;
    void handleCheckboxToggled(bool checked);// skin contact
    void trackScanning(); //Keep track of the scanning
    void checkAllScansCompleted(); //Check if scanning is completed
    int calculateAverage();
    void PrintDia();




    QList<QCheckBox*> scanCheckboxes; //List of all checkboxes
    QList<QPushButton*> tagButtonGroup; //data collections:tags
    QMap<QString,int> spotValues; // store scanned data
    Recommendation recommend;

    //testing stuff //DELETE LATER
    ReadingStorage* test_storage;
    Profile* test_user;
    QVector<ReadingStorage*> test_readings;

private slots:
    void onFahrenheitSelected();//temp conversion
    void onCelsiusSelected();//temp convresion
    void onAddTagButtonClicked(); //add tag
};
#endif // MAINWINDOW_H
