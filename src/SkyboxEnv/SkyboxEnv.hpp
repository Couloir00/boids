#ifndef __SKYBOXENV_HPP_
#define __SKYBOXENV_HPP_
#include <vector>
#include "Texture/Texture.hpp"
#include "VAO/VAO.hpp"
#include "VBO/VBO.hpp"

void generateSkybox(Texture& cubemapTex, std::vector<img::Image>& faces, const VAO& skybVAO, const VBO& skyVBO);

#endif
