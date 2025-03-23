#pragma once
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
class ThreadPool
{
public:
    // // Constructor to creates a thread pool with given
    // number of threads
    ThreadPool(size_t num_threads = std::thread::hardware_concurrency());

    // Destructor to stop the thread pool
    ~ThreadPool();

    // Enqueue task for execution by the thread pool
    void enqueue(std::function<void()> task);

    bool isEmpty();

    void StopAll();
private:
    uint32_t amountOfThreads;
    uint32_t freeThreads;

    std::mutex amountFinishedMutex;

    // Vector to store worker threads
    std::vector<std::thread> threads_;

    // Queue of tasks
    std::queue<std::function<void()> > tasks_;

    // Mutex to synchronize access to shared data
    std::mutex queue_mutex_;

    // Condition variable to signal changes in the state of
    // the tasks queue
    std::condition_variable cv_;

    // Flag to indicate whether the thread pool should stop
    // or not
    bool stop_ = false;

};

