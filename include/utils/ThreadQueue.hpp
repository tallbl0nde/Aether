#ifndef AETHER_THREAD_POOL_HPP
#define AETHER_THREAD_POOL_HPP

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace Aether {
    class ThreadQueue {
        private:
            // Mutex protecting queue
            std::mutex mutex;
            // Max number of threads
            unsigned int num;
            // Queue of functions to execute
            std::queue< std::function<void()> > queue;
            // Vector of threads
            std::vector< std::future<void> > threads;

        public:
            // Takes max number of threads
            ThreadQueue(unsigned int = std::thread::hardware_concurrency());

            // Add function to queue
            void addTask(std::function<void()>);
            // Remove all queued tasks (but not running tasks!)
            void removeQueuedTasks();

            // Checks if threads are done + continues work on queue
            void processQueue();

            // Dismisses queued tasks and waits for currently executing ones to finish
            ~ThreadQueue();
    };
};

#endif