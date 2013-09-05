// main.cpp

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "Camera.h"

int main()
{
    printf("Initializing camera\n");
    Camera* raspiCam = new Camera();
    raspiCam->init(320, 240);
    
    printf("Initializing motion detection\n");
    
    cv::namedWindow("Input Image", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Processed Image", CV_WINDOW_AUTOSIZE);
    
    printf("Running...\n");
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

