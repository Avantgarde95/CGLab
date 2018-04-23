#include "Engine.hpp"

static std::string readFile(const std::string& path);
static GLuint compileShader(GLenum type, const std::string& code);
static void checkShader(GLuint id);
static GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId);
static void checkProgram(GLuint id);

namespace Engine {
    Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
        : m_vertexShaderPath(vertexShaderPath), m_fragmentShaderPath(fragmentShaderPath) {
    }

    void Shader::create() {
        // Compile & check the vertex shader.
        GLuint vertexShaderId = compileShader(GL_VERTEX_SHADER, readFile(m_vertexShaderPath));
        checkShader(vertexShaderId);

        // Compile & check the fragment shader.
        GLuint fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, readFile(m_fragmentShaderPath));
        checkShader(fragmentShaderId);

        // Create a program and attach the shaders.
        GLuint programId = linkProgram(vertexShaderId, fragmentShaderId);
        checkProgram(programId);

        m_programId = programId;
    }

    GLuint Shader::getProgramId() const {
        return m_programId;
    }
}

static std::string readFile(const std::string& path) {
    std::string data;
    std::ifstream stream(path, std::ios::in);

    if (!stream.is_open()) {
        std::cout << "Error: File \"" << path << "\" does not exist.\n";
        std::cin.get();
        return "";
    }

    std::string line = "";

    while (std::getline(stream, line)) {
        data += "\n" + line;
    }

    stream.close();
    return data;
}

static GLuint compileShader(GLenum type, const std::string& code) {
    GLuint id = glCreateShader(type);
    const char* codePtr = code.c_str();

    glShaderSource(id, 1, &codePtr, NULL);
    glCompileShader(id);

    return id;
}

static void checkShader(GLuint id) {
    GLint result = GL_FALSE;
    GLint logLength = 0;

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 1) {
        std::vector<char> log(logLength + 1);

        glGetShaderInfoLog(id, logLength, NULL, &(log[0]));
        std::cout << &(log[0]) << "\n";
        std::cin.get();
    }
}

static GLuint linkProgram(GLuint vertexShaderId, GLuint fragmentShaderId) {
    GLuint id = glCreateProgram();

    glAttachShader(id, vertexShaderId);
    glAttachShader(id, fragmentShaderId);
    glLinkProgram(id);
    glDetachShader(id, vertexShaderId);
    glDetachShader(id, fragmentShaderId);

    return id;
}

static void checkProgram(GLuint id) {
    GLint result = GL_FALSE;
    GLint logLength = 0;

    glGetProgramiv(id, GL_COMPILE_STATUS, &result);
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 1) {
        std::vector<char> log(logLength + 1);

        glGetProgramInfoLog(id, logLength, NULL, &(log[0]));
        std::cout << "Error: " << &(log[0]) << "\n";
        std::cin.get();
    }
}
