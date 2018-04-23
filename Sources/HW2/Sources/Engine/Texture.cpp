#include "Engine.hpp"

struct PPM {
    int width;
    int height;
    std::string magicNumber;
    std::string maxValue;
    std::vector<char> data;
};

// We assign even units(2, 4, ...) for the textures.
// (Odd units are used for the FBOs.)
static GLint currTextureUnit = 2;

static std::unique_ptr<PPM> readPPM(std::string& path);

namespace Engine {
    Texture::Texture(const std::string& path) : m_texturePath(path) {}

    void Texture::create() {
        // Read the image.
        auto image = readPPM(m_texturePath);

        // Generate a texture unit.
        m_textureUnit = currTextureUnit;
        currTextureUnit += 2;

        // Create & bind the texture.
        glGenTextures(1, &m_textureId);
        glActiveTexture(GL_TEXTURE0 + m_textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            image->width,
            image->height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            &(image->data[0])
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    GLint Texture::getTextureUnit() const {
        return m_textureUnit;
    }
}

static std::unique_ptr<PPM> readPPM(std::string& path) {
    auto image = std::make_unique<PPM>();
    std::ifstream stream(path, std::ios::binary);

    if (!stream.is_open()) {
        std::cout << "Error: File \"" << path << "\" does not exist.\n";
        std::cin.get();
        return image;
    }

    // Read the header.
    stream >> image->magicNumber >> image->width >> image->height >> image->maxValue;
    stream.ignore(256, '\n');

    // Read the data.
    image->data.resize(image->width * image->height * 3);
    stream.read(&(image->data[0]), image->data.size());

    stream.close();
    return image;
}
