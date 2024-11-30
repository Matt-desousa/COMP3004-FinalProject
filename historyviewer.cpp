#include "historyviewer.h"

//will comment later


HistoryViewer::HistoryViewer(QGroupBox* chart)
{
    num_bars = chart->children().size();
    for(int i = 1; i <= num_bars; i++){
        const QString bar_name =  QString("col%1").arg(i);
        chart_bars.append(chart->findChild<QProgressBar*>(bar_name));
    }
}

void HistoryViewer::update_chart(QVector<ReadingStorage*>& data){

    int max_val = 300;
    int min_val = 0;

    int final_index = data.size() - 1;
    for(int i = 0; i < num_bars; i++){
        int data_index = final_index - i;
        int bar_value = (data_index < 0) ? 0 : ((float)data[data_index]->retrieve_session_average() / max_val  * 100);
        chart_bars[i]->setValue(bar_value);
    }
}
