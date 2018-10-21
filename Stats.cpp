#include "Stats.h"

Stats::Stats(int ini, int ante, int punt, int frm) {
    frame_inicio = ini;
    frame_anterior = ante;
    puntaje = punt;
    frame_video_anterior = frm;
}

int Stats::getFrameInicio() {
    return frame_inicio;
}

int Stats::getFrameAnterior() {
    return frame_anterior;
}

int Stats::getPuntaje() {
    return puntaje;
}

int Stats::getFrameVideoAnterior() {
    return frame_video_anterior;
}

void Stats::setFrameInicio(int frm) {
    frame_inicio = frm;
}

void Stats::setFrameAnterior(int frm) {
    frame_anterior = frm;
}

void Stats::setPuntaje(int punt) {
    puntaje = punt;
}

void Stats::setFrameVideoAnterior(int frm) {
    frame_video_anterior = frm;
}

Stats::Stats() {

}
