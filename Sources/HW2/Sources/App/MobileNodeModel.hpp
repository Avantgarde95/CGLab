#ifndef APP_MOBILE_NODE_MODEL_HPP
#define APP_MOBILE_NODE_MODEL_HPP

#include "App.hpp"

namespace App {
    // Each node of the mobile. (Hierarchical modeling.)
    class MobileNodeModel : public Engine::Model {
    public:
        void create() override;

        void draw() override;

        // Add a shape(primitive).
        void addShape(Shape shape);

        // Add a child node.
        void addChild(MobileNodeModel& child);

        // Reset the rotation speed.
        void resetRotationSpeed();
        // Increment the rotation speed.
        void addRotationSpeed(float value);

        // Set the translation.
        void setTranslation(const glm::vec3& translation);
        // Set the 'top' position. (Connected to the parent node's 'bottom' position.)
        void setTopPosition(const glm::vec3& position);
        // Set the 'bottom' position. (Connected to the child nodes' 'top' positions.)
        void setBottomPosition(const glm::vec3& position);

        // Let the setters to apply the change to the child nodes, too.
        void setViewMatrix(const glm::mat4& matrix) override;
        void setProjectionMatrix(const glm::mat4& matrix) override;
        void setLight(int index, const Engine::Light& light) override;
        void setMaterial(const Engine::Material& material) override;
        void setTexture(const Engine::Texture& texture) override;
        void setNormalMap(const Engine::Texture& normalMap) override;
        void setShader(const Engine::Shader& shader) override;

    private:
        // Parent node. (If nullptr -> 'Parent' = World)
        MobileNodeModel * m_parent = nullptr;
        // List of the child nodes.
        std::vector<MobileNodeModel*> m_childList;

        // A line which connects this node and the parent node.
        MobileWireModel m_wire;

        // Translation w.r.t the parent node.
        glm::vec3 m_translation;
        // Rotation angle w.r.t the parent node.
        float m_rotationAngle = 0.0f;
        // Rotation speed w.r.t the parent node.
        float m_rotationSpeed = 0.0f;

        // Top position: Connected to parent node's bottom position.
        glm::vec3 m_topPosition;
        // Bottom position: Connected to child nodes' top positions.
        glm::vec3 m_bottomPosition;
    };
}

#endif
