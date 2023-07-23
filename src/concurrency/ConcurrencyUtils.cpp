#include "ConcurrencyUtils.h"

#include <iostream>

namespace Concurrency
{
    int count_unique_words_concurrent(const std::string &filename)
    {
        return -1;
    }

    int process_word_queue(Queue &q)
    {
        return -1;
    }

    void read_words_to_queue(const std::string &filename)
    {
    }

    std::unique_ptr<RangeQueues> make_range_queues(unsigned int threads_num)
    {
        auto range_queues = std::make_unique<RangeQueues>();

        const float queue_weight_limit = TOTAL_WEIGHT / threads_num;

        float current_weight = queue_weight_limit;
        std::shared_ptr<Queue> queue;
        for (const auto &pair : LetterFrequency)
        {
            if (current_weight >= queue_weight_limit)
            {
                // When weight for current queue reaches limit - create a new one
                queue = std::make_shared<Queue>();
                current_weight -= queue_weight_limit;
            }

            current_weight += pair.second;
            range_queues->emplace(pair.first, queue);
        }

        return range_queues;
    }
}