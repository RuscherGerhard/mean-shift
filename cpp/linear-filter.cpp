#include <string>
#include "../headers/linear-filter.h"
#include "../headers/roi-info-factory.h"



LinearFilter::LinearFilter(const cv::Mat* origImg){

    cv::Mat* workingCopy = new cv::Mat(origImg->cols, origImg->rows, CV_8UC3, cv::Scalar(255,255,255));
    

    RoiInfo infoFact = RoiInfoFactory(origImg->cols, origImg->rows, origImg->channels()).create(0,origImg->rows, 0, origImg->cols-1);
    filterFunction(origImg, workingCopy, infoFact);


    // make show window
    std::string windowName("Processed image ;)");
    cv::namedWindow(windowName);


    workingCopy->convertTo(*workingCopy, CV_8UC3);

    // show processed image
    cv::imshow(windowName, *workingCopy);
    std::cout << "\n< Press Key! >" << std::endl;
    cv::waitKey(0);

    // destroy helper window
    cv::destroyWindow(windowName);
}
LinearFilter::~LinearFilter(){}

void LinearFilter::filterFunction(const cv::Mat* origImg, cv::Mat* workingCopy, RoiInfo info){

    const int height = origImg->rows;
    const int width = origImg->cols;
    int cn = origImg->channels();

    std::cout << "cols : " << std::to_string(width) << " | rows : " << std::to_string(height) << " | channels : " << std::to_string(cn) << std::endl;

    long row = 0; // current row
    long count(0);
    long cols = origImg->cols;
    for(long rowIdx = 0; rowIdx < origImg->rows; rowIdx++)
    {
        // row of the current pixel
        long currentRow = rowIdx * origImg->cols * cn;

        for(long colIdx = 0; colIdx < origImg->cols; colIdx++){

            // col of the current pixel (the idx of the current pixel is currentRow + currentCol!)
            long currentCol = colIdx*cn;

            myMeanShift(origImg, workingCopy, currentRow, currentCol);

            std::cout << "\r" << std::to_string(rowIdx) << " lines out of " << std::to_string(origImg->rows)<< " processed!" << std::flush;
        }
    }
}