#ifndef APP_LAND_MODEL_HPP
#define APP_LAND_MODEL_HPP

#include "App.hpp"

namespace App {
    class LandModel : public GeneralModel {
    public:
        LandModel(glm::vec2 size, float height);
    };
}

#endif
