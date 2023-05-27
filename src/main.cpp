#define DOCTEST_CONFIG_IMPLEMENT
#include <cstdlib>
#include <iostream>
#include <vector>
#include "App/App.hpp"
#include "App/AppInit.hpp"
#include "Boid/boid.hpp"
#include "Camera/FreeflyCamera.hpp"
#include "Framebuff/Framebuffer.hpp"
#include "Intensity/Intensity.hpp"
#include "LODModel/LODModel.hpp"
#include "Lights/LightManager.hpp"
#include "Model/Model.hpp"
#include "Model/ModelControls.hpp"
#include "Model/ModelLod.hpp"
#include "Shadows/Shadow.hpp"
#include "SkyboxEnv/SkyboxEnv.hpp"
#include "Sphere/sphere_vertices.hpp"
#include "Texture/Texture.hpp"
#include "doctest/doctest.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"

void runTests(int argc, char* argv[])
{
    if (doctest::Context{}.run() != 0)
        exit(EXIT_FAILURE);

    const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0;
    if (no_gpu_available)
        exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    // runTests(argc, argv);

    if (doctest::Context{}.run() != 0)
        return EXIT_FAILURE;

    const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0;
    if (no_gpu_available)
        return EXIT_SUCCESS;

    // Actual app
    auto ctx = p6::Context{{.title = "MagicBoids"}};
    ctx.maximize_window();

    ///////////////Boids creation//////////////

    // vector of numberBoids boids
    static std::vector<Boid> boids;
    int                      numberBoids = 50;
    // emplacing boids
    boids.reserve(numberBoids);
    for (int i = 0; i < numberBoids; i++)
    {
        boids.emplace_back(
            glm::vec3(p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)),
            glm::vec3(100000.f), 0.5f
        );
    }

    // boids intensities
    Intensity intensities{0.5f, 0.5f, 0.5f, 0.5f};

    // ImGui context
    intensities.GUI(ctx);

    /// INIT///
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // shaders
    const p6::Shader myShaders = p6::load_shader("Shaders/3D.vs.glsl", "Shaders/Light.fs.glsl");
    myShaders.use();
    const p6::Shader texShaders = p6::load_shader("Shaders/Tex.vs.glsl", "Shaders/normals.fs.glsl");
    texShaders.use();
    p6::Shader skyboxShader = p6::load_shader("Shaders/SkyboxEnv.vs.glsl", "Shaders/SkyboxEnv.fs.glsl");

    // camera
    FreeflyCamera camera;
    camera.setCamPosition(glm::vec3(0.0f, -2.0f, 25.0f));

    img::Image aTex = p6::load_image_buffer("Assets/wood.jpg");
    Texture    aTexture{};
    aTexture.initTexture(static_cast<int>(aTex.width()), static_cast<int>(aTex.height()), aTex.data(), GL_RGBA, GL_UNSIGNED_BYTE);

    // shadow test
    Shadow shadows({"Shaders/mapping.vs.glsl", "Shaders/mapping.fs.glsl"}, ctx.main_canvas_width(), ctx.main_canvas_height());

    // Lighting
    LightManager lightManager;
    LightManager::initLighting(lightManager, myShaders);

    // Models init
    Model    player("Assets/ghostLow.obj");
    Model    ground("Assets/Ground.obj");
    Model    manor("Assets/SpookyHouseHigh.obj");
    Model    cave("Assets/caveLow.obj");
    Model    tree("Assets/BigTreeHigh.obj");
    Model    tree2("Assets/SmallTreeHigh.obj");
    Model    fence("Assets/Fence.obj");
    ModelLOD boidsModel({"Assets/starLow.obj", "Assets/starHigh.obj"});

    // Scene Models controls
    ModelControls groundControl{glm::vec3(0.f, -3.f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls manorControl{glm::vec3(2.f, -2.65f, 4.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls caveControl{glm::vec3(-5.f, -2.5f, -8.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls treeControl{glm::vec3(0.f, -5.f, -.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls tree2Control{glm::vec3(-3.f, -5.5f, 15.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls fenceControl{glm::vec3(0.f, -5.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};

    // Skybox

    Skybox skybox;
    skybox.generateSkybox();
    skyboxShader.use();
    skyboxShader.set("skybox", 0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // Matrix and camera postion
        glm::mat4  ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4  ViewMatrix = camera.getViewMatrix();
        const auto cameraPos  = camera.getCamPosition();

        // Controls for boids
        const std::vector<ModelControls> boidControls = BoidsControls(boids, cameraPos);
        // controls for player
        ModelControls playerControl = playerControls(camera.getCamPosition(), camera.getCamFrontVector());
        glm::vec3     playerLight   = vec(playerControl.position.x, playerControl.position.y + 1.f, playerControl.position.z);

        // shadow from light POV
        glm::mat4       lightView        = glm::lookAt(lightManager.getShadowLight(), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
        const glm::mat4 lightProjection  = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4       lightSpaceMatrix = lightProjection * lightView;
        shadows.shadowRenderingModelLOD(boidsModel, ProjMatrix, lightSpaceMatrix, boidControls, myShaders, ctx);
        // shadows.shadowRenderingModel(tree2, ProjMatrix, lightSpaceMatrix, manorControl, myShaders, ctx); // disablying other shadows if we add another render

        // camera event
        camera.keyboardEvents(camera, playerControl);
        camera.limitCamera(camera);

        // Rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        myShaders.use();
        myShaders.set("LightSpaceMatrix", lightSpaceMatrix);
        myShaders.set("uLightPos_ws[" + std::to_string(lightManager.nb_lights - 1) + "]", playerLight);
        myShaders.set("uUseTexture", false);

        for (auto& b : boids)
        {
            b.flock(boids, ctx, intensities); // updating the boids here
        }

        for (auto const& boidControl : boidControls)
        {
            boidsModel.modelLODDraw(myShaders, ViewMatrix, boidControl, ProjMatrix);
        }

        // drawcalls
        player.modelDraw(myShaders, ViewMatrix, playerControl, ProjMatrix);
        ground.modelDraw(myShaders, ViewMatrix, groundControl, ProjMatrix);
        myShaders.set("uUseTexture", true);
        myShaders.set("uTexture", 1);
        aTexture.activateTexture(1);
        manor.modelDraw(myShaders, ViewMatrix, manorControl, ProjMatrix);

        myShaders.set("uUseTexture", false);
        cave.modelDraw(myShaders, ViewMatrix, caveControl, ProjMatrix);

        myShaders.set("uUseTexture", true);
        myShaders.set("uTexture", 1);
        aTexture.activateTexture(1);
        tree.modelDraw(myShaders, ViewMatrix, treeControl, ProjMatrix);
        tree2.modelDraw(myShaders, ViewMatrix, tree2Control, ProjMatrix);

        myShaders.set("uUseTexture", false);
        fence.modelDraw(myShaders, ViewMatrix, fenceControl, ProjMatrix);

        // skybox drawcall
        skybox.drawSkybox(skyboxShader, camera, ProjMatrix);
    };
    camera.actionEvents(ctx, camera);
    // Should be done last. It starts the infinite loop.
    ctx.start();
}