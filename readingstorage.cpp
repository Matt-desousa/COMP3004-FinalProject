#include "readingstorage.h"

ReadingStorage::ReadingStorage(QMap<QString,QPair<int,int>>* body_parts)
{
    for ( const auto &p : *body_parts ) //for every body part...
    {
       readings.insert((QString)p.first, QPair<int,int>(-1,-1)); //insert empty data point
    }
    body_parts_info = body_parts; //store ranges for percentage calculations
    note = new Note(); //store a note page
}


//add a reading
void ReadingStorage::log_data_point(QString body_part, QPair<int,int>  reading){
    readings[body_part] = reading;
    debug_print(body_part);
}

//get a reading
QPair<int,int>  ReadingStorage::retrieve_data_point(QString body_part){
    return readings[body_part];
}

//get a reading average (half way between max and min)
int  ReadingStorage::retrieve_data_point_average(QString body_part){
    return (readings[body_part].first + readings[body_part].second)/2;
}

//get a reading percentage (percent of the way between the recommended max and min)
int  ReadingStorage::retrieve_data_point_percent(QString body_part){
    int average = retrieve_data_point_average(body_part);
    QPair<int,int> minmax = (*body_parts_info)[body_part];
    return 100 * ((float)(average - minmax.first) / (float)(minmax.second - minmax.first));
}

//average enntire reading
int ReadingStorage::retrieve_session_average(){
    int total = 0;
    for ( const auto &p : *body_parts_info ) //for every body part...
    {
       total += retrieve_data_point_average((QString)p.first);
    }
    return total/body_parts_info->size(); //return average
}

//print a point for debugging
void ReadingStorage::debug_print(QString body_part){
    qDebug()<<"Logging Point: "
           << "(Min:" <<retrieve_data_point(body_part).first
           << "- Max:" << retrieve_data_point(body_part).second
           << ") Average:" <<retrieve_data_point_average(body_part)
           << "- Percentage of range" << retrieve_data_point_percent(body_part) << "%";
}

