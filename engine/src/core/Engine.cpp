#include "tomato/Engine.h"
#include "tomato/DefaultState.h"
#include "tomato/tomato_sim.h"
#include "tomato/services/WindowService.h"
#include "tomato/services/network/NetworkService.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/systems/System.h"
#include "tomato/input/InputTypes.h"
#include "tomato/Logger.h"

namespace tomato
{
    Engine::Engine(WindowService& window)
    : window_(window), input_(window), network_(*this), systemManager_(SystemManager{})
    {
        window_.SetWindowUserPointer(&input_);
        keyEvents_.reserve(MAX_KEY_EVENTS_NUM);

        ChangeState();
    }

    Engine::~Engine() = default;

    void Engine::SetNextState(std::unique_ptr<State>&& newState) { nextState_ = std::move(newState); }

    void Engine::Run()
    {
        std::thread th(&NetworkService::Dispatch, &network_);

        while (!window_.ShouldClose() && isRunning_)
        {
            ProcessNetPackets();
            Rollback();

            ProcessKeyEvents();

            Simulate();
            Render();

            if (nextState_)
                ChangeState();
        }

        network_.isNetThreadRunning_ = false;
        th.join();
    }

    void Engine::SetInputData(uint8_t playerID, const InputRecord &record)
    {
        inputTimelines_[playerID].SetData(record.tick, record);
    }

    void Engine::ChangeState()
    {
        if (!nextState_)
        {
            if (!currState_)
                nextState_ = std::make_unique<DefaultState>();
            else
            {
                TMT_WARN << "Incorrectly called";
                return;
            }
        }

        if (currState_)
            currState_->Exit();

        world_ = std::make_unique<World>();

        currState_ = std::move(nextState_);
        currState_->Init(*world_);

        ResetTick();
    }

    void Engine::ResetTick()
    {
        tick_ = 0;
        start_ = std::chrono::steady_clock::now();

        if (rollbackManager_)
            rollbackManager_->Capture(*world_, 0);
    }

    void Engine::ProcessKeyEvents()
    {
        keyEvents_.clear();

        window_.TMP_CheckEscapeKey();   //TODO: !!! 나중에 지울 것 !!!
        window_.PollEvents();
        input_.DrainKeyEvents(keyEvents_);
        // TODO: UI가 우선 소비 (소비하면 consumed = true)
        inputRecorder_.UpdateInputAxis(keyEvents_, tick_);

        inputTimelines_[network_.GetPlayerID()].SetData(tick_, inputRecorder_.GetCurrInputRecord());
    }

    void Engine::Simulate()
    {
        std::chrono::steady_clock::time_point cur = std::chrono::steady_clock::now();
        adder_ += std::chrono::duration<float, std::milli>(cur - start_);

        int simLimit = std::min(static_cast<int>(adder_ / dt_), MAX_SIMULATION_NUM);

        while (simLimit--) {
            systemManager_.Simulate(*this, SimContext{tick_});
            network_.SendPacket(0);
            ++tick_;

            if (rollbackManager_)
                rollbackManager_->Capture(*world_, tick_);

            adder_ -= dt_;
        }

        start_ = cur;
    }

    void Engine::Render()
    {
        systemManager_.Render(*this, SimContext{tick_});
        window_.SwapBuffers();
    }

    void Engine::ProcessNetPackets()
    {
        latestTick_ = tick_;
        network_.ProcessPendingPacket();
    }

    void Engine::Rollback()
    {
        if (!rollbackManager_ || latestTick_ == tick_)
            return;

        auto rollbackTick = latestTick_;
        if (rollbackTick < tick_)
        {
            rollbackManager_->Rollback(*world_, rollbackTick);
            //TMT_LOG << "Rollback to " << rollbackTick;

            while (rollbackTick < tick_)
            {
                systemManager_.Simulate(*this, SimContext{rollbackTick});
                ++rollbackTick;

                rollbackManager_->Capture(*world_, rollbackTick);
            }
        }
    }
}