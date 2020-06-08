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
         * @brief Remove the task with matching ID (does nothing if invalid ID)
         * @note Called internally - you shouldn't need to call it yourself!
         *
         * @param id id of task to remove
         */
        void removeTaskWithID(unsigned int id);

        /**
         * @brief Queue a task to be executed on another thread when available
         * @note Called internally - you shouldn't need to call it yourself!
         *
         * @param f function to queue
         *
         * @return id assigned to task
         */
        unsigned int addTask(std::function<void()> f);

        /**
         * @brief Block the calling thread until tasks are finished
         * @note Called internally - you shouldn't need to call it yourself!
         */
        void process();
    };
};

#endif