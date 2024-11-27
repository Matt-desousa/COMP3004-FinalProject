#include "battery.h"


Battery::Battery(QCheckBox* charging_port_UI, QProgressBar* charging_indicator_UI)
{
    in_use = false; //not in use to start
    charge_level = 100; //full battery to start

    charging_port = charging_port_UI;

    charging_indicator = charging_indicator_UI;
    charge_indicator_palette = QPalette(); // newdefault palette

    battery_sim(); //start simulation

}

//set the device as either using or not using power
void Battery::turn_on_or_off(bool isOn){
    in_use = isOn;
}


//simulate battery use, inclusing charging, power drain, and UI updates
void Battery::battery_sim(){

    if(in_use){ //if device is in use, drain battery
        charge_level -= drain_rate;
    }

    //if device is plugged in, charge battery
    if(charging_port->isChecked()){
        charge_level += charge_rate;
    }

    //run off excess charge
    if(charge_level > 100){
        charge_level = 100;
    }

    //if the battery is dead
    if(charge_level < 0){
        charge_level = 0;
        //TODO power off machine
    }

    update_charge_UI(); //update charge indicator
    QTimer::singleShot(1000, this, &Battery::battery_sim); //run battery sim again in one second
}


//update UI battery indicator
void Battery::update_charge_UI(){


    //set percent value
    charging_indicator->setValue((int)charge_level);

    //pick bar colour
    QString colour = (int)charge_level > 25 ? "green" : (int)charge_level > 10 ? "yellow" :  "red";
    //update palette
    charge_indicator_palette.setColor(QPalette::Highlight, colour);
    //send new palette to charging indicator
    charging_indicator->setPalette(charge_indicator_palette);
}


