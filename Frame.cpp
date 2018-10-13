#include <utility>

#include <utility>

#include <utility>

#include "Frame.h"

Frame::Frame(std::string vid, int num, std::vector<int> des) {
    video_name = std::move(vid);
    numero = num;
    descriptor = std::move(des);
    vecinos = std::set<std::pair<Frame, int>, comp>();
}

std::string Frame::getVideoName() {
    return video_name;
}

int Frame::getNumero() {
    return numero;
}

std::vector<int> Frame::getDescriptor() {
    return descriptor;
}

std::set<std::pair<Frame, int>, Frame::comp> Frame::getVecinos() {
    return vecinos;
}

int Frame::distancia(Frame frm) {
    int sum = 0;
    std::vector<int> frame1 = this->getDescriptor();
    std::vector<int> frame2 = frm.getDescriptor();
    for (int i = 0; i < frame1.size(); ++i) {
        sum += abs(frame1[i] - frame2[i]);
    }
    return sum;
}

void Frame::addVecino(Frame vecino, int dis) {
    vecinos.insert(std::make_pair(vecino, dis));
    if (vecinos.size() > k_vecinos) {
        auto it = vecinos.end();
        it--;
        vecinos.erase(it);
    }
}
