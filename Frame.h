#ifndef RIM_TAREA_1_FRAME_H
#define RIM_TAREA_1_FRAME_H

#include <vector>
#include <set>
#include <utility>
#include <string>

class Frame {

public:
    struct comp {
        bool operator() (const std::pair<Frame, int>& lhs, const std::pair<Frame, int>& rhs) const {
            return lhs.second < rhs.second;
        }
    };

    Frame(std::string vid, int num, std::vector<int> des);

    std::string getVideoName();

    int getNumero();

    std::vector<int> getDescriptor();

    std::set<std::pair<Frame, int>, comp> getVecinos();

    int distancia(Frame frm);

    void addVecino(Frame vecino, int dis);

private:
    std::string video_name;
    int numero;
    std::vector<int> descriptor;
    std::set<std::pair<Frame, int>, comp> vecinos;
    const int k_vecinos = 3;
};


#endif //RIM_TAREA_1_FRAME_H
