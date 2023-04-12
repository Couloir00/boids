#ifndef __VAO_HPP_
#define __VAO_HPP_
#include "p6/p6.h"

class VAO {
private:
    GLuint m_vaoId;

public:
    // vao constructor
    VAO() { glGenVertexArrays(1, &m_vaoId); }

    // vao destructor
    ~VAO() { glDeleteBuffers(1, &m_vaoId); }

    // copy construct (copy not allowed to prevent dumb moves)
    VAO(const VAO&) = delete;

    // Jules' advice to use move constructor and assignement for vao//

    // Moving constructor
    VAO(VAO&& aVao)
    noexcept
        : m_vaoId{aVao.m_vaoId}
    {
        aVao.m_vaoId = 0;
    }

    // Move assignment operator (https://en.cppreference.com/w/cpp/language/move_assignment)
    VAO& operator=(VAO&& aVao) noexcept
    {
        if (this != &aVao)
        {
            // Delete previous vao
            glDeleteBuffers(1, &m_vaoId);
            // Copy the object
            m_vaoId = aVao.m_vaoId;
            // prevent deleting the id by mistake
            aVao.m_vaoId = 0;
        }
        return *this;
    }
    // Get the id of the VAO
    GLuint getId() const { return m_vaoId; };
};

#endif