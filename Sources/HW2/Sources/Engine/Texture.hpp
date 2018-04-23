#ifndef ENGINE_TEXTURE_HPP
#define ENGINE_TEXTURE_HPP

#include "Engine.hpp"

// Since SOIL does not work in my computer, I wrote a simple PPM reader.
// Currently, only .ppm files without commments ("# blahblah...") are supported.

namespace Engine {
    // Class for handling a texture.
    class Texture {
    public:
        Texture(const std::string& path);

        // Read & bind the texture.
        virtual void create();

        GLint getTextureUnit() const;

    protected:
        // Texture path.
        std::string m_texturePath;

        // Texture unit.
        GLint m_textureUnit;

        // Texture id.
        GLuint m_textureId;
    };
}

#endif
