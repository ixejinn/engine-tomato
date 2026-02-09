#include "tomato/resource/render/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <iostream>

inline GLenum ConvertFormat(ImageFormat fmt)
{
    switch (fmt)
    {
    case ImageFormat::R8:       return GL_R8;
    case ImageFormat::RG8:      return GL_RG8;
    case ImageFormat::RGBA8:    return GL_RGBA8;

    case ImageFormat::R16F:     return GL_R16F;
    case ImageFormat::RG16F:    return GL_RG16F;
    case ImageFormat::RGBA16F:  return GL_RGBA16F;

    case ImageFormat::R32F:     return GL_R32F;
    case ImageFormat::RG32F:    return GL_RG32F;
    case ImageFormat::RGBA32F:  return GL_RGBA32F;

    case ImageFormat::R8UI:     return GL_R8UI;
    case ImageFormat::RG8UI:    return GL_RG8UI;
    case ImageFormat::RGBA8UI:  return GL_RGBA8UI;

    case ImageFormat::R32UI:    return GL_R32UI;
    case ImageFormat::RG32UI:   return GL_RG32UI;
    case ImageFormat::RGBA32UI: return GL_RGBA32UI;
    }
    return GL_RGBA8;
}

Texture::Texture(const std::string& path, ImageFormat format) : id_(0), width_(0), height_(0), channels_(0), format_(format)
{
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* data = stbi_load(path.c_str(), &width_, &height_, &channels_, 0);
    if (data)
    {
        TMT_LOG << "Success to load texture: " << path;// << '\n';
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
        TMT_ERR << "Failed to load texture: " << path;// << '\n';

    stbi_image_free(data);
}

Texture::Texture(const char* path, ImageFormat format) : Texture(std::string(path), format) {};

Texture::~Texture()
{
    if (id_ != 0)
        glDeleteTextures(1, &id_);
}

void Texture::BindTexture() const
{
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::Bind(unsigned int slot) const
{
    //glBindTextureUnit(slot, id_);
}

void Texture::UnBind(unsigned int slot) const
{
    //glBindTextureUnit(slot, 0);
}