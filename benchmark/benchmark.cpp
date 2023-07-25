#include <benchmark/benchmark.h>

#include "../src/utils/Utils.h"
#include "../src/concurrency/ConcurrencyUtils.h"

static const std::string filename = "../../res/million.txt";

static void BM_count_unique_words_custom(benchmark::State &state)
{
    for (auto _ : state)
    {
        TreeUniqueWords uw(filename);
        uw.count();
    }
}
BENCHMARK(BM_count_unique_words_custom);

static void BM_count_unique_words_unordered_set(benchmark::State &state)
{
    for (auto _ : state)
    {
        SetUniqueWords uw(filename);
        uw.count();
    }
}
BENCHMARK(BM_count_unique_words_unordered_set);

static void BM_count_unique_words_concurrency_no_sync(benchmark::State &state)
{
    for (auto _ : state)
    {
        Concurrency::UniqueWords uw(filename);
        uw.count();
    }
}
BENCHMARK(BM_count_unique_words_concurrency_no_sync);

BENCHMARK_MAIN();