#ifndef __BOID_HPP_
#define __BOID_HPP_

#include <vector>
#include "Intensity/Intensity.hpp"
#include "LODModel/LODModel.hpp"
#include "Model/ModelControls.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

using vec = glm::vec3;

class Boid {
private:
    vec   m_position;
    vec   m_velocity;
    float m_radius{0.1f};
    vec   m_direction{1.f, 0.f, 0.f};

    vec align(const std::vector<Boid>& neighbors);
    vec cohesion(const std::vector<Boid>& neighbors);
    vec separation(const std::vector<Boid>& neighbors);

public:
    Boid(vec position, vec velocity, float radius);
    ~Boid() = default;
    std::vector<Boid> searchNeighbors(const std::vector<Boid>& boids, const Intensity& intensity);
    bool              closeToEdges(p6::Context& ctx, float radius) const;
    void              flock(const std::vector<Boid>& boids, p6::Context& ctx, const Intensity& intensity);
    void              avoidEdges(p6::Context& ctx, float radius);
    ModelControls     computeControls() const;
};
std::vector<ModelControls> BoidsControls(const std::vector<Boid>& boids, const glm::vec3& viewMatrixPosition);
#endif