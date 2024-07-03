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
    unsigned long processedLines; ///< ammount of lines, processed by the filter!
    /**
     * void filterFunction : computes the current pixel, for whome the linear filter is applied
     * 
     * @param cv::Mat* origImg
     * @param cv::Mat* workingCopy
     * @return void 
     */
    void filterFunction(const cv::Mat* origImg, cv::Mat* workingCopy, const RoiInfo info); 

    /**
     * void fiiterThreadsManager : should call filterFunciton threaded and manage those spawned threads!
     * @param const cv::Mat* origImg is a pointer to the original image
     * @param const cv::Mat* workingCopy is a pointer to the target where the processed values should be written!
     */
    void filterThreadsManager(const cv::Mat* origImg, cv::Mat* workingCopy, const size_t ammountThreads);
    
};