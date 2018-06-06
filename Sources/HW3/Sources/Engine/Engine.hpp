#ifndef ENGINE_ENGINE_HPP
#define ENGINE_ENGINE_HPP

// Standard.
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <initializer_list>

// GLEW.
#include <GL/glew.h>

// GLFW.
#include <glfw3.h>

// GLM.
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/polar_coordinates.hpp>

// tinyobjloader
#include <tiny_obj_loader.h>

// SOIL.
#include <SOIL/SOIL.h>

// Engine.
#include "Renderer.hpp"

#include "Texture.hpp"
#include "FrameBuffer.hpp"

#include "Shader.hpp"
#include "Program.hpp"

#include "Light.hpp"

#include "Model.hpp"
#include "TextureModel.hpp"
#include "ShadowModel.hpp"
#include "LightModel.hpp"
#include "OBJModel.hpp"

#endif
