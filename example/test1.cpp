#include <memory>
#include <map>
#include <string>
#include <list>
#include <algorithm>
#include "../event/event.h"
#include "../example/test2.h"

class Observer1 : public Event::Observer<std::string>
{
public:
	using Father = Event::Observer<std::string>;
	virtual void async_update(const std::shared_ptr<std::string>& message)
	{
		printf("test1, update %s\n", message->c_str());
	}
};


class Test1
{
public:
	Test1() { event_bus_ = std::make_shared<Event::Event>(); }
	void regist(const std::shared_ptr<Observer1>& observer)
	{
		event_bus_->regist(std::dynamic_pointer_cast<Observer1::Father>(observer));
	}
	void test()
	{
		event_bus_->async_post(std::make_shared<std::string>("123"));
	}
	void unregist(const std::shared_ptr<Observer1>& observer)
	{
		event_bus_->unregist(std::dynamic_pointer_cast<Observer1::Father>(observer));
	}
private:
	std::shared_ptr<Event::Event> event_bus_;
};

int main()
{
	std::unique_ptr<std::mutex> up1;
	std::unique_ptr<std::mutex> up2;
	up1 = std::move(up2);
	std::shared_ptr<int> p1(new int);
	std::weak_ptr<int> p2(p1);
	std::weak_ptr<int> p3(p2);
	printf("%d", (p3.lock() == p2.lock()));
	std::shared_ptr<Observer1> ob1 = std::make_shared<Observer1>();
	std::shared_ptr<Observer2> ob2 = std::make_shared<Observer2>();
	Test1 t1;
	Test2 t2;
	t1.regist(ob1);
	t2.regist(ob2);
	t1.test();
	t2.test();
	t1.unregist(ob1);
	system("pause");
	return 0;
}
