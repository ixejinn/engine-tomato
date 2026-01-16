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

    /**
     * @brief Manages lifecycle, execution, and update order of all systems.
     *
     * 시스템 객체를 생성하고 소유하며, 각 시스템을 호출해 컴포넌트 갱신을 수행하는 클래스.
     * 역할(입력, 시뮬레이션, 게임플레이, 렌더링 등)에 따라 묶어 시스템을 호출한다.
     */
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
