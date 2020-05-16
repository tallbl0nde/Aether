#include <future>
#include <mutex>
#include <queue>
#include "ThreadPool.hpp"
#include <vector>

// Maximum number of threads running at once
static unsigned int maxThreads = 2;
// Queue of functions to run
static std::queue< std::function<void()> > queue;
// Vector of currently running threads (need future to remain in scope)
static std::vector< std::future<void> > threads;
// Mutexes for protecting queue + thread vector
static std::mutex qMutex;
static std::mutex tMutex;

namespace Aether::ThreadPool {
    void setMaxThreads(unsigned int t) {
        std::scoped_lock<std::mutex> mtx(qMutex);
        maxThreads = t;
    }

    void removeQueuedTasks() {
        std::scoped_lock<std::mutex> mtx(qMutex);
        while (!queue.empty()) {
            queue.pop();
        }
    }

    void waitUntilDone() {
        std::scoped_lock<std::mutex> mtx(tMutex);
        for (size_t i = 0; i < threads.size(); i++) {
            if (threads[i].valid()) {
                threads[i].get();
            }
        }
        threads.clear();
    }

    void addTask(std::function<void()> f) {
        std::scoped_lock<std::mutex> mtx(qMutex);
        queue.push(f);
    }

    void process() {
        // First check which threads are done + remove
        std::scoped_lock<std::mutex> tMtx(tMutex);
        int i = 0;
        while (i < threads.size()) {
            if (threads[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                threads.erase(threads.begin() + i);
            } else {
                i++;
            }
        }

        // Start new tasks
        std::scoped_lock<std::mutex> qMtx(qMutex);
        while (threads.size() < maxThreads) {
            if (queue.empty()) {
                break;
            }

            threads.emplace_back(std::async(std::launch::async, queue.front()));
            queue.pop();
        }
    }
};