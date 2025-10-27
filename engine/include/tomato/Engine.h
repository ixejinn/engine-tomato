#ifndef TOMATO_ENGINE_H
#define TOMATO_ENGINE_H

#include <chrono>
#include <memory>
#include <vector>
#include "tomato/services/WindowService.h"          // WindowService& window_
#include "tomato/services/InputService.h"           // InputService input_
#include "tomato/services/CharacterInputHistory.h"  // std::vector<CharacterInputHistory> inputHistory_
#include "tomato/ecs/SystemManager.h"               // SystemManager systemManager_

namespace tomato
{
    class World;
    class State;

    class Engine
    {
    public:
        explicit Engine(WindowService& window);
        ~Engine();

        void ChangeState();
        void SetNextState(std::unique_ptr<State>&& newState);

        void Run();

    private:
        constexpr static int MAX_PLAYER_NUM{4};
        constexpr static int MAX_SIMULATION_NUM{3};
        constexpr static int FRAME_PER_SECOND{60};
        constexpr static std::chrono::duration<float, std::milli> dt_{1000.f / FRAME_PER_SECOND};

        WindowService& window_;

        InputService input_;
        std::vector<CharacterInputHistory> inputHistory_{MAX_PLAYER_NUM};

        std::unique_ptr<World> world_{nullptr};
        SystemManager systemManager_;

        std::unique_ptr<State> currState_{nullptr};
        std::unique_ptr<State> nextState_{nullptr};
        bool isRunning_{true};

        std::chrono::duration<float, std::milli> adder_{0};
        std::chrono::steady_clock::time_point start_{};
    };
}

#endif //TOMATO_ENGINE_H
