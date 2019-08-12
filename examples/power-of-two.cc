#include <thread>

#include "absl/random/random.h"
#include "benchmark/benchmark.h"

namespace sysprog {
namespace {

using uint64 = uint64_t;
constexpr uint64 kNumIters = uint64{1} << 20;

bool isPowerOfTwo(uint64 n) { return (n & (n - 1)) == 0; }

uint64 randomNonPowerOfTwo() {
  static auto gen = absl::BitGen();
  uint64 n = gen();
  while (isPowerOfTwo(n)) {
    n = gen();
  }
  return n;
}

uint64 randomPowerOfTwo() {
  static auto gen = absl::BitGen();
  uint64 n = gen();
  return uint64{1} << flsll(n);
}

void BM_Mod(benchmark::State& state) {
  auto gen = absl::BitGen();
  uint64 numerator = gen();
  const uint64 divisor = randomNonPowerOfTwo();
  benchmark::DoNotOptimize(numerator);
  for (auto _ : state) {
    for (uint64 i = 0; i < kNumIters; i++) {
      benchmark::DoNotOptimize(numerator % divisor);
    }
  }
}

void BM_BitMask(benchmark::State& state) {
  uint64 num = absl::BitGen()();
  const uint64 divisor = randomPowerOfTwo();
  const uint64 mask = divisor & (divisor - 1);
  benchmark::DoNotOptimize(num);
  for (auto _ : state) {
    for (uint64 i = 0; i < kNumIters; i++) {
      benchmark::DoNotOptimize(num & mask);
    }
  }
}

void BM_RightShiftBy1(benchmark::State& state) {
  uint64 num = absl::BitGen()();
  benchmark::DoNotOptimize(num);
  for (auto _ : state) {
    for (uint64 i = 0; i < kNumIters; i++) {
      benchmark::DoNotOptimize(num >> 1);
    }
  }
}

void BM_DivideBy2(benchmark::State& state) {
  uint64 num = absl::BitGen()();
  benchmark::DoNotOptimize(num);
  for (auto _ : state) {
    for (uint64 i = 0; i < kNumIters; i++) {
      benchmark::DoNotOptimize(num / 2);
    }
  }
}



BENCHMARK(BM_Mod);
BENCHMARK(BM_BitMask);
BENCHMARK(BM_RightShiftBy1);
BENCHMARK(BM_DivideBy2);

}  // namespace
}  // namespace sysprog
