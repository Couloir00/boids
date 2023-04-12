#include "FreeflyCamera.hpp"
#include <cmath>
#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

FreeflyCamera::FreeflyCamera()
    : m_Position(0.f), m_Phi(glm::pi<float>()), m_Theta(0.f)
{
    computeDirectionVectors();
};

void FreeflyCamera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(glm::cos((m_Theta)) * glm::sin((m_Phi)), glm::sin((m_Theta)), glm::cos((m_Theta)) * glm::cos((m_Phi)));
    m_LeftVector  = glm::vec3(glm::sin(m_Phi + (glm::pi<float>() / 2.f)), 0, glm::cos(m_Phi + (glm::pi<float>() / 2.f)));
    m_UpVector    = glm::cross(m_FrontVector, m_LeftVector);
}

void FreeflyCamera::moveLeft(float t)
{
    m_Position += t * m_LeftVector;
}

void FreeflyCamera::moveFront(float t)
{
    m_Position += t * m_FrontVector;
}

void FreeflyCamera::rotateLeft(float degrees)
{
    m_Phi += glm::radians(degrees);
    computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

void cameraKeyControls(const p6::Key& key, FreeflyCamera& camera)
{
    // For QWERTY Keyboards

    if (key.physical == GLFW_KEY_A)
    {
        camera.moveLeft(1.f);
    }
    else if (key.physical == GLFW_KEY_D)
    {
        camera.moveLeft(-1.f);
    }
    else if (key.physical == GLFW_KEY_W)
    {
        camera.moveFront(1.f);
    }
    else if (key.physical == GLFW_KEY_S)
    {
        camera.moveFront(-1.f);
    }
}
