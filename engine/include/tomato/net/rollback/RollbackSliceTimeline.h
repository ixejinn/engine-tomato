#ifndef TOMATO_ROLLBACKSLICETIMELINE_H
#define TOMATO_ROLLBACKSLICETIMELINE_H

#include "tomato/containers/RingArray.h"
#include "tomato/net/rollback/CoreRollback.h"
#include "tomato/ecs/World.h"

namespace tomato
{
    template<typename... RollbackSlices>
    class RollbackSliceTimeline
    {
    public:
        void Rollback(World& world, uint32_t tick)
        {
            std::apply([&](auto&... timelines)
                       {
                           (timelines[tick].ApplyToWorld(world), ...);
                       }, slices);
        }

        void Capture(World& world, uint32_t tick)
        {
            std::apply([&](auto&... timelines)
                       {
                           (timelines[tick].SaveFromWorld(world), ...);
                       }, slices);
        }

    private:
        std::tuple<RingArray<RollbackSlices, ROLLBACK_WINDOW>...> slices;
    };
}

#endif //TOMATO_ROLLBACKSLICETIMELINE_H
