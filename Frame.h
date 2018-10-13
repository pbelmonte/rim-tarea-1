#ifndef RIM_TAREA_1_FRAME_H
#define RIM_TAREA_1_FRAME_H

#include <vector>
#include <set>
#include <utility>
#include <string>

class Frame {

    struct comp {
        bool operator() (const std::pair<Frame, int>& lhs, const std::pair<Frame, int>& rhs) const {
            return lhs.second < rhs.second;
        }
    };

public:
    Frame(std::string vid, int num, std::vector<int> des);

    std::string getVideoName();

    int getNumero();

    std::vector<int> getDescriptor();

    std::set<std::pair<Frame, int>, comp> getVecinos();

    int distancia(Frame frm);

private:
    std::string video_name;
    int numero;
    std::vector<int> descriptor;
    std::set<std::pair<Frame, int>, comp> vecinos;
};


#endif //RIM_TAREA_1_FRAME_H
