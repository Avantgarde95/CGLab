#include "Engine.hpp"

namespace Engine {
    void Model::draw() {
        if (!m_isCreated) {
            glGenVertexArrays(1, &m_vertexArrayId);

            glBindVertexArray(m_vertexArrayId);
            onCreate();
            glBindVertexArray(0);

            m_isCreated = true;
        }

        if (m_program == nullptr) {
            std::runtime_error("Error: Program is not set.");
        }

        m_program->use();

        onDraw();

        glBindVertexArray(m_vertexArrayId);
        glPolygonMode(GL_FRONT_AND_BACK, m_fillMode);
        glDrawArrays(m_drawMode, 0, static_cast<GLsizei>(m_positionList.size()));
        glBindVertexArray(0);
    }

    void Model::generateNormalList() {
        for (int i = 0; i + 2 < m_positionList.size(); i += 3) {
            glm::vec3 v0 = m_positionList[i];
            glm::vec3 v1 = m_positionList[i + 1];
            glm::vec3 v2 = m_positionList[i + 2];
            glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

            m_normalList.emplace_back(normal);
            m_normalList.emplace_back(normal);
            m_normalList.emplace_back(normal);
        }
    }

    glm::mat4 Model::getModelMatrix() const {
        return m_modelMatrix;
    }

    glm::mat4 Model::getViewMatrix() const {
        return m_viewMatrix;
    }

    glm::mat4 Model::getProjectionMatrix() const {
        return m_projectionMatrix;
    }

    void Model::setFillMode(FillMode fillMode) {
        m_fillMode = fillMode;
    }

    void Model::setDrawMode(DrawMode drawMode) {
        m_drawMode = drawMode;
    }

    void Model::setProgram(Program *program) {
        m_program = program;
    }

    void Model::setCameraPosition(const glm::vec3 &position) {
        m_cameraPosition = position;
    }

    void Model::setModelMatrix(const glm::mat4 &matrix) {
        m_modelMatrix = matrix;
    }

    void Model::setViewMatrix(const glm::mat4 &matrix) {
        m_viewMatrix = matrix;
    }

    void Model::setProjectionMatrix(const glm::mat4 &matrix) {
        m_projectionMatrix = matrix;
    }

    void Model::onCreate() {
        initAttribute(0, 3, sizeof(glm::vec3));
        initAttribute(1, 3, sizeof(glm::vec3));

        setAttribute(0, m_positionList);
        setAttribute(1, m_normalList);
    }

    void Model::onDraw() {
        m_program->setUniform("cameraPosition", m_cameraPosition);
        m_program->setUniform("modelMatrix", m_modelMatrix);
        m_program->setUniform("viewMatrix", m_viewMatrix);
        m_program->setUniform("projectionMatrix", m_projectionMatrix);
    }

    void Model::initAttribute(GLuint index, GLint size, GLsizei stride) {
        // Generate a VBO for the attribute.
        GLuint vboId;

        glGenBuffers(1, &vboId);
        m_attributeCache[index] = vboId;

        // Set the index of the attribute.
        glEnableVertexAttribArray(index);
        glBindBuffer(GL_ARRAY_BUFFER, m_attributeCache[index]);
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, nullptr);
    }
}
