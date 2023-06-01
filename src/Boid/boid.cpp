#include "Boid/boid.hpp"
#include <glm/gtc/random.hpp>
#include <vector>
#include "Model/ModelControls.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

using vec = glm::vec3;

Boid::Boid(vec pos, vec velocity, float radius)
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

vec Boid::align(const std::vector<Boid>& neighbors)
{
    vec alignment = vec{0.f};
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

vec Boid::cohesion(const std::vector<Boid>& neighbors)
{
    vec cohesion = vec{0.f};
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

vec Boid::separation(const std::vector<Boid>& neighbors)
{
    vec separation = vec{0.f};
    for (const auto& boid : neighbors)
    {
        vec difference = m_position - boid.m_position;
        if (vec::length() > 0)
            separation += glm::normalize(difference) / static_cast<float>(vec::length());
    }
    if (!neighbors.empty())
    {
        separation = glm::normalize(separation);
    }
    return separation;
}

void Boid::flock(const std::vector<Boid>& boids, p6::Context& ctx, const Intensity& intensity)
{
    vec cohesion = this->cohesion(searchNeighbors(boids, intensity)) * vec{intensity.cohesionIntensity};

    vec alignment = this->align(searchNeighbors(boids, intensity)) * vec{intensity.alignmentIntensity};

    vec separation = this->separation(searchNeighbors(boids, intensity)) * vec{intensity.separationIntensity};

    m_direction += cohesion + alignment + separation;
    m_direction = glm::normalize(m_direction);

    m_velocity = vec{0.2f} * m_direction; // voir si normalize ou pas
    m_position += m_velocity * vec{2.0f} * vec{ctx.delta_time()};
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

bool Boid::closeToEdges(p6::Context& ctx, float /*radius*/) const
{
    return m_position.x <= -ctx.aspect_ratio() || m_position.x >= ctx.aspect_ratio() || m_position.y <= -1 || m_position.y >= 1;
};

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