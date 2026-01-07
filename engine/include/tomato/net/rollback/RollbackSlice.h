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
        void ApplyToWorld(World& world)
        {
            for (size_t i = 0; i < entities_.size(); ++i)
            {
                const auto entity = entities_[i];
                std::apply([&](auto&... componentVec)
                           {
                               (world.Set<Components>(entity, componentVec[i]), ...);
                           }, components_);
            }
        }

        void SaveFromWorld(World& world, uint32_t tick)
        {
            for (size_t i = 0; i < entities_.size(); ++i)
            {
                const auto entity = entities_[i];
                std::apply([&](auto&... componentVec)
                           {
                               (SetComponent(world.Get<Components>(entity), componentVec[i]), ...);
                           }, components_);
            }
        }

        void Clear()
        {
            tick_ = 0;
            entities_.clear();
            std::apply([](auto&... componentVec)
                       {
                           (componentVec.clear(), ...);
                       }, components_);
        }

    private:
        template<typename Component>
        void SetComponent(Component& src, Component& dst) { dst = src; }

        uint32_t tick_{0};

        std::vector<World::Entity> entities_;
        std::tuple<std::vector<Components>...> components_;
    };
}

#endif //TOMATO_ROLLBACKSLICE_H
