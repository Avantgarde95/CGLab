#include "Engine.hpp"

namespace Engine {
    // Object for holding information about the material.
    struct Material {
        // Ambient factor.
        glm::vec3 ambient;
        // Diffuse factor.
        glm::vec3 diffuse;
        // Specular factor.
        glm::vec3 specular;

        // Shininess.
        float shininess;
    };
}
