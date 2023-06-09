#include "App/App.hpp"
#include "LODModel/LODModel.hpp"
#include "glm/fwd.hpp"

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
        updateLODModels();
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
    m_playerLight = glm::vec3(m_playerControls.position.x, m_playerControls.position.y, m_playerControls.position.z);
}

inline void App::updateShadows()
{
    m_shadows.shadowRenderingModelLOD(m_boidsLodModel, m_ProjMatrix, m_lightSpaceMatrix, m_boidControls, m_myShaders, m_ctx);
}

inline void App::updateCamera()
{
    m_camera.keyboardEvents(m_camera);
    m_camera.limitCamera(m_camera);
}

inline void App::updateRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_myShaders.use();
    m_myShaders.set("LightSpaceMatrix", m_lightSpaceMatrix);
    m_myShaders.set("uLightPos_ws[" + std::to_string(m_lightManager.nb_lights - 1) + "]", m_playerLight);
    m_myShaders.set("uUseTexture", false);
    m_myShaders.set("uEnableFog", m_fogEnabled);
    m_myShaders.set("uFogIntensity", m_fogIntensity);
    m_myShaders.set("uRed", m_fogRed);
    m_myShaders.set("uGreen", m_fogGreen);
    m_myShaders.set("uBlue", m_fogBlue);
}

inline void App::updateBoids()
{
    for (auto& boid : boids)
    {
        boid.avoidEdges(boid);
        boid.flock(boids, m_ctx, m_intensities);
    }
}

inline void App::updateLODModels()
{
    m_treeControl.aLod  = updateLOD(m_camera.getCamPosition(), m_treeControl.position);
    m_manorControl.aLod = updateLOD(m_camera.getCamPosition(), m_manorControl.position);
}

inline void App::renderBoids()
{
    if (m_lodsEnabled)
    {
        for (auto const& m_boidControl : m_boidControls)
        {
            m_boidsLodModel.modelLODDraw(m_myShaders, m_ViewMatrix, m_boidControl, m_ProjMatrix);
        }
    }
    else
    {
        for (auto const& m_boidControl : m_boidControls)
        {
            m_boidsModel.modelDraw(m_myShaders, m_ViewMatrix, m_boidControl, m_ProjMatrix);
        }
    }
}

inline void App::renderSkybox()
{
    m_skybox.drawSkybox(m_skyboxShader, m_camera, m_ProjMatrix);
}

inline void App::renderModels()
{
    m_player.modelDraw(m_myShaders, m_ViewMatrix, m_playerControls, m_ProjMatrix);

    m_myShaders.set("uUseTexture", true);
    m_myShaders.set("uTexture", 1);
    m_TexSnow.activateTexture(1);
    m_ground.modelDraw(m_myShaders, m_ViewMatrix, m_groundControl, m_ProjMatrix);
    m_myShaders.set("uUseTexture", false);

    m_myShaders.set("uUseTexture", true);
    m_myShaders.set("uTexture", 1);
    m_TexWood.activateTexture(1);
    if (m_lodsEnabled)
    {
        m_Lodtree.modelLODDraw(m_myShaders, m_ViewMatrix, m_treeControl, m_ProjMatrix);
        m_LodHouse.modelLODDraw(m_myShaders, m_ViewMatrix, m_manorControl, m_ProjMatrix);
    }
    else
    {
        m_tree.modelDraw(m_myShaders, m_ViewMatrix, m_treeControl, m_ProjMatrix);
        m_manor.modelDraw(m_myShaders, m_ViewMatrix, m_manorControl, m_ProjMatrix);
    }
    m_tree2.modelDraw(m_myShaders, m_ViewMatrix, m_tree2Control, m_ProjMatrix);
    m_tree3.modelDraw(m_myShaders, m_ViewMatrix, m_tree3Control, m_ProjMatrix);
    m_tree4.modelDraw(m_myShaders, m_ViewMatrix, m_tree4Control, m_ProjMatrix);
    m_myShaders.set("uUseTexture", false);

    m_spookyFence.modelDraw(m_myShaders, m_ViewMatrix, m_spookyFenceControl, m_ProjMatrix);
    m_fence.modelDraw(m_myShaders, m_ViewMatrix, m_fenceControl, m_ProjMatrix);

    m_myShaders.set("uUseTexture", true);
    m_myShaders.set("uTexture", 1);
    m_TexSnow.activateTexture(1);

    m_grave.modelDraw(m_myShaders, m_ViewMatrix, m_graveControl, m_ProjMatrix);
    m_grave2.modelDraw(m_myShaders, m_ViewMatrix, m_grave2Control, m_ProjMatrix);
    m_grave3.modelDraw(m_myShaders, m_ViewMatrix, m_grave3Control, m_ProjMatrix);
    m_grave4.modelDraw(m_myShaders, m_ViewMatrix, m_grave4Control, m_ProjMatrix);
    m_cave.modelDraw(m_myShaders, m_ViewMatrix, m_caveControl, m_ProjMatrix);
    m_myShaders.set("uUseTexture", false);
}

inline void App::cameraActionsEvents()
{
    m_camera.actionEvents(m_ctx, m_camera);
}