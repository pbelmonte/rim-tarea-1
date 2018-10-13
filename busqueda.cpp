#include <iostream>
#include <vector>
#include "Video.h"

#include "utils.h"

// Generar un vector con todos los comerciales
std::vector<Video> getComerciales(const std::string &dirname) {

    std::vector<std::string> files = listar_archivos(dirname);
    std::vector<std::string> comerciales;

    // se guardan solo los txt
    for (const std::string & fullpath : files) {
        std::string ext = getFileExt(fullpath);
        if (ext == "txt") {
            comerciales.push_back(fullpath);
        }
    }

    std::vector<Video> videos;

    for (const std::string & fullpath : comerciales) {
        Video comercial(fullpath);
        videos.push_back(comercial);
    }

    return videos;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "debe entregar un archivo de texto y un directorio" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string source(argv[1]); // video de television
    const std::string dirname(argv[2]); // directorio de comerciales

    Video video(source);

    std::vector<Video> comerciales = getComerciales(dirname);

    std::vector<Frame> vec = video.getFrames();

    return 0;
}
