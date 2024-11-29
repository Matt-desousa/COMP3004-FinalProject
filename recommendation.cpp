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
void Recommendation::AddAbnormalPartinQ(QString left,QString right, ReadingStorage& storage)
{
    QString temp = Reorganize(left,right);
    int average = GetAverage(storage);
    int leftaverage = storage.retrieve_data_point_average(left);
    int rightaverage = storage.retrieve_data_point_average(right);

    //if part_average is higher than the average zone
    if(leftaverage > average * 1.2 || rightaverage > average * 1.2 )
    {
        //store body part name in queue
        wrong_part.enqueue(temp);
        //store higher
        result.enqueue(higher);
    //if part_average is lower than the average zone
    }else if(leftaverage < average * 0.8 || rightaverage < average * 0.8 )
    {
        //store body part name in queue
        wrong_part.enqueue(temp);
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

//Reorganize left and right
QString Recommendation::Reorganize(QString left, QString right)
{
    if(left == "H1 Left" && right == "H1 Right")
    {
        return "H1";
    }else if (left == "H2 Left" && right == "H2 Right")
    {
        return "H2";
    }else if(left == "H3 Left" && right == "H3 Right")
    {
        return "H3";
    }else if(left == "H4 Left" && right == "H4 Right")
    {
        return "H4";
    }else if(left == "H5 Left" && right == "H5 Right")
    {
        return "H5";
    }else if(left == "H6 Left" && right == "H6 Right"){
        return "H6";
    }else if(left == "F1 Left" && right == "F1 Right"){
        return "F1";
    }else if(left == "F2 Left" && right == "F2 Right")
    {
        return "F2";
    }else if(left == "F3 Left" && right == "F3 Right")
    {
        return "F3";
    }else if(left == "F4 Left" && right == "F4 Right")
    {
        return "F4";
    }else if(left == "F5 Left" && right == "F5 Right")
    {
        return "F5";
    }else if(left == "F6 Left" && right == "F6 Right")
    {
        return "F6";
    }else{
        return "ERROR";
    }
}
