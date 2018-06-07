#include "Engine.hpp"

// We assign odd units(1, 3, 5, ...) for the FBOs.
// (Even units are used for the textures.)
static GLint currTextureUnit = 1;

namespace Engine {
    void FBO::create() {
        // Generate a texture unit.
        m_textureUnit = currTextureUnit;
        currTextureUnit += 2;

        // Create a frame buffer.
        glGenFramebuffers(1, &m_frameBufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);

        // Create an empty texture to render.
        glGenTextures(1, &m_textureId);
        glActiveTexture(GL_TEXTURE0 + m_textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            m_width,
            m_height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            0
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Attach the depth buffer.
        glGenRenderbuffers(1, &m_depthBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBufferId);

        // Configure & check the frame buffer.
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            m_textureId,
            0
        );

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Error: Failed to generate a new frame buffer.\n";
            std::cin.get();
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId);
        glViewport(0, 0, m_width, m_height);
    }

    void FBO::unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_width, m_height);
    }

    void FBO::setSize(int width, int height) {
        m_width = width;
        m_height = height;
    }
}
