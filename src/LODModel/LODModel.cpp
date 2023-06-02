#include "LODModel.hpp"

LOD updateLOD(glm::vec3 cameraPosition, glm::vec3 modelPosition)
{
    // distance between the model and the camera
    float cameraDistance = glm::distance(modelPosition, cameraPosition);

    // default LOD
    LOD lod = LOD::LOD_LOW;

    if (cameraDistance < 15)
    {
        lod = LOD::LOD_HIGH;
    }
    else // if (cameraDistance < 8)
    {
        lod = LOD::LOD_LOW;
    }
    return lod;
}