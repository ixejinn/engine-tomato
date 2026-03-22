#include "tomato/resource/render/TextureAtlas.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace tomato
{
	TextureAtlas::TextureAtlas(int w, int h) : width_(w), height_(h)
	{
		glGenTextures(1, &textureID_);
		glBindTexture(GL_TEXTURE_2D, textureID_);

		// Allocate first time
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}


	/**
	 * [Usage Scenario]
	 * 1. Bind your font shader: shader.Use();
	 * 2. Bind this atlas: atlas.Bind(0);
	 * 3. Set shader uniform: shader.SetInt("textTexture", 0);
	 * 4. Perform draw calls: glDrawElements(...);
	 *
	 * @param unit The texture unit to bind to (default is 0).
	 */
	void TextureAtlas::Bind(unsigned int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, textureID_);
	}

}