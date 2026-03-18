#include "tomato/Engine.h"
#include "tomato/DefaultState.h"
#include "tomato/tomato_sim.h"
#include "tomato/services/WindowService.h"
#include "tomato/services/network/NetworkService.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/systems/System.h"
#include "tomato/input/InputTypes.h"
#include "tomato/Logger.h"
//#include "/yj/engine-tomato/contents/TestState.h"

namespace tomato
{
    Engine::Engine(WindowService& window)
    : window_(window), input_(window), network_(*this, NetMode::NM_Client), systemManager_(SystemManager{})
    {
        window_.SetWindowUserPointer(&input_);
        keyEvents_.reserve(MAX_KEY_EVENTS_NUM);

        ChangeState();
    }

    Engine::~Engine() = default;

    void Engine::SetNextState(std::unique_ptr<State>&& newState) { nextState_ = std::move(newState); }

    void Engine::Run()
    {
        network_.ThreadStart();
        while (!window_.ShouldClose() && isRunning_)
        {
            ProcessNetPackets();
            if (network_.GetNetState() == NetworkServiceState::NSS_Playing)
                Rollback();

            ProcessKeyEvents();

            Simulate();
            Render();

            TryStartGame();
            if (nextState_)
                ChangeState();
        }
        network_.ThreadStop();
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

    //@TEST CODE
    void Engine::TryStartGame()
    {
        if (network_.GetNetState() == NetworkServiceState::NSS_Starting)
        {
            if (localStartTime == 0) return;
            auto now = static_cast<ServerTimeMs>(
                duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now().time_since_epoch()).count());

            if (static_cast<int32_t>(now - localStartTime) >= 0)
            {
                network_.SetNetState(NetworkServiceState::NSS_Playing);
                //SetNextState(std::make_unique<TestState>());
                std::cout << now << "\n##### Game Start #####\n\n";
            }
        }
    }

    void Engine::ProcessKeyEvents()
    {
        keyEvents_.clear();

        window_.TMP_CheckEscapeKey();   //TODO: !!! 나중에 지울 것 !!!
        WindowService::PollEvents();

        input_.DrainKeyEvents(keyEvents_);
        // TODO: UI가 우선 소비 (소비하면 consumed = true)
        //inputRecorder_.UpdateInputAxis(keyEvents_, tick_);

        char c = inputRecorder_.TMP_UpdateInputAxis(keyEvents_, tick_);
#if 1
        switch (c)
        {
        case 1: //Match request
            std::cout << "SendTCPPacket(TCPPacketType::MATCH_REQUEST)\n";
            network_.SendTCPPacket(TCPPacketType::MATCH_REQUEST);
            break;

        case 2: //Send TIME_SYNC_REQ
            std::cout << "SendTCPPacket(TCPPacketType::TIME_SYNC_REQ)\n";
            network_.SendTCPPacket(TCPPacketType::TIME_SYNC_REQ);
            break;

        case 3:
            std::cout << "SendTCPPacket(TCPPacketType::MATCH_INTRO_SUCCESS)\n";
            network_.SendTCPPacket(TCPPacketType::MATCH_INTRO_SUCCESS);
            network_.SetNetState(NetworkServiceState::NSS_Lobby);
            break;

        case 4:
            std::cout << "SendTCPPacket(TCPPacketType::MATCH_INTRO_FAILED)\n";
            network_.SendTCPPacket(TCPPacketType::MATCH_INTRO_FAILED);
            break;
        }
#elif 0
        SocketAddress myAddr("192.168.45.239", 9000);
        //SocketAddress myAddr("192.168.45.192", 9001);
        //SocketAddress myAddr("172.24.160.1", 9001);
        switch (c)
        {
        case 1: //Match request
            std::cout << "SendUDPPacket(UDPPacketType::HELLO)\n";
            network_.SendUDPPacket(UDPPacketType::HELLO, SendPolicy::Unicast, &myAddr);
            break;

        case 2: //Send TIME_SYNC_REQ
            std::cout << "SendUDPPacket(UDPPacketType::WELCOME)\n";
            network_.SendUDPPacket(UDPPacketType::WELCOME, SendPolicy::Unicast, &myAddr);
            break;

        case 3:
            break;

        case 4:
            break;
        }
#endif
        inputTimelines_[network_.GetMyPlayerID()].SetData(tick_, inputRecorder_.GetCurrInputRecord());
    }

    void Engine::Simulate()
    {
        std::chrono::steady_clock::time_point cur = std::chrono::steady_clock::now();   // TODO: system_clock과 차이점 확인
        adder_ += std::chrono::duration<float, std::milli>(cur - start_);

        int simLimit = std::min(static_cast<int>(adder_ / dt_), MAX_SIMULATION_NUM);

        while (simLimit--) {
            systemManager_.Simulate(*this, SimContext{tick_});
            if(network_.GetNetState() == NetworkServiceState::NSS_Playing)
                network_.SendUDPPacket(UDPPacketType::INPUT, SendPolicy::Broadcast);
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
        
        network_.ProcessQueuedUDPPacket();
        network_.ProcessQueuedTCPPacket();
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