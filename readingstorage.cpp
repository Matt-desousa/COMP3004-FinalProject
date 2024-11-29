#include "readingstorage.h"

ReadingStorage::ReadingStorage(QMap<QString,QPair<int,int>>* body_parts)
{
    for ( const auto &p : *body_parts ) //for every body part...
    {
       readings.insert((QString)p.first, -1); //insert empty data point
    }
    body_parts_info = body_parts; //store ranges for percentage calculations
    note = new Note(); //store a note page
}


//add a reading
void ReadingStorage::log_data_point(QString body_part, int  reading){
    readings[body_part] = reading;
    debug_print(body_part);
}

//get a reading
int  ReadingStorage::retrieve_data_point(QString body_part){
    return readings[body_part];
}

int ReadingStorage::retrieve_data_point_average(QString body_part){
    return readings[body_part];//sort out later
}


//get a reading percentage (percent of the way between the recommended max and min)
int  ReadingStorage::retrieve_data_point_percent(QString body_part){
    QPair<int,int> minmax = (*body_parts_info)[body_part];
    qDebug() << minmax.first << minmax.second;
    return 100 * ((float)(retrieve_data_point(body_part) - minmax.first) / (float)(minmax.second - minmax.first));
}

//average enntire reading
int ReadingStorage::retrieve_session_average(){
    int total = 0;
    for ( const auto &p : *body_parts_info ) //for every body part...
    {
       total += retrieve_data_point((QString)p.first);
    }
    return total/body_parts_info->size(); //return average
}

//print a point for debugging
void ReadingStorage::debug_print(QString body_part){
    qDebug()<<"Logging Point: "
           << "Value:" << retrieve_data_point(body_part)
           << "- Percentage of range" << retrieve_data_point_percent(body_part) << "%";
}

