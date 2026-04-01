#ifndef TOMATO_TEXT_RENDERER_H
#define TOMATO_TEXT_RENDERER_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <vector>

#include "tomato/resource/render/Font.h"

namespace tomato
{
	struct TextVertex
	{
		glm::vec2 position;
		glm::vec2 uv;
		glm::vec4 color;
	};

	class Shader;
	class TextRenderer
	{
	public:
		~TextRenderer();
		
		void Init(Shader* shader);

		// Add text data to the buffer
		void DrawString(const std::u32string& text, float x, float y, float size, const glm::vec4& color, Font* font);

		// Actually send data to GPU and Draw
		void Flush();

		int GetVertexCount() { return vertices_.size(); }
	private:
		// Helper to add a qua (2 triangles, 6 vertices)
		void AddQuad(float x, float y, float size, const Glyph& glyph, const glm::vec4& color);
		std::u32string ToUTF32(const std::string& uft8Str);

	private:
		Shader* shader_{ nullptr };

		GLuint vao_{ 0 }, vbo_{ 0 };
		int currentAtlasIndex_{ -1 };

		std::vector<TextVertex> vertices_;
	};
}

#endif // !TOMATO_TEXT_RENDERER_H