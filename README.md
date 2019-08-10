# Systems Programming Examples

A list of some common optimization techniques for doing "systems programming" to
make your code run faster, more efficient, and just plain better.


## Cache Lines & False Sharing


Executing tests from //examples:cache-lines
-----------------------------------------------------------------------------
2019-08-10 05:54:30
Run on (4 X 2800 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 262K (x2)
  L3 Unified 4194K (x1)
---------------------------------------------------------------------------------
Benchmark                                          Time           CPU Iterations
---------------------------------------------------------------------------------
BM_CountersCacheLineFalseSharing/2           2929057 ns      37267 ns       1000
BM_CountersCacheLineFalseSharing/3           4327534 ns      48526 ns       1000
BM_CountersCacheLineFalseSharing/4           5808361 ns      48766 ns       1000
BM_CountersCacheLineAwareNoFalseSharing/2     461375 ns      31524 ns      10000
BM_CountersCacheLineAwareNoFalseSharing/3     674816 ns      42130 ns      10000
BM_CountersCacheLineAwareNoFalseSharing/4     861697 ns      40315 ns      10000
