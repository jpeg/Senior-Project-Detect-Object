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
    //TODO
}

void DetectObject::resetTraining()
{
    //TODO
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

