#ifndef AETHER_THREADPOOL_HPP
#define AETHER_THREADPOOL_HPP

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

// Forward declare class
namespace Aether {
    class Window;
};

namespace Aether {
    /**
     * @brief Basic Thread Pool implemented as a singleton.
     * @note This is primarily used for texture generation, therefore use for your own tasks
     * is not guaranteed to be a good idea!
     */
    class ThreadPool {
        // Allow the Window class access to the constructor and destructor
        friend Window;

        public:
            // Forward declare nested class
            class Job;

            /**
             * @brief Enumeration of job importance levels.
             */
            enum class Importance {
                Normal,     //< Normal importance; queue at end
                High        //< High importance; queue at start
            };

        private:
            /**
             * @brief Struct grouping all needed data about a thread.
             */
            struct WorkerMetadata {
                std::condition_variable conditionVariable;      /** @brief Condition variable used to notify of changes */
                std::mutex mutex;                               /** @brief Mutex used along with condition variable */
                int jobID;                                      /** @brief ID of job being executed (0 if none) */

                WorkerMetadata() {
                    this->jobID = 0;
                }
            };

            static ThreadPool * instance;                       /** @brief Single instance of the ThreadPool object. */
            int nextID;                                         /** @brief The id to assign to the next job (note: 0 is not valid) */
            bool terminating;                                   /** @brief Flag indicating when the thread pool is terminating. */

            std::mutex jobsMutex;                               /** @brief Mutex to protect the queue of jobs. */
            std::deque<std::pair<Job *, int>> jobs;             /** @brief Queue of jobs to perform. */

            std::condition_variable workerConditionVariable;    /** @brief Condition variable used to make workers wait for a job. */
            std::vector<WorkerMetadata> workerMetadata;         /** @brief Vector of metadata for each worker thread. */
            std::vector<std::thread> workers;                   /** @brief Vector of worker threads. */

            /**
             * @brief Constructs a new ThreadPool.
             */
            ThreadPool();

            /**
             * @brief Method run by each worker thread to wait for a job and then run it.
             *
             * @param idx Index of worker thread in vector
             */
            void doWork(size_t idx);

            /**
             * @brief Destroys the Thread Pool object, stopping all running tasks
             * and clearing the queue.
             */
            ~ThreadPool();

        public:
            /**
             * @brief Prevent copying.
             */
            ThreadPool(ThreadPool &) = delete;

            /**
             * @brief Prevent assigning.
             */
            void operator=(const ThreadPool &) = delete;

            /**
             * @brief Returns the instance of the ThreadPool. One is created
             * if it does not already exist.
             *
             * @return ThreadPool instance
             */
            static ThreadPool * getInstance();

            /**
             * @brief Returns the maximum number of jobs that can be executed concurrently.
             *
             * @return Maximum number of concurrent jobs (threads)
             */
            size_t maxConcurrentJobs();

            /**
             * @brief Adds a job to the queue. The job object is deleted once it is complete.
             *
             * @param job Job to queue
             * @param importance The importance of the job (see \ref Importance)
             * @return ID of job
             */
            int queueJob(Job * job, const Importance importance);

            /**
             * @brief Removes the requested job from the queue, or waits until the
             * job is completed if already running. Does nothing if the id is not found.
             *
             * @param id ID of job to wait for
             */
            void removeOrWaitForJob(int id);
    };
};

#endif