#ifndef TOMATO_CORENETWORK_H
#define TOMATO_CORENETWORK_H

#include <cstdint>
#include <array>

namespace tomato
{
    constexpr uint16_t MAX_PACKET_SIZE{1024};

    using RawBuffer = std::array<uint8_t, MAX_PACKET_SIZE>;
}

#endif //TOMATO_CORENETWORK_H
