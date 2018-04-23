#ifndef APP_DISPLAY_MODEL_HPP
#define APP_DISPLAY_MODEL_HPP

#include "App.hpp"

namespace App {
    // Model for displaying the FBO contents.
    class DisplayModel : public Engine::Model {
    public:
        DisplayModel(float size, float x, float y);
    };
}

#endif
