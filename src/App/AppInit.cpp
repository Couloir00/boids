#include "App/App.hpp"

App::App(int window_width, int window_height)
    : m_ctx(p6::Context{{window_width, window_height, "GraveyardMystery"}})
{
    m_ctx.maximize_window();
}

void App::init()
{
    createBoids();
    initIntensities();
    initDepth();
    initCamera();
    initTextures();
    initLights();
    initSkybox();
}

inline void App::createBoids()
{
    int numberBoids = 50;
    boids.reserve(numberBoids);
    for (int i = 0; i < numberBoids; i++)
    {
        boids.emplace_back(
            glm::vec3(p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f), p6::random::number(-1.f, 1.f)),
            glm::vec3(100000.f), 0.5f
        );
    }
}

inline void App::initIntensities()
{
    m_intensities = {0.5f, 0.5f, 0.5f, 0.5f};
    m_intensities.GUI(m_ctx);
}

inline void App::initDepth()
{
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

inline void App::initCamera()
{
    m_camera.setCamPosition(glm::vec3(0.0f, -2.0f, 25.0f));
}

inline void App::initTextures()
{
    m_TexWood.initTexture(static_cast<int>(m_Wood.width()), static_cast<int>(m_Wood.height()), m_Wood.data(), GL_RGBA, GL_UNSIGNED_BYTE);
    m_TextRocks.initTexture(static_cast<int>(m_Snow.width()), static_cast<int>(m_Snow.height()), m_Snow.data(), GL_RGBA, GL_UNSIGNED_BYTE);
}

inline void App::initLights()
{
    LightManager::initLighting(m_lightManager, m_myShaders);
}

inline void App::initSkybox()
{
    m_skybox.generateSkybox();
    m_skyboxShader.use();
    m_skyboxShader.set("skybox", 0);
}