#ifndef READINGSTORAGE_H
#define READINGSTORAGE_H

#include "note.h"

#include <QObject>
#include <QMap>
#include <QPair>
#include <QDebug>

class ReadingStorage : public QObject
{
    Q_OBJECT
public:
    explicit ReadingStorage(QMap<QString,QPair<int,int>>* body_parts);

    void log_data_point(QString body_part, QPair<int,int> reading);
    QPair<int,int>  retrieve_data_point(QString body_part);
    int  retrieve_data_point_average(QString body_part);
    int  retrieve_data_point_percent(QString body_part);
    int  retrieve_session_average();

private:
    QMap<QString,QPair<int,int>> readings; //all readings (bodypart:minmax)
    Note* note;
    QMap<QString,QPair<int,int>>* body_parts_info; //used to reference body part names and recommended ranges

    void debug_print(QString body_part); //debug print a point





};

#endif // READINGSTORAGE_H
