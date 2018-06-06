#include "Engine.hpp"

static GLint generateUnit();

namespace Engine {
    Texture::Texture(const std::string &path) {
        GLsizei width;
        GLsizei height;
        unsigned char *data = SOIL_load_image(path.c_str(), &width, &height, nullptr, SOIL_LOAD_RGB);

        if (data == nullptr) {
            std::stringstream messageStream;

            messageStream << "Error: " << SOIL_last_result() << ".";

            throw std::runtime_error(messageStream.str());
        }

        create(width, height, {data}, GL_RGB, GL_RGB, false);
        SOIL_free_image_data(data);
    }

    Texture::Texture(
            GLsizei width,
            GLsizei height,
            const std::vector<GLvoid *> &dataList,
            GLint internalFormat,
            GLenum format,
            bool isCubeMap
    ) {
        std::vector<GLvoid *> newDataList(6, nullptr);

        for (int i = 0; i < dataList.size(); i++) {
            if (i >= 6) {
                break;
            }

            newDataList[i] = dataList[i];
        }

        create(width, height, newDataList, internalFormat, format, isCubeMap);
    }

    GLuint Texture::getId() const {
        return m_id;
    }

    GLint Texture::getUnit() const {
        return m_unit;
    }

    void Texture::create(
            GLsizei width,
            GLsizei height,
            const std::vector<GLvoid *> &dataList,
            GLint internalFormat,
            GLenum format,
            bool isCubeMap
    ) {
        m_unit = generateUnit();

        glGenTextures(1, &m_id);
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + m_unit));
        glBindTexture(isCubeMap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, m_id);

        if (isCubeMap) {
            for (int face = 0; face < 6; face++) {
                glTexImage2D(
                        static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face),
                        0,
                        internalFormat,
                        width,
                        height,
                        0,
                        format,
                        GL_UNSIGNED_BYTE,
                        dataList[face]
                );
            }
        }
        else {
            glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    internalFormat,
                    width,
                    height,
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    dataList[0]
            );
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

static GLint generateUnit() {
    static GLint currUnit = 0;

    currUnit++;

    return currUnit;
}
