// main.cpp

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "Camera.h"

int main()
{
    printf("Starting driver\n");
    system("if ! [ -e /dev/video0 ]\nthen\n uv4l --driver raspicam --auto-video_nr --width 320 --height 240 --encoding jpeg --nopreview\nfi");
    
    printf("Initializing camera\n");
    Camera* raspiCam = new Camera();
    raspiCam->init(320, 240);
    
    printf("Initializing motion detection\n");
    
    cv::namedWindow("Input Image", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Processed Image", CV_WINDOW_AUTOSIZE);
    
    while((cv::waitKey(1) & 0xFF) != 27)
    {
        raspiCam->captureFrame();
        cv::imshow("Input Image", raspiCam->getLastFrame());
    }
    
    cv::destroyWindow("Input Image");
    cv::destroyWindow("Processed Image");
    
    printf("Stopping motion detection\n");
    
    printf("Stopping camera\n");
    raspiCam->shutdown();
    delete raspiCam;
    
    return 0;
}
