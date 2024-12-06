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

    explicit Battery();

    void turn_on_or_off(bool isOn); //start or stop using battery power
    inline double get_battery_percent(){return charge_level;}
    void add_battery_UI(QCheckBox* charging_port_UI, QProgressBar* charging_indicator_UI);
    void update_battery_UIs(); //update the UI indicator

signals:
    void die();

private:

    QVector<QCheckBox*> charging_ports; //checkboxes whhich represents being plugged in
    QVector<QProgressBar*> charging_indicators; //charging indicator on UI that displays percentage left
    QPalette charge_indicator_palette; //colour palette for the charge indicator

    bool in_use; //whether power is actively being drained
    bool plugged_in;
    double charge_level; //battery percent

    void battery_sim(); //simulate battery use

public slots:
    void plug_change(bool);


};

#endif // BATTERY_H
