#ifndef __BOID_HPP_
#define __BOID_HPP_

#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

using vec = glm::vec2;

class Boid {
private:
    vec   m_position;
    vec   m_velocity;
    vec   m_acceleration;
    float m_maxForce;
    float m_maxSpeed;

public:
    Boid();
    Boid(const Boid& b);
    Boid(vec position, vec velocity, vec acceleration, float force, float speed);
    ~Boid() = default;
    void update();
    void draw(p6::Context& ctx);
    bool closeToEdges(p6::Context& ctx) const;
    vec  align(const std::vector<Boid>& boids);
    void flock(const std::vector<Boid>& boids);

    vec inline setVelocity(vec vel) { return m_velocity = vel; }
    vec inline getVelocity() { return m_velocity; }
};

#endif