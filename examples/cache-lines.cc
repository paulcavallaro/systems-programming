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

void BumpCounter(absl::Notification* startblock, std::atomic<int64>* counter,
                 int64 count) {
  startblock->WaitForNotification();
  for (int64 i = 0; i < count; i++) {
    (*counter)++;
  }
}

void waitThreads(std::vector<std::thread>& threads) {
  for (auto& t : threads) {
    t.join();
  }
}

constexpr int64 kNumIncrements = int64{1} << 16;

template <typename CounterT>
void CounterBench(benchmark::State& state) {
  state.PauseTiming();
  CounterT counters;
  absl::Notification startblock;
  std::vector<std::thread> threads;
  for (int i = 0; i < state.range(0); i++) {
    threads.emplace_back(BumpCounter, &startblock, getCounter(counters, i),
                         kNumIncrements);
  }
  startblock.Notify();
  state.ResumeTiming();
  waitThreads(threads);
  benchmark::DoNotOptimize(counters);
}

void BM_CountersCacheLineFalseSharing(benchmark::State& state) {
  for (auto _ : state) {
    CounterBench<NormalCounters>(state);
  }
}

void BM_CacheLineAwareCountersNoFalseSharing(benchmark::State& state) {
  for (auto _ : state) {
    CounterBench<CacheLineAwareCounters>(state);
  }
}

// Try running with 2, 3, and then 4 threads all bumping counters in this struct
// which will all usually share a cache line.
BENCHMARK(BM_CountersCacheLineFalseSharing)->Arg(2)->Arg(3)->Arg(4);
BENCHMARK(BM_CacheLineAwareCountersNoFalseSharing)->Arg(2)->Arg(3)->Arg(4);

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
