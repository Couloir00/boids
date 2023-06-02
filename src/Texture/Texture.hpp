#ifndef __TEXTURE_HPP_
#define __TEXTURE_HPP_
#include <utility>
#include "p6/p6.h"

class Texture {
private:
    GLuint m_texId{};

public:
    // vao constructor
    Texture() { glGenTextures(1, &m_texId); }

    // vbo destructor
    ~Texture() { glDeleteTextures(1, &m_texId); }

    // copy construct (copy not allowed to prevent dumb moves)
    Texture(const Texture&) = delete;

    Texture& operator=(const Texture&) = delete;

    // Moving constructor
    Texture(Texture&& aTex) noexcept
        : m_texId{std::exchange(aTex.m_texId, 0)}
    {
    }

    // Move assignment operator (https://en.cppreference.com/w/cpp/language/move_assignment)
    Texture& operator=(Texture&& aTex) noexcept
    {
        if (this != &aTex)
        {
            // Delete previous texture
            glDeleteTextures(1, &m_texId);
            m_texId = std::exchange(aTex.m_texId, 0);
        }
        return *this;
    }

    // Get the id of the texture
    GLuint getId() const { return m_texId; };

    void initTexture(const int& width, const int& height, const void* data, const GLenum& format, const GLenum& type) const
    {
        glBindTexture(GL_TEXTURE_2D, m_texId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void activateTexture(const int& texUnit) const
    {
        glActiveTexture(GL_TEXTURE0 + texUnit);
        glBindTexture(GL_TEXTURE_2D, m_texId);
    }

    void cubemapTexture(const std::vector<img::Image>& imgData) const
    {
        // BIND TEXTURE
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_texId);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, static_cast<int>(imgData[i].width()), static_cast<int>(imgData[i].height()), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData[i].data());
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // DEBIND TEXTURE
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
};

#endif