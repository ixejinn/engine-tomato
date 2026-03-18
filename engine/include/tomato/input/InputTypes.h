#ifndef TOMATO_INPUTTYPES_H
#define TOMATO_INPUTTYPES_H

#include "tomato/input/InputConstants.h"

namespace tomato
{
    /// Input axis value.
    struct InputAxis
    {
        float value;
    };

    /**
     * @brief A input event for a single key.
     */
    struct KeyEvent
    {
        KeyEvent(Key k, KeyAction a, float v, uint32_t t) : key(k), action(a), value(v), tick(t) {}

        Key key;
        KeyAction action;

        float value;

        uint32_t tick;
    };

    /**
     * @brief A input event for a mouse.
     */
    struct MouseEvent : public KeyEvent
    {
        MouseEvent(Key k, KeyAction a, float v, uint32_t t, float x, float y) : KeyEvent(k, a, v, t), xPos(x), yPos(y) {}

        float xPos, yPos;
    };
}

#endif //TOMATO_INPUTTYPES_H
