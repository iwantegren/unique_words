#include <benchmark/benchmark.h>

#include <future>
#include "../src/utils/Utils.h"
#include "../src/concurrency/ConcurrencyUtils.h"

static const std::string filename = "../../res/million.txt";

static void BM_count_unique_words_custom(benchmark::State &state)
{
    for (auto _ : state)
    {
        count_unique_words(filename);
    }
}

BENCHMARK(BM_count_unique_words_custom);

static void BM_count_unique_words_unordered_set(benchmark::State &state)
{
    for (auto _ : state)
    {
        count_unique_words_set(filename);
    }
}

BENCHMARK(BM_count_unique_words_unordered_set);

static void BM_read_words_concurrency(benchmark::State &state)
{
    for (auto _ : state)
    {
        auto range_queues = Concurrency::make_range_queues(Concurrency::DEFAULT_PROCESSING_THREADS);

        auto read_future = std::async(std::launch::async, Concurrency::read_words, std::ref(filename), std::ref(range_queues));

        read_future.wait();
        read_future.get();
    }
}

BENCHMARK(BM_read_words_concurrency);

static void BM_count_unique_words_concurrency(benchmark::State &state)
{
    for (auto _ : state)
    {
        Concurrency::count_unique_words(filename, Concurrency::DEFAULT_THREADS);
    }
}

BENCHMARK(BM_count_unique_words_concurrency);

BENCHMARK_MAIN();