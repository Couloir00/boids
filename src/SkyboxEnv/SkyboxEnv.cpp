#include "SkyboxEnv/SkyboxEnv.hpp"
#include <string>
#include <vector>

const std::array skyboxVertices =
    {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

void Skybox::generateSkybox()
{
    std::vector<std::string> cubemapFaces = {
        "Textures/SkyboxFaces/right.png",
        "Textures/SkyboxFaces/left.png",
        "Textures/SkyboxFaces/top.png",
        "Textures/SkyboxFaces/bottom.png",
        "Textures/SkyboxFaces/front.png",
        "Textures/SkyboxFaces/back.png"};
    for (auto& cubemapFace : cubemapFaces)
    {
        m_faces.emplace_back(p6::load_image_buffer("Assets/" + cubemapFace));
    }

    m_cubemapTex.cubemapTexture(m_faces);
    glBindVertexArray(m_skyboxVAO.getId());
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO.getId());
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
}

void Skybox::drawSkybox(p6::Shader& skyboxShader, FreeflyCamera& camera, const glm::mat4& projectionMatrix)
{
    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    auto view = glm::mat4(glm::mat3(camera.getViewMatrix()));
    skyboxShader.set("viewMatrix", view);
    skyboxShader.set("projectionMatrix", projectionMatrix);
    glBindVertexArray(m_skyboxVAO.getId());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTex.getId());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}