#ifndef __MODELLOD_HPP_
#define __MODELLOD_HPP_

#include <vector>
#include "LODModel/LODModel.hpp"
#include "Model/Model.hpp"
#include "Model/ModelControls.hpp"

class ModelLOD {
private:
    std::vector<Model> m_models;

public:
    explicit ModelLOD(std::array<std::string, LOD_NUMBER> paths);

    void modelLODDraw(const p6::Shader& myShader, const glm::mat4& viewMatrix, const ModelControls& modelControls, const glm::mat4& projectionMatrix);
};
#endif