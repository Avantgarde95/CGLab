#ifndef ENGINE_MODEL_HPP
#define ENGINE_MODEL_HPP

namespace Engine {
    // Base class of model.
    class Model { // NOLINT
    public:
        enum FillMode {
            FILL = GL_FILL,
            SKELETON = GL_LINE
        };

        enum DrawMode {
            TRIANGLES = GL_TRIANGLES,
            LINES = GL_LINES
        };

        void draw();

        void generateNormalList();

        // Getters.
        glm::mat4 getModelMatrix() const;
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix() const;

        // Setters.
        void setFillMode(FillMode fillMode);
        void setDrawMode(DrawMode drawMode);
        void setProgram(Program *program);
        void setCameraPosition(const glm::vec3 &position);
        void setModelMatrix(const glm::mat4 &matrix);
        void setViewMatrix(const glm::mat4 &matrix);
        void setProjectionMatrix(const glm::mat4 &matrix);

    protected:
        virtual void onCreate();
        virtual void onDraw();

        // Generate VBO and set the index for the attribute.
        void initAttribute(GLuint index, GLint size, GLsizei stride);

        // Set the value of the attributes in the shaders.
        template<typename T>
        void setAttribute(GLuint index, const std::vector<T> &buffer) {
            glBindBuffer(GL_ARRAY_BUFFER, m_attributeCache[index]);
            glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(T), buffer.data(), GL_STATIC_DRAW);
        }

        // Draw all or draw skeleton.
        FillMode m_fillMode = FillMode::FILL;
        // Primitive to use.
        DrawMode m_drawMode = DrawMode::TRIANGLES;

        GLuint m_vertexArrayId;
        bool m_isCreated = false;
        Program *m_program = nullptr;

        glm::vec3 m_cameraPosition;

        // List of vertex positions.
        std::vector<glm::vec3> m_positionList;
        // List of vertex normals.
        std::vector<glm::vec3> m_normalList;

        // Model matrix.
        glm::mat4 m_modelMatrix;
        // View matrix.
        glm::mat4 m_viewMatrix;
        // Projection matrix.
        glm::mat4 m_projectionMatrix;

        std::map<GLuint, GLuint> m_attributeCache;
    };
}

#endif
