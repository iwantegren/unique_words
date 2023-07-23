#include "ConcurrencyUtils.h"

#include <iostream>
#include "Sync.h"

namespace Concurrency
{
    int count_unique_words(const std::string &filename)
    {
        auto range_queues = Concurrency::make_range_queues(DEFAULT_THREADS);
        read_words_to_queues(filename, range_queues);

        for (auto &pair : *range_queues.get())
        {
            std::cout << pair.first << " - queue: [";

            Queue *q = pair.second.q_ptr.get();
            while (!q->empty())
            {
                std::cout << q->front() << ", ";
                q->pop();
            }
            std::cout << "]\n";
        }

        return -1;
    }

    int process_word_queue(SyncQueue &sync)
    {
        // Create processign threads that do:
        //      1. Check if queue empty
        //      2. Take words from queue and process them
        //      3. If queue empty check flag end
        //      4. Return number of unique words
        return -1;
    }

    void read_words_to_queues(const std::string &filename, std::unique_ptr<RangeQueues> &range_queues)
    {
        char c_end = 'z' + 1;
        for (char c = 'a'; c != c_end; ++c)
        {
            range_queues->at(c).q_ptr->push({c});
            std::cout << "push " << c << "\n";
        }
        // One thread for reading words and putting them into queues
        // oher threads for processing
    }

    std::unique_ptr<RangeQueues> make_range_queues(unsigned int threads_num)
    {
        if (threads_num < 1)
            threads_num = DEFAULT_THREADS;
        else if (threads_num > MAX_THREADS)
            threads_num = MAX_THREADS;

        auto range_queues = std::make_unique<RangeQueues>();

        const float queue_weight_limit = TOTAL_WEIGHT / threads_num;

        // Initial queue
        auto queue = std::make_shared<Queue>();
        unsigned int queue_idx = 0;
        float current_weight = 0;

        for (const auto &pair : LetterFrequency)
        {
            range_queues->emplace(pair.first, SyncQueue{q_mutexes[queue_idx], queue});
            current_weight += pair.second;

            if (current_weight >= queue_weight_limit)
            {
                // When weight for current queue reaches limit - create a new one
                queue = std::make_shared<Queue>();
                ++queue_idx;
                current_weight -= queue_weight_limit;
            }
        }

        return range_queues;
    }
}