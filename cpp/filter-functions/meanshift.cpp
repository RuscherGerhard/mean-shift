#include "../../headers/filter-function-headers/meanshift.h"
#include <math.h>
#include <vector>
#include <tuple>


const float onePercentOfChar = 255.0f/100.f;

unsigned int flatKernel(const int x1, const int x2,const unsigned int lambda){
    // return 1 if the diff between x1 and x2 is small enough!
    return abs(x1-x2) <= lambda ? 1 : 0;
}



unsigned char applyMeanshift(
    const cv::Mat* origImg,
    cv::Mat* img,
    const u_long currentRow, 
    const u_long currentCol,
    const u_long Rad,
    const unsigned int lambda,
    const unsigned char currentColorVal,
    const u_long offset=0
    
    ){
        
        u_int cn = origImg->channels();
        u_long rowSize = cn*origImg->cols;

        int kernelSum(0);
        int kernelResult(0);
        
        // calculate init row and col for the window(we loop from the upper left to the lower right of the Filter Window, so the init position in the image will be the upper right position of the window)
        u_long initRow = (currentRow > (Rad + 1) * rowSize) ? currentRow - (Rad + 1) * rowSize : 0; 
        u_long initCol = (currentCol > (Rad + 1) * cn)      ? currentCol - (Rad + 1) * cn      : 0;

        // row loop
        for(u_long rowOS = initRow; rowOS <= currentRow + Rad * rowSize; rowOS = rowOS + rowSize){

            if(rowOS > rowSize * origImg->rows)
            {
                break;
            }

            // col loop!   
            for( u_long col = initCol; col < currentCol + (Rad * cn); col = col + 3){

                if(col > rowSize ){
                    break;
                }

                u_long idx = rowOS + col + offset;

                unsigned int kernelValue = flatKernel(origImg->at<unsigned char>(idx), currentColorVal, lambda);

                kernelResult = kernelResult + origImg->at<unsigned char>(idx) * kernelValue;

                kernelSum = kernelSum + kernelValue;
            }
        }
        
        return static_cast<unsigned char>( (float)kernelResult / (float)kernelSum);
}


// main meanshift function!
void myMeanShift(const cv::Mat* origImg, cv::Mat* img, const long currentRow, const long currentCol){
        u_long Rad = 5;
        unsigned int lambda = 20;

        unsigned char currentValueB = origImg->at<unsigned char> (currentRow + currentCol);
        unsigned char currentValueG = origImg->at<unsigned char> (currentRow + currentCol + 1);
        unsigned char currentValueR = origImg->at<unsigned char> (currentRow + currentCol + 2);
 
        // converge to the densest point in the cluster!!!()
        for(int i = 0; i < 20; i++)
        {
            currentValueB = applyMeanshift(origImg, img, currentRow, currentCol, Rad, lambda, currentValueB);
            currentValueG = applyMeanshift(origImg, img, currentRow, currentCol, Rad, lambda, currentValueG,1);
            currentValueR = applyMeanshift(origImg, img, currentRow, currentCol, Rad, lambda, currentValueR,2);


        

        img->at<unsigned char>(currentRow + currentCol)   = currentValueB;
        img->at<unsigned char>(currentRow + currentCol+1) = currentValueG;
        img->at<unsigned char>(currentRow + currentCol+2) = currentValueR;        
        }
}