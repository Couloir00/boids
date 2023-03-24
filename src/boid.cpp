#include "boid.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

using vec = glm::vec2;

Boid::Boid()
    : m_position(vec{0.f}), m_velocity(p6::random::point() * vec{0.01f}), m_acceleration(vec{0.f}){};

Boid::Boid(const Boid& b)
    : m_position(b.m_position), m_velocity(b.m_velocity), m_acceleration(b.m_acceleration){};

Boid::Boid(vec pos, vec vel, vec acc)
    : m_position(pos), m_velocity(vel), m_acceleration(acc){};

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

bool Boid::closeToEdges(p6::Context& ctx) const
{
    return m_position.x <= -ctx.aspect_ratio() + 0.05 || m_position.x >= ctx.aspect_ratio() - 0.05 || m_position.y <= -1 + 0.05 || m_position.y >= 1 - 0.05;
};