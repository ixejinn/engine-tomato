#ifndef TOMATO_INPUTTYPES_H
#define TOMATO_INPUTTYPES_H

#include "tomato/input/InputConstants.h"

namespace tomato
{
    struct InputAxis
    {
        float value;
    };

    struct KeyEvent
    {
        KeyEvent(Key k, int a) : key(k), action(a) {}

        Key key;
        int action;
        bool consumed{false};
    };
}

#endif //TOMATO_INPUTTYPES_H
