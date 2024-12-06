#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <string>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Setup UI
    ui->setupUi(this);

    history_viewer = new HistoryViewer(ui->HistoryChart, ui->note_controls);

    tagButtonGroup = {ui->activeTag, ui->calmTag, ui->basisMorningTag};
    ranges = {
        {"H1 Left", {5, 190}}, {"H1 Right", {5, 190}}, {"H2 Left", {5, 170}}, {"H2 Right", {5, 170}}, {"H3 Left", {5, 140}}, {"H3 Right", {5, 140}}, {"H4 Left", {5, 170}}, {"H4 Right", {5, 170}}, {"H5 Left", {5, 200}}, {"H5 Right", {5, 200}}, {"H6 Left", {5, 200}}, {"H6 Right", {5, 200}}, {"F1 Left", {5, 160}}, {"F1 Right", {5, 160}}, {"F2 Left", {5, 130}}, {"F2 Right", {5, 130}}, {"F3 Left", {5, 150}}, {"F3 Right", {5, 150}}, {"F4 Left", {5, 150}}, {"F4 Right", {5, 150}}, {"F5 Left", {5, 130}}, {"F5 Right", {5, 130}}, {"F6 Left", {5, 150}}, {"F6 Right", {5, 150}}};
    // Initialize with default values
    for(const QString &spot: ranges.keys()){
        spotValues[spot] = ranges[spot].first;
    }


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
    });

    // Disable slider and dropdown
    ui->horizontalSlider->setEnabled(false);
    ui->dropdown->setEnabled(false);

    // Connect the checkbox to the handle function
    connect(ui->skinContactChecked, &QCheckBox::toggled, this, &MainWindow::handleCheckboxToggled);

    // Add tag buttons
    connect(ui->addTag, &QPushButton::clicked, this, &MainWindow::onAddTagButtonClicked);

    // Temperature conversion
    connect(ui->fahrenheitRadioButton, &QRadioButton::pressed, this, &MainWindow::onFahrenheitSelected);
    connect(ui->celsiusRadioButton, &QRadioButton::pressed, this, &MainWindow::onCelsiusSelected);

    //Print
    connect(ui->Dia_button, &QRadioButton::pressed, this, &MainWindow::PrintDia);


    //create history chart dropdown
    ui->ChartSelection->addItem("Average");
    for(const QString &body_part: ranges.keys()){
        ui->ChartSelection->addItem(body_part);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete history_viewer;
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
void MainWindow::saveNotes(Profile* currentProfile)
{
    Note* new_note = history_viewer->get_note(currentProfile);

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
        new_note->emotionalState = VERY_SAD;
    else if (ui->emoStateSad->isChecked())
        new_note->emotionalState = SAD;
    else if (ui->emoStateNeutral->isChecked())
        new_note->emotionalState = NEUTRAL;
    else if (ui->emoStateHappy->isChecked())
        new_note->emotionalState = HAPPY;
    else if (ui->emoStateVeryHappy->isChecked())
        new_note->emotionalState = VERY_HAPPY;

    if (ui->overallFeelingVerySad->isChecked())
        new_note->overallFeeling = VERY_SAD;
    else if (ui->overallFeelingSad->isChecked())
        new_note->overallFeeling = SAD;
    else if (ui->OverallFeelingNeutral->isChecked())
        new_note->overallFeeling = NEUTRAL;
    else if (ui->overallFeelingHappy->isChecked())
        new_note->overallFeeling = HAPPY;
    else if (ui->overallFeelingVeryHappy->isChecked())
        new_note->overallFeeling = VERY_HAPPY;

    QStringList tags;

    for (QPushButton *button : tagButtonGroup)
    {
        if (button->isChecked())
        {
            tags.append(button->text());
        }
    }

    // Add more tag buttons as needed
    new_note->tags = tags;

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
                       .arg(new_note->tags.join(", "))
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

void MainWindow::addData(Profile* currentProfile)
{qDebug("aaaa");
    if(currentProfile->getSessions()->size() == 0){ //if user has no sessions
        //insert some random readings just to show graph history
        for (int i = 0; i < 15; i++){
            ReadingStorage* new_test_reading = new ReadingStorage(&ranges);
            new_test_reading->debug_populate_logs();
            currentProfile->log_session(new_test_reading);

        }
    }
qDebug("aaaa");
    history_viewer->update_chart(*currentProfile->getSessions()); //update graph
    qDebug("aaaa");
}

int MainWindow::calculateAverage(){
    int total = 0;
    for(const int &value : spotValues){
        total += value;

    }
    return total/spotValues.size();
}

void MainWindow::processRyodorakuData(Profile* currentProfile){


    ReadingStorage* new_results = new ReadingStorage(&ranges); //make new empty session
    for (const QString &key : spotValues.keys()) {
        new_results->log_data_point(key,  spotValues[key]); //fill it with the results
        //do this all at once here to avoid partial logging if therre's an incomplete shutdown
    }
    currentProfile->log_session(new_results); //add to the currentuser
    history_viewer->update_chart(*currentProfile->getSessions()); //update graph

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


//Print Diagnosis
void MainWindow::PrintDia()
{
    recommend.AddAbnormalPartinQ("H1 Left","H1 Right", spotValues);
    recommend.AddAbnormalPartinQ("H2 Left","H2 Right", spotValues);
    recommend.AddAbnormalPartinQ("H3 Left","H3 Right", spotValues);
    recommend.AddAbnormalPartinQ("H4 Left","H4 Right", spotValues);
    recommend.AddAbnormalPartinQ("H5 Left","H5 Right", spotValues);
    recommend.AddAbnormalPartinQ("H6 Left","H6 Right", spotValues);
    recommend.AddAbnormalPartinQ("F1 Left","F1 Right", spotValues);
    recommend.AddAbnormalPartinQ("F2 Left","F2 Right", spotValues);
    recommend.AddAbnormalPartinQ("F3 Left","F3 Right", spotValues);
    recommend.AddAbnormalPartinQ("F4 Left","F4 Right", spotValues);
    recommend.AddAbnormalPartinQ("F5 Left","F5 Right", spotValues);
    recommend.AddAbnormalPartinQ("F6 Left","F6 Right", spotValues);
    QString temp_body = "";
    int temp_degree = 0;
    if(recommend.GetWrong_partSize() == 0){
        ui->Dia->append("Every part is healthy");
        ui->Organ->append("None");
        ui->Supp->append("None");
    }
    while(recommend.GetWrong_partSize() != 0)
    {
        temp_body = recommend.GetWrong_part();
        temp_degree = recommend.Getresult();
        if(temp_body == "H1" && temp_degree == 1)
        {
            ui->Dia->append("Stiff and painful sholder muscles, rush of blood to the head"
                            "hot lfashes, piles, asthma");
            ui->Organ->append("Lung");
            ui->Supp->append("Propolis - Improve immune defense");
        }else if(temp_body == "H1" && temp_degree == -1){
            ui->Dia->append("Cold feet or numbness, shortness of breath, coughing spells");
            ui->Organ->append("Lung");
            ui->Supp->append("Propolis - Improve immune defense");
        //H2
        }else if(temp_body == "H2" && temp_degree == 1){
            ui->Dia->append("Stiff and painful shoulder muscles, middle age level wrenched"
                            " shoulders, brachical neuralgia.");
            ui->Organ->append("Pericardium");
            ui->Supp->append("Veino Tune - Improves venous blood flow");

        }else if(temp_body == "H2" && temp_degree == -1){
            ui->Dia->append("Palpitation, heated sensation of the palms");
            ui->Organ->append("Pericardium");
            ui->Supp->append("Veino Tune - Improves venous blood flow");
        //H3
        }else if(temp_body == "H3" && temp_degree == 1){
            ui->Dia->append("Puffed sensation of the stomach, constipation");
            ui->Organ->append("Heart");
            ui->Supp->append("AntimeGrin Hyper - Helps with high blood pressure relief and optimization");
        }else if(temp_body == "H3" && temp_degree == -1){
            ui->Dia->append("Palpitation");
            ui->Organ->append("Heart");
            ui->Supp->append("Migrenol Hypo - Helps raise low blood pressure and optimization");
            ui->Supp->append("Immuno Tune - Spports immune health function");
        //H4
        }else if(temp_body == "H4" && temp_degree == 1){
            ui->Dia->append("Headaches, abnormalities of the lower abdomen, joint pain");
            ui->Organ->append("Small intestine");
            ui->Supp->append("Intesti San - helps maintain intestinal health");
            ui->Supp->append("Immuno Tune - Spports immune health function");
        }else if(temp_body == "H4" && temp_degree == -1){
            ui->Dia->append("Headache, abnormalities of the lower abdomen");
            ui->Organ->append("Small intestine");
            ui->Supp->append("Intesti San - helps maintain intestinal health");
        //H5
        }else if(temp_body == "H5" && temp_degree == 1){
            ui->Dia->append("Ringing ears, difficulty in hearing");
            ui->Organ->append("Lymph vessel");
            ui->Supp->append("Edmea Relieve - Promotes proper response to inflammation");
            ui->Supp->append("Immuno Tune - Spports immune health function");
        }else if(temp_body == "H5" && temp_degree == -1){
            ui->Dia->append("Tiredness or tendency to tire, the healthy glow disappears and body hairs"
                            " increase in density");
            ui->Organ->append("Lymph vessel");
            ui->Supp->append("Edmea Relieve - Promotes proper response to inflammation");
            ui->Supp->append("Immuno Tune - Spports immune health function");
        //H6
        }else if(temp_body == "H6" && temp_degree == 1){
            ui->Dia->append("Stiff and painful shoulder muscles, tooth ache");
            ui->Organ->append("Large intestine");
            ui->Supp->append("Edmea Relieve - Promotes proper response to inflammation");
        }else if(temp_body == "H6" && temp_degree == -1){
            ui->Dia->append("Stiff and painful shoulder muscles");
            ui->Organ->append("Large intestine");
            ui->Supp->append("Edmea Relieve - Promotes proper response to inflammation");
        //F1
        }else if(temp_body == "F1" && temp_degree == 1){
            ui->Dia->append("General weakness of the stomach, knee joint pain");
            ui->Organ->append("Spleen / Pancrease");
            ui->Supp->append("Immuno Tune - Supports immune health function");
            ui->Supp->append("Pancre Norm - Normalizes blood sugar levels");
        }else if(temp_body == "F1" && temp_degree == -1){
            ui->Dia->append("General weakness of stomach,abnormalities of the knee joint, insomnia, glycosuria");
            ui->Organ->append("Spleen / Pancrease");
            ui->Supp->append("Immuno Tune - supports immune health function");
            ui->Supp->append("Pancre Norm - Normalizes blood sugar levels");
        //F2
        }else if(temp_body == "F2" && temp_degree == 1){
            ui->Dia->append("Insomnia, readily provoked abnormalities in menstruation, lumbar pain" );
            ui->Organ->append("Liver");
            ui->Supp->append("Hepa Gugull - support for liver function & recovery");
        }else if(temp_body == "F2" && temp_degree == -1){
            ui->Dia->append("Faintness or dizziness on abrupt standing up(orthostatic circulatory disorder)");
            ui->Organ->append("Liver");
            ui->Supp->append("Hepa Gugull - support for liver function & recovery");
        //F3
        }else if(temp_body == "F3" && temp_degree == 1){
            ui->Dia->append("Fretting and fuming, anxiety");
            ui->Organ->append("Kidney and Adrenal galnds");
            ui->Supp->append("RenActive - Kidney Health Support");
            ui->Supp->append("Edma Relieve - Promotes preper response to inflammation");
        }else if(temp_body == "F3" && temp_degree == -1){
            ui->Dia->append("Loss of willingness to concentrate and general tiredness, Coldness in hips and legs");
            ui->Organ->append("Kidney and Adrenal galnds");
            ui->Supp->append("RenActive - Kidney Health Support");
            ui->Supp->append("Edma Relieve - Promotes preper response to inflammation");
        //F4
        }else if(temp_body == "F4" && temp_degree == 1){
            ui->Dia->append("Stiff neck muscles, headaches, sciatic neurities, lumbar pain");
            ui->Organ->append("Bladder");
            ui->Supp->append("RenoSan - Regulates disorders including kidney stone issues");
        }else if(temp_body == "F4" && temp_degree == -1){
            ui->Dia->append("Stiffness and painful neck muscles, lumbar pain, dullness of the feet");
            ui->Organ->append("Bladder");
            ui->Supp->append("RenoSan - Regulates disorders including kidney stone issues");
        //F5
        }else if(temp_body == "F5" && temp_degree == 1){
            ui->Dia->append("Headaches");
            ui->Organ->append("GallBladder");
            ui->Supp->append("EnergyTune - Improves metabolic activation");
            ui->Supp->append("Digest Active - Digestive system support");
            ui->Supp->append("CalmBelly - Optimize your digestive process");
        }else if(temp_body == "F5" && temp_degree == -1){
            ui->Dia->append("Abnormalities of eyes, dizziness (Meniere's disease)");
            ui->Organ->append("GallBladder");
            ui->Supp->append("EnergyTune - Improves metabolic activation");
            ui->Supp->append("Digest Active - Digestive system support");
            ui->Supp->append("CalmBelly - Optimize your digestive process");
        //F6
        }else if(temp_body == "F6" && temp_degree == 1){
            ui->Dia->append("Anomalies of the joints, middle age wrenched shoulder, elbow neuralgia");
            ui->Organ->append("Stomach");
            ui->Supp->append("Digest Active - Digestive system support");
            ui->Supp->append("CalmBelly - Optimize your digestive process");
        }else if(temp_body == "F6" && temp_degree == -1){
            ui->Dia->append("Stiff and painful shoulder muscles, distention of stomach, bloating of face, yawning");
            ui->Organ->append("Stomach");
            ui->Supp->append("Digest Active - Digestive system support");
            ui->Supp->append("CalmBelly - Optimize your digestive process");
        }else{
            ui->Dia->append("ERROR");
        }

    }
}



void MainWindow::display_note(Profile* currentProfile){
    Note* n = history_viewer->get_note(currentProfile);

    ui->bodyTemp->setValue(n->bodyTemp);
    ui->celsiusRadioButton->setChecked(n->tempUnit == C);
    ui->fahrenheitRadioButton->setChecked(n->tempUnit == F);
    ui->bloodPressureLeftSystolic->setText(QString("%1").arg(n->bloodPressureRightDiastolic));
    ui->bloodPressureLeftDiastolic->setText(QString("%1").arg(n->bloodPressureLeftDiastolic));
    ui->bloodPressureRightSystolic->setText(QString("%1").arg(n->bloodPressureRightSystolic));
    ui->bloodPressureRightDiastolic->setText(QString("%1").arg(n->bloodPressureLeftSystolic));
    ui->heartRate->setText(QString("%1").arg(n->heartRate));
    ui->sleepHrs->setText(QString("%1").arg(n->sleepHrs));
    ui->sleepMins->setText(QString("%1").arg(n->sleepMins));
    ui->weight->setText(QString("%1").arg(n->weight));
    ui->lbsRadioButton->setChecked(n->weightUnit == LBS);
    ui->kgRadioButton->setChecked(n->weightUnit == KG);
    ui->notes->setText(n->notes);

    QVector<QRadioButton*> emoRadios = {ui->emoStateVerySad, ui->overallFeelingSad, ui->emoStateNeutral, ui->emoStateHappy, ui->emoStateVeryHappy};
    QVector<QRadioButton*> overallRadios = {ui->overallFeelingVerySad, ui->overallFeelingSad, ui->OverallFeelingNeutral, ui->overallFeelingHappy, ui->overallFeelingVeryHappy};
    emoRadios[n->emotionalState]->setChecked(true);
    overallRadios[n->overallFeeling]->setChecked(true);



    for (QPushButton *button : tagButtonGroup)
    {
        button->setChecked(false);
        for(QString tag_name : n->tags){
            if(button->text() == tag_name){
              button->setChecked(true);
            }
        }
    }

}

void MainWindow::update_chart(Profile* currentProfile){
    history_viewer->update_chart(*currentProfile->getSessions());
}
