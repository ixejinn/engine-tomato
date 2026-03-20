#ifndef TOMATO_FONT_H
#define TOMATO_FONT_H

#include <map>
#include <string>

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace tomato
{
	class TextureAtlas;

	struct Glyph
	{
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;

		glm::vec2 uvMin;
		glm::vec2 uvMax;
	};

	class Font
	{
	public:
		Font();

		void SetFontSize(int size);
		
		const Glyph& GetGlyph(char32_t codepoint);

	private:
		const Glyph& GetGlyph(char32_t codepoint);

	private:
		FT_Face face;
		TextureAtlas* atlas;
		std::unordered_map<char32_t, Glyph> glyphs;
	};

}
#endif //TOMATO_FONT_H