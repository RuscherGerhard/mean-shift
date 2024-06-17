#include "../headers/linear-filter.h"

enum Errors {
    ERROR_SUCCESS,
    NO_IMG_PATH,
    IMG_NOT_FOUND
} Errors_T;

int throwError(const std::string err_str, const Errors err_code){
    // print error message
    std::cout << err_str.c_str() << std::endl;
    std::cout << "<< press key! >>" << std::endl;

    // wait for user interaction
    std::cin.get();

    // return error code
    return static_cast<int>( err_code);
}

int main(int argc, char* argv[]){
    // check if we have argument
    if(argc <= 0)
        return throwError(" Error : No image path found ! ", NO_IMG_PATH);

    int argIdx = 1;
    cv::Mat image = cv::imread(argv[argIdx]); 

    if(image.empty()){
        std::cout << argv[0] << std::endl;
        return throwError(std::string(" Error : Image is empty, no image found!!! ").append(argv[argIdx]), IMG_NOT_FOUND);
    }

    std::cout << std::to_string(image.size().width)<< "/" << std::to_string(image.size().height) << std::endl;
    //making new linear filter

    LinearFilter* filter = new LinearFilter(image);

    // construct the window

    cv::namedWindow(argv[argIdx]);

    cv::imshow(argv[argIdx], image);

    cv::waitKey(0);

    cv::destroyWindow(argv[argIdx]);

    return 0;
    
}