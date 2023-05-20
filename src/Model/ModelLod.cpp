#include "Model/ModelLod.hpp"
#include "LODModel/LODModel.hpp"

ModelLOD::ModelLOD(std::array<std::string, LOD_NUMBER> paths)
{
    for (int i = 0; i < LOD_NUMBER; i++)
    {
        m_models.emplace_back(paths[i]);
    }
}

void ModelLOD::modelLODDraw(const p6::Shader& myShader, const glm::mat4& viewMatrix, const ModelControls& modelControls, const glm::mat4& projectionMatrix)
{
    m_models[modelControls.aLod].modelDraw(myShader, viewMatrix, modelControls, projectionMatrix);
}