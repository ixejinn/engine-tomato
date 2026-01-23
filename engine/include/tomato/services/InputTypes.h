#ifndef TOMATO_INPUTTYPES_H
#define TOMATO_INPUTTYPES_H

#include <cstdint>

namespace tomato
{
    /// Bitmask representing player input actions.
    /// 플레이어의 조작 입력을 비트 플래그 형태로 묶어 하나의 uint16_t 값으로 표현한다.
    /// 각 비트는 하나의 입력 행동을 의미한다.
    enum class InputAction : uint16_t
    {
        NONE    = 0,
        UP      = 1 << 0,
        DOWN    = 1 << 1,
        LEFT    = 1 << 2,
        RIGHT   = 1 << 3,
        JUMP    = 1 << 4,
        END
    };

    inline InputAction operator&(const InputAction l, const InputAction r)
    {
        return static_cast<InputAction>(static_cast<uint16_t>(l) & static_cast<uint16_t>(r));
    }

    inline InputAction operator|(const InputAction l, const InputAction r)
    {
        return static_cast<InputAction>(static_cast<uint16_t>(l) | static_cast<uint16_t>(r));
    }

    inline InputAction operator~(const InputAction x)
    {
        return static_cast<InputAction>(~static_cast<uint16_t>(x));
    }

    inline InputAction operator^(const InputAction l, const InputAction r)
    {
        return static_cast<InputAction>(static_cast<uint16_t>(l) ^ static_cast<uint16_t>(r));
    }

    inline InputAction& operator|=(InputAction& l, const InputAction r)
    {
        l = l | r;
        return l;
    }

    inline bool HasAction(const InputAction x, const InputAction action)
    {
        return (x & action) != InputAction::NONE;
    }

    /**
     * @brief Input state recorded for a single simulation tick.
     *
     * 특정 틱에서 한 플레이어의 입력 상태를 나타낸다.
     */
    struct InputRecord
    {
        uint32_t tick{0};

        /// Key state (held)
        InputAction key{0};
        /// Key down edge (up → down)
        InputAction keydown{0};
    };
}

#endif //TOMATO_INPUTTYPES_H
