#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

#include "Video.h"
#include "utils.h"

Video::Video(std::string path) {
    filepath = path;
    std::string filename = getFileName(path);
    std::ifstream file;
    file.open(path);
    std::string input;
    std::vector<int> descriptor;

    int frm_num = 0;
    while (file) {
        descriptor = std::vector<int>();
        std::getline(file, input);
        std::stringstream ss(input);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin, end);
        if (vstrings.empty())
            break;
        for (const std::string &s : vstrings) {
            descriptor.push_back(std::stoi(s));
        }
        Frame frm(filename, frm_num, descriptor);
        frames.push_back(frm);
        frm_num++;
    }

    file.close();
}

std::string Video::getFilePath() {
    return filepath;
}

std::vector<Frame> Video::getFrames() {
    return frames;
}
