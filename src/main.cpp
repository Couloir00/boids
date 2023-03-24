
#include "p6/p6.h"
#define DOCTEST_CONFIG_IMPLEMENT
#include <cstdlib>
#include <vector>
#include "boid.hpp"
#include "doctest/doctest.h"
#include "glm/fwd.hpp"

// TO DO normaliser la fenêtre et faire en sorte que les boids restent dedans puis poursuivre avec la vidéo
// The coding train https://www.youtube.com/watch?v=mhjuuHl6qHM&t=276s 9min

using vec = glm::vec2;

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "giveItATry"}};
    ctx.maximize_window();

    // float WindowRatio = ctx.aspect_ratio();

    // vector of numberBoids boids
    std::vector<Boid> boids;
    int               numberBoids = 100;
    boids.reserve(numberBoids);
    for (int i = 0; i < numberBoids; i++)
    {
        boids.emplace_back();
    }

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        for (auto& b : boids)
        {
            b.update();
            if (b.closeToEdges(ctx))
            {
                b.setVelocity(-b.getVelocity());
            }
            b.draw(ctx);
        }
        ctx.circle(
            p6::Center{ctx.mouse()},
            p6::Radius{0.2f}
        );
    };
    // Should be done last. It starts the infinite loop.
    ctx.start();
}