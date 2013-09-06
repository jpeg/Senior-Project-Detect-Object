// DetectObject.h

#include <opencv2/opencv.hpp>

class DetectObject
{
    // Constants
public:
    static const int IMAGE_WIDTH = 320;
    static const int IMAGE_HEIGHT = 240;
    static const int IMAGE_CHANNELS = 3;
    static const int CELL_SIZE = 20;
    
private:
    enum IMAGE_CHANNELS_ENUM { HUE, LIGHTNESS, SATURATION };
    enum IMAGE_CONFIDENCE_CUTOFF_ENUM { MINIMUM, MAXIMUM };
    static const int ROWS = IMAGE_WIDTH / CELL_SIZE;
    static const int COLUMNS = IMAGE_WIDTH / CELL_SIZE;
    static const float CONFIDENCE_LEVEL = 0.95;
    
    // Variables
private:
    unsigned char cell[CELL_SIZE][CELL_SIZE];
    
    int trainingHistoryLength;
    unsigned char trainingConfidenceCutoffs[ROWS][COLUMNS][IMAGE_CHANNELS][2];
    
    unsigned char lastImageResults[ROWS][COLUMNS][IMAGE_CHANNELS];
    
    // Constructors
public:
    DetectObject();
    
    // Destructor
public:
    ~DetectObject();
    
    // Methods
public:
    void init();
    void train(cv::Mat image);
    void resetTraining();
    bool checkObject(cv::Mat image);
    cv::Mat generateDebugImage(cv::Mat inputImage);
};

