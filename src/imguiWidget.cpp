#include "imguiWidget.h"

void blackHoleWidget(BlackHoleSettings& bh, ImFont* font)
{
    ImGui::PushFont(font);
    ImGui::Begin("BlackHole Settings");
    ImGui::Text("Accretion disk orbit type");
    ImGui::ListBox(" ", &bh.accretionDiskOrbit, bh.accretionType, IM_ARRAYSIZE(bh.accretionType));

    ImGui::Checkbox("Disk ON/OFF", &bh.disk);
    ImGui::Checkbox("Doppler Shift", &bh.dopplerEffect);
    ImGui::Checkbox("Gravitational Redshift", &bh.gravitationalRedshift);
    ImGui::Checkbox("Relativistic Beaming", &bh.beaming);
    ImGui::Checkbox("Realistic Temperature", &bh.realisticTemperature);
    ImGui::Text("Accretion disk temperature");
    ImGui::SliderFloat("K(Kelvin)", &bh.accretionTemp, 2000.0f, 20000.0f);

    ImGui::End();
    ImGui::PopFont();
}