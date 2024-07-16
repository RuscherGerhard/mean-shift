#include "../headers/linear-filter.h"

enum Errors {
    ERROR_SUCCESS,
    NO_IMG_PATH,
    IMG_NOT_FOUND
} Errors_T;

int throwError(const std::string err_str, const Errors err_code){
     std::cout << "<< press key! >>" << std::endl;

    // wait for user interaction
    std::cin.get();

    // return error code
    return static_cast<int>( err_code);
}

void throwWarning(const std::string err_str){
    // print error message
    std::cout << "WARNING : " << err_str.c_str() << "\n" << std::endl;
}

void showStats(unsigned long rows, unsigned long cols, size_t threadAmmount, std::string name){

    std::cout<< "| Name    : " << name << std::endl;
    std::cout<< "============" << std::endl;
    std::cout<< "| rows    : " << std::to_string(rows) << std::endl;
    std::cout<< "| cols    : " << std::to_string(cols) << std::endl;
    std::cout<< "| threads : " << std::to_string(threadAmmount)<<"\n _" << std::endl;
    
}


int MAX_THREAD_LIMIT = 29;

int main(int argc, char* argv[]){
    // check if we have argument
    if(argc <= 1){

        return throwError(" No image path found ! \n USAGE:\n ./meanshit <path/to/image> <amount of threads(optional)>", NO_IMG_PATH);
    }

    int argIdx = 1;
    cv::Mat image = cv::imread(argv[argIdx]); 

    if(image.empty()){
        std::cout << argv[0] << std::endl;
        return throwError(std::string(" Image is empty, no image found!!! ").append(argv[argIdx]), IMG_NOT_FOUND);
    }

    size_t threadAmmount = 2;
    
    if(argc > 2){
        threadAmmount = atoi(argv[2]);
    }

    if(threadAmmount > MAX_THREAD_LIMIT){
        throwWarning(std::string(" To many threads, setting limit to max thread limit : ").append(std::to_string(MAX_THREAD_LIMIT)));
        threadAmmount = MAX_THREAD_LIMIT;
    }
    showStats(image.rows, image.cols, threadAmmount, argv[1]);

    LinearFilter* filter = new LinearFilter(&image, threadAmmount);

    // construct the window
    cv::namedWindow(argv[argIdx]);

    cv::imshow(argv[argIdx], image);

     std::cout << "\n< < Press Key! > >" << std::endl;

    cv::waitKey(0);

    cv::destroyWindow(argv[argIdx]);

    return 0;
}