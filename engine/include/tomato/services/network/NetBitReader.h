#ifndef TOMATO_NETBITREADER_H
#define TOMATO_NETBITREADER_H

#include <cstdint>
#include <concepts>

namespace tomato
{
    class NetBitReader
    {
    public:
        NetBitReader(uint8_t* const input, const int16_t& byteSize);

        // outValue < maxValue
        template<typename T>
        requires std::same_as<T, uint8_t>
              || std::same_as<T, uint16_t>
              || std::same_as<T, uint32_t>
        void ReadInt(T& outValue, const T& maxValue)
        {
            outValue = static_cast<T>(DeserializeInt(maxValue));
        }

    private:
        uint32_t DeserializeInt(const uint32_t maxValue);

        uint8_t* buffer_{nullptr};  // reference
        int16_t byteNum_;
        int16_t bitPos_{-1};
    };
}

#endif //TOMATO_NETBITREADER_H
