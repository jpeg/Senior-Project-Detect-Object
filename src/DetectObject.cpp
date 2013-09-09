// DetectObject.cpp

#include "DetectObject.h"

DetectObject::DetectObject()
{
}

DetectObject::~DetectObject()
{
}

void DetectObject::init()
{
    this->resetTraining();
}

void DetectObject::train(cv::Mat image)
{
    cv::Mat imageHLS;
    cv::cvtColor(image, imageHLS, CV_BGR2HLS);
    
    int* cellData = new int[IMAGE_CHANNELS];
    
    for(int row=0; row<ROWS; row++)
    {
        for(int column=0; column<COLUMNS; column++)
        {
            this->cellFunction(row, column, &imageHLS, cellData);
            
            if(trainingHistoryLength == 0)
            {
                for(int channel=0; channel<IMAGE_CHANNELS; channel++)
                {
                    this->trainingData[row][column][channel][SUM] = cellData[channel];
                    this->trainingData[row][column][channel][SUM_SQUARES] = cellData[channel]*cellData[channel];
                }
            }
            else
            {
                for(int channel=0; channel<IMAGE_CHANNELS; channel++)
                {
                    this->trainingData[row][column][channel][SUM] += cellData[channel];
                    this->trainingData[row][column][channel][SUM_SQUARES] += cellData[channel]*cellData[channel];
                }
            }
            
            for(int channel=0; channel<IMAGE_CHANNELS; channel++)
            {
                this->trainingHistoryLength++;
                this->trainingData[row][column][channel][STANDARD_DEVIATION] = 
                        sqrt((trainingHistoryLength * trainingData[row][column][channel][SUM_SQUARES] 
                              - trainingData[row][column][channel][SUM] * trainingData[row][column][channel][SUM_SQUARES]) 
                              / (trainingHistoryLength * trainingHistoryLength));
            }
        }
    }
}

void DetectObject::resetTraining()
{
    this->trainingHistoryLength = 0;
}

bool DetectObject::checkObject(cv::Mat image)
{
    //TODO
    return false;
}

cv::Mat DetectObject::generateDebugImage(cv::Mat inputImage)
{
    //TODO
    return inputImage;
}

void DetectObject::cellFunction(int row, int column, cv::Mat* imageHLS, int cellData[])
{
    //TODO
}

