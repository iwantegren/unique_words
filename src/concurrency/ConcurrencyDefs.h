#pragma once

#include <unordered_map>
#include <queue>
#include <string>
#include <memory>
#include <mutex>

namespace Concurrency
{
    constexpr unsigned int WORD_SEGMENT = 1000'000;

    constexpr unsigned int MIN_THREADS = 2;

    // Make too many threads doesn't make better performance
    // because total weight of queue can be less than weight of one single letter
    // and distribution becomes too uneven
    constexpr unsigned int MAX_THREADS = 8;
    constexpr unsigned int MAX_PROCESSING_THREADS = MAX_THREADS - 1;

    // This is the optimal number of processing threads
    // that gives pretty even disttribution of queues' weights
    constexpr unsigned int DEFAULT_THREADS = 7;
    constexpr unsigned int DEFAULT_PROCESSING_THREADS = DEFAULT_THREADS - 1;

    using Queue = std::queue<std::string>;
    using RangeQueues = std::unordered_map<char, std::shared_ptr<Queue>>;

    constexpr float TOTAL_WEIGHT = 99.99;
    static const std::unordered_map<char, float>
        LetterFrequency = {
            {
                'a',
                8.12,
            },
            {
                'b',
                1.49,
            },
            {
                'c',
                2.71,
            },
            {
                'd',
                4.32,
            },
            {
                'e',
                12.02,
            },
            {
                'f',
                2.30,
            },
            {
                'g',
                2.03,
            },
            {
                'h',
                5.92,
            },
            {
                'i',
                7.31,
            },
            {
                'j',
                0.10,
            },
            {
                'k',
                0.69,
            },
            {
                'l',
                3.98,
            },
            {
                'm',
                2.61,
            },
            {
                'n',
                6.95,
            },
            {
                'o',
                7.68,
            },
            {
                'p',
                1.82,
            },
            {
                'q',
                0.11,
            },
            {
                'r',
                6.02,
            },
            {
                's',
                6.28,
            },
            {
                't',
                9.10,
            },
            {
                'u',
                2.88,
            },
            {
                'v',
                1.11,
            },
            {
                'w',
                2.09,
            },
            {
                'x',
                0.17,
            },
            {
                'y',
                2.11,
            },
            {
                'z',
                0.07,
            },
    };
}