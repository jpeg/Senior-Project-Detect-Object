// Camera.cpp

#include "Camera.h"

Camera::Camera()
{
#if defined(__linux__) && defined(__arm__)
    // Start raspicam V4L driver if there's no video device
    system("if ! [ -e /dev/video0 ]\nthen\n uv4l --driver raspicam --auto-video_nr --width 320 --height 240 --encoding jpeg --nopreview\nfi");
#endif
}

Camera::~Camera()
{
}

bool Camera::init(int width, int height)
{
    this->width = width;
    this->height = height;
    
    this->vidCap = new cv::VideoCapture(-1);
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

bool Camera::captureFrame()
{
    return this->vidCap->read(this->currentFrame);
}

