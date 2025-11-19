#ifndef TOMATO_NETBITWRITER_H
#define TOMATO_NETBITWRITER_H

#include <cstdint>
#include <concepts>

namespace tomato
{
    class NetBitWriter
    {
    public:
        NetBitWriter(uint8_t* output, int16_t byteSize);

        // inValue < maxValue
        template<typename T>
        requires std::same_as<T, uint8_t>
              || std::same_as<T, uint16_t>
              || std::same_as<T, uint32_t>
        void WriteInt(const T inValue, const T maxValue)
        {
            SerializeInt(inValue, maxValue);
        }

    private:
        void SerializeInt(uint32_t value, uint32_t maxValue);

        uint8_t* buffer_{nullptr};  // reference
        int16_t byteNum_;
        int16_t bitPos_{-1};
    };
}

#endif //TOMATO_NETBITWRITER_H
