# Common Systems Programming Optimizations & Tricks

A listing of some common optimization techniques and tricks for doing "systems
programming" to make your code run faster, be more efficient, and look cooler to
other people.


## Cache Lines & False Sharing

To show the effect in practice, we benchmark two different structs of
std::atomic<int64> counters, `NormalCounters` and `CacheLineAwareCounters`.

```
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
```

The benchmark uses either 1, 2, 3, or 4 threads, each bumping a separate atomic
counter inside the struct 64K times. Here are the results on a 2013 MacBook Pro:

```
$ bazel run -c opt //examples:cache-lines
Executing tests from //examples:cache-lines
-----------------------------------------------------------------------------
Cache Line Size: 64
sizeof(NormalCounters) = 64
sizeof(CacheLineAwareCounters) = 256
2019-08-13 01:16:18
Run on (4 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
---------------------------------------------------------------------------
Benchmark                                    Time           CPU Iterations
---------------------------------------------------------------------------
BM_NormalCounters/threads:1                389 us        387 us       1812
BM_NormalCounters/threads:2               1264 us       2517 us        234
BM_NormalCounters/threads:3               1286 us       3718 us        225
BM_NormalCounters/threads:4               1073 us       3660 us        204
BM_CacheLineAwareCounters/threads:1        386 us        385 us       1799
BM_CacheLineAwareCounters/threads:2        200 us        400 us       1658
BM_CacheLineAwareCounters/threads:3        208 us        581 us       1152
BM_CacheLineAwareCounters/threads:4        193 us        721 us       1008
```

![Graph of CPU Time vs. # of Threads](/images/sysprog-false-sharing.png)

## The Magic Power of 2

In current hardware, division and modulo, is one of the most expensive
operations, where expensive here means "longest
latency". [Agner Fog's listing of instruction latencies](https://www.agner.org/optimize/instruction_tables.pdf)
lists Intel Skylake's `DIV` instruction operating on two 64 bit registers having
a latency of 35-88 cycles, compared to `ADD` instruction operating on the same
two 64 bit registers having a latency of 1 cycle.

To show how much faster using a bitmask vs. using vision, we measure executing
1M modulo operations, versus executing 1M bitmasks.

```
$ bazel run -c opt //examples:power-of-two
Executing tests from //examples:power-of-two
-----------------------------------------------------------------------------
2019-08-13 02:24:03
Run on (4 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
--------------------------------------------------------
Benchmark                 Time           CPU Iterations
--------------------------------------------------------
BM_Mod                 9261 us       9234 us         75
BM_BitMask              325 us        324 us       1984
```

## Lock Striping

Locks can be used for mutual exclusion when you want to have multiple threads
access shared data exclusively. The downside though is if the shared data is
frequently accessed and the critical section is non-trivial, your threads could
spend most of their time contending on the lock, instead of actually doing work.

One solution is *lock striping* by chunking up the data and then using different
locks for different chunks of data.

To show the perf improvement, we measure a single lock hash set againsta lock
striped hash set with varying number of chunks inserting 1M items.

```
$ bazel run -c opt //examples:lock-striping
Executing tests from //examples:lock-striping
-----------------------------------------------------------------------------
2019-08-24 22:24:37
Run on (4 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
--------------------------------------------------------------------------
Benchmark                                   Time           CPU Iterations
--------------------------------------------------------------------------
BM_SingleLock/threads:1                    65 ms         65 ms         11
BM_SingleLock/threads:2                   140 ms        254 ms          2
BM_SingleLock/threads:3                   140 ms        332 ms          3
BM_SingleLock/threads:4                   142 ms        405 ms          4
BM_LockStriping_4_Chunks/threads:1         71 ms         69 ms          9
BM_LockStriping_4_Chunks/threads:2         90 ms        178 ms          4
BM_LockStriping_4_Chunks/threads:3         89 ms        248 ms          3
BM_LockStriping_4_Chunks/threads:4         82 ms        299 ms          4
BM_LockStriping_8_Chunks/threads:1         70 ms         69 ms         10
BM_LockStriping_8_Chunks/threads:2         74 ms        143 ms          4
BM_LockStriping_8_Chunks/threads:3         71 ms        198 ms          3
BM_LockStriping_8_Chunks/threads:4         60 ms        200 ms          4
```

Graph:

![Graph of CPU Time vs. # of Threads](/images/sysprog-lock-striping.png)
