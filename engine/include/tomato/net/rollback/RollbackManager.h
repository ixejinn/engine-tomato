#ifndef TOMATO_ROLLBACKMANAGER_H
#define TOMATO_ROLLBACKMANAGER_H

#include <cstdint>
#include <tuple>
#include "tomato/containers/Timeline.h"
#include "tomato/Logger.h"

namespace tomato
{
    class World;

    /**
     * @brief Type-erased interface for RollbackManager.
     *
     * 엔진에서 RollbackManager를 포인터로 보관/호출하기 위해 만든 인터페이스 클래스.
     * RollbackManager은 템플릿 클래스이므로 엔진에서 템플릿 인스턴스를 알지 않아도 다룰 수 있도록 한다.
     */
    class RollbackManagerB
    {
    public:
        virtual ~RollbackManagerB() = default;

        /// Applies the stored RollbackSlices of given tick to the World.
        virtual void Rollback(World& world, uint32_t tick) = 0;

        /// Captures RollbackSlices from the world for the given tick.
        virtual void Capture(World& world, uint32_t tick) = 0;
    };

    /**
     * @brief Rollback manager componsed of multiple RollbackSlice timelines.
     * @tparam RollbackSliceT Types of RollbackSlices.
     *
     * 여러 RollbackSlice의 타임라인을 보관하고, 특정 tick에 대해 월드를 캡처하거나 RollbackSlice로부터 복원한다.
     */
    template<typename... RollbackSliceT>
    class RollbackManager final : public RollbackManagerB
    {
    public:
        void Rollback(World& world, uint32_t tick) override
        {
            const auto storedTick = std::get<0>(timelines_)[tick].GetTick();
            if (storedTick != tick)
            {
                TMT_WARN << "Rollback tick mismatch (requested: " << tick << ", stored: " << storedTick << ")";
                return;
            }

            std::apply([&](auto&... timeline)
                       {
                           (timeline[tick].ApplyToWorld(world), ...);
                       }, timelines_);
        }

        void Capture(World& world, uint32_t tick) override
        {
            std::apply([&](auto&... timeline)
                       {
                           (timeline[tick].SaveFromWorld(world, tick), ...);
                       }, timelines_);
        }

    private:
        std::tuple<Timeline<RollbackSliceT>...> timelines_;
    };
}

#endif //TOMATO_ROLLBACKMANAGER_H
