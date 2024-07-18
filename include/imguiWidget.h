#ifndef BLACKHOLERAYTRACER_IMGUIWIDGET_H
#define BLACKHOLERAYTRACER_IMGUIWIDGET_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

struct BlackHoleSettings {
    int accretionDiskOrbit = 0;
    const char* accretionType[2] = {"kepler orbit", "relativistic orbit"};
    bool disk = true;
    bool dopplerEffect = true;
    bool gravitationalRedshift = true;
    bool beaming = true;
    bool realisticTemperature = false;
    float accretionTemp = 3500.0f;
};

void blackHoleWidget(BlackHoleSettings& bh, ImFont* font);

#endif //BLACKHOLERAYTRACER_IMGUIWIDGET_H
