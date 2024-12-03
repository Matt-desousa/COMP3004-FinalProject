#ifndef CREATEPROFILEWINDOW_H
#define CREATEPROFILEWINDOW_H

#include <QMainWindow>

namespace Ui {
class CreateProfileWindow;
}

class CreateProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CreateProfileWindow(QWidget *parent = nullptr);
    ~CreateProfileWindow();
    inline void getUI(Ui::CreateProfileWindow** ui){*ui=this->ui;}

private:
    Ui::CreateProfileWindow *ui;
};

#endif // CREATEPROFILEWINDOW_H
