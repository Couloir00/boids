#include "Intensity/Intensity.hpp"
#include "imgui.h"

void Intensity::GUI(p6::Context& ctx)
{
    ctx.imgui = [&]() {
        ImGui::Begin("Boids behaviors");

        // Intensity GUI
        ImGui::SliderFloat("Separation", &(separationIntensity), 0.00f, 1.f);
        ImGui::SliderFloat("Cohesion", &(cohesionIntensity), 0.00f, 1.f);
        ImGui::SliderFloat("Alignment", &(alignmentIntensity), 0.00f, 1.f);
        ImGui::SliderFloat("Perception", &(perceptionIntensity), 0.001f, 10.f);

        ImGui::End();
    };
}