#include "App.hpp"

namespace App {
    void MobileNodeModel::create() {
        Engine::Model::create();
        m_wire.create();

        for (auto& child : m_childList) {
            child->create();
        }
    }

    void MobileNodeModel::draw() {
        // Apply the transformations.
        m_rotationAngle += m_rotationSpeed;

        if (m_rotationAngle > 360.0f) {
            m_rotationAngle -= 360.0f;
        }

        m_modelMatrix = glm::translate(m_translation) * glm::rotate(m_rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

        // Apply the parent node's transformation.
        if (m_parent != nullptr) {
            this->m_modelMatrix = m_parent->m_modelMatrix * this->m_modelMatrix;
        }

        // Draw this node.
        Engine::Model::draw();

        // Draw the wire.
        if (m_parent != nullptr) {
            auto startPosition = glm::vec3(m_parent->m_modelMatrix * glm::vec4(m_parent->m_bottomPosition, 1.0f));
            auto endPosition = glm::vec3(this->m_modelMatrix * glm::vec4(this->m_topPosition, 1.0f));

            m_wire.setPositions(startPosition, endPosition);
            m_wire.draw();
        }

        // Draw the child nodes.
        for (auto& child : m_childList) {
            child->draw();
        }
    }

    void MobileNodeModel::addShape(Shape shape) {
        glm::mat4 matrix = shape.getMatrix();
        glm::mat4 inverseMatrix = glm::inverse(matrix);

        inverseMatrix[0][3] = 0.0f;
        inverseMatrix[1][3] = 0.0f;
        inverseMatrix[2][3] = 0.0f;

        glm::mat4 normalMatrix = glm::transpose(inverseMatrix);

        for (auto& it : shape.getPositionList()) {
            m_positionList.emplace_back(matrix * glm::vec4(it, 1.0f));
        }

        for (auto& it : shape.getNormalList()) {
            m_normalList.emplace_back(normalMatrix * glm::vec4(it, 1.0f));
        }

        for (auto& it : shape.getColorList()) {
            m_colorList.emplace_back(it);
        }

        for (auto& it : shape.getUVList()) {
            m_uvList.emplace_back(it);
        }
    }

    void MobileNodeModel::addChild(MobileNodeModel& child) {
        m_childList.emplace_back(&child);
        child.m_parent = this;
    }

    void MobileNodeModel::resetRotationSpeed() {
        m_rotationSpeed = 0.0f;
    }

    void MobileNodeModel::addRotationSpeed(float value) {
        m_rotationSpeed += value;
    }

    void MobileNodeModel::setTranslation(const glm::vec3& translation) {
        m_translation = translation;
    }

    void MobileNodeModel::setTopPosition(const glm::vec3& position) {
        m_topPosition = position;
    }

    void MobileNodeModel::setBottomPosition(const glm::vec3& position) {
        m_bottomPosition = position;
    }

    void MobileNodeModel::setViewMatrix(const glm::mat4& matrix) {
        Engine::Model::setViewMatrix(matrix);
        m_wire.setViewMatrix(matrix);

        for (auto& child : m_childList) {
            child->setViewMatrix(matrix);
        }
    }

    void MobileNodeModel::setProjectionMatrix(const glm::mat4& matrix) {
        Engine::Model::setProjectionMatrix(matrix);
        m_wire.setProjectionMatrix(matrix);

        for (auto& child : m_childList) {
            child->setProjectionMatrix(matrix);
        }
    }

    void MobileNodeModel::setLight(int index, const Engine::Light& light) {
        Engine::Model::setLight(index, light);
        m_wire.setLight(index, light);

        for (auto& child : m_childList) {
            child->setLight(index, light);
        }
    }

    void MobileNodeModel::setMaterial(const Engine::Material& material) {
        Engine::Model::setMaterial(material);
        m_wire.setMaterial(material);

        for (auto& child : m_childList) {
            child->setMaterial(material);
        }
    }

    void MobileNodeModel::setTexture(const Engine::Texture& texture) {
        Engine::Model::setTexture(texture);
        m_wire.setTexture(texture);

        for (auto& child : m_childList) {
            child->setTexture(texture);
        }
    }

    void MobileNodeModel::setNormalMap(const Engine::Texture& normalMap) {
        Engine::Model::setNormalMap(normalMap);
        m_wire.setTexture(normalMap);

        for (auto& child : m_childList) {
            child->setNormalMap(normalMap);
        }
    }

    void MobileNodeModel::setShader(const Engine::Shader& shader) {
        Engine::Model::setShader(shader);
        m_wire.setShader(shader);

        for (auto& child : m_childList) {
            child->setShader(shader);
        }
    }
}
