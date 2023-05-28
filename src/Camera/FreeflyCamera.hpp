#ifndef __FREEFLYCAMERA_HPP_
#define __FREEFLYCAMERA_HPP_

#include "Model/ModelControls.hpp"
#include "glm/fwd.hpp"
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

    // camera controls checker
    bool Z     = false;
    bool Q     = false;
    bool S     = false;
    bool D     = false;
    bool SPACE = false;
    bool CTRL  = false;
    bool P     = false;

    // character position limits (very raw way to do it, could be improved)
    float minX = -29.0f;
    float maxX = 29.0f;
    float minZ = -28.5f;
    float maxZ = 30.5f;
    float minY = -2.2f;
    float maxY = 14.f;

public:
    FreeflyCamera();

    void      computeDirectionVectors();
    void      moveLeft(float t);
    void      moveFront(float t);
    void      moveUp(float t);
    void      moveDown(float t);
    void      rotateLeft(float degrees);
    void      rotateUp(float degrees);
    glm::mat4 getViewMatrix() const;

    glm::vec3 getCamPosition() const;
    glm::vec3 getCamFrontVector() const;
    glm::vec3 getCamLeftVector() const;
    glm::vec3 getCamUpVector() const;

    inline void setCamPosition(glm::vec3 pos) { m_Position = pos; }
    void        keyboardEvents(FreeflyCamera& camera, ModelControls& playerControl) const;
    void        actionEvents(p6::Context& ctx, FreeflyCamera& camera);
    void        limitCamera(FreeflyCamera& camera);
};

#endif