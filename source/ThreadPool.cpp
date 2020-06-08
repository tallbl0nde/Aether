#include <future>
#include <mutex>
#include "Aether/ThreadPool.hpp"
#include <vector>

// Some nice typedefs
typedef struct {
    std::function<void()> func;
    unsigned int id;
} queuePair;

typedef struct {
    std::future<void> future;
    unsigned int id;
} threadPair;

// Maximum number of threads running at once
static unsigned int maxThreads = 2;
// Next ID to assign to task
static unsigned int nextID = 0;
// Queue of functions to run
static std::vector<queuePair> queue;
// Vector of currently running threads
static std::vector<threadPair> threads;
// Mutexes for protecting queue + thread vector
static std::mutex qMutex;
static std::mutex tMutex;

namespace Aether::ThreadPool {
    void setMaxThreads(unsigned int t) {
        std::scoped_lock<std::mutex> mtx(qMutex);
        maxThreads = t;
    }

    void removeTaskWithID(unsigned int id) {
        // Check if executing
        std::unique_lock<std::mutex> mtx(tMutex);
        for (size_t i = 0; i < threads.size(); i++) {
            // If the ID matches wait until it's done
            if (threads[i].id == id) {
                threads[i].future.get();
                threads.erase(threads.begin() + i);
                return;
            }
        }
        mtx.unlock();

        // Otherwise check if it's queued
        std::scoped_lock<std::mutex> mtx2(qMutex);
        std::vector<queuePair>::iterator it = std::find_if(queue.begin(), queue.end(), [id](const queuePair & val) {
            return val.id == id;
        });
        if (it != queue.end()) {
            queue.erase(it);
        }
    }

    unsigned int addTask(std::function<void()> f) {
        std::scoped_lock<std::mutex> mtx(qMutex);
        queue.push_back(queuePair{f, nextID});
        return nextID++;
    }

    void process() {
        // First check which threads are done + remove
        std::scoped_lock<std::mutex> tMtx(tMutex);
        int i = 0;
        while (i < threads.size()) {
            if (threads[i].future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
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

            threads.emplace_back(threadPair{std::async(std::launch::async, queue[0].func), queue[0].id});
            queue.erase(queue.begin());
        }
    }
};