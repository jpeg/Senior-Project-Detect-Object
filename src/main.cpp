// main.cpp

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "Camera.h"
#include "DetectObject.h"

int main()
{
    printf("Initializing camera\n");
    Camera* raspiCam = new Camera();
    raspiCam->init(DetectObject::IMAGE_WIDTH, DetectObject::IMAGE_HEIGHT);
    
    printf("Initializing motion detection\n");
    DetectObject* detectObject = new DetectObject();
    detectObject->init();
    
    cv::namedWindow("Input Image", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Debug", CV_WINDOW_AUTOSIZE);
    
    const int trainingFrames = 50;
    printf("Training... (%i frames)\n", trainingFrames);
    for(int i=0; i<trainingFrames; i++)
    {
        raspiCam->captureFrame();
        detectObject->train(raspiCam->getLastFrame());
        
        cv::imshow("Input Image", raspiCam->getLastFrame());
    }
    
    printf("Running...\n");
    while((cv::waitKey(1) & 0xFF) != 27)
    {
        raspiCam->captureFrame();
        detectObject->checkObject(raspiCam->getLastFrame());
        
        cv::imshow("Input Image", raspiCam->getLastFrame());
        cv::imshow("Debug", detectObject->generateDebugImage(raspiCam->getLastFrame()));
        printf("FPS: %f\n", raspiCam->getFPS());
    }
    
    cv::destroyWindow("Input Image");
    cv::destroyWindow("Debug");
    
    printf("Stopping motion detection\n");
    delete detectObject;
    
    printf("Stopping camera\n");
    raspiCam->shutdown();
    delete raspiCam;
    
    return 0;
}

