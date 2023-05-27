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
void FreeflyCamera::moveUp(float t)
{
    m_Position += t * glm::vec3(0.f, 1.f, 0.f);
}

void FreeflyCamera::moveDown(float t)
{
    m_Position -= t * glm::vec3(0.f, 1.f, 0.f);
}

void FreeflyCamera::rotateLeft(float degrees)
{
    m_Phi += glm::radians(degrees);
    computeDirectionVectors();
}

void FreeflyCamera::rotateUp(float degrees)
{
    m_Theta += glm::radians(degrees);
    computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}

glm::vec3 FreeflyCamera::getCamPosition() const
{
    return m_Position;
}

glm::vec3 FreeflyCamera::getCamFrontVector() const
{
    return m_FrontVector;
}

glm::vec3 FreeflyCamera::getCamLeftVector() const
{
    return m_LeftVector;
}

glm::vec3 FreeflyCamera::getCamUpVector() const
{
    return m_UpVector;
}

void FreeflyCamera::keyboardEvents(FreeflyCamera& camera, ModelControls& playerControl) const
{
    if (Z)
        camera.moveFront(0.1f);
    if (Q)
        camera.moveLeft(0.1f);
    if (S)
        camera.moveFront(-0.1f);
    if (D)
        camera.moveLeft(-0.1f);
    if (SPACE)
        camera.moveUp(0.1f);
    if (CTRL)
        camera.moveDown(0.1f);
    if (P)
    {
        std::cout << "x = " << playerControl.position.x << std::endl;
        std::cout << "y = " << playerControl.position.y << std::endl;
        std::cout << "z = " << playerControl.position.z << std::endl;
    }
}

void FreeflyCamera::actionEvents(p6::Context& ctx, FreeflyCamera& camera)
{
    ctx.key_pressed = [&](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
        {
            Z = true;
        }
        if (key.physical == GLFW_KEY_A)
        {
            Q = true;
        }
        if (key.physical == GLFW_KEY_S)
        {
            S = true;
        }
        if (key.physical == GLFW_KEY_D)
        {
            D = true;
        }
        if (key.physical == GLFW_KEY_P)
        {
            P = true;
        }
        if (key.physical == GLFW_KEY_SPACE)
        {
            SPACE = true;
        }
        if (key.physical == GLFW_KEY_LEFT_CONTROL)
        {
            CTRL = true;
        }
    };

    ctx.key_released = [&](const p6::Key& key) {
        if (key.physical == GLFW_KEY_W)
        {
            Z = false;
        }
        if (key.physical == GLFW_KEY_A)
        {
            Q = false;
        }
        if (key.physical == GLFW_KEY_S)
        {
            S = false;
        }
        if (key.physical == GLFW_KEY_D)
        {
            D = false;
        }
        if (key.physical == GLFW_KEY_P)
        {
            P = false;
        }
        if (key.physical == GLFW_KEY_SPACE)
        {
            SPACE = false;
        }
        if (key.physical == GLFW_KEY_LEFT_CONTROL)
        {
            CTRL = false;
        }
    };

    ctx.mouse_dragged = [&](p6::MouseDrag drag) {
        camera.rotateLeft(drag.delta.x * 25.f);
        camera.rotateUp(-drag.delta.y * 25.f);
    };
}

void FreeflyCamera::limitCamera(FreeflyCamera& camera)
{
    // Vérification des limites de position
    if (camera.getCamPosition().x < minX)
        camera.setCamPosition(glm::vec3(minX, camera.getCamPosition().y, camera.getCamPosition().z));
    if (camera.getCamPosition().x > maxX)
        camera.setCamPosition(glm::vec3(maxX, camera.getCamPosition().y, camera.getCamPosition().z));
    if (camera.getCamPosition().z < minZ)
        camera.setCamPosition(glm::vec3(camera.getCamPosition().x, camera.getCamPosition().y, minZ));
    if (camera.getCamPosition().z > maxZ)
        camera.setCamPosition(glm::vec3(camera.getCamPosition().x, camera.getCamPosition().y, maxZ));

    if (camera.getCamPosition().y < minY)

        camera.setCamPosition(glm::vec3(camera.getCamPosition().x, minY, camera.getCamPosition().z));

    if (camera.getCamPosition().y > maxY)
        camera.setCamPosition(glm::vec3(camera.getCamPosition().x, maxY, camera.getCamPosition().z));
}