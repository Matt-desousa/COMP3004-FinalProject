#ifndef DEFS_H
#define DEFS_H

#include <QObject>
#include <QMap>

enum SEX{
    FEMALE,
    MALE,
    UNDEFINED
};

enum MASS{
    MASS_UNIT_UNSPECIFIED,
    KG,
    LBS
};

enum TEMP{
    TEMP_UNIT_UNSPECIFIED,
    C,
    F
};

enum MOOD{
    VERY_SAD,
    SAD,
    NEUTRAL,
    HAPPY,
    VERY_HAPPY
};

#define NUM_USERS 5;            // Number of Users
#define CHARGE_LEVEL_START 40   //%starting charge
#define CHARGE_RATE 3.0f;       // %/sec charge
#define DRAIN_RATE 1.0f;        // %/sec drain

#endif // DEFS_H
