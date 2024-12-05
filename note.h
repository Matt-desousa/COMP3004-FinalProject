#ifndef NOTE_H
#define NOTE_H

#include <QObject>

#include "defs.h"

class Note : public QObject
{
    Q_OBJECT
public:
    explicit Note();

    double bodyTemp;
    TEMP tempUnit;
    int bloodPressureLeftSystolic;
    int bloodPressureLeftDiastolic;
    int bloodPressureRightSystolic;
    int bloodPressureRightDiastolic;
    int heartRate;
    int sleepHrs;
    int sleepMins;
    float weight;
    MASS weightUnit;
    MOOD emotionalState;
    MOOD overallFeeling;
    QString tagsString;
    QString notes;
};

#endif // NOTE_H
