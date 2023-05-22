#ifndef _LIGHTS_HPP__
#define _LIGHTS_HPP__

#include <iostream>
#include <vector>
#include "Sphere/common.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

enum class LightType {
    Ponctual,
    Directional
};

class Light {
public:
    glm::vec3 m_color;
    float     m_intensity;

    inline LightType getType() const { return m_type; }

protected:
    LightType m_type;
    Light(LightType type)
        : m_type(type){};
};

class PointLight : public Light {
private:
public:
    glm::vec3 m_position;

    PointLight()
        : Light(LightType::Ponctual){

        };
};

class DirectionalLight : public Light {
public:
    glm::vec3 m_direction{};

    DirectionalLight()
        : Light(LightType::Directional){};
};

class LightManager {
private:
    std::vector<Light*> m_lights;

public:
    int  nb_lights = 0;
    void addPointLight(glm::vec3 position, glm::vec3 color, float intensity);
    void addDirectionalLight(glm::vec3 direction, glm::vec3 color, float intensity);

    inline const std::vector<Light*>& getLights() const { return m_lights; }
};

#endif