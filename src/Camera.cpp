// Camera.cpp

#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

bool Camera::init(int width, int height)
{
    this->width = width;
    this->height = height;
    
    this->vidCap = new cv::VideoCapture(0);
    if (!this->vidCap->isOpened())
    {
        return false;
    }
    
    this->vidCap->set(CV_CAP_PROP_FRAME_WIDTH, this->width);
    this->vidCap->set(CV_CAP_PROP_FRAME_HEIGHT, this->height);
    
    return true;
}

void Camera::shutdown()
{
    delete this->vidCap;
}

void Camera::captureFrame()
{
    this->vidCap->read(this->currentFrame);
}
