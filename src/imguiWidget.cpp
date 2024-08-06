#include "imguiWidget.h"

void blackHoleWidget(BlackHoleSettings& bh, ImFont* font)
{
    ImGui::SetNextWindowPos(ImVec2(1500, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(420, 320), ImGuiCond_Always);

    ImGui::PushFont(font);
    ImGui::Begin("BlackHole Settings");
    ImGui::Text("Integration Type");
    ImGui::ListBox("  ", &bh.integration, bh.integrationType, IM_ARRAYSIZE(bh.integrationType));
    ImGui::Spacing();

    //ImGui::Text("Accretion disk orbit type");
    //ImGui::ListBox(" ", &bh.accretionDiskOrbit, bh.accretionType, IM_ARRAYSIZE(bh.accretionType));

    ImGui::Checkbox("Disk ON/OFF", &bh.disk);
    ImGui::Checkbox("Doppler Shift, Beaming", &bh.dopplerEffect);
    //ImGui::Checkbox("Gravitational Redshift", &bh.gravitationalRedshift);
    //ImGui::Checkbox("Relativistic Beaming", &bh.beaming);
    ImGui::Checkbox("Realistic Temperature", &bh.realisticTemperature);
    ImGui::Text("Accretion Disk Temperature");
    ImGui::SliderFloat("K(Kelvin)", &bh.accretionTemp, 2000.0f, 20000.0f);

    ImGui::End();
    ImGui::PopFont();
}

void rendererWidget(RendererSettings& rs, ImFont* font, int fps)
{
    ImGui::SetNextWindowPos(ImVec2(1500, 320), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(420, 324), ImGuiCond_Always);

    ImGui::PushFont(font);
    ImGui::Begin("Renderer Settings");

    ImGui::Text("FPS: %d", fps);

    ImGui::Text("ToneMapping Type");
    ImGui::ListBox(" ", &rs.toneMapping, rs.toneMappingType, IM_ARRAYSIZE(rs.toneMappingType));

    ImGui::Checkbox("Bloom", &rs.bloomEnabled);

    float tmp = log10f(rs.bloomStrength);
    ImGui::Text("Bloom Strength(log10)");
    ImGui::SliderFloat(" ", &tmp, logMin, logMax);
    rs.bloomStrength = powf(10.0f, tmp);

    ImGui::End();
    ImGui::PopFont();
}