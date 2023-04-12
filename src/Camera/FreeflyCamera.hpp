#ifndef __FREEFLYCAMERA_HPP_
#define __FREEFLYCAMERA_HPP_

#include "glm/glm.hpp"
#include "p6/p6.h"

class FreeflyCamera {
private:
    glm::vec3 m_Position;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;
    float     m_Phi;
    float     m_Theta;

public:
    FreeflyCamera();
    void      computeDirectionVectors();
    void      moveLeft(float t);
    void      moveFront(float t);
    void      rotateLeft(float degrees);
    void      rotateUp(float degrees);
    glm::mat4 getViewMatrix() const;
};

void cameraControls(const p6::Key& key, FreeflyCamera& freeCamera);

#endif