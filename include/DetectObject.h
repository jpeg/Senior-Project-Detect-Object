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
    static const int CELL_SIZE = 20;
    
private:
    enum IMAGE_CHANNELS_ENUM { HUE, LIGHTNESS, SATURATION };
    static const int IMAGE_DATA_VALUES = 3;
    enum IMAGE_DATA_VALUES { SUM, SUM_SQUARES, STANDARD_DEVIATION };
    enum IMAGE_CONFIDENCE_CUTOFF_ENUM { MINIMUM, MAXIMUM };
    static const int ROWS = IMAGE_WIDTH / CELL_SIZE;
    static const int COLUMNS = IMAGE_WIDTH / CELL_SIZE;
    static const float CONFIDENCE_LEVEL_STANDARD_DEVIATIONS = 2;
    
    // Variables
private:
    unsigned char cell[CELL_SIZE][CELL_SIZE][IMAGE_CHANNELS];
    
    int trainingHistoryLength;
    unsigned char trainingData[ROWS][COLUMNS][IMAGE_CHANNELS][IMAGE_DATA_VALUES];
    
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
    
private:
    void cellFunction(int row, int column, cv::Mat* imageHLS, int cellData[]);
};

