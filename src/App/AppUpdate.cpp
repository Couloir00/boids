#include "App/App.hpp"

void App::update()
{
    m_ctx.update = [&]() {
        updateMatrix();
        updateControls();
        updatePlayerLight();
        updateShadows();
        updateCamera();
        updateRender();
        updateBoids();
        renderBoids();
        renderSkybox();
        renderModels();
    };
    cameraActionsEvents();
    m_ctx.start();
}

inline void App::updateMatrix()
{
    m_ProjMatrix = glm::perspective(glm::radians(70.f), m_ctx.aspect_ratio(), 0.1f, 100.f);
    m_ViewMatrix = m_camera.getViewMatrix();

    glm::mat4       lightView       = glm::lookAt(m_lightManager.getShadowLight(), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
    const glm::mat4 lightProjection = glm::perspective(glm::radians(70.f), m_ctx.aspect_ratio(), 0.1f, 100.f);
    m_lightSpaceMatrix              = lightProjection * lightView;
}

inline void App::updateControls()
{
    m_boidControls   = BoidsControls(boids, m_camera.getCamPosition());
    m_playerControls = playerControls(m_camera.getCamPosition(), m_camera.getCamFrontVector());
}

inline void App::updatePlayerLight()
{
    m_playerLight = glm::vec3(m_playerControls.position.x, m_playerControls.position.y + 1.f, m_playerControls.position.z);
}

inline void App::updateShadows()
{
    m_shadows.shadowRenderingModelLOD(m_boidsModel, m_ProjMatrix, m_lightSpaceMatrix, m_boidControls, m_myShaders, m_ctx);
}

inline void App::updateCamera()
{
    m_camera.keyboardEvents(m_camera, m_playerControls);
    m_camera.limitCamera(m_camera);
}

inline void App::updateRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_myShaders.use();
    m_myShaders.set("LightSpaceMatrix", m_lightSpaceMatrix);
    m_myShaders.set("uLightPos_ws[" + std::to_string(m_lightManager.nb_lights - 1) + "]", m_playerLight);
    m_myShaders.set("uUseTexture", false);
}

inline void App::updateBoids()
{
    for (auto& boid : boids)
    {
        boid.flock(boids, m_ctx, m_intensities); // updating the boids here
    }
}

inline void App::renderBoids()
{
    for (auto const& m_boidControl : m_boidControls)
    {
        m_boidsModel.modelLODDraw(m_myShaders, m_ViewMatrix, m_boidControl, m_ProjMatrix);
    }
}

inline void App::renderSkybox()
{
    m_skybox.drawSkybox(m_skyboxShader, m_camera, m_ProjMatrix);
}

inline void App::renderModels()
{
    m_player.modelDraw(m_myShaders, m_ViewMatrix, m_playerControls, m_ProjMatrix);
    m_ground.modelDraw(m_myShaders, m_ViewMatrix, m_groundControl, m_ProjMatrix);
    m_myShaders.set("uUseTexture", true);
    m_myShaders.set("uTexture", 1);
    m_Texture.activateTexture(1);
    m_manor.modelDraw(m_myShaders, m_ViewMatrix, m_manorControl, m_ProjMatrix);

    m_myShaders.set("uUseTexture", false);
    m_cave.modelDraw(m_myShaders, m_ViewMatrix, m_caveControl, m_ProjMatrix);

    m_myShaders.set("uUseTexture", true);
    m_myShaders.set("uTexture", 1);
    m_Texture.activateTexture(1);
    m_tree.modelDraw(m_myShaders, m_ViewMatrix, m_treeControl, m_ProjMatrix);
    m_tree2.modelDraw(m_myShaders, m_ViewMatrix, m_tree2Control, m_ProjMatrix);

    m_myShaders.set("uUseTexture", false);
    m_fence.modelDraw(m_myShaders, m_ViewMatrix, m_fenceControl, m_ProjMatrix);
}

inline void App::cameraActionsEvents()
{
    m_camera.actionEvents(m_ctx, m_camera);
}