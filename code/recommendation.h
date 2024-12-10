#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H
#include "readingstorage.h"
#include <QString>
#include <QQueue>
#include <QPair>
class Recommendation
{
public:
    Recommendation();
    ~Recommendation();
    void AddAbnormalPartinQ(QString left, QString Right,QMap<QString,int>& data);
    int GetAverage(QMap<QString,int>& data);
    int GetWrong_partSize();
    QString GetWrong_part();
    int Getresult();
    QString Reorganize(QString left, QString right);

private:
    int part_average;
    int higher;
    int lower;
    QQueue<QString> wrong_part;
    QQueue<int> result;


};

#endif // RECOMMENDATION_H
