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
    void onProfileCreated(string name); // When a new profile is created. Used to update the list of profiles on the login paage.
    void onCancelButtonPressed();

signals:
    void closeCreateProfile();
};

#endif // CREATEPROFILEWINDOW_H
