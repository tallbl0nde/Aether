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
            std::mutex qMutex;
            // Mutex protecting thread vector
            std::mutex tMutex;
            // Max number of threads
            unsigned int num;
            // Queue of functions to execute
            std::queue< std::function<void()> > queue;
            // Vector of threads
            std::vector< std::future<void> > threads;

        public:
            // Takes max number of threads
            ThreadQueue(unsigned int = 2);

            // Add function to queue
            void addTask(std::function<void()>);
            // Remove all queued tasks (but not running tasks!)
            void removeQueuedTasks();
            // Blocks until all tasks are finished (DO NOT CALL WITHOUT CALLING removeQueuedTasks()!)
            void waitUntilDone();

            // Checks if threads are done + continues work on queue
            void processQueue();

            // Dismisses queued tasks and waits for currently executing ones to finish
            ~ThreadQueue();
    };
};

#endif