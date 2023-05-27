#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#define DOCTEST_CONFIG_IMPLEMENT
#include <cstdlib>
#include <iostream>
#include <vector>
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
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "img/src/Image.h"
#include "imgui.h"
#include "p6/p6.h"

void runTests(int argc, char* argv[])
{
    if (doctest::Context{}.run() != 0)
        exit(EXIT_FAILURE);

    const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0;
    if (no_gpu_available)
        exit(EXIT_SUCCESS);
}

void handleCameraControl(FreeflyCamera& camera, bool Z, bool Q, bool S, bool D, bool SPACE, bool CTRL)
{
    if (Z)
        camera.moveFront(0.1f);
    if (Q)
        camera.moveLeft(0.1f);
    if (S)
        camera.moveFront(-0.1f);
    if (D)
        camera.moveLeft(-0.1f);
    if (SPACE)
        camera.moveUp(0.1f);
    if (CTRL)
        camera.moveDown(0.1f);
}

int main(int argc, char* argv[])
{
    runTests(argc, argv);

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
    myShaders.use();
    const p6::Shader texShaders = p6::load_shader("Shaders/Tex.vs.glsl", "Shaders/normals.fs.glsl");
    texShaders.use();

    // camera
    FreeflyCamera camera;
    camera.setCamPosition(glm::vec3(0.0f, -2.0f, 25.0f));
    img::Image aTex = p6::load_image_buffer("Assets/wood.jpg");
    Texture    aTexture{};
    aTexture.initTexture(static_cast<int>(aTex.width()), static_cast<int>(aTex.height()), aTex.data(), GL_RGBA, GL_UNSIGNED_BYTE);

    // shadow test
    Shadow shadows({"Shaders/mapping.vs.glsl", "Shaders/mapping.fs.glsl"}, ctx.main_canvas_width(), ctx.main_canvas_height());

    Texture                 cubemapTex;
    std::vector<img::Image> Images;
    p6::Shader              skyboxShader = p6::load_shader("Shaders/SkyboxEnv.vs.glsl", "Shaders/SkyboxEnv.fs.glsl");
    VAO                     skyboxVAO;
    VBO                     skyboxVBO;
    generateSkybox(cubemapTex, Images, skyboxVAO, skyboxVBO);
    skyboxShader.use();
    skyboxShader.set("skybox", 0);

    // Model init test
    Model test("Assets/ghostLow.obj");
    Model ground("Assets/Ground.obj");
    Model manor("Assets/SpookyHouseHigh.obj");
    Model cave("Assets/caveLow.obj");
    Model tree("Assets/BigTreeHigh.obj");
    Model tree2("Assets/SmallTreeHigh.obj");
    Model fence("Assets/Fence.obj");

    ModelLOD boidsModel({"Assets/starLow.obj", "Assets/starHigh.obj"});

    // light
    //********************************************************************************************
    LightManager lightManager;
    lightManager.addPointLight(glm::vec3(-1.0f, 5.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f), 100.0f);
    lightManager.addPointLight(glm::vec3(3.5f, -.2f, -0.4f), glm::vec3(0.0f, 0.5f, 1.0f), 5.0f);
    lightManager.addPointLight(glm::vec3(-5.0f, 0.0f, -8.0f), glm::vec3(0.0f, 1.0f, .7f), 2.0f);
    lightManager.addDirectionalLight(glm::vec3(-2.0f, 30.0f, 7.0f), glm::vec3(.2f, .5f, .5f), 2.0f);

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
            // std::cout << pointLight[0].getPosition().y << "\n";
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
    glm::vec3 lightPos(-1.0f, 5.0f, 0.0f); // pos of first pointlight in the manager
    // camera controls checker
    bool Z     = false;
    bool Q     = false;
    bool S     = false;
    bool D     = false;
    bool SPACE = false;
    bool CTRL  = false;
    bool P     = false;

    // character position limits
    float minX = -29.0f;
    float maxX = 29.0f;
    float minZ = -28.5f;
    float maxZ = 30.5f;
    float minY = -2.2f;
    float maxY = 14.f;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ModelControls playerControl = playerControls(camera.getCamPosition(), camera.getCamFrontVector());

        ModelControls groundControl{glm::vec3(0.f, -3.f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
        ModelControls manorControl{glm::vec3(2.f, -2.65f, 4.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
        ModelControls caveControl{glm::vec3(-5.f, -2.5f, -8.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
        ModelControls treeControl{glm::vec3(0.f, -5.f, -.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
        ModelControls tree2Control{glm::vec3(-3.f, -5.5f, 15.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
        ModelControls fenceControl{glm::vec3(0.f, -5.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
        // ModelControls crossGraveControl{glm::vec3(8.f, -2.5f, 9.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_HIGH};

        glm::vec3 playerLight = vec(playerControl.position.x, playerControl.position.y + 1.f, playerControl.position.z);

        glm::mat4  ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4  ViewMatrix = camera.getViewMatrix();
        const auto cameraPos  = camera.getCamPosition();

        const std::vector<ModelControls> boidControls = BoidsControls(boids, cameraPos);

        // shadow

        glm::mat4       lightView        = glm::lookAt(lightPos, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
        const glm::mat4 lightProjection  = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4       lightSpaceMatrix = lightProjection * lightView;
        shadows.shadowRendering(boidsModel, ProjMatrix, lightSpaceMatrix, boidControls, myShaders, ctx);
        // shadows.shadowRendering(ma, ProjMatrix, lightSpaceMatrix, boidControls, myShaders, ctx);

        handleCameraControl(camera, Z, Q, S, D, SPACE, CTRL);
        // camera moving
        if (P)
        {
            std::cout << "x = " << playerControl.position.x << std::endl;
            std::cout << "y = " << playerControl.position.y << std::endl;
            std::cout << "z = " << playerControl.position.z << std::endl;
        }

        //*****************************************************************************************
        // Vérification des limites de position
        if (camera.getCamPosition().x < minX)
            camera.setCamPosition(glm::vec3(minX, camera.getCamPosition().y, camera.getCamPosition().z));
        if (camera.getCamPosition().x > maxX)
            camera.setCamPosition(glm::vec3(maxX, camera.getCamPosition().y, camera.getCamPosition().z));
        if (camera.getCamPosition().z < minZ)
            camera.setCamPosition(glm::vec3(camera.getCamPosition().x, camera.getCamPosition().y, minZ));
        if (camera.getCamPosition().z > maxZ)
            camera.setCamPosition(glm::vec3(camera.getCamPosition().x, camera.getCamPosition().y, maxZ));

        if (camera.getCamPosition().y < minY)

            camera.setCamPosition(glm::vec3(camera.getCamPosition().x, minY, camera.getCamPosition().z));

        if (camera.getCamPosition().y > maxY)
            camera.setCamPosition(glm::vec3(camera.getCamPosition().x, maxY, camera.getCamPosition().z));

        //*****************************************************************************************

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        myShaders.use();
        myShaders.set("LightSpaceMatrix", lightSpaceMatrix);
        myShaders.set("uLightPos_ws[" + std::to_string(lightManager.nb_lights - 1) + "]", playerLight);
        myShaders.set("uUseTexture", false);

        // drawcalls
        test.modelDraw(myShaders, ViewMatrix, playerControl, ProjMatrix);
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

        // crossGraveModel.modelLODDraw(myShaders, ViewMatrix, crossGraveControl, ProjMatrix);

        for (auto& b : boids)
        {
            b.flock(boids, ctx, intensities); // updating the boids here
        }

        for (auto const& boidControl : boidControls)
        {
            boidsModel.modelLODDraw(myShaders, ViewMatrix, boidControl, ProjMatrix);
        }

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

    ctx.key_pressed = [&Z, &Q, &S, &D, &SPACE, &CTRL, &P](const p6::Key& key) {
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
        if (key.physical == GLFW_KEY_P)
        {
            P = true;
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

    ctx.key_released = [&Z, &Q, &S, &D, &SPACE, &CTRL, &P](const p6::Key& key) {
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
        if (key.physical == GLFW_KEY_P)
        {
            P = false;
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