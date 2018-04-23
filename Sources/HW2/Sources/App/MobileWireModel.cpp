#include "App.hpp"

namespace App {
    MobileWireModel::MobileWireModel() : Engine::Model() {
        m_drawMode = GL_LINES;

        m_positionList.emplace_back(0.0f, 0.0f, 0.0f);
        m_positionList.emplace_back(0.0f, 0.0f, 0.0f);
        m_normalList.emplace_back(0.0f, 1.0f, 0.0f);
        m_normalList.emplace_back(0.0f, 1.0f, 0.0f);
        m_colorList.emplace_back(1.0f, 1.0f, 1.0f);
        m_colorList.emplace_back(1.0f, 1.0f, 1.0f);
        m_uvList.emplace_back(0.0f, 0.0f);
        m_uvList.emplace_back(1.0f, 1.0f);
    }

    void MobileWireModel::setPositions(const glm::vec3& startPosition, const glm::vec3& endPosition) {
        m_positionList[0] = startPosition;
        m_positionList[1] = endPosition;

        // Reset the VBO to apply the change.
        setAttribute(0, m_positionList);
    }
}
