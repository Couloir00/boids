#include "LightManager.hpp"
#include "Lights/LightManager.hpp"
#include "Sphere/common.hpp"
#include "glm/ext/vector_float3.hpp"

void LightManager::addPointLight(glm::vec3 position, glm::vec3 color, float intensity)
{
    auto* light = new PointLight();
    light->setPosition(position);
    light->m_color     = color;
    light->m_intensity = intensity;
    m_lights.push_back(light);
    nb_lights++;
}

void LightManager::addDirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity)
{
    auto* light        = new DirectionalLight();
    light->m_direction = direction;
    light->m_color     = color;
    light->m_intensity = intensity;
    m_lights.push_back(light);
    nb_lights++;
}

void LightManager::initLighting(LightManager& lightManager, const p6::Shader& myShaders)
{
    lightManager.addPointLight(glm::vec3(-1.0f, 5.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f), 100.0f);
    lightManager.addPointLight(glm::vec3(3.5f, -.2f, -0.4f), glm::vec3(0.0f, 0.5f, 1.0f), 5.0f);
    lightManager.addPointLight(glm::vec3(-5.0f, 0.0f, -8.0f), glm::vec3(0.0f, 1.0f, .7f), 2.0f);
    lightManager.addDirectionalLight(glm::vec3(-2.0f, 30.0f, 7.0f), glm::vec3(.2f, .5f, .5f), 2.0f);

    // playerLight must be declared after all other lights
    lightManager.addPointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(.0f, 0.0f, 1.0f), 10.0f);

    const std::vector<Light*>& lights = lightManager.getLights();
    int                        i      = 0;
    for (const auto& light : lights)
    {
        if (light->getType() == LightType::Ponctual)
        {
            const PointLight* pointLight  = static_cast<const PointLight*>(light);
            glm::vec3         lightPos_ws = pointLight->getPosition();
            myShaders.set("uLightType[" + std::to_string(i) + "]", 0);
            myShaders.set("uLightPos_ws[" + std::to_string(i) + "]", lightPos_ws);
            // std::cout << pointLight[0].getPosition().y << "\n";
        }
        else if (light->getType() == LightType::Directional)
        {
            const DirectionalLight* directionalLight = static_cast<const DirectionalLight*>(light);
            glm::vec3               LightDir_ws      = directionalLight->m_direction;
            myShaders.set("uLightType[" + std::to_string(i) + "]", 1);
            myShaders.set("uLightDir_ws[" + std::to_string(i) + "]", LightDir_ws);
        }
        myShaders.set("uLightColor[" + std::to_string(i) + "]", light->m_color);
        myShaders.set("uLightIntensity[" + std::to_string(i) + "]", light->m_intensity);
        i++;
    }
}