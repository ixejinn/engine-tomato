#ifndef TOMATO_ATLAS_MANAGER_H
#define TOMATO_ATLAS_MANAGER_H

#include <memory>
#include <vector>
#include "tomato/resource/render/TextureAtlas.h"

namespace tomato
{
	struct AllocationResult
	{
		int atlasIndex;
		int x, y;
	};

	class AtlasManager
	{
	public:
		static AtlasManager& GetInstance()
		{
			static AtlasManager instance;
			return instance;
		}

		AllocationResult RequestAllocate(int w, int h);
		void BindAtlas(int index, unsigned int unit = 0);
		
		TextureAtlas* GetAtlas(int index) { return atlases[index].get(); }

	private:
		AtlasManager() = default;
		// Add a new 2048x2048 atlas to the pool
		void CreateNewAtlas();

		std::vector<std::unique_ptr<TextureAtlas>> atlases;
	};
}

#endif // !TOMATO_ATLAS_MANAGER_H
