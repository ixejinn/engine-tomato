#ifndef TOMATO_INPUTTYPES_H
#define TOMATO_INPUTTYPES_H

#include <cstdint>

namespace tomato
{
    // 조작 입력을 비트 마스킹
    enum class InputAction : uint16_t
    {
        NONE    = 0,
        UP      = 1 << 0,
        DOWN    = 1 << 1,
        LEFT    = 1 << 2,
        RIGHT   = 1 << 3,
        JUMP    = 1 << 4
    };

    inline InputAction operator|(const InputAction& l, const InputAction& r)
    {
        return static_cast<InputAction>(static_cast<uint16_t>(l) | static_cast<uint16_t>(r));
    }

    inline InputAction operator&(const InputAction& l, const InputAction& r)
    {
        return static_cast<InputAction>(static_cast<uint16_t>(l) & static_cast<uint16_t>(r));
    }

    inline InputAction operator^(const InputAction& l, const InputAction& r)
    {
        return static_cast<InputAction>(static_cast<uint16_t>(l) ^ static_cast<uint16_t>(r));
    }

    inline InputAction operator~(const InputAction& x)
    {
        return static_cast<InputAction>(~static_cast<uint16_t>(x));
    }

    inline InputAction& operator|=(InputAction& l, const InputAction& r)
    {
        l = l | r;
        return l;
    }

    inline bool HasAction(const InputAction& x, const InputAction& action)
    {
        return (x & action) != InputAction::NONE;
    }

    struct InputRecord
    {
        uint32_t tick{0};

        InputAction keypress{0};    // 눌러진 상태
        InputAction keydown{0};     // 누르는 순간
    };
}

#endif //TOMATO_INPUTTYPES_H
