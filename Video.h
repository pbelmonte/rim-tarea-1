#ifndef RIM_TAREA_1_VIDEO_H
#define RIM_TAREA_1_VIDEO_H

#include <string>
#include <vector>
#include "Frame.h"

class Video {

public:
    explicit Video(std::string path);

    std::string getFilePath();

    std::vector<Frame> getFrames() const;

private:
    std::string filepath;
    std::vector<Frame> frames;
};


#endif //RIM_TAREA_1_VIDEO_H
