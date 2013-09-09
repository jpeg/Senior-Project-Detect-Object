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
    
    for(int row=0; row<ROWS; row++)
    {
        for(int column=0; column<COLUMNS; column++)
        {
            cv::Scalar cellData = this->cellFunction(row, column, &imageHLS);
            
            if(trainingHistoryLength == 0)
            {
                for(int channel=0; channel<IMAGE_CHANNELS; channel++)
                {
                    this->trainingData[row][column][channel].sum = cellData[channel];
                    this->trainingData[row][column][channel].sumSquares = (int)cellData[channel]*cellData[channel];
                }
            }
            else
            {
                for(int channel=0; channel<IMAGE_CHANNELS; channel++)
                {
                    this->trainingData[row][column][channel].sum += cellData[channel];
                    this->trainingData[row][column][channel].sumSquares += (int)cellData[channel]*cellData[channel];
                }
            }
            
            for(int channel=0; channel<IMAGE_CHANNELS; channel++)
            {
                this->trainingHistoryLength++;
                this->trainingData[row][column][channel].standardDeviation = 
                        sqrt((trainingHistoryLength * trainingData[row][column][channel].sumSquares 
                              - (long)trainingData[row][column][channel].sum * trainingData[row][column][channel].sum) 
                              / ((long)trainingHistoryLength * trainingHistoryLength));
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
    cv::Mat imageHLS;
    cv::cvtColor(image, imageHLS, CV_BGR2HLS);
    
    updateImageResults(&imageHLS);
    
    return false; //TODO, determine if imageResults meet criteria for an object detection
}

cv::Mat DetectObject::generateDebugImage(cv::Mat inputImage, IMAGE_CHANNELS_ENUM channel)
{
    cv::Mat debugImage(inputImage.size(), inputImage.type());
    cv::Mat change(cv::Size(CELL_SIZE, CELL_SIZE), inputImage.type());
    cv::Mat noChange(cv::Size(CELL_SIZE, CELL_SIZE), inputImage.type());
    change = cv::Scalar(0.0f, 0.0f, 80.0f);   // red
    noChange = cv::Scalar(0.0f, 80.0f, 0.0f); //green
    
    for(int row=0; row<ROWS; row++)
    {
        for(int column=0; column<COLUMNS; column++)
        {
            cv::Rect cellRect(column*CELL_SIZE, row*CELL_SIZE, CELL_SIZE, CELL_SIZE);
            cv::Mat cellInputImage(inputImage, cellRect);
            cv::Mat cellDebugImage(debugImage, cellRect);
            
            if(this->imageResults[row][column][channel])
            {
                cv::add(cellInputImage, change, cellDebugImage);
            }
            else
            {
                cv::add(cellInputImage, noChange, cellDebugImage);
            }
        }
    }
    
    return debugImage;
}

void DetectObject::updateImageResults(cv::Mat* imageHLS)
{
    for(int row=0; row<ROWS; row++)
    {
        for(int column=0; column<COLUMNS; column++)
        {
            cv::Scalar cellData = this->cellFunction(row, column, imageHLS);
            
            for(int channel=0; channel<IMAGE_CHANNELS; channel++)
            {
                int diffFromMean = abs((int)cellData[channel] - this->trainingData[row][column][channel].mean);
                
                if(diffFromMean > CONFIDENCE_LEVEL_STANDARD_DEVIATIONS * this->trainingData[row][column][channel].standardDeviation)
                {
                    this->imageResults[row][column][channel] = true;
                }
                else
                {
                    this->imageResults[row][column][channel] = false;
                }
            }
        }
    }
}

cv::Scalar DetectObject::cellFunction(int row, int column, cv::Mat* imageHLS)
{
    // Get just the grid cell from image
    cv::Rect cellRect(column*CELL_SIZE, row*CELL_SIZE, CELL_SIZE, CELL_SIZE);
    cv::Mat cellImage(*imageHLS, cellRect);
    
    // Average each channel for the cell
    return cv::mean(cellImage);
}

