#ifndef READINGSTORAGE_H
#define READINGSTORAGE_H

#include "note.h"

#include <QObject>
#include <QMap>
#include <QPair>
#include <QDebug>
#include <QRandomGenerator>

class ReadingStorage : public QObject
{
    Q_OBJECT
public:
    explicit ReadingStorage(QMap<QString,QPair<int,int>>* body_parts);
    ~ReadingStorage();

    void log_data_point(QString body_part, int reading);
    int  retrieve_data_point(QString body_part);
    int  retrieve_data_point_percent(QString body_part); //percent between min and max
    QPair<int,int>  retrieve_data_point_range(QString body_part); //get min and max
    int  retrieve_session_average(); //overall average of 24 points

    inline Note* get_note(){return note;}
    inline QMap<QString,int>* get_readings(){return &readings;}

    void debug_print(QString body_part); //debug print a point
    void debug_populate_logs(); //fill storage with random data

private:
    QMap<QString,int> readings; //all readings (bodypart:value)
    Note* note;
    QMap<QString,QPair<int,int>>* body_parts_info; //used to reference body part names and recommended ranges
};

#endif // READINGSTORAGE_H
