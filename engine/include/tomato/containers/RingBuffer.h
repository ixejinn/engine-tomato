#ifndef TOMATO_RINGBUFFER_H
#define TOMATO_RINGBUFFER_H
#include <atomic>
#include <cstddef>
#include <new>
#include <utility>

namespace tomato
{
#ifdef __cpp_lib_hardware_interference_size
	constexpr std::size_t CACHE_LINE_SIZE = std::hardware_destructive_interference_size;
#else
	constexpr std::size_t CACHE_LINE_SIZE = 64;
#endif

	template<typename T, std::size_t capacity>
	class RingBuffer
	{
	public:
		//RingBuffer()

	private:
		static constexpr std::size_t mask_ = capacity - 1;
		struct Cell {
			alignas(T) std::byte buffer[sizeof(T)];
		};

		alignas(CACHE_LINE_SIZE) std::atomic<size_t> write_{ 0 };
		alignas(CACHE_LINE_SIZE) std::atomic<size_t> read_{ 0 };
		alignas(CACHE_LINE_SIZE) std::array<cell_t, capacity> cells_;
	};
}




#endif //TOMATO_RINGBUFFER_H