#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QMainWindow>
#include <string>
using namespace std;

#include "defs.h"

class Profile;

namespace Ui {
class ProfileWindow;
}

class ProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProfileWindow(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password, QWidget *parent = nullptr);
    ~ProfileWindow();
    inline void getUI(Ui::ProfileWindow** ui){*ui=this->ui;}

private:
    Ui::ProfileWindow *ui;
};

#endif // PROFILEWINDOW_H
