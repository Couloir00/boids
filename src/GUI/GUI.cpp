#include "GUI.hpp"
#include <imgui.h>

void graphicsUtils::utilitiesWindow(bool& lodsEnabled, bool& fogEnabled, float& fogIntensity, float& fogRed, float& fogGreen, float& fogBlue)
{
    ImGui::Begin("Utilities");
    ImGui::Text("LODs");
    ImGui::Checkbox("Enable LODs", &lodsEnabled);
    ImGui::Text("Fog");
    ImGui::Checkbox("Enable Fog", &fogEnabled);
    ImGui::SliderFloat("Fog Intensity", &fogIntensity, 0.0001f, 0.01f);
    ImGui::SliderFloat("Fog Red", &fogRed, 0.0f, 1.0f);
    ImGui::SliderFloat("Fog Green", &fogGreen, 0.0f, 1.0f);
    ImGui::SliderFloat("Fog Blue", &fogBlue, 0.0f, 1.0f);
    ImGui::End();
}