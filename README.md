# Count unique words

This is a simple project that counts unique words in large text files.
We assume that our priority is lower complexity of algorithm and we can use large amounts of memory.

Tool implements diiferent ways of counting unique words:
- Custom structure, described [below](#custom-implementation)
- Standard hash-table based implementation using `std::unordered_set`
- Multithreaded solution (**default**), described [below](#multithreaded-solution)

> Disclaimer:
> This soultion was made as test task. 
> 
> It wasn't properly tested and documented as it should be for a commercial product.
> 
> This code could possibly containg bugs and weak points.
## Parameters

Tool can be launched with different parameters:
| argument | type  | description                                                                                                                |
| -------- | ----- | -------------------------------------------------------------------------------------------------------------------------- |
| -i       | value | Input file. Requires following filename                                                                                    |
| -s       | flag  | Used only with `-nc` flag. Set standard implementation. Without this flag custom implementation will be used               |
| -p       | flag  | Set preprocess of input file, disabled by default. Used for files that contains characters other than [a-z] and whitespace |
| -v       | flag  | Set verbose mode, disabled by default                                                                                      |
| -nc      | flag  | Set no-concurrency mode that uses single thread solutions, disabled by default                                             |

## Structure

`/` contains scripts for convenient build and tests

`src/` contains C++ source files

`res/` contains some small text files for testing

`benchmark/` contains executable for benchmarking and comparing implementations

## Scripts

`build.sh` build exectuable `unique_words` in default directory `out/build/`

`run_test.sh` run simple tests with already built executable in default build directory `out/build/`

`run_benchmark.sh` build and run benchmark executable from directory `benchmark/`

`run_all.sh` runs all three scripts in order given above

## Result of benchmark

All implementations were tested on the same pretty small file `res/million.txt` ~6 Mb size.

Testing on large files >1 Gb size could provide different results.

```
2023-07-25T16:20:33+03:00
Running ./benchmark
Run on (16 X 2496 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 24576 KiB (x1)
Load Average: 0.32, 0.20, 0.19
------------------------------------------------------------------------------------
Benchmark                                          Time             CPU   Iterations
------------------------------------------------------------------------------------
BM_count_unique_words_custom                    1297 ms         1297 ms            1
BM_count_unique_words_unordered_set              189 ms          189 ms            4
BM_count_unique_words_concurrency                209 ms        0.999 ms          100
```

## Custom implementation

This custom implementation is based on the idea of O(const)+ complexity for search/insertion operation.

The custom structure was made as n-ary tree, where n is dynamic number limited in number of letters in English - 26.
Each parent could contain up to 26 children for each separate letter. 

This way access to given word in tree will have O(k) complexity where k is number of letters in given word.
If we suppose that input text is regular text in English, we can assume that in general k is limited, so it could be considered as const.

Also considering cost of insertion of new elements we can say that complexity is amortized O(const)+

This implementation uses more memory and slower for small texts than `std::unordered_set`, but theoreticaly could provide more effeciency for really large amounts of text.

## Multithreaded solution

This solution based on idea _single producer multiple consumer_ with low-locking.

In our program except the main thread we spawn N more threads (number of threads based on std::thread::hardware_concurrency but with some limitations)
```
thread[0]: producer thread for reading words from file and putting them into queues for consumers
thread[1]:
...
thread[N-1]: consumer thread for processing words from it's own queue
```

The idea of parallel computations is that we can filter all words into groups based on first letters. This way we will have different groups of words that could be stored and calculated independently.

We use statistical frequencies (weights) of first letters in english to achieve better performance.
With these weights we divide the alphabet into N-1 groups with approximately equal total weights.
Each group will have its own queue for words that starts on these letters and will be processed with separate thread.

However we needed some syncronization, so to avoid overhead with frequently used mutexes was used approach, where reading thread is working while all other threads are waiting. This approach is faster than parallel run of all threads and syncing with mutexes.

And to not load the whole file into memory, reading thread reads some part of the file (now it's 1000 000 words), then takes a break, starts processing threads. After they handle given words, reading thread will continue working and so on.

So, the multithreaded solution works in approximately the following steps:
```
- Initialize frequency based queues
- Run reading thread
  - It will wait for all other threads
- Run N-1 processing threads
  - Notify reading thread to start

- Read words until reach 1 mil words. Pause in this thread
- Process words presented in queues. When they are over pause and wait for next part.
...
- Read words until reach 1 mil words. Pause in this thread
- Process words presented in queues. When they are over pause and wait for next part.
...
- Read words until EOF. Stop this thread
- Process words presented in queues. When they are over and reached EOF - stop all threads 
  and return number of unique words from each thread. 
- Sum up all results.
```