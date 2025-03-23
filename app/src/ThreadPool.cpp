#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t num_threads)
{
    amountOfThreads = num_threads;
    std::cout << "Using " << num_threads << " threads.";

    freeThreads = amountOfThreads;
    // Creating worker threads
    if (num_threads > 12) {
        for (size_t i = 0; i < num_threads; ++i) {
            threads_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    // The reason for putting the below code
                    // here is to unlock the queue before
                    // executing the task so that other
                    // threads can perform enqueue tasks
                    {
                        // Locking the queue so that data
                        // can be shared safely
                        std::unique_lock<std::mutex> lock(
                            queue_mutex_);

                        // Waiting until there is a task to
                        // execute or the pool is stopped
                        cv_.wait(lock, [this] {
                            return !tasks_.empty() || stop_;
                            });

                        // exit the thread in case the pool
                        // is stopped and there are no tasks
                        if (stop_ && tasks_.empty()) {
                            return;
                        }

                        // Get the next task from the queue
                        task = move(tasks_.front());
                        tasks_.pop();

                        std::lock_guard<std::mutex> freeThreadsLock(amountFinishedMutex);
                        freeThreads--;
                    }
                    task();

                    {
                        std::lock_guard<std::mutex> freeThreadsLock(amountFinishedMutex);

                        freeThreads++;
                    }
                }
                });
        }
    }
    else {
        freeThreads = 0;
        amountOfThreads = 0;
        std::cout << "sike. Using just main thread (your cpu is too slow :D)\n";
    }
}

ThreadPool::~ThreadPool()
{
    if (amountOfThreads == 0)
        return;

    {
        // Lock the queue to update the stop flag safely
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }

    // Notify all threads
    cv_.notify_all();

    // Joining all worker threads to ensure they have
    // completed their tasks
    for (auto& thread : threads_) {
        thread.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task)
{
    if (amountOfThreads == 0) {
        task();
        return;
    }

    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        tasks_.emplace(move(task));
    }
    cv_.notify_one();
}

bool ThreadPool::isEmpty()
{
    std::lock_guard<std::mutex> freeThreadsLock(amountFinishedMutex);
    //std::cout << freeThreads << ' ';
    return amountOfThreads == freeThreads;
}

void ThreadPool::StopAll()
{
    if (amountOfThreads == 0)
        return;
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        while (!tasks_.empty())
            tasks_.pop();
    }
}
