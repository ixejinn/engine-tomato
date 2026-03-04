#include "tomato/resource/render/Texture.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace tomato
{
    void Texture::Create()
    {
        std::unique_ptr<Texture> ptr{new Texture};
        AssetRegistry<Texture>::GetInstance().Register(PrimitiveName, std::move(ptr));
    }

    void Texture::Create(const char* filename, Format format)
    {
        std::unique_ptr<Texture> ptr{new Texture(filename, format)};
        AssetRegistry<Texture>::GetInstance().Register(filename, std::move(ptr));
    }

    Texture::GLFormat Texture::ConvertFormatGL(tomato::Texture::Format format)
    {
        switch (format)
        {
            case Format::RGB8:
                return {GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, STBI_rgb};
            case Format::RGBA8:
            default:
                return {GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, STBI_rgb_alpha};
            case Format::SRGBA8:
                return {GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE, STBI_rgb_alpha};
            case Format::RGBA16F:
                return {GL_RGBA16F, GL_RGBA, GL_FLOAT, STBI_rgb_alpha};
            case Format::R8:
                return {GL_R8, GL_RED, GL_UNSIGNED_BYTE, 1};
            case Format::Depth24Stencil8:
                return {GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0};
        }
    }

    Texture::Texture() : format_(ConvertFormatGL(Format::RGBA8))
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &textureId_);

        GLubyte white[4] = {255, 255, 255, 255};
        glTextureStorage2D(textureId_, 1, format_.internalFormat, 1, 1);
        glTextureSubImage2D(textureId_, 0, 0, 0, 1, 1, format_.format, format_.type, white);
    }

    Texture::Texture(const char* filename, Format format) : format_(ConvertFormatGL(format))
    {
        stbi_set_flip_vertically_on_load(true);

        glCreateTextures(GL_TEXTURE_2D, 1, &textureId_);

        // Set wrapping options
        glTextureParameteri(textureId_, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureId_, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set filtering options
        glTextureParameteri(textureId_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(textureId_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, actualCh;
        if (unsigned char* image = stbi_load(filename, &width, &height, &actualCh, format_.channels))
        {
            if (format_.channels != 0 && actualCh > format_.channels)
                TMT_WARN << "Data loss occurs: " << filename;

            glTextureStorage2D(textureId_, 1, format_.internalFormat, width, height);
            glTextureSubImage2D(textureId_, 0, 0, 0, width, height, format_.format, format_.type, image);

            glGenerateTextureMipmap(textureId_);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            stbi_image_free(image);
        }
        else
            TMT_ERR << "Failed to load texture: " << filename;
    }

    Texture::~Texture()
    {
        if (textureId_ != 0)
            glDeleteTextures(1, &textureId_);
    }

    void Texture::Bind() const
    {
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, textureId_);
        glBindTextureUnit(0, textureId_);
    }
}