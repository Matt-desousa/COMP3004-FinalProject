#include "readingstorage.h"

ReadingStorage::ReadingStorage(QMap<QString,QPair<int,int>>* body_parts)
{
    body_parts_info = body_parts; //store ranges for percentage calculations
    note = new Note(); //store a note page
}

ReadingStorage::~ReadingStorage()
{
    delete note;
}

//add a reading
void ReadingStorage::log_data_point(QString body_part, int  reading){
    readings[body_part] = reading;
}

//get a reading
int  ReadingStorage::retrieve_data_point(QString body_part){
    return readings[body_part];
}

//get a reading percentage (percent of the way between the recommended max and min)
int  ReadingStorage::retrieve_data_point_percent(QString body_part){
    QPair<int,int> minmax = (*body_parts_info)[body_part];
    return 100 * ((float)(retrieve_data_point(body_part) - minmax.first) / (float)(minmax.second - minmax.first));
}

//get min and max
QPair<int,int> ReadingStorage::retrieve_data_point_range(QString body_part){
    return (*body_parts_info)[body_part];
}

//average entire reading
int ReadingStorage::retrieve_session_average(){
    int total = 0;
    for (QString &p : (*body_parts_info).keys() ) //for every body part...
    {
       total += retrieve_data_point(p);
    }
    return total/body_parts_info->size(); //return average
}

//print a point for debugging
void ReadingStorage::debug_print(QString body_part){
    qDebug()<<"Logging Point: "
           << "Value:" << retrieve_data_point(body_part)
           << "- Percentage of range" << retrieve_data_point_percent(body_part) << "%";
}

//randomly fill logs
void ReadingStorage::debug_populate_logs(){
    for (const QString &p: (*body_parts_info).keys()) //for every body part...
    {
        QString  body_part = p;
        QPair<int, int> val_range = retrieve_data_point_range(body_part);
        int random_val = QRandomGenerator::global()->bounded(val_range.first, val_range.second);
        log_data_point(body_part,  random_val); //add random point reading between min and max
    }

    //get some random vals
    int r1 = QRandomGenerator::global()->bounded(0, 100);
    int r2 = QRandomGenerator::global()->bounded(0, 100);
    int r3 = QRandomGenerator::global()->bounded(0, 100);
    note->notes = QString("%1 %2 %3").arg(QString((char)r1+r2),QString((char)r2+r3),QString((char)r3+r1)); //generate some ascii junk
    note->sleepHrs = r1/2; //and some more random values...
    note->sleepMins = r2/2;
    note->weightUnit = LBS;
    note->tempUnit = C;
    note->weight = r1*2;
    note->bodyTemp = r1+r2-r3;
}

