#pragma once

#include <string>

#include "ConcurrencyDefs.h"

namespace Concurrency
{
    int count_unique_words(const std::string &filename, unsigned int threads_num);

    int process_words(std::mutex &m, std::shared_ptr<Queue> q_ptr);
    bool read_words(const std::string &filename, std::unique_ptr<RangeQueues> &range_queues);

    std::unique_ptr<RangeQueues> make_range_queues(unsigned int processing_threads_num);

    void test_queue_distribution(unsigned int processing_threads_num, std::unique_ptr<RangeQueues> &range_queues);
}