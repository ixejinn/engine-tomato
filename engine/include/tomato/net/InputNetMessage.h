#ifndef TOMATO_INPUTNETMESSAGE_H
#define TOMATO_INPUTNETMESSAGE_H

#include "tomato/net/NetMessage.h"
#include "tomato/input/InputRecord.h"

namespace tomato
{
    class NetBitReader;
    class NetBitWriter;
    class Engine;

    /**
     * @brief Network message carrying per-tick player input for rollback simulation.
     *
     * 롤백 넷코드를 위해 플레이어의 입력을 tick 단위로 전달하는 메시지.
     * 이동/점프/공격 등 의도가 있는 입력만 포함한다. 의도에 대한 결과(상태)는 포함하지 않음
     *
     * @note 필요 시, tick의 하위 비트만 전송하여 크기를 줄일 수 있다. (복원을 위한 후처리 필요)
     */

    // TODO: 입력 타임라인 복구 및 패킷 손실을 대비하여 최근 여러 tick의 입력을 묶어서 전송할 수 있도록 수정
    struct InputNetMessage : public NetMessage
    {
    private:
        void Serialize(NetBitWriter& writer) override;
        void Build(Engine& engine) override;

        void Deserialize(NetBitReader& reader) override;
        void Apply(SocketAddress &fromAddr, Engine &engine) override;

        uint32_t tick{0};
        InputRecord inputRecord;
    };
}

#endif //TOMATO_INPUTNETMESSAGE_H
