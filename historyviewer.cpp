#include "historyviewer.h"

//will comment later


HistoryViewer::HistoryViewer(QGroupBox* chart, QGroupBox* note_viewer)
{
    //store and connect note browser buttons
    note_previous_button = note_viewer->findChild<QPushButton*>("note_previous");
    connect(note_previous_button, &QPushButton::pressed, this, &HistoryViewer::previous_note);
    note_next_button = note_viewer->findChild<QPushButton*>("note_next");
    connect(note_next_button, &QPushButton::pressed, this, &HistoryViewer::next_note);
    note_counter = note_viewer->findChild<QLabel*>("note_counter");

    //find and store chart bars in array
    num_bars = 20;
    for(int i = 1; i <= num_bars; i++){
        const QString bar_name =  QString("col%1").arg(i);
        chart_bars.append(chart->findChild<QProgressBar*>(bar_name));
        chart_bars.back()->setToolTip("ERROR");
    }

    note_index = 0;

    //find and store chart labels and selector
    chart_selector = chart->findChild<QComboBox*>("ChartSelection");
    chart_avg = chart->findChild<QLabel*>("label_avg");
    chart_max = chart->findChild<QLabel*>("label_max");
    chart_min = chart->findChild<QLabel*>("label_min");
}

//draw the requested data to the chart
void HistoryViewer::update_chart(QVector<ReadingStorage*>& data){
    if(data.size()  == 0){return;} //no readings to display

    QString body_part = chart_selector->currentText(); //get what is in the combobox
    bool is_average_chart_selected = chart_selector->currentIndex() == 0; //index 0 is always "average" instead of a specific body part
    //"average" refers to the average value of all 24 points

    //data min and max to set bars' scale
    int min;
    int max;

    if(is_average_chart_selected){ //set hard coded max and min of session average
        min = 5;
        max = 162;
    }
    else{
        QPair<int,int> minmax = data[0]->retrieve_data_point_range(body_part); //query arbitrary session to get body part range
        min = minmax.first;
        max = minmax.second;
    }

    QVector<int> values; //temp buffer to store retrieved values

    //COLLECT DATA
    for(int i = 0; i < data.size(); i++){
        int val;

        //if the user wants the total average
        if(is_average_chart_selected){
            val = data[i]->retrieve_session_average();
        }
        //if user wants a specific body part
        else{
            val = data[i]->retrieve_data_point(body_part);
        }
        values.append(val);
    }


    //DRAW BARS
    int chart_sum = 0;
    int num_used_bars = 0;
    for(int i = 0; i < num_bars; i++){
        int bar_value;
        if(i >= values.size()){ //no more data (empty bar)
            bar_value = min; //set to bottom  (no bar showing)
            chart_bars[i]->setToolTip("No Data");
            chart_bars[i]->setEnabled(false); //grey out
        }
         else{ //draw a bar
            bar_value = values[i];
            chart_bars[i]->setToolTip(QString("%1").arg(bar_value)); //set hover text
            chart_bars[i]->setEnabled(true);
            num_used_bars++;
            chart_sum += bar_value;
        }

        if(max - min == 0){return;} //won't ever happen but stops compiler x/0 warning on next line
        int bar_percent = ((bar_value - min) * 100) / (max - min); //get percent
        chart_bars[i]->setValue(bar_percent); //draw bar
    }

    chart_max->setText(QString("%1").arg(max));
    chart_min->setText(QString("%1").arg(min));
    if(num_used_bars == 0){return;} //won't ever happen but stops compiler x/0 warning on next line
    chart_avg->setText(QString("%1").arg(chart_sum / num_used_bars));

}

//retrieve a note from a user
Note* HistoryViewer::get_note(Profile* user){
        int total_note_count = user->getSessions()->size();
        if(note_index >= total_note_count){note_index = total_note_count - 1;} //cap index to avoid IOOB
        note_counter->setText(QString("%1").arg(total_note_count - note_index)); //update note counter label (reverse order from indexes)
        Note* n = user->getSessions()->at(note_index)->get_note();
        return n;
}

void HistoryViewer::next_note(){
    if(note_index == 0){return;}
    note_index--;
}

void HistoryViewer::previous_note(){
    note_index++; //overflow is handled elsewhere
}


