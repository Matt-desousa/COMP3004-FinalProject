#ifndef HISTORYVIEWER_H
#define HISTORYVIEWER_H

#include <QProgressBar>
#include <QPushButton>
#include <QGroupBox>
#include <QVector>
#include <QLabel>
#include <QDebug>

#include "defs.h"
#include "readingstorage.h"
#include "profile.h"

class HistoryViewer : public QObject
{
    Q_OBJECT
public:
    explicit HistoryViewer(QGroupBox* chart, QGroupBox* note_viewer);

    void update_chart(QVector<ReadingStorage*>& data);

    Note* get_note(Profile* user);


    void next_note();
    void previous_note();

private:
    int num_bars;
    QVector<QProgressBar*> chart_bars;
    QPushButton* note_previous_button;
    QPushButton* note_next_button;
    QLabel* note_counter;

    int note_index;

};

#endif // HISTORYVIEWER_H
