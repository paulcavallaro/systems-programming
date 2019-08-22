#include <thread>

#include "absl/container/flat_hash_set.h"
#include "absl/synchronization/mutex.h"
#include "benchmark/benchmark.h"

namespace sysprog {
namespace {

using uint64 = uint64_t;

class ThreadSafeHashSet {
 public:
  void Insert(uint64 i) {
    absl::MutexLock lock(&mu_);
    hash_set_.insert(i);
  }

  bool Contains(uint64 i) {
    absl::MutexLock lock(&mu_);
    return hash_set_.contains(i);
  }

 private:
  absl::Mutex mu_;
  absl::flat_hash_set<uint64> hash_set_;
};

template <size_t kNumLocks>
class LockStripedHashSet {
 public:
  void Insert(uint64 i) {
    const size_t idx = i % kNumLocks;
    absl::MutexLock lock(&mu_[idx]);
    hash_set_[idx].insert(i);
  }

  bool Contains(uint64 i) {
    const size_t idx = i % kNumLocks;
    absl::MutexLock lock(&mu_[idx]);
    return hash_set_[idx].contains(i);
  }

 private:
  std::array<absl::Mutex, kNumLocks> mu_;
  std::array<absl::flat_hash_set<uint64>, kNumLocks> hash_set_;
};

constexpr uint64 kNumIters = 1 << 20;

void BM_SingleLock(benchmark::State& state) {
  // Make static so that each thread will use the same object.
  static ThreadSafeHashSet hash_set;

  for (auto _ : state) {
    for (uint64 i = 0; i < kNumIters; i++) {
      hash_set.Insert(i);
    }
    for (uint64 i = 0; i < kNumIters; i++) {
      benchmark::DoNotOptimize(hash_set.Contains(i));
    }
  }
  benchmark::DoNotOptimize(hash_set);
}

void BM_LockStriping_4(benchmark::State& state) {
  // Make static so that each thread will use the same object.
  static LockStripedHashSet<4> hash_set;

  for (auto _ : state) {
    for (uint64 i = 0; i < kNumIters; i++) {
      hash_set.Insert(i);
    }
    for (uint64 i = 0; i < kNumIters; i++) {
      benchmark::DoNotOptimize(hash_set.Contains(i));
    }
  }
  benchmark::DoNotOptimize(hash_set);
}

void BM_LockStriping_8(benchmark::State& state) {
  // Make static so that each thread will use the same object.
  static LockStripedHashSet<8> hash_set;

  for (auto _ : state) {
    for (uint64 i = 0; i < kNumIters; i++) {
      hash_set.Insert(i);
    }
    for (uint64 i = 0; i < kNumIters; i++) {
      benchmark::DoNotOptimize(hash_set.Contains(i));
    }
  }
  benchmark::DoNotOptimize(hash_set);
}

// Try running with differing number of threads to show contention interference.
BENCHMARK(BM_SingleLock)
    ->Threads(1)
    ->Threads(16)
    ->Threads(64)
    ->Threads(128);
BENCHMARK(BM_LockStriping_4)
    ->Threads(1)
    ->Threads(16)
    ->Threads(64)
    ->Threads(128);
BENCHMARK(BM_LockStriping_8)
    ->Threads(1)
    ->Threads(16)
    ->Threads(64)
    ->Threads(128);

}  // namespace
}  // namespace sysprog
