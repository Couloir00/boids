#include "boid.hpp"
#include <vector>
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "p6/p6.h"

using vec = glm::vec2;

Boid::Boid()
    : m_position(p6::random::point()), m_velocity(p6::random::point() * vec{0.01f}), m_acceleration(vec{0.f}), m_maxForce(0.2), m_maxSpeed(10){};

Boid::Boid(const Boid& b)
    : m_position(b.m_position), m_velocity(b.m_velocity), m_acceleration(b.m_acceleration), m_maxForce(b.m_maxForce), m_maxSpeed(b.m_maxSpeed){};

Boid::Boid(vec pos, vec vel, vec acc, float force, float speed)
    : m_position(pos), m_velocity(vel), m_acceleration(acc), m_maxForce(force), m_maxSpeed(speed){};

void Boid::draw(p6::Context& ctx)
{
    ctx.fill = {1.f};
    ctx.circle(p6::Center{m_position}, p6::Radius{0.05});
};

void Boid::update()
{
    m_position += m_velocity;
    m_velocity += m_acceleration;
};

vec Boid::align(const std::vector<Boid>& boids) // float alignmentRadius, float alignmentStrength)
{
    float perception = 0.1f;
    vec   steering   = vec{0.f};
    int   total      = 0;
    for (auto& other : boids)
    {
        float d = glm::distance(m_position, other.m_position);
        if (&other != this && d < perception)
        {
            steering += other.m_velocity;
            total++;
        }
    }
    if (total > 0)
    {
        steering /= static_cast<float>(total);
        // set magnitude to control Speed
        if (glm::length(steering) > m_maxSpeed)
        {
            steering = glm::normalize(steering) * m_maxSpeed;
        }

        steering -= m_velocity;
        // limit force
        if (glm::length(steering) > m_maxForce)
        {
            steering = glm::normalize(steering) * m_maxForce;
        }
    }
    return steering;
};

void Boid::flock(const std::vector<Boid>& boids)
{
    vec alignment = this->align(boids);
    m_velocity += alignment;
    // limit velocity
    if (glm::length(m_velocity) > m_maxSpeed)
    {
        m_velocity = glm::normalize(m_velocity) * m_maxSpeed;
    }

    // on the coding train
    // m_acceleration = alignment;
}

bool Boid::closeToEdges(p6::Context& ctx) const
{
    return m_position.x <= -ctx.aspect_ratio() + 0.05 || m_position.x >= ctx.aspect_ratio() - 0.05 || m_position.y <= -1 + 0.05 || m_position.y >= 1 - 0.05;
};