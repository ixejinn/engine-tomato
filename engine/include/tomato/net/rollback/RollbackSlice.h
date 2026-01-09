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
            auto view = world.GetRegistry().view<Components...>();

            for (int i = 0; i < entities_.size(); i++)
            {
                std::apply([&](auto&... componentVec)
                           {
                                // 일단 엔티티가 중간에 삭제되지 않는다는 가정
                               (SetComponent(componentVec[i], view.template get<Components>(entities_[i])), ...);
                           }, components_);
            }
        }

        void SaveFromWorld(World& world, uint32_t tick)
        {
            tick_ = tick;

            auto view = world.GetRegistry().view<Components...>();

            int entityCnt = view.size_hint();
            entities_.resize(entityCnt);
            std::apply([&](auto&... componentVec)
                       {
                           (componentVec.resize(entityCnt), ...);
                       }, components_);

            int entityIdx = 0;
            for (auto entity : view)
            {
                entities_[entityIdx] = entity;

                std::apply([&](auto&... componentVec)
                           {
                               (SetComponent(view.template get<Components>(entity), componentVec[entityIdx]), ...);
                           }, components_);

                ++entityIdx;
            }

            if (entityCnt != entityIdx)
                entities_.resize(entityIdx);
        }

    private:
        template<typename Component>
        void SetComponent(Component& src, Component& dst) { dst = src; }

        uint32_t tick_{0};

        std::vector<Entity> entities_;
        std::tuple<std::vector<Components>...> components_;
    };
}

#endif //TOMATO_ROLLBACKSLICE_H
