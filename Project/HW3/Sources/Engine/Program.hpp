#ifndef ENGINE_PROGRAM_HPP
#define ENGINE_PROGRAM_HPP

namespace Engine {
    // Program object.
    class Program {
    public:
        explicit Program(std::initializer_list<Shader *> shaderList);

        // Use(glUseProgram) the program.
        void use();

        // Set the value of the uniform in the shaders.
        void setUniform(const std::string &name, GLint value);
        void setUniform(const std::string &name, GLfloat value);
        void setUniform(const std::string &name, const glm::vec3 &value);
        void setUniform(const std::string &name, const glm::mat4 &value);

        GLuint getId() const;

    private:
        GLint getUniformLocation(const std::string &name);

        GLuint m_id;

        std::map<std::string, GLint> m_uniformCache;
    };
}

#endif
