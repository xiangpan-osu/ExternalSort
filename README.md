# External GroupBy

### Problem Statement

The goal is to implement a program which can group a collection of key-value pairs by their keys. The program will combine all values in sorted order associated with a key, and return the resulting keys in sorted order.
The program will take a memory limit as input as well as the key-value pairs from stdin. It should write the output (key, sorted list of values) in stdout. The function should work even if the input size exceeds the size of memory.

As an example:

input (stdin)
```
1000000
a y
b x
a z
```

expected output (stdout)
```
a y z
b x
```

### Tests

Run brute force solution included in this package
```
$ scripts/run_test.sh scripts/brute_force.py
Test finished successfully! Peak memory usage was 68888 kilobytes.
```

Run a wrong solution using system command `cat`
```
$ scripts/run_test.sh cat
Test failed! Peak memory usage was 768 kilobytes.
```
