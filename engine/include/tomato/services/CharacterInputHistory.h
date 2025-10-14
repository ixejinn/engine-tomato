#ifndef TOMATO_CHARACTERINPUTHISTORY_H
#define TOMATO_CHARACTERINPUTHISTORY_H

#include <vector>
#include "tomato/services/InputTypes.h"    // std::vector<InputRecord> history_

namespace tomato
{
    class CharacterInputHistory
    {
    private:
        // !!! 네트워크 지연 커버리지 기준으로 수정 필요 !!!
        // 100틱 - 1.67초 (최소한의 롤백 보장)
        // 300틱 - 5초
        // 500틱 - 8.33초
        constexpr static int MAX_INPUT_TICKS = 100;

        std::vector<InputRecord> history_{MAX_INPUT_TICKS};

    public:
        const InputRecord& operator[](uint32_t tick) { return history_[tick % MAX_INPUT_TICKS]; }

        void SetInputHistory(uint32_t tick, InputRecord record) { history_[tick % MAX_INPUT_TICKS] = record; }
    };
}

#endif //TOMATO_CHARACTERINPUTHISTORY_H
