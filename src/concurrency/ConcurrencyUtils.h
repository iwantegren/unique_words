#pragma once

#include <string>

#include "ConcurrencyDefs.h"

namespace Concurrency
{
    int count_unique_words(const std::string &filename);

    int process_word_queue(Queue &q);
    void read_words_to_queues(const std::string &filename, std::unique_ptr<RangeQueues> &range_queues);

    std::unique_ptr<RangeQueues> make_range_queues(unsigned int threads_num);
}