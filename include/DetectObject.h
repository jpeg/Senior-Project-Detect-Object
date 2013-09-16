// DetectObject.h

#include <opencv2/opencv.hpp>
#include <math.h>

class DetectObject
{
    // Constants
public:
    static const int IMAGE_WIDTH = 320;
    static const int IMAGE_HEIGHT = 240;
    static const int IMAGE_CHANNELS = 3;
    enum IMAGE_CHANNELS_ENUM { HUE, LIGHTNESS, SATURATION };
    static const int CELL_SIZE = 20;
    
private:
    static const int ROWS = IMAGE_HEIGHT / CELL_SIZE;
    static const int COLUMNS = IMAGE_WIDTH / CELL_SIZE;
    static const float CONFIDENCE_LEVEL_STANDARD_DEVIATIONS = 2.0f;
    
    // Variables
private:
    int trainingHistoryLength;
    struct ImageTrainingData
    {
        // Important values
        unsigned char mean;
        float standardDeviation;
        
        // Needed for training
        unsigned int sum;
        unsigned long sumSquares;
    };
    ImageTrainingData trainingData[ROWS][COLUMNS][IMAGE_CHANNELS];
    
    bool imageResults[ROWS][COLUMNS];
    
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
    
private:
    void updateImageResults(cv::Mat* imageHLS);
    cv::Scalar cellFunction(int row, int column, cv::Mat* imageHLS);
};

