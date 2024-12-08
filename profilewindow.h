#ifndef PROFILEWINDOW_H
#define PROFILEWINDOW_H

#include <QMainWindow>
#include <string>
using namespace std;

#include "defs.h"
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class ProfileWindow; }
QT_END_NAMESPACE

class ProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProfileWindow(QWidget *parent = nullptr);
    ~ProfileWindow();
    inline void getUI(Ui::ProfileWindow** ui){*ui=this->ui;}

private:
    Ui::ProfileWindow *ui;

protected:
    void closeEvent(QCloseEvent * event){ emit closed();}

signals:
    void closed();
};

#endif // PROFILEWINDOW_H
