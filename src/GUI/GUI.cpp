#include "GUI.hpp"

void graphicsUtils::utilitiesWindow(bool& lodsEnabled, bool& fogEnabled)
{
    ImGui::Begin("Utilities");
    ImGui::Text("LODs");
    ImGui::Checkbox("Enable LODs", &lodsEnabled);
    ImGui::Text("Fog");
    ImGui::Checkbox("Enable Fog", &fogEnabled);
    ImGui::End();
}