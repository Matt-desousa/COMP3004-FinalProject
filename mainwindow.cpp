#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    //DELETE LATER


    // Setup UI
    ui->setupUi(this);

    // Init the Device
    device = new Device();

    // Setup Battery
    battery = new Battery(ui->isCharging, ui->ChargeIndicator);
    battery->turn_on_or_off(true); // start using battery power

    tagButtonGroup = {ui->activeTag, ui->calmTag, ui->basisMorningTag};
    ranges = {
        {"H1 Left", {5, 190}}, {"H1 Right", {5, 190}}, {"H2 Left", {5, 170}}, {"H2 Right", {5, 170}}, {"H3 Left", {5, 140}}, {"H3 Right", {5, 140}}, {"H4 Left", {5, 170}}, {"H4 Right", {5, 170}}, {"H5 Left", {5, 200}}, {"H5 Right", {5, 200}}, {"H6 Left", {5, 200}}, {"H6 Right", {5, 200}}, {"F1 Left", {5, 160}}, {"F1 Right", {5, 160}}, {"F2 Left", {5, 130}}, {"F2 Right", {5, 130}}, {"F3 Left", {5, 150}}, {"F3 Right", {5, 150}}, {"F4 Left", {5, 150}}, {"F4 Right", {5, 150}}, {"F5 Left", {5, 130}}, {"F5 Right", {5, 130}}, {"F6 Left", {5, 150}}, {"F6 Right", {5, 150}}};
    // Initialize with default values
    for(const QString &spot: ranges.keys()){
        spotValues[spot] = ranges[spot].first;
    }
    test_storage = new ReadingStorage(&ranges);
    // Add all checkboxes to the list
    scanCheckboxes = {
            ui->checkboxH1Left, ui->checkboxH1Right, ui->checkboxH2Left, ui->checkboxH2Right,
            ui->checkboxH3Left, ui->checkboxH3Right, ui->checkboxH4Left, ui->checkboxH4Right,
            ui->checkboxH5Left, ui->checkboxH5Right, ui->checkboxH6Left, ui->checkboxH6Right,
            ui->checkboxF1Left, ui->checkboxF1Right, ui->checkboxF2Left, ui->checkboxF2Right,
            ui->checkboxF3Left, ui->checkboxF3Right, ui->checkboxF4Left, ui->checkboxF4Right,
            ui->checkboxF5Left, ui->checkboxF5Right, ui->checkboxF6Left, ui->checkboxF6Right
        };
    for (QCheckBox *checkbox : scanCheckboxes) {
            checkbox->setEnabled(false);
        }

    // Connect dropdown to update the slider range and value
    connect(ui->dropdown, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
            {
            QString selectedOption = ui->dropdown->currentText();
            QPair<int,int> range = ranges[selectedOption];

            //update range
            ui->horizontalSlider->setRange(range.first,range.second);

            //reset slider to the minimum value of the range
            ui->horizontalSlider->setValue(range.first);
            ui->horizontalSlider->setEnabled(false);
            qDebug()<<"Prepare for scanning the next point. Please lift the device off the skin and move to the next scanning point.";
    });

    // Update slider value and store data
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, [=](int value){
        ui->sliderValue->setText(QString::number(value));
        QString selectedSpot = ui->dropdown->currentText();
        spotValues[selectedSpot] = value;
        test_storage->log_data_point(selectedSpot, value);
    });

    // Disable slider and dropdown
    ui->horizontalSlider->setEnabled(false);
    ui->dropdown->setEnabled(false);

    // Connect the checkbox to the handle function
    connect(ui->skinContactChecked, &QCheckBox::toggled, this, &MainWindow::handleCheckboxToggled);

    // Add tag buttons
    connect(ui->addTag, &QPushButton::clicked, this, &MainWindow::onAddTagButtonClicked);
    // Save Notes
    connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::saveNotes);

    // Temperature conversion
    connect(ui->fahrenheitRadioButton, &QRadioButton::pressed, this, &MainWindow::onFahrenheitSelected);
    connect(ui->celsiusRadioButton, &QRadioButton::pressed, this, &MainWindow::onCelsiusSelected);

    // Create Profile
    connect(ui->btnCreateProfile, SIGNAL(pressed()), this, SLOT(onCreateProfile()));
    connect(device, SIGNAL(userCreated(int, string)), this, SLOT(updateProfiles(int, string)));


    //debug
    //DELETE LATER
    ui->result->setEnabled(false);
    connect(ui->result,&QPushButton::clicked,this,&MainWindow::processRyodorakuData);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete battery;
}

//helper function: Trackaing the process of scanning
void MainWindow::trackScanning(){
    QString selectedSpot = ui->dropdown->currentText();
    // Find and check the corresponding checkbox by matching text
        for (QCheckBox *checkbox : scanCheckboxes) {
            if (checkbox->text() == selectedSpot) {
                checkbox->setEnabled(true);
                checkbox->setChecked(true);
                checkbox->setEnabled(false);
                break;
            }
        }
        //check if all checkboxes are checked
        checkAllScansCompleted();
}

void MainWindow::checkAllScansCompleted(){
    bool allChecked = true;
    for(QCheckBox *checkbox : scanCheckboxes){
        if(!checkbox->isChecked()){
            allChecked = false;
            break;
        }
    }
    if(allChecked){
        ui->result->setEnabled(true);
    }
}
// Skin contact
void MainWindow::handleCheckboxToggled(bool checked)
{
    // last State is false (lift the device off the skin) and putting it back on the next point
    if (checked == true && lastState == false)
    {
        qDebug() << "Successive measurement.";
        ui->horizontalSlider->setEnabled(true);
        ui->dropdown->setEnabled(true);
        trackScanning();
    }
    else if (checked == false && lastState == true )
    {
        qDebug() << "The device is off the skin";
        ui->horizontalSlider->setEnabled(false);
    }
    else if(checked == true && lastState == true){
        qDebug() << "The device is off the skin";
        ui->horizontalSlider->setEnabled(false);
    }
    lastState = checked;
}

// Save notes(data collection)
void MainWindow::saveNotes()
{
    //Note* new_note = current_user->get_latest_note(); //ACTIVATE LATER
    Note* new_note = new Note(); //DELETE LATER

    // Retreive all the informations
    new_note->bodyTemp = ui->bodyTemp->value();
    new_note->tempUnit = ui->celsiusRadioButton->isChecked() ? C : F;
    new_note->bloodPressureLeftSystolic = ui->bloodPressureLeftSystolic->text().toInt();
    new_note->bloodPressureLeftDiastolic = ui->bloodPressureLeftDiastolic->text().toInt();
    new_note->bloodPressureRightSystolic = ui->bloodPressureRightSystolic->text().toInt();
    new_note->bloodPressureRightDiastolic = ui->bloodPressureRightDiastolic->text().toInt();
    new_note->heartRate = ui->heartRate->text().toInt();
    new_note->sleepHrs = ui->sleepHrs->text().toInt();
    new_note->sleepMins = ui->sleepMins->text().toInt();
    new_note->weight = ui->weight->text().toInt();
    new_note->weightUnit = ui->lbsRadioButton->isChecked() ? LBS : KG;
    new_note->notes = ui->notes->toPlainText();

    if (ui->emoStateVerySad->isChecked())
        new_note->emotionalState = "Very Sad";
    else if (ui->emoStateSad->isChecked())
        new_note->emotionalState = "Sad";
    else if (ui->emoStateNeutral->isChecked())
        new_note->emotionalState = "Neutral";
    else if (ui->emoStateHappy->isChecked())
        new_note->emotionalState = "Happy";
    else if (ui->emoStateVeryHappy->isChecked())
        new_note->emotionalState = "Very Happy";

    if (ui->overallFeelingVerySad->isChecked())
        new_note->overallFeeling = "Very Sad";
    else if (ui->overallFeelingSad->isChecked())
        new_note->overallFeeling = "Sad";
    else if (ui->OverallFeelingNeutral->isChecked())
        new_note->overallFeeling = "Neutral";
    else if (ui->overallFeelingHappy->isChecked())
        new_note->overallFeeling = "Happy";
    else if (ui->overallFeelingVeryHappy->isChecked())
        new_note->overallFeeling = "Very Happy";

    QStringList tags;

    for (QPushButton *button : tagButtonGroup)
    {
        if (button->isChecked())
        {
            tags.append(button->text());
        }
    }

    // Add more tag buttons as needed
   new_note->tagsString = tags.join(", ");

    // DELETE LATER
    // Combine Data into a Structured Format
    QString data = QString(
                       "Body Temperature: %1 %2\n"
                       "Blood Pressure (Left Hand): %3/%4\n"
                       "Blood Pressure (Right Hand): %5/%6\n"
                       "Heart Rate: %7 bpm\n"
                       "Sleeping Time: %8 h %9 m\n"
                       "Current Weight: %10 %11\n"
                       "Emotional State: %12\n"
                       "Overall Feeling: %13\n"
                       "Tags: %14\n"
                       "Notes: %15\n")
                       .arg(new_note->bodyTemp)
                       .arg(new_note->tempUnit)
                       .arg(new_note->bloodPressureLeftSystolic)
                       .arg(new_note->bloodPressureLeftDiastolic)
                       .arg(new_note->bloodPressureRightSystolic)
                       .arg(new_note->bloodPressureRightDiastolic)
                       .arg(new_note->heartRate)
                       .arg(new_note->sleepHrs)
                       .arg(new_note->sleepMins)
                       .arg(new_note->weight)
                       .arg(new_note->weightUnit)
                       .arg(new_note->emotionalState)
                       .arg(new_note->overallFeeling)
                       .arg(new_note->tagsString)
                       .arg(new_note->notes);

    qDebug() << "DEBUG DATA: " << data;
}

// temperture conversion
void MainWindow::onFahrenheitSelected()
{
    ui->bodyTemp->setRange(32.0, 212.0);
    ui->bodyTemp->setValue(32.0);
}

void MainWindow::onCelsiusSelected()
{
    ui->bodyTemp->setRange(0.0, 100.0);
    ui->bodyTemp->setValue(0.0);
}

// add tag
void MainWindow::onAddTagButtonClicked()
{
    bool ok;
    QString tagName = QInputDialog::getText(this, "Add Tag", "Enter Tag Name:", QLineEdit::Normal, "", &ok);

    // If the user clicks "OK" and enters a name, create the tag button
    if (ok && !tagName.isEmpty())
    {
        QPushButton *newTagButton = new QPushButton("+" + tagName, this);
        newTagButton->setCheckable(true);

        // Add the button to the layout
        ui->formLayout->addWidget(newTagButton);

        // add the button to the tagButtonGroup
        tagButtonGroup.append(newTagButton);
    }
    ui->addTag->setChecked(false);
}

void MainWindow::onCreateProfile()
{
    string fName = ui->txtFName->text().toStdString();
    string lName = ui->txtLName->text().toStdString();
    SEX sex = (SEX) ui->cmbSex->currentIndex();
    float weight = ui->dsbWeight->value();
    float height = ui->dsbHeight->value();
    QDate date = ui->datDOB->date();
    string phoneNum = ui->txtPhoneNum->text().toStdString();
    string email = ui->txtEmail->text().toStdString();
    string password = ui->txtPass->text().toStdString();
    string conPassword = ui->txtConPass->text().toStdString();

    bool validUser = true;

    if (password.compare(conPassword)) return;

    device->createUser(fName, lName, sex, weight, height, date, phoneNum, email, password);
}

void MainWindow::updateProfiles(int add, string name)
{
    if (add){
        ui->cmbProfile->addItem(QString::fromStdString(name));
    }
}

int MainWindow::calculateAverage(){
    int total = 0;
    for(const int &value : spotValues){
        total += value;

    }
    return total/spotValues.size();
}

void MainWindow::processRyodorakuData(){
    int average = calculateAverage();
    int below = average * 0.8;
    int above = average * 1.2;

    // Result container for classifications
    QMap<QString, QString> results;

    for (const QString &key : spotValues.keys()) {
            int value = spotValues[key];
            if (value < below) {
                results[key] = "BelowNorm";
            } else if (value > above) {
                results[key] = "AboveNorm";
            } else {
                results[key] = "Normal";
            }
        }

        // DELETE LATER
        qDebug() << "Ryodoraku Data Results:";
        for (const QString &key : results.keys()) {
            qDebug() << key << ":" << results[key];
        }
}

