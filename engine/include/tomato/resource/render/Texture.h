#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include <string>

#include "tomato/Logger.h"

enum class ImageFormat
{
    R8, RG8, RGBA8,
    R16F, RG16F, RGBA16F,
    R32F, RG32F, RGBA32F,
    R8UI, RG8UI, RGBA8UI,
    R32UI, RG32UI, RGBA32UI
};

class Texture
{
public:
    Texture(const std::string& path, ImageFormat format = ImageFormat::RGBA8);
    Texture(const char* path, ImageFormat format = ImageFormat::RGBA8);
    ~Texture();

    void BindTexture() const;
    void Bind(unsigned int slot = 0) const;
    void UnBind(unsigned int slot) const;

    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }
    unsigned int GetID() const { return id_; }

 
private:
    unsigned int id_;
    int width_, height_, channels_;
    ImageFormat format_;
};

#endif