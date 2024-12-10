#include "battery.h"


Battery::Battery()
{
    in_use = false; //not in use to start
    plugged_in = false; //not plugged in to start
    charge_level = CHARGE_LEVEL_START; //full battery to start

    charge_indicator_palette = QPalette(); // newdefault palette

    battery_sim(); //start simulation

}

void Battery::add_battery_UI(QCheckBox* charging_port_UI, QProgressBar* charging_indicator_UI){
    charging_ports.push_back(charging_port_UI);
    charging_indicators.push_back(charging_indicator_UI);
    connect(charging_port_UI, SIGNAL(clicked(bool)), this, SLOT(plug_change(bool)));
}

//set the device as either using or not using power
void Battery::turn_on_or_off(bool isOn){
    in_use = isOn;
}


//simulate battery use, inclusing charging, power drain, and UI updates
void Battery::battery_sim(){

    if(in_use){ //if device is in use, drain battery
        charge_level -= DRAIN_RATE;
    }

    //if device is plugged in, charge battery
    if(plugged_in){
        charge_level += CHARGE_RATE;
    }

    //run off excess charge
    if(charge_level > 100){
        charge_level = 100;
    }

    //if the battery is dead
    if(charge_level <= 0){
        charge_level = 0;
        qDebug("battery dead");
        emit die();
    }

    update_battery_UIs(); //update charge indicators
    QTimer::singleShot(1000, this, &Battery::battery_sim); //run battery sim again in one second
}


//update UI battery indicator
void Battery::update_battery_UIs(){

    //pick bar colour
    QString colour = (int)charge_level > 25 ? "green" : (int)charge_level > 10 ? "yellow" :  "red";
    //update palette
    charge_indicator_palette.setColor(QPalette::Highlight, colour);


    //for each battery UI
    for(QProgressBar* charging_indicator : charging_indicators){
        //set percent value
        charging_indicator->setValue((int)charge_level);
        //send new palette to charging indicator
        charging_indicator->setPalette(charge_indicator_palette);
    }

}

//signal when a plug is checked on or off
void Battery::plug_change(bool state){
    plugged_in = state; //update plug state
    for(QCheckBox* charging_port : charging_ports){
        charging_port->setChecked(plugged_in); //update change to all UI checkboxes
    }
}


