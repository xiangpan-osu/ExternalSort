# External Sort


### Overview
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


# Algorithm Design
The basic idea is quite simple:

I read key-value pairs from input and use multiset (a binary search tree based structure in C++) to store and sort them until I reach the memory limit. In that case I dump my current sorted key-value pairs from memory to a disk file and then clear my current memory. I keep track of how many files I have created and use the current file count as the file name for the next file to be created. I keep doing this until input are all read.

After parsing the input, I use an array to hold all the file pointers. In this way I can use an integer to index a particular file. I read one key from each file and combine its file index to form a key-file_index pair. I use a min heap to store these pairs. Number of elements of the min heap will be smaller or equal to the total number of created files. After extracting a key from each of the file, I can tell the minimum key by looking at the top of my min heap. At this time I start to load values of the current minimum key into memory. Again I use a multiset to store and sort the values. Since the question statement states that the size of all the values of any key can fit in the memory. So I don't need to worry about the memory limit will be exceeded here. After I collected and sorted all the values of the current minimum key, I will print them to stdout, clear that multiset container, and move on to the next minimum key and its values. I keep doing this until all the keys and their values are processed. In the end I will delete all the temporary files created in the first phase.


# Code Structure
There are two main functions in my code - scatter_input for parsing input and gather_output for printing output. More details can be found in my comments inside the code.


# Time/Space Complexity
Time Complexity: O(nlogn) - n is total number of key-value pairs from input.
Reason: We can view the whole program as to sort and output n key-value pairs. I use binary search tree and heap based structures to sort the input, both of which will take average O(nlogn) time to sort.

Space Complexity: O(mem) - mem is the size of the memory limit.
Reason: This program will not allow exceeding the memory usage of memory limit.


# Build/Test
To Build:
make -j8

To Clean:
make clean

For testing, I think the included testing scripts are pretty good, so I stick with them for testing my code. I assume $MAX_ALLOWED_MEMORY_IN_KB to be at least 100000 to start with, allowing my program to run. $MEM_LIMIT_IN_BYTES, $NUM_PAIRS, $KEY_LENGTH, and $VALUE_LENGTH variables are subject to change based on different testing needs.

To Test:
./run_test.sh ./kv_sort

Please DO NOT forget the "./" before "kv_sort", otherwise the test will fail. Here are some configurations I tried on my laptop. I haven't tried really large input since no doubt it will take a long time to run on my really old laptop. However you are free to try large input sets as you wish. I bet Databricks' machines will be super fast. :)

$MEM_LIMIT_IN_BYTES = 10000000
$MAX_ALLOWED_MEMORY_IN_KB = 100000

Small:
$NUM_PAIRS = 1000000
$KEY_LENGTH = 3
$VALUE_LENGTH = 10

Medium:
$NUM_PAIRS = 8000000
$KEY_LENGTH = 6
$VALUE_LENGTH = 20

Large:
$NUM_PAIRS = 16000000
$KEY_LENGTH = 12
