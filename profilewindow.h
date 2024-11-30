#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QMainWindow>

#include "device.h"

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProfileWindow(Device* device, QWidget *parent = nullptr);
    ~ProfileWindow();

private:
    Ui::ProfileWindow *ui;
    Device* device;

private slots:
    void onUserLogin(string name);
};

#endif // PROFILEWINDOW_H
