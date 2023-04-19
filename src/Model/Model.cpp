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

    // Debind the vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind the vao
    glBindVertexArray(m_vao.getId());

    // Enable index attributes, position = 0 by default
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORMALS  = 1;
    const GLuint VERTEX_ATTR_UVS      = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORMALS);
    glEnableVertexAttribArray(VERTEX_ATTR_UVS);

    // rebind vbos to specify attributes of the vao

    glBindBuffer(GL_ARRAY_BUFFER, m_vboPos.getId());
    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboNormals.getId());
    glVertexAttribPointer(VERTEX_ATTR_NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboUVs.getId());
    glVertexAttribPointer(VERTEX_ATTR_NORMALS, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

    // debind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // debind vao
    glBindVertexArray(0);
}

void Model::modelDraw(const p6::Shader& myShader, const glm::mat4& viewMatrix, const ModelControls& controls, const glm::mat4& projectionMatrix)
{
    /// ModelView MATRIX : ///

    // mvmatrix at the correct position
    glm::mat4 MVMatrix = viewMatrix;
    MVMatrix           = glm::translate(MVMatrix, controls.position);

    // scaling model
    MVMatrix = glm::scale(MVMatrix, glm::vec3(controls.scale));

    /// ModelViewProjection MATRIX/ : //

    // compute the MVP matrix
    glm::mat4 MVPMatrix = projectionMatrix * MVMatrix;

    /// Normal Matrix : ///

    // compute the normalMatrix
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    /// Uniform variables from shader///

    // classic uniform
    /*GLuint uMVMatrixID = glGetUniformLocation(myShader.id(), "uMVMatrix");
    glUniformMatrix4fv(uMVMatrixID, 1, GL_FALSE, glm::value_ptr(MVMatrix));*/

    // p6 shader abstraction
    myShader.set("uMVMatrix", MVMatrix);
    myShader.set("uMVPMatrix", MVPMatrix);
    myShader.set("uNormalMatrix", NormalMatrix);

    // Binding vao for model
    glBindVertexArray(m_vao.getId());
    // draw the array of vertices
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_outVertices.size()));
    // debind vao
    glBindVertexArray(0);
}

/*
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string        lineHeader;
        lineStream >> lineHeader;

        // OBJ reading

        // vertices
        if (lineHeader == "v")
        {
            glm::vec3 vertex(0.0f);
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            // fill the array with the corresponding indices from the file
            tempVertices.push_back(vertex);
        }

        // vertices tex
        else if (lineHeader == "vt")
        {
            glm::vec2 uv;
            lineStream >> uv.x >> uv.y;
            // fill the array with the corresponding indices from the file
            tempUvs.push_back(uv);
        }

        // vertices normal
        else if (lineHeader == "vn")
        {
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            // fill the array with the corresponding indices from the file
            tempNormals.push_back(normal);
        }

        // faces
        else if (lineHeader == "f")
        {
            unsigned int vertexIndex[3];
            unsigned int uvIndex[3];
            unsigned int normalIndex[3];

            for (int i = 0; i < 3; ++i)
            {
                lineStream >> vertexIndex[i];
                lineStream.ignore(1); // ignore(1) is to skip a line
                lineStream >> uvIndex[i];
                lineStream.ignore(1);
                lineStream >> normalIndex[i];
            }

            // fill the arrays with the corresponding indices from the file
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

            // Check if the file reading is successful
            if (file.bad())
            {
                std::cout << "Error: Failed to read file: " << m_modelPath << std::endl;
                file.close();
                return;
            }

            file.close();
        }
    }

    // Process Data

    // For each vertex of each triangle
    for (unsigned int vertexIndex : vertexIndices)
    {
        glm::vec3 vertex = tempVertices[vertexIndex - 1];
        m_outVertices.push_back(vertex);
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
    }*/

/* a possible improved version need to be tested
std::array<unsigned int, 3> vertexIndex{};
std::array<unsigned int, 3> uvIndex{};
std::array<unsigned int, 3> normalIndex{};

for (int i = 0; i < 3; ++i)
{
    if (!(lineStream >> vertexIndex[i]))
    {
        std::cerr << "Error parsing vertex index at index " << i << std::endl;
    }
    else
    {
        --vertexIndex[i]; // Convert to 0-based index
    }

    if (!(lineStream.ignore(1)))
    {
        std::cerr << "Error ignoring delimiter after vertex index at index " << i << std::endl;
    }

    if (!(lineStream >> uvIndex[i]))
    {
        std::cerr << "Error parsing UV index at index " << i << std::endl;
    }
    else
    {
        --uvIndex[i]; // Convert to 0-based index
    }

    if (!(lineStream.ignore(1)))
    {
        std::cerr << "Error ignoring delimiter after UV index at index " << i << std::endl;
    }

    if (!(lineStream >> normalIndex[i]))
    {
        std::cerr << "Error parsing normal index at index " << i << std::endl;
    }
    else
    {
        --normalIndex[i]; // Convert to 0-based index
    }

    if (i < 2 && !(lineStream.ignore(1)))
    {
        std::cerr << "Error ignoring delimiter after normal index at index " << i << std::endl;
    }

    // Push back all indices into their respective containers
    vertexIndices.push_back(vertexIndex[i]);
    uvIndices.push_back(uvIndex[i]);
    normalIndices.push_back(normalIndex[i]);
}*/