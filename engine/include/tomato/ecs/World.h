#ifndef TOMATO_WORLD_H
#define TOMATO_WORLD_H

#include "tomato_ecs.h"

namespace tomato
{
    /**
     * @brief ECS world that owns entities and components, backed by EnTT.
     *
     * EnTT를 사용하지만, 엔진 외부로 직접 노출하지 않도록 최소한의 추상화 계층을 제공하는 클래스.
     */
    class World
    {
    public:
        World();

        Registry& GetRegistry() { return registry_; }
        const Registry& GetRegistry() const { return registry_; }

        Entity CreateEntity() { return registry_.create(); }

    private:
        void Init();

        Registry registry_;
    };
}

#endif //TOMATO_WORLD_H
