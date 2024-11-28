#include "recommendation.h"

Recommendation::Recommendation()
{
    part_average = 0;
    higher = 1;
    lower = -1;

}

Recommendation::~Recommendation()
{

}

int Recommendation::GetAverage(ReadingStorage& storage)
{
    int average = 0;
    int sum = 0;
    sum = storage.retrieve_data_point_average("H1 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H1 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H2 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H2 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H3 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H3 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H4 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H4 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H5 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H5 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H6 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("H6 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F1 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F1 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F2 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F2 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F3 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F3 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F4 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F4 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F5 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F5 Left");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F6 Right");
    average = average + sum;
    sum = storage.retrieve_data_point_average("F6 Left");
    average = average + sum;
    return average/24;



}
//add abnormal part in body_part & higher or lower
void Recommendation::AddAbnormalPartinQ(QString body_part, ReadingStorage& storage)
{
    part_average = storage.retrieve_data_point_average(body_part);
    int average = GetAverage(storage);

    //if part_average is higher than the average zone
    if(part_average > average * 1.2 )
    {
        //store body part name in queue
        wrong_part.enqueue(body_part);
        //store higher
        result.enqueue(higher);
    //if part_average is lower than the average zone
    }else if(part_average < average * 0.8 )
    {
        //store body part name in queue
        wrong_part.enqueue(body_part);
        //store lower
        result.enqueue(lower);
    }
}

//Get Wrong_part size
int Recommendation::GetWrong_partSize()
{
    return wrong_part.size();
}

//Get First element of worng_part
QString Recommendation::GetWrong_part()
{
    return wrong_part.dequeue();
}

//Get First element of result

int Recommendation::Getresult()
{
    return result.dequeue();
}
