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
    UniqueWords::UniqueWords(const std::string &filename) : ::UniqueWords(filename),
                                                            threads_num(get_threads_num()),
                                                            processing_threads_num(threads_num - 1)
    {
    }

    int UniqueWords::count()
    {
        auto range_queues = make_range_queues();

        end_of_input = false;
        ready_to_process = false;
        auto input_thread = std::async(std::launch::async, read_words, std::ref(filename), std::ref(range_queues), processing_threads_num);

        std::future<int> processing_threads[processing_threads_num];
        for (auto i = 0; i < processing_threads_num; ++i)
        {
            processing_threads[i] = std::async(std::launch::async, process_words, q_ptrs[i].lock());
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

    unsigned int UniqueWords::get_threads_num()
    {
        auto threads_num = std::thread::hardware_concurrency();

        if (threads_num < MIN_THREADS)
            threads_num = DEFAULT_THREADS;
        else if (threads_num > MAX_THREADS)
            threads_num = MAX_THREADS;

        return threads_num;
    }

    std::unique_ptr<RangeQueues> UniqueWords::make_range_queues()
    {
        auto range_queues = std::make_unique<RangeQueues>();

        const float queue_weight_limit = TOTAL_WEIGHT / processing_threads_num;

        // Initial queue
        auto queue = std::make_shared<Queue>();
        unsigned int queue_idx = 0;
        q_ptrs[queue_idx] = queue;
        float current_weight = 0;

        for (const auto &pair : LetterFrequency)
        {
            range_queues->emplace(pair.first, queue);
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

    bool UniqueWords::read_words(const std::string &filename, std::unique_ptr<RangeQueues> &range_queues, unsigned int processing_threads_num)
    {
        end_of_input = false;
        ready_to_process = false;

        {
            std::unique_lock<std::mutex> l(m);
            cv_input_ready.wait(l, [&processing_threads_num]
                                { return is_all_ready(processing_threads_num); });
        }

        std::ifstream file(filename);

        if (!file.is_open())
        {
            end_of_input = true;
            return false;
        }

        unsigned int word_count = 0;
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string word;
            while (iss >> word)
            {
                ++word_count;
                range_queues->at(word[0])->push(word);

                if (word_count == WORD_SEGMENT)
                {
                    word_count = 0;

                    {
                        ready_to_process = true;
                        threads_ready = 0;

                        cv_process_ready.notify_all();
                        std::unique_lock<std::mutex> l(m);
                        cv_input_ready.wait(l, [&processing_threads_num]
                                            { return is_all_ready(processing_threads_num); });

                        ready_to_process = false;
                    }
                }
            }
        }

        file.close();
        end_of_input = true;
        ready_to_process = true;

        cv_process_ready.notify_all();

        return true;
    }

    int UniqueWords::process_words(std::shared_ptr<Queue> queue)
    {
        int count = 0;
        std::unordered_set<std::string> unique_words;

        process_ready();
        cv_input_ready.notify_one();

        while (true)
        {
            { // wait
                std::unique_lock<std::mutex> l(m);
                cv_process_ready.wait(l, []()
                                      { return ready_to_process; });
            }

            while (!queue->empty())
            {
                count += unique_words.emplace(queue->front()).second;
                queue->pop();
            }

            if (end_of_input)
                break;

            process_ready();
            cv_input_ready.notify_one();
        }

        return count;
    }

    void UniqueWords::process_ready()
    {
        ++threads_ready;
    }

    bool UniqueWords::is_all_ready(unsigned int processing_threads_num)
    {
        return threads_ready >= processing_threads_num;
    }

    void test_queue_distribution(unsigned int processing_threads_num, std::unique_ptr<RangeQueues> &range_queues)
    {
        if (processing_threads_num > MAX_PROCESSING_THREADS)
            processing_threads_num = MAX_PROCESSING_THREADS;

        char end = 'z' + 1;
        for (char c = 'a'; c != end; ++c)
        {
            std::string s;
            s += c;
            range_queues->at(c)->push(s);
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