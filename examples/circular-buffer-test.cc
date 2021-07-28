#include "examples/circular-buffer.h"

#include "benchmark/benchmark.h"

namespace sysprog {
namespace {

void BM_CircularBuffer(benchmark::State& state) {
  CircularBuffer<int> buffer(1024);
  for (auto _ : state) {
    buffer.Push(0);
    benchmark::DoNotOptimize(buffer.Pop());
  }
}

BENCHMARK(BM_CircularBuffer);

}  // namespace
}  // namespace sysprog
