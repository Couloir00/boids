#define DOCTEST_CONFIG_IMPLEMENT
#include <cstdlib>
#include <vector>
#include "Boid/boid.hpp"
#include "Camera/FreeflyCamera.hpp"
#include "Intensity/Intensity.hpp"
#include "LODModel/LODModel.hpp"
#include "Lights/LightManager.hpp"
#include "Model/Model.hpp"
#include "Model/ModelControls.hpp"
#include "Model/ModelLod.hpp"
#include "SkyboxEnv/SkyboxEnv.hpp"
#include "Sphere/sphere_vertices.hpp"
#include "Texture/Texture.hpp"
#include "doctest/doctest.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "img/src/Image.h"
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
    int numberBoids = 10;
    // float     radius      = 0.05f;
    Intensity intensities{0.5f, 0.5f, 0.5f, 0.5f};
    boids.reserve(numberBoids);
    for (int i = 0; i < numberBoids; i++)
    {
        boids.emplace_back(
            glm::vec3(p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)),
            glm::vec3(100000.f), 0.5
        );
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
        // ImGui::SliderFloat("Radius", &radius, 0.001f, 1.f);
        ImGui::End();
    };

    /// INIT///
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    // shaders
    const p6::Shader myShaders = p6::load_shader("Shaders/3D.vs.glsl", "Shaders/Light.fs.glsl");

    // camera
    FreeflyCamera camera;
    img::Image    aTex = p6::load_image_buffer("Assets/remi.png");
    Texture       aTexture{};
    aTexture.initTexture(static_cast<int>(aTex.width()), static_cast<int>(aTex.height()), aTex.data(), GL_RGBA, GL_UNSIGNED_BYTE);

    Texture                 cubemapTex;
    std::vector<img::Image> Images;
    p6::Shader              skyboxShader = p6::load_shader("Shaders/SkyboxEnv.vs.glsl", "Shaders/SkyboxEnv.fs.glsl");
    VAO                     skyboxVAO;
    VBO                     skyboxVBO;
    generateSkybox(cubemapTex, Images, skyboxVAO, skyboxVBO);
    skyboxShader.use();
    skyboxShader.set("skybox", 0);

    // Model init test
    Model    test("Assets/ghost.obj");
    ModelLOD boidsModel({"Assets/ghost.obj", "Assets/Star-LOD2.obj"});
    Model    ground("Assets/cube3.obj");

    // light
    //********************************************************************************************
    LightManager lightManager;
    lightManager.addPointLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 10.0f);
    lightManager.addPointLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 10.0f);
    lightManager.addPointLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 10.0f);
    lightManager.addDirectionalLight(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(1.0f, 0.0f, 0.0f), 100.0f);

    // playerLight must be declared after all other lights
    lightManager.addPointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(.0f, 0.0f, 1.0f), 10.0f);

    const std::vector<Light*>& lights = lightManager.getLights();
    int                        i      = 0;
    for (const auto& light : lights)
    {
        if (light->getType() == LightType::Ponctual)
        {
            const PointLight* pointLight  = static_cast<const PointLight*>(light);
            glm::vec3         lightPos_ws = pointLight->m_position;
            myShaders.set("uLightType[" + std::to_string(i) + "]", 0);
            myShaders.set("uLightPos_ws[" + std::to_string(i) + "]", lightPos_ws);
        }
        else if (light->getType() == LightType::Directional)
        {
            const DirectionalLight* directionalLight = static_cast<const DirectionalLight*>(light);
            myShaders.set("uLightType[" + std::to_string(i) + "]", 1);
            myShaders.set("uLightDir_ws[" + std::to_string(i) + "]", directionalLight->m_direction);
        }
        myShaders.set("uLightColor[" + std::to_string(i) + "]", light->m_color);
        myShaders.set("uLightIntensity[" + std::to_string(i) + "]", light->m_intensity);
        i++;
    }

    // camera controls checker
    bool Z     = false;
    bool Q     = false;
    bool S     = false;
    bool D     = false;
    bool SPACE = false;
    bool CTRL  = false;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ModelControls playerControl = playerControls(camera.getCamPosition(), camera.getCamFrontVector());

        ModelControls control{glm::vec3(0.f, -3.f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};

        glClearColor(1.000f, 0.662f, 0.970f, 1.000f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 playerLight = vec(playerControl.position.x, playerControl.position.y + 1.f, playerControl.position.z);
        myShaders.set("uLightPos_ws[" + std::to_string(lightManager.nb_lights - 1) + "]", playerLight);

        myShaders.use();
        myShaders.set("uUseTexture", false);
        glm::mat4  ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4  ViewMatrix = camera.getViewMatrix();
        const auto cameraPos  = camera.getCamPosition();

        // camera moving
        if (Z)
            camera.moveFront(0.1f);
        if (Q)
            camera.moveLeft(0.1);
        if (S)
            camera.moveFront(-0.1);
        if (D)
            camera.moveLeft(-0.1);
        if (SPACE)
            camera.moveUp(0.1);
        if (CTRL)
            camera.moveDown(0.1);

        const std::vector<ModelControls> boidControls = BoidsControls(boids, cameraPos);

        test.modelDraw(myShaders, ViewMatrix, playerControl, ProjMatrix);

        ground.modelDraw(myShaders, ViewMatrix, control, ProjMatrix);

        for (auto& b : boids)
        {
            b.flock(boids, ctx, intensities); // updating the boids here
        }

        for (auto const& boid : boidControls)
        {
            boidsModel.modelLODDraw(myShaders, ViewMatrix, boid, ProjMatrix);
        }
        myShaders.set("uUseTexture", true);
        myShaders.set("uTexture", 1);
        aTexture.activateTexture(1);

        // skybox drawcall
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        auto view = glm::mat4(glm::mat3(camera.getViewMatrix()));
        skyboxShader.set("viewMatrix", view);
        skyboxShader.set("projectionMatrix", ProjMatrix);
        glBindVertexArray(skyboxVAO.getId());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex.getId());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
    };

    ctx.key_pressed = [&Z, &Q, &S, &D, &SPACE, &CTRL](const p6::Key& key) {
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
        if (key.physical == GLFW_KEY_SPACE)
        {
            SPACE = true;
        }
        if (key.physical == GLFW_KEY_LEFT_CONTROL)
        {
            CTRL = true;
        }
    };

    ctx.key_released = [&Z, &Q, &S, &D, &SPACE, &CTRL](const p6::Key& key) {
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
        if (key.physical == GLFW_KEY_SPACE)
        {
            SPACE = false;
        }
        if (key.physical == GLFW_KEY_LEFT_CONTROL)
        {
            CTRL = false;
        }
    };

    ctx.mouse_dragged = [&](p6::MouseDrag drag) {
        camera.rotateLeft(drag.delta.x * 25.f);
        camera.rotateUp(-drag.delta.y * 25.f);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}