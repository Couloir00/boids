#ifndef __APPINIT_HPP_
#define __APPINIT_HPP_

#include "App/App.hpp"

inline void App::init()
{
    initCtx();
    createBoids();
    initIntensities();
    initShaders();
}

inline void App::initCtx()
{
    m_ctx = p6::Context{{.title = "MagicBoids"}};
    m_ctx.maximize_window();
}

inline void App::createBoids()
{
    ///////////////Boids creation//////////////
    int numberBoids = 50;
    // emplacing boids
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

inline void App::initShaders()
{
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // shaders
    m_myShaders = p6::load_shader("Shaders/3D.vs.glsl", "Shaders/Light.fs.glsl");
    m_myShaders.use();
    m_texShaders = p6::load_shader("Shaders/Tex.vs.glsl", "Shaders/normals.fs.glsl");
    m_texShaders.use();
    m_skyboxShader = p6::load_shader("Shaders/SkyboxEnv.vs.glsl", "Shaders/SkyboxEnv.fs.glsl");
}

#endif