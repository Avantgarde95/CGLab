#ifndef ENGINE_SHADER_HPP
#define ENGINE_SHADER_HPP

#include "Engine.hpp"

namespace Engine {
    // Shader object.
    class Shader {
    public:
        enum Type {
            VERTEX = GL_VERTEX_SHADER,
            FRAGMENT = GL_FRAGMENT_SHADER
        };

        Shader(Type type, const std::string &path);

        GLuint getId() const;

    private:
        GLuint m_id;
    };
}

#endif
