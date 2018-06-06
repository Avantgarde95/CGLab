#ifndef ENGINE_TEXTURE_MODEL_HPP
#define ENGINE_TEXTURE_MODEL_HPP

#include "Engine.hpp"

namespace Engine {
    // Mixin for enabling texture mapping.
    template<typename T>
    class TextureModel : public T {
    public:
        // Generate UVs using the vertex positions.
        void generateUVList() {
            auto pi = 3.1415926535f;
            auto halfPi = pi / 2.0f;

            for (auto &position: this->m_positionList) {
                // Generate UVs using cylindrical coordinates.
                auto polarPosition = glm::polar(position);

                m_uvList.emplace_back(
                        (polarPosition.x + halfPi) / pi,
                        (polarPosition.y + halfPi) / pi
                );
            }
        }

        void setTexture(Texture *texture) {
            m_texture = texture;
        }

    protected:
        virtual void onCreate() {
            T::onCreate();

            this->initAttribute(2, 2, sizeof(glm::vec2));
            this->setAttribute(2, m_uvList);
        }

        virtual void onDraw() {
            T::onDraw();

            if (m_texture == nullptr) {
                throw std::runtime_error("Error: Texture is not set.");
            }

            this->m_program->setUniform("textureUnit", m_texture->getUnit());
        }

        Texture *m_texture = nullptr;

        std::vector<glm::vec2> m_uvList;
    };
}

#endif
