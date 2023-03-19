#ifndef __BOID_HPP_
#define __BOID_HPP_

#include "glm/fwd.hpp"
#include "p6/p6.h"

using vec = glm::vec2;

class Boid {
private:
    vec m_position;
    vec m_velocity;
    vec m_acceleration;

public:
    Boid();
    Boid(const Boid& b);
    Boid(vec position, vec velocity, vec acceleration);
    // ~Boid();
    void update();
    void draw(p6::Context& ctx);
};

#endif