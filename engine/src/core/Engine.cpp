#include "tomato/Engine.h"
#include "tomato/DefaultState.h"
#include "tomato/SimulationContext.h"
#include "tomato/services/WindowService.h"
#include "tomato/services/network/NetworkService.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/systems/System.h"

namespace tomato
{
    Engine::Engine(WindowService& window)
    : window_(window), network_(*this), systemManager_(SystemManager{})
    {
        ChangeState();
    }

    Engine::~Engine() = default;

    void Engine::ChangeState()
    {
        world_ = std::make_unique<World>();

        if (currState_)
        {
            currState_->Exit();

            if (nextState_)
            {
                currState_ = std::move(nextState_);
                currState_->Init(*world_);
            }
            else
                currState_.reset();
        }
        else
        {
            currState_ = std::make_unique<DefaultState>();
            currState_->Init(*world_);
        }

        start_ = std::chrono::steady_clock::now();
    }

    void Engine::SetNextState(std::unique_ptr<State>&& newState) { nextState_ = std::move(newState); }

    void Engine::Run()
    {
        while (!window_.ShouldClose() && isRunning_)
        {
            if (nextState_)
                ChangeState();

            if (!currState_)
                isRunning_ = false;

            // 입력
            window_.TMP_CheckEscapeKey();
            input_.BeginFrame();
            input_.UpdateRecord(window_.GetHandle(), tick_);
            inputHistory_[network_.GetPlayerID()].SetInputHistory(tick_, input_.GetCurrInputRecord());

            // 네트워크 관련 객체:          
            // 다른 플레이어로부터 들어온 늦은 입력을 히스토리에 저장하고,
            // 롤백 해야 할 틱 번호 찾음
            latestTick_ = tick_;
            network_.ProcessPendingPacket();

            // 롤백
            while (latestTick_ < tick_)
            {
                systemManager_.Simulate(*this, SimContext{latestTick_});
                latestTick_++;
            }

            // 고정 시간 시뮬레이션
            std::chrono::steady_clock::time_point cur = std::chrono::steady_clock::now();
            adder_ += std::chrono::duration<float, std::milli>(cur - start_);
            int simulationNum = std::min(static_cast<int>(adder_ / dt_), MAX_SIMULATION_NUM);
            while (simulationNum--) {
                // !!! 지금은 비어있는 Simulation Context 전달 !!!
                systemManager_.Simulate(*this, SimContext{tick_});

                adder_ -= dt_;
            }
            start_ = cur;

            // 렌더
            systemManager_.Render(*this, SimContext{});

            window_.SwapBuffers();
            window_.PollEvents();
        }
    }

    void Engine::SetInputHistory(uint8_t playerID, const tomato::InputRecord &record)
    {
        inputHistory_[playerID].SetInputHistory(record.tick, record);
    }
}