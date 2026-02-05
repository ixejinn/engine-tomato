#ifndef TOMATO_NETBITWRITER_H
#define TOMATO_NETBITWRITER_H

#include <cstdint>
#include <concepts>

#include "tomato/services/network/CoreNetwork.h"

namespace tomato
{
    class NetBitWriter
    {
    public:
        NetBitWriter(uint8_t* output, int16_t byteSize);
        NetBitWriter(RawBuffer* rawBuffer)
        : buffer_(rawBuffer->data()), byteNum_(MAX_PACKET_SIZE), bitPos_(0) {}

        // inValue < maxValue
        template<typename T>
        requires std::same_as<T, uint8_t>
              || std::same_as<T, uint16_t>
              || std::same_as<T, uint32_t>
        void WriteInt(const T inValue, uint32_t maxValue)
        {
            SerializeInt(inValue, maxValue);
        }

        int16_t GetByteSize() const{
            if (bitPos_ < 0)
                return 0;
            return (bitPos_ + 7) / 8;
        }

    private:
        void SerializeInt(uint32_t value, uint32_t maxValue);

        uint8_t* buffer_{nullptr};  // reference
        int16_t byteNum_;
        int16_t bitPos_{-1};
    };
}

#endif //TOMATO_NETBITWRITER_H