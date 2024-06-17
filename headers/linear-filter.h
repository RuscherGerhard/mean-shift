#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

class LinearFilter {
    public: 
        LinearFilter(cv::Mat workingCopyImg);
        ~LinearFilter();

        virtual void filterFunction(cv::Mat* img); 
    
};