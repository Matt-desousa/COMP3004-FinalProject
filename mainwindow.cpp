#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    // Setup UI
    ui->setupUi(this);

    // Setup Battery
    battery = new Battery(ui->isCharging, ui->ChargeIndicator);
    battery->turn_on_or_off(true); // start using battery power

    tagButtonGroup = {ui->activeTag, ui->calmTag, ui->basisMorningTag};

    ranges = {
        {"H1 Left", {5, 190}}, {"H1 Right", {5, 190}}, {"H2 Left", {5, 170}}, {"H2 Right", {5, 170}}, {"H3 Left", {5, 140}}, {"H3 Right", {5, 140}}, {"H4 Left", {5, 170}}, {"H4 Right", {5, 170}}, {"H5 Left", {5, 200}}, {"H5 Right", {5, 200}}, {"H6 Left", {5, 200}}, {"H6 Right", {5, 200}}, {"F1 Left", {5, 160}}, {"F1 Right", {5, 160}}, {"F2 Left", {5, 130}}, {"F2 Right", {5, 130}}, {"F3 Left", {5, 150}}, {"F3 Right", {5, 150}}, {"F4 Left", {5, 150}}, {"F4 Right", {5, 150}}, {"F5 Left", {5, 130}}, {"F5 Right", {5, 130}}, {"F6 Left", {5, 150}}, {"F6 Right", {5, 150}}};

    // Connect dropdown to update the slider range and value
    connect(ui->dropdown, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
            {
            QString selectedOption = ui->dropdown->currentText();
            QPair<int,int> range = ranges[selectedOption];

            //update range
            ui->horizontalSlider->setRange(range.first,range.second);

            //reset slider to the minimum value of the range
            ui->horizontalSlider->setValue(range.first); });

    // update slider value
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, [=](int value)
            { ui->sliderValue->setText(QString::number(value)); });

    // disable slider and dropdown
    ui->horizontalSlider->setEnabled(false);
    ui->dropdown->setEnabled(false);

    // connect the checkbox to the handle function
    connect(ui->skinContactChecked, &QCheckBox::toggled, this, &MainWindow::handleCheckboxToggled);

    // add tag buttons
    connect(ui->addTag, &QPushButton::clicked, this, &MainWindow::onAddTagButtonClicked);
    // save Notes
    connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::saveNotes);

    // temperture conversion
    connect(ui->fahrenheitRadioButton, &QRadioButton::pressed, this, &MainWindow::onFahrenheitSelected);
    connect(ui->celsiusRadioButton, &QRadioButton::pressed, this, &MainWindow::onCelsiusSelected);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete battery;
}

// Skin contact
void MainWindow::handleCheckboxToggled(bool checked)
{
    // last State is false (lift the device off the skin) and putting it back on the next point
    if (checked && !lastState)
    {
        qDebug() << "Successive measurement.";
        ui->horizontalSlider->setEnabled(true);
        ui->dropdown->setEnabled(true);
    }
    else if (!checked && lastState)
    {
        qDebug() << "Lift the device off the skin";
        ui->horizontalSlider->setEnabled(false);
    }
    lastState = checked;
}

// Save notes(data collection)
void MainWindow::saveNotes()
{
    // Retreive all the informations
    double bodyTemp = ui->bodyTemp->value();
    QString tempUnit = ui->celsiusRadioButton->isChecked() ? "°C" : "°F";
    int bloodPressureLeftSystolic = ui->bloodPressureLeftSystolic->text().toInt();
    int bloodPressureLeftDiastolic = ui->bloodPressureLeftDiastolic->text().toInt();
    int bloodPressureRightSystolic = ui->bloodPressureRightSystolic->text().toInt();
    int bloodPressureRightDiastolic = ui->bloodPressureRightDiastolic->text().toInt();
    int heartRate = ui->heartRate->text().toInt();
    int sleepHrs = ui->sleepHrs->text().toInt();
    int sleepMins = ui->sleepMins->text().toInt();
    double weight = ui->weight->text().toInt();
    QString weightUnit = ui->lbsRadioButton->isChecked() ? "lb" : "kg";
    QString notes = ui->notes->toPlainText();

    QString emotionalState;
    if (ui->emoStateVerySad->isChecked())
        emotionalState = "Very Sad";
    else if (ui->emoStateSad->isChecked())
        emotionalState = "Sad";
    else if (ui->emoStateNeutral->isChecked())
        emotionalState = "Neutral";
    else if (ui->emoStateHappy->isChecked())
        emotionalState = "Happy";
    else if (ui->emoStateVeryHappy->isChecked())
        emotionalState = "Very Happy";

    QString overallFeeling;
    if (ui->overallFeelingVerySad->isChecked())
        overallFeeling = "Very Sad";
    else if (ui->overallFeelingSad->isChecked())
        overallFeeling = "Sad";
    else if (ui->OverallFeelingNeutral->isChecked())
        overallFeeling = "Neutral";
    else if (ui->overallFeelingHappy->isChecked())
        overallFeeling = "Happy";
    else if (ui->overallFeelingVeryHappy->isChecked())
        overallFeeling = "Very Happy";

    QStringList tags;

    for (QPushButton *button : tagButtonGroup)
    {
        if (button->isChecked())
        {
            tags.append(button->text());
        }
    }

    // Add more tag buttons as needed
    QString tagsString = tags.join(", ");

    // delete later
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
                       .arg(bodyTemp)
                       .arg(tempUnit)
                       .arg(bloodPressureLeftSystolic)
                       .arg(bloodPressureLeftDiastolic)
                       .arg(bloodPressureRightSystolic)
                       .arg(bloodPressureRightDiastolic)
                       .arg(heartRate)
                       .arg(sleepHrs)
                       .arg(sleepMins)
                       .arg(weight)
                       .arg(weightUnit)
                       .arg(emotionalState)
                       .arg(overallFeeling)
                       .arg(tagsString)
                       .arg(notes);

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
