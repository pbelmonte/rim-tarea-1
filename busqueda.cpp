#include <fstream>
#include <chrono>
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
}

// Guardar informacion de frames y vecinos en archivo
void save(const std::vector<Frame> &videoFrames, const std::string &filename) {
    std::ofstream file;
    file.open(filename);
    for (Frame videoFrame : videoFrames) {
        std::set<std::pair<Frame, int>, Frame::comp> vecinos = videoFrame.getVecinos();
        file << videoFrame.getNumero();
        for (const auto &vecino : vecinos) {
            file << "          ";
            file << vecino.first.getVideoName() << " - " << vecino.first.getNumero() << " - ";
            file << vecino.second;
        }
        file << std::endl;
    }
    file.close();
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "debe entregar un archivo de texto y un directorio" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string source(argv[1]); // video de television
    const std::string dirname(argv[2]); // directorio de comerciales

    std::cout << "Procesando... Esto puede tardar unos minutos" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    Video video(source);

    std::vector<Video> comerciales = getComerciales(dirname);

    std::vector<Frame> videoFrames = video.getFrames();

    getDistancias(videoFrames, getFramesComerciales(comerciales));

    save(videoFrames, changeFile(source, getFileName(source), getFileName(source) + "_distancias"));

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Tiempo de procesamiento: " << elapsed.count() << " segundos" << std::endl;

    return 0;
}
