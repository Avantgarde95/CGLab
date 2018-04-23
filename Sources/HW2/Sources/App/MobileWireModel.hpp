#ifndef MOBILE_WIRE_MODEL_HPP
#define MOBILE_WIRE_MODEL_HPP

#include "App.hpp"

namespace App {
    // Model for connecting the nodes.
    class MobileWireModel : public Engine::Model {
    public:
        MobileWireModel();

        void setPositions(const glm::vec3& startPosition, const glm::vec3& endPosition);
    };
}

#endif
