#include "Engine.hpp"

static std::string readFile(const std::string &path);

namespace Engine {
    Shader::Shader(Shader::Type type, const std::string &path) {
        // Read the shader.
        auto code = readFile(path);
        auto codePtr = code.c_str();

        // Compile the shader.
        m_id = glCreateShader(type);

        glShaderSource(m_id, 1, &codePtr, nullptr);
        glCompileShader(m_id);

        // Check the shader.
        GLint result;
        GLint logLength;

        glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 1) {
            auto log = std::vector<char>(static_cast<unsigned>(logLength) + 1);

            glGetShaderInfoLog(m_id, logLength, nullptr, log.data());

            throw std::runtime_error(log.data());
        }
    }

    GLuint Shader::getId() const {
        return m_id;
    }
}

static std::string readFile(const std::string &path) {
    std::string data;
    std::ifstream stream(path, std::ios::in);

    if (!stream.is_open()) {
        throw std::runtime_error("Error: File \"" + path + "\" does not exist.");
    }

    std::string line;

    while (std::getline(stream, line)) {
        data += "\n" + line;
    }

    stream.close();
    return data;
}
