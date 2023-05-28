#ifndef __APP_HPP_
#define __APP_HPP_

#define DOCTEST_CONFIG_IMPLEMENT
#include <vector>
#include "Boid/boid.hpp"
#include "Camera/FreeflyCamera.hpp"
#include "GUI/GUI.hpp"
#include "Intensity/Intensity.hpp"
#include "Lights/LightManager.hpp"
#include "Model/ModelControls.hpp"
#include "Model/ModelLod.hpp"
#include "Shadows/Shadow.hpp"
#include "SkyboxEnv/SkyboxEnv.hpp"
#include "Texture/Texture.hpp"
#include "doctest/parts/doctest_fwd.h"
#include "glm/fwd.hpp"
#include "img/src/Image.h"

class App {
public:
    explicit App(int window_width = 1920, int window_height = 1080);
    void init();
    void update();

private:
    // Init
    void        createBoids();
    void        initIntensities();
    static void initDepth();
    void        initCamera();
    void        initTextures();
    void        initLights();
    void        initSkybox();

    // Update
    void updateMatrix();
    void updateControls();
    void updatePlayerLight();
    void updateShadows();
    void updateCamera();
    void updateRender();
    void updateBoids();
    void renderBoids();
    void renderSkybox();
    void renderModels();
    void cameraActionsEvents();
    void showGUI();

private:
    p6::Context       m_ctx;
    Intensity         m_intensities;
    std::vector<Boid> boids;
    p6::Shader        m_myShaders = p6::load_shader("Shaders/3D.vs.glsl", "Shaders/Light.fs.glsl"),
               m_texShaders       = p6::load_shader("Shaders/Tex.vs.glsl", "Shaders/normals.fs.glsl"),
               m_skyboxShader     = p6::load_shader("Shaders/SkyboxEnv.vs.glsl", "Shaders/SkyboxEnv.fs.glsl");
    FreeflyCamera              m_camera;
    img::Image                 m_Img{p6::load_image_buffer("Assets/wood.jpg")};
    LightManager               m_lightManager;
    Texture                    m_Texture{};
    Shadow                     m_shadows{{"Shaders/mapping.vs.glsl", "Shaders/mapping.fs.glsl"}, m_ctx.main_canvas_width(), m_ctx.main_canvas_height()};
    Skybox                     m_skybox;
    glm::mat4                  m_ProjMatrix{};
    glm::mat4                  m_ViewMatrix{};
    glm::mat4                  m_lightSpaceMatrix{};
    std::vector<ModelControls> m_boidControls;
    ModelControls              m_playerControls{};
    glm::vec3                  m_playerLight{};
    ModelLOD                   m_boidsModel{{"Assets/starLow.obj", "Assets/starHigh.obj"}};

    // GUI
    bool m_lodsEnabled = true;
    bool m_fogEnabled  = true;

    // scene models
    Model m_player{"Assets/ghostLow.obj"};
    Model m_ground{"Assets/Ground.obj"};
    Model m_manor{"Assets/SpookyHouseHigh.obj"};
    Model m_cave{"Assets/caveLow.obj"};
    Model m_tree{"Assets/BigTreeHigh.obj"};
    Model m_tree2{"Assets/SmallTreeHigh.obj"};
    Model m_fence{"Assets/Fence.obj"};

    // scene models controls

    ModelControls m_groundControl{glm::vec3(0.f, -3.f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_manorControl{glm::vec3(2.f, -2.65f, 4.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_caveControl{glm::vec3(-5.f, -2.5f, -8.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_treeControl{glm::vec3(0.f, -5.f, -.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_tree2Control{glm::vec3(-3.f, -5.5f, 15.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_fenceControl{glm::vec3(0.f, -5.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
};

inline void runTestsAndCheckGPU(int argc, char* argv[])
{
    if (doctest::Context{}.run() != 0)
        exit(EXIT_FAILURE);

    const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0;
    if (no_gpu_available)
        exit(EXIT_SUCCESS);
}

#endif