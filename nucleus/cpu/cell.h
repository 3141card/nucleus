/**
 * (c) 2014 Nucleus project. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/common.h"
#include "nucleus/cpu/thread.h"

#include <mutex>
#include <vector>
#include <set>

enum CellThreadType
{
    CELL_THREAD_PPU,
    CELL_THREAD_SPU,
    CELL_THREAD_RAWSPU,
};

class Cell
{
    std::vector<CellThread*> m_threads;
    std::mutex m_mutex;

    // Thread ID information
    std::set<u64> m_thread_ids;
    u64 m_current_id = 1;

public:
    // Count and iterate through threads
    size_t size() { return m_threads.size(); }
    std::vector<CellThread*>::const_iterator begin() { return m_threads.begin(); }
    std::vector<CellThread*>::const_iterator end() { return m_threads.end(); }

    // Thread management
    CellThread* addThread(CellThreadType type, u32 entry);
    CellThread* getThread(u64 id);
    void removeThread(u64 id);

    // Current thread ID
    CellThread* getCurrentThread();
    void setCurrentThread(CellThread* thread);

    // Control CPU state
    void run();
    void pause();
    void stop();
};
