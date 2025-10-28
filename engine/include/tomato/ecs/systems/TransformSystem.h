#ifndef TOMATO_TRANSFORMSYSTEM_H
#define TOMATO_TRANSFORMSYSTEM_H

#include "System.h"

namespace tomato
{
    class Engine;

    class TransformSystem : public System
    {
    public:
        void Update(Engine& engine, const SimContext& ctx) override;
    };
}

#endif //TOMATO_TRANSFORMSYSTEM_H
