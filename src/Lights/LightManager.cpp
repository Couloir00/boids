#include "LightManager.hpp"
#include "Lights/LightManager.hpp"
#include "Sphere/common.hpp"
#include "glm/ext/vector_float3.hpp"

void LightManager::addPointLight(glm::vec3 position, glm::vec3 color, float intensity)
{
    auto* light        = new PointLight(); // auto = PoinctualLight*
    light->m_position  = position;
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