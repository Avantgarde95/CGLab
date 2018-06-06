#ifndef ENGINE_DISPLAY_MODEL_HPP
#define ENGINE_DISPLAY_MODEL_HPP

#include "App.hpp"

namespace App {
    class DisplayModel : public Engine::TextureModel<Engine::Model> {
    public:
        explicit DisplayModel(float size);

        GLint getResolution() const;

        void setResolution(GLint resolution);
        void setDepthMap(Engine::Texture *depthMap);

    private:
        void onDraw() override;

        Engine::Texture *m_depthMap = nullptr;

        GLint m_resolution = 160;
    };
}

#endif
