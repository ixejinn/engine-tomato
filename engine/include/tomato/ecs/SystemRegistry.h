#ifndef TOMATO_SYSTEMREGISTRY_H
#define TOMATO_SYSTEMREGISTRY_H

#include <functional>
#include <memory>
#include <vector>
#include "tomato/tomato_sim.h"
#include "tomato/containers/EnumArray.h"

namespace tomato
{
    class System;

    using Factory = std::function<std::unique_ptr<System>()>;

    /**
     * @brief Global registry that stores system factory functions grouped by SystemPhase.
     *
     * 시스템 생성 함수를 저장하는 싱글톤 클래스.
     * SystemManager는 SystemRegistry를 통해 생성자에서 시스템 객체를 자동으로 생성할 수 있다.
     */
    class SystemRegistry
    {
    private:
        SystemRegistry() = default;

    public:
        ~SystemRegistry() = default;

        SystemRegistry& operator=(const SystemRegistry&) = delete;

        void RegisterFactory(SystemPhase phase, Factory&& factory);
        const std::vector<Factory>& GetFactory(SystemPhase phase);

        static SystemRegistry& GetInstance()
        {
            static SystemRegistry instance;
            return instance;
        }

    private:
        EnumArray<SystemPhase, std::vector<Factory>> factories_;
    };

    /**
     * @brief Helper struct used for static registration of system factories.
     *
     * 자동 등록을 위한 구조체.
     * 각 시스템 .cpp의 익명 네임스페이스에서 SystemRegistryEntry 정적 객체를 생성하여,
     * 시스템 생성 함수를 SystemRegistry에 자동으로 등록한다.
     * 이를 위한 REGISTER_SYSTEM(PHASE, CLASS) 매크로가 있다.
     */
    struct SystemRegistryEntry
    {
        SystemRegistryEntry(const SystemPhase phase, Factory&& factory)
        {
            SystemRegistry::GetInstance().RegisterFactory(phase, std::move(factory));
        }
    };
}

#define REGISTER_SYSTEM(PHASE, CLASS)\
namespace { static tomato::SystemRegistryEntry CLASS##Entry{PHASE, []() { return std::make_unique<tomato::CLASS>(); }}; }

#endif //TOMATO_SYSTEMREGISTRY_H
