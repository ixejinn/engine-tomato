#ifndef TOMATO_NETBITWRITER_H
#define TOMATO_NETBITWRITER_H

#include <cstdint>
#include <concepts>

#include "tomato/services/network/CoreNetwork.h"

namespace tomato
{
    /**
     * @brief Bit-level writer for network message serialization.
     *
     * buffer_에 최소 비트만 사용하여 비트 단위 직렬화한다.
     *
     * @note 현재는 int 계열 기본 타입에 대한 직렬화만 지원하지만,
     *       향후 메시지 타입이 늘어날 경우, 기본 타입에 대한 비트 단위 처리는 BitWriter로 옮길 수 있다.
     */
    class NetBitWriter
    {
    public:
        /**
         * @param output
         * @param byteSize
         */
        NetBitWriter(uint8_t* output, int16_t byteSize);

        /**
         * @param rawBuffer
         */
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

    private:
        void SerializeInt(uint32_t value, uint32_t maxValue);

        uint8_t* buffer_{nullptr};
        int16_t byteNum_;
        int16_t bitPos_{-1};
    };
}

#endif //TOMATO_NETBITWRITER_H
