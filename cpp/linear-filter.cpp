#include <string>
#include "../headers/linear-filter.h"
#include "../headers/roi-info-factory.h"
#include <thread>
#include <vector>



LinearFilter::LinearFilter(const cv::Mat* origImg):processedLines(0){

    cv::Mat* workingCopy = new cv::Mat(origImg->rows, origImg->cols, CV_8UC3, cv::Scalar(255,255,255));

    // kickoff the threads to process the image!
    size_t threadAmmount = 32;
    filterThreadsManager(origImg, workingCopy, threadAmmount);
 
    // // make show window
    std::string windowName("Processed image ;)");
    cv::namedWindow(windowName);

    // show processed image
    cv::imshow(windowName, *workingCopy);
    std::cout << "\n\n< Press Key! >" << std::endl;
    cv::waitKey(0);

    // destroy helper window
    cv::destroyWindow(windowName);
}
LinearFilter::~LinearFilter(){}

void LinearFilter::filterFunction(const cv::Mat* origImg, cv::Mat* workingCopy, RoiInfo info){

    const int height = info.rows;
    const int width = info.cols;
    int cn = info.cn;

    // std::cout << "cols : " << std::to_string(width) << " | rows : " << std::to_string(height) << " | channels : " << std::to_string(cn) << std::endl;
    // std::cout << "start row : " << std::to_string(info.start_row) << " | end_row : " << std::to_string(info.end_row) << " | channels : " << std::to_string(cn) << std::endl;

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

            myMeanShift(origImg, workingCopy, currentRow, currentCol);

            // std::cout << "\r" << std::to_string(rowIdx) << " lines out of " << std::to_string(origImg->rows)<< " processed!" << std::flush;
        }
        processedLines++;
    }
}

void LinearFilter::filterThreadsManager(const cv::Mat* origImg, cv::Mat* workingCopy, const size_t ammountThreads){
    
    RoiInfoFactory fact = RoiInfoFactory(origImg->cols, origImg->rows, origImg->channels());

    std::vector<std::thread*>threadVector(ammountThreads, NULL);

    unsigned long rows = origImg->rows;

    unsigned long modulo = rows % (unsigned long)ammountThreads;
    unsigned long step = (unsigned long)( (rows - modulo) / ammountThreads );

    unsigned long currentLine = 0;
    unsigned long lastLine = step;

    for(size_t i = 0; i < ammountThreads; i++){

        RoiInfo info = fact.create(currentLine, lastLine, 0, origImg->cols-1);
        if(info.bad)
        {
            // if the info is bad, we are done!
            break;
        }

        std::thread* t = new std::thread( [this, origImg, workingCopy, info](){
            this->filterFunction(origImg, workingCopy, info); 
        });

        threadVector[i] = t;
        
        currentLine = currentLine + step;
        lastLine = currentLine + step <= rows ? currentLine + step : currentLine + modulo;
    }

    // std::cout << " ++++++++++++ thread vector size " << std::to_string(threadVector.size()) << " step size " << std::to_string(step)<< std::endl;
    // std::cout << "cols : " << std::to_string(origImg->cols) << " | rows : " << std::to_string(rows) << " | channels : " << origImg->channels() << std::endl;
    // while(processedLines < origImg->rows){

    //     unsigned long prol = processedLines;
    //     int cnt = 0;
    //     for(int i = 0; i < 500; i++)
    //         cnt++;
        
    //     std::cout << "\r { " << std::to_string(prol) << " } lines out of " << std::to_string(rows)<< " processed!" << " pause for : " << std::to_string(cnt) << std::flush;
    // }

    for(size_t i = 0; i < ammountThreads; i++){
        if(threadVector[i] != NULL)
            threadVector[i]->join();
    }
}