#pragma once

#include <mutex>
#include <atomic>
#include <memory>
#include "ConcurrencyDefs.h"

namespace Concurrency
{
    static std::weak_ptr<Queue> q_ptrs[MAX_PROCESSING_THREADS];

    static std::mutex m;
    static std::atomic_bool end_of_input;

    static std::condition_variable cv_input_ready;
    static bool ready_to_process;

    static std::condition_variable cv_process_ready;
    static std::atomic_int threads_ready;
}
