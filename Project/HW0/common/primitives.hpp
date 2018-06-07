#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

void compute_normal_and_face(std::vector<glm::vec3>& point_vec, std::vector<glm::vec3>& normal_vec, std::vector<glm::vec3>& element_vec, int a, int b, int c);
// Initialize point of sphere primitives
void generate_sphere_coord(std::vector<glm::vec3 >&, std::vector<glm::vec3>&, std::vector<unsigned int>&, int);

#endif
