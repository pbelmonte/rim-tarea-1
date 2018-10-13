#ifndef RIM_TAREA_1_VIDEO_H
#define RIM_TAREA_1_VIDEO_H

#include <string>
#include <vector>

class Video {

public:
    Video(std::string name, std::vector<std::vector<int>> vector);

    std::string getFilename();

    std::vector<std::vector<int>> getDescriptors();

    int distance(std::vector<int> frame1, std::vector<int> frame2);

private:
    std::string filename;
    std::vector<std::vector<int>> descriptors;
};


#endif //RIM_TAREA_1_VIDEO_H
