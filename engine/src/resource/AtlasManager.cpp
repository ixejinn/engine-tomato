#include "tomato/resource/AtlasManager.h"
#include "tomato/Logger.h"

namespace tomato
{
	AllocationResult AtlasManager::RequestAllocate(int w, int h)
	{
		// 1. If no atlas exists, create the first one
		if (atlases.empty())
			CreateNewAtlas();

		// 2. Check ONLY the last atlas in the list
		int x, y;
		if (atlases.back()->Allocate(w, h, x, y))
			return { static_cast<int>(atlases.size() - 1), x, y };

		// 3. If the last one is full, create a new one and allocate there
		CreateNewAtlas();
		atlases.back()->Allocate(w, h, x, y);
		
		return { static_cast<int>(atlases.size() - 1, x, y) };
	}
	void AtlasManager::BindAtlas(int index, unsigned int unit)
	{
		if (index >= 0 && index < static_cast<int>(atlases.size()))
			atlases[index]->Bind(unit);
		else
			TMT_ERR << "Invalid Atlas Index: " << index;
	}

	void AtlasManager::CreateNewAtlas()
	{
		atlases.push_back(std::make_unique<TextureAtlas>());
		TMT_DEBUG << "New TextureAtlas created. Current Total: " << atlases.size();
	}
}