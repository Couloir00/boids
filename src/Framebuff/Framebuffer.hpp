// buffer initialization from
// https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
//

#ifndef __FRAMEBUFFER_HPP_
#define __FRAMEBUFFER_HPP_

#include <array>
#include <utility>
#include "Texture/Texture.hpp"
#include "p6/p6.h"

// defining a framebuffer to handle shadow (same way as texture or vao/vbo)

class FrameBuffer {
private:
    GLuint  m_bufferId{};
    Texture m_DepthMap;
    int     m_shadowWidth{};
    int     m_shadowHeight{};

public:
    // framebuff constructor
    FrameBuffer(const int& shadowWidth, const int& shadowHeight)
        : m_shadowWidth(shadowWidth), m_shadowHeight(shadowHeight)
    {
        // init
        glGenFramebuffers(1, &m_bufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_bufferId);
        glBindTexture(GL_TEXTURE_2D, m_DepthMap.getId());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowWidth, m_shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        std::array<float, 4> borderColor = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor.data());
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthMap.getId(), 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // framebuff destructor
    ~FrameBuffer() { glDeleteFramebuffers(1, &m_bufferId); }

    // copy construct (copy not allowed to prevent dumb moves)
    FrameBuffer(const FrameBuffer&) = delete;

    FrameBuffer& operator=(const FrameBuffer&) = delete;

    // Moving constructor
    FrameBuffer(FrameBuffer&& aBuff) noexcept
        : m_bufferId{std::exchange(aBuff.m_bufferId, 0)}
    {
    }

    // Move assignment operator (https://en.cppreference.com/w/cpp/language/move_assignment)
    FrameBuffer& operator=(FrameBuffer&& aBuff) noexcept
    {
        if (this != &aBuff)
        {
            // Delete previous framebuff
            glDeleteFramebuffers(1, &m_bufferId);
            std::exchange(aBuff.m_bufferId, 0);
        }
        return *this;
    }

    // Get the id of the framebuff
    GLuint getId() const { return m_bufferId; };

    GLuint getTexId() const { return m_DepthMap.getId(); }
    int    getShadowWidth() const { return m_shadowWidth; }
    int    getShadowHeight() const { return m_shadowHeight; }
};
#endif