#ifndef TOMATO_SPSCQ_H
#define TOMATO_SPSCQ_H

#include <vector>
#include <atomic>
#include <tomato/Logger.h>

template<typename T>
class SPSCQueue
{
public:
	explicit SPSCQueue(size_t capacity) : capacity_(capacity), buffer_(capacity), head_(0), tail_(0) {}

	bool Enqueue(const T& item)
	{
		size_t head = head_.load(std::memory_order_relaxed);
		size_t next = (head + 1) % capacity_;

		if (next == tail_.load(std::memory_order_acquire))
		{
			TMT_ERR << "SPSCQueue is full.";
			return false;
		}

		buffer_[head] = item;
		head_.store(next, std::memory_order_release);

		return true;
	}

	bool Dequeue(T& out)
	{
		size_t tail = tail_.load(std::memory_order_relaxed);

		if (Empty())
		{
			TMT_ERR << "SPSCQueue is empty.";
			return false;
		}

		out = buffer_[tail];
		tail_.store((tail + 1) % capacity_, std::memory_order_release);

		return true;
	}

	bool Empty() const { return head_ == tail_;	}
	const size_t GetSize() const { return capacity_; }
	
#ifdef _DEBUG
	const T& DebugTailValue() const
	{
		size_t last = (tail == 0) ? (capacity_ - 1) : (tail_ - 1);
		return buffer_[last];
	}
#endif

private:
	size_t capacity_;
	std::vector<T> buffer_;

	std::atomic<size_t> head_; //only producer
	std::atomic<size_t> tail_; //only consumer
};

#endif // !TOMATO_SPSCQ_H
