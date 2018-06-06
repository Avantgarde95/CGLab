#include "App.hpp"

namespace App {
    void GeneralModel::select(bool isSelected) {
        m_isSelected = isSelected;
    }

    void GeneralModel::setBrushTexture(Engine::Texture *texture) {
        m_brushTexture = texture;
    }

    void GeneralModel::onDraw() {
        GeneralBaseModel::onDraw();

        if (m_brushTexture == nullptr) {
            throw std::runtime_error("Error: Brush texture is not set.");
        }

        m_program->setUniform("brushTextureUnit", m_brushTexture->getUnit());
        m_program->setUniform("isSelected", m_isSelected);
    }
}
