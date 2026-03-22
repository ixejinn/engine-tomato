#ifndef TOMATO_FONT_H
#define TOMATO_FONT_H

#include <map>
#include <string>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "tomato/resource/render/TextureAtlas.h"

namespace tomato
{
	struct Glyph
	{
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;

		glm::vec2 uvMin;
		glm::vec2 uvMax;
	};

	/**
	 * @class Font
	 * @brief Manages FreeType face loading and glyph generation.
	 * * Note:
	 * - When rendering text, you must use a specialized Font Shader.
	 * - A Projection Matrix (e.g., Orthographic) is required to calculate
	 * the correct text position on the screen.
 */
	class Font
	{
	public:
		constexpr static const char* PrimitiveName = "assets/fonts/SpoqaHanSansNeo-Medium.ftf";

		static void Create(const char* path = PrimitiveName);

		//void SetFontSize(int size);
		
		const Glyph& GetGlyph(char32_t codepoint);

	private:
		const Glyph& LoadGlyph(char32_t codepoint);

	private:
		Font(const char* path = PrimitiveName);
		~Font();

		FT_Face face;
		TextureAtlas* atlas;
		std::unordered_map<char32_t, Glyph> glyphs;

		uint32_t baseSize{ 48 };
	};

}
#endif //TOMATO_FONT_H