#ifndef TOMATO_TIMELINE_H
#define TOMATO_TIMELINE_H

#include <cstddef>
#include <cstdint>
#include "tomato/net/rollback/CoreRollback.h"
#include "tomato/containers/RingArray.h"

namespace tomato
{
    template<typename T, std::size_t N = ROLLBACK_WINDOW>
    class Timeline
    {
    public:
        const T& operator[](uint32_t tick) { return data_[tick]; }
        const T& operator[](uint32_t tick) const { return data_[tick]; }

        void SetData(uint32_t tick, const T& newData) { data_[tick] = newData; }
        void SetData(uint32_t tick, T&& newData) { data_[tick] = std::move(newData); }

        template<typename... Args>
        void EmplaceData(uint32_t tick, Args&&... args) { data_[tick] = T(std::forward<Args>(args)...); }

    private:
        RingArray<T, N> data_;
    };
}

#endif //TOMATO_TIMELINE_H
