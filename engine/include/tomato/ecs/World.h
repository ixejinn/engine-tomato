#ifndef TOMATO_WORLD_H
#define TOMATO_WORLD_H

#include "tomato_ecs.h"

namespace tomato
{
    class World
    {
    public:
        Registry& GetRegistry() { return registry_; }
        const Registry& GetRegistry() const { return registry_; }

        Entity CreateEntity() { return registry_.create(); }

    private:
        Registry registry_;
    };
}

#endif //TOMATO_WORLD_H
