#include <iostream>
#include <stdio.h>
#include "./filter-function-headers/meanshift.h"


struct RoiInfo;
/** 
 * Class: LinearFilter
 * A main class! Idea: for applying all flavors of linear filter functions chosen(right now there is only one filter implemented!)
 * */
class LinearFilter {
    public:
    // C-Tor 
        LinearFilter(const cv::Mat* workingCopyImg);
    // D-Tor
        ~LinearFilter();

    private:
    /**
     * virtual void filterFunction : computes the current pixel, for whome the linear filter is applied
     * 
     * @param cv::Mat* origImg
     * @param cv::Mat* workingCopy
     * @return void 
     */
        virtual void filterFunction(const cv::Mat* origImg, cv::Mat* workingCopy, const RoiInfo info); 
    
};