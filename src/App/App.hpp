#ifndef __APP_HPP_
#define __APP_HPP_

#define DOCTEST_CONFIG_IMPLEMENT
#include <vector>
#include "Boid/boid.hpp"
#include "Camera/FreeflyCamera.hpp"
#include "GUI/GUI.hpp"
#include "Intensity/Intensity.hpp"
#include "LODModel/LODModel.hpp"
#include "Lights/LightManager.hpp"
#include "Model/Model.hpp"
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
    void updateLODModels();
    void renderBoids();
    void renderSkybox();
    void renderModels();
    void cameraActionsEvents();
    void showGUI();

private:
    p6::Context       m_ctx;
    Intensity         m_intensities;
    std::vector<Boid> boids;

    // Shaders
    p6::Shader m_myShaders    = p6::load_shader("Shaders/3D.vs.glsl", "Shaders/Light.fs.glsl"),
               m_skyboxShader = p6::load_shader("Shaders/SkyboxEnv.vs.glsl", "Shaders/SkyboxEnv.fs.glsl");

    FreeflyCamera m_camera;
    // Images for textures
    img::Image m_Wood{p6::load_image_buffer("Assets/Textures/wood.jpg")};
    img::Image m_Snow{p6::load_image_buffer("Assets/Textures/snow.png")};
    img::Image m_Stone{p6::load_image_buffer("Assets/Textures/graveTex.jpg")};

    // Actual texture generation
    Texture m_TexWood{}, m_TexSnow{}, m_TexStone{};

    LightManager m_lightManager;

    Shadow m_shadows{{"Shaders/mapping.vs.glsl", "Shaders/mapping.fs.glsl"}, m_ctx.main_canvas_width(), m_ctx.main_canvas_height()};
    Skybox m_skybox;

    // Matrix
    glm::mat4 m_ProjMatrix{};
    glm::mat4 m_ViewMatrix{};
    glm::mat4 m_lightSpaceMatrix{};
    glm::vec3 m_playerLight{};

    // GUI
    bool  m_lodsEnabled  = true;
    bool  m_fogEnabled   = true;
    float m_fogIntensity = 0.005f;
    float m_fogRed = 0.074f, m_fogGreen = 0.305f, m_fogBlue = 0.391f;

    // scene models
    Model m_boidsModel{"Assets/starLow.obj"};
    Model m_player{"Assets/ghostLow.obj"};
    Model m_ground{"Assets/Ground.obj"};
    Model m_manor{"Assets/SpookyHouseHigh.obj"};
    Model m_spookyFence{"Assets/SpookyFence.obj"};
    Model m_cave{"Assets/caveHigh.obj"};
    Model m_fence{"Assets/Fence.obj"};

    Model m_tree{"Assets/BigTreeHigh.obj"};
    Model m_tree2{"Assets/SmallTreeHigh.obj"};
    Model m_tree3{"Assets/BigTreeHigh.obj"};
    Model m_tree4{"Assets/SmallTreeHigh.obj"};

    Model m_grave{"Assets/graveHigh.obj"};
    Model m_grave2{"Assets/crossGraveHigh.obj"};
    Model m_grave3{"Assets/cross.obj"};
    Model m_grave4{"Assets/graves.obj"};

    Model m_rock{"Assets/Rock2.obj"};

    ModelLOD m_boidsLodModel{{"Assets/starLow.obj", "Assets/starHigh.obj"}};
    ModelLOD m_Lodtree{{"Assets/BigTreeLow.obj", "Assets/BigTreeHigh.obj"}};
    ModelLOD m_LodHouse{{"Assets/SpookyHouseLow.obj", "Assets/SpookyHouseHigh.obj"}};

    // scene models controls
    std::vector<ModelControls> m_boidControls;
    ModelControls              m_playerControls{};
    ModelControls              m_groundControl{glm::vec3(0.f, -3.f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls              m_manorControl{glm::vec3(5.f, -2.7f, 4.0f), glm::vec3(-5.f, -.1f, 90.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls              m_spookyFenceControl{glm::vec3(5.f, -2.5f, 4.0f), glm::vec3(-5.f, -.1f, 90.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls              m_caveControl{glm::vec3(-5.f, -2.5f, -8.0f), glm::vec3(1.f, -0.6f, -36.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls              m_fenceControl{glm::vec3(0.f, -5.0f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};

    ModelControls m_treeControl{glm::vec3(0.f, -5.f, -.5f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_tree2Control{glm::vec3(-3.f, -5.5f, 15.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_tree3Control{glm::vec3(-17.f, -6.f, -4.7f), glm::vec3(1.f, 0.f, -83.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_tree4Control{glm::vec3(19.f, -5.5f, -15.0f), glm::vec3(1.f, 0.f, -53.f), glm::vec3(0.f), 1.f, LOD_LOW};

    ModelControls m_graveControl{glm::vec3(-17.f, -3.2f, 5.0f), glm::vec3(1.f, 2.f, -37.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_grave2Control{glm::vec3(-14.f, -3.2f, -16.0f), glm::vec3(1.f, 4.3f, -90.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_grave3Control{glm::vec3(0.f, -3.2f, 0.f), glm::vec3(1.f, 0.f, -87.f), glm::vec3(0.f), 1.f, LOD_LOW};
    ModelControls m_grave4Control{glm::vec3(0.f, -3.1f, 0.0f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), 1.f, LOD_LOW};
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