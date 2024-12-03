#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QDebug>

LoginWindow::LoginWindow(Device* device, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    this->device = device;

    createProfileWindow = new CreateProfileWindow(device, this);

    // Login Profile
    connect(ui->btnLogin, SIGNAL(pressed()), this, SLOT(onLoginButtonPressed()));

    // Getting all the name of previously created profiles
    list<string> names;
    device->getProfileNames(&names);
    for (string name : names){
        ui->cmbProfile->addItem(QString::fromStdString(name));
    }

    // Create Profile
    connect(ui->btnCreateProfile, SIGNAL(pressed()), createProfileWindow, SLOT(show()));
    connect(ui->btnCreateProfile, SIGNAL(pressed()), this, SLOT(hide()));

    // Profile Created
    connect(device, SIGNAL(profileCreated(string)), this, SLOT(show()));
    connect(device, SIGNAL(profileCreated(string)), this, SLOT(onProfileCreated(string)));

    // Profile Updated
    connect(device, SIGNAL(profileUpdated(string)), this, SLOT(onProfileUpdate(string)));

    // Profile Deleted
    connect(device, SIGNAL(profileDeleted()), this, SLOT(onProfileDelete()));

    this->show();
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::onLoginButtonPressed()
{
    string password = ui->txtLoginPass->text().toStdString();
    int index = ui->cmbProfile->currentIndex();
    bool loggedIn = device->verifyProfile(password, index);

    if (loggedIn){
        this->hide();
        ui->txtLoginPass->setStyleSheet("");
    }
    else{
        ui->txtLoginPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
    }

    ui->txtLoginPass->setText("");
}

void LoginWindow::onProfileCreated(string name)
{
    ui->cmbProfile->addItem(QString::fromStdString(name));
}

void LoginWindow::onProfileUpdate(string name)
{
    qDebug() << "User Updated.";
    int index = ui->cmbProfile->currentIndex();
    ui->cmbProfile->setItemText(index, QString::fromStdString(name));
}

void LoginWindow::onProfileDelete()
{
    qDebug() << "User Deleted.";
    int index = ui->cmbProfile->currentIndex();
    ui->cmbProfile->removeItem(index);
}
