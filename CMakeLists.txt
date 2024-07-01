cmake_minimum_required(VERSION 3.10)
project(meanshift)
find_package(OpenCV REQUIRED)
include_directories(./headers)
add_executable(
    meanshift 
    ./cpp/main.cpp
    ./cpp/linear-filter.cpp
    ./headers/linear-filter.h
    ./headers/filter-function-headers/meanshift
    ./cpp/filter-functions/meanshift.cpp
    ./headers/roi-info-factory.h
    ./cpp/roi-info-factory.cpp
)
target_link_libraries(meanshift ${OpenCV_LIBS})