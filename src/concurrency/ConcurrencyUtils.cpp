#include "ConcurrencyUtils.h"

#include <iostream>
#include <fstream>
#include <sstream>
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
        end_of_input = false;

        std::ifstream file(filename);

        if (!file.is_open())
        {
            end_of_input = true;
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string word;
            while (iss >> word)
            {
                auto sync_queue = range_queues->at(word[0]);

                std::unique_lock<std::mutex> ul(sync_queue.m);
                sync_queue.q_ptr->push(word);
                ul.unlock();
            }
        }

        file.close();
        end_of_input = true;
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