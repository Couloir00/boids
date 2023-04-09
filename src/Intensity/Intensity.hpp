#ifndef __INTENSITY_HPP_
#define __INTENSITY_HPP_

struct Intensity {
    float m_separationIntensity = 0.5f;
    float m_cohesionIntensity   = 0.5f;
    float m_alignmentIntensity  = 0.5f;
    float m_perceptionIntensity = 0.5f;
    bool  isGui();
};
#endif