#ifndef TOMATO_MEMORYPOOL_H
#define TOMATO_MEMORYPOOL_H

#include <array>
#include <cstddef>

namespace tomato
{
    template<typename T, std::size_t N = 0>
    class MemoryPool
    {
    public:
        MemoryPool();

        T* Allocate();
        void Deallocate(T* data);

        size_t UsedSize() const { return usedSize_; }
        size_t FreeSize() const { return size_ - usedSize_; }

    private:
        T* pool_;

        int size_{0};
        int usedSize_{0};
    };

    template<typename T, std::size_t N>
    inline MemoryPool<T, N>::MemoryPool() : size_(N)
    {
    }
}

#endif //TOMATO_MEMORYPOOL_H
