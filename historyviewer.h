#ifndef HISTORYVIEWER_H
#define HISTORYVIEWER_H

#include "readingstorage.h"
#include <QProgressBar>
#include <QGroupBox>
#include <QVector>
#include <QDebug>
#include "defs.h"

class HistoryViewer : public QObject
{
    Q_OBJECT
public:
    explicit HistoryViewer(QGroupBox* chart);

    void update_chart(QVector<ReadingStorage*>& data);

private:
    int num_bars;
    QVector<QProgressBar*> chart_bars;

};

#endif // HISTORYVIEWER_H
