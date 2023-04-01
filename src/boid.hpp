#ifndef __BOID_HPP_
#define __BOID_HPP_

#include <vector>
#include "glm/fwd.hpp"
#include "p6/p6.h"

using vec = glm::vec2;

class Boid {
private:
    vec m_position;
    vec m_velocity;
    vec m_direction;

public:
    static float alignmentIntensity, cohesionIntensity, separationIntensity, perception;

    Boid();
    Boid(const Boid& b);
    Boid(vec position, vec velocity, vec direction);
    ~Boid() = default;

    std::vector<Boid> searchNeighbors(const std::vector<Boid>& boids);

    void draw(p6::Context& ctx, float radius);
    bool closeToEdges(p6::Context& ctx, float radius) const;

    vec align(const std::vector<Boid>& boids);
    vec cohesion(const std::vector<Boid>& boids);
    vec separation(const std::vector<Boid>& boids);

    void flock(const std::vector<Boid>& boids, p6::Context& ctx);

    void avoidEdges(p6::Context& ctx, float radius);

    vec inline setVelocity(vec vel) { return m_velocity = vel; }
    vec inline getVelocity() { return m_velocity; }
};

#endif