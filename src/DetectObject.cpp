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
    
    this->trainingHistoryLength++;
    
    for(int row=0; row<ROWS; row++)
    {
        for(int column=0; column<COLUMNS; column++)
        {
            cv::Scalar cellData = this->cellFunction(row, column, &imageHLS, &image);
            
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
                this->trainingData[row][column][channel].mean = this->trainingData[row][column][channel].sum / trainingHistoryLength;
                this->trainingData[row][column][channel].standardDeviation = 
                        sqrt((trainingHistoryLength * trainingData[row][column][channel].sumSquares 
                              - (long)trainingData[row][column][channel].sum * trainingData[row][column][channel].sum) 
                              / (float)((long)trainingHistoryLength * trainingHistoryLength));
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
    
    updateImageResults(&imageHLS, &image);
    
    return false; //TODO, determine if imageChannelResults meet criteria for an object detection
}

cv::Mat DetectObject::generateDebugImage(cv::Mat inputImage)
{
    cv::Mat debugImage(inputImage.size(), inputImage.type());
    cv::Mat change(cv::Size(CELL_SIZE, CELL_SIZE), inputImage.type());
    cv::Mat noChange(cv::Size(CELL_SIZE, CELL_SIZE), inputImage.type());
    change = cv::Scalar(0.0f, 0.0f, 80.0f);   // red
    noChange = cv::Scalar(0.0f, 80.0f, 0.0f); // green
    
    for(int row=0; row<ROWS; row++)
    {
        for(int column=0; column<COLUMNS; column++)
        {
            cv::Rect cellRect(column*CELL_SIZE, row*CELL_SIZE, CELL_SIZE, CELL_SIZE);
            cv::Mat cellInputImage(inputImage, cellRect);
            cv::Mat cellDebugImage(debugImage, cellRect);
            
            if(this->imageResults[row][column])
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

void DetectObject::updateImageResults(cv::Mat* imageHLS, cv::Mat* imageBGR)
{
    for(int row=0; row<ROWS; row++)
    {
        for(int column=0; column<COLUMNS; column++)
        {
            cv::Scalar cellData = this->cellFunction(row, column, imageHLS, imageBGR);
            
            int diffFromMean = abs((int)cellData[HUE] - this->trainingData[row][column][HUE].mean);
            if(diffFromMean > 128)
            {
                diffFromMean = 256 - diffFromMean;
            }
            bool hue = (diffFromMean > CONFIDENCE_LEVEL_STANDARD_DEVIATIONS_HUE
                        * this->trainingData[row][column][HUE].standardDeviation);
            
            diffFromMean = abs((int)cellData[LIGHTNESS] - this->trainingData[row][column][LIGHTNESS].mean);
            bool lightness = (diffFromMean > CONFIDENCE_LEVEL_STANDARD_DEVIATIONS_LIGHTNESS
                        * this->trainingData[row][column][LIGHTNESS].standardDeviation);
            
            diffFromMean = abs((int)cellData[SATURATION] - this->trainingData[row][column][SATURATION].mean);
            bool saturation = (diffFromMean > CONFIDENCE_LEVEL_STANDARD_DEVIATIONS_SATURATION
                        * this->trainingData[row][column][SATURATION].standardDeviation);
            
            this->imageResults[row][column] = (hue || (saturation && lightness));
        }
    }
}

cv::Scalar DetectObject::cellFunction(int row, int column, cv::Mat* imageHLS, cv::Mat* imageBGR)
{
    // Get just the grid cell from image
    cv::Rect cellRect(column*CELL_SIZE, row*CELL_SIZE, CELL_SIZE, CELL_SIZE);
    cv::Mat cellImageHLS(*imageHLS, cellRect);
    cv::Mat cellImageBGR(*imageBGR, cellRect);
    
    // Average each channel for the cell
    cv::Scalar avg = cv::mean(cellImageHLS);
    cv::Scalar avgBGR = cv::mean(cellImageBGR);
    
    // Find hue average from RBG image
    cv::Mat pixel(1,1, imageBGR->type(), avgBGR);
    cv::cvtColor(pixel, pixel, CV_BGR2HLS);
    avg[HUE] = pixel.data[0];
    
    return avg;
}

