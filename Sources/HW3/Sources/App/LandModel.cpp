#include "App.hpp"

namespace App {
    LandModel::LandModel(glm::vec2 size, float height) {
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
                    (xy.y - 0.5f) * size.x,
                    height,
                    (xy.x - 0.5f) * size.y
            );

            m_normalList.emplace_back(0.0f, 1.0f, 0.0f);
            m_uvList.emplace_back(xy.x, xy.y);
        }
    }
}
