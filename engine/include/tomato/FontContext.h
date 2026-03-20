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

	FT_Library GetLibrary() const noexcept { return library_; }

private:
	FT_Library library_;
};

#endif // !TOMATO_FONTCONTEXT_H
