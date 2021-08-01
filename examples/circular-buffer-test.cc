#include "examples/circular-buffer.h"

#include <queue>

#include "benchmark/benchmark.h"

namespace sysprog {
namespace {

constexpr size_t kBufferSize = 2048;

void BM_CircularBuffer_InsertAndPop(benchmark::State& state) {
  CircularBuffer<int> buffer(kBufferSize);
  for (auto _ : state) {
    for (int i = 0; i < kBufferSize - 1; i++) {
      buffer.push_back(i);
    }
    while (!buffer.empty()) {
      buffer.pop_front();
    }
    benchmark::DoNotOptimize(buffer);
  }
}

void BM_CircularBuffer_Insert(benchmark::State& state) {
  CircularBuffer<int> buffer(kBufferSize);
  for (auto _ : state) {
    for (int i = 0; i < kBufferSize - 1; i++) {
      buffer.push_back(i);
    }
  }
  benchmark::DoNotOptimize(buffer);
}

void BM_StdQueue_InsertAndPop(benchmark::State& state) {
  std::queue<int> buffer;
  for (auto _ : state) {
    for (int i = 0; i < kBufferSize - 1; i++) {
      buffer.push(i);
    }
    while (!buffer.empty()) {
      buffer.pop();
    }
    benchmark::DoNotOptimize(buffer);
  }
}

void BM_StdQueue_Insert(benchmark::State& state) {
  for (auto _ : state) {
    std::queue<int> buffer;
    for (int i = 0; i < kBufferSize - 1; i++) {
      buffer.push(i);
    }
    benchmark::DoNotOptimize(buffer);
  }
}

BENCHMARK(BM_CircularBuffer_InsertAndPop);
BENCHMARK(BM_CircularBuffer_Insert);

BENCHMARK(BM_StdQueue_InsertAndPop);
BENCHMARK(BM_StdQueue_Insert);

}  // namespace
}  // namespace sysprog
