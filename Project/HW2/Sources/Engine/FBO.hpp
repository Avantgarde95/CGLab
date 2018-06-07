#ifndef ENGINE_FBO_HPP
#define ENGINE_FBO_HPP

#include "Engine.hpp"

namespace Engine {
    // Frame buffer object for implementing render-to-texture technique.
    // (Note that this is a subclass of Texture. So you can use this like a normal texture.)
    class FBO : public Texture {
    public:
        FBO() : Texture("") {}

        // Create & bind a FBO and generate a new texture unit.
        void create() override;

        // Start rendering on the FBO.
        void bind() const;

        // Stop rendering on the FBO.
        void unbind() const;

        void setSize(int width, int height);

    private:
        int m_width;
        int m_height;

        GLuint m_frameBufferId;
        GLuint m_depthBufferId;
    };
}

#endif
