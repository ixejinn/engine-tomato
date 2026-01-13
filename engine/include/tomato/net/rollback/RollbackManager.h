#ifndef TOMATO_ROLLBACKMANAGER_H
#define TOMATO_ROLLBACKMANAGER_H

#include <cstdint>
#include <tuple>
#include "tomato/containers/RingArray.h"
#include "tomato/net/rollback/CoreRollback.h"
#include "tomato/Logger.h"

namespace tomato
{
    class World;

    class RollbackManagerB
    {
    public:
        virtual ~RollbackManagerB() = default;

        virtual void Rollback(World& world, uint32_t tick) = 0;
        virtual void Capture(World& world, uint32_t tick) = 0;
    };

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
        std::tuple<RingArray<RollbackSliceT, ROLLBACK_WINDOW>...> timelines_;
    };
}

#endif //TOMATO_ROLLBACKMANAGER_H
