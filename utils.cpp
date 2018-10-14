#include "utils.h"

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

// Obtener la extension de un archivo
std::string getFileExt(const std::string& s) {

    size_t i = s.rfind('.', s.length());
    if (i != std::string::npos) {
        return(s.substr(i+1, s.length() - i));
    }

    return("");
}

// Cambiar el nombre (o extension) de un archivo
std::string changeFile(std::string s, const std::string &from, const std::string &to) {

    size_t start_pos = s.rfind(from);
    if(start_pos == std::string::npos)
        return "";
    s.replace(start_pos, from.length(), to);
    return s;
}

// Obtener el nombre del archivo
std::string getFileName(const std::string &s) {
    std::string st = changeFile(s, getFileExt(s), "");
    st = st.substr(0, st.length() - 1);
    size_t i = st.rfind('/', st.length());
    if (i != std::string::npos) {
        return st.substr(i+1, st.length());
    }
    return "";
}
