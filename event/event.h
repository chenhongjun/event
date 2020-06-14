#ifndef EVENT_EVENT_H_
#define EVENT_EVENT_H_
#include <cassert>
#include <string>
#include <tuple>
#include <list>
#include <map>
#include <memory>
#include <algorithm>
#include <mutex>
#include "thread_pool.h"

namespace Event
{
class Event;
template<typename MSGTYPE>
class Observer
{
    friend class Event;
    using ObserverType = Observer<MSGTYPE>;
    using ObserverPtrWeak = std::weak_ptr<ObserverType>;
    using ObserverList = std::list<ObserverPtrWeak>;
    using Mutex = std::mutex;
    using MutexPtrUnique = std::unique_ptr<std::mutex>;
    using EventObserverList = std::tuple<ObserverList, MutexPtrUnique>;
    using Complate = std::owner_less<std::weak_ptr<Event>>;
public:
    virtual void update(const MSGTYPE& message) {};
    virtual void async_update(const std::shared_ptr<MSGTYPE>& message) {};
private:
    static std::map<std::weak_ptr<Event>, EventObserverList, Complate> event_observer_list_map_;
    static std::mutex map_mutex_;
};
template<typename MSGTYPE>
std::map<std::weak_ptr<Event>,\
    typename Observer<MSGTYPE>::EventObserverList,\
    typename Observer<MSGTYPE>::Complate> Observer<MSGTYPE>::event_observer_list_map_;
template<typename MSGTYPE>
std::mutex Observer<MSGTYPE>::map_mutex_;

class Event : public std::enable_shared_from_this<Event>
{
public:
    Event(std::uint32_t async_thread_num = 2) :async_thread_pool_(async_thread_num) {}
    ~Event() = default;
    template<typename MSGTYPE>
    void regist(const std::shared_ptr<Observer<MSGTYPE>>& observer)
    {
        auto this_ptr = get_this_ptr<MSGTYPE>();
        {
            std::unique_lock<std::mutex> map_lock(Observer<MSGTYPE>::map_mutex_);
            auto& observer_list = Observer<MSGTYPE>::event_observer_list_map_[this_ptr];
            map_lock.unlock();
            std::unique_lock<std::mutex> list_lock(*std::get<1>(observer_list));
            std::get<0>(observer_list).push_back(observer);
        }
    }

    template<typename MSGTYPE>
    void unregist(const std::shared_ptr<Observer<MSGTYPE>>& observer)
    {
        auto this_ptr = get_this_ptr<MSGTYPE>();
        {
            std::unique_lock<std::mutex> map_lock(Observer<MSGTYPE>::map_mutex_);
            auto& observer_list = Observer<MSGTYPE>::event_observer_list_map_[this_ptr];
            map_lock.unlock();
            std::unique_lock<std::mutex> list_lock(*std::get<1>(observer_list));
            std::remove_if(std::get<0>(observer_list).begin(),
                std::get<0>(observer_list).end(),
                [&observer](const auto& item) {
                    return observer == item.lock();
                });
            if (std::get<0>(observer_list).empty())
            {
                map_lock.lock();
                Observer<MSGTYPE>::event_observer_list_map_.erase(this_ptr);
            }
        }
    }

    template<typename MSGTYPE>
    void post(const MSGTYPE& message)
    {
        auto this_ptr = get_this_ptr<MSGTYPE>();
        {
            std::unique_lock<std::mutex> map_lock(Observer<MSGTYPE>::map_mutex_);
            auto& observer_list = Observer<MSGTYPE>::event_observer_list_map_[this_ptr];
            map_lock.unlock();
            std::unique_lock<std::mutex> list_lock(*std::get<1>(observer_list));
            typename Observer<MSGTYPE>::ObserverList buffer = std::get<0>(observer_list);
            list_lock.unlock();
            std::for_each(buffer.begin(),
                buffer.end(),
                [&message](const auto& item) {
                    auto observer = item.lock();
                    if (observer != nullptr)
                        observer->update(message);
                });
        }
    }

    template<typename MSGTYPE>
    void async_post(const std::shared_ptr<MSGTYPE>& message)
    {

        auto this_ptr = get_this_ptr<MSGTYPE>();
        {
            std::unique_lock<std::mutex> map_lock(Observer<MSGTYPE>::map_mutex_);
            auto& observer_list = Observer<MSGTYPE>::event_observer_list_map_[this_ptr];
            map_lock.unlock();
            std::unique_lock<std::mutex> list_lock(*std::get<1>(observer_list));
            typename Observer<MSGTYPE>::ObserverList buffer = std::get<0>(observer_list);
            list_lock.unlock();
            std::for_each(buffer.begin(),
                buffer.end(),
                [this, &message](const auto& item) {
                    auto observer = item.lock();
                    if (observer != nullptr)
                    {
                        async_thread_pool_.post_task([observer, message]() {
                            observer->async_update(message);
                            });
                    }
                });
        }
    }
private:
    template<typename MSGTYPE>
    std::shared_ptr<Event> get_this_ptr()
    {
        auto this_ptr = shared_from_this();
        assert(this_ptr);
        std::lock_guard<std::mutex> lock(Observer<MSGTYPE>::map_mutex_);
        if (Observer<MSGTYPE>::event_observer_list_map_.find(this_ptr) == Observer<MSGTYPE>::event_observer_list_map_.end())
        {
            auto this_ptr_weak = weak_from_this();
            typename Observer<MSGTYPE>::MutexPtrUnique mutex_ptr(new typename Observer<MSGTYPE>::Mutex);
            Observer<MSGTYPE>::event_observer_list_map_[this_ptr_weak] = std::make_tuple(typename Observer<MSGTYPE>::ObserverList(), std::move(mutex_ptr));
        }
        return this_ptr;
    }
    ThreadPool async_thread_pool_;
}; // class Event
} // namespace Event
#endif // EVENT_EVENT_H_
