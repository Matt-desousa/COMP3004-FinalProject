#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QDebug>

LoginWindow::LoginWindow(Device* device, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    this->device = device;

    createProfileWindow = new CreateProfileWindow(device);

    // Switch between login and create profile windows
    connect(this, SIGNAL(createProfile()), createProfileWindow, SLOT(show()));
    connect(createProfileWindow, SIGNAL(closeCreateProfile()), this, SLOT(show()));

    // Login Profile
    connect(ui->btnLogin, SIGNAL(pressed()), this, SLOT(onLoginButtonPressed()));
    connect(device, SIGNAL(userLogin(string)), this, SLOT(onUserLogin(string)));

    // Getting all the name of previously created profiles
    list<string> names;
    device->getUserNames(&names);
    for (string name : names){
        ui->cmbProfile->addItem(QString::fromStdString(name));
    }

    // Create Profile
    connect(ui->btnCreateProfile, SIGNAL(pressed()), createProfileWindow, SLOT(show()));
    connect(ui->btnCreateProfile, SIGNAL(pressed()), this, SLOT(hide()));

    // Profile Created
    connect(device, SIGNAL(userCreated(string)), this, SLOT(onProfileCreated(string)));

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
    device->verifyUser(password, index);
}

void LoginWindow::onUserLogin()
{
    this->hide();
}

void LoginWindow::onProfileCreated(string name)
{
    ui->cmbProfile->addItem(QString::fromStdString(name));
}
