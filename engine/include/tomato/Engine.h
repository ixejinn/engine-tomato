#ifndef TOMATO_ENGINE_H
#define TOMATO_ENGINE_H

#include <chrono>
#include <memory>
#include <thread>
#include <array>

#include "tomato/services/InputService.h"           // InputService input_
#include "tomato/containers/Timeline.h"
#include "tomato/services/network/NetworkService.h" // NetworkService::MAX_PLAYER_NUM
#include "tomato/ecs/SystemManager.h"               // SystemManager systemManager_
#include "tomato/EngineConfig.h"
#include "tomato/net/rollback/RollbackManager.h"
#include "tomato/net/rollback/RollbackSlice.h"
#include "tomato/input/InputRecorder.h"

namespace tomato
{
    class WindowService;
    class World;
    class State;

    // TODO: NetworkService 사용 여부에 따라 생성?
    class Engine
    {
    public:
        using PlayerInputTimelines = std::array<Timeline<InputRecord>, EngineConfig::MAX_PLAYER_NUM>;

        static constexpr int FRAME_PER_SECOND{60};
        static constexpr float FIXED_DELTA_TIME{1.f / FRAME_PER_SECOND};

        explicit Engine(WindowService& window);
        ~Engine();

        void SetNextState(std::unique_ptr<State>&& newState);

        void Run();

        const WindowService& GetWindow() const { return window_; }

        const PlayerInputTimelines& GetInputTimeline() const { return inputTimelines_; }
        void SetInputData(uint8_t playerID, const InputRecord &record);

        World& GetWorld() { return *world_; }
        const World& GetWorld() const { return *world_; }

        uint32_t GetTick() const { return tick_; }

        uint32_t GetLatestTick() const { return latestTick_; }
        void SetLatestTick(uint32_t newTick) { latestTick_ = std::min(latestTick_, newTick); }

        NetworkService& GetNetworkService() { return network_; }

        template<typename... RollbackSliceT>
        void SetRollbackManager()
        {
            rollbackManager_ = std::make_unique<
                    RollbackManager<RollbackSliceT...>
                    >();
        }

    private:
        static constexpr int MAX_SIMULATION_NUM{3};
        static constexpr std::chrono::duration<float, std::milli> dt_{1000.f / FRAME_PER_SECOND};

        static constexpr int MAX_KEY_EVENTS_NUM{64};

        void ChangeState();
        void ResetTick();

        void ProcessKeyEvents();
        void Simulate();
        void Render();

        void ProcessNetPackets();
        void Rollback();

        WindowService& window_;

        InputService input_;
        std::vector<KeyEvent> keyEvents_;

        PlayerInputTimelines inputTimelines_;

        // 사용하는 경우에만 만들게 하던지..
        // 매칭 서버랑 연결될 때 내 playerID 설정할 수 있는 함수 필요
        NetworkService network_;

        uint32_t tick_{0};
        uint32_t latestTick_{0};

        std::unique_ptr<World> world_{nullptr};
        SystemManager systemManager_;

        InputRecorder inputRecorder_;

        std::unique_ptr<RollbackManagerB> rollbackManager_{nullptr};

        std::unique_ptr<State> currState_{nullptr};
        std::unique_ptr<State> nextState_{nullptr};
        bool isRunning_{true};

        std::chrono::duration<float, std::milli> adder_{0};
        std::chrono::steady_clock::time_point start_{};
    };
}

#endif //TOMATO_ENGINE_H
