#ifndef TOMATO_INPUTCONSTANTS_H
#define TOMATO_INPUTCONSTANTS_H

#include <cstdint>

namespace tomato
{
    /**
     * @brief Input code for buttons and axes.
     *
     * 키보드/마우스 버튼과 마우스 축(MouseX/MouseY)을 하나의 enum으로 관리한다.
     *
     * @details
     * AXIS_BEGIN을 기준으로 버튼과 축 영역을 구분하므로,
     * AXIS_BEGIN 자체는 실제 입력 코드가 아닌 경계로 취급한다.
     */
    enum class Key
    {
        UNKNOWN,

        // BUTTONS ------------------------------------------------
        LeftMouseButton,
        RightMouseButton,

        Escape,
        SpaceBar,

        Left,
        Up,
        Right,
        Down,

        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        // AXES ---------------------------------------------------
        AXIS_BEGIN,

        MouseX,
        MouseY,

        COUNT
    };

    inline bool IsButton(Key key) { return key < Key::AXIS_BEGIN; }

    /**
     * @brief Bitmask for logical player intents for a single simulation tick.
     */
    enum class InputIntent : uint16_t
    {
        NONE    = 0,

        UP      = 1 << 0,
        DOWN    = 1 << 1,
        LEFT    = 1 << 2,
        RIGHT   = 1 << 3,

        JUMP    = 1 << 4,

        COUNT
    };

    // bitwise operations for InputIntent flags ------------------------------

    inline InputIntent operator&(const InputIntent l, const InputIntent r)
    {
        return static_cast<InputIntent>(static_cast<uint16_t>(l) & static_cast<uint16_t>(r));
    }

    inline InputIntent operator|(const InputIntent l, const InputIntent r)
    {
        return static_cast<InputIntent>(static_cast<uint16_t>(l) | static_cast<uint16_t>(r));
    }

    inline InputIntent operator~(const InputIntent x)
    {
        return static_cast<InputIntent>(~static_cast<uint16_t>(x));
    }

    inline InputIntent operator^(const InputIntent l, const InputIntent r)
    {
        return static_cast<InputIntent>(static_cast<uint16_t>(l) ^ static_cast<uint16_t>(r));
    }

    inline InputIntent& operator|=(InputIntent& l, const InputIntent r)
    {
        l = l | r;
        return l;
    }

    inline InputIntent& operator&=(InputIntent& l, const InputIntent r)
    {
        l = l & r;
        return l;
    }

    inline bool HasIntent(const InputIntent x, const InputIntent intent)
    {
        return (x & intent) != InputIntent::NONE;
    }
}

#endif //TOMATO_INPUTCONSTANTS_H
