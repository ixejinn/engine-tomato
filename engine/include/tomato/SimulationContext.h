#ifndef TOMATO_SIMULATIONCONTEXT_H
#define TOMATO_SIMULATIONCONTEXT_H

#include <cstdint>

namespace tomato
{
    struct SimContext
    {
        uint32_t tick{0};
    };
}

#endif //TOMATO_SIMULATIONCONTEXT_H
