#include <string>
#include "../headers/linear-filter.h"
#include "../headers/roi-info-factory.h"
#include <thread>



LinearFilter::LinearFilter(const cv::Mat* origImg){

    cv::Mat* workingCopy = new cv::Mat(origImg->cols, origImg->rows, CV_8UC3, cv::Scalar(255,255,255));
    
    unsigned long end_row2 = origImg->rows;
    unsigned long end_col2 = origImg->cols-1;
    unsigned long start_row12 = (unsigned long)(origImg->rows/2.0f);
    unsigned long start_col2 = (unsigned long)(origImg->cols/2.0f);

    RoiInfo infoFractOne = RoiInfoFactory(origImg->cols, origImg->rows, origImg->channels()).create(0,start_row12, 0, end_col2);
    RoiInfo infoFractTwo = RoiInfoFactory(origImg->cols, origImg->rows, origImg->channels()).create(start_row12,origImg->rows, 0, end_col2);

    std::thread first( [this, origImg, workingCopy, infoFractOne](){
        this->filterFunction(origImg, workingCopy, infoFractOne,0); 
    });
    std::thread second( [this,origImg, workingCopy, infoFractTwo](){
        this->filterFunction( origImg, workingCopy, infoFractTwo,10); 
    });


    // TODO while loop, which checks the value of the processed lines, and when the lines have hit the max lines count int the pic, join all threads!

    first.join();
    second.join();
    // make show window
    std::string windowName("Processed image ;)");
    cv::namedWindow(windowName);


    // workingCopy->convertTo(*workingCopy, CV_8UC3);

    // show processed image
    cv::imshow(windowName, *workingCopy);
    std::cout << "\n\n< Press Key! >" << std::endl;
    cv::waitKey(0);

    // destroy helper window
    cv::destroyWindow(windowName);
}
LinearFilter::~LinearFilter(){}

void LinearFilter::filterFunction(const cv::Mat* origImg, cv::Mat* workingCopy, RoiInfo info, const unsigned char offset){

    const int height = info.rows;
    const int width = info.cols;
    int cn = info.cn;

    std::cout << "cols : " << std::to_string(width) << " | rows : " << std::to_string(height) << " | channels : " << std::to_string(cn) << std::endl;

    long row = 0; // current row
    long count(0);
    long cols = origImg->cols;
    for(long rowIdx = info.start_row; rowIdx < info.end_row; rowIdx++)
    {
        // row of the current pixel
        long currentRow = rowIdx * width * cn;

        for(long colIdx = info.start_col; colIdx < info.end_col; colIdx++){

            // col of the current pixel (the idx of the current pixel is currentRow + currentCol!)
            long currentCol = colIdx*cn;

            myMeanShift(origImg, workingCopy, currentRow, currentCol, offset);

            // std::cout << "\r" << std::to_string(rowIdx) << " lines out of " << std::to_string(origImg->rows)<< " processed!" << std::flush;
        }
    }
}