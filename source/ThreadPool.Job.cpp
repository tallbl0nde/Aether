#include "Aether/ThreadPool.Job.hpp"

namespace Aether {
    ThreadPool::Job::Job() {
        this->completed = false;
    }

    bool ThreadPool::Job::run() {
        if (this->completed) {
            return false;
        }

        this->work();
        this->completed = true;
        return true;
    }

    ThreadPool::Job::~Job() {

    }
}