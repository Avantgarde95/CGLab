#include "App.hpp"

namespace App {
    App::Shape & Shape::buildPyramid() {
        float height = m_valueAttributeMap["height"];
        float radius = m_valueAttributeMap["radius"];
        int level = static_cast<int>(m_valueAttributeMap["level"]);
        glm::vec3 color = m_vectorAttributeMap["color"];

        glm::vec3 apex{ 0, height / 2.0f, 0 };
        glm::vec3 baseCenter{ 0, -height / 2.0f, 0 };
        std::vector<glm::vec3> baseVertices;
        float angle = (2.0f * 3.141592f) / level;

        for (int i = 0; i < level + 1; i++) {
            baseVertices.emplace_back(
                baseCenter.x + radius * std::sin(angle * i),
                baseCenter.y,
                baseCenter.z + radius * std::cos(angle * i)
            );
        }

        for (int i = 0; i < level; i++) {
            glm::vec3 v0 = baseVertices[i];
            glm::vec3 v1 = baseVertices[i + 1];

            addTriangle(v0, v1, apex, color);
            addTriangle(v1, v0, baseCenter, color);
        }

        return *this;
    }

    App::Shape & Shape::buildPrism() {
        float height = m_valueAttributeMap["height"];
        float radius = m_valueAttributeMap["radius"];
        int level = static_cast<int>(m_valueAttributeMap["level"]);
        glm::vec3 color = m_vectorAttributeMap["color"];

        glm::vec3 topCenter(0, height / 2.0f, 0);
        glm::vec3 bottomCenter(0, -height / 2.0f, 0);
        std::vector<glm::vec3> topVertices;
        std::vector<glm::vec3> bottomVertices;
        float angle = (2.0f * 3.141592f) / level;

        for (int i = 0; i < level + 1; i++) {
            topVertices.emplace_back(
                topCenter.x + radius * std::sin(angle * i),
                topCenter.y,
                topCenter.z + radius * std::cos(angle * i)
            );

            bottomVertices.emplace_back(
                bottomCenter.x + radius * std::sin(angle * i),
                bottomCenter.y,
                bottomCenter.z + radius * std::cos(angle * i)
            );
        }

        for (int i = 0; i < level; i++) {
            glm::vec3 v0 = topVertices[i];
            glm::vec3 v1 = topVertices[i + 1];
            glm::vec3 w0 = bottomVertices[i];
            glm::vec3 w1 = bottomVertices[i + 1];

            addTriangle(v0, v1, topCenter, color);
            addTriangle(v0, w0, w1, color);
            addTriangle(w1, v1, v0, color);
            addTriangle(w1, w0, bottomCenter, color);
        }

        return *this;
    }

    App::Shape & Shape::buildCone() {
        return setAttribute("level", 30).buildPyramid();
    }

    App::Shape & Shape::buildCylinder() {
        return setAttribute("level", 30).buildPrism();
    }

    std::vector<glm::vec3>& Shape::getPositionList() {
        return m_positionList;
    }

    std::vector<glm::vec3>& Shape::getNormalList() {
        return m_normalList;
    }

    std::vector<glm::vec3>& Shape::getColorList() {
        return m_colorList;
    }

    std::vector<glm::vec2>& Shape::getUVList() {
        return m_uvList;
    }

    glm::mat4& Shape::getMatrix() {
        return m_matrix;
    }

    App::Shape & Shape::setMatrix(const glm::mat4& matrix) {
        m_matrix = matrix;
        return *this;
    }

    App::Shape & Shape::setAttribute(const std::string& key, float value) {
        m_valueAttributeMap[key] = value;
        return *this;
    }

    App::Shape & Shape::setAttribute(const std::string& key, const glm::vec3& value) {
        m_vectorAttributeMap[key] = value;
        return *this;
    }

    void Shape::addTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 color) {
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v1));
        glm::vec3 p0 = glm::polar(v0);
        glm::vec3 p1 = glm::polar(v1);
        glm::vec3 p2 = glm::polar(v2);
        float pi = 3.141592f;
        float halfPi = pi / 2.0f;

        m_positionList.emplace_back(v0);
        m_positionList.emplace_back(v1);
        m_positionList.emplace_back(v2);

        m_colorList.emplace_back(color);
        m_colorList.emplace_back(color);
        m_colorList.emplace_back(color);

        m_normalList.emplace_back(normal);
        m_normalList.emplace_back(normal);
        m_normalList.emplace_back(normal);

        m_uvList.emplace_back((p0.x + halfPi) / pi, (p0.y + halfPi) / pi);
        m_uvList.emplace_back((p1.x + halfPi) / pi, (p1.y + halfPi) / pi);
        m_uvList.emplace_back((p2.x + halfPi) / pi, (p2.y + halfPi) / pi);
    }
}
