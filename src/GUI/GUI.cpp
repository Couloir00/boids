#include "GUI.hpp"
#include <imgui.h>

void graphicsUtils::utilitiesWindow(bool& lodsEnabled, bool& fogEnabled, float& fogIntensity)
{
    ImGui::Begin("Utilities");
    ImGui::Text("LODs");
    ImGui::Checkbox("Enable LODs", &lodsEnabled);
    ImGui::Text("Fog");
    ImGui::Checkbox("Enable Fog", &fogEnabled);
    ImGui::SliderFloat("Fog Intensity", &fogIntensity, 0.0001, 0.01);
    ImGui::End();
}