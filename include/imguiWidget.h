#ifndef BLACKHOLERAYTRACER_IMGUIWIDGET_H
#define BLACKHOLERAYTRACER_IMGUIWIDGET_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <valarray>

constexpr float logMin = -3.0;  //log10(0.001)
constexpr float logMax = 0.0;   //log10(1)

struct BlackHoleSettings {
    int integration = 0;
    const char* integrationType[2] = {"Verlet", "RK4"};
    int accretionDiskOrbit = 0;
    const char* accretionType[2] = {"kepler orbit", "relativistic orbit"};
    bool disk = true;
    bool dopplerEffect = true;
    bool gravitationalRedshift = true;
    bool beaming = true;
    bool realisticTemperature = true;
    float accretionTemp = 7500.0f;
};

struct RendererSettings{
    int toneMapping = 0;
    const char* toneMappingType[4] = {"Hable", "Aces", "Reinhard", "Linear"};

    bool bloomEnabled = true;
    float bloomStrength = 0.1f;
};

void blackHoleWidget(BlackHoleSettings& bh, ImFont* font, int currentWindowWidth, int currentWindowHeight);

void rendererWidget(RendererSettings& rs, ImFont* font, int fps, int &fullScreen, int currentWindowWidth, int currentWindowHeight);

#endif //BLACKHOLERAYTRACER_IMGUIWIDGET_H
