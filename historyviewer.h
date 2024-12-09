#ifndef HISTORYVIEWER_H
#define HISTORYVIEWER_H

#include <QProgressBar>
#include <QPushButton>
#include <QGroupBox>
#include <QVector>
#include <QLabel>
#include <QDebug>
#include <QComboBox>

#include "defs.h"
#include "readingstorage.h"
#include "profile.h"

class HistoryViewer : public QObject
{
    Q_OBJECT
public:
    explicit HistoryViewer(QGroupBox* chart, QGroupBox* note_viewer);

    void update_chart(QVector<ReadingStorage*>& data); //draw chart

    Note* get_note(Profile* user);


    void next_note();
    void previous_note();
    inline void reset(){note_index = 0;}

private:
    int num_bars;
    QVector<QProgressBar*> chart_bars;
    QPushButton* note_previous_button;
    QPushButton* note_next_button;
    QLabel* note_counter;
    QComboBox* chart_selector;
    QLabel* chart_avg;
    QLabel* chart_max;
    QLabel* chart_min;

    int note_index; //keeps track of which note the viewer is looking at

};

#endif // HISTORYVIEWER_H
