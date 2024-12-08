#include "device.h"
#include <QDate>
#include <iostream>
#include <QDebug>
#include <QInputDialog>
#include <QComboBox>

#include "ui_createprofilewindow.h"
#include "ui_loginwindow.h"
#include "ui_mainwindow.h"

Device::Device(QObject *parent)
    : QObject{parent}
{
    /*
        Init the windos and UIs
    */
    mainWindow = new MainWindow;
    loginWindow = new LoginWindow;
    createWindow = new CreateProfileWindow;

    mainWindow->getUI(&mwUI);
    loginWindow->getUI(&lwUI);
    createWindow->getUI(&cwUI);


    /*
        Init default values and pointers
    */
    history_viewer = new HistoryViewer(mwUI->HistoryChart, mwUI->note_controls);

    tagButtonGroup = {mwUI->activeTag, mwUI->calmTag, mwUI->basisMorningTag};
    ranges = {
        {"H1 Left", {5, 190}}, {"H1 Right", {5, 190}}, {"H2 Left", {5, 170}}, {"H2 Right", {5, 170}}, {"H3 Left", {5, 140}}, {"H3 Right", {5, 140}}, {"H4 Left", {5, 170}}, {"H4 Right", {5, 170}}, {"H5 Left", {5, 200}}, {"H5 Right", {5, 200}}, {"H6 Left", {5, 200}}, {"H6 Right", {5, 200}}, {"F1 Left", {5, 160}}, {"F1 Right", {5, 160}}, {"F2 Left", {5, 130}}, {"F2 Right", {5, 130}}, {"F3 Left", {5, 150}}, {"F3 Right", {5, 150}}, {"F4 Left", {5, 150}}, {"F4 Right", {5, 150}}, {"F5 Left", {5, 130}}, {"F5 Right", {5, 130}}, {"F6 Left", {5, 150}}, {"F6 Right", {5, 150}}};
    // Initialize with default values
    for(const QString &spot: ranges.keys()){
        spotValues[spot] = ranges[spot].first;
    }

    // Add all checkboxes to the list
    scanCheckboxes = {
            mwUI->checkboxH1Left, mwUI->checkboxH1Right, mwUI->checkboxH2Left, mwUI->checkboxH2Right,
            mwUI->checkboxH3Left, mwUI->checkboxH3Right, mwUI->checkboxH4Left, mwUI->checkboxH4Right,
            mwUI->checkboxH5Left, mwUI->checkboxH5Right, mwUI->checkboxH6Left, mwUI->checkboxH6Right,
            mwUI->checkboxF1Left, mwUI->checkboxF1Right, mwUI->checkboxF2Left, mwUI->checkboxF2Right,
            mwUI->checkboxF3Left, mwUI->checkboxF3Right, mwUI->checkboxF4Left, mwUI->checkboxF4Right,
            mwUI->checkboxF5Left, mwUI->checkboxF5Right, mwUI->checkboxF6Left, mwUI->checkboxF6Right
        };


    /*
        Main Window Signals and Slots
    */
    // Logout Button Pressed
    connect(mwUI->btnLogOut, SIGNAL(pressed()), this, SLOT(onProfileLogout()));

    // Profile Button Pressed
    connect(mwUI->btnProfile, SIGNAL(pressed()), this, SLOT(onProfileShow()));

    // Auto Scan Button Pressed
    connect(mwUI->btnScan, SIGNAL(pressed()), this, SLOT(onAutoScanPressed()));

    // Save Notes Button Pressed
    connect(mwUI->saveBtn, SIGNAL(pressed()), this, SLOT(onSaveNotesPressed()));

    // Cycle Next and Previous Notes Button Pressed
    connect(mwUI->note_next, &QRadioButton::released, this, &Device::display_note);
    connect(mwUI->note_previous, &QRadioButton::released, this, &Device::display_note);

    // Chart Selection Combo Box Text Changed
    connect(mwUI->ChartSelection, &QComboBox::currentTextChanged, this, &Device::update_chart);

    // Connect dropdown to update the slider range and value
    connect(mwUI->dropdown, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index)
            {
            QString selectedOption = mwUI->dropdown->currentText();
            QPair<int,int> range = ranges[selectedOption];

            //update range
            mwUI->horizontalSlider->setRange(range.first,range.second);

            //reset slider to the minimum value of the range
            mwUI->horizontalSlider->setValue(range.first);
            mwUI->horizontalSlider->setEnabled(false);
            qDebug()<<"Prepare for scanning the next point. Please lift the device off the skin and move to the next scanning point.";
    });

    // Update slider value and store data
    connect(mwUI->horizontalSlider, &QSlider::valueChanged, this, [=](int value){
        mwUI->sliderValue->setText(QString::number(value));
        QString selectedSpot = mwUI->dropdown->currentText();
        spotValues[selectedSpot] = value;
    });

    // Skin Contact Checkbox Toggled
    connect(mwUI->skinContactChecked, &QCheckBox::toggled, this, &Device::handleCheckboxToggled);

    // Add Tag Button Pressed
    connect(mwUI->addTag, &QPushButton::clicked, this, &Device::onAddTagButtonClicked);

    // Temperature conversion
    connect(mwUI->fahrenheitRadioButton, &QRadioButton::pressed, this, &Device::onFahrenheitSelected);
    connect(mwUI->celsiusRadioButton, &QRadioButton::pressed, this, &Device::onCelsiusSelected);

    // Result Button Pressed
    connect(mwUI->result,SIGNAL(pressed()),this,SLOT(processRyodorakuData()));


    /*
        Login Window Signals and Slots
    */
    // Login Button Pressed
    connect(lwUI->btnLogin, SIGNAL(pressed()), this, SLOT(onProfileLogin()));

    // Create Profile Button Pressed
    connect(lwUI->btnCreateProfile, SIGNAL(pressed()), createWindow, SLOT(show()));
    connect(lwUI->btnCreateProfile, SIGNAL(pressed()), loginWindow, SLOT(hide()));


    /*
        Create Profile Window Signals and Slots
    */
    // Cancel Button Pressed
    connect(cwUI->btnCancel, SIGNAL(pressed()), loginWindow, SLOT(show()));
    connect(cwUI->btnCancel, SIGNAL(pressed()), createWindow, SLOT(hide()));

    // Create Profile Button Pressed
    connect(cwUI->btnCreateProfile, SIGNAL(pressed()), this, SLOT(onProfileCreated()));

    // Init Battery
    battery = new Battery();
    battery->add_battery_UI(mwUI->isCharging, mwUI->ChargeIndicator); // Adding UI elements to the battery
    battery->add_battery_UI(lwUI->isCharging, lwUI->ChargeIndicator);
    battery->add_battery_UI(cwUI->isCharging, cwUI->ChargeIndicator);
    battery->update_battery_UIs();
    connect(battery, &Battery::die, this, &Device::shutdown); // On Battery Depleted
    battery->turn_on_or_off(true); // start using battery power

    // Creating a test profile
    createProfile("Test", "User", UNDEFINED, 50, 175, QDate(), "911", "x@y.z", "test");

    // Show the loginWindow to start the program
    loginWindow->show();
}

bool Device::createProfile(string fName, string lName, SEX sex, float weight, float height, QDate date, string phoneNum, string email, string password)
{

    if (fName == "" || lName == "" || weight <= 0 || height <= 0 || phoneNum == "" || email == "" || password == ""){
        qDebug() << "Invalid input.";
        return false;
    }

    int maxUsers = NUM_USERS;
    if (profiles.size() < maxUsers){ // Checking if there are less than NUM_USERS
        profiles.push_back(new Profile(nextID++, fName, lName, sex, weight, height, date, phoneNum, email, password));

        qDebug() << "User created.";

        createWindow->hide();
        lwUI->cmbProfile->addItem(QString::fromStdString(fName + " " + lName)); // Adding the new profile to the loginWindow list of profiles
        loginWindow->show();
        return true;
    }
    else {
        qDebug() << "Maximum users reached.";
        return false;
    }
}

bool Device::verifyProfile(string password, int index)
{
    // Iterate through profiles to get the profile at index
    list<Profile*>::iterator it;
    it = profiles.begin();
    for (int i = 0; i < index; i++){
        it++;
    }

    if ((*it)->verifyPassword(password)){ // Verify the profiles password
        qDebug() << "User verified.";

        currentProfile = *it; // Setting the currentProfile

        // On Profile Updated
        connect(currentProfile, SIGNAL(profileUpdated(string)), this, SLOT(onProfileUpdate(string)));

        // On Profile Deleted
        connect(currentProfile, SIGNAL(profileDeleted()), this, SLOT(onProfileDeleted()));

        return true;
    }
    else {
        qDebug() << "Incorrect password.";
        return false;
    }
}

void Device::logoutProfile()
{
    if (currentProfile != NULL){
        disconnect(currentProfile, SIGNAL(profileUpdated(string)), this, SLOT(onProfileUpdate(string)));
        disconnect(currentProfile, SIGNAL(profileDeleted()), this, SLOT(onProfileDeleted()));
    }
    currentProfile = NULL;

    reset(); // Reset the data collection tab in mainWindow

    // Reset the recommendations tab in mainWindow
    mwUI->Dia->setText("");
    mwUI->Organ->setText("");
    mwUI->Supp->setText("");
}

void Device::addData()
{
    if(currentProfile->getSessions()->size() == 0){ //if user has no sessions
        qDebug("Adding random data.");
        //insert some random readings just to show graph history
        for (int i = 0; i < 15; i++){
            ReadingStorage* new_test_reading = new ReadingStorage(&ranges);
            new_test_reading->debug_populate_logs();
            currentProfile->log_session(new_test_reading);

        }
    }
    history_viewer->update_chart(*currentProfile->getSessions()); //update graph
}

void Device::display_note()
{
    Note* n = history_viewer->get_note(currentProfile);

    mwUI->bodyTemp->setValue(n->bodyTemp);
    mwUI->celsiusRadioButton->setChecked(n->tempUnit == C);
    mwUI->fahrenheitRadioButton->setChecked(n->tempUnit == F);
    mwUI->bloodPressureLeftSystolic->setText(QString("%1").arg(n->bloodPressureRightDiastolic));
    mwUI->bloodPressureLeftDiastolic->setText(QString("%1").arg(n->bloodPressureLeftDiastolic));
    mwUI->bloodPressureRightSystolic->setText(QString("%1").arg(n->bloodPressureRightSystolic));
    mwUI->bloodPressureRightDiastolic->setText(QString("%1").arg(n->bloodPressureLeftSystolic));
    mwUI->heartRate->setText(QString("%1").arg(n->heartRate));
    mwUI->sleepHrs->setText(QString("%1").arg(n->sleepHrs));
    mwUI->sleepMins->setText(QString("%1").arg(n->sleepMins));
    mwUI->weight->setText(QString("%1").arg(n->weight));
    mwUI->lbsRadioButton->setChecked(n->weightUnit == LBS);
    mwUI->kgRadioButton->setChecked(n->weightUnit == KG);
    mwUI->notes->setText(n->notes);

    QVector<QRadioButton*> emoRadios = {mwUI->emoStateVerySad, mwUI->overallFeelingSad, mwUI->emoStateNeutral, mwUI->emoStateHappy, mwUI->emoStateVeryHappy};
    QVector<QRadioButton*> overallRadios = {mwUI->overallFeelingVerySad, mwUI->overallFeelingSad, mwUI->OverallFeelingNeutral, mwUI->overallFeelingHappy, mwUI->overallFeelingVeryHappy};
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

void Device::update_chart()
{
    history_viewer->update_chart(*currentProfile->getSessions());
}

void Device::reset()
{
    // Reset the spot values.
    for (const QString &key : spotValues.keys()) {
        spotValues[key] = 5;
    }

    // Set mainWindow ui elements to default states and values
    mwUI->dropdown->setEnabled(false);
    mwUI->dropdown->setCurrentIndex(0);
    mwUI->horizontalSlider->setValue(5);
    mwUI->horizontalSlider->setEnabled(false);
    mwUI->skinContactChecked->setChecked(false);
    for (QCheckBox *checkbox : scanCheckboxes){
        checkbox->setChecked(false);
    }
    mwUI->result->setEnabled(false);
}

void Device::shutdown(){
    QMessageBox msgBox;
    msgBox.setText("Battery Exhausted, Shutting Down");
    msgBox.exec();
    if(mainWindow){mainWindow->close(); delete mainWindow;}
    if(loginWindow){loginWindow->close(); delete loginWindow;}
    if(createWindow){createWindow->close(); delete createWindow;}
    exit(0);
}

void Device::handleCheckboxToggled(bool checked)
{
    // last State is false (lift the device off the skin) and putting it back on the next point
    if (checked == true && lastState == false)
    {
        qDebug() << "Successive measurement.";
        mwUI->horizontalSlider->setEnabled(true);
        mwUI->dropdown->setEnabled(true);
        trackScanning();
    }
    else if (checked == false && lastState == true )
    {
        qDebug() << "The device is off the skin";
        mwUI->horizontalSlider->setEnabled(false);
    }
    else if(checked == true && lastState == true){
        qDebug() << "The device is off the skin";
        mwUI->horizontalSlider->setEnabled(false);
    }
    lastState = checked;
}

void Device::trackScanning()
{
    QString selectedSpot = mwUI->dropdown->currentText();
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

void Device::checkAllScansCompleted()
{
    bool allChecked = true;
    for(QCheckBox *checkbox : scanCheckboxes){
        if(!checkbox->isChecked()){
            allChecked = false;
            break;
        }
    }
    if(allChecked){
        mwUI->result->setEnabled(true);
    }
}

int Device::calculateAverage()
{
    int total = 0;
    for(const int &value : spotValues){
        total += value;

    }
    return total/spotValues.size();
}

void Device::PrintDia()
{
    mwUI->Dia->setText("");
    mwUI->Organ->setText("");
    mwUI->Supp->setText("");

    QMap<QString,int> currentSession = *currentProfile->getSessions()->front()->get_readings(); // Get the most recent readings

    recommend.AddAbnormalPartinQ("H1 Left","H1 Right", currentSession);
    recommend.AddAbnormalPartinQ("H2 Left","H2 Right", currentSession);
    recommend.AddAbnormalPartinQ("H3 Left","H3 Right", currentSession);
    recommend.AddAbnormalPartinQ("H4 Left","H4 Right", currentSession);
    recommend.AddAbnormalPartinQ("H5 Left","H5 Right", currentSession);
    recommend.AddAbnormalPartinQ("H6 Left","H6 Right", currentSession);
    recommend.AddAbnormalPartinQ("F1 Left","F1 Right", currentSession);
    recommend.AddAbnormalPartinQ("F2 Left","F2 Right", currentSession);
    recommend.AddAbnormalPartinQ("F3 Left","F3 Right", currentSession);
    recommend.AddAbnormalPartinQ("F4 Left","F4 Right", currentSession);
    recommend.AddAbnormalPartinQ("F5 Left","F5 Right", currentSession);
    recommend.AddAbnormalPartinQ("F6 Left","F6 Right", currentSession);
    QString temp_body = "";
    int temp_degree = 0;
    if(recommend.GetWrong_partSize() == 0){
        mwUI->Dia->append("Every part is healthy");
        mwUI->Organ->append("None");
        mwUI->Supp->append("None");
    }
    while(recommend.GetWrong_partSize() != 0)
    {
        temp_body = recommend.GetWrong_part();
        temp_degree = recommend.Getresult();
        if(temp_body == "H1" && temp_degree == 1)
        {
            mwUI->Dia->append("Stiff and painful sholder muscles, rush of blood to the head"
                            "hot lfashes, piles, asthma");
            mwUI->Organ->append("Lung");
            mwUI->Supp->append("Propolis - Improve immune defense");
        }else if(temp_body == "H1" && temp_degree == -1){
            mwUI->Dia->append("Cold feet or numbness, shortness of breath, coughing spells");
            mwUI->Organ->append("Lung");
            mwUI->Supp->append("Propolis - Improve immune defense");
        //H2
        }else if(temp_body == "H2" && temp_degree == 1){
            mwUI->Dia->append("Stiff and painful shoulder muscles, middle age level wrenched"
                            " shoulders, brachical neuralgia.");
            mwUI->Organ->append("Pericardium");
            mwUI->Supp->append("Veino Tune - Improves venous blood flow");

        }else if(temp_body == "H2" && temp_degree == -1){
            mwUI->Dia->append("Palpitation, heated sensation of the palms");
            mwUI->Organ->append("Pericardium");
            mwUI->Supp->append("Veino Tune - Improves venous blood flow");
        //H3
        }else if(temp_body == "H3" && temp_degree == 1){
            mwUI->Dia->append("Puffed sensation of the stomach, constipation");
            mwUI->Organ->append("Heart");
            mwUI->Supp->append("AntimeGrin Hyper - Helps with high blood pressure relief and optimization");
        }else if(temp_body == "H3" && temp_degree == -1){
            mwUI->Dia->append("Palpitation");
            mwUI->Organ->append("Heart");
            mwUI->Supp->append("Migrenol Hypo - Helps raise low blood pressure and optimization");
            mwUI->Supp->append("Immuno Tune - Spports immune health function");
        //H4
        }else if(temp_body == "H4" && temp_degree == 1){
            mwUI->Dia->append("Headaches, abnormalities of the lower abdomen, joint pain");
            mwUI->Organ->append("Small intestine");
            mwUI->Supp->append("Intesti San - helps maintain intestinal health");
            mwUI->Supp->append("Immuno Tune - Spports immune health function");
        }else if(temp_body == "H4" && temp_degree == -1){
            mwUI->Dia->append("Headache, abnormalities of the lower abdomen");
            mwUI->Organ->append("Small intestine");
            mwUI->Supp->append("Intesti San - helps maintain intestinal health");
        //H5
        }else if(temp_body == "H5" && temp_degree == 1){
            mwUI->Dia->append("Ringing ears, difficulty in hearing");
            mwUI->Organ->append("Lymph vessel");
            mwUI->Supp->append("Edmea Relieve - Promotes proper response to inflammation");
            mwUI->Supp->append("Immuno Tune - Spports immune health function");
        }else if(temp_body == "H5" && temp_degree == -1){
            mwUI->Dia->append("Tiredness or tendency to tire, the healthy glow disappears and body hairs"
                            " increase in density");
            mwUI->Organ->append("Lymph vessel");
            mwUI->Supp->append("Edmea Relieve - Promotes proper response to inflammation");
            mwUI->Supp->append("Immuno Tune - Spports immune health function");
        //H6
        }else if(temp_body == "H6" && temp_degree == 1){
            mwUI->Dia->append("Stiff and painful shoulder muscles, tooth ache");
            mwUI->Organ->append("Large intestine");
            mwUI->Supp->append("Edmea Relieve - Promotes proper response to inflammation");
        }else if(temp_body == "H6" && temp_degree == -1){
            mwUI->Dia->append("Stiff and painful shoulder muscles");
            mwUI->Organ->append("Large intestine");
            mwUI->Supp->append("Edmea Relieve - Promotes proper response to inflammation");
        //F1
        }else if(temp_body == "F1" && temp_degree == 1){
            mwUI->Dia->append("General weakness of the stomach, knee joint pain");
            mwUI->Organ->append("Spleen / Pancrease");
            mwUI->Supp->append("Immuno Tune - Supports immune health function");
            mwUI->Supp->append("Pancre Norm - Normalizes blood sugar levels");
        }else if(temp_body == "F1" && temp_degree == -1){
            mwUI->Dia->append("General weakness of stomach,abnormalities of the knee joint, insomnia, glycosuria");
            mwUI->Organ->append("Spleen / Pancrease");
            mwUI->Supp->append("Immuno Tune - supports immune health function");
            mwUI->Supp->append("Pancre Norm - Normalizes blood sugar levels");
        //F2
        }else if(temp_body == "F2" && temp_degree == 1){
            mwUI->Dia->append("Insomnia, readily provoked abnormalities in menstruation, lumbar pain" );
            mwUI->Organ->append("Liver");
            mwUI->Supp->append("Hepa Gugull - support for liver function & recovery");
        }else if(temp_body == "F2" && temp_degree == -1){
            mwUI->Dia->append("Faintness or dizziness on abrupt standing up(orthostatic circulatory disorder)");
            mwUI->Organ->append("Liver");
            mwUI->Supp->append("Hepa Gugull - support for liver function & recovery");
        //F3
        }else if(temp_body == "F3" && temp_degree == 1){
            mwUI->Dia->append("Fretting and fuming, anxiety");
            mwUI->Organ->append("Kidney and Adrenal galnds");
            mwUI->Supp->append("RenActive - Kidney Health Support");
            mwUI->Supp->append("Edma Relieve - Promotes preper response to inflammation");
        }else if(temp_body == "F3" && temp_degree == -1){
            mwUI->Dia->append("Loss of willingness to concentrate and general tiredness, Coldness in hips and legs");
            mwUI->Organ->append("Kidney and Adrenal galnds");
            mwUI->Supp->append("RenActive - Kidney Health Support");
            mwUI->Supp->append("Edma Relieve - Promotes preper response to inflammation");
        //F4
        }else if(temp_body == "F4" && temp_degree == 1){
            mwUI->Dia->append("Stiff neck muscles, headaches, sciatic neurities, lumbar pain");
            mwUI->Organ->append("Bladder");
            mwUI->Supp->append("RenoSan - Regulates disorders including kidney stone issues");
        }else if(temp_body == "F4" && temp_degree == -1){
            mwUI->Dia->append("Stiffness and painful neck muscles, lumbar pain, dullness of the feet");
            mwUI->Organ->append("Bladder");
            mwUI->Supp->append("RenoSan - Regulates disorders including kidney stone issues");
        //F5
        }else if(temp_body == "F5" && temp_degree == 1){
            mwUI->Dia->append("Headaches");
            mwUI->Organ->append("GallBladder");
            mwUI->Supp->append("EnergyTune - Improves metabolic activation");
            mwUI->Supp->append("Digest Active - Digestive system support");
            mwUI->Supp->append("CalmBelly - Optimize your digestive process");
        }else if(temp_body == "F5" && temp_degree == -1){
            mwUI->Dia->append("Abnormalities of eyes, dizziness (Meniere's disease)");
            mwUI->Organ->append("GallBladder");
            mwUI->Supp->append("EnergyTune - Improves metabolic activation");
            mwUI->Supp->append("Digest Active - Digestive system support");
            mwUI->Supp->append("CalmBelly - Optimize your digestive process");
        //F6
        }else if(temp_body == "F6" && temp_degree == 1){
            mwUI->Dia->append("Anomalies of the joints, middle age wrenched shoulder, elbow neuralgia");
            mwUI->Organ->append("Stomach");
            mwUI->Supp->append("Digest Active - Digestive system support");
            mwUI->Supp->append("CalmBelly - Optimize your digestive process");
        }else if(temp_body == "F6" && temp_degree == -1){
            mwUI->Dia->append("Stiff and painful shoulder muscles, distention of stomach, bloating of face, yawning");
            mwUI->Organ->append("Stomach");
            mwUI->Supp->append("Digest Active - Digestive system support");
            mwUI->Supp->append("CalmBelly - Optimize your digestive process");
        }else{
            mwUI->Dia->append("ERROR");
        }
    }
}

void Device::onProfileCreated()
{
    // Get all of the information from the createWindow
    string fName = cwUI->txtFName->text().toStdString();
    string lName = cwUI->txtLName->text().toStdString();
    float weight = cwUI->dsbWeight->value();
    float height = cwUI->dsbHeight->value();
    QDate date = cwUI->datDOB->date();
    string phoneNum = cwUI->txtPhoneNum->text().toStdString();
    string email = cwUI->txtEmail->text().toStdString();
    string password = cwUI->txtPass->text().toStdString();
    string conPassword = cwUI->txtConPass->text().toStdString();

    // Get which radio button is checked
    SEX sex;
    if (cwUI->rbtnMale->isChecked()){
        sex = MALE;
    }
    else if (cwUI->rbtnFemale->isChecked()){
        sex = FEMALE;
    }
    else sex = UNDEFINED;

    // Check that both passwords match
    if (password.compare(conPassword)){
        qDebug() << "Passwords do not match.";
        cwUI->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        cwUI->txtConPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        return;
    }

    // Boolean to check if the profile was succesfully created
    bool validUser = createProfile(fName, lName, sex, weight, height, date, phoneNum, email, password);

    // If the profile wasnt valid, set the empty fields to red.
    if (!validUser){
        if (fName == "") cwUI->txtFName->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtFName->setStyleSheet("");
        if (lName == "") cwUI->txtLName->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtLName->setStyleSheet("");
        if (weight <= 0) cwUI->dsbWeight->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->dsbWeight->setStyleSheet("");
        if (height <= 0) cwUI->dsbHeight->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->dsbHeight->setStyleSheet("");
        if (phoneNum == "") cwUI->txtPhoneNum->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtPhoneNum->setStyleSheet("");
        if (email == "") cwUI->txtEmail->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtEmail->setStyleSheet("");
        if (password == "") cwUI->txtPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtPass->setStyleSheet("");
        if (conPassword == "") cwUI->txtConPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
        else cwUI->txtConPass->setStyleSheet("");
    }
    else { // Otherwise set the creatWindow UI to default
        cwUI->txtFName->setStyleSheet("");
        cwUI->txtLName->setStyleSheet("");
        cwUI->dsbWeight->setStyleSheet("");
        cwUI->dsbHeight->setStyleSheet("");
        cwUI->txtPhoneNum->setStyleSheet("");
        cwUI->txtEmail->setStyleSheet("");
        cwUI->txtPass->setStyleSheet("");
        cwUI->txtConPass->setStyleSheet("");
    }
}

void Device::onProfileLogin()
{
    // Get the password and index of the profile
    string password = lwUI->txtLoginPass->text().toStdString();
    int index = lwUI->cmbProfile->currentIndex();

    // If the loginWindow profiles list is empty/there are no profiles
    if (index == -1) return;

    // Verify the profile
    bool loggedIn = verifyProfile(password, index);

    if (loggedIn){ // If the profile is verified, set up the mainWindow for the currentProfile
        mainWindow->show();
        mwUI->lblCurrentUser->setText(QString::fromStdString(currentProfile->getName()));

        addData(); // Display data in the chart
        display_note();
        PrintDia(); // Display the most recent recommendations

        loginWindow->hide();
        lwUI->txtLoginPass->setStyleSheet("");
    }
    else{
        lwUI->txtLoginPass->setStyleSheet("border-style: solid;border-width: 2px;border-color: red");
    }

    lwUI->txtLoginPass->setText("");
}

void Device::onProfileLogout()
{
    qDebug() << "User Logged out.";
    mainWindow->hide();
    mwUI->lblCurrentUser->setText(QString::fromStdString(""));
    loginWindow->show();
    logoutProfile();
}

void Device::onProfileUpdate(string name)
{
    qDebug() << "User Updated.";
    int index = lwUI->cmbProfile->currentIndex();
    lwUI->cmbProfile->setItemText(index, QString::fromStdString(name)); // Update the profile name in the loginWindow
    mwUI->lblCurrentUser->setText(QString::fromStdString(name)); // Update the profile name in the mainWindow
}

void Device::onProfileDeleted()
{
    list<Profile*>::iterator it;
    it = profiles.begin();
    for (;it != profiles.end(); it++){ // Iterate through the profiles
        if ((*it) == currentProfile){ // When the currentProfile (the one to delete) is reached
            // Remove the profile from the list
            profiles.remove(currentProfile);

            // Remove the profile from all UI
            qDebug() << "User Deleted.";
            int index = lwUI->cmbProfile->currentIndex();
            lwUI->cmbProfile->removeItem(index);

            if (profiles.size() == 0){ // If there are not more profiles, show the createProfileWindow
                createWindow->show();
            }
            else // Otherwise just show the loginWindow
                loginWindow->show();

            mainWindow->hide();

            break;
        }
    }
}

void Device::onProfileShow()
{
    if (currentProfile != NULL)
        currentProfile->showProfile();
}

void Device::onAutoScanPressed()
{
    // Reset the scan
    mwUI->skinContactChecked->setChecked(false);
    lastState = false;

    srand(time(0));
    int numScans = scanCheckboxes.size();
    for (int i = 0; i < numScans; i++) { // For each scanCheckbox
        mwUI->dropdown->setCurrentIndex(i); // Set the dropdown to the current index
        mwUI->skinContactChecked->toggle(); // Toggle the skin contact
        QPair<int,int> range = ranges[mwUI->dropdown->currentText()]; // Get the ranges for the selected option
        // (rand()%(range.second-range.first+1))+range.first
        mwUI->horizontalSlider->setValue(5); // Randomize the value of the slider
        mwUI->skinContactChecked->toggle(); // Toggle the skin contact
    }
}

void Device::onFahrenheitSelected()
{
    mwUI->bodyTemp->setRange(32.0, 212.0);
    mwUI->bodyTemp->setValue(32.0);
}

void Device::onCelsiusSelected()
{
    mwUI->bodyTemp->setRange(0.0, 100.0);
    mwUI->bodyTemp->setValue(0.0);
}

void Device::onAddTagButtonClicked()
{
    bool ok;
    QString tagName = QInputDialog::getText(mainWindow, "Add Tag", "Enter Tag Name:", QLineEdit::Normal, "", &ok);

    // If the user clicks "OK" and enters a name, create the tag button
    if (ok && !tagName.isEmpty())
    {
        QPushButton *newTagButton = new QPushButton("+" + tagName, mainWindow);
        newTagButton->setCheckable(true);

        // Add the button to the layout
        mwUI->formLayout->addWidget(newTagButton);

        // add the button to the tagButtonGroup
        tagButtonGroup.append(newTagButton);
    }
    mwUI->addTag->setChecked(false);
}

void Device::onSaveNotesPressed()
{
    Note* new_note = history_viewer->get_note(currentProfile);

    // Retreive all the informations
    new_note->bodyTemp = mwUI->bodyTemp->value();
    new_note->tempUnit = mwUI->celsiusRadioButton->isChecked() ? C : F;
    new_note->bloodPressureLeftSystolic = mwUI->bloodPressureLeftSystolic->text().toInt();
    new_note->bloodPressureLeftDiastolic = mwUI->bloodPressureLeftDiastolic->text().toInt();
    new_note->bloodPressureRightSystolic = mwUI->bloodPressureRightSystolic->text().toInt();
    new_note->bloodPressureRightDiastolic = mwUI->bloodPressureRightDiastolic->text().toInt();
    new_note->heartRate = mwUI->heartRate->text().toInt();
    new_note->sleepHrs = mwUI->sleepHrs->text().toInt();
    new_note->sleepMins = mwUI->sleepMins->text().toInt();
    new_note->weight = mwUI->weight->text().toInt();
    new_note->weightUnit = mwUI->lbsRadioButton->isChecked() ? LBS : KG;
    new_note->notes = mwUI->notes->toPlainText();

    if (mwUI->emoStateVerySad->isChecked())
        new_note->emotionalState = VERY_SAD;
    else if (mwUI->emoStateSad->isChecked())
        new_note->emotionalState = SAD;
    else if (mwUI->emoStateNeutral->isChecked())
        new_note->emotionalState = NEUTRAL;
    else if (mwUI->emoStateHappy->isChecked())
        new_note->emotionalState = HAPPY;
    else if (mwUI->emoStateVeryHappy->isChecked())
        new_note->emotionalState = VERY_HAPPY;

    if (mwUI->overallFeelingVerySad->isChecked())
        new_note->overallFeeling = VERY_SAD;
    else if (mwUI->overallFeelingSad->isChecked())
        new_note->overallFeeling = SAD;
    else if (mwUI->OverallFeelingNeutral->isChecked())
        new_note->overallFeeling = NEUTRAL;
    else if (mwUI->overallFeelingHappy->isChecked())
        new_note->overallFeeling = HAPPY;
    else if (mwUI->overallFeelingVeryHappy->isChecked())
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

    //qDebug() << "DEBUG DATA: " << data;
}


void Device::processRyodorakuData()
{
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

        //qDebug() << "Ryodoraku Data Results:";
        //for (const QString &key : results.keys()) {
        //    qDebug() << key << ":" << results[key];
        //}

    PrintDia();

    reset(); // Reset the data collection tab after each set of readings is processed
}
