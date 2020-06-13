#ifndef EVENT_EVENT_H_
#define EVENT_EVENT_H_
#include <string>
#include <list>
#include <map>
#include <memory>
#include <algorithm>
#include <mutex>

template<typename MSGTYPE>
class Observer
{
	friend class EventBus;
public:
	virtual void update(const std::shared_ptr<MSGTYPE>& message) = 0;
private:
	static std::mutex mutex_;
	static std::list<std::weak_ptr<Observer<MSGTYPE>>> observer_list_;
};
#include "event.inl"

class EventBus
{
public:
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
					observer->update(message);
			});
	}
};
#endif // EVENT_EVENT_H_