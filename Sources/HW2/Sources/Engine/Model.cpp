#include "Engine.hpp"

namespace Engine {
    void Model::create() {
        // Create & bind a VAO.
        glGenVertexArrays(1, &m_vaoId);
        glBindVertexArray(m_vaoId);

        // Allocate VBO for each attribute.
        initAttribute(0, 3, sizeof(glm::vec3));
        initAttribute(1, 3, sizeof(glm::vec3));
        initAttribute(2, 3, sizeof(glm::vec3));
        initAttribute(3, 2, sizeof(glm::vec2));

        // Initialize each attribute's VBO.
        setAttribute(0, m_positionList);
        setAttribute(1, m_normalList);
        setAttribute(2, m_colorList);
        setAttribute(3, m_uvList);

        // Unbind the VAO.
        glBindVertexArray(0);
    }

    void Model::draw() {
        // Update the uniforms.
        glUseProgram(m_programId);

        setUniform("u_modelMatrix", m_modelMatrix);
        setUniform("u_viewMatrix", m_viewMatrix);
        setUniform("u_projectionMatrix", m_projectionMatrix);

        setUniform("u_textureUnit", m_textureUnit);
        setUniform("u_normalMapUnit", m_normalMapUnit);

        setUniform("u_material.ambient", m_material.ambient);
        setUniform("u_material.diffuse", m_material.diffuse);
        setUniform("u_material.specular", m_material.specular);
        setUniform("u_material.shininess", m_material.shininess);

        for (int i = 0; i < m_lightList.size(); i++) {
            auto& light = m_lightList[i];
            std::string name = "u_lightList[" + std::to_string(i) + "]";

            setUniform(name + ".type", light.type);
            setUniform(name + ".position", light.position);
            setUniform(name + ".direction", light.direction);
            setUniform(name + ".ambient", light.ambient);
            setUniform(name + ".diffuse", light.diffuse);
            setUniform(name + ".specular", light.specular);
            setUniform(name + ".angle", light.angle);
            setUniform(name + ".attenuation", light.attenuation);
        }

        // Bind the VAO.
        glBindVertexArray(m_vaoId);

        // Draw the model.
        glPolygonMode(GL_FRONT_AND_BACK, (m_fill ? GL_FILL : GL_LINE));
        glDrawArrays(m_drawMode, 0, static_cast<GLsizei>(m_positionList.size()));

        // Unbind the VAO.
        glBindVertexArray(0);
    }

    glm::mat4 Model::getModelMatrix() const {
        return m_modelMatrix;
    }

    void Model::setFill(bool fill) {
        m_fill = fill;
    }

    void Model::setModelMatrix(const glm::mat4& matrix) {
        m_modelMatrix = matrix;
    }

    void Model::setViewMatrix(const glm::mat4& matrix) {
        m_viewMatrix = matrix;
    }

    void Model::setProjectionMatrix(const glm::mat4& matrix) {
        m_projectionMatrix = matrix;
    }

    void Model::setLight(int index, const Light& light) {
        m_lightList[index] = light;
    }

    void Model::setMaterial(const Material& material) {
        m_material = material;
    }

    void Model::setTexture(const Texture& texture) {
        m_textureUnit = texture.getTextureUnit();
    }

    void Model::setNormalMap(const Texture& normalMap) {
        m_normalMapUnit = normalMap.getTextureUnit();
    }

    void Model::setShader(const Shader& shader) {
        m_programId = shader.getProgramId();
    }

    void Model::initAttribute(GLuint index, GLint size, GLsizei stride) {
        // Generate a VBO for the attribute.
        GLuint id;

        glGenBuffers(1, &id);
        m_attributeCache[index] = id;

        // Set the index of the attribute.
        glEnableVertexAttribArray(index);
        glBindBuffer(GL_ARRAY_BUFFER, m_attributeCache[index]);

        glVertexAttribPointer(
            index,    // index
            size,     // size
            GL_FLOAT, // type
            GL_FALSE, // normalized
            stride,   // stride
            nullptr   // pointer
        );
    }

    template<typename T>
    void Model::setAttribute(GLuint index, const std::vector<T>& buffer) {
        glBindBuffer(GL_ARRAY_BUFFER, m_attributeCache[index]);

        glBufferData(
            GL_ARRAY_BUFFER,
            buffer.size() * sizeof(T),
            &(buffer[0]),
            GL_STATIC_DRAW
        );
    }

    void Model::setUniform(const std::string& name, GLint value) {
        glUniform1i(getUniformLocation(name), value);
    }

    void Model::setUniform(const std::string& name, GLfloat value) {
        glUniform1f(getUniformLocation(name), value);
    }

    void Model::setUniform(const std::string& name, const glm::vec3& value) {
        glUniform3fv(getUniformLocation(name), 1, &(value[0]));
    }

    void Model::setUniform(const std::string& name, const glm::mat4& value) {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &(value[0][0]));
    }

    GLint Model::getUniformLocation(const std::string& name) {
        GLint location;

        if (m_uniformCache.count(name) == 0) {
            location = glGetUniformLocation(m_programId, name.c_str());
            m_uniformCache[name] = location;
        }
        else {
            location = m_uniformCache[name];
        }

        return location;
    }
}
