#ifndef ENGINE_SHADER_HPP
#define ENGINE_SHADER_HPP

namespace Engine {
    // Class for handling a pair of vertex shader and a fragment shader.
    class Shader {
    public:
        Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

        // Read & compile the shaders and generate a new program id.
        void create();

        GLuint getProgramId() const;

    private:
        std::string m_vertexShaderPath;
        std::string m_fragmentShaderPath;

        GLuint m_programId;
    };
}

#endif
