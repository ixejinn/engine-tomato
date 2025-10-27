#ifndef TOMATO_WORLD_H
#define TOMATO_WORLD_H

#include "entt/entt.hpp"

namespace tomato
{
    class World
    {
    public:
        entt::registry& GetRegistry() { return registry_; }
        const entt::registry& GetRegistry() const { return registry_; }

        entt::entity Create() { return registry_.create(); }

        template <typename T, typename... Args>
        T& Emplace(entt::entity e, Args&&... args) { return registry_.emplace<T>(e, std::forward<Args>(args)...); }

        template <typename... Ts>
        auto View() { return registry_.view<Ts...>(); }

        template <typename... Ts>
        auto View() const { return registry_.view<const Ts...>(); }

    private:
        entt::registry registry_;
    };
}

#endif //TOMATO_WORLD_H
