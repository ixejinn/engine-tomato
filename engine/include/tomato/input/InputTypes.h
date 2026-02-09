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
     * @brief A raw input event for a single key/button.
     */
    struct KeyEvent
    {
        KeyEvent(Key k, KeyAction a) : key(k), action(a) {}

        Key key;
        KeyAction action;

        /// Indicates whether this event has been consumed by the UI layer.
        bool consumed{false};
    };
}

#endif //TOMATO_INPUTTYPES_H
