#include "App.hpp"

namespace App {
    SkyModel::SkyModel(glm::vec3 size, glm::vec3 center) {
        m_positionList = {
                // Front.
                {0.0f, 1.0f, 1.0f},
                {1.0f, 0.0f, 1.0f},
                {0.0f, 0.0f, 1.0f},
                {0.0f, 1.0f, 1.0f},
                {1.0f, 1.0f, 1.0f},
                {1.0f, 0.0f, 1.0f},

                // Back.
                {0.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f},
                {1.0f, 1.0f, 0.0f},
                {0.0f, 0.0f, 0.0f},
                {1.0f, 0.0f, 0.0f},
                {1.0f, 1.0f, 0.0f},

                // Right.
                {1.0f, 1.0f, 1.0f},
                {1.0f, 1.0f, 0.0f},
                {1.0f, 0.0f, 1.0f},
                {1.0f, 1.0f, 0.0f},
                {1.0f, 0.0f, 0.0f},
                {1.0f, 0.0f, 1.0f},

                // Left.
                {0.0f, 1.0f, 1.0f},
                {0.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f},
                {0.0f, 1.0f, 1.0f},
                {0.0f, 0.0f, 1.0f},
                {0.0f, 0.0f, 0.0f},

                // Top.
                {0.0f, 1.0f, 1.0f},
                {0.0f, 1.0f, 0.0f},
                {1.0f, 1.0f, 0.0f},
                {1.0f, 1.0f, 1.0f},
                {0.0f, 1.0f, 1.0f},
                {1.0f, 1.0f, 0.0f},

                // Bottom.
                {0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f},
                {1.0f, 0.0f, 1.0f},
                {0.0f, 0.0f, 0.0f},
                {1.0f, 0.0f, 1.0f},
                {1.0f, 0.0f, 0.0f}
        };

        m_normalList = {
                // front
                {0.0f,  0.0f,  -1.0f},
                {0.0f,  0.0f,  -1.0f},
                {0.0f,  0.0f,  -1.0f},
                {0.0f,  0.0f,  -1.0f},
                {0.0f,  0.0f,  -1.0f},
                {0.0f,  0.0f,  -1.0f},

                // Back.
                {0.0f,  0.0f,  1.0f},
                {0.0f,  0.0f,  1.0f},
                {0.0f,  0.0f,  1.0f},
                {0.0f,  0.0f,  1.0f},
                {0.0f,  0.0f,  1.0f},
                {0.0f,  0.0f,  1.0f},

                // Right.
                {-1.0f, 0.0f,  0.0f},
                {-1.0f, 0.0f,  0.0f},
                {-1.0f, 0.0f,  0.0f},
                {-1.0f, 0.0f,  0.0f},
                {-1.0f, 0.0f,  0.0f},
                {-1.0f, 0.0f,  0.0f},

                // Left.
                {1.0f,  0.0f,  0.0f},
                {1.0f,  0.0f,  0.0f},
                {1.0f,  0.0f,  0.0f},
                {1.0f,  0.0f,  0.0f},
                {1.0f,  0.0f,  0.0f},
                {1.0f,  0.0f,  0.0f},

                // Top.
                {0.0f,  -1.0f, 0.0f},
                {0.0f,  -1.0f, 0.0f},
                {0.0f,  -1.0f, 0.0f},
                {0.0f,  -1.0f, 0.0f},
                {0.0f,  -1.0f, 0.0f},
                {0.0f,  -1.0f, 0.0f},

                // Bottom.
                {0.0f,  1.0f,  0.0f},
                {0.0f,  1.0f,  0.0f},
                {0.0f,  1.0f,  0.0f},
                {0.0f,  1.0f,  0.0f},
                {0.0f,  1.0f,  0.0f},
                {0.0f,  1.0f,  0.0f}
        };

        m_uvList = {
                // Front.
                {1 / 4.0f, 2 / 3.0f},
                {2 / 4.0f, 1 / 3.0f},
                {1 / 4.0f, 1 / 3.0f},
                {1 / 4.0f, 2 / 3.0f},
                {2 / 4.0f, 2 / 3.0f},
                {2 / 4.0f, 1 / 3.0f},

                // Back.
                {1.0f,     2 / 3.0f},
                {1.0f,     1 / 3.0f},
                {3 / 4.0f, 2 / 3.0f},
                {1.0f,     1 / 3.0f},
                {3 / 4.0f, 1 / 3.0f},
                {3 / 4.0f, 2 / 3.0f},

                // Right.
                {2 / 4.0f, 2 / 3.0f},
                {3 / 4.0f, 2 / 3.0f},
                {2 / 4.0f, 1 / 3.0f},
                {3 / 4.0f, 2 / 3.0f},
                {3 / 4.0f, 1 / 3.0f},
                {2 / 4.0f, 1 / 3.0f},

                // Left.
                {1 / 4.0f, 2 / 3.0f},
                {0,        1 / 3.0f},
                {0,        2 / 3.0f},
                {1 / 4.0f, 2 / 3.0f},
                {1 / 4.0f, 1 / 3.0f},
                {0,        1 / 3.0f},

                // Top.
                {1 / 4.0f, 2 / 3.0f},
                {1 / 4.0f, 1.0f},
                {2 / 4.0f, 1.0f},
                {2 / 4.0f, 2 / 3.0f},
                {1 / 4.0f, 2 / 3.0f},
                {2 / 4.0f, 1.0f},

                // Bottom.
                {1 / 4.0f, 0},
                {1 / 4.0f, 1 / 3.0f},
                {2 / 4.0f, 1 / 3.0f},
                {1 / 4.0f, 0},
                {2 / 4.0f, 1 / 3.0f},
                {2 / 4.0f, 0}
        };

        for (auto &position: m_positionList) {
            position = {
                    (position.x - 0.5f) * size.x + center.x,
                    (position.y - 0.5f) * size.y + center.y,
                    (position.z - 0.5f) * size.z + center.z
            };
        }
    }
}
