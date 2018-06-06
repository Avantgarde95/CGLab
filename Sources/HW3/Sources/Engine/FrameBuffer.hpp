#ifndef ENGINE_FRAMEBUFFER_HPP
#define ENGINE_FRAMEBUFFER_HPP

#include "Engine.hpp"

namespace Engine {
    // Frame buffer object which implements render-to-texture technique.
    class FrameBuffer {
    public:
        FrameBuffer(GLsizei width, GLsizei height);

        // Bind & unbind the frame buffer.
        void bind() const;
        void unbind() const;

        Texture *getColorTexture();
        Texture *getDepthTexture();

        void setSize(GLsizei width, GLsizei height);

    private:
        // Color texture.
        Texture m_colorTexture;
        // Depth texture. (We can use this for shadow mapping.)
        Texture m_depthTexture;

        GLsizei m_width;
        GLsizei m_height;
        GLuint m_frameBufferId;
        GLuint m_renderBufferId;
    };
}

#endif
