#include <chrono>
#include <fstream>
#include <utility>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"

#include "utils.h"

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
std::pair<std::string, std::vector<std::vector<int>>> getDescriptores(const std::string &video, bool esComercial) {

    cv::VideoCapture cap(video);

    if (!cap.isOpened()) {
        std::cout << "no se pudo abrir el video \"" << video << "\"" << std::endl;
    }

    std::vector<std::vector<int>> vectores;

    int total_frames = (int) cap.get(cv::CAP_PROP_FRAME_COUNT) / frameskip;

    int i = 0;

    int porcentaje = 0;

    int porcentaje_anterior = -1;

    int frame = 0;
    while(cap.grab()) {
        if (frame % frameskip == 0) {

            i++;
            porcentaje = (i * 100) / total_frames;
            if (porcentaje - porcentaje_anterior == 1 && !esComercial) {
                std::cout << "\r" << "Progreso: [";
                if (porcentaje < 100)
                    std::cout << " ";
                if (porcentaje < 10)
                    std::cout << " ";
                std::cout << porcentaje << "%] ";
                std::cout << "[";
                for (int j = 1; j <= 100; j++) {
                    if (j <= porcentaje)
                        std::cout << "#";
                    else
                        std::cout << ".";
                }
                std::cout << "]" << std::flush;
                porcentaje_anterior = porcentaje;
            }

            cv::Mat frame_mat;
            cap.retrieve(frame_mat);
            vectores.push_back(getVector(frame_mat));
            frame++;
        } else {
            frame++;
            continue;
        }
    }

    if (!esComercial)
        std::cout << std::endl;

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
        descriptores.push_back(getDescriptores(fullpath, true));
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
    std::cout << "Obteniendo video... Esto puede tardar unos minutos" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    std::pair<std::string, std::vector<std::vector<int>>> video = getDescriptores(source, false);

    std::string outputfile = video.first;
    save(video.second, changeFile(outputfile, getFileExt(outputfile), "txt"));

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Tiempo en obtener video: " << elapsed.count() << " segundos" << std::endl;


    // obtener comerciales
    std::cout << "Obteniendo comerciales..." << std::endl;
    auto start2 = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<std::string, std::vector<std::vector<int>>>> comerciales = getComerciales(dirname);

    for (const std::pair<std::string, std::vector<std::vector<int>>> &comercial : comerciales) {
        outputfile = comercial.first;
        save(comercial.second, changeFile(outputfile, getFileExt(outputfile), "txt"));
    }

    auto finish2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed2 = finish2 - start2;
    std::cout << "Tiempo en obtener comerciales: " << elapsed2.count() << " segundos" << std::endl;

    return 0;
}
