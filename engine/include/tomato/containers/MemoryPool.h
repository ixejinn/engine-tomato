#ifndef TOMATO_MEMORYPOOL_H
#define TOMATO_MEMORYPOOL_H

#include <cstddef>
#include <cstring>
#include <cstdint>
#include <mutex>
#include "tomato/Logger.h"

#ifdef max
#undef max
#undef min
#endif // max

namespace tomato
{
    template<typename T, std::size_t N = 0>
    class MemoryPool
    {
    public:
        MemoryPool();
        ~MemoryPool();

        template<typename... Args>
        T* Allocate(Args&&... args)
        {
            if (free_ == nullptr)
            {
                TMT_ERR << "Fulled memory pool - Return nullptr";
                return nullptr;
            }

            mtx.lock();

            usedSize_++;

            void* cur = free_;
            // free_에 *free_(free_의 다음 청크 주소값) 복사
            std::memcpy(&free_, free_, sizeof(void*));

            // 할당 상태 비트 1
            auto curB = static_cast<std::byte*>(cur);
            auto beginB = static_cast<std::byte*>(pool_);
            auto offset = (curB - beginB) / chunkSize_;
            SetValid(offset);

            mtx.unlock();

            return ::new (cur) T(std::forward<Args>(args)...);
        }

        bool Deallocate(T* data)
        {
            // 범위 확인
            auto curB = reinterpret_cast<std::byte*>(data);
            auto beginB = static_cast<std::byte*>(pool_);
            auto endB = static_cast<std::byte*>(pool_) + N * chunkSize_;
            if (curB < beginB || curB >= endB)
            {
                TMT_LOG << "Not data in memory pool range";
                return false;
            }

            // 정렬 확인
            auto diffB = curB - beginB;
            if (diffB % chunkSize_ != 0)
            {
                TMT_LOG << "Not the starting address of the chunk of memory pool";
                return false;
            }

            mtx.lock();

            // 할당 상태 확인
            auto offset = diffB / chunkSize_;
            if (!IsValid(offset))
            {
                TMT_LOG << "Already returned memory chunk";
                return false;
            }

            usedSize_--;

            // 객체 소멸자 호출
            data->~T();

            // data에 free_(free 청크 첫번째 주소) 복사
            std::memcpy(data, &free_, sizeof(void*));
            free_ = data;

            // 할당 상태 비트 0
            SetInvalid(offset);

            mtx.unlock();

            return true;
        }

        size_t UsedSize() const { return usedSize_; }
        size_t FreeSize() const { return size_ - usedSize_; }

    private:
        bool IsValid(int idx) const { return valid_[idx >> 3] & (1 << (idx & 7)); }

        void SetValid(int idx)   { valid_[idx >> 3] |=  (1 << (idx & 7)); }
        void SetInvalid(int idx) { valid_[idx >> 3] &= ~(1 << (idx & 7)); }

        void* pool_{nullptr};
        void* free_{nullptr};

        uint8_t* valid_{nullptr};

        std::size_t size_{0};
        std::size_t usedSize_{0};

        std::size_t chunkSize_{0};

        std::mutex mtx;
    };

    template<typename T, std::size_t N>
    inline MemoryPool<T, N>::MemoryPool()
    : size_(N), chunkSize_(std::max(sizeof(void*), sizeof(T)))
    {
        // 공간 할당 및 정렬
        pool_ = ::operator new(
                chunkSize_ * N,
                std::align_val_t(std::max(alignof(void*), alignof(T))));

        // 청크 체인 연결
        auto base = static_cast<std::byte*>(pool_);
        for (int i = 0; i < N - 1; i++)
        {
            void* next = base + (i + 1) * chunkSize_;
            std::memcpy(base + i * chunkSize_, &next, sizeof(void*));
        }
        void* tmp = nullptr;
        std::memcpy(base + (N - 1) * chunkSize_, &tmp, sizeof(void*));
        free_ = pool_;

        // 할당 상태 배열 할당 및 초기화
        valid_ = new uint8_t[((N - 1) >> 3) + 1]();
    }

    template<typename T, std::size_t N>
    inline MemoryPool<T, N>::~MemoryPool()
    {
        // 반납되지 않은 객체 소멸자 호출
        auto base = static_cast<std::byte*>(pool_);
        for (int i = 0; i < N; i++)
        {
            if (IsValid(i))
            {
                T* cur = reinterpret_cast<T*>(base + i * chunkSize_);
                cur->~T();
            }
        }

        // 메모리 해제
        ::operator delete(pool_, std::align_val_t(std::max(alignof(void*), alignof(T))));
        delete[] valid_;
    }
}

#endif //TOMATO_MEMORYPOOL_H
