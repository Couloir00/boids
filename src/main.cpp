
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <cstdlib>
#include <vector>
#include "Boid/boid.hpp"
#include "Intensity/Intensity.hpp"
#include "doctest/doctest.h"
#include "glm/fwd.hpp"
#include "imgui.h"

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "giveItATry"}};
    ctx.maximize_window();

    // vector of numberBoids boids
    static std::vector<Boid> boids;
    int                      numberBoids = 100;
    float                    radius      = 0.05f;
    Intensity                intensities{0.5f, 0.5f, 0.5f, 0.5f};
    boids.reserve(numberBoids);
    for (int i = 0; i < numberBoids; i++)
    {
        boids.emplace_back();
    }

    // ImGui context
    ctx.imgui = [&]() {
        ImGui::Begin("Sliders");
        ImGui::Text("Behaviors");

        // Intensity GUI
        if (intensities.isGui())
        {
            for (auto& boid : boids)
            {
                boid.setIntensities(intensities);
            }
        }
        ImGui::Text("Boids");
        ImGui::SliderFloat("Radius", &radius, 0.001f, 1.f);
        ImGui::End();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);

        // Boids shape
        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.2f}
        );

        for (auto& b : boids)
        {
            b.avoidEdges(ctx, radius);
            b.flock(boids, ctx); // updating the boids here
            //  if (b.closeToEdges(ctx))
            //  {
            //      b.setVelocity(-b.getVelocity());
            //  }
            b.draw(ctx, radius);
        }
    };
    // Should be done last. It starts the infinite loop.
    ctx.start();
}