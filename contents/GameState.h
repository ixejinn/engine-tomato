#ifndef TOMATO_GAMESTATE_H
#define TOMATO_GAMESTATE_H

#include "tomato/State.h"

class GameState : public tomato::State
{
public:
    void Init(tomato::World& world);
    void Exit();
};

#endif //TOMATO_GAMESTATE_H
