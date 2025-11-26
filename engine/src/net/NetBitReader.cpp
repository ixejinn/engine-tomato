#include "tomato/net/NetBitReader.h"
#include "tomato/services/network/CoreNetwork.h"
#include "tomato/Logger.h"

namespace tomato
{
    NetBitReader::NetBitReader(uint8_t* const buffer, const int16_t byteSize)
    {
        if (byteSize <= MAX_PACKET_SIZE)
        {
            buffer_ = buffer;
            byteNum_ = byteSize;
            bitPos_ = 0;
        }
    }

    uint32_t NetBitReader::DeserializeInt(const uint32_t maxValue)
    {
        if (bitPos_ < 0)

        {
            TMT_ERR << "Invalid BitReader";
            return 0;
        }

        // Use local variable to avoid Load-Hit-Store
        uint32_t value = 0;
        int16_t pos = bitPos_;
        int16_t num = byteNum_;

        for (uint32_t mask = 1; (value + mask) < maxValue && mask; mask <<= 1, pos++)
        {
            if (pos >= num * 8)
            {
                TMT_ERR << "Invalid access";
                break;
            }

            if (buffer_[pos >> 3] & (1 << (pos & 7)))
                value |= mask;
        }

        bitPos_ = pos;
        return value;
    }
}