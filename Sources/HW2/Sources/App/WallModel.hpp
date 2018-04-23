#ifndef APP_WALL_MODEL_HPP
#define APP_WALL_MODEL_HPP

#include "App.hpp"

namespace App {
    // Wall model.
    class WallModel : public Engine::Model {
    public:
        WallModel(float size, float x, float y, float z);
    };
}

#endif
