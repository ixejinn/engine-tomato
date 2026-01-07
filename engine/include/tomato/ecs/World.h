#ifndef TOMATO_WORLD_H
#define TOMATO_WORLD_H

#include <entt/entt.hpp>
#include "tomato/tomato_types.h"

namespace tomato
{
    class World
    {
    public:
        using Entity = entity;

        entt::registry& GetRegistry() { return registry_; }
        const entt::registry& GetRegistry() const { return registry_; }

        Entity Create() { return registry_.create(); }

        template<typename T, typename... Args>
        T& Emplace(entt::entity e, Args&&... args) { return registry_.emplace<T>(e, std::forward<Args>(args)...); }

        template<typename... Ts>
        auto View() { return registry_.view<Ts...>(); }

        template<typename... Ts>
        auto View() const { return registry_.view<const Ts...>(); }

        template<typename... Ts>
        decltype(auto) Get(Entity e) { return registry_.get<Ts...>(e); }

        template<typename T>
        void Set(Entity e, T value)
        {
            if (registry_.valid(e))
                registry_.get<T>(e) = value;
        }

        bool Valid(Entity e) { return registry_.valid(e); }

    private:
        entt::registry registry_;
    };
}

#endif //TOMATO_WORLD_H
