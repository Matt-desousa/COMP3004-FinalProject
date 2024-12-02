#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

#include "createprofilewindow.h"

#include "device.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(Device* device, QWidget *parent = nullptr);
    ~LoginWindow();

private:
    Ui::LoginWindow *ui;
    CreateProfileWindow* createProfileWindow;
    Device* device;

private slots:
    void onLoginButtonPressed(); // Login in a user
    void onProfileCreated(string name);
    void onProfileUpdate(string name);
    void onProfileDelete();
};

#endif // LOGINWINDOW_H
