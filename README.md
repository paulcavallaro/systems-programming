# Encyclopedia of Common Systems Programming Optimizations & Tricks

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
[examples/cache-lines.cc](examples/cache-lines.cc) we use absl'
`ABSL_CACHELINE_ALIGNED` macro to achieve this.


```
Executing tests from //examples:cache-lines
-----------------------------------------------------------------------------
2019-08-10 06:00:55
Run on (4 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
---------------------------------------------------------------------------------
Benchmark                                          Time           CPU Iterations
---------------------------------------------------------------------------------
BM_CountersCacheLineFalseSharing/2           2929666 ns      38000 ns       1000
BM_CountersCacheLineFalseSharing/3           4370282 ns      46281 ns       1000
BM_CountersCacheLineFalseSharing/4           5564662 ns      49653 ns       1000
BM_CacheLineAwareCountersNoFalseSharing/2     462555 ns      31671 ns      10000
BM_CacheLineAwareCountersNoFalseSharing/3     683453 ns      42251 ns      10000
BM_CacheLineAwareCountersNoFalseSharing/4     865445 ns      40905 ns      10000
```
