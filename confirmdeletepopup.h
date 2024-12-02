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
    void onUserVerified(bool);

private:
    Ui::ConfirmDeletePopup *ui;

private slots:
    void onOKButtonPressed();

signals:
    void verifyUser(string);
    void userDeleted();
};

#endif // CONFIRMDELETEPOPUP_H
