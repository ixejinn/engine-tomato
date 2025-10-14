#ifndef TOMATO_TESTSTATE_H
#define TOMATO_TESTSTATE_H

#include "tomato/State.h"

class TestState : public tomato::State
{
public:
    void Init(tomato::World& world);
    void Exit();
};

#endif //TOMATO_TESTSTATE_H
