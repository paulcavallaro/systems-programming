#include "examples/circular-buffer.h"

#include "benchmark/benchmark.h"

namespace sysprog {
namespace {

void BM_CircularBuffer(benchmark::State& state) {
  CircularBuffer<int> buffer(1024);
  for (auto _ : state) {
    buffer.push_back(0);
    benchmark::DoNotOptimize(buffer.pop_front());
  }
}

BENCHMARK(BM_CircularBuffer);

}  // namespace
}  // namespace sysprog
