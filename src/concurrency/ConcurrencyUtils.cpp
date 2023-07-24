#include "ConcurrencyUtils.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <future>
#include <functional>
#include <unordered_set>
#include <chrono>
#include <thread>

#include "Sync.h"

namespace Concurrency
{
    int count_unique_words(const std::string &filename, unsigned int threads_num)
    {
        const auto processing_threads_num = threads_num - 1;
        auto range_queues = Concurrency::make_range_queues(processing_threads_num);

        end_of_input = false;
        auto input_thread = std::async(std::launch::async, read_words, std::ref(filename), std::ref(range_queues));

        std::future<int> processing_threads[processing_threads_num];
        for (auto i = 0; i < processing_threads_num; ++i)
        {
            processing_threads[i] = std::async(std::launch::async, process_words, std::ref(q_mutexes[i]), q_ptrs[i].lock());
        }

        input_thread.wait();
        if (input_thread.get() == false)
        {
            // Something wrong with input
            return -1;
        }

        int count = 0;
        for (auto &f : processing_threads)
        {
            f.wait();
            count += f.get();
        }

        return count;
    }

    int process_words(std::mutex &m, std::shared_ptr<Queue> q_ptr)
    {
        int count = 0;
        std::unordered_set<std::string> unique_words;

        std::unique_lock<std::mutex> l(m, std::defer_lock);

        while (true)
        {
            l.lock();
            if (!q_ptr->empty())
            {
                count += unique_words.emplace(q_ptr->front()).second;
                q_ptr->pop();
            }
            l.unlock();

            if (end_of_input)
            {
                l.lock();
                if (q_ptr->empty())
                    return count;
                l.unlock();
            }
        }
    }

    bool read_words(const std::string &filename, std::unique_ptr<RangeQueues> &range_queues)
    {
        end_of_input = false;

        std::ifstream file(filename);

        if (!file.is_open())
        {
            end_of_input = true;
            return false;
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

        return true;
    }

    std::unique_ptr<RangeQueues> make_range_queues(unsigned int processing_threads_num)
    {
        if (processing_threads_num > MAX_PROCESSING_THREADS)
            processing_threads_num = MAX_PROCESSING_THREADS;

        auto range_queues = std::make_unique<RangeQueues>();

        const float queue_weight_limit = TOTAL_WEIGHT / processing_threads_num;

        // Initial queue
        auto queue = std::make_shared<Queue>();
        unsigned int queue_idx = 0;
        q_ptrs[queue_idx] = queue;
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
                q_ptrs[queue_idx] = queue;
                current_weight -= queue_weight_limit;
            }
        }

        return range_queues;
    }

    void test_queue_distribution(unsigned int processing_threads_num, std::unique_ptr<RangeQueues> &range_queues)
    {
        if (processing_threads_num > MAX_PROCESSING_THREADS)
            processing_threads_num = MAX_PROCESSING_THREADS;

        char end = 'z' + 1;
        for (char c = 'a'; c != end; ++c)
        {
            auto sync_queue = range_queues->at(c);
            std::unique_lock<std::mutex> ul(sync_queue.m);
            std::string s;
            s += c;
            sync_queue.q_ptr->push(s);
            ul.unlock();
        }

        for (auto i = 0; i < processing_threads_num; ++i)
        {
            std::cout << i << " queue: [";

            float count = 0;
            auto q = q_ptrs[i].lock();
            while (!q->empty())
            {
                std::cout << q->front() << ", ";
                count += LetterFrequency.at(q->front()[0]);
                q->pop();
            }
            std::cout << "] = ";

            std::cout << count << "\n";
        }
    }
}