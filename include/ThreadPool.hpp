#ifndef AETHER_THREAD_POOL_HPP
#define AETHER_THREAD_POOL_HPP

#include <functional>

namespace Aether {
    /**
     * @brief A Thread Pool implementation which processes queued tasks on other threads.
     * It is designed for multi-threaded texture generation - please do not use it for your own tasks!
     */
    namespace ThreadPool {
        /**
         * @brief Set the maximum number of threads to use for tasks (default of 2)
         * It is safe to call this at anytime.
         *
         * @param t maximum number of threads
         */
        void setMaxThreads(unsigned int t);

        /**
         * @brief Remove any remaining queued tasks (called internally upon exit)
         * @note This does not wait for running tasks to finish, call \ref waitUntilDone() for that!
         */
        void removeQueuedTasks();

        /**
         * @brief Block the calling thread until tasks are finished (called internally upon exit)
         * @note This does not clear the queue - please call \ref removeQueueTasks() first!
         */
        void waitUntilDone();

        /**
         * @brief Queue a task to be executed on another thread when available
         * @note Called internally - you shouldn't need to call it yourself!
         *
         * @param f function to queue
         */
        void addTask(std::function<void()> f);

        /**
         * @brief Block the calling thread until tasks are finished
         * @note Called internally - you shouldn't need to call it yourself!
         */
        void process();
    };
};

#endif