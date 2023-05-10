#define DOCTEST_CONFIG_IMPLEMENT
#include <cstdlib>
#include <vector>
#include "Boid/boid.hpp"
#include "Camera/FreeflyCamera.hpp"
#include "Intensity/Intensity.hpp"
#include "Model/Model.hpp"
#include "Model/ModelControls.hpp"
#include "Sphere/common.hpp"
#include "Sphere/sphere_vertices.hpp"
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
    static std::vector<Boid>
              boids;
    int       numberBoids = 100;
    float     radius      = 0.05f;
    Intensity intensities{0.5f, 0.5f, 0.5f, 0.5f};
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
        ImGui::SliderFloat("Separation", &(intensities.separationIntensity), 0.00f, 1.f);
        ImGui::SliderFloat("Cohesion", &(intensities.cohesionIntensity), 0.00f, 1.f);
        ImGui::SliderFloat("Alignment", &(intensities.alignmentIntensity), 0.00f, 1.f);
        ImGui::SliderFloat("Perception", &(intensities.perceptionIntensity), 0.001f, 10.f);

        ImGui::Text("Boids");
        ImGui::SliderFloat("Radius", &radius, 0.001f, 1.f);
        ImGui::End();
    };

    /// INIT///
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    // shaders
    const p6::Shader myShaders = p6::load_shader("Shaders/3D.vs.glsl", "Shaders/ponctualLight.fs.glsl");
    // light
    glm::vec3 lightPos       = glm::vec3(0.0f, 1.0f, -3.0f);
    glm::vec3 lightIntensity = glm::vec3(3.0f, 3.0f, 3.0f);
    myShaders.set("uLightPos_vs", lightPos);
    myShaders.set("uLightIntensity", lightIntensity);
    // camera
    FreeflyCamera camera;
    // Model init test
    Model test("Assets/ghost.obj");

    test.modelLoad();
    test.modelInitialize();

    // camera controls checker
    bool Z = false;
    bool Q = false;
    bool S = false;
    bool D = false;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ModelControls controls{glm::vec3(0.f, 0.f, -1.5), glm::vec3(0.01, 0, 2), 1.f};
        // ctx.background(p6::NamedColor::Blue);
        // glClearColor(1.000f, 0.662f, 0.970f, 1.000f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShaders.use();
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 ViewMatrix = camera.getViewMatrix();

        // camera moving
        if (Z)
            camera.moveFront(0.1f);
        if (Q)
            camera.moveLeft(0.1);
        if (S)
            camera.moveFront(-0.1);
        if (D)
            camera.moveLeft(-0.1);

        // mouse
        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.2f}
        );

        test.modelDraw(myShaders, ViewMatrix, controls, ProjMatrix);

        for (auto& b : boids)
        {
            b.avoidEdges(ctx, radius);
            b.flock(boids, ctx, intensities); // updating the boids here
            b.draw(ctx, radius);
        }
    };

    ctx.key_pressed = [&Z, &Q, &S, &D](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
        {
            Z = true;
        }
        if (key.physical == GLFW_KEY_A)
        {
            Q = true;
        }
        if (key.physical == GLFW_KEY_S)
        {
            S = true;
        }
        if (key.physical == GLFW_KEY_D)
        {
            D = true;
        }
    };

    ctx.key_released = [&Z, &Q, &S, &D](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
        {
            Z = false;
        }
        if (key.physical == GLFW_KEY_A)
        {
            Q = false;
        }
        if (key.physical == GLFW_KEY_S)
        {
            S = false;
        }
        if (key.physical == GLFW_KEY_D)
        {
            D = false;
        }
    };

    ctx.mouse_dragged = [&](p6::MouseDrag drag) {
        camera.rotateLeft(drag.delta.x * 25.f);
        camera.rotateUp(drag.delta.y * 25.f);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}