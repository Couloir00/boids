#include "Intensity.hpp"
#include "imgui.h"

bool Intensity::isGui()
{
    return ImGui::SliderFloat("Separation", &m_separationIntensity, 0.00f, 1.f)
           || ImGui::SliderFloat("Cohesion", &m_cohesionIntensity, 0.00f, 1.f)
           || ImGui::SliderFloat("Alignment", &m_alignmentIntensity, 0.00f, 1.f)
           || ImGui::SliderFloat("Perception", &m_perceptionIntensity, 0.001f, 10.f);
}