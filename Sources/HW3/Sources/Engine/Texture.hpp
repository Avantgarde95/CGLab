#ifndef ENGINE_TEXTURE_HPP
#define ENGINE_TEXTURE_HPP

#include "Engine.hpp"

namespace Engine {
    // Texture object.
    class Texture {
    public:
        // Constructor: Use the image file.
        explicit Texture(const std::string &path);

        // Constructor: Provide the data directly.
        Texture(
                GLsizei width,
                GLsizei height,
                const std::vector<GLvoid *> &dataList,
                GLint internalFormat,
                GLenum format,
                bool isCubeMap
        );

        GLuint getId() const;
        GLint getUnit() const;

    private:
        void create(
                GLsizei width,
                GLsizei height,
                const std::vector<GLvoid *> &dataList,
                GLint internalFormat,
                GLenum format,
                bool isCubeMap
        );

        GLint m_unit;
        GLuint m_id;
    };
}

#endif
