#pragma once

#include <mutex>
#include <atomic>
#include "ConcurrencyDefs.h"

namespace Concurrency
{
    static std::mutex q_mutexes[MAX_THREADS];
    static std::atomic_bool end_of_input;
}
