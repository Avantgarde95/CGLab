#ifndef ENGINE_LIGHT_HPP
#define ENGINE_LIGHT_HPP

#include "Engine.hpp"

namespace Engine {
    // Object which represents a single light.
    struct Light {
        // Light type. (-1: Turn off, 0: Directional light, 1: Point light, 2: Spotlight)
        GLint type = -1;

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
