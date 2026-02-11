#ifndef TOMATO_TEXTURE_H
#define TOMATO_TEXTURE_H

#include <glad/glad.h>

namespace tomato
{
    enum class TextureFormat
    {
        // 일반적인 색상 포맷 (LDR)
        RGB8,           // 투명도 없는 이미지 (JPG 등)
        RGBA8,          // 투명도 있는 표준 이미지 (PNG 등)
        SRGBA8,         // 감마 보정이 적용된 표준 이미지

        // 고정밀 포맷 (HDR)
        RGBA16F,        // 반정밀도 부동소수점 (HDR, 블룸 효과 등)

        // 데이터 및 마스크용
        R8,             // 흑백, 마스크, 글꼴, 그림자 맵

        // 렌더용
        Depth24Stencil8 // 깊이 + 스텐실
    };

    class Texture
    {
    public:
        Texture(const char* filename, TextureFormat format = TextureFormat::RGBA8);
        ~Texture();

        void Bind() const;

    private:
        struct GLFormat
        {
            GLenum internalFormat;  // GPU가 VRAM에 저장하는 방식
            GLenum format;          // 데이터의 채널 구성
            GLenum type;            // 데이터의 자료형
            int channels;           // 채널 개수
        };
        static GLFormat ConvertFormatGL(TextureFormat format);

        GLuint textureId_{0};
        GLFormat format_;
    };
}

#endif //TOMATO_TEXTURE_H