#ifndef ENGINE_SHADOW_MODEL_HPP
#define ENGINE_SHADOW_MODEL_HPP

#include "Engine.hpp"

namespace Engine {
    // Mixin for enabling shadow mapping.
    template<typename T>
    class ShadowModel : public T {
    public:
        void setShadowMap(Texture *shadowMap) {
            m_shadowMap = shadowMap;
        }

        void setLightViewMatrix(const glm::mat4 &matrix) {
            m_lightViewMatrix = matrix;
        }

        void setLightProjectionMatrix(const glm::mat4 &matrix) {
            m_lightProjectionMatrix = matrix;
        }

    protected:
        virtual void onDraw() {
            T::onDraw();

            if (m_shadowMap == nullptr) {
                throw std::runtime_error("Error: Shadow map is not set.");
            }

            this->m_program->setUniform("shadowMapUnit", m_shadowMap->getUnit());
            this->m_program->setUniform("lightViewMatrix", m_lightViewMatrix);
            this->m_program->setUniform("lightProjectionMatrix", m_lightProjectionMatrix);
        }

        Texture *m_shadowMap = nullptr;

        glm::mat4 m_lightViewMatrix;
        glm::mat4 m_lightProjectionMatrix;
    };
}

#endif
