#ifndef RIM_TAREA_1_UTILS_H
#define RIM_TAREA_1_UTILS_H

#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#if defined WIN32 || defined _WIN32
#define IS_WINDOWS 1
#endif

#if defined __linux || defined __linux__
#define IS_LINUX 1
#endif

void agregar_archivo(const std::string &dirname, const std::string &name, std::vector<std::string> &list);

std::vector<std::string> listar_archivos(const std::string &dirname);

std::string getFileExt(const std::string& s);

std::string changeFile(std::string s, const std::string &from, const std::string &to);

std::string getFileName(const std::string& s);

#endif //RIM_TAREA_1_UTILS_H
