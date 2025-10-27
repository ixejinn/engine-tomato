#ifndef TOMATO_SYSTEMMANAGER_H
#define TOMATO_SYSTEMMANAGER_H

#include <vector>
#include <memory>

namespace tomato
{
    class System;
    class World;
    class SimContext;
    class CharacterInputHistory;

    class SystemManager
    {
    public:
        SystemManager(std::vector<CharacterInputHistory>& inputHistory);
        ~SystemManager();

        void Simulate(World& world, const SimContext& ctx);
        void Render(const World& world, const SimContext& ctx);

    private:
        std::vector<std::unique_ptr<System>> controllers_;
        std::vector<std::unique_ptr<System>> integrators_;
        std::vector<std::unique_ptr<System>> collisions_;
        std::vector<std::unique_ptr<System>> gameRules_;
        std::vector<std::unique_ptr<System>> spawners_;

        std::vector<std::unique_ptr<System>> renderers_;
    };
}

#endif //TOMATO_SYSTEMMANAGER_H
