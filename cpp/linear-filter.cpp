#include <string>
#include "../headers/linear-filter.h"


LinearFilter::LinearFilter(cv::Mat workingCopyImg){

    std::cout << "Constructed Filter!!!!" << std::endl;

    filterFunction(&workingCopyImg);

    std::string windowName("FilterWindow ;)");

    std::cout << std::to_string(workingCopyImg.size().width)<< "/" << std::to_string(workingCopyImg.size().height) << std::endl;

    // cv::namedWindow(windowName);
    // cv::imshow(windowName, workingCopyImg);

    // cv::waitKey(0);

    // cv::destroyWindow(windowName);
}
LinearFilter::~LinearFilter(){}

void LinearFilter::filterFunction(cv::Mat* img){
    //TODO implement the filters!!!!
    std::cout<<"fiterFunction Called" << std::endl;
    const int height = img->rows;
    const int width = img->cols;

    int cn = img->channels();
    
    long row = 0; // current row
    for(u_long rowIdx = 0; rowIdx < img->rows; rowIdx++)
    {

        u_long currentRow = rowIdx*img->cols * cn;
        for(u_long colIdx = 0; colIdx < img->cols; colIdx++){
            u_long currentCol = colIdx * cn;
            img->data[currentRow + currentCol+1] = 0;
            img->data[currentRow + currentCol+2] = 0;
            img->data[currentRow + currentCol+3] = 0; 
        }
    }
}