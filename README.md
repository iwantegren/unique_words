# Count unique words

This is a simple project that counts unique words in large text files.
We assume that our priority is lower complexity of algorithm and we can use large amounts of memory.

Tool implements two ways of counting words (actually storing unique words):
- Custom structure, described [below](#custom-implementation)
- Standard hash-table based implementation using `std::unordered_set`

## Parameters

Tool can be launched with different parameters:
| argument | type  | description                                                                                                                |
| -------- | ----- | -------------------------------------------------------------------------------------------------------------------------- |
| -i       | value | Input file. Requires following filename                                                                                    |
| -s       | flag  | Set standard implementation. Without this flag custom implementation will be used                                          |
| -p       | flag  | Set preprocess of input file, disabled by default. Used for files that contains characters other than [a-z] and whitespace |
| -v       | flag  | Set verbose mode, disabled by default                                                                                      |

## Structure

`/` contains C++ files and scripts for convenient build and tests

`res/` contains some small text files for testing

`benchmark/` contains executable for benchmarking and comparing implementations

## Scripts

`build.sh` build exectuable `unique_words` in default directory `out/build/`

`run_test.sh` run simple tests with already built executable in default build directory `out/build/`

`run_benchmark.sh` build and run benchmark executable from directory `benchmark/`

## Result of benchmark

Both implementations were tested on the same pretty small file `res/million.txt` ~6 Mb size.

Testing on large files >1 Gb size could provide different results.

```
2023-07-17T13:12:30+03:00
Running ./benchmark
Run on (16 X 2496 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x8)
  L1 Instruction 32 KiB (x8)
  L2 Unified 1280 KiB (x8)
  L3 Unified 24576 KiB (x1)
Load Average: 0.09, 0.03, 0.00
------------------------------------------------------------------------------
Benchmark                                    Time             CPU   Iterations
------------------------------------------------------------------------------
BM_count_unique_words_custom        1329120410 ns   1329096900 ns            1
BM_count_unique_words_unordered_set  188701378 ns    188409875 ns            4
```

## Custom implementation

This custom implementation is based on the idea of O(const)+ complexity for search/insertion operation.

The custom structure was made as n-ary tree, where n is dynamic number limited in number of letters in English - 26.
Each parent could contain up to 26 children for each separate letter. 

This way access to given word in tree will have O(k) complexity where k is number of letters in given word.
If we suppose that input text is regular text in English, we can assume that in general k is limited, so it could be considered as const.

Also considering cost of insertion of new elements we can say that complexity is amortized O(const)+

This implementation uses more memory and slower for small texts than `std::unordered_set`, but theoreticaly could provide more effeciency for really large amounts of text.