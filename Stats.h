#ifndef RIM_TAREA_1_STATS_H
#define RIM_TAREA_1_STATS_H

#include <string>

class Stats {

public:

    Stats();

    Stats(int ini, int ante, int punt, int frm);

    int getFrameInicio();

    int getFrameAnterior();

    int getPuntaje();

    int getFrameVideoAnterior();

    void setFrameInicio(int frm);

    void setFrameAnterior(int frm);

    void setPuntaje(int punt);

    void setFrameVideoAnterior(int frm);

private:
    int frame_inicio;
    int frame_anterior;
    int puntaje;
    int frame_video_anterior;
};


#endif //RIM_TAREA_1_STATS_H
