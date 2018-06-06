#ifndef APP_SKY_MODEL_HPP
#define APP_SKY_MODEL_HPP

#include "App.hpp"

namespace App {
    class SkyModel : public GeneralModel {
    public:
        explicit SkyModel(glm::vec3 size, glm::vec3 center);
    };
}

#endif
