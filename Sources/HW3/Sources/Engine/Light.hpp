#ifndef ENGINE_LIGHT_HPP
#define ENGINE_LIGHT_HPP

#include "Engine.hpp"

namespace Engine {
    // Object which represents a single light.
    struct Light {
        enum Type {
            OFF = 0,
            DIRECTIONAL = 1,
            POINT = 2,
            SPOT = 3
        };

        // Light type.
        Type type;

        // Position of the light source.
        glm::vec3 position;
        // Direction the light. (i.e Direction of the cone.)
        glm::vec3 direction;

        // Ambient factor.
        glm::vec3 ambient;
        // Diffuse factor.
        glm::vec3 diffuse;
        // Specular factor.
        glm::vec3 specular;

        // Cone angle.
        GLfloat angle;
        // Attenuation factor.
        GLfloat attenuation;
    };
}

#endif
