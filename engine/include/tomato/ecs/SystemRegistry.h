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

    using SystemFactory = std::function<std::unique_ptr<System>()>;

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

        SystemRegistry(const SystemRegistry&) = delete;
        SystemRegistry& operator=(const SystemRegistry&) = delete;

        void RegisterFactory(SystemPhase phase, SystemFactory&& factory);
        const std::vector<SystemFactory>& GetFactory(SystemPhase phase);

        static SystemRegistry& GetInstance()
        {
            static SystemRegistry instance;
            return instance;
        }

    private:
        EnumArray<SystemPhase, std::vector<SystemFactory>> factories_{};
    };
}

#endif //TOMATO_SYSTEMREGISTRY_H
