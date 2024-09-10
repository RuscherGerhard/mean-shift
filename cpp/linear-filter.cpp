#include <string>
#include "../headers/linear-filter.h"
#include "../headers/roi-info-factory.h"
#include <thread>
#include <vector>

// TODO fix index bug at image margin
// TODO build engine and readme instructions for usage and building!


LinearFilter::LinearFilter(const cv::Mat* origImg, const size_t threadAmmount):processedLines(0){

    cv::Mat* workingCopy = new cv::Mat(origImg->rows, origImg->cols, CV_8UC3, cv::Scalar(0,255,0));

    // kickoff the threads to process the image!
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

    long row = 0; // current row
    long count(0);
    long cols = origImg->cols;

    // cv::Mat* scndWorkingCopy = new cv::Mat() 
    for(long rowIdx = info.start_row; rowIdx <= info.end_row; rowIdx++)
    {
        processedLines++;
        // row of the current pixel
        long currentRow = rowIdx * width * cn;

        for(long colIdx = info.start_col; colIdx <= info.end_col; colIdx++){

            // col of the current pixel (the idx of the current pixel is currentRow + currentCol!)
            long currentCol = colIdx*cn;

            myMeanShift(origImg, workingCopy, currentRow, currentCol);
        }        
    }
}

void LinearFilter::filterThreadsManager(const cv::Mat* origImg, cv::Mat* workingCopy, const size_t ammountThreads){
    
    // initi the info factory
    RoiInfoFactory fact = RoiInfoFactory(origImg->cols, origImg->rows, origImg->channels());
    // init a vector to thread pointers
    std::vector<std::thread*>threadVector(ammountThreads, NULL);

    // calculate the row steps to determine the image partitions, each thread is working on!
    unsigned long rows = origImg->rows;

    unsigned long modulo = rows % (unsigned long)ammountThreads;
    unsigned long step = (unsigned long)( (rows - modulo) / ammountThreads ); // the usual amount of lines for one thread

    unsigned long currentLine = 0;
    unsigned long lastLine = step;

    size_t lastThreadIdx = ammountThreads - 2;

    //  init the threads!
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
        
        currentLine = currentLine + step + 1;

        if(i == lastThreadIdx){
            // this is calculating the ammount of rows for the last thread
            lastLine = origImg->rows; // TODO  FIX to many lines in last thread!!!!! 
        } else {
            lastLine = currentLine + step;
        }
    }

    while(processedLines < workingCopy->rows-1){

        unsigned long prol = processedLines;
        int cnt = 0;
        for(int i = 0; i < 500; i++)
            cnt++;
        
        std::cout << "\r { " << std::to_string(prol) << " } lines out of " << std::to_string(rows)<< " processed!" << std::flush;
    }

    for(size_t i = 0; i < ammountThreads; i++){
        if(threadVector[i] != NULL)
            threadVector[i]->join();
    }
}