# Common Systems Programming Optimizations & Tricks

A listing of some common optimization techniques and tricks for doing "systems
programming" to make your code run faster, be more efficient, and look cooler to
other people.


## Cache Lines & False Sharing

False sharing is a fairly well-understood problem in optimizing multi-threaded
code on modern SMP systems. The problem has been
[written](https://software.intel.com/en-us/articles/avoiding-and-identifying-false-sharing-among-threads)
[about](https://mechanical-sympathy.blogspot.com/2011/07/false-sharing.html)
[fairly](https://dzone.com/articles/false-sharing)
[extensively](https://herbsutter.com/2009/05/15/effective-concurrency-eliminate-false-sharing/),
but the idea is that memory on a machine is broken up into the smallest atomic
unit, called a "cache line" or "cache-line". As of 2019 they're usually
somewhere around 32-256 bytes, with 64 bytes being the most common. To support
multiple cores on a single machine reading from the same memory in an atomic
way, such as with check-and-set, only one core on a machine can have exclusive
access to a single cache line. The problem of false sharing is when you
accidentally put two atomic counters in the same cache line, where now atomic
accesses to one counter adversely effects atomic accesses to the other counter.

The name "false sharing" comes from this adverse effect, even though these two
counters shouldn't be affecting one another from a correctness standpoint. They
are, big air quotes incoming, "falsely sharing" a cache line for no good reason.

The solution is to force these counters onto separate cache lines, which you can
do in C/C++ by forcing the alignment of the members of a struct/class. In
[examples/cache-lines.cc](examples/cache-lines.cc) we use absl's
[`ABSL_CACHELINE_ALIGNED`](https://github.com/abseil/abseil-cpp/blob/fa00c321073c7ea40a4fc3dfc8a06309eae3d025/absl/base/optimization.h#L99-L148)
macro to achieve this.

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

The benchmark uses either 2, 3, or 4 threads, each bumping a separate atomic
counter 64K times. Here are the results on a 2013 MacBook Pro:

```
$ bazel run -c opt //examples:cache-lines
Cache Line Size: 64
sizeof(NormalCounters) = 64
sizeof(CacheLineAwareCounters) = 256
2019-08-11 13:14:46
Run on (4 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
---------------------------------------------------------------------------------
Benchmark                                          Time           CPU Iterations
---------------------------------------------------------------------------------
BM_CountersCacheLineFalseSharing/2           2993777 ns      36418 ns       1000
BM_CountersCacheLineFalseSharing/3           4366733 ns      44498 ns       1000
BM_CountersCacheLineFalseSharing/4           6110684 ns      44818 ns       1000
BM_CacheLineAwareCountersNoFalseSharing/2     453697 ns      31485 ns      10000
BM_CacheLineAwareCountersNoFalseSharing/3     668418 ns      43824 ns      10000
BM_CacheLineAwareCountersNoFalseSharing/4     845970 ns      38363 ns      10000
```

We can see `CPU` doesn't change much, since it's only measuring the main
thread's CPU usage, but `Time` clearly is much worse when we have false sharing
with `NormalCounters` and gets worse as we add more threads contending. It's
also interesting to note that `CacheLineAwareCounters` also sees degrading
performance with more threads, instead of scaling out perfectly.
