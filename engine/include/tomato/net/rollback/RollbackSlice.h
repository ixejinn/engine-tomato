#ifndef TOMATO_ROLLBACKSLICE_H
#define TOMATO_ROLLBACKSLICE_H

#include <cstdint>
#include <vector>
#include <tuple>
#include "tomato/ecs/World.h"

namespace tomato
{
    // 롤백할 컴포넌트 조합의 컴포넌트 값들을 저장하고,
    // World에서 해당 컴포넌트 조합을 가지고 있는 엔티티의 컴포넌트 값을 저장된 값으로 롤백
    template<typename... Components>
    class RollbackSlice
    {
    public:
        void SetRollbackSlice(World& world)
        {
            for (size_t i = 0; i < entities.size(); ++i)
            {
                const auto entity = entities[i];
                std::apply([&](auto&... componentVec)
                {
                    (world.Set<Components>(entity, componentVec[i]), ...);
                }, data);
            }
        }

        void Clear()
        {
            tick = 0;
            entities.clear();
            std::apply([](auto&... componentVec)
            {
                (componentVec.clear(), ...);
            }, data);
        }

    private:
        uint32_t tick{0};

        std::vector<World::Entity> entities;
        std::tuple<std::vector<Components>...> data;
    };
}

#endif //TOMATO_ROLLBACKSLICE_H
