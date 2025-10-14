#ifndef TOMATO_DEFAULTSTATE_H
#define TOMATO_DEFAULTSTATE_H

#include "tomato/State.h"

namespace tomato
{
    class DefaultState : public State
    {
    public:
        void Init(World& world) override;
        void Exit() override;
    };
}

#endif //TOMATO_DEFAULTSTATE_H
