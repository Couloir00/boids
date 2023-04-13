#define DOCTEST_CONFIG_IMPLEMENT
#include <cstdlib>
#include <vector>
#include "Boid/boid.hpp"
#include "Camera/FreeflyCamera.hpp"
#include "Intensity/Intensity.hpp"
#include "Model/Model.hpp"
#include "Model/ModelControls.hpp"
#include "doctest/doctest.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "imgui.h"
#include "p6/p6.h"

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
    // shaders
    const p6::Shader myShaders = p6::load_shader("Shaders/3D.vs.glsl", "Shaders/normals.fs.glsl");
    // camera
    FreeflyCamera camera;
    // Model init test
    Model test("assets/new-graveyard.obj");

    test.modelLoad();
    test.modelInitialize();

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ModelControls controls{glm::vec3(-1.23f, 0.f, -1), glm::vec3(0.01, 0, 2), 0.1};
        // ctx.background(p6::NamedColor::Blue);
        glClearColor(1.000f, 0.662f, 0.970f, 1.000f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShaders.use();
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 ViewMatrix = camera.getViewMatrix();

        // mouse
        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.2f}
        );

        test.modelDraw(myShaders, ViewMatrix, controls, ProjMatrix);

        for (auto& b : boids)
        {
            b.avoidEdges(ctx, radius);
            b.flock(boids, ctx); // updating the boids here
            b.draw(ctx, radius);
        }
    };
    // camera to fix
    /*
    ctx.key_pressed = [&](const p6::Key& key) {
        cameraControls(key, camera);
    };*/

    // Should be done last. It starts the infinite loop.
    ctx.start();
}