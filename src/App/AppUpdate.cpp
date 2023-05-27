#include "App/App.hpp"

/*void App::update()
{
    m_ctx.update = [&]() {

    };
    m_camera.actionEvents(m_ctx, m_camera);
    m_ctx.start();
}*/

void App::updateBoids()
{
    for (auto& b : boids)
    {
        b.flock(boids, m_ctx, m_intensities); // updating the boids here
    }
}