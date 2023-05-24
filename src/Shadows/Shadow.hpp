#ifndef __SHADOW_HPP_
#define __SHADOW_HPP_

#include <array>
#include <string>
#include "Framebuff/Framebuffer.hpp"
#include "Model/ModelControls.hpp"
#include "Model/ModelLod.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Shadow {
private:
    FrameBuffer m_shadowBuffer;
    p6::Shader  m_shadowShader;

public:
    Shadow(std::array<std::string, 2> shadowShaderPaths, const int& shadowWidth, const int& shadowHeight);

    void shadowRendering(ModelLOD& shadowModeler, const glm::mat4& projectionMatrix, const glm::mat4 lightSpaceMatrix, const std::vector<ModelControls>& shadowControls, const p6::Shader& shader, const p6::Context& ctx);
};

#endif