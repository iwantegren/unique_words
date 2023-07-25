#pragma once

#include <atomic>

#include "../utils/UniqueWordsBase.h"
#include "ConcurrencyDefs.h"

namespace Concurrency
{
    class UniqueWords : public ::UniqueWords
    {
    public:
        UniqueWords(const std::string &filename);

        int count() override;

        const unsigned int threads_num;
        const unsigned int processing_threads_num;

    protected:
        static unsigned int get_threads_num();

        std::unique_ptr<RangeQueues> make_range_queues();

        static bool read_words(const std::string &filename, std::unique_ptr<RangeQueues> &range_queues, unsigned int processing_threads_num);
        static int process_words(std::shared_ptr<Queue> queue);

        static void process_ready();
        static bool is_all_ready(unsigned int processing_threads_num);
    };

    void test_queue_distribution(unsigned int processing_threads_num, std::unique_ptr<RangeQueues> &range_queues);
}