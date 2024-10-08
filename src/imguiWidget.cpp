#include "imguiWidget.h"

void blackHoleWidget(BlackHoleSettings& bh, ImFont* font, int currentWindowWidth, int currentWindowHeight)
{
    ImGui::SetNextWindowPos(ImVec2(currentWindowWidth - 420, currentWindowHeight), ImGuiCond_Always);
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

void rendererWidget(RendererSettings& rs, ImFont* font, int fps, int &fullScreen, int currentWindowWidth, int currentWindowHeight)
{
    ImGui::SetNextWindowPos(ImVec2(currentWindowWidth - 420, currentWindowHeight + 320), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(420, 360), ImGuiCond_Always);

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

    if (ImGui::Button("FullScreen/Window"))
    {
        if (fullScreen == 0)
            fullScreen = 1;
        else if (fullScreen == 1)
            fullScreen = 2;
        else if (fullScreen == 2)
            fullScreen = 1;
    }

    ImGui::End();
    ImGui::PopFont();
}