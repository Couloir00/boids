/*
OBJ LOADER ADAPTED FROM
http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
Github link : https://github.com/opengl-tutorials/ogl
*/
#include "Model/Model.hpp"
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/matrix.hpp"
#include "p6/p6.h"

Model::Model(std::string modelPath)
    : m_modelPath((std::move(modelPath))) // clang tidy suggests std::move to avoid unecessary copies
{
    modelLoad();
    modelInitialize();
}

void Model::modelLoad()
{
    // Properties
    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> uvIndices;
    std::vector<unsigned int> normalIndices;
    // Temp vectors
    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempUvs;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec3> tempColors;

    // Open the file
    std::ifstream file(p6::make_absolute_path(m_modelPath));
    if (!file.is_open())
    {
        std::cout << "Error: Failed to open file: " << m_modelPath << std::endl;
        return;
    }

    while (!file.eof()) //
    {
        std::string lineHeader;

        file >> lineHeader;

        // OBJ reading

        if (lineHeader == "v")
        {
            glm::vec3 vertex(0.f);
            file.ignore(1); // skip a line
            file >> vertex.x >> vertex.y >> vertex.z;

            tempVertices.push_back(vertex);

            if (file.peek() == '\n')
            {
                continue;
            }

            glm::vec3 vertColors(0.f);
            file >> vertColors.x >> vertColors.y >> vertColors.z;

            tempColors.push_back(vertColors);
        }
        else if (lineHeader == "vt")
        {
            glm::vec2 uv;
            file.ignore(1);
            file >> uv.x >> uv.y;
            tempUvs.push_back(uv);
        }
        else if (lineHeader == "vn")
        {
            glm::vec3 normal;
            file.ignore(1);
            file >> normal.x;
            file >> normal.y;
            file >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (lineHeader == "f")
        {
            unsigned int vertexIndex[3];
            unsigned int uvIndex[3];
            unsigned int normalIndex[3];

            file.ignore(1);
            file >> vertexIndex[0];
            file.ignore(1);
            file >> uvIndex[0];
            file.ignore(1);
            file >> normalIndex[0];

            file >> vertexIndex[1];
            file.ignore(1);
            file >> uvIndex[1];
            file.ignore(1);
            file >> normalIndex[1];

            file >> vertexIndex[2];
            file.ignore(1);
            file >> uvIndex[2];
            file.ignore(1);
            file >> normalIndex[2];

            for (unsigned int vertex : vertexIndex)
            {
                vertexIndices.push_back(vertex);
            }
            for (unsigned int uv : uvIndex)
            {
                uvIndices.push_back(uv);
            }
            for (unsigned int normal : normalIndex)
            {
                normalIndices.push_back(normal);
            }
        }
    }

    // For each vertex of each triangle
    for (unsigned int vertexIndex : vertexIndices)
    {
        glm::vec3 vertex = tempVertices[vertexIndex - 1];
        m_outVertices.push_back(vertex);

        if (tempColors.empty())
        {
            continue;
        }

        glm::vec3 color = tempColors[vertexIndex - 1];
        m_outColors.push_back(color);
    }

    if (tempColors.empty())
    {
        for (unsigned int i = 0; i < m_outVertices.size(); i++)
        {
            m_outColors.emplace_back(1.0f, 1.0f, 1.0f); // white
        }
    }

    for (unsigned int uvIndex : uvIndices)
    {
        glm::vec2 uv = tempUvs[uvIndex - 1];
        m_outUvs.push_back(uv);
    }

    for (unsigned int normalIndex : normalIndices)
    {
        glm::vec3 normal = tempNormals[normalIndex - 1];
        m_outNormals.push_back(normal);
    }
}

// what we have seen during TPs (binding/debinding)
void Model::modelInitialize()
{
    // Init
    // Bind the Vbo for properties

    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vboPos.getId());
    // fill
    glBufferData(GL_ARRAY_BUFFER, m_outVertices.size() * sizeof(glm::vec3), m_outVertices.data(), GL_STATIC_DRAW);

    // tex
    glBindBuffer(GL_ARRAY_BUFFER, m_vboUVs.getId());
    // fill
    glBufferData(GL_ARRAY_BUFFER, m_outUvs.size() * sizeof(glm::vec2), m_outUvs.data(), GL_STATIC_DRAW);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals.getId());
    // fill
    glBufferData(GL_ARRAY_BUFFER, m_outNormals.size() * sizeof(glm::vec3), m_outNormals.data(), GL_STATIC_DRAW);

    // color
    glBindBuffer(GL_ARRAY_BUFFER, m_vboColor.getId());
    // fill
    glBufferData(GL_ARRAY_BUFFER, m_outColors.size() * sizeof(glm::vec3), m_outColors.data(), GL_STATIC_DRAW);

    // Debind the vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind the vao
    glBindVertexArray(m_vao.getId());

    // Enable index attributes, position = 0 by default
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMALS  = 1;
    const GLuint VERTEX_ATTR_UVS      = 2;
    const GLuint VERTEX_ATTR_COLOR    = 3;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMALS);
    glEnableVertexAttribArray(VERTEX_ATTR_UVS);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    // rebind vbos to specify attributes of the vao

    glBindBuffer(GL_ARRAY_BUFFER, m_vboPos.getId());
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals.getId());
    glVertexAttribPointer(VERTEX_ATTR_NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboUVs.getId());
    glVertexAttribPointer(VERTEX_ATTR_UVS, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboColor.getId());
    glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    // debind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // debind vao
    glBindVertexArray(0);
}

void Model::modelDraw(const p6::Shader& myShader, const glm::mat4& viewMatrix, const ModelControls& modelControls, const glm::mat4& projectionMatrix)
{
    // Rotate model
    glm::vec3 frontAxis;
    glm::vec3 leftAxis;
    glm::vec3 upAxis;
    computeRotationVectors(frontAxis, leftAxis, upAxis, modelControls.direction);

    // Rotation matrix
    glm::mat4 RotationMatrix = glm::mat4(
        glm::vec4(leftAxis, 0.0f),
        glm::vec4(upAxis, 0.0f),
        glm::vec4(frontAxis, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) // homogene
    );
    /// ModelView MATRIX : TRS ///

    glm::mat4 ModelMatrix = glm::mat4(1.0f);
    ModelMatrix           = glm::translate(ModelMatrix, modelControls.position);
    ModelMatrix           = ModelMatrix * RotationMatrix;
    ModelMatrix           = glm::scale(ModelMatrix, glm::vec3{modelControls.scale});

    glm::mat4 MVMatrix = viewMatrix * ModelMatrix;

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glm::mat4 MVPMatrix = projectionMatrix * MVMatrix;

    /// Uniform variables from shader///

    // classic uniform
    /*GLuint uMVMatrixID = glGetUniformLocation(myShader.id(), "uMVMatrix");
    glUniformMatrix4fv(uMVMatrixID, 1, GL_FALSE, glm::value_ptr(MVMatrix));*/

    // p6 shader abstraction
    myShader.set("uMVMatrix", MVMatrix);
    myShader.set("uMVPMatrix", MVPMatrix);
    myShader.set("uNormalMatrix", NormalMatrix);
    myShader.set("model", ModelMatrix);

    // Binding vao for model
    glBindVertexArray(m_vao.getId());
    // draw the array of vertices
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_outVertices.size()));
    // debind vao
    glBindVertexArray(0);
}

void computeRotationVectors(glm::vec3& frontAxis, glm::vec3& leftAxis, glm::vec3& upAxis, const glm::vec3& rotDir)
{
    frontAxis = glm::normalize(rotDir);
    leftAxis  = glm::normalize(glm::cross(frontAxis, glm::vec3(0, 1, 0)));
    upAxis    = glm::normalize(glm::cross(leftAxis, frontAxis));
}