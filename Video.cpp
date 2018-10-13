#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

#include "Video.h"

Video::Video(std::string filepath) {
    filename = filepath;
    std::ifstream file;
    file.open(filepath);
    std::string input;
    std::vector<int> descriptor;

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
            descriptor.push_back(stoi(s));
        }
        descriptors.push_back(descriptor);
    }

    file.close();
}

std::string Video::getFilename() {
    return filename;
}

std::vector<std::vector<int>> Video::getDescriptors() {
    return descriptors;
}

int Video::distance(std::vector<int> frame1, std::vector<int> frame2) {
    int sum = 0;
    for (int i = 0; i < frame1.size(); ++i) {
        sum += abs(frame1[i] - frame2[i]);
    }
    return sum;
}
