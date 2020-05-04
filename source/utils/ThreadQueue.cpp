#include "ThreadQueue.hpp"

namespace Aether {
    ThreadQueue::ThreadQueue(unsigned int threads) {
        this->num = threads;
    }

    void ThreadQueue::addTask(std::function<void()> f) {
        std::lock_guard<std::mutex> mtx(this->qMutex);
        this->queue.push(f);
    }

    void ThreadQueue::waitUntilDone() {
        std::lock_guard<std::mutex> mtx(this->tMutex);
        for (size_t i = 0; i < this->threads.size(); i++) {
            if (this->threads[i].valid()) {
                this->threads[i].get();
            }
        }
        this->threads.clear();
    }

    void ThreadQueue::removeQueuedTasks() {
        std::lock_guard<std::mutex> mtx(this->qMutex);
        while (!this->queue.empty()) {
            this->queue.pop();
        }
    }

    void ThreadQueue::processQueue() {
        // First check which threads are done + remove
        std::lock_guard<std::mutex> tMtx(this->tMutex);
        int i = 0;
        while (i < this->threads.size()) {
            if (this->threads[i].wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                this->threads.erase(this->threads.begin() + i);
            } else {
                i++;
            }
        }

        // Start new tasks
        std::lock_guard<std::mutex> qMtx(this->qMutex);
        while (this->threads.size() < this->num) {
            if (this->queue.empty()) {
                break;
            }

            this->threads.emplace_back(std::async(std::launch::async, this->queue.front()));
            this->queue.pop();
        }
    }

    ThreadQueue::~ThreadQueue() {
        this->removeQueuedTasks();
        this->waitUntilDone();
    }
};