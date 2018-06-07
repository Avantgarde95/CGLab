#include "App.hpp"

namespace App {
    DisplayModel::DisplayModel(float size, float x, float y) {
        std::vector<glm::vec2> xyList{
            {1, 1},
            {0, 0},
            {1, 0},
            {1, 1},
            {0, 1},
            {0, 0}
        };

        for (auto& xy : xyList) {
            m_positionList.emplace_back(xy.x * size + x, xy.y * size + y, 0.0f);
            m_colorList.emplace_back(1.0f, 1.0f, 1.0f);
            m_normalList.emplace_back(0.0f, 0.0f, 1.0f);
            m_uvList.emplace_back(xy.x, xy.y);
        }
    }
}
