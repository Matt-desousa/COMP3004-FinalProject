#include "confirmdeletepopup.h"
#include "ui_confirmdeletepopup.h"

#include <QDebug>

ConfirmDeletePopup::ConfirmDeletePopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmDeletePopup)
{
    ui->setupUi(this);

}

ConfirmDeletePopup::~ConfirmDeletePopup()
{
    delete ui;
}
