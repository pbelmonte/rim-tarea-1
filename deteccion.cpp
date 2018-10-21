#include <utility>
#include <fstream>
#include <iterator>
#include <unordered_map>
#include "utils.h"
#include "Frame.h"
#include "Stats.h"

// Divide un string usando un separador y guarda el resultado en un vector
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

/*
 * Genera un objeto Frame a partir de un string del tipo "nombre - numero - distancia" que sera un vecino
 * como no se tiene (ni se necesita) un vector descriptor se crea con un vector vacio
 */
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

    std::string nombre = changeFile(filename, "_distancias", "");
    nombre = getFileName(nombre);

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

// Encuentra el frame en donde se termina el comercial
int getEnd(const std::string &comercial, int frame_actual, std::vector<Frame> frames) {

    // la cantidad de veces que no se ha encontrado un frame del comercial entre los vecinos del frame del video
    int strikes = 0;

    // frame de termino
    int end = frame_actual;

    for (int i = frame_actual; i < frames.size(); i++) {
        std::set<std::pair<Frame, int>, Frame::comp> vecinos = frames[i].getVecinos();
        for (auto frm : vecinos) {
            if (frm.first.getVideoName() == comercial) {
                end = i;
                strikes = 0;
                break;
            }
        }
        strikes++;

        if (strikes == 10) { // ya se acabó el comercial
            return end;
        }
    }
    return 0;
}

/*
 * Detecta los comerciales y guarda los datos en el archivo detecciones.txt
 * se usa un sistema de puntos, si se encuentra un comercial, se le aumenta el puntaje a este
 */
void detectar(std::vector<Frame> frames) {

    std::ofstream file;
    file.open("detecciones.txt");

    // comerciales que tienen puntos
    std::unordered_map<std::string, Stats> comerciales;

    int i = 0;
    while (i < frames.size()) {
        Frame frm = frames[i];
        std::set<std::pair<Frame, int>, Frame::comp> vecinos = frm.getVecinos();
        for (const auto &vecino : vecinos) {
            std::string comercial = vecino.first.getVideoName();
            int numero = vecino.first.getNumero();

            if (comerciales.count(comercial)) { // si el comercial esta en el map
                int ini = comerciales[comercial].getFrameInicio();
                int ante = comerciales[comercial].getFrameAnterior();
                int punt = comerciales[comercial].getPuntaje();
                int ante_video = comerciales[comercial].getFrameVideoAnterior();

                Stats newStats(ini, numero, punt + 1, frm.getNumero());
                if (ante_video == frm.getNumero()) { // estamos en otro vecino del mismo frame
                    if (ante < numero)
                        newStats.setFrameAnterior(ante);
                }

                else if (numero - ante > 0 && numero - ante <= 2) {
                    newStats.setPuntaje(newStats.getPuntaje() + 1);
                }

                else {
                    newStats.setPuntaje(newStats.getPuntaje() - 1);
                }

                comerciales[comercial] = newStats;


                /*
                 * Se le restan puntos a los comerciales que no eran vecinos y si tienen un puntaje menor o igual
                 * a 0 se eliminan del map de comerciales
                 */
                auto it = comerciales.begin();

                while (it != comerciales.end()) {
                    if (it->first != comercial) {
                        it->second.setPuntaje(it->second.getPuntaje() - 4);
                        if (it->second.getPuntaje() <= 0) {
                            it = comerciales.erase(it);
                        }
                    } else {
                        it++;
                    }
                }

                /* Si se alcanza un puntaje alto se asume que se encontro un comercial, solo hay que encontrar donde
                 * termina usando la funcion getEnd
                 */
                if (comerciales[comercial].getPuntaje() > 30) {

                    int end = getEnd(comercial, frm.getNumero(), frames);
                    float fraps = fps / frameskip;
                    float inicio = comerciales[comercial].getFrameInicio() / fraps;
                    float fin = end / fraps;
                    float largo = fin - inicio;

                    // Se guardan los datos en el archivo
                    file << frm.getVideoName() << "\t" << inicio << "\t" << largo << "\t" << comercial;
                    file << std::endl;
                    i = end;
                    comerciales.erase(comercial);
                    break;
                }

            }
            else { // si el comercial no esta en el map
                Stats newStats(frm.getNumero(), numero, 4, frm.getNumero());
                comerciales[comercial] = newStats;
            }
        }

        i++;
    }
}

int main(int argc, char *argv[]) {

    const std::string source(argv[1]); // video

    std::vector<Frame> frames = getFrames(source);

    detectar(frames);

    return 0;
}