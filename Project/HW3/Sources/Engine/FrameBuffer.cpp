#include "Engine.hpp"

namespace Engine {
    FrameBuffer::FrameBuffer(GLsizei width, GLsizei height) :
            m_width(width),
            m_height(height),
            m_colorTexture(width, height, {nullptr}, GL_RGB, GL_RGB, false),
            m_depthTexture(width, height, {nullptr}, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, false) {
        // Create a frame buffer.
        glGenFramebuffers(1, &m_frameBufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

        // Attach the depth buffer.
        glGenRenderbuffers(1, &m_renderBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);

        glFramebufferRenderbuffer(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                GL_RENDERBUFFER,
                m_renderBufferId
        );

        // Configure & check the frame buffer.
        glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                m_colorTexture.getId(),
                0
        );

        glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D,
                m_depthTexture.getId(),
                0
        );

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Error: Failed to generate a new frame buffer.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
        glViewport(0, 0, m_width, m_height);
    }

    void FrameBuffer::unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_width, m_height);
    }

    Texture *FrameBuffer::getColorTexture() {
        return &m_colorTexture;
    }

    Texture *FrameBuffer::getDepthTexture() {
        return &m_depthTexture;
    }

    void FrameBuffer::setSize(GLsizei width, GLsizei height) {
        m_width = width;
        m_height = height;
    }
}
