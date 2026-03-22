#include "tomato/resource/render/Font.h"
#include "tomato/FontContext.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/Logger.h"
#include <fstream>

namespace tomato
{
	void Font::Create(const char* path)
	{
		std::unique_ptr<Font> ptr{ new Font(path) };

		const char* assetKey = (path == PrimitiveName) ? "DefaultFont" : path;

		AssetRegistry<Font>::GetInstance().Register(assetKey, std::move(ptr));
		TMT_INFO << "Font Registered with key" << assetKey;
	}

	const Glyph& Font::GetGlyph(char32_t codepoint)
	{
		auto it = glyphs.find(codepoint);

		//If found, return the cached glyph
		if (it != glyphs.end())
			return it->second;
		
		//If not found, load, cache, and return the reference
		return LoadGlyph(codepoint);
	}

	const Glyph& Font::LoadGlyph(char32_t codepoint)
	{
		if (FT_Load_Char(face, codepoint, FT_LOAD_RENDER))
		{
			TMT_ERR << "ERROR::FreeType: Failed to load Glyph for: " << (uint32_t)codepoint;
			throw std::runtime_error("Failed to load Glyph");
		}

		// Allocate space in the atlas and upload glyph bitmap (To be implemented)
		int x, y;
		if (!atlas->Allocate(face->glyph->bitmap.width, face->glyph->bitmap.rows, x, y))
			TMT_WARN << "Atlas if full.";

		atlas->Upload(x, y, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);


		// Calcuate UV coordinates (Normalized 0.0 to 1.0)
		// Divide pixel coordinates by atlas size to get normalized values.
		float uvX = (float)x / atlas->GetWidth();
		float uvY = (float)y / atlas->GetHeight();
		float uvW = (float)face->glyph->bitmap.width / atlas->GetWidth();
		float uvH = (float)face->glyph->bitmap.rows / atlas->GetHeight();

		Glyph glyph = {
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x >> 6), // Divide by 64 to convert 26.6 fixed-point to pixels (Shift right by 6)

			glm::vec2(uvX, uvY), // UV Start point
			glm::vec2(uvX + uvW, uvY + uvH) // UV End point
		};

		return (glyphs[codepoint] = glyph);
	}

	Font::Font(const char* path)
	{
		FT_Library ft = FontContext::GetInstance().Get();
		if (FT_New_Face(ft, path, 0, &face))
			TMT_ERR << "ERROR::FreeType: Failed to load font " << path;

		else
			FT_Set_Pixel_Sizes(face, 0, baseSize);
	}
	Font::~Font()
	{
		FT_Done_Face(face);
	}
}