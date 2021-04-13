#ifndef AETHER_THREADPOOL_JOB_HPP
#define AETHER_THREADPOOL_JOB_HPP

#include "Aether/ThreadPool.hpp"

namespace Aether {
    /**
     * @brief Abstract class representing a job for the ThreadPool. It is designed to be
     * inherited to define the job.
     */
    class ThreadPool::Job {
        private:
            /**
             * @brief Flag indicating if the job has already been completed.
             */
            bool completed;

        protected:
            /**
             * @brief The work to perform. This must be defined by derived classes
             * to actually perform the work. Invoked by \ref run() if needed.
             *
             * @note This will only ever be executed once per job object.
             */
            virtual void work() = 0;

        public:
            /**
             * @brief Constructs a new default Job object.
             */
            Job();

            /**
             * @brief Runs the job. Does nothing if the job has already
             * been completed.
             *
             * @return true if the job was completed, false if it was previously run.
             */
            bool run();

            /**
             * @brief Destroys the Job object.
             */
            ~Job();
    };
};

#endif