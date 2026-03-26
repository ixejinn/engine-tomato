#include "tomato/resource/TextRenderer.h"
#include "tomato/resource/AtlasManager.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/resource/render/Shader.h"
#include "tomato/Logger.h"

namespace tomato
{
	TextRenderer::~TextRenderer()
	{
		if (vao_ != 0) glDeleteVertexArrays(1, &vao_);
		if (vbo_ != 0) glDeleteBuffers(1, &vbo_);
	}

	void TextRenderer::Init(Shader* shader)
	{
		shader_ = shader;

		// 1. Create and bind VAO
		glGenVertexArrays(1, &vao_);
		glGenBuffers(1, &vbo_);
		glBindVertexArray(vao_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);

		// 2. Pre-allocate VBO memory (e.g., space for 1000 characters)
		// 1 char = 6 vertices (2 triangles)
		size_t reservedSize = sizeof(TextVertex) * 6 * 1000;
		glBufferData(GL_ARRAY_BUFFER, reservedSize, nullptr, GL_DYNAMIC_DRAW);

		// 3. Set Vertex Attributes
		// Position (vec2)
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)offsetof(TextVertex, position));
	
		// UV (vec2)
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)offsetof(TextVertex, uv));

		// Color (vec4)
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(TextVertex), (void*)offsetof(TextVertex, color));

		glBindVertexArray(0);
		
		TMT_DEBUG << "TextRenderer initialized with reserved capacity: 1000 chars.";
	}

	void TextRenderer::DrawText(const std::string& text, float x, float y, float size, glm::vec4& color, Font* font)
	{
		/*shader_->Use();
		shader_->SetUniformVec3("textColor", color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(vao_);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);*/
		
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			const Glyph& glyph = font->GetGlyph(*c);

			if (currentAtlasIndex_ != -1 && currentAtlasIndex_ != glyph.atlasIndex)
				Flush();
			currentAtlasIndex_ = glyph.atlasIndex;

			AddQuad(x, y, size, glyph, color);

			x += (glyph.advance >> 6) * size;
		}
	}

	void TextRenderer::Flush()
	{
		if (vertices_.empty()) return;

		shader_->Use();
		//shader_->SetUniformVec3("textColor", color.x, color.y, color.z);
		AtlasManager::GetInstance().BindAtlas(currentAtlasIndex_);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_.size() * sizeof(TextVertex), vertices_.data());

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_.size()));

		vertices_.clear();

	}

	void TextRenderer::AddQuad(float x, float y, float size, const Glyph& glyph, const glm::vec4& color)
	{
		float xpos = x + glyph.bearing.x * size;
		float ypos = y - (glyph.size.y - glyph.bearing.y) * size;
		float w = glyph.size.x * size;
		float h = glyph.size.y * size;

		vertices_.emplace_back(TextVertex{ {xpos, ypos + h},		{0.0f, 0.0f}, color });
		vertices_.emplace_back(TextVertex{ {xpos, ypos},			{0.0f, 1.0f}, color });
		vertices_.emplace_back(TextVertex{ {xpos + w, ypos},		{1.0f, 1.0f}, color });

		vertices_.emplace_back(TextVertex{ {xpos, ypos + h},		{0.0f, 0.0f}, color });
		vertices_.emplace_back(TextVertex{ {xpos + w, ypos},		{1.0f, 1.0f}, color });
		vertices_.emplace_back(TextVertex{ {xpos + w, ypos + h},	{1.0f, 0.0f}, color });
	}



}