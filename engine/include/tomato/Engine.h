#ifndef TOMATO_ENGINE_H
#define TOMATO_ENGINE_H

#include <chrono>
#include <memory>
#include <thread>
#include <array>

#include "tomato/services/InputService.h"           // InputService input_
#include "tomato/net/rollback/PlayerInputTimeline.h"    // PlayersInputTimeline
#include "tomato/services/network/NetworkService.h" // NetworkService::MAX_PLAYER_NUM
#include "tomato/ecs/SystemManager.h"               // SystemManager systemManager_
#include "tomato/EngineConfig.h"

namespace tomato
{
    class WindowService;
    class World;
    class State;

    class Engine
    {
    public:
        using PlayersInputTimeline = std::array<PlayerInputTimeline, EngineConfig::MAX_PLAYER_NUM>;

        static constexpr int FRAME_PER_SECOND{60};
        static constexpr float FIXED_DELTA_TIME{1.f / FRAME_PER_SECOND};

        explicit Engine(WindowService& window);
        ~Engine();

        void ChangeState();
        void SetNextState(std::unique_ptr<State>&& newState);

        void Run();

        const WindowService& GetWindow() const { return window_; }

        const PlayersInputTimeline& GetInputTimeline() const { return inputTimeline_; }
        void SetInputTimeline(uint8_t playerID, const InputRecord &record);

        World& GetWorld() { return *world_; }
        const World& GetWorld() const { return *world_; }

        uint32_t GetTick() const { return tick_; }

        uint32_t GetLatestTick() const { return latestTick_; }
        void SetLatestTick(uint32_t newTick) { latestTick_ = newTick; }

        NetworkService& GetNetworkService() { return network_; }

    private:
        static constexpr int MAX_SIMULATION_NUM{3};
        static constexpr std::chrono::duration<float, std::milli> dt_{1000.f / FRAME_PER_SECOND};

        WindowService& window_;

        InputService input_;
        PlayersInputTimeline inputTimeline_;

        // 사용하는 경우에만 만들게 하던지..
        // 매칭 서버랑 연결될 때 내 playerID 설정할 수 있는 함수 필요
        NetworkService network_;

        uint32_t tick_{0};
        uint32_t latestTick_{0};

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
