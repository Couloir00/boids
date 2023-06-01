#include "GUI.hpp"
#include <imgui.h>

void graphicsUtils::utilitiesWindow(bool& lodsEnabled, bool& fogEnabled, float& fogIntensity, float& fogRed, float& fogGreen, float& fogBlue)
{
    ImGui::Begin("Utilities");
    ImGui::Text("LODs");
    ImGui::Checkbox("Enable LODs", &lodsEnabled);
    ImGui::Text("Fog");
    ImGui::Checkbox("Enable Fog", &fogEnabled);
    ImGui::SliderFloat("Fog Intensity", &fogIntensity, 0.0001f, 0.025f);
    float fogColor[3] = {fogRed, fogGreen, fogBlue};
    if (ImGui::ColorEdit3("Fog Color", fogColor))
    {
        fogRed   = fogColor[0];
        fogGreen = fogColor[1];
        fogBlue  = fogColor[2];
    }
    ImGui::End();
}