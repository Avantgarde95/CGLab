#include "App.hpp"

namespace App {
    WallModel::WallModel(float size, float x, float y, float z) {
        std::vector<glm::vec2> xyList = {
            { 1.0f, 1.0f },
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 1.0f, 1.0f },
            { 0.0f, 1.0f },
            { 0.0f, 0.0f }
        };

        std::vector<glm::ivec3> axisList = {
            {0, 1, 2},
            {2, 0, 1},
            {1, 2, 0}
        };

        for (auto& axis : axisList) {
            for (auto& xy : xyList) {
                glm::vec3 position{ x, y, z };
                glm::vec3 normal{ 0.0f, 0.0f, 0.0f };

                position[axis.x] += xy.x * size;
                position[axis.y] += xy.y * size;

                normal[axis.z] = 1.0f;

                m_positionList.emplace_back(position);
                m_colorList.emplace_back(1.0f, 1.0f, 1.0f);
                m_normalList.emplace_back(normal);
                m_uvList.emplace_back(xy.x, xy.y);
            }
        }
    }
}
