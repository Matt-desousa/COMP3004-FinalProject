#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //Setup UI
    ui->setupUi(this);

    //Setup Battery
    battery = new Battery(ui->isCharging,  ui->ChargeIndicator);
    battery->turn_on_or_off(true); //start using battery power

    ranges = {
            {"H1 Left",{5,190}},{"H1 Right",{5,190}},{"H2 Left",{5,170}},{"H2 Right",{5,170}},{"H3 Left",{5,140}},{"H3 Right",{5,140}},
            {"H4 Left",{5,170}},{"H4 Right",{5,170}},{"H5 Left",{5,200}},{"H5 Right",{5,200}},{"H6 Left",{5,200}},{"H6 Right",{5,200}},
            {"F1 Left",{5,160}},{"F1 Right",{5,160}},{"F2 Left",{5,130}},{"F2 Right",{5,130}},{"F3 Left",{5,150}},{"F3 Right",{5,150}},
            {"F4 Left",{5,150}},{"F4 Right",{5,150}},{"F5 Left",{5,130}},{"F5 Right",{5,130}},{"F6 Left",{5,150}},{"F6 Right",{5,150}}
        };

        // Connect dropdown to update the slider range and value
        connect(ui->dropdown,QOverload<int>::of(&QComboBox::currentIndexChanged),this,[=](int index){
            QString selectedOption = ui->dropdown->currentText();
            QPair<int,int> range = ranges[selectedOption];

            //update range
            ui->horizontalSlider->setRange(range.first,range.second);

            //reset slider to the minimum value of the range
            ui->horizontalSlider->setValue(range.first);
        });

        //update slider value
        connect(ui->horizontalSlider, &QSlider::valueChanged, this, [=](int value) {
                ui->sliderValue->setText(QString::number(value));
            });

        //disable slider and dropdown
        ui->horizontalSlider->setEnabled(false);
        ui->dropdown->setEnabled(false);
        //connect the checkbox to the handle function
        connect(ui->skinContactChecked,&QCheckBox::toggled,this,&MainWindow::handleCheckboxToggled);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete battery;
}

//Skin contact
void MainWindow::handleCheckboxToggled(bool checked){
    qDebug()<<"CHECKING: " << checked << " " << lastState;
    // last State is false (lift the device off the skin) and putting it back on the next point
    if(checked && !lastState){
        qDebug() << "Successive measurement.";
        ui->horizontalSlider->setEnabled(true);
        ui->dropdown->setEnabled(true);
    }
    else if(!checked && lastState){
        qDebug() << "Lift the device off the skin";
        ui->horizontalSlider->setEnabled(false);
    }
    lastState = checked;
}
