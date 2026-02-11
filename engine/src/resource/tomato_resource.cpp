#include "tomato/resource/AssetRegistry.h"

namespace tomato
{
    constexpr AssetId GetAssetID(const char* filename) { return FNV1a64B(filename); }

    constexpr AssetId FNV1a64B(const char* str)
    {
        uint64_t hash = 14695981039346656037ULL;
        while (*str)
        {
            hash ^= (uint64_t)(*str);
            hash *= 1099511628211ULL;
        }

        return hash;
    }
}