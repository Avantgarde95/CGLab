#ifndef ENGINE_LIGHT_MODEL_HPP
#define ENGINE_LIGHT_MODEL_HPP

#include "Engine.hpp"

namespace Engine {
    // Mixin for enabling lighting.
    template<typename T>
    class LightModel : public T {
    public:
        LightModel() {
            for (auto &light: m_lightList) {
                light.type = Light::Type::OFF;
            }
        }

        void setLight(int index, const Light &light) {
            m_lightList[index] = light;
        }

    protected:
        virtual void onDraw() {
            T::onDraw();

            for (int i = 0; i < m_lightList.size(); i++) {
                auto &light = m_lightList[i];
                std::string name = "lightList[" + std::to_string(i) + "]";

                this->m_program->setUniform(name + ".type", static_cast<GLint>(light.type));
                this->m_program->setUniform(name + ".position", light.position);
                this->m_program->setUniform(name + ".direction", light.direction);
                this->m_program->setUniform(name + ".ambient", light.ambient);
                this->m_program->setUniform(name + ".diffuse", light.diffuse);
                this->m_program->setUniform(name + ".specular", light.specular);
                this->m_program->setUniform(name + ".angle", light.angle);
                this->m_program->setUniform(name + ".attenuation", light.attenuation);
            }
        }

        std::vector<Light> m_lightList{5};
    };
}

#endif
