/*
OBJ LOADER ADAPTED FROM
http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
IN ORDER TO READ OBJ.file (custom meshes from Blender)
Github link : https://github.com/opengl-tutorials/ogl
*/
#ifndef __MODEL_HPP_
#define __MODEL_HPP_

#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Model {
private:
    std::vector<glm::vec3> m_outVertices;
    std::vector<glm::vec2> m_outUvs;
    std::vector<glm::vec3> m_outNormals;
    std::string            m_Path;
};
#endif