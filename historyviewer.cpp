#include "historyviewer.h"

//will comment later


HistoryViewer::HistoryViewer(QGroupBox* chart, QGroupBox* note_viewer)
{
    note_previous_button = note_viewer->findChild<QPushButton*>("note_previous");
    connect(note_previous_button, &QPushButton::pressed, this, &HistoryViewer::previous_note);
    note_next_button = note_viewer->findChild<QPushButton*>("note_next");
    connect(note_next_button, &QPushButton::pressed, this, &HistoryViewer::next_note);
    note_counter = note_viewer->findChild<QLabel*>("note_counter");

    num_bars = 20;
    for(int i = 1; i <= num_bars; i++){
        const QString bar_name =  QString("col%1").arg(i);
        chart_bars.append(chart->findChild<QProgressBar*>(bar_name));
    }

    note_index = 0;

    chart_selector = chart->findChild<QComboBox*>("ChartSelection");

}

void HistoryViewer::update_chart(QVector<ReadingStorage*>& data){

    if(data.size()  == 0){return;} //no readings to display

    QString body_part = chart_selector->currentText();

    QVector<int> values;


    for(int i = 0; i < data.size(); i++){
        int val;
        if(chart_selector->currentIndex() == 0){
            val = (float)data[i]->retrieve_session_average() / 162.0f * 100;


        }
        else{
            val = data[i]->retrieve_data_point_percent(body_part);
        }

        values.append(val);
    }


    int final_index = data.size() - 1;
    for(int i = 0; i < num_bars; i++){
        int data_index = final_index - i;
        int bar_value = (data_index < 0) ? 0 : values[data_index];
        chart_bars[i]->setValue(bar_value);
    }

    qDebug("chart update complete");
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


