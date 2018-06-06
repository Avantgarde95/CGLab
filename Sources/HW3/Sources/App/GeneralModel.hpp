#ifndef APP_COMMON_MODEL_HPP
#define APP_COMMON_MODEL_HPP

#include "App.hpp"

namespace App {
    using GeneralBaseModel = Engine::LightModel<Engine::ShadowModel<Engine::TextureModel<Engine::Model>>>;

    class GeneralModel : public GeneralBaseModel {
    public:
        // Select the model and apply the special effect.
        void select(bool isSelected);

        void setBrushTexture(Engine::Texture *texture);

    private:
        void onDraw() override;

        Engine::Texture *m_brushTexture = nullptr;

        GLint m_isSelected = false;
    };
}

#endif
