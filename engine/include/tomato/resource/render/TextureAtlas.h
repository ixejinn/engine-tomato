#ifndef TOMATO_TEXTURE_ATLAS_H
#define TOMATO_TEXTURE_ATLAS_H

namespace tomato
{
	class TextureAtlas
	{
	public:
		TextureAtlas(int w, int h);
		~TextureAtlas();

		bool Allocate(int w, int h, int& outX, int& outY); //packing
		void Upload(int x, int y, int w, int h, unsigned char* data);
		void Bind(unsigned int unit = 0) const;
		void Clear(); 

		unsigned int GetID() const { return textureID_; }

		int GetWidth() const { return width_; }
		int GetHeight() const { return height_; }

	private:
		unsigned int textureID_;
		int width_, height_;

		// For Shelf Packing
		int currentX_{ 0 };
		int currentY_{ 0 };
		int maxRowHeight_{ 0 };
	};
}

#endif // !TOMATO_TEXTURE_ATLAS_H
