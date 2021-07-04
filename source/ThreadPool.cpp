#include "Aether/ThreadPool.hpp"
#include "Aether/ThreadPool.Job.hpp"

// Start with no instance
namespace Aether {
    ThreadPool * ThreadPool::instance = nullptr;

    ThreadPool::ThreadPool() {
        this->nextID = 1;
        this->terminating = false;

        // Find a suitable number of threads to use
        unsigned int threadCount = std::thread::hardware_concurrency();
        if (threadCount == 0) {
            // Use 4 threads if we're on a Switch, otherwise default to 1
            #ifdef __SWITCH__
                threadCount = 4;
            #else
                threadCount = 1;
            #endif
        }

        // Start our worker threads
        for (size_t idx = 0; idx < threadCount; idx++) {
            this->workers.push_back(std::thread(&ThreadPool::doWork, this, idx));
        }
        this->workerMetadata = std::vector<WorkerMetadata>(threadCount);
    }

    void ThreadPool::doWork(size_t idx) {
        while (!this->terminating) {
            Job * job = nullptr;

            {
                // Wait for a job on the queue, or the threadpool to terminate
                std::unique_lock<std::mutex> mtx(this->jobsMutex);
                this->workerConditionVariable.wait(mtx, [this]() {
                    return (!this->jobs.empty() || this->terminating);
                });

                // If terminating don't try to do work
                if (this->terminating) {
                    continue;
                }

                {
                    // Get the job's ID and associate with thread
                    WorkerMetadata & meta = this->workerMetadata[idx];
                    std::unique_lock<std::mutex> mtx2(meta.mutex);
                    meta.jobID = jobs.front().second;
                }
                job = jobs.front().first;
                jobs.pop_front();
            }

            // Run the job and delete the object
            job->run();
            delete job;

            // Notify job was completed
            {
                WorkerMetadata & meta = this->workerMetadata[idx];
                std::unique_lock<std::mutex> mtx(meta.mutex);
                meta.jobID = 0;
                meta.conditionVariable.notify_all();
            }
        }
    }

    ThreadPool::~ThreadPool() {
        // Notify that threads should terminate
        this->terminating = true;
        this->workerConditionVariable.notify_all();

        // Join all threads
        for (std::thread & thread : this->workers) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        this->workers.clear();
    }

    ThreadPool * ThreadPool::getInstance() {
        if (ThreadPool::instance == nullptr) {
            ThreadPool::instance = new ThreadPool();
        }

        return ThreadPool::instance;
    }

    size_t ThreadPool::maxConcurrentJobs() {
        return this->workers.size();
    }

    int ThreadPool::queueJob(Job * job, const Importance importance) {
        // Lock access to the queue and add job
        std::scoped_lock<std::mutex> mtx(this->jobsMutex);
        int jobID = this->nextID++;
        switch (importance) {
            case Importance::Normal:
                this->jobs.push_back(std::pair<Job *, int>(job, jobID));
                break;

            case Importance::High:
                this->jobs.push_front(std::pair<Job *, int>(job, jobID));
                break;
        }

        // Make sure the next allocated ID will not be zero, as zero is
        // used for specific signals
        if (this->nextID == 0) {
            this->nextID++;
        }

        // Notify one worker that a job is available
        this->workerConditionVariable.notify_one();
        return jobID;
    }

    void ThreadPool::removeOrWaitForJob(int id) {
        // Check if job is on queue
        std::scoped_lock<std::mutex> mtx(this->jobsMutex);
        for (size_t i = 0; i < this->jobs.size(); i++) {
            if (this->jobs[i].second == id) {
                delete this->jobs[i].first;
                this->jobs.erase(this->jobs.begin() + i);
                return;
            }
        }

        // Otherwise check if currently running
        for (size_t i = 0; i < this->workerMetadata.size(); i++) {
            WorkerMetadata & meta = this->workerMetadata[i];
            std::unique_lock<std::mutex> mtx(meta.mutex);

            // Skip if not the matching ID
            if (meta.jobID != id) {
                continue;
            }

            // Wait for thread to finish
            meta.conditionVariable.wait(mtx, [&meta]() {
                return meta.jobID == 0;
            });
            return;
        }
    }
}
