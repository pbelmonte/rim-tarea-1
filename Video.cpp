#include <utility>

#include <utility>

#include "Video.h"

Video::Video(std::string name, std::vector<std::vector<int>> vector) {
    filename = std::move(name);
    descriptors = std::move(vector);
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
