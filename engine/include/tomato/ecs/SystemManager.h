#ifndef TOMATO_SYSTEMMANAGER_H
#define TOMATO_SYSTEMMANAGER_H

#include <array>
#include <vector>
#include <memory>

#include "tomato/containers/EnumArray.h"
#include "tomato/tomato_sim.h"

namespace tomato
{
    class Engine;
    class System;
    class World;

    /**
     * @brief Manages lifecycle, execution, and update order of all systems.
     *
     * 시스템 객체를 생성하고 소유하며, 각 시스템의 업데이트 함수를 호출해 컴포넌트 갱신을 수행하는 클래스.
     * 시스템의 업데이트 순서를 결정한다.
     */
    class SystemManager
    {
    public:
        SystemManager();
        ~SystemManager();

        void Simulate(Engine& engine, const SimContext& ctx);
        void Render(Engine& engine, const SimContext& ctx);

    private:
        using SystemPtr = std::unique_ptr<System>;

        static constexpr SystemPhase simulationOrder_[] =
                {
                SystemPhase::CONTROLLER,
                SystemPhase::INTEGRATOR
                };

        static constexpr SystemPhase renderOrder_[] =
                {
                SystemPhase::RENDER,
                SystemPhase::CAMERA
                };

        EnumArray<SystemPhase, std::vector<SystemPtr>> systems_;
    };
}

#endif //TOMATO_SYSTEMMANAGER_H
