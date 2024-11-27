#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "battery.h"
#include "readingstorage.h"
#include <QMap>
#include <QPair>
#include <QDebug>

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
    Battery* battery;
    QMap<QString,QPair<int,int>> ranges; //ranges for each spot(min,max)
    bool lastState;
    void handleCheckboxToggled(bool checked);// skin contact

    ReadingStorage* test_storage; //DELETE LATER
};
#endif // MAINWINDOW_H
