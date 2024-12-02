#include "confirmdeletepopup.h"
#include "ui_confirmdeletepopup.h"

#include <QDebug>

ConfirmDeletePopup::ConfirmDeletePopup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmDeletePopup)
{
    ui->setupUi(this);

    connect(ui->btnCancel, SIGNAL(pressed()), this, SLOT(hide()));
    connect(ui->btnDelete, SIGNAL(pressed()), this, SLOT(onOKButtonPressed()));
}

ConfirmDeletePopup::~ConfirmDeletePopup()
{
    delete ui;
}

void ConfirmDeletePopup::onUserVerified(bool isVerified)
{
    if (isVerified) hide();
    else {
        ui->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        ui->txtPass->setText("");
    }
}

void ConfirmDeletePopup::onOKButtonPressed()
{
    string conPass = ui->txtPass->text().toStdString();
    emit verifyUser(conPass);
}
