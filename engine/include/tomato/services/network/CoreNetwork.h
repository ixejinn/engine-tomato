#ifndef TOMATO_CORENETWORK_H
#define TOMATO_CORENETWORK_H

#include <cstdint>
#include <array>

namespace tomato
{
    /// Maximum payload byte size.
    /// IP fragmentation을 피하기 위해 MTU 미만으로 제한.
    constexpr uint16_t MAX_PACKET_SIZE{1024};

    /// Raw byte buffer for network
    using RawBuffer = std::array<uint8_t, MAX_PACKET_SIZE>;
}

#endif //TOMATO_CORENETWORK_H
