#include "note.h"

Note::Note()
{

     bodyTemp = -1;
     tempUnit = TEMP_UNIT_UNSPECIFIED;
     bloodPressureLeftSystolic = -1;
     bloodPressureLeftDiastolic = -1;
     bloodPressureRightSystolic = -1;
     bloodPressureRightDiastolic = -1;
     heartRate = -1;
     sleepHrs = -1;
     sleepMins = -1;
     weight = -1;
     weightUnit = MASS_UNIT_UNSPECIFIED;
     emotionalState = NEUTRAL;
     overallFeeling = NEUTRAL;
     tagsString = "NONE";
     notes = "NONE";

}
