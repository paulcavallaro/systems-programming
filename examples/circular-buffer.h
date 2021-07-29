#pragma once

#include <vector>

static_assert(sizeof(size_t) == sizeof(uint64_t), "64-bit only");

constexpr size_t RoundUpToPow2(size_t x) {
  if (x == 1) return x;
  return 1 << ((8 * sizeof(size_t)) - __builtin_clzl(x - 1));
}

// Simple fixed-size circular or ring buffer. The buffer can hold size - 1
// items, and size is rounded up to the nearest power of 2.
template <typename T>
class CircularBuffer {
 public:
  CircularBuffer(size_t size) : size_(RoundUpToPow2(size)), mask_(size_ - 1) {
    buf_.reserve(size_);
  }

  void push_back(T item) {
    buf_[write_] = item;
    write_ = (write_ + 1) & mask_;
  }

  T pop_front() {
    auto ret = buf_[read_];
    read_ = (read_ + 1) & mask_;
    buf_[read_].~T();
    return ret;
  }

  T peek() {
    return buf_[read_];
  }

  bool empty() {
    return read_ == write_;
  }

  bool full() {
    return read_ == ((write_ + 1) & mask_);
  }

 private:
  size_t size_ = 0;
  size_t read_ = 0;
  size_t write_ = 0;
  size_t mask_ = 0;
  std::vector<T> buf_;
  bool full_ = false;
};
