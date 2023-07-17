#include <benchmark/benchmark.h>

#include "../Utils.h"

static void BM_count_unique_words(benchmark::State &state)
{
    for (auto _ : state)
    {
        count_unique_words("../../res/million.txt");
    }
}

BENCHMARK(BM_count_unique_words);

static void BM_count_unique_words_set(benchmark::State &state)
{
    for (auto _ : state)
    {
        count_unique_words_set("../../res/million.txt");
    }
}

BENCHMARK(BM_count_unique_words_set);

BENCHMARK_MAIN();