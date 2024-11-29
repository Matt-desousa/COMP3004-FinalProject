#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QCheckBox>
#include <QProgressBar>
#include <QTimer>
#include <QtGui/QPalette>

#include "defs.h"

class Battery : public QObject
{
    Q_OBJECT

public:

    explicit Battery(QCheckBox* charging_port_UI, QProgressBar* charging_indicator_UI);

    void turn_on_or_off(bool isOn); //start or stop using battery power
    inline double get_battery_percent(){return charge_level;}

signals:

private:

    QCheckBox* charging_port; //checkbox whhich represents being plugged in
    QProgressBar* charging_indicator; //charging indicator on UI that displays percentage left
    QPalette charge_indicator_palette; //colour palette for the charge indicator

    bool in_use; //whether power is aactively being drained
    double charge_level; //battery percent

    void battery_sim(); //simulate battery use
    void update_charge_UI(); //update the UI indicator



};

#endif // BATTERY_H
