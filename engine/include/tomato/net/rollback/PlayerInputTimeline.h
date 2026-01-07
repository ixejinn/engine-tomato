#ifndef TOMATO_PLAYERINPUTTIMELINE_H
#define TOMATO_PLAYERINPUTTIMELINE_H

#include "tomato/services/InputTypes.h"
#include "tomato/containers/RingArray.h"
#include "tomato/net/rollback/CoreRollback.h"

namespace tomato
{
    class PlayerInputTimeline
    {
    public:
        const InputRecord& operator[](uint32_t tick) { return timeline_[tick]; }
        const InputRecord& operator[](uint32_t tick) const { return timeline_[tick]; }

        void SetInputTimeline(uint32_t tick, const InputRecord& record) { timeline_[tick] = record; }

    private:
        RingArray<InputRecord, ROLLBACK_WINDOW> timeline_;
    };
}

#endif //TOMATO_PLAYERINPUTTIMELINE_H
