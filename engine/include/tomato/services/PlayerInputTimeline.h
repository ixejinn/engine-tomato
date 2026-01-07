#ifndef TOMATO_PLAYERINPUTTIMELINE_H
#define TOMATO_PLAYERINPUTTIMELINE_H

#include "tomato/services/InputTypes.h"
#include "tomato/containers/RingArray.h"

namespace tomato
{
    class PlayerInputTimeline
    {
    public:
        // !!! 네트워크 지연 커버리지 기준으로 수정 필요 !!!
        // 100틱 - 1.67초 (최소한의 롤백 보장)
        // 300틱 - 5초
        // 500틱 - 8.33초
        constexpr static int INPUT_ROLLBACK_WINDOW = 128;

        const InputRecord& operator[](uint32_t tick) { return timeline_[tick]; }
        const InputRecord& operator[](uint32_t tick) const { return timeline_[tick]; }

        void SetInputTimeline(uint32_t tick, const InputRecord& record) { timeline_[tick] = record; }

    private:
        RingArray<InputRecord, INPUT_ROLLBACK_WINDOW> timeline_;
    };
}

#endif //TOMATO_PLAYERINPUTTIMELINE_H
