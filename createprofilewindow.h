#ifndef CREATEPROFILEWINDOW_H
#define CREATEPROFILEWINDOW_H

#include <QMainWindow>

#include "device.h"

namespace Ui {
class CreateProfileWindow;
}

class CreateProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CreateProfileWindow(Device* device, QWidget *parent = nullptr);
    ~CreateProfileWindow();

private:
    Ui::CreateProfileWindow *ui;
    Device* device;

private slots:
    void onCreateProfileButtonPressed(); // Creating a profile
};

#endif // CREATEPROFILEWINDOW_H
