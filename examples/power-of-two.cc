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

uint64 getNum() {
  static auto gen = absl::BitGen();
  return gen();
}

void BM_Mod(benchmark::State& state) {
  uint64 num = getNum();
  uint64 divisor = randomNonPowerOfTwo();
  for (auto _ : state) {
    for (uint64 i = 0; i < kNumIters; i++) {
      uint64 res = num % divisor;
      benchmark::DoNotOptimize(res);
    }
  }
  benchmark::DoNotOptimize(num);
}

void BM_BitMask(benchmark::State& state) {
  uint64 num = getNum();
  uint64 divisor = randomPowerOfTwo();
  uint64 mask = divisor & (divisor - 1);
  for (auto _ : state) {
    for (uint64 i = 0; i < kNumIters; i++) {
      uint64 res = num & mask;
      benchmark::DoNotOptimize(res);
    }
  }
}

void BM_RightShiftBy1(benchmark::State& state) {
  uint64 num = getNum();
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

void BM_DivideBy3(benchmark::State& state) {
  uint64 num = absl::BitGen()();
  benchmark::DoNotOptimize(num);
  for (auto _ : state) {
    for (uint64 i = 0; i < kNumIters; i++) {
      benchmark::DoNotOptimize(num / 3);
    }
  }
}

BENCHMARK(BM_Mod)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_BitMask)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_RightShiftBy1)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_DivideBy2)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_DivideBy3)->Unit(benchmark::kMicrosecond);

}  // namespace
}  // namespace sysprog
