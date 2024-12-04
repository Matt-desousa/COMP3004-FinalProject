#include "historyviewer.h"

//will comment later


HistoryViewer::HistoryViewer(QGroupBox* chart, QGroupBox* note_viewer)
{
    note_previous_button = note_viewer->findChild<QPushButton*>("note_previous");
    connect(note_previous_button, &QPushButton::pressed, this, &HistoryViewer::previous_note);
    note_next_button = note_viewer->findChild<QPushButton*>("note_next");
    connect(note_next_button, &QPushButton::pressed, this, &HistoryViewer::next_note);
    note_counter = note_viewer->findChild<QLabel*>("note_counter");

    num_bars = chart->children().size();
    for(int i = 1; i <= num_bars; i++){
        const QString bar_name =  QString("col%1").arg(i);
        chart_bars.append(chart->findChild<QProgressBar*>(bar_name));
    }

    note_index = 0;
}

void HistoryViewer::update_chart(QVector<ReadingStorage*>& data){

    if(data.size()  == 0){return;}

    QVector<int> values;
    int max_val = -10000;
    int min_val = 10000;

    for(int i = 0; i < data.size(); i++){

        int val = data[i]->retrieve_session_average();

        if(val < min_val){min_val = val;}
        if(val > max_val){max_val = val;}
        values.append(val);
    }

    max_val += (max_val / 10); //gap on top of graph

    int final_index = data.size() - 1;
    for(int i = 0; i < num_bars; i++){
        int data_index = final_index - i;
        int bar_value = (data_index < 0) ? 0 : ((float)values[data_index] / max_val  * 100);
        chart_bars[i]->setValue(bar_value);
    }
}

Note* HistoryViewer::get_note(Profile* user){

        int total_note_count = user->getSessions()->size();
        if(note_index >= total_note_count){note_index = total_note_count - 1;}
        qDebug() << QString("getting ref to note %1").arg(note_index + 1);
        int index = total_note_count - 1  - note_index;
        note_counter->setText(QString("%1").arg(index + 1));
        Note* n = user->getSessions()->at(index)->get_note();
        return n;

}

void HistoryViewer::next_note(){
    if(note_index == 0){return;}
    note_index--;
    qDebug() << QString("Note %1").arg(note_index);
    //display_note();
}

void HistoryViewer::previous_note(){
    //if(note_index == 0){return;}
    note_index++;
    qDebug() << QString("Note %1").arg(note_index);
    //display_note();
}


