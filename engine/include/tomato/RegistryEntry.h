#ifndef TOMATO_REGISTRYENTRY_H
#define TOMATO_REGISTRYENTRY_H

#include "tomato/ecs/SystemRegistry.h"
#include "tomato/net/NetMessageRegistry.h"

namespace tomato
{
    /**
     * @brief Helper struct used for static registration of factories.
     *
     * 자동 등록을 위한 구조체.
     * 각 시스템 .cpp의 익명 네임스페이스에서 RegistryEntry 정적 객체를 생성하여,
     * 시스템 생성 함수를 SystemRegistry에 자동으로 등록한다.
     * 이를 위한 REGISTER_SYSTEM(PHASE, CLASS) 매크로가 있다.
     */
    struct RegistryEntry
    {
        RegistryEntry(const SystemPhase phase, SystemFactory&& factory)
        {
            SystemRegistry::GetInstance().RegisterFactory(phase, std::move(factory));
        }

        RegistryEntry(const NetMessageType type, NetMessageFactory&& factory)
        {
            NetMessageRegistry::GetInstance().RegisterFactory(type, std::move(factory));
        }
    };
}

#define REGISTER_SYSTEM(PHASE, CLASS)\
namespace { static tomato::RegistryEntry CLASS##Entry{PHASE, []() { return std::make_unique<tomato::CLASS>(); }}; }

#define REGISTER_NET_MESSAGE(TYPE, CLASS) \
namespace { static tomato::RegistryEntry CLASS##Entry{TYPE, []() { return std::make_unique<tomato::CLASS>(); }}; }

#endif //TOMATO_REGISTRYENTRY_H
