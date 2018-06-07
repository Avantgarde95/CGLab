#include "App.hpp"

namespace App {
    DisplayModel::DisplayModel(float size) {
        std::vector<glm::vec2> xyList{
                {1, 1},
                {0, 0},
                {1, 0},
                {1, 1},
                {0, 1},
                {0, 0}
        };

        for (auto &xy : xyList) {
            m_positionList.emplace_back(
                    (xy.x - 0.5f) * size,
                    (xy.y - 0.5f) * size,
                    0.0f
            );

            m_normalList.emplace_back(0.0f, 0.0f, 1.0f);
            m_uvList.emplace_back(xy.x, xy.y);
        }
    }

    GLint DisplayModel::getResolution() const {
        return m_resolution;
    }

    void DisplayModel::setResolution(GLint resolution) {
        if (resolution < 10) {
            m_resolution = 10;
        }
        else if (resolution > 1210) {
            m_resolution = 1210;
        }
        else {
            m_resolution = resolution;
        }
    }

    void DisplayModel::setDepthMap(Engine::Texture *depthMap) {
        m_depthMap = depthMap;
    }

    void DisplayModel::onDraw() {
        Engine::TextureModel<Engine::Model>::onDraw();

        if (m_depthMap == nullptr) {
            throw std::runtime_error("Error: Depth map is not set.");
        }

        m_program->setUniform("depthMapUnit", m_depthMap->getUnit());
        m_program->setUniform("resolution", m_resolution);
    }
}
