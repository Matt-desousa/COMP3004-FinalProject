#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QMainWindow>
#include <string>
using namespace std;

#include "defs.h"
#include "user.h"

class User;

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProfileWindow(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QWidget *parent = nullptr);
    ~ProfileWindow();

private:
    Ui::ProfileWindow *ui;
    User* parent;

private slots:
    void onConfirmButtonPressed();

signals:
    void updateProfile(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password);
    void onDeleteUserPressed();
    void userVerified(bool);
};

#endif // PROFILEWINDOW_H
