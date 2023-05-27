#ifndef __SKYBOXENV_HPP_
#define __SKYBOXENV_HPP_
#include <vector>
#include "Camera/FreeflyCamera.hpp"
#include "Texture/Texture.hpp"
#include "VAO/VAO.hpp"
#include "VBO/VBO.hpp"
#include "glm/fwd.hpp"

class Skybox {
private:
    Texture                 m_cubemapTex;
    std::vector<img::Image> m_faces;
    VAO                     m_skyboxVAO;
    VBO                     m_skyboxVBO;

public:
    Skybox() = default;
    void generateSkybox();
    void drawSkybox(p6::Shader& skyboxShader, FreeflyCamera& camera, const glm::mat4& projectionMatrix);
};

#endif
