#ifndef __LOD_MODEL_HPP_
#define __LOD_MODEL_HPP_
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"

enum LOD {
    LOD_LOW,   // low details
    LOD_HIGH,  // high details
    LOD_NUMBER // number of lods
};

LOD updateLOD(glm::vec3 cameraPosition, glm::vec3 modelPosition);
#endif