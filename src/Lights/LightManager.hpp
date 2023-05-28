#ifndef _LIGHTS_HPP__
#define _LIGHTS_HPP__

#include <iostream>
#include <vector>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "p6/p6.h"

enum class LightType {
    Ponctual,
    Directional
};

class Light {
public:
    glm::vec3        m_color{};
    float            m_intensity{};
    inline LightType getType() const { return m_type; }

protected:
    LightType m_type;
    explicit Light(LightType type)
        : m_type(type){};
};

class PointLight : public Light {
private:
    glm::vec3 m_position{};

public:
    PointLight()
        : Light(LightType::Ponctual){};
    inline glm::vec3 getPosition() const { return m_position; }
    inline void      setPosition(glm::vec3 position) { m_position = position; }
};

class DirectionalLight : public Light {
public:
    glm::vec3 m_direction{};
    DirectionalLight()
        : Light(LightType::Directional){};
    inline glm::vec3 getDirection() const { return m_direction; }
};

class LightManager {
private:
    std::vector<Light*> m_lights;
    glm::vec3           m_ShadowLight = {-1.0f, 5.0f, 0.0f};

public:
    int  nb_lights = 0;
    void addPointLight(glm::vec3 position, glm::vec3 color, float intensity);
    void addDirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity);

    inline const std::vector<Light*>& getLights() const { return m_lights; }
    inline glm::vec3                  getShadowLight() const { return m_ShadowLight; }
    static void                       initLighting(LightManager& lightManager, const p6::Shader& myShaders);
};

#endif