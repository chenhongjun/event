#ifndef EVENT_EVENT_H_
#define EVENT_EVENT_H_
#include <string>
#include <list>
#include <map>
#include <memory>
#include <algorithm>
#include <mutex>
#include "thread_pool.h"

namespace Event
{
template<typename MSGTYPE>
class Observer
{
    friend class Event;
public:
    virtual void update(const std::shared_ptr<MSGTYPE>& message) = 0;
private:
    static std::mutex mutex_;
    static std::list<std::weak_ptr<Observer<MSGTYPE>>> observer_list_;
};
#include "event.inl"

class Event
{
public:
    Event(std::uint32_t async_thread_num = 2):async_thread_pool_(async_thread_num) {}
    ~Event() =default;
    template<typename MSGTYPE>
    void regist(const std::shared_ptr<Observer<MSGTYPE>>& observer)
    {
        std::lock_guard<std::mutex> lock(Observer<MSGTYPE>::mutex_);
        Observer<MSGTYPE>::observer_list_.push_back(observer);
    }

    template<typename MSGTYPE>
    void unregist(const std::shared_ptr<Observer<MSGTYPE>>& observer)
    {
        std::lock_guard<std::mutex> lock(Observer<MSGTYPE>::mutex_);
        auto iter = std::remove_if(Observer<MSGTYPE>::observer_list_.begin(),
            Observer<MSGTYPE>::observer_list_.end(),
            [&observer](const auto& item) {
                return observer == item.lock();
            });
    }

    template<typename MSGTYPE>
    void post(const std::shared_ptr<MSGTYPE>& message)
    {
        std::list<std::weak_ptr<Observer<MSGTYPE>>> buffer;
        {
            std::lock_guard<std::mutex> lock(Observer<MSGTYPE>::mutex_);
            buffer = Observer<MSGTYPE>::observer_list_;
        }
        std::for_each(buffer.begin(),
            buffer.end(),
            [&message](const auto& item) {
                auto observer = item.lock();
                if (observer != nullptr)
                    observer->update(message);
            });
    }

    template<typename MSGTYPE>
    void async_post(const std::shared_ptr<MSGTYPE>& message)
    {
        std::list<std::weak_ptr<Observer<MSGTYPE>>> buffer;
        {
            std::lock_guard<std::mutex> lock(Observer<MSGTYPE>::mutex_);
            buffer = Observer<MSGTYPE>::observer_list_;
        }
        std::for_each(buffer.begin(),
            buffer.end(),
            [&message](const auto& item) {
                auto observer = item.lock();
                if (observer != nullptr)
                {
                    async_thread_pool_.post_task([observer]() {
                        observer->update(message);
                    });
                }
            });
    }
private:
    ThreadPool async_thread_pool_;
}; // class Event
} // namespace Event
#endif // EVENT_EVENT_H_