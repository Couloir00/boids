#include "Boid/Boid.hpp"
#include <glm/gtc/random.hpp>
#include <vector>
#include "Model/ModelControls.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

Boid::Boid(glm::vec3 pos, glm::vec3 velocity, float radius)
    : m_position(pos), m_velocity(velocity), m_radius(radius)
{
    m_direction = glm::normalize(glm::vec3(p6::random::number(), p6::random::number(), p6::random::number()));
};

std::vector<Boid> Boid::searchNeighbors(const std::vector<Boid>& boids, const Intensity& intensity)
{
    std::vector<Boid> neighbors{};
    for (const auto& other : boids)
    {
        float distance = glm::distance(m_position, other.m_position);
        if (&other != this && distance <= intensity.perceptionIntensity)
        {
            neighbors.emplace_back(other);
        }
    }
    return neighbors;
}

glm::vec3 Boid::align(const std::vector<Boid>& neighbors)
{
    glm::vec3 alignment = glm::vec3{0.f};
    for (const auto& boid : neighbors)
    {
        alignment += boid.m_direction;
    }
    if (!neighbors.empty())
    {
        alignment = glm::normalize(alignment);
    }
    return alignment;
};

glm::vec3 Boid::cohesion(const std::vector<Boid>& neighbors)
{
    glm::vec3 cohesion = glm::vec3{0.f};
    for (const auto& boid : neighbors)
    {
        cohesion += boid.m_position;
    }
    if (!neighbors.empty())
    {
        cohesion /= static_cast<float>(neighbors.size());
        cohesion -= m_position;
        cohesion = glm::normalize(cohesion);
    }
    return cohesion;
};

glm::vec3 Boid::separation(const std::vector<Boid>& neighbors)
{
    glm::vec3 separation = glm::vec3{0.f};
    for (const auto& boid : neighbors)
    {
        glm::vec3 difference = m_position - boid.m_position;
        if (glm::vec3::length() > 0)
            separation += glm::normalize(difference) / static_cast<float>(glm::vec3::length());
    }
    if (!neighbors.empty())
    {
        separation = glm::normalize(separation);
    }
    return separation;
}

void Boid::flock(const std::vector<Boid>& boids, p6::Context& ctx, const Intensity& intensity)
{
    glm::vec3 cohesion = this->cohesion(searchNeighbors(boids, intensity)) * glm::vec3{intensity.cohesionIntensity};

    glm::vec3 alignment = this->align(searchNeighbors(boids, intensity)) * glm::vec3{intensity.alignmentIntensity};

    glm::vec3 separation = this->separation(searchNeighbors(boids, intensity)) * glm::vec3{intensity.separationIntensity};

    m_direction += cohesion + alignment + separation;
    m_direction = glm::normalize(m_direction);

    m_velocity = glm::vec3{0.2f} * m_direction; // voir si normalize ou pas
    m_position += m_velocity * glm::vec3{2.0f} * glm::vec3{ctx.delta_time()};
}

void Boid::avoidEdges(const Boid& boid)
{
    if (boid.m_position.x < minX)
    {
        m_direction.x = 1.0f;
    }
    else if (boid.m_position.x > maxX)
    {
        m_direction.x = -1.0f;
    }

    if (boid.m_position.y < minY)
    {
        m_direction.y = 1.0f;
    }
    else if (boid.m_position.y > maxY)
    {
        m_direction.y = -1.0f;
    }

    if (boid.m_position.z < minZ)
    {
        m_direction.z = 1.0f;
    }
    else if (boid.m_position.z > maxZ)
    {
        m_direction.z = -1.0f;
    }

    m_direction = glm::normalize(m_direction);
}

ModelControls Boid::computeControls() const
{
    return ModelControls{
        .position  = m_position,
        .direction = m_direction,
        .speed     = m_velocity,
        .scale     = m_radius,
        .aLod      = LOD_LOW,
    };
}

std::vector<ModelControls> BoidsControls(const std::vector<Boid>& Boids, const glm::vec3& viewMatrixPosition)
{
    std::vector<ModelControls> controlsBoids{Boids.size()};
    for (auto const& boid : Boids)
    {
        ModelControls controls = boid.computeControls();
        controls.aLod          = updateLOD(viewMatrixPosition, controls.position);
        controlsBoids.emplace_back(controls);
    }
    return controlsBoids;
}