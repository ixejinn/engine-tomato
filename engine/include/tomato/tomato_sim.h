#ifndef TOMATO_TOMATO_SIM_H
#define TOMATO_TOMATO_SIM_H

#include <cstdint>
#include <type_traits>
#include "tomato/Logger.h"

namespace tomato
{
    enum class SystemPhase : uint8_t
    {
        CONTROLLER = 0,
        INTEGRATOR,
        RENDER,
        CAMERA,
        COUNT
    };

    constexpr std::size_t ToIndex(SystemPhase phase)
    {
        auto idx = static_cast<std::size_t>(phase);

        if (phase >= SystemPhase::COUNT)
        {
            if (std::is_constant_evaluated())
                static_assert("Invalid system phase");
            else
            {
                TMT_ERR << "Invalid system phase " << idx;
                std::abort();
            }
        }

        return idx;
    }

    struct SimContext
    {
        uint32_t tick{0};
    };
}

#endif //TOMATO_TOMATO_SIM_H
