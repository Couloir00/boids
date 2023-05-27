#ifndef __APP_HPP_
#define __APP_HPP_

#include <vector>
#include "Boid/boid.hpp"
#include "Camera/FreeflyCamera.hpp"
#include "Intensity/Intensity.hpp"
#include "Lights/LightManager.hpp"
#include "Texture/Texture.hpp"
#include "img/src/Image.h"

class App {
public:
    App() = default;
    void init();
    void update();

private:
    // Init
    void initCtx();
    void createBoids();
    void initIntensities();
    void initShaders();
    void initCamera();
    void initTextures();
    void initShadow();
    void initLights();
    void initSkybox();

    // Update

    void keyEvent();
    void updateBoids();

private:
    p6::Context       m_ctx;
    Intensity         m_intensities;
    std::vector<Boid> boids;
    p6::Shader        m_myShaders{""}, m_texShaders{""}, m_skyboxShader{""};
    FreeflyCamera     m_camera;
    img::Image        m_Img{p6::load_image_buffer("Assets/wood.jpg")};
    LightManager      m_lightManager;
};

#endif