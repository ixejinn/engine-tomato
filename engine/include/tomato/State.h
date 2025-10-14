#ifndef TOMATO_STATE_H
#define TOMATO_STATE_H

namespace tomato
{
    class World;

    class State
    {
    public:
        virtual ~State() = default;

        virtual void Init(World& world) = 0;
        virtual void Exit() = 0;
    };
}

#endif //TOMATO_STATE_H
