#ifndef __INTENSITY_HPP_
#define __INTENSITY_HPP_

struct Intensity {
    float separationIntensity = 0.5f;
    float cohesionIntensity   = 0.5f;
    float alignmentIntensity  = 0.5f;
    float perceptionIntensity = 0.5f;

    void GUI();
};
#endif