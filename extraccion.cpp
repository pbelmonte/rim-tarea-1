#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <utility>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"

#include "utils.h"

const int frameskip = 10; // frames que se salta el programa al generar los descriptores
const int framesize = 10; // tamano de los frames chicos

// Obtener el vector descriptor de un frame
std::vector<int> getVector(const cv::Mat &frame) {

    cv::Mat gray_frame;

    cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY); // escala de grises

    cv::Mat small_frame;

    cv::resize(gray_frame, small_frame, cv::Size(framesize, framesize)); // bajar la resolucion

    std::vector<int> vector(framesize * framesize);

    for (int i = 0; i < small_frame.rows; ++i) {
        for (int j = 0; j < small_frame.cols; ++j) {
            vector.push_back(small_frame.at<uchar>(i,j));
        }
    }

    return vector;
}

// Obtener descriptores de un video completo
std::pair<std::string, std::vector<std::vector<int>>> getDescriptores(const std::string &video) {

    cv::VideoCapture cap(video);

    if (!cap.isOpened()) {
        std::cout << "no se pudo abrir el video \"" << video << "\"" << std::endl;
    }

    std::vector<std::vector<int>> vectores;

    int frame = 0;
    while(cap.grab()) {
        if (frame % frameskip == 0) {
            cv::Mat frame_mat;
            cap.retrieve(frame_mat);
            vectores.push_back(getVector(frame_mat));
            frame++;
        } else {
            frame++;
            continue;
        }
    }

    return std::make_pair(video, vectores);
}

// Generar un vector con los descriptores de todos los comerciales
std::vector<std::pair<std::string, std::vector<std::vector<int>>>> getComerciales(const std::string &dirname) {

    std::vector<std::string> files = listar_archivos(dirname);
    std::vector<std::string> comerciales;

    // se guardan solo los archivos que son videos
    for (const std::string & fullpath : files) {
        std::string ext = getFileExt(fullpath);
        if (ext == "mpg" || ext == "mp4") {
            comerciales.push_back(fullpath);
        }
    }

    std::vector<std::pair<std::string, std::vector<std::vector<int>>>> descriptores;

    for (const std::string & fullpath : comerciales) {
        descriptores.push_back(getDescriptores(fullpath));
    }

    return descriptores;
}

// Guardar descriptores de video en archivo
void save(std::vector<std::vector<int>> vector_of_vectors, const std::string &filename) {
    std::ofstream file;
    file.open(filename);
    for (std::vector<int> vector : vector_of_vectors) {
        for (int v : vector) {
            file << v << " ";
        }
        file << std::endl;
    }
    file.close();
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "debe entregar un archivo y un directorio" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string source(argv[1]); // video de television
    const std::string dirname(argv[2]); // directorio de comerciales

    //obtener video
    std::cout << "Obteniendo video..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    std::pair<std::string, std::vector<std::vector<int>>> video = getDescriptores(source);

    std::string outputfile = std::get<0>(video);
    save(std::get<1>(video), changeFileExt(outputfile, getFileExt(outputfile), "txt"));

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Tiempo en obtener video: " << elapsed.count() << " segundos" << std::endl;


    // obtener comerciales
    std::cout << "Obteniendo comerciales..." << std::endl;
    auto start2 = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<std::string, std::vector<std::vector<int>>>> comerciales = getComerciales(dirname);

    for (const std::pair<std::string, std::vector<std::vector<int>>> &comercial : comerciales) {
        outputfile = std::get<0>(comercial);
        save(std::get<1>(comercial), changeFileExt(outputfile, getFileExt(outputfile), "txt"));
    }

    auto finish2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed2 = finish2 - start2;
    std::cout << "Tiempo en obtener comerciales: " << elapsed2.count() << " segundos" << std::endl;

    return 0;
}
