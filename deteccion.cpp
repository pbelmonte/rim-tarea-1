#include <utility>

#include <fstream>
#include <iterator>
#include <sstream>
#include "utils.h"
#include "Frame.h"

void dump(std::ostream &out, const std::vector<std::string> &v)
{
    for (const auto &i : v) {
        out << '\'' << i << '\'' << ' ';
    }

    out << std::endl;
}

size_t split(const std::string &txt, std::vector<std::string> &strs, const std::string &separator)
{
    size_t pos = txt.find(separator);
    size_t initialPos = 0;
    strs.clear();

    while(pos != std::string::npos) {
        strs.push_back( txt.substr(initialPos, pos - initialPos));
        initialPos = pos + separator.length();

        pos = txt.find(separator, initialPos);
    }

    strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

    return strs.size();
}

Frame createVecino(const std::string &input) {
    std::vector<std::string> vector;

    split(input, vector, " - ");

    Frame frame(vector[0], std::stoi(vector[1]), std::vector<int>());

    return frame;
}

// Genera un vector de objetos Frames a partir de un archivo del tipo nombre_video_distancias.txt
std::vector<Frame> getFrames(const std::string &filename) {
    std::ifstream file;
    file.open(filename);
    std::string input;

    std::string nombre = changeFile(filename, "_distancias.txt", "");

    std::vector<Frame> frames;

    while (file) {
        std::getline(file, input);

        if (input.empty()) {
            break;
        }

        std::vector<std::string> splitted;
        split(input, splitted, "          ");

        Frame frame(nombre, std::stoi(splitted[0]), std::vector<int>());

        for (int i = 1; i <= 3 ; ++i) {
            frame.addVecino(createVecino(splitted[i]), i);
        }

        frames.push_back(frame);
    }

    file.close();

    return frames;
}

int main(int argc, char *argv[]) {

    const std::string source(argv[1]); // video

    std::vector<Frame> frames = getFrames(source);

    return 0;
}