/*
OBJ LOADER ADAPTED FROM
http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
IN ORDER TO READ OBJ.file (custom meshes from Blender)
Github link : https://github.com/opengl-tutorials/ogl
*/
#ifndef __MODEL_HPP_
#define __MODEL_HPP_

#include <string>
#include <vector>
#include "Model/ModelControls.hpp"
#include "VAO/VAO.hpp"
#include "VBO/VBO.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Model {
private:
    // OBJ model properties
    std::vector<glm::vec3> m_outVertices;
    std::vector<glm::vec2> m_outUvs;
    std::vector<glm::vec3> m_outNormals;
    std::vector<glm::vec3> m_outColors;
    std::string            m_modelPath;

    // defining a VAO
    VAO m_vao;
    // defining VBO instances for position,texcoord, normals & colors
    VBO m_vboPos;
    VBO m_vboNormals;
    VBO m_vboUVs;
    VBO m_vboColor;

public:
    // default construct, specify default or else we have an undefined reference :(
    Model() = default;

    // Construct from the path
    Model(std::string modelPath);
    // Init opengl context for model
    void modelInitialize();
    // read and load the model file
    void modelLoad();
    // draw the model
    void modelDraw(const p6::Shader& myShader, const glm::mat4& viewMatrix, const ModelControls& modelControls, const glm::mat4& projectionMatrix);
};

void computeRotationVectors(glm::vec3& frontAxis, glm::vec3& leftAxis, glm::vec3& upAxis, const glm::vec3& rotDir);

#endif