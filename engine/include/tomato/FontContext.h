#ifndef TOMATO_FONTCONTEXT_H
#define TOMATO_FONTCONTEXT_H

#include "tomato/Logger.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class FontContext
{
public:
	FontContext()
	{
		if (FT_Init_FreeType(&library_))
		{
			TMT_ERR << "ERROR::FREETYPE: Could not init FreeType Library";
		}
	}

	~FontContext()
	{
		FT_Done_FreeType(library_);
	}

	FT_Library Get() const noexcept { return library_; }
	static FontContext& GetInstance()
	{
		static FontContext instance;
		return instance;
	}

private:
	FT_Library library_ = nullptr;
};

#endif // !TOMATO_FONTCONTEXT_H
