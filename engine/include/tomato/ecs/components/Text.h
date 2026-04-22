#ifndef TOMATO_TEXT_H
#define TOMATO_TEXT_H

#include "tomato/resource/tomato_resource.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/resource/render/Font.h"
#include <string>

namespace tomato
{
	struct TextComponent
	{
		/**
		 * @brief The text to be rendered.
		 * @note Use UTF-32 literals with 'U' prefix, e.g., U"Your Text Here".
		 * @todo Planned update:
		 * Modify the write interface to accept standard std::string (UTF-8)
		 * and handle internal conversion to UTF-32 for better usability.
		 */
		std::string text{};
		glm::vec4 color{ 0.0f, 0.0f, 0.0f, 1.0f };
		float fontSize{ 0.6f };
		AssetId font{ GetAssetID(Font::PrimitiveName) };
	};
}

#endif // !TOMATO_TEXT_H
