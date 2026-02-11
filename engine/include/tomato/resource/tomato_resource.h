#ifndef TOMATO_TOMATO_RESOURCE_H
#define TOMATO_TOMATO_RESOURCE_H

#include <cstdint>

namespace tomato
{
    using AssetId = uint64_t;

    constexpr AssetId GetAssetID(const char* filename);

    /**
     * @brief Fowler-Noll-Vo(FNV)-1a hash function for 64-bit unsigned integer.
     * @see https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
     */
    constexpr AssetId FNV1a64B(const char* str);
}

#endif //TOMATO_TOMATO_RESOURCE_H
