#ifndef __VBO_HPP_
#define __VBO_HPP_
#include <utility>
#include "p6/p6.h"

class VBO {
private:
    GLuint m_vboId;

public:
    // vao constructor
    VBO() { glGenBuffers(1, &m_vboId); }

    // vbo destructor
    ~VBO() { glDeleteBuffers(1, &m_vboId); }

    // copy construct (copy not allowed to prevent dumb moves)
    VBO(const VBO&) = delete;

    // Jules' advice to use move constructor and assignement for vbo//

    // Moving constructor
    VBO(VBO&& aVbo)
    noexcept
        : m_vboId{std::exchange(aVbo.m_vboId, 0)}
    {
    }

    // Move assignment operator (https://en.cppreference.com/w/cpp/language/move_assignment)
    VBO& operator=(VBO&& aVbo) noexcept
    {
        if (this != &aVbo)
        {
            // Delete previous vbo
            glDeleteBuffers(1, &m_vboId);
            std::exchange(aVbo.m_vboId, 0);
        }
        return *this;
    }
    // Get the id of the VBO
    GLuint getId() const { return m_vboId; };
};

#endif