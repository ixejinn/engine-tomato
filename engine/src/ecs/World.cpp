#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Render.h"

namespace tomato
{
    World::World()
    {
        Init();
    }

    void World::Init()
    {
        registry_.group<PositionComponent, RenderComponent>();
    }
}