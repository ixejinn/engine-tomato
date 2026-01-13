#include "tomato/Engine.h"
#include "tomato/DefaultState.h"
#include "tomato/SimulationContext.h"
#include "tomato/services/WindowService.h"
#include "tomato/services/network/NetworkService.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/systems/System.h"
#include "tomato/net/rollback/RollbackSlice.h"

#include <thread>

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

        tick_ = 0;
        start_ = std::chrono::steady_clock::now();

        if (rollbackManager_)
            rollbackManager_->Capture(*world_, 0);
    }

    void Engine::SetNextState(std::unique_ptr<State>&& newState) { nextState_ = std::move(newState); }

    void Engine::Run()
    {
        std::thread th(&NetworkService::Dispatch, &network_);
        int frameCnt = 0;

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
            inputTimelines_[network_.GetPlayerID()].SetData(tick_, input_.GetCurrInputRecord());

            // 네트워크 관련 객체:          
            // 다른 플레이어로부터 들어온 늦은 입력을 히스토리에 저장하고,
            // 롤백 해야 할 틱 번호 찾음
            latestTick_ = tick_;
            network_.ProcessPendingPacket();
            auto rollbackTick = latestTick_;

            // 롤백
            if (rollbackManager_ && rollbackTick != tick_)
            {
                rollbackManager_->Rollback(*world_, rollbackTick);
                std::cout << "Rollback to " << latestTick_ << "\n";
                while (rollbackTick < tick_)
                {
                    systemManager_.Simulate(*this, SimContext{rollbackTick});
                    ++rollbackTick;

                    rollbackManager_->Capture(*world_, rollbackTick);
                }
            }

            // 고정 시간 시뮬레이션
            std::chrono::steady_clock::time_point cur = std::chrono::steady_clock::now();
            adder_ += std::chrono::duration<float, std::milli>(cur - start_);
            int simulationNum = std::min(static_cast<int>(adder_ / dt_), MAX_SIMULATION_NUM);
            while (simulationNum--) {
                //std::cout << "current tick: " << tick_ << "\n";
                systemManager_.Simulate(*this, SimContext{tick_});
                network_.SendPacket(0);
                ++tick_;

                if (rollbackManager_)
                    rollbackManager_->Capture(*world_, tick_);

                adder_ -= dt_;
            }
            start_ = cur;

            // 렌더
            systemManager_.Render(*this, SimContext{tick_});

            window_.SwapBuffers();
            window_.PollEvents();

            frameCnt++;
        }

        network_.isNetThreadRunning_ = false;
        th.join();
    }

    void Engine::SetInputData(uint8_t playerID, const InputRecord &record)
    {
        inputTimelines_[playerID].SetData(record.tick, record);
    }
}