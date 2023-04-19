#ifndef __MODELCONTROLS_HPP_
#define __MODELCONTROLS_HPP_

#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"

struct ModelControls {
    glm::vec3 position;
    glm::vec3 direction;
    float     scale;
};
#endif