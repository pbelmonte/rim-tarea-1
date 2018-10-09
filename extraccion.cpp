#include <iostream>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"

int main(int argc, char *argv[]) {
    //const std::string source = argv[1];

    cv::VideoCapture inputVideo("comerciales/");

    std::cout << "buena" << std::endl;

    if(!inputVideo.isOpened())
        return -1;

    return 0;
}