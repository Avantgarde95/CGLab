#ifndef ENGINE_MODEL_HPP
#define ENGINE_MODEL_HPP

#include "Engine.hpp"

namespace Engine {
    // Base class for the models.
    class Model {
    public:
        // Create the model.
        virtual void create();

        // Draw the model.
        virtual void draw();

        glm::mat4 getModelMatrix() const;

        void setFill(bool fill);

        void setModelMatrix(const glm::mat4& matrix);
        virtual void setViewMatrix(const glm::mat4& matrix);
        virtual void setProjectionMatrix(const glm::mat4& matrix);

        virtual void setLight(int index, const Light& light);
        virtual void setMaterial(const Material& material);

        virtual void setTexture(const Texture& texture);
        virtual void setNormalMap(const Texture& normalMap);
        virtual void setShader(const Shader& shader);

    protected:
        // Generate a VBO for the attribute and set the index.
        void initAttribute(GLuint index, GLint size, GLsizei stride);

        // Set the value of the attribute.
        template<typename T> void setAttribute(GLuint index, const std::vector<T>& buffer);

        // Set the value of the uniform.
        void setUniform(const std::string& name, GLint value);
        void setUniform(const std::string& name, GLfloat value);
        void setUniform(const std::string& name, const glm::vec3& value);
        void setUniform(const std::string& name, const glm::mat4& value);

        // Retrieve the location of the uniform.
        GLint getUniformLocation(const std::string& name);

        // List of vertex positions.
        std::vector<glm::vec3> m_positionList;
        // List of normal vectors.
        std::vector<glm::vec3> m_normalList;
        // List of colors. (RGB)
        std::vector<glm::vec3> m_colorList;
        // List of texture coordinates. (UV)
        std::vector<glm::vec2> m_uvList;

        // Model matrix.
        glm::mat4 m_modelMatrix;
        // View matrix. (= inverse(Eye's model matrix))
        glm::mat4 m_viewMatrix;
        // Projection matrix.
        glm::mat4 m_projectionMatrix;

        // Material information.
        Material m_material;
        // List of light information. (Maximum: 5)
        std::vector<Light> m_lightList{ 5 };

        // Whether we draw the whole object or only its wireframe.
        bool m_fill = true;
        // OpenGL drawing mode. (ex. GL_TRIANGLES, GL_LINES, etc.)
        GLenum m_drawMode = GL_TRIANGLES;

        // Texture unit.
        GLint m_textureUnit;
        // Normal map unit.
        GLint m_normalMapUnit;
        // Program id.
        GLuint m_programId;
        // VAO id.
        GLuint m_vaoId;

        // Map of (attribute index, VBO id).
        std::map<GLuint, GLuint> m_attributeCache;
        // Map of (uniform name, uniform location).
        std::map<std::string, GLint> m_uniformCache;
    };
}

#endif
