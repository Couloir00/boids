// shadow rendering from
// https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/3.1.3.shadow_mapping/shadow_mapping.cpp
//

#include "Shadows/Shadow.hpp"
#include "Framebuff/Framebuffer.hpp"
#include "Model/Model.hpp"
#include "Model/ModelControls.hpp"
#include "Model/ModelLod.hpp"

Shadow::Shadow(std::array<std::string, 2> shadowShaderPaths, const int& shadowWidth, const int& shadowHeight)
    : m_shadowBuffer(FrameBuffer(shadowWidth, shadowHeight)), m_shadowShader(p6::load_shader(shadowShaderPaths[0], shadowShaderPaths[1]))
{
}

void Shadow::shadowRenderingModelLOD(ModelLOD& shadowLODModeler, const glm::mat4& projectionMatrix, const glm::mat4 lightSpaceMatrix, const std::vector<ModelControls>& shadowControls, const p6::Shader& shader, const p6::Context& ctx, bool& lodsEnabled)
{
    m_shadowShader.use();
    m_shadowShader.set("LightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, m_shadowBuffer.getShadowWidth(), m_shadowBuffer.getShadowHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer.getId());
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_shadowBuffer.getTexId());
    shader.set("shadowMap", 0);
    for (const auto& shadowControl : shadowControls)
    {
        shadowLODModeler.modelLODDraw(m_shadowShader, lightSpaceMatrix, shadowControl, projectionMatrix, lodsEnabled);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, ctx.main_canvas_width(), ctx.main_canvas_height()); // reset viewport
}

void Shadow::shadowRenderingModel(Model& shadowModeler, const glm::mat4& projectionMatrix, const glm::mat4 lightSpaceMatrix, const ModelControls& shadowControls, const p6::Shader& shader, const p6::Context& ctx)
{
    m_shadowShader.use();
    m_shadowShader.set("LightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, m_shadowBuffer.getShadowWidth(), m_shadowBuffer.getShadowHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowBuffer.getId());
    glClear(GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_shadowBuffer.getTexId());
    shader.set("shadowMap", 0);
    shadowModeler.modelDraw(m_shadowShader, lightSpaceMatrix, shadowControls, projectionMatrix);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, ctx.main_canvas_width(), ctx.main_canvas_height()); // reset viewport
}
