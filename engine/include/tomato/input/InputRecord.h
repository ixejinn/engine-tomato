#ifndef TOMATO_INPUTRECORD_H
#define TOMATO_INPUTRECORD_H

#include "tomato/input/InputConstants.h"

namespace tomato
{
    /**
     * @brief Per-tick input snapshot used by the simulation (and rollback).
     *
     * 특정 틱에서 한 플레이어의 입력 스냅샷.
     */
    struct InputRecord
    {
        uint32_t tick{0};

        /// Key state (held)
        InputIntent held{InputIntent::NONE};
        /// Key down edge (up → down)
        InputIntent down{InputIntent::NONE};
    };
}

#endif //TOMATO_INPUTRECORD_H
