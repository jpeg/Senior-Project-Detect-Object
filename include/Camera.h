// Camera.h

#include <opencv2/opencv.hpp>

class Camera
{
private:
    cv::VideoCapture* vidCap;
    int width;
    int height;
    cv::Mat currentFrame;
    
    // Constructors
public:
    Camera();
    
    // Destructor
public:
    ~Camera();
    
    // Methods
public:
    bool init(int width, int height);
    void shutdown();
    void captureFrame();
    
    // Accessor Methods
public:
    cv::Mat getLastFrame() { return currentFrame; }
};

