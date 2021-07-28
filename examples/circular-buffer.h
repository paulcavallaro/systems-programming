#pragma once

#include <vector>

template <typename T>
class CircularBuffer {
 public:
  CircularBuffer(size_t size) : size_(size) { buf_.reserve(size_); }

  void Push(T item) {
    buf_[write_] = item;
    write_ = (write_ + 1) % size_;
  }

  T Pop() {
    auto ret = buf_[read_];
    read_ = (read_ + 1) % size_;
    return ret;
  }

 private:
  size_t size_ = 0;
  size_t read_ = 0;
  size_t write_ = 0;
  std::vector<T> buf_;
  bool full_ = false;
};
