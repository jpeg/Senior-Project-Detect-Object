// Camera.h

#include <opencv2/opencv.hpp>
#include <ctime>

class Camera
{
private:
    cv::VideoCapture* vidCap;
    int width;
    int height;
    cv::Mat currentFrame;
    std::clock_t currentFrameTime;
    std::clock_t lastFrameTime;
    
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
    bool captureFrame();
    float getFPS();
    
    // Accessor Methods
public:
    cv::Mat getLastFrame() { return currentFrame; }
};

