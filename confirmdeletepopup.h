#ifndef CONFIRMDELETEPOPUP_H
#define CONFIRMDELETEPOPUP_H

#include <QDialog>
#include <string>
using namespace std;

namespace Ui {
class ConfirmDeletePopup;
}

class ConfirmDeletePopup : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDeletePopup(QWidget *parent = nullptr);
    ~ConfirmDeletePopup();
    inline void getUI(Ui::ConfirmDeletePopup** ui){*ui=this->ui;}

private:
    Ui::ConfirmDeletePopup *ui;
};

#endif // CONFIRMDELETEPOPUP_H
