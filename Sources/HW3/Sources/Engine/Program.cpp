#include "Engine.hpp"

namespace Engine {
    Program::Program(std::initializer_list<Engine::Shader *> shaderList) {
        // Create a program.
        m_id = glCreateProgram();

        // Link the shaders.
        for (auto &it : shaderList) {
            glAttachShader(m_id, it->getId());
        }

        glLinkProgram(m_id);

        for (auto &it : shaderList) {
            glDetachShader(m_id, it->getId());
        }

        // Check the program.
        GLint result;
        GLint logLength;

        glGetProgramiv(m_id, GL_COMPILE_STATUS, &result);
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 1) {
            std::vector<char> log(static_cast<unsigned>(logLength + 1));

            glGetProgramInfoLog(m_id, logLength, nullptr, log.data());

            throw std::runtime_error(log.data());
        }
    }

    void Program::use() {
        glUseProgram(getId());
    }

    void Program::setUniform(const std::string &name, GLint value) {
        glUniform1i(getUniformLocation(name), value);
    }

    void Program::setUniform(const std::string &name, GLfloat value) {
        glUniform1f(getUniformLocation(name), value);
    }

    void Program::setUniform(const std::string &name, const glm::vec3 &value) {
        glUniform3fv(getUniformLocation(name), 1, &(value[0]));
    }

    void Program::setUniform(const std::string &name, const glm::mat4 &value) {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &(value[0][0]));
    }

    GLuint Program::getId() const {
        return m_id;
    }

    GLint Program::getUniformLocation(const std::string &name) {
        GLint location;

        if (m_uniformCache.count(name) == 0) {
            location = glGetUniformLocation(getId(), name.c_str());
            m_uniformCache[name] = location;
        }
        else {
            location = m_uniformCache[name];
        }

        return location;
    }
}
