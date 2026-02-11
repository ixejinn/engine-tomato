#ifndef TOMATO_STATE_H
#define TOMATO_STATE_H

namespace tomato
{
    class World;

    /**
     * @brief Base interface for states.
     */
    class State
    {
    public:
        //TODO: src/core/State.cpp 수정
        State();
        virtual ~State();

        virtual void Init(World& world) = 0;
        virtual void Exit() = 0;
    };
}

#endif //TOMATO_STATE_H
