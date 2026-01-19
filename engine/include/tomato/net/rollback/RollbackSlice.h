#ifndef TOMATO_ROLLBACKSLICE_H
#define TOMATO_ROLLBACKSLICE_H

#include <cstdint>
#include <vector>
#include <tuple>
#include "tomato/ecs/World.h"

namespace tomato
{
    /**
     * @brief Rollback slice responsible for capturing and restoring component state.
     * @tparam Components Component type to capture and restore.
     *
     * 특정 컴포넌트 조합(Components...)을 가진 엔티티들의 컴포넌트 값을 저장하고,
     * 저장된 값으로 World의 컴포넌트를 복원(rollback)한다.
     *
     * @note 현재 구현은 "엔티티가 도중에 삭제되지 않는다"를 가정으로 한다.
     */
    template<typename... Components>
    class RollbackSlice
    {
    public:
        /// Applies the stored snapshot values to the World.
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

        /// Captures component values from the World for the given tick.
        void SaveFromWorld(World& world, uint32_t tick)
        {
            currTick_ = tick;

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

        /// Returns the tick associated with the stored snapshot.
        uint32_t GetTick() const { return currTick_; }

    private:
        template<typename Component>
        void SetComponent(Component& src, Component& dst) { dst = src; }

        uint32_t currTick_{0};

        std::vector<Entity> entities_;
        std::tuple<std::vector<Components>...> components_;
    };
}

#endif //TOMATO_ROLLBACKSLICE_H
