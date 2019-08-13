#include <iostream>
#include <thread>

#include "absl/synchronization/notification.h"
#include "benchmark/benchmark.h"

using int64 = int64_t;

// NormalCounters is straight forward naive implementation of a struct of
// counters.
struct ABSL_CACHELINE_ALIGNED NormalCounters {
  std::atomic<int64> success{0};
  std::atomic<int64> failure{0};
  std::atomic<int64> okay{0};
  std::atomic<int64> meh{0};
};

// CacheLineAwareCounters forces each counter onto a separate cache line to
// avoid any false sharing between the counters.
struct ABSL_CACHELINE_ALIGNED CacheLineAwareCounters {
  ABSL_CACHELINE_ALIGNED std::atomic<int64> success{0};
  ABSL_CACHELINE_ALIGNED std::atomic<int64> failure{0};
  ABSL_CACHELINE_ALIGNED std::atomic<int64> okay{0};
  ABSL_CACHELINE_ALIGNED std::atomic<int64> meh{0};
};

namespace sysprog {
namespace {

template <typename T>
std::atomic<int64>* getCounter(T& counters, int i) {
  switch (i) {
    case 0:
      return &counters.success;
    case 1:
      return &counters.failure;
    case 2:
      return &counters.okay;
    case 3:
      return &counters.meh;
    default:
      return nullptr;
  }
}

static_assert(
    sizeof(NormalCounters) != sizeof(CacheLineAwareCounters),
    "NormalCounters and CacheLineAwareCounters should have different sizes due "
    "to aligning members to different cache lines -- otherwise benchmarks will "
    "not show the difference in performance.");

constexpr int64 kNumIncrements = int64{1} << 16;

void BM_NormalCounters(benchmark::State& state) {
  // Make the counters static so that each thread will use the same counters.
  static NormalCounters counters;
  std::atomic<int64>* counter = getCounter(counters, state.thread_index);
  *counter = 0;
  for (auto _ : state) {
    for (int64 i = 0; i < kNumIncrements; i++) {
      (*counter)++;
    }
  }
  benchmark::DoNotOptimize(*counter);
}

void BM_CacheLineAwareCounters(benchmark::State& state) {
  // Make the counters static so that each thread will use the same counters.
  static CacheLineAwareCounters counters;
  std::atomic<int64>* counter = getCounter(counters, state.thread_index);
  *counter = 0;
  for (auto _ : state) {
    for (int64 i = 0; i < kNumIncrements; i++) {
      (*counter)++;
    }
  }
  benchmark::DoNotOptimize(*counter);
}

// Try running with 1, 2, 3, and then 4 threads all bumping separate counters in
// the given counters struct
BENCHMARK(BM_NormalCounters)->Threads(1)->Threads(2)->Threads(3)->Threads(4);
BENCHMARK(BM_CacheLineAwareCounters)
    ->Threads(1)
    ->Threads(2)
    ->Threads(3)
    ->Threads(4);

}  // namespace
}  // namespace sysprog

int main(int argc, char** argv) {
  std::cerr << "Cache Line Size: " << ABSL_CACHELINE_SIZE << std::endl;
  std::cerr << "sizeof(NormalCounters) = " << sizeof(NormalCounters)
            << std::endl;
  std::cerr << "sizeof(CacheLineAwareCounters) = "
            << sizeof(CacheLineAwareCounters) << std::endl;
  ::benchmark::Initialize(&argc, argv);
  if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
  ::benchmark::RunSpecifiedBenchmarks();
  return 0;
}
