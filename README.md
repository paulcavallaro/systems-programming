# Systems Programming Examples

A list of some common optimization techniques for doing "systems programming" to
make your code run faster, more efficient, and just plain better.


## Cache Lines & False Sharing


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
