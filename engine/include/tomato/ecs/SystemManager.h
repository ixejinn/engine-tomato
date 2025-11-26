#ifndef TOMATO_SYSTEMMANAGER_H
#define TOMATO_SYSTEMMANAGER_H

#include <vector>
#include <memory>

namespace tomato
{
    class Engine;
    class System;
    class World;
    class SimContext;
    class CharacterInputHistory;

    class SystemManager
    {
    public:
        SystemManager();
        ~SystemManager();

        void Simulate(Engine& engine, const SimContext& ctx);
        void Render(const Engine& engine, const SimContext& ctx);

    private:
        std::vector<std::unique_ptr<System>> controllers_;
        std::vector<std::unique_ptr<System>> integrators_;
        std::vector<std::unique_ptr<System>> collisions_;
        std::vector<std::unique_ptr<System>> gameRules_;
        std::vector<std::unique_ptr<System>> spawners_;

        std::vector<std::unique_ptr<System>> renderers_;
        std::vector<std::unique_ptr<System>> camera_;
    };
}

#endif //TOMATO_SYSTEMMANAGER_H
