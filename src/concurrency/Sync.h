#pragma once

#include <mutex>
#include <atomic>
#include <memory>
#include "ConcurrencyDefs.h"

namespace Concurrency
{
    static std::mutex q_mutexes[MAX_PROCESSING_THREADS];
    static std::weak_ptr<Queue> q_ptrs[MAX_PROCESSING_THREADS];
    static std::atomic_bool end_of_input;
}
