#include <iostream>
#include <vector>
#include "Video.h"

#include "utils.h"

const int k_vecinos = 3; // numero de vecinos mas cercanos de cada frame

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

// Juntar todos los frames de los comerciales
std::vector<Frame> getFramesComerciales(std::vector<Video> comerciales) {
    std::vector<Frame> frames;
    for (const Video &comercial : comerciales) {
        for (const Frame &frms : comercial.getFrames()) {
            frames.push_back(frms);
        }
    }
    return frames;
}

// Guardar distancias entre frames
void getDistancias(std::vector<Frame> &videoFrames, const std::vector<Frame> &comercialesFrames) {
    for (Frame& videoFrame : videoFrames) {
        for (const Frame &comercialFrame : comercialesFrames) {
            int distancia = videoFrame.distancia(comercialFrame);
            videoFrame.addVecino(comercialFrame, distancia);
        }
    }

    /*for (Frame videoFrame : videoFrames) {
        std::set<std::pair<Frame, int>, Frame::comp> vecinos = videoFrame.getVecinos();
        std::cout << vecinos.size() << std::endl;
        std::cout << videoFrame.getVideoName() << " " << videoFrame.getNumero() << " ";
        for (std::pair<Frame, int> par : vecinos) {
            std::cout << "|";
            std::cout << std::get<0>(par).getVideoName() << " " << std::get<0>(par).getNumero() << " ";
            std::cout << std::get<1>(par);
        }
        std::cout << std::endl;
    }*/
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

    std::vector<Frame> videoFrames = video.getFrames();

    getDistancias(videoFrames, getFramesComerciales(comerciales));

    return 0;
}
