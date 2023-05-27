#ifndef __INTENSITY_HPP_
#define __INTENSITY_HPP_
#include "p6/p6.h"

struct Intensity {
    float separationIntensity = 0.5f;
    float cohesionIntensity   = 0.5f;
    float alignmentIntensity  = 0.5f;
    float perceptionIntensity = 0.5f;

    void GUI(p6::Context& ctx);
};
#endif