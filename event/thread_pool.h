#ifndef EVENT_THREAD_POOL_H_
#define EVENT_THREAD_POOL_H_
#include <vector>
#include <deque>
#include <functional>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
namespace Event
{
class ThreadPool
{
public:
	ThreadPool(std::uint32_t thread_num);
	~ThreadPool();
private:
	void proc();
	std::atomic_bool is_run_ = false;
	std::vector<std::thread> threads_;
	std::deque<std::function<void()>> task_queue_;
	std::mutex mutex_;
	std::condition_variable cv_;
}; // class ThreadPool
} // namespace Event
#endif // EVENT_THREAD_POOL_H_