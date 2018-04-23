#ifndef APP_SHAPE_HPP
#define APP_SHAPE_HPP

#include "App.hpp"

namespace App {
    // Primitive generator.
    class Shape {
    public:
        Shape & buildPyramid();
        Shape & buildPrism();
        Shape & buildCone();
        Shape & buildCylinder();

        std::vector<glm::vec3>& getPositionList();
        std::vector<glm::vec3>& getNormalList();
        std::vector<glm::vec3>& getColorList();
        std::vector<glm::vec2>& getUVList();

        glm::mat4& getMatrix();

        Shape & setMatrix(const glm::mat4& matrix);
        Shape & setAttribute(const std::string& key, float value);
        Shape & setAttribute(const std::string& key, const glm::vec3& value);

    private:
        void addTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 color);

        // Matrix for positioning the primitive.
        glm::mat4 m_matrix;

        std::vector<glm::vec3> m_positionList;
        std::vector<glm::vec3> m_normalList;
        std::vector<glm::vec3> m_colorList;
        std::vector<glm::vec2> m_uvList;

        // Scalar attributes. (ex. height, radius)
        std::map<std::string, float> m_valueAttributeMap;
        // Vector attributes. (ex. color)
        std::map<std::string, glm::vec3> m_vectorAttributeMap;
    };
}

#endif
