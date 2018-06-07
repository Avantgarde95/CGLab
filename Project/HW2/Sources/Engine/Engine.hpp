#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP

// All headers used in 'Engine' module are included in here.

// -- Standard headers.
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// -- GLEW
#include <GL/glew.h>

// -- GLFW
#include <glfw3.h>

// -- GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

// -- Engine
#include "Window.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "FBO.hpp"
#include "Model.hpp"

#endif
