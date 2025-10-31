#ifndef TOMATO_ENGINE_H
#define TOMATO_ENGINE_H

#include <chrono>
#include <memory>
#include <vector>
#include "tomato/services/InputService.h"           // InputService input_
#include "tomato/services/CharacterInputHistory.h"  // std::vector<CharacterInputHistory> inputHistory_
#include "tomato/ecs/SystemManager.h"               // SystemManager systemManager_

namespace tomato
{
    class WindowService;
    class World;
    class State;

    class Engine
    {
    public:
        static constexpr int FRAME_PER_SECOND{60};
        static constexpr float FIXED_DELTA_TIME{1.f / FRAME_PER_SECOND};

        explicit Engine(WindowService& window);
        ~Engine();

        void ChangeState();
        void SetNextState(std::unique_ptr<State>&& newState);

        void Run();

        const WindowService& GetWindow() const { return window_; }

        const std::vector<CharacterInputHistory>& GetInputHistory() const { return inputHistory_; }

        World& GetWorld() { return *world_; }
        const World& GetWorld() const { return *world_; }

    private:
         static constexpr int MAX_PLAYER_NUM{4};
         static constexpr int MAX_SIMULATION_NUM{3};
         static constexpr std::chrono::duration<float, std::milli> dt_{1000.f / FRAME_PER_SECOND};

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
