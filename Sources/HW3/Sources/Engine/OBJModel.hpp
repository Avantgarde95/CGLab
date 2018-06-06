#ifndef ENGINE_OBJ_MODEL_HPP
#define ENGINE_OBJ_MODEL_HPP

#include "Engine.hpp"

namespace Engine {
    // Mixin for constructing the model from an .obj file.
    template<typename T>
    class OBJModel : public T {
    public:
        explicit OBJModel(const std::string &path) {
            std::string basePath = path + "/../";
            std::string error;
            tinyobj::attrib_t attribute;
            std::vector<tinyobj::shape_t> shapeList;
            std::vector<tinyobj::material_t> materialList;

            tinyobj::LoadObj(
                    &attribute,
                    &shapeList,
                    &materialList,
                    &error,
                    path.c_str(),
                    basePath.c_str(),
                    true
            );

            if (!error.empty()) {
                std::cout << error << "\n";
            }

            bool hasNormal = !attribute.normals.empty();
            bool hasUV = !attribute.texcoords.empty();

            for (auto &shape : shapeList) {
                size_t indexOffset = 0;

                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
                    for (auto v = 0; v < 3; v++) {
                        auto index = shape.mesh.indices[indexOffset + v];

                        this->m_positionList.emplace_back(
                                attribute.vertices[index.vertex_index * 3 + 0],
                                attribute.vertices[index.vertex_index * 3 + 1],
                                attribute.vertices[index.vertex_index * 3 + 2]
                        );

                        if (hasNormal) {
                            this->m_normalList.emplace_back(
                                    attribute.normals[index.normal_index * 3 + 0],
                                    attribute.normals[index.normal_index * 3 + 1],
                                    attribute.normals[index.normal_index * 3 + 2]
                            );
                        }

                        if (hasUV) {
                            this->m_uvList.emplace_back(
                                    attribute.texcoords[index.texcoord_index * 2 + 0],
                                    attribute.texcoords[index.texcoord_index * 2 + 1]
                            );
                        }
                    }

                    indexOffset += 3;
                }
            }

            if (!hasNormal) {
                this->generateNormalList();
            }

            if (!hasUV) {
                this->generateUVList();
            }
        }
    };
}

#endif
