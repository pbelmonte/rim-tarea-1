#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>

#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"

#if defined WIN32 || defined _WIN32
#define IS_WINDOWS 1
#endif

#if defined __linux || defined __linux__
#define IS_LINUX 1
#endif

void agregar_archivo(const std::string &dirname, const std::string &name, std::vector<std::string> &list) {
    std::string fullpath = dirname + "/" + name;
#if IS_WINDOWS
    struct stat64 st;
    	int status = stat64(fullpath.c_str(), &st);
#elif IS_LINUX
    struct stat st;
    int status = stat(fullpath.c_str(), &st);
#endif
    if (status == 0 && S_ISREG(st.st_mode)) {
        list.push_back(fullpath);
    }
}

std::vector<std::string> listar_archivos(const std::string &dirname) {
    std::vector<std::string> list;
#if IS_WINDOWS
    DIR *dp = opendir(dirname.c_str());
    	if (dp == NULL) {
    		std::cout << "error abriendo " << dirname << std::endl;
    		return list;
    	}
    	struct dirent *dir_entry;
    	while ((dir_entry = readdir(dp)) != NULL) {
    		std::string name(dir_entry->d_name);
    		agregar_archivo(dirname, name, list);
    	}
    	if (closedir(dp) != 0) {
    		std::cout << "error cerrando " << dirname << std::endl;
    	}
#elif IS_LINUX
    struct dirent **namelist = NULL;
    int len = scandir(dirname.c_str(), &namelist, NULL, NULL);
    if (len < 0) {
        std::cout << "error abriendo " << dirname << std::endl;
        return list;
    }
    for (int i = 0; i < len; ++i) {
        std::string name(namelist[i]->d_name);
        agregar_archivo(dirname, name, list);
        free(namelist[i]);
    }
    free(namelist);
#endif
    std::sort(list.begin(), list.end());
    return list;
}


const int frameskip = 10; // frames que se salta el programa al generar los descriptores
const int framesize = 14; // tamano de los frames chicos

// Obtener la extension de un archivo
std::string getFileExt(const std::string& s) {

    size_t i = s.rfind('.', s.length());
    if (i != std::string::npos) {
        return(s.substr(i+1, s.length() - i));
    }

    return("");
}

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

// Obtener descriptor de un video completo
std::vector<std::vector<int>> getDescriptor(const std::string &video) {

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

    return vectores;
}

// Generar un vector con los descriptores de todos los comerciales
std::vector<std::vector<std::vector<int>>> getComerciales(const std::string &dirname) {

    std::vector<std::string> files = listar_archivos(dirname);
    std::vector<std::string> comerciales;

    // se eliminan los archivos que no son videos
    for (const std::string & fullpath : files) {
        std::string ext = getFileExt(fullpath);
        if (ext == "mpg" || ext == "mp4") {
            comerciales.push_back(fullpath);
        }
    }

    std::vector<std::vector<std::vector<int>>> descriptores(files.size());

    for (const std::string & fullpath : comerciales) {
        descriptores.push_back(getDescriptor(fullpath));
    }

    return descriptores;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "debe entregar un archivo y un directorio" << std::endl;
        return EXIT_FAILURE;
    }

    const std::string source(argv[1]); // video de television
    const std::string dirname(argv[2]); // directorio de comerciales

    std::cout << "Obteniendo video..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> video = getDescriptor(source);
    // std::ofstream video_file ("descriptor_video.txt");
    // video_file << video << std::endl;
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Tiempo en obtener video: " << elapsed.count() << " segundos" << std::endl;

    std::cout << "Obteniendo comerciales..." << std::endl;
    auto start2 = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<std::vector<int>>> comerciales = getComerciales(dirname);
    auto finish2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed2 = finish2 - start2;
    std::cout << "Tiempo en obtener comerciales: " << elapsed2.count() << " segundos" << std::endl;

    return 0;
}
