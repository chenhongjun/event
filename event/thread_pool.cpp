#include "thread_pool.h"

namespace Event
{
ThreadPool::ThreadPool(std::uint32_t thread_num)
{
    is_run_.store(true);
    for (std::uint32_t i = 0; i != thread_num; ++i)
        threads_.emplace_back(&ThreadPool::proc, this);
}

ThreadPool::~ThreadPool()
{
    is_run_.store(false);
    cv_.notify_all(); // poke all proc thread
    for (std::uint32_t i = 0; i != threads_.size(); ++i)
    {
        auto&& item = threads_.at(i);
        if (item.joinable())
            item.join();
    }
    threads_.clear();
    if (!task_queue_.empty())
    {
        printf("Warning, ThreadPool have %lu unfinished tasks when destruct.", task_queue_.size());
        task_queue_.clear();
    }
}

void ThreadPool::post_task(const std::function<void()>& task)
{
    std::lock_guard<std::mutex> lock(mutex_);
    bool notify_flag = task_queue_.empty();
    task_queue_.push_back(task);
    if (notify_flag)
    {
        cv_.notify_one();
    }
}

void ThreadPool::proc()
{
    while (is_run_.load())
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (task_queue_.empty())
        {
            cv_.wait(lock,[this]() {
                return !task_queue_.empty() || !is_run_.load(); });
        }
        if (!is_run_.load())
            break;
        std::function<void()> func;
        func = task_queue_.front();
        task_queue_.pop_front();
        lock.unlock();
        func();
    }
}
} // namespace Event
