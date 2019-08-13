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
-----------------------------------------------------------------------------------------
Benchmark                                                  Time           CPU Iterations
-----------------------------------------------------------------------------------------
BM_CountersCacheLineFalseSharing/threads:1            385352 ns     382320 ns       1822
BM_CountersCacheLineFalseSharing/threads:2           1511585 ns    3017959 ns        292
BM_CountersCacheLineFalseSharing/threads:3           1542650 ns    4428203 ns        231
BM_CountersCacheLineFalseSharing/threads:4           1470036 ns    5643076 ns        144
BM_CacheLineAwareCountersNoFalseSharing/threads:1     387208 ns     382898 ns       1818
BM_CacheLineAwareCountersNoFalseSharing/threads:2     198721 ns     396481 ns       1508
BM_CacheLineAwareCountersNoFalseSharing/threads:3     220657 ns     578803 ns       1155
BM_CacheLineAwareCountersNoFalseSharing/threads:4     192835 ns     755800 ns       1048
```

## The Magic Power of 2

In current hardware, division and modulo, is one of the most expensive
operations, where expensive here means "longest
latency". [Agner Fog's listing of instruction latencies](https://www.agner.org/optimize/instruction_tables.pdf)
lists Intel Skylake's `DIV` instruction operating on two 64 bit registers having
a latency of 35-88 cycles, compared to `ADD` instruction operating on the same
two 64 bit registers having a latency of 1 cycle.
