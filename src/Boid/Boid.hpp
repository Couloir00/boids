#ifndef __BOID_HPP_
#define __BOID_HPP_

#include <vector>
#include "Intensity/Intensity.hpp"
#include "LODModel/LODModel.hpp"
#include "Model/ModelControls.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec3 m_position{};
    glm::vec3 m_velocity{};
    float     m_radius{0.1f};
    glm::vec3 m_direction{1.f, 0.f, 0.f};

    // boids position limits (very raw way to do it, could be improved)
    float minX = -29.0f;
    float maxX = 29.0f;
    float minZ = -28.5f;
    float maxZ = 30.5f;
    float minY = -2.2f;
    float maxY = 14.f;

    static glm::vec3 align(const std::vector<Boid>& neighbors);
    glm::vec3        cohesion(const std::vector<Boid>& neighbors);
    glm::vec3        separation(const std::vector<Boid>& neighbors);

public:
    Boid()            = default;
    Boid(const Boid&) = default;
    Boid(glm::vec3 position, glm::vec3 velocity, float radius);
    Boid& operator=(const Boid&)           = delete;
    Boid& operator=(Boid&& aBoid) noexcept = delete;
    ~Boid()                                = default;
    std::vector<Boid> searchNeighbors(const std::vector<Boid>& boids, const Intensity& intensity);
    void              flock(const std::vector<Boid>& boids, p6::Context& ctx, const Intensity& intensity);
    void              avoidEdges(const Boid& boid);
    ModelControls     computeControls() const;
};
std::vector<ModelControls> BoidsControls(const std::vector<Boid>& boids, const glm::vec3& viewMatrixPosition);
#endif